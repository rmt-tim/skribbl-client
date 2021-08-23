#include "Component.h"

Component::Component() : texture(nullptr), ID(-1), bounds() {

}

// constructor
Component::Component(SDL_Rect bounds, Color color, SDL_Texture* texture, int ID) : ID(ID), bounds(bounds), color(color), texture(texture) {

}

Component::Component(const Component& other) {
	bounds = other.bounds;
	color = other.color;
	texture = other.texture;
	ID = other.ID;
}

// init
void Component::init(SDL_Rect bounds, Color color, SDL_Texture* texture, int ID) {
	setBounds(bounds);
	setColor(color);
	setTexture(texture);
	setID(ID);
}

// getters
int Component::getID() const {
	return ID;
}

SDL_Rect Component::getBounds() const {
	return bounds;
}

SDL_Texture* Component::getTexture() const {
	return texture;
}

Color Component::getColor() const {
	return color;
}

// setters
void Component::setID(int ID) {
	this->ID = ID;
}

void Component::setBounds(SDL_Rect bounds) {
	this->bounds = bounds;
}

void Component::setTexture(SDL_Texture* texture) {
	this->texture = texture;
}

void Component::setColor(Color color) {
	this->color = color;
}

// operator overload
bool Component::operator==(const Component& other) {
	return ID == other.ID;
}
