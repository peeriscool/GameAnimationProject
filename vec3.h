#ifndef _H_VEC3_
#define _H_VEC3_
struct vec3 {
	 union {
		struct {
			float x;
			float y;
			float z;
			};
			float v[3];
	 };
	 inline vec3() : x(0.0f), y(0.0f), z(0.0f) { }
	 inline vec3(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) { }
	 inline vec3(float *fv) :
		 x(fv[0]), y(fv[1]), z(fv[2]) { }

	 };

		vec3 operator+(const vec3& l, const vec3 &r);
		vec3 operator-(const vec3& l, const vec3 &r);
		vec3 operator*(const vec3& l, const vec3 &r);
		vec3 operator*(const vec3& v, float f);
		bool operator==(const vec3& l, const vec3& r);
		bool operator!=(const vec3& l, const vec3& r);
		float dot(const vec3 &l, const vec3 &r);

		float lenSq(const vec3& v);
		float len(const vec3& v);

		void normalize(vec3& v);
		vec3 normalized(const vec3& v);
		float angle(const vec3& l, const vec3& r);
		vec3 project(const vec3& a, const vec3& b);
		vec3 reject(const vec3& a, const vec3& b);
		vec3 reflect(const vec3& a, const vec3& b);
		vec3 cross(const vec3& l, const vec3& r);
		vec3 lerp(const vec3& s, const vec3& e, float t);
		vec3 slerp(const vec3& s, const vec3& e, float t);
		vec3 nlerp(const vec3& s, const vec3& e, float t);

		inline float degreesToRadians(float degrees);
		vec3 toRadians(const vec3& degreesVec);
#endif #pragma once
		//should use globals instead of local definition
#define VEC3_EPSILON 0.000001f
#define VEC3_TODEGREES 57.2958f
#define VEC3_TORADIANS 0.0174533f
#define PI 3.141592653589793 //should make a global definition
