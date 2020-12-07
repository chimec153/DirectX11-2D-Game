#pragma once

#include "EngineMath.h"

typedef union __declspec(dllexport) _tagVector2
{
	struct
	{
		float x, y;
	};

	_tagVector2();
	_tagVector2(const _tagVector2& v);
	_tagVector2(const DirectX::XMVECTOR& v);
	_tagVector2(const POINT& pt);
	_tagVector2(float x, float y);

	float operator[] (const int& iIdx)	const;
	float& operator[] (const int& iIdx);

	// operator =
	_tagVector2& operator = (const _tagVector2& v);
	_tagVector2& operator = (const DirectX::XMVECTOR& v);
	_tagVector2& operator = (const POINT& pt);
	_tagVector2& operator = (float f[2]);
	_tagVector2& operator = (float f);

	// operator ==
	bool operator == (const _tagVector2& v)	const;
	bool operator == (const DirectX::XMVECTOR& v)	const;

	bool operator != (const _tagVector2& v)	const;
	bool operator != (const DirectX::XMVECTOR& v)	const;

	//operator +
	_tagVector2 operator+ (const _tagVector2& v)	const;
	_tagVector2 operator+ (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator+ (const POINT& pt);
	_tagVector2 operator+ (float f[2]);
	_tagVector2 operator+ (float f)	const;

	//operator +=
	_tagVector2 operator+= (const _tagVector2& v);
	_tagVector2 operator+= (const DirectX::XMVECTOR& v);
	_tagVector2 operator+= (const POINT& pt);
	_tagVector2 operator+= (float f[2]);
	_tagVector2 operator+= (float f);

	//operator -
	_tagVector2 operator- (const _tagVector2& v)	const;
	_tagVector2 operator- (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator- (const POINT& pt);
	_tagVector2 operator- (float f[2]);
	_tagVector2 operator- (float f)	const;

	//operator -=
	_tagVector2 operator-= (const _tagVector2& v);
	_tagVector2 operator-= (const DirectX::XMVECTOR& v);
	_tagVector2 operator-= (const POINT& pt);
	_tagVector2 operator-= (float f[2]);
	_tagVector2 operator-= (float f);

	//operator *
	_tagVector2 operator* (const _tagVector2& v)	const;
	_tagVector2 operator* (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator* (const POINT& pt);
	_tagVector2 operator* (float f[2]);
	_tagVector2 operator* (float f)	const;

	//operator *=
	_tagVector2 operator*= (const _tagVector2& v);
	_tagVector2 operator*= (const DirectX::XMVECTOR& v);
	_tagVector2 operator*= (const POINT& pt);
	_tagVector2 operator*= (float f[2]);
	_tagVector2 operator*= (float f);

	//operator /
	_tagVector2 operator/ (const _tagVector2& v)	const;
	_tagVector2 operator/ (const DirectX::XMVECTOR& v)	const;
	_tagVector2 operator/ (const POINT& pt);
	_tagVector2 operator/ (float f[2]);
	_tagVector2 operator/ (float f)	const;

	//operator /=
	_tagVector2 operator/= (const _tagVector2& v);
	_tagVector2 operator/= (const DirectX::XMVECTOR& v);
	_tagVector2 operator/= (const POINT& pt);
	_tagVector2 operator/= (float f[2]);
	_tagVector2 operator/= (float f);

	void Convert(const DirectX::XMVECTOR& v);
	DirectX::XMVECTOR Convert()	const;

	float Length()	const;
	float Dot(const _tagVector2& v)	const;
	float Angle(const _tagVector2& v)	const;
	float AngleAxis()	const;
	void Normalize();
	float Distance(const _tagVector2& v)	const;

}Vector2, *PVector2;

