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

void Controller::startGame() {
    send_message({ {"type", "startGame"} });
}

void Controller::login(std::string username) {
    send_message({ {"type", "username"}, {"username", username} });
}

void Controller::setDrawer(std::string username) {
    drawer = this->username == username;
}

bool Controller::isDrawer() const {
    return drawer;
}

void Controller::setStart(bool start) {
    this->start = start;
}

bool Controller::hasStarted() const {
    return start;
}

void Controller::setWord(std::string word) {
    this->word = word;
}

std::string Controller::getWord() const {
    return word;
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
    login(name);
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
