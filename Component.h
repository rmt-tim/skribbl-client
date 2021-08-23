#pragma once
#include "Color.h"
#include <SDL/SDL_rect.h>
#include <SDL/SDL_render.h>
class Component
{
private:
	int ID;
	SDL_Rect bounds;
	SDL_Texture* texture;
	Color color;
public:
	// constructors
	Component();
	Component(SDL_Rect bounds, Color color, SDL_Texture* texture = nullptr, int ID = -1);
	Component(const Component& other);

	// init
	void init(SDL_Rect bounds, Color color, SDL_Texture* texture = nullptr, int ID = -1);

	// getters
	int getID() const;
	SDL_Rect getBounds() const;
	SDL_Texture* getTexture() const;
	Color getColor() const;

	// setters
	void setID(int ID);
	void setBounds(SDL_Rect bounds);
	void setTexture(SDL_Texture* texture);
	void setColor(Color color);

	// operator overload
	bool operator==(const Component& other);
};

