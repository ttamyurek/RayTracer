#include "Primitive.h"
#include "HitData.h"
#include "Object.h"

/*float determinant(const Vector &vec1, const Vector &vec2, const Vector &vec3)
{
	return (
		vec1.x * vec2.y * vec3.z
		+ vec2.x * vec3.y * vec1.z
		+ vec3.x * vec1.y * vec2.z
		- vec3.x * vec2.y * vec1.z
		- vec2.x * vec1.y * vec3.z
		- vec1.x * vec3.y * vec2.z);
}*/

bool Triangle::Intersect(const Ray &ray, HitData &hitData)
{
	const float EPSILON = 0.00001;
    
    Vector v01 = v1 - v0;
    Vector v02 = v2 - v0;
    Vector pvec = cross(ray.direction(), v02);
    float det = dot(v01, pvec);

	/*TODO: Delete this block later
	float detA = determinant(-v01, -v02, ray.direction());
	auto beta = determinant(v0-ray.origin(), -v02, ray.direction()) / detA;
	auto gamma = determinant(-v01, v0 - ray.origin(), ray.direction()) / detA;
	auto param = determinant(-v01, -v02, v0 - ray.origin()) / detA;
	auto alpha = 1 - beta - gamma;
	if (ray.direction().x > 0.99)
		int debug = 0;*/

    if( doubleSided && abs(det) < EPSILON) return false;
    if( !doubleSided && det < EPSILON) return false;
    
    Vector tvec  = ray.origin() - v0;
    float u = dot(tvec, pvec) / det;
    if( u < 0.0f || u > 1.0f) return false;
    
    Vector qvec = cross(tvec, v01);
    float v = dot(ray.direction(), qvec) / det;
    if(v < 0 || u + v > 1) return false;
    
    float t = dot(v02, qvec) / det;
    if( t > 0.0 )
    {
        float w = 1.0 - u - v;
		hitData.hit = true;
		hitData.position = ray.at(t);
		hitData.normal = normalize(n0 * w + n1 * u + n2 * v);
		hitData.t = t;

		hitData.uv.u = uv0.u * w + uv1.u * u + uv2.u * v;
		hitData.uv.v = uv0.v * w + uv1.v * u + uv2.v * v;
		hitData.material = parent->material;
		hitData.ray = ray;
		hitData.primitive = this;
        return true;
    } else return false;
    
    
}

bool Sphere::Intersect(const Ray &ray, HitData &hitData)
{
	return false;
}