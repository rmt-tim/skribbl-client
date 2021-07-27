#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

using u16 = std::uint16_t;

inline std::string wsa_error_message(const char* calling_function)
{
    DWORD error_code = WSAGetLastError();
    LPVOID message_buffer;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&message_buffer,
        0,
        NULL
    );
    std::string error_message;
    error_message.append(calling_function);
    error_message.append("() failed with error code ");
    error_message.append(std::to_string(error_code));
    error_message.append(": ");
    error_message.append((const char*)message_buffer);
    return error_message;
}

inline void throw_wsa_error(const char* calling_function)
{
    throw std::runtime_error{wsa_error_message(calling_function)};
}

struct winsock_library {
    ~winsock_library()
    {
        WSACleanup();
    }

    winsock_library()
    {
        ::WSAData wsa_data;
        if (::WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            /* throw std::runtime_error{"WSAStartup failed.\n"}; */
            throw_wsa_error("WSAStartup");
        }
    }
};

enum class protocol { tcp, udp };
enum class host_type { client, server };

inline SOCKET create_socket(const char* nodename, u16 port, protocol proto, host_type host)
{
    ::addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == protocol::tcp ? SOCK_STREAM : SOCK_DGRAM;
    if (host == host_type::server) hints.ai_flags = AI_PASSIVE;

    ::addrinfo* addresses = nullptr;
    int status = ::getaddrinfo(nodename, std::to_string(port).c_str(), &hints, &addresses);
    if (status != 0) throw std::runtime_error{::gai_strerrorA(status)};

    unsigned sockfd = INVALID_SOCKET;
    ::addrinfo* p = nullptr;
    for (p = addresses; p != nullptr; p = p->ai_next) {
        sockfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == INVALID_SOCKET) continue;

        int yes = 1;
        if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1) {
            ::freeaddrinfo(addresses);
            std::perror("setsockopt():");
            throw_wsa_error("setsockopt");
            /* throw std::runtime_error{"setsockopt failed."}; */
        }

        if (host == host_type::server) {
            if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                ::closesocket(sockfd);
                continue;
            }
        } else if (proto == protocol::tcp) {
            if (::connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                ::closesocket(sockfd);
                continue;
            }
        }

        break;
    }

    ::freeaddrinfo(addresses);

    if (p == nullptr) throw std::runtime_error{"Server failed to bind."};

    if (host == host_type::server && proto != protocol::udp) {
        if (::listen(sockfd, 16)) {
            std::perror("listen():");
            /* throw std::runtime_error{"listen failed."}; */
            throw_wsa_error("listen");
        }
    }

    return sockfd;
}

class tcp_socket {
    SOCKET sockfd = INVALID_SOCKET;

public:
    ~tcp_socket()
    {
        ::closesocket(sockfd);
    }

    tcp_socket(tcp_socket&& other)
        : sockfd{other.sockfd}
    {
        other.sockfd = INVALID_SOCKET;
    }

    tcp_socket& operator=(tcp_socket&& other)
    {
        if (sockfd != INVALID_SOCKET) ::closesocket(sockfd);
        sockfd = other.sockfd;
        other.sockfd = INVALID_SOCKET;
        return *this;
    }

    tcp_socket(SOCKET fd)
        : sockfd{fd}
    {}

    tcp_socket(const char* nodename, u16 port)
        : sockfd{create_socket(nodename, port, protocol::tcp, host_type::client)}
    {}

    void send(std::string data)
    {
        int bytes_sent = 0;
        int bytes_to_send = data.length();
        int n;
        while (bytes_sent < data.length()) {
            n = ::send(sockfd, data.data() + bytes_sent, bytes_to_send - bytes_sent, 0);
            if (n == -1) break;
            bytes_sent += n;
        }
        /* if (n == -1) throw std::runtime_error{"Not all data was sent."}; */
        if (n == -1) throw_wsa_error("send");
    }

    std::string receive()
    {
        std::string data;
        char buffer[4096];
        int bytes_received = 0;
        bytes_received = ::recv(sockfd, buffer, 4096, 0);
        if (bytes_received == -1) throw_wsa_error("recv");
        data.append(buffer, bytes_received);
        return data;
    }
};

class tcp_server_socket {
    SOCKET sockfd = INVALID_SOCKET;

public:
    ~tcp_server_socket()
    {
        ::closesocket(sockfd);
    }

    tcp_server_socket(tcp_server_socket&& other)
        : sockfd{other.sockfd}
    {
        other.sockfd = INVALID_SOCKET;
    }

    tcp_server_socket& operator=(tcp_server_socket&& other)
    {
        ::closesocket(sockfd);
        sockfd = other.sockfd;
        other.sockfd = INVALID_SOCKET;
        return *this;
    }

    tcp_server_socket(u16 port)
        : sockfd{create_socket(nullptr, port, protocol::tcp, host_type::server)}
    {}

    tcp_socket accept()
    {
        assert(sockfd != INVALID_SOCKET);
        ::sockaddr_storage their_addr;
        ::socklen_t addr_size = sizeof their_addr;
        SOCKET newfd = ::accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
        if (newfd == INVALID_SOCKET) throw_wsa_error("accept");
        return tcp_socket{newfd};
    }
};
