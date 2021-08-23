#include "MessagePanel.h"
#include "Config.h"
#include "Utils.h"
#include <iostream>

MessagePanel::MessagePanel() : Window(), font(nullptr), button(), buttonColor(GRAY), isPressed(false) {

}

MessagePanel::~MessagePanel() {
	closeWindow();
}

void MessagePanel::init(std::string message, Font* font) {
	Window::init("Notification!", glm::ivec2(MESSAGE_PANEL_WIDTH, MESSAGE_PANEL_HEIGHT));
	initComponents();
	setMessage(message);
	setFont(font);
}

void MessagePanel::initComponents() {
	button = { 200, 200, 100, 50 };
}

void MessagePanel::setMessage(std::string message) {
	this->message = message;
}

void MessagePanel::setFont(Font* font) {
	this->font = font;
}

void MessagePanel::draw() {
	// draw background color
	Color c = WHITE;
	SDL_SetRenderDrawColor(renderer, c.getR(), c.getG(), c.getB(), c.getA());
	SDL_RenderFillRect(renderer, NULL);

	// draw the message
	Utils::drawText(message, BLACK, renderer, font, glm::ivec2(0, MESSAGE_PANEL_HEIGHT / 4), MESSAGE_PANEL_WIDTH);

	// draw button
	Utils::drawButton(buttonColor, "OK", BLACK, renderer, font, button);

	// refresh the screen
	SDL_RenderPresent(renderer);
}

void MessagePanel::update() {
	if (isPressed && !inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		closeWindow();
	}
	else if (inputManager->isKeyPressed(SDL_BUTTON_LEFT) && Utils::isPointInsideBounds(inputManager->getMouseCoordinates(), button)) {
		setButtonColor(YELLOW_GREEN);
		setIsPressed(true);
	}
}

void MessagePanel::setIsPressed(bool isPressed) {
	this->isPressed = isPressed;
}

void MessagePanel::setButtonColor(Color color) {
	this->buttonColor = color;
}

void MessagePanel::closeWindow() {
	Window::close();
	setIsPressed(false);
	setButtonColor(GRAY);
}

SDL_Window* MessagePanel::getWindow() const {
	return window;
}
