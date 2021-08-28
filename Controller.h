#pragma once
#include "InputManager.h"
#include "source/client/Client.h"
#include <glm/glm.hpp>
#include <string>

enum class Mode {
	NONE,
	PAINTING,
	ERASING,
	TEXT
};

enum class ScreenState {
	FREEZE,
	REFRESH
};

class Controller
{
private:
	static Controller* INSTANCE;
	Mode mode;
	Mode previousMode;
	InputManager inputManager;
	ScreenState screenState;
	glm::ivec2 textPosition;
	glm::ivec2 indicatorPosition;
	std::string text;
	std::string username;
	std::string word;
	bool drawer;
	bool start;
private:
	Controller();
	~Controller();
public:
	static Controller* getInstance();

	// getters
	Mode getActionState() const;
	Mode getPreviousActionState() const;
	ScreenState getScreenState() const;
	InputManager* getInputManager();
	glm::ivec2 getTextPosition() const;
	glm::ivec2 getIndicatorPosition() const;
	std::string getText() const;
	std::string getUsername() const;

	// modes
	bool isNone() const;
	bool isPainting() const;
	bool isEraseing() const;
	bool isWriting() const;

	// setters
	void setMode(Mode actionState);
	void setScreenState(ScreenState screenState);
	void setPrevoiusMode(Mode previousActionState);
	void setTextPosition(glm::ivec2 textPosition);
	void setIndicatorPosition(glm::ivec2 indicatorPosition);
	void appendText(char* newText, int index);
	void setUsername(std::string name);
	void resetText();
	bool removeCharacter();
	void updatePreviousMode();

	// network
	void startGame();
	void login(std::string username);
	
	void setDrawer(std::string username);
	bool isDrawer() const;

	void setStart(bool start);
	bool hasStarted() const;

	void setWord(std::string word);
	std::string getWord() const;
};

