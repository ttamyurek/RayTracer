#pragma once
#include <vector>
#include "Vector.h"

typedef unsigned char uint8;

class Image {
private:
	std::vector<uint8> data;
public:
	int imageWidth;
	int imageHeight;

	Image(int imageWidth, int imageHeight);
	Vector getPixel(int row, int col);
	void setPixel(int row, int col, Vector color);
	void save(const char *filename);

};