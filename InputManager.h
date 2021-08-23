#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>
class InputManager
{
private:
	std::unordered_map<unsigned int, bool> keyMap;
	glm::ivec2 mouseCoordinates;
	bool moving;
	bool append;
	std::string text;
	int clickNumber;
	unsigned int windowID;
public:
	// setters
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	void setMouseCoordinates(int x, int y);
	void setMoving(bool moving);
	void setAppend(bool append);
	void setClickNumber(int clickNumber);
	void setWindowID(unsigned int windowID);
	void setTextInput(char* newText, bool append);
	void setText(char* newText);

	// getters
	glm::ivec2 getMouseCoordinates() const;
	bool isKeyPressed(unsigned int keyID);
	bool isMoving() const;
	bool isAppend() const;
	bool isDoubleClick() const;
	unsigned int getWindowID() const;
	std::string getText() const;
};

