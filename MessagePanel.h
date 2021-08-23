#pragma once
#include "Font.h"
#include "Component.h"
#include "InputManager.h"
#include "Window.h"
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_render.h>
#include <glm/glm.hpp>

class MessagePanel : public Window
{
private:
	Font* font;
	glm::ivec4 button;
	Color buttonColor;
	bool isPressed;
	std::string message;
public:
	MessagePanel();
	~MessagePanel();
	void init(std::string message, Font* font);
	void setMessage(std::string message);
	void setFont(Font* font);
	void draw();
	void update();
	void closeWindow();
	SDL_Window* getWindow() const;
private:
	void initComponents();
	void setIsPressed(bool isPressed);
	void setButtonColor(Color color);
};

