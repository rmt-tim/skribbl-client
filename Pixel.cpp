#include "Pixel.h"

Pixel::Pixel(Color color, glm::ivec2 position) : color(color), position(position) {

}

// getters
Color Pixel::getColor() const {
	return color;
}

glm::ivec2 Pixel::getPosition() const {
	return position;
}

// setters
void Pixel::setColor(Color color) {
	this->color = color;
}

void Pixel::setPosition(glm::ivec2 position) {
	this->position = position;
}

// operator overload
bool Pixel::operator==(const Pixel& other) {
	return color == other.color;
}

bool Pixel::operator!=(const Pixel& other) {
	return !operator==(other);
}
