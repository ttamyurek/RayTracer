#include "Primitive.h"

bool Triangle::Intersect(const Ray &ray, HitData &hitData)
{
	const float EPSILON = 0.00001;
    
    Vector v10 = v1 - v0;
    Vector v02 = v0 - v2;
    Vector pvec = cross(ray.direction, v02);
    float det = dot(v10, pvec);
    
    if( doubleSided && abs(det) < kEpsilon ) return false;
    if( !doubleSided && det < kEpsilon ) return false;
    
    Vector tvec  = ray.origin - v0;
    float u = dot(tvec, pvec) / det;
    if( u < 0.0f || u > 1.0f) return false;
    
    Vector qvec = cross(tvec, v10);
    float v = dot(ray.direction, qvec) / det;
    if(v < 0 || u + v > 1) return false;
    
    float t = dot(v02, qvec) / det;
    if( t > 0.0 )
    {
        w = 1.0 - u - v;
        Vector intersection = ray.origin + ray.direction * t;
        Vector normal = normalize(w * n0 + u * n1 + v * n2);
        UV uv;
        uv.u = w * uv0.u + u * uv1.u + v * uv2.u;
        uv.v = w * uv0.v + u * uv1.v + v * uv2.v;
        
        return true;
    } else return false;
    
    
}
