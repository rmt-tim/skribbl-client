#include "TextPanel.h"
#include "Utils.h"
#include "Config.h"
#include "source/client/Client.h"
#include <iostream>

TextPanel::TextPanel() : Window(), font(nullptr), button(), buttonColor(YELLOW_GREEN), pressed(false), textLine(nullptr) {

}

TextPanel::~TextPanel() {
	closeWindow();
}

void TextPanel::init(Font* font) {
	Window::init(TEXTING_PANEL, glm::ivec2(TEXTING_PANEL_WIDTH, TEXTING_PANEL_HEIGHT));
	setFont(font);
	initComponents();
}

void TextPanel::initComponents() {
	button = { 150, 200, 200, 50 };

	if (textLine == nullptr) {
		textLine = new TextLine(glm::ivec2(TEXTING_PANEL_WIDTH / 2 - 75, TEXTING_PANEL_HEIGHT / 2 - font->getTextHeight()), 7, renderer, font);
	}
}

void TextPanel::draw() {
	// draw panel background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, NULL);

	// draw text background
	Utils::drawRectangle(CITRON, renderer, glm::ivec4(TEXTING_PANEL_WIDTH / 2 - 75, TEXTING_PANEL_HEIGHT / 2 - font->getTextHeight(), 150, font->getTextHeight()));

	// draw submit button
	Utils::drawButton(buttonColor, "Submit", BLACK, renderer, font, button);

	// draw text 
	Utils::drawText("Unesite vase ime", BLACK, renderer, font, glm::ivec2(150, 50), TEXTING_PANEL_WIDTH);

	// drawText area
	if (textLine != nullptr) {
		textLine->draw();
	}

	SDL_RenderPresent(renderer);
}

void TextPanel::update() {
	if (isPressed() && !inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		Controller::getInstance()->setUsername(textLine->getText());
		closeWindow();
	}
	else if (inputManager->isKeyPressed(SDL_BUTTON_LEFT) && Utils::isPointInsideBounds(inputManager->getMouseCoordinates(), button)) {
		setButtonColor(RED);
		setPressed(true);
	}

	if (textLine != nullptr) {
		textLine->update(DESIRED_FRAME_TIME);
	}
}

void TextPanel::setFont(Font* font) {
	this->font = font;
}

void TextPanel::closeWindow() {
	close();
	resetTextLine();
	setPressed(false);
	setButtonColor(YELLOW_GREEN);
}

void TextPanel::setPressed(bool pressed) {
	this->pressed = pressed;
}

void TextPanel::setButtonColor(Color color) {
	this->buttonColor = color;
}

void TextPanel::resetTextLine() {
	delete textLine;
	textLine = nullptr;
}

bool TextPanel::isPressed() {
	return pressed;
}
