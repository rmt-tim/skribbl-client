#pragma once
#include "InputManager.h"
#include "MainPanel.h"
#include "ColorPicker.h"
#include "Font.h"
#include "Controller.h"
#include "Timer.h"
#include "ChatPanel.h"
#include "MessagePanel.h"
#include "TextPanel.h"
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <SDL_IMAGE/SDL_image.h>
#include <vector>
#include <stack>
#include <string>

class MainFrame
{
private:
	enum class GameState {
		PLAY,
		EXIT
	};
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	GameState gameState;
	InputManager* inputManager;
	ColorPicker colorPicker;
	MessagePanel messagePanel;
	TextPanel textPanel;
	Controller* controller;
	Font font;
	Font normalFont;
	SDL_Rect rubber;
	MainPanel mainPanel;
	ChatPanel chatPanel;
	Timer timer;
	glm::ivec2 start;
	glm::ivec2 end;
	Uint32 panelTimer;
	bool showed;
public:
	MainFrame();
	~MainFrame();
	void callback(std::string username);
private:
	void init();
	void initSDL();
	void initTTF();
	void initFont();
	void initSDL_Image();
	void initController();
	void initComponents();
	void run();
	void handleInputEvents();
	void handleWindowEvents(SDL_Event& event);
	void update();
	void updateColorPicker();
	void updateMessagePanel();
	void updateTextPanel();
	void updateMainPanel();
	void updateCursorActivity(glm::ivec2 mouseCoords);
	void initDraw();
	void drawHUD();
	void drawChatPanel();
	void updateCursor();
	void updateScreen();
	void reset();
	void returnToPreviousState();
	void savePreviousState();
	void setMode(Mode mode);
	void paint(glm::ivec2 mouseCoords);
	void erase(glm::ivec2 mouseCoords);
	void updatePanelTimer(Uint32 deltaTime);
	void updateChatPanel();
	bool doRefresh();
};

