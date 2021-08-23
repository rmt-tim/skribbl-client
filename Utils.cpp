#include "Utils.h"
#include "ImageLoader.h"
#include "Config.h"
#include <iostream>
#include <queue>

SDL_Rect Utils::screenBounds = { 0, MAIN_PANEL_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - MAIN_PANEL_HEIGHT };

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 Utils::rmask = 0xff000000;
Uint32 Utils::gmask = 0x00ff0000;
Uint32 Utils::bmask = 0x0000ff00;
Uint32 Utils::amask = 0x000000ff;
#else
Uint32 Utils::rmask = 0x000000ff;
Uint32 Utils::gmask = 0x0000ff00;
Uint32 Utils::bmask = 0x00ff0000;
Uint32 Utils::amask = 0xff000000;
#endif

bool Utils::squareCollision(SDL_Rect r1, SDL_Rect r2) {
    return areColliding(r1, r2) || areColliding(r2, r1);
}

bool Utils::areColliding(SDL_Rect r1, SDL_Rect r2) {
    if (r1.x + r1.w < r2.x) {
        return false;
    }
    else if (r2.x + r2.w < r1.x) {
        return false;
    }
    else if (r1.y < r2.y) {
        return false;
    }
    else if (r2.y + r2.h < r1.y) {
        return false;
    }

    return true;
}

bool Utils::isPointInsideBounds(glm::ivec2 mouseCoords, SDL_Rect bounds) {
    if (mouseCoords.x >= bounds.x && mouseCoords.x <= bounds.x + bounds.w) {
        if (mouseCoords.y >= bounds.y && mouseCoords.y <= bounds.y + bounds.h) {
            return true;
        }
    }
    return false;
}

bool Utils::isPointInsideBounds(glm::ivec2 mouseCoords, glm::ivec4 bounds) {
    if (mouseCoords.x >= bounds.x && mouseCoords.x <= bounds.x + bounds.z) {
        if (mouseCoords.y >= bounds.y && mouseCoords.y <= bounds.y + bounds.w) {
            return true;
        }
    }
    return false;
}

int Utils::calculateRGBValueFromPositon(int x, int A, int B, int C) {
    return ((x - A) / (float)B) * C;
}

int Utils::calculatePositionFromRGBValue(int A, int B, int C, int D) {
    return (D * B / (float)C) + A;
}

void Utils::paintWithBucket(glm::ivec2 seedCoords, Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, screenBounds.w, screenBounds.h, 32, rmask, gmask, bmask, amask);
    getPixels(renderer, surface);

    Pixel seedPixel = getPixel(seedCoords, surface);
    Color seedColor = seedPixel.getColor();

    std::queue<Pixel> pixels;
    pixels.push(seedPixel);

    int i = 0;
    while (i != 3600) {
        Pixel currentPixel = pixels.front();

        pixels.pop();

        getNeighbours(currentPixel, seedColor, surface, pixels);

        currentPixel.setColor(color);
        paintPixel(currentPixel, renderer);
        paintSurfacePixel(currentPixel, surface);

        i++;
    }

    SDL_FreeSurface(surface);
}

void Utils::getNeighbours(Pixel pixel, Color color, SDL_Surface* surface, std::queue<Pixel>& pixels) {
    glm::ivec2 pixelCoords = pixel.getPosition();

    // pixel on the left
    glm::ivec2 position = glm::ivec2(pixelCoords.x - 1, pixelCoords.y);
    if (isPointInsideBounds(position, screenBounds)) {
        pixel = getPixel(position, surface);
        if (pixel.getColor() == color) {
            pixels.push(getPixel(position, surface));
        }
    }

    // pixel on the rigth
    position = glm::ivec2(pixelCoords.x + 1, pixelCoords.y);
    if (isPointInsideBounds(position, screenBounds)) {
        pixel = getPixel(position, surface);
        if (pixel.getColor() == color) {
            pixels.push(getPixel(position, surface));
        }
    }

    // pixel on the top
    position = glm::ivec2(pixelCoords.x, pixelCoords.y - 1);
    if (isPointInsideBounds(position, screenBounds)) {
        pixel = getPixel(position, surface);
        if (pixel.getColor() == color) {
            pixels.push(getPixel(position, surface));
        }
    }

    // pixel on the bottom
    position = glm::ivec2(pixelCoords.x, pixelCoords.y + 1);
    if (isPointInsideBounds(position, screenBounds)) {
        pixel = getPixel(position, surface);
        if (pixel.getColor() == color) {
            pixels.push(getPixel(position, surface));
        }
    }
}

SDL_Cursor* Utils::getCursor(Mode state) {
    switch (state)
    {
    case Mode::NONE: {
        return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    }
    case Mode::PAINTING: {
        return SDL_CreateColorCursor(ImageLoader::loadCursorSurface(PENCIL_CURSOR_PATH), PENCIL_CURSOR_HOT_X, PENCIL_CURSOR_HOT_Y);
    }
    case Mode::ERASING: {
        return SDL_CreateColorCursor(ImageLoader::loadCursorSurface(RUBBER_CURSOR_PATH), RUBBER_CURSOR_HOT_X, RUBBER_CURSOR_HOT_Y);
    }
    case Mode::TEXT: {
        return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    }
    default:
        return SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    }
}

