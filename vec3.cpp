#include "vec3.h"
#include <math.h>

vec3 operator+(const vec3 &l, const vec3 &r)			//plus
{
	return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

vec3 operator-(const vec3 &l, const vec3 &r)			//minus
{
	return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

vec3 operator*(const vec3 &v, float f)					//scale
{
	return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator*(const vec3 &l, const vec3 &r)			//multiply
{
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}
bool operator==(const vec3& l, const vec3& r)			//equal
{
	vec3 diff(l - r);
	return lenSq(diff) < VEC3_EPSILON;
}
bool operator!=(const vec3& l, const vec3& r)			//Nequal
{
	return !(l == r);
}

float dot(const vec3 &l, const vec3 &r)					//dotproduct -1/1
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}
float lenSq(const vec3& v)								//square length
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
float len(const vec3& v)								//vector length
{
	float lensq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lensq < VEC3_EPSILON){ return 0.0f;	}
	return sqrtf(lensq);	
}
														//unit vectors
void normalize(vec3& v)									//returns vector normalized
{
	float lensq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lensq < VEC3_EPSILON) { return; }
	float invLen = 1.0f / sqrtf(lensq);
	v.x *= invLen;
	v.y *= invLen;
	v.z *= invLen;

}
vec3 normalized(const vec3 &v)							//makes a new normalized vector
{
	float lensq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lensq < VEC3_EPSILON) { return v; }
	float invLen = 1.0f / sqrtf(lensq);
	return vec3(
		v.x * invLen,
		v.y * invLen,
		v.z * invLen);
}
float angle(const vec3& l, const vec3& r)				//angle bewtween 2 vec3
{
	float sqMagL = l.x * l.x + l.y * l.y + l.z * l.z;
	float sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;

	if (sqMagL < VEC3_EPSILON || sqMagR < VEC3_EPSILON) { return 0.0f; }
	float dot = l.x * r.x + l.y * r.y + l.z * r.z;
	float len = sqrtf(sqMagL) * sqrtf(sqMagR);
	return acosf(dot / len);
}

vec3 project(const vec3& a, const vec3& b)				// calculates parallel and perpenducular components using
{
	float magBSQ = len(b);
	if (magBSQ < VEC3_EPSILON) { return vec3(); }
	float scale = dot(a, b) / magBSQ;
	return b * scale;
}
vec3 reject(const vec3& a, const vec3& b)				//oppesite of project
{
	vec3 projection = project(a, b);
	return a - projection;
}
vec3 reflect(const vec3& a, const vec3& b)				//mirror / bounce
{
	float magBSq = len(b);
	if (magBSq < VEC3_EPSILON) {return vec3();}
	float scale = dot(a, b) / magBSq;
	vec3 proj2 = b * (scale * 2);
	return a - proj2;
}

vec3 cross(const vec3& l, const vec3& r)				//cross product
{
	return vec3
	(
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	);
}
vec3 lerp(const vec3& s, const vec3& e, float t)		//(linear interpolation) takes shortest path between vectors
{
	return vec3(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
}
vec3 slerp(const vec3& s, const vec3& e, float t)		//spherical linear interpolation  //has a constant velocity interpolation
{
	if (t < 0.01f) {return lerp(s, e, t);}
	vec3 from = normalized(s);
	vec3 to = normalized(e);
	float theta = angle(from, to);
	float sin_theta = sinf(theta);
	float a = sinf((1.0f - t) * theta) / sin_theta;
	float b = sinf(t * theta) / sin_theta;
	return from * a + to * b;
}
vec3 nlerp(const vec3& s, const vec3& e, float t)		//cheaper, close approx of slerp 
{
	vec3 linear
	(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
	return normalized(linear);
}

inline float degreesToRadians(float degrees) {
	return degrees * static_cast<float>(PI) / 180.0f;
}

// Function to convert vec3 from degrees to radians
vec3 toRadians(const vec3& degreesVec) {
	return vec3(
		degreesToRadians(degreesVec.x),
		degreesToRadians(degreesVec.y),
		degreesToRadians(degreesVec.z)
	);
}