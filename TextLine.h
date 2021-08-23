#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL/SDL_render.h>
#include <SDL/SDL.h>
#include "Font.h"
#include "InputManager.h"
class TextLine
{
private:
	std::string text;
	Uint32 deleteTimer;
	Uint32 cursorTimer;
	Uint32 showTime;
	Font* font;
	InputManager* inputManager;
	SDL_Renderer* renderer;
	glm::ivec2 cursorPosition;
	glm::ivec2 textPosition;
	int textSize;
	bool visible;
	bool enable;
public:
	TextLine(glm::ivec2 textPosition, int textSize, SDL_Renderer* renderer, Font* font);
	~TextLine();
	void draw();
	void update(Uint32 deltaTime);
	void clear();
	void setEnable(bool state);
	void setVisibleCursor(bool visible);
	bool isEnabled();
	int getSize();
	std::string getText();
private:
	void init();
	void reset();
	void updateText(Uint32 deltaTime);
	void updateCursor(Uint32 deltaTime);
	void updateCursorPosition();
	void updateCursorVisibility(Uint32 deltaTime);
	void appendText();
	void drawText();
	void drawCursor();
	void showCursor();
	bool isVisible();
};

