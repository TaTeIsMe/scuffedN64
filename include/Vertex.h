#pragma once
#include "Matrix.h"
struct Vertex
{
	float x;
	float y;
	float z;
	float w;
	Vertex();
	Vertex(float x, float y, float z, float w = 1);
	float length()const;
	static float distance(const Vertex& ver1, const Vertex& ver2);
	static Vertex crossProduct(const Vertex& ver1,const Vertex& ver2);
	static float dotProduct(const Vertex& ver1,const Vertex& ver2);
	Vertex crossProduct(const Vertex& ver2)const;
	Vertex normalized()const;
	Vertex& operator *=(float scalar);
	Vertex& operator *=(const Matrix& transformation);
	Vertex& operator +=(const Vertex& vertex);
	Vertex& operator -=(const Vertex& vertex);
	friend const Vertex operator *(const Vertex& lhs,float rhs);
	friend const Vertex operator *(float lhs, const Vertex& rhs);
	friend const Vertex operator *(const Vertex& lhs, const Matrix& rhs);
	friend const Vertex operator +(const Vertex& lhs, const Vertex& rhs);
	friend const Vertex operator -(const Vertex& lhs, const Vertex& rhs);
};
