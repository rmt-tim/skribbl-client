#include "MainPanel.h"
#include "Config.h"
#include "Utils.h"
#include "ImageLoader.h"
#include <iostream>

MainPanel::MainPanel() : brushSize(1), renderer(nullptr), controller(nullptr), selectedColor(BLACK) {

}

void MainPanel::init() {
	initComponents();
}

void MainPanel::initComponents() {
	initPanels();
	initButtons();
	initController();
}

void MainPanel::initPanels() {
	panels.reserve(2);
	// ================= <Main Panel> ================= //
	SDL_Rect mainPanelBounds;

	mainPanelBounds.w = MAIN_PANEL_WIDTH;
	mainPanelBounds.h = MAIN_PANEL_HEIGHT;

	mainPanelBounds.x = MAIN_PANEL_START_X;
	mainPanelBounds.y = MAIN_PANEL_START_Y;

	panels.emplace_back(mainPanelBounds, SILVER);

	// ================= <Color Panel> ================= //
	SDL_Rect colorPanelBounds;

	colorPanelBounds.w = COLOR_PANEL_WIDTH;
	colorPanelBounds.h = COLOR_PANEL_HEIGHT;

	colorPanelBounds.x = COLOR_PANEL_START_X;
	colorPanelBounds.y = COLOR_PANEL_START_Y;

	panels.emplace_back(colorPanelBounds, GRAY);

	// ================= <Current Color Panel> ================= //
	SDL_Rect currentColorPanel;

	currentColorPanel.w = CURRENT_COLOR_PANEL_WIDTH;
	currentColorPanel.h = CURRENT_COLOR_PANEL_HEIGHT;

	currentColorPanel.x = CURRENT_COLOR_PANEL_X;
	currentColorPanel.y = CURRENT_COLOR_PANEL_Y;

	panels.emplace_back(currentColorPanel, GRAY);

	// ================= <Utility Panel> ================= //
	SDL_Rect utilityPanel;

	utilityPanel.w = UTILITY_PANEL_WIDTH;
	utilityPanel.h = UTILITY_PANEL_HEIGHT;

	utilityPanel.x = UTILITY_PANEL_X;
	utilityPanel.y = UTILITY_PANEL_Y;

	panels.emplace_back(utilityPanel, GRAY);
}

void MainPanel::initButtons() {
	// ================= <Color Buttons> ================= //
	colorButtons.reserve(COLOR_BUTTON_NUMBER);

	for (size_t i = 0; i < 2; i++) {
		for (size_t j = 0; j < (COLOR_BUTTON_NUMBER - 1) / 2; j++) {
			SDL_Rect bounds;

			bounds.w = COLOR_BUTTON_WIDTH;
			bounds.h = COLOR_BUTTON_HEIGHT;

			bounds.x = COLOR_BUTTON_START_X + j * COLOR_BUTTON_OFFSET;
			bounds.y = COLOR_BUTTON_START_Y + i * COLOR_BUTTON_OFFSET;

			colorButtons.emplace_back(bounds, Utils::getButtonColor(i * COLOR_BUTTON_NUMBER / 2 + j));
		}
	}

	// ================= <Current Color Button> ================= //
	SDL_Rect bounds;

	bounds.w = CURRENT_COLOR_BUTTON_WIDTH;
	bounds.h = CURRENT_COLOR_BUTTON_HEIGHT;

	bounds.x = CURRENT_COLOR_BUTTON_X;
	bounds.y = CURRENT_COLOR_BUTTON_Y;

	colorButtons.emplace_back(bounds, BLACK);

	// ================= <Brush Buttons> ================= //
	brushButtons.reserve(2);
	for (size_t i = 0; i < 2; i++) {
		SDL_Texture* texutre = nullptr;
		if (i == 0) {
			texutre = ImageLoader::loadTexture(PLUS_BUTTON_PATH, renderer);
		}
		else {
			texutre = ImageLoader::loadTexture(MINUS_BUTTON_PATH, renderer);
		}

		SDL_Rect bounds;

		bounds.w = PANEL_BUTTON_WIDTH;
		bounds.h = PANEL_BUTTON_HEIGHT;

		bounds.x = BUTTON_START_X + i * BUTTON_SAPWN_DIFF_X;
		bounds.y = BUTTON_START_Y;

		brushButtons.emplace_back(bounds, BLACK, texutre, i);
	}

	// ================= <Utility Buttons> ================= //
	utilityButtons.reserve(4);
	for (size_t i = 0; i < 2; i++) {
		SDL_Texture* texture = nullptr;
		int ID = 0;

		Utils::setUtilityButton(i, renderer, &texture, &ID);

		SDL_Rect bounds;

		bounds.w = UTILITY_BUTTON_WIDTH;
		bounds.h = UTILITY_BUTTON_HEIGHT;

		bounds.x = UTILITY_BUTTON_START_X + 2;
		bounds.y = UTILITY_BUTTON_START_Y + i * UTILITY_BUTTON_VERTICAL_OFFSET;

		utilityButtons.emplace_back(bounds, GREEN, texture, ID);
	}
}

