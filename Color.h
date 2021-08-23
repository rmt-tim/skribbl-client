#pragma once
#include <SDL/SDL.h>
#include <vector>

// some predefined colors
#define RED Color(255, 0, 0, 255)
#define GREEN Color(0, 255, 0, 255)
#define BLUE Color(0, 0, 255, 255)
#define YELLOW Color(255, 255, 0, 255)
#define WHITE Color(255, 255, 255, 255)
#define BLACK Color(0, 0, 0, 255)
#define BYZANTINE Color(179, 32, 181, 255)
#define SEA_GREEN Color(68, 141, 94, 255)
#define TRUE_BLUE Color(58, 108, 190, 255)
#define CELTIC_BLUE Color(33, 108, 227, 255)
#define SKY_MAGENTA Color(214, 115, 189, 255)
#define LEMON_GLACIER Color(251, 255, 0, 255)
#define CITRON Color(154, 157, 22, 255)
#define MARIGOLD Color(236, 162, 26, 255)
#define BRICK_RED Color(190, 64, 77, 255)
#define LIGHT_STEEL_BLUE Color(187, 207, 240, 255)
#define DARK_CORNFLOWER_BLUE Color(21, 59, 120, 255)
#define LIME_GREEN Color(53, 196, 48, 255)
#define FOREST_GREEN_WEB Color(42, 147, 38, 255)
#define ZOMP Color(35, 162, 130, 255)
#define YELLOW_GREEN Color(142, 212, 88, 255)
#define SILVER Color(192, 192, 192, 255)
#define GRAY Color(128, 128, 128, 255)

class Color
{
private:
	std::vector<Uint8> coefficients;
public:
	// constructors;
	Color();
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	// init
	void init(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	// getters
	Uint8 getR();
	Uint8 getG();
	Uint8 getB();
	Uint8 getA();

	// setters
	void setR(Uint8 r);
	void setG(Uint8 g);
	void setB(Uint8 b);
	void setA(Uint8 a);

	// operator overload
	bool operator==(const Color& color);
	bool operator!=(const Color& color);
};

