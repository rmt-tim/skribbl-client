#include "ColorPicker.h"
#include "Config.h"
#include "Utils.h"
#include "ImageLoader.h"
#include <glm/glm.hpp>
#include <iostream>

ColorPicker::ColorPicker() : window(nullptr), inputManager(nullptr), renderer(nullptr), visible(false), clickedRed(false), clickedGreen(false), clickedBlue(false), clickedSubmit(false), hasReset(true), redLabel("0"), greenLabel("0"), blueLabel("0") {

}

ColorPicker::~ColorPicker() {
	resetClass();
}

void ColorPicker::init(Color color) {
	initWindow();
	initComponents(color);
	draw();
	setVisible(true);
}

void ColorPicker::initWindow() {
	// create window
	if (window == nullptr) {
		window = SDL_CreateWindow(COLOR_PICKER.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, COLOR_PICKER_WIDTH, COLOR_PICKER_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
	}

	if (window == nullptr) {
		std::cout << "Color picker creation failed." << std::endl;
		return;
	}

	// create renderer
	if (renderer == nullptr) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	}

	if (renderer == nullptr) {
		std::cout << "Color picker renderer failed to be created." << std::endl;
		return;
	}

	if (inputManager == nullptr) {
		inputManager = Controller::getInstance()->getInputManager();
	}
}

void ColorPicker::initComponents(Color color) {
	// init colorPanel bounds
	SDL_Rect colorPanelBounds;

	colorPanelBounds.w = COLOR_PICKER_COLOR_PANEL_WIDTH;
	colorPanelBounds.h = COLOR_PICKER_COLOR_PANEL_HEIGHT;

	colorPanelBounds.x = COLOR_PICKER_COLOR_PANEL_START_X;
	colorPanelBounds.y = COLOR_PICKER_COLOR_PANEL_START_Y;

	colorPanel.init(colorPanelBounds, color);

	// init red toggleButton
	SDL_Rect toggleBounds;

	toggleBounds.w = TOGGLE_BUTTON_WIDTH;
	toggleBounds.h = TOGGLE_BUTTON_HEIGHT;

	toggleBounds.x = Utils::calculatePositionFromRGBValue(COLOR_INTERVAL_START_X, COLOR_INTERVAL_WIDTH, COLOR_RGB_MAX, color.getR());
	toggleBounds.y = RED_TOGGLE_BUTTON_START_Y - toggleBounds.h / 2;

	toggleRed.init(toggleBounds, BLACK);

	// init green toggleButton
	toggleBounds.x = toggleBounds.x = Utils::calculatePositionFromRGBValue(COLOR_INTERVAL_START_X, COLOR_INTERVAL_WIDTH, COLOR_RGB_MAX, color.getG());
	toggleBounds.y = GREEN_TOGGLE_BUTTON_START_Y - toggleBounds.h / 2;

	toggleGreen.init(toggleBounds, BLACK);

	// init blue toogleButton
	toggleBounds.x = toggleBounds.x = Utils::calculatePositionFromRGBValue(COLOR_INTERVAL_START_X, COLOR_INTERVAL_WIDTH, COLOR_RGB_MAX, color.getB());
	toggleBounds.y = BLUE_TOGGLE_BUTTON_START_Y - toggleBounds.h / 2;

	toggleBlue.init(toggleBounds, BLACK);

	// init submit button
	SDL_Rect submitBounds;

	submitBounds.w = SUBMIT_BUTTON_WIDTH;
	submitBounds.h = SUBMIT_BUTTON_HEIGHT;

	submitBounds.x = SUBMIT_BUTTON_START_X;
	submitBounds.y = SUBMIT_BUTTON_START_Y;

	submitButton.init(submitBounds, SILVER);

	// init factors
	redLabel = std::to_string(color.getR());
	greenLabel = std::to_string(color.getG());
	blueLabel = std::to_string(color.getB());
	setColor(color);
}

// update
void ColorPicker::update() {
	if (inputManager->getWindowID() != SDL_GetWindowID(window)) {
		return;
	}

	if (!inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		reset();
		return;
	}

	glm::ivec2 mouseCoords = inputManager->getMouseCoordinates();

	// check if user is scrolling left or right
	if (clickedRed == true) {
		updateToggleButton(mouseCoords, &toggleRed);
		updateColor(mouseCoords, Factor::R);
		updateColorPanel();
		draw();
		return;
	}
	else if (clickedGreen == true) {
		updateToggleButton(mouseCoords, &toggleGreen);
		updateColor(mouseCoords, Factor::G);
		updateColorPanel();
		draw();
		return;
	}
	else if (clickedBlue == true) {
		updateToggleButton(mouseCoords, &toggleBlue);
		updateColor(mouseCoords, Factor::B);
		updateColorPanel();
		draw();
		return;
	}
	else if (clickedSubmit == true) {
		submitButton.setColor(GRAY);
		draw();
		return;
	}

	// update toggleRed
	if (Utils::isPointInsideBounds(mouseCoords, toggleRed.getBounds())) {
		clickedRed = true;
		hasReset = false;
		return;
	}

	// update toggleGreen
	if (Utils::isPointInsideBounds(mouseCoords, toggleGreen.getBounds())) {
		clickedGreen = true;
		hasReset = false;
		return;
	}

	// update toggleBlue
	if (Utils::isPointInsideBounds(mouseCoords, toggleBlue.getBounds())) {
		clickedBlue = true;
		hasReset = false;
		return;
	}

	if (Utils::isPointInsideBounds(mouseCoords, submitButton.getBounds())) {
		clickedSubmit = true;
		hasReset = false;
		return;
	}

}

