#pragma once
#include "Color.h"
#include "Pixel.h"
#include "Font.h"
#include "Controller.h"
#include <glm/glm.hpp>
#include <SDL/SDL_surface.h>
#include <SDL/SDL.h>
#include <SDL_TTF/SDL_ttf.h>
#include <string>
#include <vector>
#include <queue>
class Utils
{
private:
	static Uint32 rmask, gmask, bmask, amask;
	static SDL_Rect screenBounds;
public:
	static bool squareCollision(SDL_Rect r1, SDL_Rect r2);
	static bool areColliding(SDL_Rect r1, SDL_Rect r2);
	static bool isPointInsideBounds(glm::ivec2 mouseCoords, SDL_Rect bounds);
	static bool isPointInsideBounds(glm::ivec2 mouseCoords, glm::ivec4 bounds);
	static int calculateRGBValueFromPositon(int x, int A, int B, int C);
	static int calculatePositionFromRGBValue(int A, int B, int C, int D);
	static SDL_Cursor* getCursor(Mode state);
	static std::vector<SDL_Rect> getLinePath(glm::ivec2 p1, glm::ivec2 p2, int pathWidth, int pathHeight);
	static Color getButtonColor(int index);
	static void paintWithBucket(glm::ivec2 seedCoords, Color color, SDL_Renderer* renderer);
	static void setUtilityButton(int index, SDL_Renderer* renderer, SDL_Texture** texture, int* ID);
	static void setActionState(int ID);
	static void drawText(std::string text, Color color, SDL_Renderer* renderer, Font* font, glm::ivec2 position);
	static void drawText(std::string text, Color color, SDL_Renderer* renderer, Font* font, glm::ivec2 position, int width);
	static void drawRectangle(Color color, SDL_Renderer* renderer, glm::ivec4 posisiton, bool fill = true);
	static void drawButton(Color buttonColor, std::string text, Color textColor, SDL_Renderer* renderer, Font* font, glm::ivec4 posisiton);
	static void drawCenteredText(std::string text, Color color, SDL_Renderer* renderer, Font* font, glm::ivec4 dimensions);
	static SDL_Window* createWindow(std::string title, glm::ivec2 dimensions);
	static SDL_Renderer* createRenderer(SDL_Window* window);
	static SDL_Texture* makeScreenshot(glm::ivec4 dimensions, SDL_Renderer* renderer);
	static void drawScreenshot(glm::ivec4 dimensions, SDL_Renderer* renderer, SDL_Texture* screenshot);
private:
	static glm::fvec2 lerp(glm::fvec2 p1, glm::fvec2 p2, float t);
	static Pixel getPixel(glm::ivec2 position, SDL_Surface* surface);
	static void getNeighbours(Pixel pixel, Color color, SDL_Surface* surface, std::queue<Pixel>& pixels);
	static void getPixels(SDL_Renderer* renderer, SDL_Surface* surface);
	static void paintPixel(Pixel pixel, SDL_Renderer* renderer);
	static void paintPixel(Pixel pixel, Color color, SDL_Renderer* renderer);
	static void paintSurfacePixel(Pixel pixel, SDL_Surface* surface);

	template <typename T>
	static bool contains(std::vector<T> vector, T value);

	template <typename T, typename F>
	static bool contains(std::queue<T, F> queue, T node);
};

template<typename T>
inline bool Utils::contains(std::vector<T> vector, T value) {
	for (size_t i = 0; i < vector.size(); i++) {
		if (vector[i] == value) {
			return true;
		}
	}
	return false;
}

template<typename T, typename F>
inline bool Utils::contains(std::queue<T, F> queue, T value) {
	while (!queue.empty()) {
		if (queue.front() == value) {
			return true;
		}
		queue.pop();
	}
	return false;
}
