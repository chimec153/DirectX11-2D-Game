#pragma once

#include "Matrix.h"

typedef struct __declspec(align(16)) _tagVector3
{
	float x, y, z;

	_tagVector3();
	_tagVector3(const _tagVector3& v);
	_tagVector3(const DirectX::XMVECTOR& v);
	_tagVector3(float x, float y, float z);

	float operator[] (const int& iIdx)	const;
	float& operator[] (const int& iIdx);

	// operator =
	_tagVector3& operator = (const _tagVector3& v);
	_tagVector3& operator = (const DirectX::XMVECTOR& v);
	_tagVector3& operator = (float f);

	// operator ==
	bool operator == (const _tagVector3& v)	const;
	bool operator == (const DirectX::XMVECTOR& v)	const;

	bool operator != (const _tagVector3& v)	const;
	bool operator != (const DirectX::XMVECTOR& v)	const;

	//operator +
	_tagVector3 operator+ (const _tagVector3& v)	const;
	_tagVector3 operator+ (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator+ (float f)	const;

	//operator +=
	_tagVector3 operator+= (const _tagVector3& v);
	_tagVector3 operator+= (const DirectX::XMVECTOR& v);
	_tagVector3 operator+= (float f);

	//operator -
	_tagVector3 operator- (const _tagVector3& v)	const;
	_tagVector3 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator- (float f)	const;

	//operator -=
	_tagVector3 operator-= (const _tagVector3& v);
	_tagVector3 operator-= (const DirectX::XMVECTOR& v);
	_tagVector3 operator-= (float f);

	//operator *
	_tagVector3 operator* (const _tagVector3& v)	const;
	_tagVector3 operator* (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator* (float f)	const;

	//operator *=
	_tagVector3 operator*= (const _tagVector3& v);
	_tagVector3 operator*= (const DirectX::XMVECTOR& v);
	_tagVector3 operator*= (float f);

	//operator /
	_tagVector3 operator/ (const _tagVector3& v)	const;
	_tagVector3 operator/ (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator/ (float f)	const;

	//operator /=
	_tagVector3 operator/= (const _tagVector3& v);
	_tagVector3 operator/= (const DirectX::XMVECTOR& v);
	_tagVector3 operator/= (float f);

	void Convert(const DirectX::XMVECTOR& v);
	DirectX::XMVECTOR Convert()	const;

	float Length()	const;
	float Dot(const _tagVector3& v)	const;
	void Normalize();
	float Distance(const _tagVector3& v)	const;
	_tagVector3 Cross(const _tagVector3& v)	const;

	void TransformNormal(const DirectX::XMMATRIX& m);
	void TransformCoord(const DirectX::XMMATRIX& m);

	static _tagVector3 One;
	static _tagVector3 Zero;
	static _tagVector3 Axis[3];
}Vector3, *PVector3;

