#include "color.h"

Color::Color() {
	this->R = 0;
	this->G = 0;
	this->B = 0;
}

Color::Color(float r, float g, float b) {
	this->R = r;
	this->G = g;
	this->B = b;
}

Color Color::operator*(const Color& c) const {
	// Multiplikation zweier Color Objekte
	float newR = this->R * c.R;
	float newG = this->G * c.G;
	float newB = this->B * c.B;
	return Color{ newR, newG, newB };
}

Color Color::operator*(const float Factor) const {
	// Multiplikation von this mit einem Faktor
	float newR = this->R * Factor;
	float newG = this->G * Factor;
	float newB = this->B * Factor;
	return Color{ newR, newG, newB };
}

Color Color::operator+(const Color& c) const {
	// Addition zweier Color Objekte
	float newR = this->R + c.R;
	float newG = this->G + c.G;
	float newB = this->B + c.B;
	return Color{ newR, newG, newB };
}

Color& Color::operator+=(const Color& c) {
	// Addition eines Color Objektes auf this;
	this->R += c.R;
	this->G += c.G;
	this->B += c.B;
	return *this;
}