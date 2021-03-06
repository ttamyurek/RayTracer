#pragma once
#include "Vector.h"
#include "Ray.h"
#include "RandomGenerator.h"

class Camera {
private:
	Vector center; // M
	Vector vertical; // Y
	Vector horizontal; // X

public:
	Vector position; // E
	Vector viewDir; // V
	Vector upDir; // U
	Vector rightDir; //A

	float focalDistance; /* Distance from viewer to focal plane	*/
	float verticalFOV; /* Total vertical field of view, in radians	*/

	int imageWidth;
	int imageHeight;

	float aspectRatio;

	Camera()
	{}

	Camera(const Vector position, const Vector viewDir, const Vector upDir, const float focalDistance, const float verticalFOV, const int imageWidth, const int imageHeight)
	{
		this->position = position; // E
		this->viewDir = viewDir.normalize();
		this->upDir = upDir.normalize();
		this->focalDistance = focalDistance;
		this->verticalFOV = verticalFOV;

		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->aspectRatio = (float)imageWidth / imageHeight;

		this->rightDir = this->viewDir.cross(upDir).normalize(); // A = V x U
		this->upDir = rightDir.cross(this->viewDir).normalize(); // B = A x V

		this->center = position + this->viewDir * focalDistance; // M = E + c * V
		this->vertical = this->upDir * focalDistance * tan(verticalFOV / 2.0f); // Y = c * tan(FOV / 2) * B
		this->horizontal = this->rightDir * focalDistance * tan(verticalFOV * aspectRatio / 2.0f); // X = c * tan( FOV * (w/h) / 2 ) * A

	}

	Ray shootRay(int row, int col)
	{
		float sx = (col + 0.5) / imageWidth;
		float sy = (row + 0.5) / imageHeight;

		Vector point = center + horizontal * (2.0f * sx - 1.0f) + vertical * (2.0f * sy - 1.0f);  // P = M + (2 * sx - 1) * X + (2 * sy - 1) * Y
		Vector rayDirection = (point - position).normalize(); // P - E

		return Ray(position, rayDirection);
	}

	Ray shootJitteredRay(int row, int col, float level, float offset)
	{
		float sx = (col + 0.5 + randf(-level / 2, level / 2) + offset) / imageWidth;
		float sy = (row + 0.5 + randf(-level / 2, level / 2) + offset) / imageHeight;

		Vector point = center + horizontal * (2.0f * sx - 1.0f) + vertical * (2.0f * sy - 1.0f);  // P = M + (2 * sx - 1) * X + (2 * sy - 1) * Y
		Vector rayDirection = (point - position).normalize(); // P - E

		return Ray(position, rayDirection);
	}

	bool IntersectImagePlane(Vector point, int& pixelX, int& pixelY)
	{
		Vector rayDir(point - position);
		Vector horizontalOffset = rightDir * dot(rightDir, rayDir) + horizontal;
		Vector verticalOffset = upDir * dot(upDir, rayDir) + vertical;
		pixelX = horizontalOffset.length() * (imageWidth / 2) / horizontal.length();
		pixelY = verticalOffset.length() * (imageHeight / 2) / vertical.length();
		if (pixelX > imageWidth || pixelX < 0 || pixelY > imageHeight || pixelY < 0)
		{
			return true;
		}
		else false;
	}
};