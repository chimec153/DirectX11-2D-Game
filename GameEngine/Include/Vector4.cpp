#include "Vector4.h"

_tagVector4 _tagVector4::White(1.f, 1.f, 1.f, 1.f);
_tagVector4 _tagVector4::Black(0.f, 0.f, 0.f, 1.f);
_tagVector4 _tagVector4::Red(1.f, 0.f, 0.f, 1.f);
_tagVector4 _tagVector4::Green(0.f, 1.f, 0.f, 1.f);
_tagVector4 _tagVector4::Blue(0.f, 0.f, 1.f, 1.f);

_tagVector4::_tagVector4()	:
	x(0.f),
	y(0.f),
	z(0.f),
	w(0.f)
{
}

_tagVector4::_tagVector4(const _tagVector4& v)	:
	x(v.x),
	y(v.y),
	z(v.z),
	w(v.w)
{
}

_tagVector4::_tagVector4(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}

_tagVector4::_tagVector4(float x, float y, float z, float w)	:
	x(x),
	y(y),
	z(z),
	w(w)
{
}

float _tagVector4::operator[](const int& iIdx) const
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}

	assert(false);

	return -1;
}

float& _tagVector4::operator[](const int& iIdx)
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}

	assert(false);

	return w;
}

_tagVector4& _tagVector4::operator=(const _tagVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;

	return *this;
}

_tagVector4& _tagVector4::operator=(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);

	return *this;
}

_tagVector4& _tagVector4::operator=(float f)
{
	x = f;
	y = f;
	z = f;
	w = f;

	return *this;
}

bool _tagVector4::operator==(const _tagVector4& v) const
{
	return x == v.x && y==v.y && z == v.z && w == v.w;
}

bool _tagVector4::operator==(const DirectX::XMVECTOR& v) const
{
	_tagVector4 v1(v);

	return v1.x == x && v1.y == y && v1.z == z && v1.w ==w;
}

bool _tagVector4::operator!=(const _tagVector4& v) const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool _tagVector4::operator!=(const DirectX::XMVECTOR& v) const
{
	_tagVector4 v1(v);

	return v1.x != x || v1.y != y || v1.z != z || v1.w != w;
}

_tagVector4 _tagVector4::operator+(const _tagVector4& v) const
{
	return _tagVector4(x + v.x, y + v.y, z+ v.z, w+v.w);
}

_tagVector4 _tagVector4::operator+(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector4(v1 + v);
}

_tagVector4 _tagVector4::operator+(float f) const
{
	return _tagVector4(x + f, y + f, z + f, w + f);
}

_tagVector4 _tagVector4::operator+=(const _tagVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

_tagVector4 _tagVector4::operator+=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 += v;

	Convert(v1);

	return _tagVector4(v1);
}

_tagVector4 _tagVector4::operator+=(float f)
{
	x += f;
	y += f;
	z += f;
	w += f;

	return *this;
}

_tagVector4 _tagVector4::operator-(const _tagVector4& v) const
{
	return _tagVector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

_tagVector4 _tagVector4::operator-(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector4(v1 - v);
}

_tagVector4 _tagVector4::operator-(float f) const
{
	return _tagVector4(x - f, y - f, z - f, w - f);
}

_tagVector4 _tagVector4::operator-=(const _tagVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

_tagVector4 _tagVector4::operator-=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 -= v;

	Convert(v1);

	return _tagVector4(v1);
}

_tagVector4 _tagVector4::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;

	return *this;
}

_tagVector4 _tagVector4::operator*(const _tagVector4& v) const
{
	return _tagVector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

_tagVector4 _tagVector4::operator*(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector4(v1 * v);
}

_tagVector4 _tagVector4::operator*(float f) const
{
	return _tagVector4(x * f, y * f, z * f, w * f);
}

_tagVector4 _tagVector4::operator*=(const _tagVector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

_tagVector4 _tagVector4::operator*=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 *= v;

	Convert(v1);

	return _tagVector4(v1);
}

_tagVector4 _tagVector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;

	return *this;
}

_tagVector4 _tagVector4::operator/(const _tagVector4& v) const
{
	return _tagVector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

_tagVector4 _tagVector4::operator/(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector4(v1 / v);
}

_tagVector4 _tagVector4::operator/(float f) const
{
	return _tagVector4(x / f, y / f, z / f, w / f);
}

_tagVector4 _tagVector4::operator/=(const _tagVector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

_tagVector4 _tagVector4::operator/=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 /= v;

	Convert(v1);

	return _tagVector4(v1);
}

_tagVector4 _tagVector4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;

	return *this;
}

void _tagVector4::Convert(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}

DirectX::XMVECTOR _tagVector4::Convert() const
{
	return DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
}
