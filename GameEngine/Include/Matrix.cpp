#include "Matrix.h"
#include "Vector3.h"

_tagMatrix::_tagMatrix()
{
	m = DirectX::XMMatrixIdentity();
}

_tagMatrix::_tagMatrix(const _tagMatrix& _m)
{
	m = _m.m;
}

_tagMatrix::_tagMatrix(const DirectX::XMMATRIX& _m)
{
	m = _m;
}
 
_tagMatrix::_tagMatrix(const Vector4 _v[4])
{
	for(int i=0;i<4;++i)
		v[i] = _v[i];
}

Vector4& _tagMatrix::operator[](size_t idx)
{
	return v[idx];
}

_tagMatrix& _tagMatrix::operator=(const _tagMatrix& _m)
{
	m = _m.m;

	return *this;
}

_tagMatrix& _tagMatrix::operator=(const DirectX::XMMATRIX& _m)
{
	m = _m;

	return *this;
}

_tagMatrix _tagMatrix::operator*(const _tagMatrix& _m)	const
{
	return _tagMatrix(m * _m.m);
}

_tagMatrix _tagMatrix::operator*(const DirectX::XMMATRIX& _m)	const
{
	return _tagMatrix(m * _m);
}

_tagMatrix _tagMatrix::operator*(const Vector4 _v[4])	const
{
	return m*_tagMatrix(_v).m;
}

_tagMatrix& _tagMatrix::operator*=(const _tagMatrix& _m)
{
	m = DirectX::XMMatrixMultiply(m, _m.m);

	return *this;
}

_tagMatrix& _tagMatrix::operator*=(const DirectX::XMMATRIX& _m)
{
	m *= _m;

	return *this;
}

_tagMatrix& _tagMatrix::operator*=(const Vector4 _v[4])
{
	for(int i=0;i<4;++i)
		v[i] *= _v[i];

	return *this;
}

void _tagMatrix::Identity()
{
	m = DirectX::XMMatrixIdentity();
}

void _tagMatrix::Transpose()
{
	m = DirectX::XMMatrixTranspose(m);
}

void _tagMatrix::Inverse()
{
	DirectX::XMVECTOR v = DirectX::XMMatrixDeterminant(m);

	m = DirectX::XMMatrixInverse(&v,m);
}

void _tagMatrix::Scale(const _tagVector3& v)
{
	m = DirectX::XMMatrixScaling(v.x, v.y, v.z);
}

void _tagMatrix::Scale(float x, float y, float z)
{
	m = DirectX::XMMatrixScaling(x, y, z);
}

void _tagMatrix::Translation(const _tagVector3& v)
{
	m = DirectX::XMMatrixTranslation(v.x, v.y, v.z);
}

void _tagMatrix::Translation(float x, float y, float z)
{
	m = DirectX::XMMatrixTranslation(x, y, z);
}

void _tagMatrix::Rotate(const _tagVector3& v)
{
	DirectX::XMMATRIX mX = DirectX::XMMatrixRotationX(DegToRad(v.x));
	DirectX::XMMATRIX mY = DirectX::XMMatrixRotationY(DegToRad(v.y));
	DirectX::XMMATRIX mZ = DirectX::XMMatrixRotationZ(DegToRad(v.z));

	m = mX * mY * mZ;
}

void _tagMatrix::Rotate(float x, float y, float z)
{
	DirectX::XMMATRIX mX = DirectX::XMMatrixRotationX(DegToRad(x));
	DirectX::XMMATRIX mY = DirectX::XMMatrixRotationY(DegToRad(y));
	DirectX::XMMATRIX mZ = DirectX::XMMatrixRotationZ(DegToRad(z));

	m = mX * mY * mZ;
}

void _tagMatrix::RotateX(float x)
{
	m = DirectX::XMMatrixRotationX(DegToRad(x));
}

void _tagMatrix::RotateY(float y)
{
	m = DirectX::XMMatrixRotationY(DegToRad(y));
}

void _tagMatrix::RotateZ(float z)
{
	m = DirectX::XMMatrixRotationZ(DegToRad(z));
}

void _tagMatrix::RotateAxis(const _tagVector3& v, float fAngle)
{
	DirectX::XMVECTOR v1 = v.Convert();

	m = DirectX::XMMatrixRotationAxis(v1, DegToRad(fAngle));
}

void _tagMatrix::RotationQuaternion(const _tagVector4& q)
{
	m = DirectX::XMMatrixRotationQuaternion(q.Convert());
}

_tagMatrix _tagMatrix::StaticIdentity()
{
	return DirectX::XMMatrixIdentity();
}

_tagMatrix _tagMatrix::StaticTranspose(const _tagMatrix& m)
{
	return DirectX::XMMatrixTranspose(m.m);
}

_tagMatrix _tagMatrix::StaticInverse(const _tagMatrix& m)
{
	DirectX::XMVECTOR v = DirectX::XMMatrixDeterminant(m.m);

	return DirectX::XMMatrixInverse(&v, m.m);
}

_tagMatrix _tagMatrix::StaticScale(const _tagVector3& _v)
{
	return DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
}

_tagMatrix _tagMatrix::StaticScale(float x, float y, float z)
{
	return DirectX::XMMatrixScaling(x, y, z);
}

_tagMatrix _tagMatrix::StaticTranslation(const _tagVector3& _v)
{
	return DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
}

_tagMatrix _tagMatrix::StaticTranslation(float x, float y, float z)
{
	return DirectX::XMMatrixTranslation(x, y, z);
}

_tagMatrix _tagMatrix::StaticRotate(const _tagVector3& v)
{
	DirectX::XMMATRIX mX = DirectX::XMMatrixRotationX(DegToRad(v.x));
	DirectX::XMMATRIX mY = DirectX::XMMatrixRotationY(DegToRad(v.y));
	DirectX::XMMATRIX mZ = DirectX::XMMatrixRotationZ(DegToRad(v.z));

	return mX * mY * mZ;
}

_tagMatrix _tagMatrix::StaticRotate(float x, float y, float z)
{
	DirectX::XMMATRIX mX = DirectX::XMMatrixRotationX(DegToRad(x));
	DirectX::XMMATRIX mY = DirectX::XMMatrixRotationY(DegToRad(y));
	DirectX::XMMATRIX mZ = DirectX::XMMatrixRotationZ(DegToRad(z));

	return mX * mY * mZ;
}

_tagMatrix _tagMatrix::StaticRotateX(float x)
{
	return DirectX::XMMatrixRotationX(DegToRad(x));
}

_tagMatrix _tagMatrix::StaticRotateY(float y)
{
	return DirectX::XMMatrixRotationY(DegToRad(y));
}

_tagMatrix _tagMatrix::StaticRotateZ(float z)
{
	return DirectX::XMMatrixRotationZ(DegToRad(z));
}

_tagMatrix _tagMatrix::StaticRotateAxis(const _tagVector3& _v, float fAngle)
{
	return DirectX::XMMatrixRotationAxis(_v.Convert(), DegToRad(fAngle));
}

_tagMatrix _tagMatrix::StaticRotationQuaternion(const _tagVector4& q)
{
	return _tagMatrix(DirectX::XMMatrixRotationQuaternion(q.Convert()));
}
