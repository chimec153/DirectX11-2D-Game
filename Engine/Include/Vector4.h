#pragma once

#include "EngineMath.h"

typedef __declspec(dllexport) union _tagVector4
{
	struct
	{
		float x, y, z, w;
	};

	_tagVector4();
	_tagVector4(const _tagVector4& v);
	_tagVector4(const DirectX::XMVECTOR& v);
	_tagVector4(float x, float y, float z, float w);

	float operator[] (const int& iIdx)	const;
	float& operator[] (const int& iIdx);

	// operator =
	_tagVector4& operator = (const _tagVector4& v);
	_tagVector4& operator = (const DirectX::XMVECTOR& v);
	_tagVector4& operator = (float f);

	// operator ==
	bool operator == (const _tagVector4& v)	const;
	bool operator == (const DirectX::XMVECTOR& v)	const;

	bool operator != (const _tagVector4& v)	const;
	bool operator != (const DirectX::XMVECTOR& v)	const;

	//operator +
	_tagVector4 operator+ (const _tagVector4& v)	const;
	_tagVector4 operator+ (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator+ (float f)	const;

	//operator +=
	_tagVector4& operator+= (const _tagVector4& v);
	_tagVector4& operator+= (const DirectX::XMVECTOR& v);
	_tagVector4& operator+= (float f);

	//operator -
	_tagVector4 operator- (const _tagVector4& v)	const;
	_tagVector4 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator- (float f)	const;

	//operator -=
	_tagVector4& operator-= (const _tagVector4& v);
	_tagVector4& operator-= (const DirectX::XMVECTOR& v);
	_tagVector4& operator-= (float f);

	//operator *
	_tagVector4 operator* (const _tagVector4& v)	const;
	_tagVector4 operator* (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator* (float f)	const;

	//operator *=
	_tagVector4& operator*= (const _tagVector4& v);
	_tagVector4& operator*= (const DirectX::XMVECTOR& v);
	_tagVector4& operator*= (float f);

	//operator /
	_tagVector4 operator/ (const _tagVector4& v)	const;
	_tagVector4 operator/ (const DirectX::XMVECTOR& v)	const;
	_tagVector4 operator/ (float f)	const;

	//operator /=
	_tagVector4& operator/= (const _tagVector4& v);
	_tagVector4& operator/= (const DirectX::XMVECTOR& v);
	_tagVector4& operator/= (float f);

	void Convert(const DirectX::XMVECTOR& v);
	DirectX::XMVECTOR Convert()	const;

	static _tagVector4 LerpAndNormalize(_tagVector4 p, _tagVector4 q, float s);
	static _tagVector4 Slerp(_tagVector4 p, _tagVector4 q, float s);
	void Normalize();
	float Length()	const;
	float Dot(const _tagVector4& v) const;
	void QuaternionRotation(const union _tagMatrix& rot);

	static _tagVector4 White;
	static _tagVector4 Black;
	static _tagVector4 Red;
	static _tagVector4 Green;
	static _tagVector4 Blue;

}Vector4, *PVector4;

_tagVector4 operator * (float f, _tagVector4 v);
_tagVector4 operator - (_tagVector4 v);