std::vector<SDL_Rect> Utils::getLinePath(glm::ivec2 p1, glm::ivec2 p2, int pathWidth, int pathHeight) {
    std::vector<SDL_Rect> path;
    std::vector<glm::ivec2> points;

    /*int dx = std::abs(p2.x - p1.x);
    int dy = std::abs(p2.y - p1.y);
    int N = std::max(dx, dy);

    float divN = ((N == 0) ? 0.0f : (1.0f / N));
    float xStep = dx * divN;
    float yStep = dy * divN;

    float x = p1.x;
    float y = p1.y;

    path.reserve(N + 1);
    points.reserve(N + 1);

    for (int step = 0; step <= N; step++, x += xStep, y += yStep) {
        points.push_back(glm::ivec2(std::round(x), std::round(y)));
    }*/

    glm::fvec2 start = p1;
    glm::fvec2 end = p2;

    float N = glm::distance(start, end);

    path.reserve(N + 1);
    path.reserve(N + 1);

    for (int step = 0; step <= N; step++) {
        float t = ((N == 0) ? 0.0f : (step / N));
        points.push_back(glm::round(lerp(start, end, t)));
    }

    for (size_t i = 0; i < points.size(); i++) {
        SDL_Rect rect;
        glm::ivec2 point = points[i];

        rect.w = pathWidth;
        rect.h = pathHeight;

        rect.x = point.x - pathWidth / 2;
        rect.y = point.y - pathHeight / 2;

        path.push_back(rect);
    }

    return path;
}

Color Utils::getButtonColor(int index) {
    switch (index)
    {
    case 0:
        return RED;
    case 1:
        return GREEN;
    case 2:
        return BLUE;
    case 3:
        return YELLOW;
    case 4:
        return WHITE;
    case 5:
        return BLACK;
    case 6:
        return BYZANTINE;
    case 7:
        return SEA_GREEN;
    case 8:
        return TRUE_BLUE;
    case 9:
        return SKY_MAGENTA;
    case 10:
        return LEMON_GLACIER;
    case 11:
        return CITRON;
    case 12:
        return MARIGOLD;
    case 13:
        return BRICK_RED;
    case 14:
        return DARK_CORNFLOWER_BLUE;
    case 15:
        return LIME_GREEN;
    case 16:
        return FOREST_GREEN_WEB;
    case 17:
        return ZOMP;
    case 18:
        return LIGHT_STEEL_BLUE;
    case 19:
        return YELLOW_GREEN;
    default:
        return BLACK;
    }
    return Color();
}

glm::fvec2 Utils::lerp(glm::fvec2 p1, glm::fvec2 p2, float t) {
    return p1 * (1 - t) + p2 * t;
}

void Utils::getPixels(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Rect rect = { screenBounds.x, screenBounds.y, screenBounds.w, screenBounds.h };
    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);
}

Pixel Utils::getPixel(glm::ivec2 position, SDL_Surface* surface) {
    Uint32* pixel = (Uint32*)surface->pixels + (position.y - MAIN_PANEL_HEIGHT) * surface->pitch / 4 + position.x;

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;

    SDL_GetRGBA(*pixel, surface->format, &r, &g, &b, &a);

    return Pixel(Color(r, g, b, a), position);
}

void Utils::paintPixel(Pixel pixel, SDL_Renderer* renderer) {
    Color color = pixel.getColor();
    glm::ivec2 pixelCoords = pixel.getPosition();
    SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
    SDL_RenderDrawPoint(renderer, pixelCoords.x, pixelCoords.y);
}

void Utils::paintPixel(Pixel pixel, Color color, SDL_Renderer* renderer) {
    glm::ivec2 pixelCoords = pixel.getPosition();
    SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
    SDL_RenderDrawPoint(renderer, pixelCoords.x, pixelCoords.y);
}

void Utils::paintSurfacePixel(Pixel pixel, SDL_Surface* surface) {
    glm::ivec2 position = pixel.getPosition();
    Color color = pixel.getColor();

    Uint32* pix = (Uint32*)surface->pixels + (position.y - MAIN_PANEL_HEIGHT) * surface->pitch / 4 + position.x;
    *pix = SDL_MapRGBA(surface->format, color.getR(), color.getG(), color.getB(), color.getA());

    pixel = getPixel(pixel.getPosition(), surface);
}

