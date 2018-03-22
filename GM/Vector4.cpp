#include "Vector4.h"


Vector4::Vector4(float _x, float _y, float _z, float _h) :
	x(_x), y(_y), z(_z), h(_h)
{}

Vector4 Vector4::operator+(const Vector4 & other) {
	return Vector4(x + other.x, y + other.y, z + other.z, h);
}

Vector4 Vector4::operator-(const Vector4 & other) {
	return Vector4(x - other.x, y - other.y, z - other.z, h);
}

Vector4 Vector4::operator*(const float & r) {
	return Vector4(x * r, y * r, z * r, h);
}

Vector4 Vector4::operator/(const float & r) {
	return Vector4(x / r, y / r, z / r, h );
}

Vector4 Vector4::operator-() const {
	return Vector4(-x, -y, -z, h);
}

Vector4& Vector4::operator+=(const Vector4& other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vector4& Vector4::operator-=(const Vector4& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vector4& Vector4::operator*=(float r) {
	x *= r;
	y *= r;
	z *= r;

	return *this;
}

Vector4& Vector4::operator/=(float r) {
	x /= r;
	y /= r;
	z /= r;

	return *this;
}

Vector4& Vector4::normalize() {
	float length = Vector4::len(*this);

	x /= length;
	y /= length;
	z /= length;

	return *this;
}

float Vector4::len(const Vector4 & v) {
	return std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Vector4::len2(const Vector4 & v) {
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

float Vector4::innerProduct(const Vector4 & l, const Vector4 & r) {
	return (l.x * r.x) + (l.y * r.y) + (l.z * r.z);
}

Vector4 Vector4::crossProduct(const Vector4 & l, const Vector4 & r) {
	return Vector4(l.y * r.z - l.z * r.y, 
				   l.z * r.x -l.x * r.z, 
				   l.x * r.y - l.y * r.x, 
				   1);
}