void MainPanel::initController() {
	if (controller == nullptr) {
		controller = Controller::getInstance();
		inputManager = controller->getInputManager();
	}
}

// update functions
bool MainPanel::update() {
	return updateColorButtons() || updateBrushButtons() || updateUtilityButtons();
}

bool MainPanel::openColorPicker() {
	if (Utils::isPointInsideBounds(inputManager->getMouseCoordinates(), colorButtons[colorButtons.size() - 1].getBounds()) && inputManager->isDoubleClick()) {
		return true;
	}
	return false;
}

bool MainPanel::updateColorButtons() {
	for (size_t i = 0; i < colorButtons.size(); i++) {
		if (Utils::isPointInsideBounds(inputManager->getMouseCoordinates(), colorButtons[i].getBounds())) {
			selectedColor = colorButtons[i].getColor();
			colorButtons[colorButtons.size() - 1].setColor(selectedColor);
			return true;
		}
	}
	return false;
}

bool MainPanel::updateBrushButtons() {
	for (size_t i = 0; i < brushButtons.size(); i++) {
		if (Utils::isPointInsideBounds(inputManager->getMouseCoordinates(), brushButtons[i].getBounds())) {
			if (brushButtons[i].getID() == 0) {
				brushSize = glm::clamp(brushSize + BRUSH_INCREMENT, 1, 20);
			}
			else {
				brushSize = glm::clamp(brushSize - BRUSH_INCREMENT, 1, 20);
			}
			return true;
		}
	}
	return false;
}

bool MainPanel::updateUtilityButtons() {
	for (size_t i = 0; i < utilityButtons.size(); i++) {
		if (Utils::isPointInsideBounds(inputManager->getMouseCoordinates(), utilityButtons[i].getBounds())) {
			Utils::setActionState(utilityButtons[i].getID());
			return true;
		}
	}
	return false;
}

// getters
std::vector<Component> MainPanel::getPanels() const {
	return panels;
}

std::vector<Component> MainPanel::getColorButtons() const {
	return colorButtons;
}

std::vector<Component> MainPanel::getBrushButtons() const {
	return brushButtons;
}

std::vector<Component> MainPanel::getUtilityButtons() const {
	return utilityButtons;
}

int MainPanel::getBrushSize() const {
	return brushSize;
}

Color MainPanel::getSelectedColor() const {
	return selectedColor;
}

// setters
void MainPanel::setRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

void MainPanel::setSelectedColor(Color selectedColor) {
	colorButtons[colorButtons.size() - 1].setColor(selectedColor);
	this->selectedColor = selectedColor;
}

void MainPanel::setFont(Font font) {
	this->font = font;
}
