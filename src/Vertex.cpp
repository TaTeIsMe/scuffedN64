#include "Vertex.h"
#include<cmath>
#include<iostream>
Vertex::Vertex() :x(0),y(0),z(0),w(1)
{
}

Vertex::Vertex(float x, float y, float z, float w ) :x(x), y(y), z(z), w(w){

}

float Vertex::length() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vertex::distance(const Vertex& ver1,const Vertex& ver2)
{
	return (ver1 - ver2).length();
}


float Vertex::dotProduct(const Vertex& ver1,const Vertex& ver2)
{
	return ver1.x * ver2.x + ver1.y * ver2.y + ver1.z * ver2.z;
}

Vertex& Vertex::operator-=(const Vertex& vertex)
{
	x -= vertex.x;
	y -= vertex.y;
	z -= vertex.z;
	w -= vertex.w;
	return *this;
}

Vertex& Vertex::operator +=(const Vertex& vertex) {
	x += vertex.x;
	y += vertex.y;
	z += vertex.z;
	w += vertex.w;
	return *this;
};

Vertex& Vertex::operator *=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
};

Vertex& Vertex::operator *=(const Matrix& transformation) {
	if (transformation.getHeight() != 4 || transformation.getWidth() != 4) {
		std::cerr << "Vertex can only be multiplied by 4x4 matrix";
	}
	float tempx = x;
	float tempy = y;
	float tempz = z;
	float tempw = w;
	x = transformation[0][0] * tempx + transformation[0][1] * tempy + transformation[0][2] * tempz + transformation[0][3] * tempw;
	y = transformation[1][0] * tempx + transformation[1][1] * tempy + transformation[1][2] * tempz + transformation[1][3] * tempw;
	z = transformation[2][0] * tempx + transformation[2][1] * tempy + transformation[2][2] * tempz + transformation[2][3] * tempw;
	w = transformation[3][0] * tempx + transformation[3][1] * tempy + transformation[3][2] * tempz + transformation[3][3] * tempw;
	return *this;
}

const Vertex operator *(const Vertex& lhs, float rhs) {
	return Vertex(lhs) *= rhs;
};

const Vertex operator *(float lhs, const Vertex& rhs) {
	return Vertex(rhs) *= lhs;
};

const Vertex operator *(const Vertex& lhs, const Matrix& rhs) {
	return Vertex(lhs) *= rhs;
};

const Vertex operator +(const Vertex& lhs, const Vertex& rhs) {
	return Vertex(lhs) += rhs;
};

const Vertex operator -(const Vertex& lhs, const Vertex& rhs) {
	return Vertex(lhs) -= rhs;
};

Vertex Vertex::crossProduct(const Vertex& ver1,const  Vertex& ver2)
{
	return Vertex((ver1.y * ver2.z - ver1.z * ver2.y), -(ver1.x * ver2.z - ver1.z * ver2.x), (ver1.x * ver2.y - ver1.y * ver2.x));
}

Vertex Vertex::crossProduct(const Vertex& ver2) const
{
	return crossProduct(*this, ver2);
}

Vertex Vertex::normalized() const
{
	if (length() == 0)return Vertex(this->x, this->y, this->z);
	return Vertex(x / length(), y / length(), z / length(),w/length());
}