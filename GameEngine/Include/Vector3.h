#pragma once

#include "Matrix.h"

typedef __declspec(dllexport) union _tagVector3
{
	struct
	{
		float x, y, z;
	};

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

	_tagVector3& operator++();
	_tagVector3 operator++(int i);

	//operator +=
	_tagVector3 operator+= (const _tagVector3& v);
	_tagVector3 operator+= (const DirectX::XMVECTOR& v);
	_tagVector3 operator+= (float f);

	//operator -
	_tagVector3 operator- (const _tagVector3& v)	const;
	_tagVector3 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector3 operator- (float f)	const;

	_tagVector3 operator- ();

	_tagVector3& operator--();
	_tagVector3 operator--(int i);

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

	_tagVector3 TransformNormal(const Matrix& m);
	_tagVector3 TransformCoord(const Matrix& m);

	static _tagVector3 One;
	static _tagVector3 Zero;
	static _tagVector3 Axis[(int)WORLD_AXIS::AXIS_END];
}Vector3, *PVector3;

_tagVector3 operator * (float f, const _tagVector3& v);
_tagVector3 operator * (const DirectX::XMFLOAT3& v1, const _tagVector3& v2);

_tagVector3 operator / (float f, const _tagVector3& v);
_tagVector3 operator / (const DirectX::XMFLOAT3& v1, const _tagVector3& v2);