#include "TextLine.h"
#include "Controller.h"
#include "Utils.h"
#include <iostream>

TextLine::TextLine(glm::ivec2 textPosition, int textSize, SDL_Renderer* renderer, Font* font) : textPosition(textPosition), textSize(textSize), font(font), cursorPosition(), deleteTimer(0), cursorTimer(0), showTime(0), enable(true), inputManager(nullptr), renderer(renderer) {
	init();
}

TextLine::~TextLine() {
	reset();
}

void TextLine::draw() {
	drawText();
	drawCursor();
}

void TextLine::update(Uint32 deltaTime) {
	if (isEnabled()) {
		updateText(deltaTime);
		updateCursor(deltaTime);
	}
}

void TextLine::clear() {
	text.clear();
}

void TextLine::setEnable(bool state) {
	this->enable = state;
}

bool TextLine::isEnabled() {
	return enable;
}

int TextLine::getSize() {
	return text.size();
}

std::string TextLine::getText() {
	return text;
}

void TextLine::init() {
	inputManager = Controller::getInstance()->getInputManager();
}

void TextLine::reset() {
	font = nullptr;
	inputManager = nullptr;
	renderer = nullptr;
}

void TextLine::updateText(Uint32 deltaTime) {
	if (inputManager->isAppend() && text.size() < textSize) {
		appendText();
		showCursor();
	}
	else if (inputManager->isKeyPressed(SDLK_BACKSPACE) && deleteTimer >= 1000 && !text.empty()) {
		text.erase(text.size() - 1);
		showCursor();
	}
	else if (inputManager->isKeyPressed(SDLK_BACKSPACE) && deleteTimer == 0 && !text.empty()) {
		text.erase(text.size() - 1);
		showCursor();
		deleteTimer += deltaTime;
	}
	else if (inputManager->isKeyPressed(SDLK_BACKSPACE) && deleteTimer < 1000) {
		deleteTimer += deltaTime;
	}
	else if (!inputManager->isKeyPressed(SDLK_BACKSPACE) && deleteTimer != 0) {
		deleteTimer = 0;
	}
}

void TextLine::updateCursor(Uint32 deltaTime) {
	updateCursorPosition();
	updateCursorVisibility(deltaTime);
}

void TextLine::updateCursorPosition() {
	if (!text.empty()) {
		glm::ivec2 textDimensions = font->getTextDimensions(text);
		cursorPosition = { textDimensions.x, textDimensions.y };
	}
	else {
		cursorPosition = { 0, font->getTextHeight() };
	}
}

void TextLine::updateCursorVisibility(Uint32 deltaTime) {
	if (cursorTimer >= 1000 && !isVisible()) {
		showCursor();
	}
	else if (cursorTimer >= 500 && isVisible()) {
		showTime -= 100;
		if (showTime <= 0) {
			setVisibleCursor(false);
		}
	}
	else {
		cursorTimer += deltaTime;
	}
}

void TextLine::appendText() {
	std::string newText = inputManager->getText();
	if (newText.size() + text.size() > textSize) {
		int difference = (newText.size() + text.size()) - textSize;
		newText.erase(newText.end() - difference);
	}
	text += newText;
}

void TextLine::drawText() {
	if (!text.empty()) {
		Utils::drawText(text, BLACK, renderer, font, textPosition);
	}
}

void TextLine::drawCursor() {
	if (isVisible()) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawLine(renderer, textPosition.x + cursorPosition.x, textPosition.y, textPosition.x + cursorPosition.x, textPosition.y + cursorPosition.y);
	}
}

void TextLine::showCursor() {
	cursorTimer = 0;
	showTime = 1000;
	setVisibleCursor(true);
}

void TextLine::setVisibleCursor(bool visible) {
	this->visible = visible;
}

bool TextLine::isVisible() {
	return visible;
}
