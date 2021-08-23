#pragma once
#include "GUIBase.h"
#include "Font.h"
#include "TextLine.h"
#include <SDL/SDL_render.h>
#include <string>
#include <vector>
#include <queue>

class ChatPanel : GUIBase
{
private:
	SDL_Renderer* renderer;
	TextLine* textLine;
	std::queue<std::string> messages;
	Font* font;
	int offsetY;
public:
	ChatPanel();
	~ChatPanel();

	void draw();
	void update(Uint32 deltaTime);
	void init(SDL_Renderer* renderer, Font* font);

	void setRenderer(SDL_Renderer* renderer);
	void setFont(Font* font);
	void addMessage(std::string message);
private:
	void drawPanels();
	void drawMessages();
	void initTextLine();
	void enterEvent();
	void updateTextLine(Uint32 deltaTime);
};

