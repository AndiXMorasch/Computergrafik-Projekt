#include "vector.h"
#include <math.h>

Vector::Vector(float x, float y, float z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

Vector::Vector()
{
	// uninitialisiert
}

float Vector::dot(const Vector& v) const
{
	// (1) Winkel zwischen zwei Vektoren berechnen
	// (2) Senkrechtprojektion
	// (2) sind Vektoren senkrecht aufeinander?
	// Wenn Skalarprodukt = 0, dann ja, sonst nein!

	return (v.X * this->X) + (v.Y * this->Y) + (v.Z * this->Z);
}

Vector Vector::cross(const Vector& v) const
{
	// Sinn: (1) Gegeben sind zwei Vektoren, das 
	// Kreuzprodukt bestimmt dann einen neuen Vektor 
	// der Senkrecht auf den beiden Vektoren steht.

	float newX = (this->Y * v.Z) - (this->Z * v.Y);
	float newY = (this->Z * v.X) - (this->X * v.Z);
	float newZ = (this->X * v.Y) - (this->Y * v.X);

	return Vector(newX, newY, newZ);
}

Vector Vector::operator+(const Vector& v) const
{
	// Addition zweier Vektoren
	float newX = this->X + v.X;
	float newY = this->Y + v.Y;
	float newZ = this->Z + v.Z;
	return Vector(newX, newY, newZ);
}

Vector Vector::operator-(const Vector& v) const
{
	// Subtraktion zweier Vektoren
	float newX = this->X - v.X;
	float newY = this->Y - v.Y;
	float newZ = this->Z - v.Z;
	return Vector(newX, newY, newZ);
}

Vector Vector::operator*(float c) const
{
	// Multiplikation zweier Vektoren
	float newX = this->X * c;
	float newY = this->Y * c;
	float newZ = this->Z * c;
	return Vector(newX, newY, newZ);
}

Vector Vector::operator-() const
{
	// Negation des Vektors
	float newX = this->X - (this->X * 2);
	float newY = this->Y - (this->Y * 2);
	float newZ = this->Z - (this->Z * 2);
	return Vector(newX, newY, newZ);
}

Vector& Vector::operator+=(const Vector& v)
{
	// Addition eines Vektors auf this;
	this->X = this->X + v.X;
	this->Y = this->Y + v.Y;
	this->Z = this->Z + v.Z;
	return *this;
}

Vector& Vector::normalize()
{
	// Sinn: Ein normierter Vektor hat immer die
	// Länge 1 und berechnet sich durch 1/Länge von
	// Vektor x-mal dem Vektor x! (Einheitskreis)
	float norm = 1 / length();
	this->X = norm * this->X;
	this->Y = norm * this->Y;
	this->Z = norm * this->Z;
	return *this;
}

float Vector::length() const
{
	// Länge des Vektors this;
	return sqrt(lengthSquared());
}

float Vector::lengthSquared() const
{
	// Quadrieren der einzelnen Komponenten von this;
	return pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2);
}

Vector Vector::reflection(const Vector& normal) const
{
	// Quelle: https://www.onlinemathe.de/forum/Reflexion-eines-Vektors-an-einer-Normalen
	// Sinn: Liefert den Reflexionsvektor zurück.
	return *this - (normal * 2 * dot(normal));
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	// Schritt 1: Schnittpunkt mit der Ebene bestimmen in der das Dreieck liegt

	// Ebenengleichung
	Vector n = (b - a).cross(c - a).normalize();
	float dFloat = n.dot(a);

	// Strahlgleichung
	s = (dFloat - n.dot(*this)) / (n.dot(d));

	// Wenn s < 0, dann zeigt der Strahl vom Dreieck weg
	if (s < 0) {
		return false;
	}

	// Schritt 2: Prüfen, ob Schnittpunkt p mit Ebene innerhalb des Dreiecks liegt

	Vector p = *this + (d * s);

	float abc = ((b - a).cross(c - a)).length() / 2;
	float abp = ((b - a).cross(p - a)).length() / 2;
	float acp = ((c - a).cross(p - a)).length() / 2;
	float bcp = ((c - b).cross(p - b)).length() / 2;
	float epsilon = 1e-6;

	if (abc + epsilon >= abp + acp + bcp) {
		// Schnitt liegt vor
		return true;
	}
	// Schnitt liegt nicht vor
	return false;
}
