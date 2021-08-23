#include "Controller.h"

Controller* Controller::INSTANCE = nullptr;

Controller::Controller() : mode(Mode::NONE), screenState(ScreenState::REFRESH), previousMode(Mode::PAINTING), textPosition(0, 0), username("None") {

}

Controller::~Controller() {
    delete INSTANCE;
}

// getters
Controller* Controller::getInstance() {
    if (INSTANCE == nullptr) {
        INSTANCE = new Controller();
    }
    return INSTANCE;
}

Mode Controller::getActionState() const {
    return mode;
}

Mode Controller::getPreviousActionState() const {
    return previousMode;
}

ScreenState Controller::getScreenState() const {
    return screenState;
}

InputManager* Controller::getInputManager() {
    return &inputManager;
}

glm::ivec2 Controller::getTextPosition() const {
    return textPosition;
}

glm::ivec2 Controller::getIndicatorPosition() const {
    return indicatorPosition;
}

std::string Controller::getText() const {
    return text;
}

std::string Controller::getUsername() const {
    return username;
}

bool Controller::isNone() const {
    return mode == Mode::NONE;
}

bool Controller::isPainting() const {
    return mode == Mode::PAINTING;
}

bool Controller::isEraseing() const {
    return mode == Mode::ERASING;
}

bool Controller::isWriting() const {
    return mode == Mode::TEXT;
}

// setters
void Controller::setMode(Mode actionState) {
    this->mode = actionState;
}

void Controller::updatePreviousMode() {
    this->previousMode = mode;
}

void Controller::setScreenState(ScreenState screenState) {
    this->screenState = screenState;
}

void Controller::setPrevoiusMode(Mode previousActionState) {
    this->previousMode = previousActionState;
}

void Controller::setTextPosition(glm::ivec2 textPosition) {
    this->textPosition = textPosition;
}

void Controller::setIndicatorPosition(glm::ivec2 indicatorPosition) {
    this->indicatorPosition = indicatorPosition;
}

void Controller::appendText(char* newText, int index) {
    text.insert(index, newText);
}

void Controller::setUsername(std::string name) {
    this->username = name;
}

void Controller::resetText() {
    text = "";
}

bool Controller::removeCharacter() {
    if (!text.empty()) {
        text.pop_back();
        return true;
    }
    return false;
}
