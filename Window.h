#pragma once
#include "InputManager.h"
#include <SDL/SDL.h>
#include <SDL/SDL_render.h>
#include <glm/glm.hpp>
#include <string>
class Window
{
protected:
	SDL_Window* window;
	SDL_Renderer* renderer;
	InputManager* inputManager;
	bool visible;
public:
	Window();
	~Window();

	void init(std::string title, glm::ivec2 dimensions);

	virtual void draw() = 0;
	virtual void update() = 0;

	bool isVisible();
protected:
	void setVisible(bool visible);
	void setInputManager(InputManager* inputManager);

	virtual void initComponents() = 0;

	void close();
private:
	void reset();
	void resetWindow();
	void resetRenderer();

	void initWindow(std::string title, glm::ivec2 dimensions);
	void initRenderer();
};

