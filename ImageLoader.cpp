#include "ImageLoader.h"
#include <SDL_IMAGE/SDL_image.h>
#include <iostream>

std::unordered_map<std::string, SDL_Surface*> ImageLoader::surfaceMap;

ImageLoader::~ImageLoader() {
	for (auto it = surfaceMap.begin(); it != surfaceMap.end(); it++) {
		SDL_FreeSurface(it->second);
	}
}

SDL_Texture* ImageLoader::loadTexture(std::string filePath, SDL_Renderer* renderer) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* loadedSurface = loadSurface(filePath);

	texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	if (texture == nullptr) {
		std::cout << "Failed to load the texture from image." << std::endl;
		return nullptr;
	}

	SDL_FreeSurface(loadedSurface);

	return texture;
}

SDL_Surface* ImageLoader::loadSurface(std::string filePath) {
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

	if (loadedSurface == nullptr) {
		std::cout << "Failed to load the image." << std::endl;
		return nullptr;
	}

	return loadedSurface;
}

SDL_Surface* ImageLoader::loadCursorSurface(std::string filePath) {
	SDL_Surface* loadedSurface = findSurface(filePath);

	if (loadedSurface == nullptr) {
		loadedSurface = loadSurface(filePath);
		surfaceMap[filePath] = loadedSurface;
	}

	return loadedSurface;
}

SDL_Surface* ImageLoader::getFontSurface(std::string text, TTF_Font* font, Color color) {
	SDL_Color colorFG{ color.getR(), color.getG(), color.getB(), color.getA() };
	SDL_Surface* textSurface = nullptr;

	if ((textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), colorFG)) == nullptr) {
		std::cout << "TTF_RenderText_Solid has failed." << std::endl;
	}

	return textSurface;
}

SDL_Surface* ImageLoader::findSurface(std::string filePath) {
	auto iterator = surfaceMap.find(filePath);

	if (iterator != surfaceMap.end()) {
		return iterator->second;
	}

	return nullptr;
}
