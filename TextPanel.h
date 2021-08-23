#pragma once
#include "Window.h"
#include "Font.h"
#include "TextLine.h"
#include "Color.h"
#include <glm/glm.hpp>
#include <string>
class TextPanel : public Window
{
private:
	Font* font;
	TextLine* textLine;
	glm::ivec4 button;
	Color buttonColor;
	bool pressed;
public:
	TextPanel();
	~TextPanel();
	void init(Font* font);
	void draw();
	void update();
	void setFont(Font* font);
private:
	void initComponents();
	void closeWindow();
	void setPressed(bool pressed);
	void setButtonColor(Color color);
	void resetTextLine();
	bool isPressed();
};

