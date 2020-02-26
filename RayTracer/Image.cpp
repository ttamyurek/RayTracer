#include "Image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(int width, int height)
{
	imageWidth = width;
	imageHeight = height;
	data.resize(width * height * 3);
}

Vector Image::getPixel(int row, int col)
{
	Vector pixel;
	pixel.x = data[3 * (imageWidth * (imageHeight - row - 1) + col) + 0] / 255.0f;
	pixel.y = data[3 * (imageWidth * (imageHeight - row - 1) + col) + 1] / 255.0f;
	pixel.z = data[3 * (imageWidth * (imageHeight - row - 1) + col) + 2] / 255.0f;
	return pixel;
}

void Image::setPixel(int row, int col, Vector color)
{
	//TODO: Exposure control
	color = color / color.Max();
	data[3 * (imageWidth * (imageHeight - row - 1) + col) + 0] = (uint8) (color.x * 255.0);
	data[3 * (imageWidth * (imageHeight - row - 1) + col) + 1] = (uint8) (color.y * 255.0);
	data[3 * (imageWidth * (imageHeight - row - 1) + col) + 2] = (uint8) (color.z * 255.0);
}

void Image::save(const char *filename)
{
	int stb_return = stbi_write_bmp(filename, imageWidth, imageHeight, 3, data.data());
}