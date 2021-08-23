#include "GUIBase.h"
#include "Controller.h"

GUIBase::GUIBase() : position(0, 0), dimension(0, 0) {
	setInputManager();
}

GUIBase::GUIBase(glm::ivec2 position, glm::ivec2 dimension) : position(position), dimension(dimension) {
	setInputManager();
}

void GUIBase::setPosition(glm::ivec2 position) {
	this->position = position;
}

void GUIBase::setDimension(glm::ivec2 dimension) {
	this->dimension = dimension;
}

void GUIBase::setInputManager() {
	inputManager = Controller::getInstance()->getInputManager();
}

glm::ivec2 GUIBase::getPosition() const {
	return position;
}

glm::ivec2 GUIBase::getDimension() const {
	return dimension;
}