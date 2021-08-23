#pragma once
#include "InputManager.h"
#include <glm/glm.hpp>
#include <SDL/SDL.h>
class GUIBase
{
protected:
	glm::ivec2 position;
	glm::ivec2 dimension;
	InputManager* inputManager;
public:
	GUIBase();
	GUIBase(glm::ivec2 position, glm::ivec2 dimension);

	virtual void draw() = 0;
	virtual void update(Uint32 deltaTime) = 0;

	void setPosition(glm::ivec2 position);
	void setDimension(glm::ivec2 dimension);

	glm::ivec2 getPosition() const;
	glm::ivec2 getDimension() const;
private:
	void setInputManager();
};

