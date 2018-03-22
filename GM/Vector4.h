#ifndef _VECTOR4_
#define _VECTOR4_

#include <iostream>

using namespace std;

class Vector4 {
public:
	float x, y, z, h;

	Vector4(float x = 0.0, float y = 0.0, float z = 0.0, float h = 1.0);

	Vector4 operator+(const Vector4 &);
	Vector4 operator-(const Vector4 &);
	Vector4 operator*(const float &);
	Vector4 operator/(const float &);

	Vector4 operator-() const;

	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float r);
	Vector4& operator/=(float r);
	
	Vector4& normalize();

	static float len(const Vector4 &);
	static float len2(const Vector4 &);
	static float innerProduct(const Vector4 &, const Vector4 &);
	static Vector4 crossProduct(const Vector4 &, const Vector4 &);

	friend std::ostream& operator<<(std::ostream& os, const Vector4& obj)
	{
		os << "[" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.h << "]";
		return os;
	}
};

#endif