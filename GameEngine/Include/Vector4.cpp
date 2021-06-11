#include "Vector4.h"
#include "Matrix.h"

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

_tagVector4::_tagVector4(const DirectX::XMVECTOR& v)	:
	x(0.f)
	, y(0.f)
	, z(0.f)
	, w(0.f)
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

_tagVector4& _tagVector4::operator+=(const _tagVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

_tagVector4& _tagVector4::operator+=(const DirectX::XMVECTOR& v)
{
	_tagVector4 _v = v;

	x += _v.x;
	y += _v.y;
	w += _v.w;
	z += _v.z;

	return *this;
}

_tagVector4& _tagVector4::operator+=(float f)
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

_tagVector4& _tagVector4::operator-=(const _tagVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

_tagVector4& _tagVector4::operator-=(const DirectX::XMVECTOR& v)
{
	_tagVector4 _v = v;

	x -= _v.x;
	y -= _v.y;
	w -= _v.w;
	z -= _v.z;

	return *this;
}

_tagVector4& _tagVector4::operator-=(float f)
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

_tagVector4& _tagVector4::operator*=(const _tagVector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

_tagVector4& _tagVector4::operator*=(const DirectX::XMVECTOR& v)
{
	_tagVector4 _v = v;

	x *= _v.x;
	y *= _v.y;
	w *= _v.w;
	z *= _v.z;

	return *this;
}

_tagVector4& _tagVector4::operator*=(float f)
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

_tagVector4& _tagVector4::operator/=(const _tagVector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

_tagVector4& _tagVector4::operator/=(const DirectX::XMVECTOR& v)
{
	_tagVector4 _v = v;

	x /= _v.x;
	y /= _v.y;
	w /= _v.w;
	z /= _v.z;

	return *this;
}

_tagVector4& _tagVector4::operator/=(float f)
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

_tagVector4 _tagVector4::LerpAndNormalize(_tagVector4 p, _tagVector4 q, float s)
{
	_tagVector4 v = (1.f - s) * p + s * q;

	v.Normalize();

	return v;
}

_tagVector4 _tagVector4::Slerp(_tagVector4 p, _tagVector4 q, float s)
{
	if ((p - q).Length() > (p + q).Length())
		q = -q;

	float cosPhi = p.Dot(q);

	if (cosPhi > (1.f - 0.001f))
		return LerpAndNormalize(p, q, s);

	float phi = acosf(cosPhi);

	float sinPhi = sinf(phi);

	return sinf(phi * (1.f - s)) / sinPhi * p + (sinf(phi * s) / sinPhi) * q;
}

void _tagVector4::Normalize()
{
	float fLength = Length();

	x /= fLength;
	y /= fLength;
	z /= fLength;
	w /= fLength;
}

float _tagVector4::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float _tagVector4::Dot(const _tagVector4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

void _tagVector4::QuaternionRotation(const Matrix& rot)
{
	Convert(DirectX::XMQuaternionRotationMatrix(rot.m));
}

_tagVector4 operator*(float f, _tagVector4 v)
{
	return _tagVector4(f * v.x, f * v.y, f * v.z, f * v.w);
}

_tagVector4 operator-(_tagVector4 v)
{
	return _tagVector4(-v.x, -v.y, -v.z, -v.w);
}
