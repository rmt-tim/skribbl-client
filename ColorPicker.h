#pragma once
#include "Component.h"
#include "InputManager.h"
#include "Color.h"
#include "Font.h"
#include <SDL/SDL.h>
#include <SDL/SDL_render.h>
#include <string>
class ColorPicker
{
private:
	enum class Factor {
		R,
		G,
		B
	};
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Component colorPanel;
	Component submitButton;

	Component toggleRed;
	Component toggleGreen;
	Component toggleBlue;

	Font font;

	std::string redLabel;
	std::string greenLabel;
	std::string blueLabel;

	Color color;
	InputManager* inputManager;

	bool clickedRed;
	bool clickedGreen;
	bool clickedBlue;
	bool clickedSubmit;
	bool hasReset;
	bool visible;
public:
	// constructors
	ColorPicker();
	~ColorPicker();

	// init
	void init(Color color);

	// update
	void update();

	// setters
	void setFont(Font font);
	void closeWindow();

	// getters
	bool isVisible() const;
	Color getColor() const;
	SDL_Window* getWindow() const;
private:
	// init
	void initWindow();
	void initComponents(Color color);

	// update
	void updateToggleButton(glm::ivec2 mouseCoords, Component* toggleButton);
	void updateColor(glm::ivec2 mouseCoords, Factor factor);
	void updateColorPanel();

	// setters
	void setVisible(bool visible);
	void setColor(Color color);
	void reset();
	void resetClass();

	// draw
	void draw();
};

