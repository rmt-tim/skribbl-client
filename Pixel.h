#pragma once
#include "Color.h"
#include <glm/glm.hpp>
class Pixel
{
private:
	Color color;
	glm::ivec2 position;
public:
	Pixel(Color color, glm::ivec2 position);

	// getters
	Color getColor() const;
	glm::ivec2 getPosition() const;

	// setters
	void setColor(Color color);
	void setPosition(glm::ivec2 position);

	// operator overload
	bool operator==(const Pixel& other);
	bool operator!=(const Pixel& other);
};

