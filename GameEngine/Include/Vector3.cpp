#include "Vector3.h"

_tagVector3 _tagVector3::One(1.f, 1.f, 1.f);
_tagVector3 _tagVector3::Zero(0.f, 0.f, 0.f);
_tagVector3 _tagVector3::Axis[(int)WORLD_AXIS::AXIS_END] =
{
	_tagVector3(1.f, 0.f, 0.f),
	_tagVector3(0.f, 1.f, 0.f),
	_tagVector3(0.f, 0.f, 1.f)
};

_tagVector3::_tagVector3()	:
	x(0.f),
	y(0.f),
	z(0.f)
{
}

_tagVector3::_tagVector3(const _tagVector3& v)	:
	x(v.x),
	y(v.y),
	z(v.z)
{
}

_tagVector3::_tagVector3(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

_tagVector3::_tagVector3(float x, float y, float z)	:
	x(x),
	y(y),
	z(z)
{
}

float _tagVector3::operator[](const int& iIdx) const
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	assert(false);

	return -1;
}

float& _tagVector3::operator[](const int& iIdx)
{
	switch (iIdx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}

	assert(false);

	return z;
}

_tagVector3& _tagVector3::operator=(const _tagVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

_tagVector3& _tagVector3::operator=(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);

	return *this;
}

_tagVector3& _tagVector3::operator=(float f)
{
	x = f;
	y = f;
	z = f;

	return *this;
}

bool _tagVector3::operator==(const _tagVector3& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

bool _tagVector3::operator==(const DirectX::XMVECTOR& v) const
{
	_tagVector3 v1(v);

	return v1.x == x || v1.y == y || v1.z == z;
}

bool _tagVector3::operator!=(const _tagVector3& v) const
{
	return x != v.x || y != v.y || z != v.z;
}

bool _tagVector3::operator!=(const DirectX::XMVECTOR& v) const
{
	_tagVector3 v1(v);

	return x != v1.x || y != v1.y || z != v1.z;
}

_tagVector3 _tagVector3::operator+(const _tagVector3& v) const
{
	return _tagVector3(x + v.x, y + v.y, z + v.z);
}

_tagVector3 _tagVector3::operator+(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 + v);
}

_tagVector3 _tagVector3::operator+(float f) const
{
	return _tagVector3(x + f, y + f, z + f);
}

_tagVector3& _tagVector3::operator++()
{
	++x;
	++y;
	++z;

	return *this;
}

_tagVector3 _tagVector3::operator++(int i)
{
	_tagVector3 v = *this;

	++x;
	++y;
	++z;

	return v;
}

_tagVector3 _tagVector3::operator+=(const _tagVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

_tagVector3 _tagVector3::operator+=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 += v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;

	return *this;
}

_tagVector3 _tagVector3::operator-(const _tagVector3& v) const
{
	return _tagVector3(x - v.x, y - v.y, z - v.z);
}

_tagVector3 _tagVector3::operator-(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 - v);
}

_tagVector3 _tagVector3::operator-(float f) const
{
	return _tagVector3(x - f, y - f, z - f);
}

_tagVector3 _tagVector3::operator-()
{
	return _tagVector3(-x, -y, -z);
}

_tagVector3& _tagVector3::operator--()
{
	--x;
	--y;
	--z;

	return *this;
}

_tagVector3 _tagVector3::operator--(int i)
{
	_tagVector3 v = *this;

	--x;
	--y;
	--z;

	return v;
}

_tagVector3 _tagVector3::operator-=(const _tagVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator-=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 -= v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;

	return *this;
}

_tagVector3 _tagVector3::operator*(const _tagVector3& v) const
{
	return _tagVector3(x * v.x, y * v.y, z * v.z);
}

_tagVector3 _tagVector3::operator*(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 * v);
}

_tagVector3 _tagVector3::operator*(float f) const
{
	return _tagVector3(x * f, y * f, z * f);
}

_tagVector3 _tagVector3::operator*=(const _tagVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator*=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 *= v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

_tagVector3 _tagVector3::operator/(const _tagVector3& v) const
{
	return _tagVector3(x / v.x, y / v.y, z / v.z);
}

_tagVector3 _tagVector3::operator/(const DirectX::XMVECTOR& v) const
{
	DirectX::XMVECTOR v1 = Convert();

	return _tagVector3(v1 / v);
}

_tagVector3 _tagVector3::operator/(float f) const
{
	return _tagVector3(x / f, y / f, z / f);
}

_tagVector3 _tagVector3::operator/=(const _tagVector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

_tagVector3 _tagVector3::operator/=(const DirectX::XMVECTOR& v)
{
	DirectX::XMVECTOR v1 = Convert();

	v1 /= v;

	Convert(v1);

	return _tagVector3(v1);
}

_tagVector3 _tagVector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}

void _tagVector3::Convert(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

DirectX::XMVECTOR _tagVector3::Convert() const
{
	return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

float _tagVector3::Length() const
{
	DirectX::XMVECTOR v = Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector3Length(v));
}

float _tagVector3::Dot(const _tagVector3& v) const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1,v2));
}

void _tagVector3::Normalize()
{
	DirectX::XMVECTOR v1 = Convert();

	Convert(DirectX::XMVector3Normalize(v1));
}

float _tagVector3::Distance(const _tagVector3& v) const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	v1 -= v2;

	return DirectX::XMVectorGetX(DirectX::XMVector3Length(v1));
}

_tagVector3 _tagVector3::Cross(const _tagVector3& v) const
{
	DirectX::XMVECTOR v1 = Convert();
	DirectX::XMVECTOR v2 = v.Convert();

	return _tagVector3(DirectX::XMVector3Cross(v1, v2));
}

_tagVector3 _tagVector3::TransformNormal(const Matrix& m)
{
	DirectX::XMVECTOR v = Convert();

	return _tagVector3(DirectX::XMVector3TransformNormal(v, m.m));
}

_tagVector3 _tagVector3::TransformCoord(const Matrix& m)
{
	DirectX::XMVECTOR v = Convert();

	return _tagVector3(DirectX::XMVector3TransformCoord(v, m.m));
}

_tagVector3& operator++(_tagVector3& v)
{
	++v.x;
	++v.y;
	++v.z;

	return v;
}

_tagVector3 operator*(float f, const _tagVector3& v)
{
	return _tagVector3(f * v.x, f * v.y, f * v.z);
}

_tagVector3 operator*(const DirectX::XMFLOAT3& v1, const _tagVector3& v2)
{
	return _tagVector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

_tagVector3 operator/(float f, const _tagVector3& v)
{
	return _tagVector3(f / v.x, f / v.y, f / v.z);
}

_tagVector3 operator/(const DirectX::XMFLOAT3& v1, const _tagVector3& v2)
{
	return _tagVector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}
