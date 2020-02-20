#include "Vector.h"

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

	Camera(const Vector position, const Vector viewDir, const Vector upDir, const float verticalFOV, const int imageWidth, const int imageHeight)
	{
		this->position = position; // E
		this->viewDir = viewDir.normalize();
		this->upDir = upDir.normalize();
		this->verticalFOV = verticalFOV;

		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->aspectRatio = imageWidth / imageHeight;

		this->rightDir = viewDir.cross(upDir).normalize(); // A = V x U
		this->upDir = rightDir.cross(viewDir).normalize(); // B = A x V

		this->center = position + this->viewDir * focalDistance; // M = E + c * V
		this->vertical = this->upDir * focalDistance * tan(verticalFOV / 2.0f); // Y = c * tan(FOV / 2) * B
		this->horizontal = rightDir * focalDistance * tan(verticalFOV * aspectRatio / 2.0f); // X = c * tan( FOV * (w/h) / 2 ) * A

		/*
		float sx = (i + 0.5) / IMAGE_WIDTH;
			float sy = (j + 0.5) / IMAGE_HEIGHT;

			glm::vec3 Point = center + (2.0f*sx - 1.0f) * horizontal + (2.0f*sy - 1.0f) * vertical;
			glm::vec3 RayDirection = glm::normalize(Point - camPos);
			//RayTrace(Point*/
	}
};