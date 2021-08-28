#include "ChatPanel.h"
#include "Config.h"
#include "Color.h"
#include "Controller.h"
#include "Utils.h"
#include "source/client/Client.h"
#include <iostream>

ChatPanel::ChatPanel() : GUIBase(), offsetY(0), textLine(nullptr), font(nullptr), renderer(nullptr) {

}

ChatPanel::~ChatPanel() {

}

void ChatPanel::init(SDL_Renderer* renderer, Font* font) {
	setRenderer(renderer);
	setFont(font);
	setPosition(glm::ivec2(CHAT_PANEL_START_X, CHAT_PANEL_START_Y));
	setDimension(glm::ivec2(CHAT_PANEL_WIDTH, CHAT_PANEL_HEIGHT));
	initTextLine();
}

void ChatPanel::draw() {
	// draw panels
	drawPanels();

	// draw messages
	drawMessages();

	if (textLine != nullptr) {
		textLine->draw();
	}
}

void ChatPanel::update(Uint32 deltaTime) {
	updateTextLine(deltaTime);
	enterEvent();
}

void ChatPanel::drawPanels() {
	// draw background color
	Utils::drawRectangle(WHITE, renderer, glm::ivec4(CHAT_PANEL_START_X, CHAT_PANEL_START_Y, CHAT_PANEL_WIDTH, CHAT_PANEL_HEIGHT));

	// draw border
	Utils::drawRectangle(BLACK, renderer, glm::ivec4(CHAT_PANEL_START_X, CHAT_PANEL_START_Y, CHAT_PANEL_WIDTH, CHAT_PANEL_HEIGHT), false);

	// draw textPanel
	Utils::drawRectangle(BLACK, renderer, glm::ivec4(TEXT_PANEL_START_X, TEXT_PANEL_START_Y, TEXT_PANEL_WIDTH, TEXT_PANEL_HEIGHT), false);

	// draw inputPanel
	Utils::drawRectangle(BLACK, renderer, glm::ivec4(INPUT_PANEL_START_X, INPUT_PANEL_START_Y, INPUT_PANEL_WIDTH, INPUT_PANEL_HEIGHT), false);
}

void ChatPanel::drawMessages() {
	if (offsetY > TEXT_PANEL_START_Y + TEXT_PANEL_HEIGHT - TTF_FontHeight(font->getFont())) {
		messages.pop();
	}

	std::queue<std::string> temp = messages;
	int i = 0;

	while (!temp.empty()) {
		std::string text = temp.front();

		SDL_Texture* texture = nullptr;
		SDL_Rect bounds;

		offsetY = TEXT_PANEL_START_Y + i * TTF_FontHeight(font->getFont());

		font->obtainTextData(text, BLACK, renderer, &texture, &bounds, glm::ivec2(TEXT_PANEL_START_X + 3, offsetY));

		SDL_RenderCopy(renderer, texture, NULL, &bounds);
		SDL_DestroyTexture(texture);

		temp.pop();

		i++;
	}
}

void ChatPanel::initTextLine() {
	if (textLine == nullptr) {
		textLine = new TextLine(glm::ivec2(INPUT_PANEL_START_X + 2, INPUT_PANEL_START_Y + ((INPUT_PANEL_HEIGHT - font->getTextHeight()) / 2)), 15, renderer, font);
		textLine->setEnable(false);
	}
}

void ChatPanel::enterEvent() {
	if (textLine->isEnabled() && inputManager->isKeyPressed(SDLK_RETURN)) {
		std::string message = textLine->getText();

		if (start(message)) {
			Controller::getInstance()->startGame();
		}
		else if(Controller::getInstance()->hasStarted() && !Controller::getInstance()->isDrawer()) {
			Controller::getInstance()->sendMessage(message);
		}

		textLine->clear();
		inputManager->releaseKey(SDLK_RETURN);
	}
}

void ChatPanel::updateTextLine(Uint32 deltaTime) {
	if (textLine == nullptr || Controller::getInstance()->isDrawer()) {
		return;
	}

	glm::ivec2 mouseCoords = inputManager->getMouseCoordinates();

	if (!textLine->isEnabled() && Utils::isPointInsideBounds(mouseCoords, glm::ivec4(INPUT_PANEL_START_X, INPUT_PANEL_START_Y, INPUT_PANEL_WIDTH, INPUT_PANEL_HEIGHT)) && inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		textLine->setEnable(true);
	}
	else if (textLine->isEnabled() && Utils::isPointInsideBounds(mouseCoords, glm::ivec4(CHAT_PANEL_START_X, CHAT_PANEL_START_Y, CHAT_PANEL_WIDTH, CHAT_PANEL_HEIGHT))) {
		textLine->update(deltaTime);
	}
	else if (textLine->isEnabled() && !Utils::isPointInsideBounds(mouseCoords, glm::ivec4(CHAT_PANEL_START_X, CHAT_PANEL_START_Y, CHAT_PANEL_WIDTH, CHAT_PANEL_HEIGHT))) {
		textLine->setEnable(false);
		textLine->setVisibleCursor(false);
	}
}

bool ChatPanel::start(std::string message) {
	return message == "/start";
}

void ChatPanel::setRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

void ChatPanel::setFont(Font* font) {
	this->font = font;
}

void ChatPanel::addMessage(std::string message) {
	if (!message.empty()) {
		messages.push(message);
	}
}


