#include "InputManager.h"

// setters
void InputManager::pressKey(unsigned int keyID) {
	keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	keyMap[keyID] = false;
}

void InputManager::setMouseCoordinates(int x, int y) {
	mouseCoordinates = glm::ivec2(x, y);
}

void InputManager::setMoving(bool moving) {
	this->moving = moving;
}

void InputManager::setAppend(bool append) {
	this->append = append;
}

void InputManager::setClickNumber(int clickNumber) {
	this->clickNumber = clickNumber;
}

void InputManager::setWindowID(unsigned int windowID) {
	this->windowID = windowID;
}

void InputManager::setTextInput(char* newText, bool append) {
	setAppend(append);
	setText(newText);
}

void InputManager::setText(char* newText) {
	if (newText == nullptr) {
		text.clear();
	}
	else {
		text.append(newText);
	}
}

// getters
glm::ivec2 InputManager::getMouseCoordinates() const {
	return mouseCoordinates;
}

bool InputManager::isKeyPressed(unsigned int keyID) {
	auto iterator = keyMap.find(keyID);

	if (iterator != keyMap.end()) {
		return iterator->second;
	}

	return false;
}

bool InputManager::isDoubleClick() const {
	return clickNumber == 2;
}

unsigned int InputManager::getWindowID() const {
	return windowID;
}

std::string InputManager::getText() const {
	return text;
}

bool InputManager::isMoving() const {
	return moving;
}

bool InputManager::isAppend() const {
	return append;
}
