#include <fstream>
#include <cstring>
#include "rgbimage.h"
#include "color.h"
#include "assert.h"

RGBImage::RGBImage(unsigned int Width, unsigned Height)
{
	// Quelle: https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
	this->m_Width = Width;
	this->m_Height = Height;
	this->m_Image = new Color[m_Width * m_Height];
}

RGBImage::~RGBImage() {
	// Destruktor
	delete[] m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {
	// Setzt ein mitgegebenes Color Objekt an die Position (x,y) des m_Image Arrays
	if (x >= m_Width || y >= m_Height) {
		throw std::invalid_argument("Die mitgegebene Höhe oder Breite über- / unterschreitet die zulässigen Bereiche");
	}
	this->m_Image[x + m_Width * y] = c;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const {
	// Gibt das Color Objekt an der Stelle (x,y) des m_Images Arrays zurück
	if (x >= m_Width || y >= m_Height) {
		throw std::invalid_argument("Die mitgegebene Höhe oder Breite über- / unterschreitet die zulässigen Bereiche");
	}
	return this->m_Image[x + m_Width * y];
}

unsigned int RGBImage::width() const {
	// Gibt die Breite des 2D Arrays zurück
	return this->m_Width;
}

unsigned int RGBImage::height() const {
	// Gibt die Höhe des 2D Arrays zurück
	return this->m_Height;
}

RGBImage& RGBImage::SobelFilter(RGBImage& dest, const RGBImage& src, float factor)
{
	// Sobel-Filter: Der Sobel-Filter wird dazu verwendet eine Mix-Map zu erzeugen.
	// Der Sinn einer Mix-Map besteht darin, steile Bereiche im Terrain zu erkennen
	// und durch helle Pixel hervorzuheben. Dieses Prinzip der Kantenhervorhebung
	// lässt sich natürlich nicht nur auf Terrains verwenden, sondern auf allerlei Bilder.

	// src = Eingangsbild
	// dest = Ausgangsbild
	assert(dest.height() == src.height());
	assert(dest.width() == src.width());

	// Filter Maske
	float  K[3][3] = { {1.0f, 0.0f, -1.0f},
		{2.0f, 0.0f, -2.0f},
		{1.0f, 0.0f, -1.0f} };

	// Für jeden Pixel aus dem RGB-Image (links nach rechts)
	for (int x = 1; x < src.width() - 1; x++) {
		for (int y = 1; y < src.height() - 1; y++) {
			Color U; // Farbwert bezüglich horizontaler farbl. Unterschiede
			Color V; // Farbwert bezüglich vertikaler farbl. Unterschiede
			// Alle Farbwerte (9) in der 3x3 Matrix multiplizieren mit der korrespondierenden Zahl in der Filter-Maske
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					// Berechnung von U und V
					U += src.getPixelColor(x + i - 1, y + j - 1) * K[i][j]; // Filter-Maske bezüglich horizontaler farbl. Unterschiede
					V += src.getPixelColor(x + i - 1, y + j - 1) * K[j][i]; // Filter-Maske bezüglich vertikaler farbl. Unterschiede
				}
			}
			// Gesetzte U und V Farben dazu nutzen um neue RGB Werte lt. Formel auszurechnen
			Color pixelColor;
			pixelColor.R = sqrt(pow(U.R, 2) + pow(V.R, 2));
			// pixelColor.G = sqrt(pow(U.G, 2) + pow(V.G, 2));
			// pixelColor.B = sqrt(pow(U.B, 2) + pow(V.B, 2));
			// Pixelfarbe in dem destination RGBImage setzen
			dest.setPixelColor(x, y, Color(pixelColor.R, pixelColor.R, pixelColor.R) * factor); // factor = Verstärkung der Kantenhervorhebung
		}
	}
	return dest;
}

unsigned char RGBImage::convertColorChannel(float v) {
	// Umwandlung eines floats v in einen 256 Farbkanal (8 Bit)
	if (v > 1.0) {
		return (unsigned char)(1.0 * 255);
	}
	else if (v < 0.0) {
		return (unsigned char)(0.0 * 255);
	}
	return (unsigned char)(v * 255);
}

// Quelle: https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
bool RGBImage::saveToDisk(const char* Filename) {

	// Datei öffnen und Platz im Speicher schaffen
	FILE* f = fopen(Filename, "wb");
	unsigned char* img = nullptr;
	unsigned int filesize = (14 + 40) + 3 * width() * height();
	img = (unsigned char*)malloc(3 * width() * height());
	memset(img, 0, 3 * width() * height());

	// Zuweisung der einzelnen Farbwerte in das dafür vorgesehene Array
	for (int i = 0; i < width(); i++) {
		for (int j = 0; j < height(); j++) {
			img[(i + j * width()) * 3 + 2] = (unsigned char)convertColorChannel(getPixelColor(i, j).R);
			img[(i + j * width()) * 3 + 1] = (unsigned char)convertColorChannel(getPixelColor(i, j).G);
			img[(i + j * width()) * 3 + 0] = (unsigned char)convertColorChannel(getPixelColor(i, j).B);
		}
	}

	// Anlage der Arrays gemäß dem Dateiformat
	unsigned char fileHeader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char infoHeader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	unsigned char padding[3] = { 0, 0, 0 };

	// Byteweise Aufteilung in die jeweiligen dafür vorgesehenen Plätze des Arrays, da
	// die Dateigröße, Breite und Höhe größer sein kann als 1 Byte
	fileHeader[2] = (unsigned char)(filesize);
	fileHeader[3] = (unsigned char)(filesize >> 8);
	fileHeader[4] = (unsigned char)(filesize >> 16);
	fileHeader[5] = (unsigned char)(filesize >> 24);
	infoHeader[4] = (unsigned char)(width());
	infoHeader[5] = (unsigned char)(width() >> 8);
	infoHeader[6] = (unsigned char)(width() >> 16);
	infoHeader[7] = (unsigned char)(width() >> 24);
	infoHeader[8] = (unsigned char)(height());
	infoHeader[9] = (unsigned char)(height() >> 8);
	infoHeader[10] = (unsigned char)(height() >> 16);
	infoHeader[11] = (unsigned char)(height() >> 24);

	// Schreiben der beiden header in die Datei
	fwrite(fileHeader, 1, 14, f);
	fwrite(infoHeader, 1, 40, f);

	// Einlesen in die Datei von beginnend von unten nach oben
	for (int i = 0; i < height(); i++) {
		fwrite(img + (width() * (height() - i - 1) * 3), 3, width(), f);
		// Auffüllen der verbliebenen "Rest"Bytes aus dem img-Array
		fwrite(padding, 1, (4 - (width() * 3) % 4) % 4, f);
	}

	free(img);
	fclose(f);

	return true;
}
