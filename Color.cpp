#include "Color.h"

Color::Color() {
	init(0, 0, 0, 0);
}

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	init(r, g, b, a);
}

void Color::init(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	coefficients.resize(4);
	setR(r);
	setG(g);
	setB(b);
	setA(a);
}

// getters
Uint8 Color::getR() {
	return coefficients[0];
}

Uint8 Color::getG() {
	return coefficients[1];
}

Uint8 Color::getB() {
	return coefficients[2];
}

Uint8 Color::getA() {
	return coefficients[3];
}

// setters
void Color::setR(Uint8 r) {
	coefficients[0] = r;
}

void Color::setG(Uint8 g) {
	coefficients[1] = g;
}

void Color::setB(Uint8 b) {
	coefficients[2] = b;
}

void Color::setA(Uint8 a) {
	coefficients[3] = a;
}

// operator overload
bool Color::operator==(const Color& color) {
	return coefficients[0] == color.coefficients[0] && coefficients[1] == color.coefficients[1] && coefficients[2] == color.coefficients[2] && coefficients[3] == color.coefficients[3];
}

bool Color::operator!=(const Color& color) {
	return !operator==(color);
}