void Utils::setUtilityButton(int index, SDL_Renderer* renderer, SDL_Texture** texture, int* ID) {
    switch (index)
    {
    case 0: {
        *texture = ImageLoader::loadTexture(PENCIL_BUTTON_PATH, renderer);
        *ID = PENCIL_BUTTON_ID;
        break;
    }
    case 1: {
        *texture = ImageLoader::loadTexture(RUBBER_BUTTON_PATH, renderer);
        *ID = RUBBER_BUTTON_ID;
        break;
    }
    default:
        *texture = ImageLoader::loadTexture(PENCIL_BUTTON_PATH, renderer);
        *ID = PENCIL_BUTTON_ID;
        break;
    }
}

void Utils::setActionState(int ID) {
    switch (ID)
    {
    case PENCIL_BUTTON_ID: {
        Controller::getInstance()->setMode(Mode::PAINTING);
        break;
    }
    case RUBBER_BUTTON_ID: {
        Controller::getInstance()->setMode(Mode::ERASING);
        break;
    }
    case TEXT_BUTTON_ID: {
        Controller::getInstance()->setMode(Mode::TEXT);
        break;
    }
    default:
        Controller::getInstance()->setMode(Mode::PAINTING);
        break;
    }
}

void Utils::drawText(std::string text, Color color, SDL_Renderer* renderer, Font* font, glm::ivec2 position) {
    SDL_Rect bounds;
    SDL_Texture* texture = nullptr;
    font->obtainTextData(text, color, renderer, &texture, &bounds, position);
    SDL_RenderCopy(renderer, texture, NULL, &bounds);
    SDL_DestroyTexture(texture);
}

void Utils::drawText(std::string text, Color color, SDL_Renderer* renderer, Font* font, glm::ivec2 position, int width) {
    SDL_Rect bounds;
    SDL_Texture* texture = nullptr;
    font->obtainTextData(text, color, renderer, &texture, &bounds, position);

    bounds.x = width / 2 - bounds.w / 2; // center the text horizontally on the panel
    bounds.y = bounds.y - bounds.h / 2;

    SDL_RenderCopy(renderer, texture, NULL, &bounds);
    SDL_DestroyTexture(texture);
}

void Utils::drawRectangle(Color color, SDL_Renderer* renderer, glm::ivec4 posisiton, bool fill) {
    SDL_Rect bounds = { posisiton.x, posisiton.y, posisiton.z, posisiton.w };
    SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
    if (fill) {
        SDL_RenderFillRect(renderer, &bounds);
    }
    else {
        SDL_RenderDrawRect(renderer, &bounds);
    }
}

void Utils::drawButton(Color buttonColor, std::string text, Color textColor, SDL_Renderer* renderer, Font* font, glm::ivec4 posisiton) {
    drawRectangle(buttonColor, renderer, posisiton);
    drawCenteredText(text, textColor, renderer, font, posisiton);
}

void Utils::drawCenteredText(std::string text, Color color, SDL_Renderer* renderer, Font* font, glm::ivec4 dimensions) {
    SDL_Rect bounds;
    SDL_Texture* texture = nullptr;
    font->obtainTextData(text, color, renderer, &texture, &bounds, glm::ivec2(0, 0));

    bounds.x = dimensions.x + dimensions.z / 2 - bounds.w / 2; // center the text horizontally on the panel
    bounds.y = dimensions.y + dimensions.w / 2 - bounds.h / 2; // center the text vertically on the panel

    SDL_RenderCopy(renderer, texture, NULL, &bounds);
    SDL_DestroyTexture(texture);
}

SDL_Window* Utils::createWindow(std::string title, glm::ivec2 dimensions) {
    SDL_Window* window = nullptr;

    // create window
    if (window == nullptr) {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dimensions.x, dimensions.y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
    }

    if (window == nullptr) {
        std::cout << "Message panel creation failed." << std::endl;
        return nullptr;;
    }

    return window;
}

SDL_Renderer* Utils::createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = nullptr;

    // create renderer
    if (renderer == nullptr) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    }

    if (renderer == nullptr) {
        std::cout << "Message panel renderer failed to be created." << std::endl;
        return nullptr;
    }

    return renderer;
}

SDL_Texture* Utils::makeScreenshot(glm::ivec4 dimensions, SDL_Renderer* renderer) {
    SDL_Texture* screenshot = nullptr;
    SDL_Surface* sshot = SDL_CreateRGBSurface(0, dimensions.z + 10, dimensions.w + 10, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    SDL_Rect rect = { dimensions.x, dimensions.y, dimensions.z + 10, dimensions.w + 10 };

    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

    if ((screenshot = SDL_CreateTextureFromSurface(renderer, sshot)) == nullptr) {
        std::cout << "Screenshot creation failed." << std::endl;
    }

    SDL_FreeSurface(sshot);

    return screenshot;
}

void Utils::drawScreenshot(glm::ivec4 dimensions, SDL_Renderer* renderer, SDL_Texture* screenshot) {
    SDL_Rect dest = { dimensions.x, dimensions.y, dimensions.z + 10, dimensions.w + 10 };
    SDL_RenderCopy(renderer, screenshot, NULL, &dest);
}