void ColorPicker::updateToggleButton(glm::ivec2 mouseCoords, Component* toggleButton) {
	SDL_Rect bounds = toggleButton->getBounds();
	bounds.x = glm::clamp(mouseCoords.x - bounds.w / 2, COLOR_INTERVAL_START_X, COLOR_INTERVAL_END_X);
	toggleButton->setBounds(bounds);
}

void ColorPicker::updateColor(glm::ivec2 mouseCoords, Factor factor) {
	mouseCoords.x = glm::clamp(mouseCoords.x, COLOR_INTERVAL_START_X, COLOR_INTERVAL_END_X);
	switch (factor)
	{
	case Factor::R: {
		int value = Utils::calculateRGBValueFromPositon(mouseCoords.x, COLOR_INTERVAL_START_X, COLOR_INTERVAL_WIDTH, COLOR_RGB_MAX);
		color.setR(value);
		redLabel = std::to_string(value);
		break;
	}
	case Factor::G: {
		int value = Utils::calculateRGBValueFromPositon(mouseCoords.x, COLOR_INTERVAL_START_X, COLOR_INTERVAL_WIDTH, COLOR_RGB_MAX);
		color.setG(value);
		greenLabel = std::to_string(value);
		break;
	}
	case Factor::B: {
		int value = Utils::calculateRGBValueFromPositon(mouseCoords.x, COLOR_INTERVAL_START_X, COLOR_INTERVAL_WIDTH, COLOR_RGB_MAX);
		color.setB(value);
		blueLabel = std::to_string(value);
		break;
	}
	default:
		break;
	}
}

void ColorPicker::updateColorPanel() {
	colorPanel.setColor(color);
}

void ColorPicker::draw() {
	// draw background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, NULL);

	// draw colorPanel border
	SDL_Rect bounds = colorPanel.getBounds();
	Color color = BLACK;

	bounds.x = bounds.x - 2;
	bounds.y = bounds.y - 2;

	bounds.w = bounds.w + 4;
	bounds.h = bounds.h + 4;

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderFillRect(renderer, &bounds);

	// draw colorPanel
	bounds = colorPanel.getBounds();
	color = colorPanel.getColor();

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderFillRect(renderer, &bounds);

	// draw red interval
	color = RED;

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderDrawLine(renderer, COLOR_INTERVAL_START_X, COLOR_INTERVAL_START_Y, COLOR_INTERVAL_END_X, COLOR_INTERVAL_END_Y);

	// draw green interval
	color = GREEN;

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderDrawLine(renderer, COLOR_INTERVAL_START_X, COLOR_INTERVAL_START_Y + COLOR_INTERVAL_HORIZONTAL_OFFSET, COLOR_INTERVAL_END_X, COLOR_INTERVAL_END_Y + COLOR_INTERVAL_HORIZONTAL_OFFSET);

	// draw blue interval
	color = BLUE;

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderDrawLine(renderer, COLOR_INTERVAL_START_X, COLOR_INTERVAL_START_Y + 2 * COLOR_INTERVAL_HORIZONTAL_OFFSET, COLOR_INTERVAL_END_X, COLOR_INTERVAL_END_Y + 2 * COLOR_INTERVAL_HORIZONTAL_OFFSET);

	// draw red toggleButton
	bounds = toggleRed.getBounds();
	color = toggleRed.getColor();

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderFillRect(renderer, &bounds);

	// draw green toggleButton
	bounds = toggleGreen.getBounds();
	color = toggleGreen.getColor();

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderFillRect(renderer, &bounds);

	// draw blue toggleButton
	bounds = toggleBlue.getBounds();
	color = toggleBlue.getColor();

	SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderFillRect(renderer, &bounds);

	// draw red label
	Utils::drawText(redLabel, BLACK, renderer, &font, glm::ivec2(LABEL_START_X, LABEL_START_Y), COLOR_PICKER_WIDTH);

	// draw green label
	Utils::drawText(greenLabel, BLACK, renderer, &font, glm::ivec2(LABEL_START_X, LABEL_START_Y + LABEL_OFFSET), COLOR_PICKER_WIDTH);

	// draw blue label
	Utils::drawText(blueLabel, BLACK, renderer, &font, glm::ivec2(LABEL_START_X, LABEL_START_Y + 2 * LABEL_OFFSET), COLOR_PICKER_WIDTH);

	// draw button border
	bounds = submitButton.getBounds();

	Utils::drawButton(submitButton.getColor(), SUBMIT_BUTTON, BLACK, renderer, &font, glm::ivec4(bounds.x, bounds.y, bounds.w, bounds.h));

	// update screen
	SDL_RenderPresent(renderer);
}

void ColorPicker::setFont(Font font) {
	this->font = font;
}

void ColorPicker::closeWindow() {
	resetClass();
	reset();
	setColor(BLACK);
	submitButton.setColor(SILVER);
	setVisible(false);

	redLabel = "0";
	greenLabel = "0";
	blueLabel = "0";
}

// setters
void ColorPicker::setVisible(bool visible) {
	this->visible = visible;
}

void ColorPicker::setColor(Color color) {
	this->color = color;
}

void ColorPicker::reset() {
	if (!hasReset) {
		clickedRed = false;
		clickedGreen = false;
		clickedBlue = false;

		submitButton.setColor(SILVER);
		draw();
		hasReset = true;

		if (clickedSubmit) {
			setVisible(false);
			clickedSubmit = false;
		}
	}
}

void ColorPicker::resetClass() {
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
}

// getters
bool ColorPicker::isVisible() const {
	return visible;
}

Color ColorPicker::getColor() const {
	return color;
}

SDL_Window* ColorPicker::getWindow() const {
	return window;
}
