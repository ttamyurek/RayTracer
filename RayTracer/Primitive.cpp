#include "Primitive.h"
#include "Object.h"

bool Triangle::Intersect(const Ray &ray, HitData &hitData)
{
	const float EPSILON = 0.00001;
    
    Vector v10 = v1 - v0;
    Vector v02 = v0 - v2;
    Vector pvec = cross(ray.direction(), v02);
    float det = dot(v10, pvec);
    
    if( doubleSided && abs(det) < EPSILON) return false;
    if( !doubleSided && det < EPSILON) return false;
    
    Vector tvec  = ray.origin() - v0;
    float u = dot(tvec, pvec) / det;
    if( u < 0.0f || u > 1.0f) return false;
    
    Vector qvec = cross(tvec, v10);
    float v = dot(ray.direction(), qvec) / det;
    if(v < 0 || u + v > 1) return false;
    
    float t = dot(v02, qvec) / det;
    if( t > 0.0 )
    {
        float w = 1.0 - u - v;
		Vector intersection = ray.at(t);
        Vector normal = normalize(n0 * w + n1 * u + n2 * v);
        UV uv;
        uv.u = uv0.u * w + uv1.u * u + uv2.u * v;
        uv.v = uv0.v * w + uv1.v * u + uv2.v * v;
        hitData = HitData(true, intersection, normal, t, uv, parent->material, ray, this);
        return true;
    } else return false;
    
    
}
