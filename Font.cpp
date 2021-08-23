#include "Font.h"
#include "ImageLoader.h"
#include <iostream>

Font::Font() : font(nullptr) {

}

Font::Font(std::string fontPath, int size) {
	init(fontPath, size);
}

Font::~Font() {
	/*if (font != nullptr) {
		TTF_CloseFont(font);
	}*/
}

// init
void Font::init(std::string fontPath, int size) {
	loadFont(fontPath, size);
}

void Font::loadFont(std::string fontPath, int size) {
	if ((font = TTF_OpenFont(fontPath.c_str(), size)) == nullptr) {
		std::cout << "Failed to load font: " << fontPath << std::endl;
	}
}

// getters
void Font::obtainTextData(std::string text, Color color, SDL_Renderer* renderer, SDL_Texture** texture, SDL_Rect* bounds, glm::ivec2 position) {
	SDL_Surface* fontSurface = ImageLoader::getFontSurface(text, font, color);

	if (fontSurface == nullptr) {
		return;
	}

	// set up text position
	*bounds = { position.x, position.y, fontSurface->w, fontSurface->h };

	if ((*texture = SDL_CreateTextureFromSurface(renderer, fontSurface)) == nullptr) {
		std::cout << "SDL_CreateTextureFromSurface has failed." << std::endl;
	}

	SDL_FreeSurface(fontSurface);
}

void Font::setFont(TTF_Font*& font) {
	this->font = font;
}

glm::ivec2 Font::getTextDimensions(std::string text) {
	SDL_Surface* fontSurface = ImageLoader::getFontSurface(text, font, BLACK);
	return glm::ivec2(fontSurface->w, fontSurface->h);
}

int Font::getTextHeight() {
	return TTF_FontHeight(font);
}

TTF_Font* Font::getFont() const {
	return font;
}
