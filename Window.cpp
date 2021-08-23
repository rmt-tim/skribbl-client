#include "Window.h"
#include "Utils.h"
#include "Controller.h"

Window::Window() : window(nullptr), renderer(nullptr), inputManager(nullptr), visible(false) {

}

Window::~Window() {
	close();
}

void Window::init(std::string title, glm::ivec2 dimensions) {
	initWindow(title, dimensions);
	initRenderer();
	setInputManager(Controller::getInstance()->getInputManager());
	setVisible(true);
}

void Window::initWindow(std::string title, glm::ivec2 dimensions) {
	window = Utils::createWindow(title, dimensions);
}

void Window::initRenderer() {
	renderer = Utils::createRenderer(window);
}

void Window::close() {
	reset();
	setVisible(false);
}

void Window::reset() {
	resetWindow();
	resetRenderer();
}

void Window::resetWindow() {
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

void Window::resetRenderer() {
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
}

void Window::setVisible(bool visible) {
	this->visible = visible;
}

void Window::setInputManager(InputManager* inputManager) {
	this->inputManager = inputManager;
}

bool Window::isVisible() {
	return visible;
}
