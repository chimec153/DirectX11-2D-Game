#pragma once

#include "Vector4.h"
 
typedef __declspec(align(16)) union _tagMatrix
{
	DirectX::XMMATRIX m;

	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	Vector4 v[4];

	_tagMatrix();
	_tagMatrix(const _tagMatrix& m);
	_tagMatrix(const DirectX::XMMATRIX& m);
	_tagMatrix(const Vector4 _v[4]);

	Vector4& operator[] (size_t idx);

	// operator =
	_tagMatrix& operator= (const _tagMatrix& m);
	_tagMatrix& operator= (const DirectX::XMMATRIX& m);

	// operator *
	_tagMatrix operator* (const _tagMatrix& m)	const;
	_tagMatrix operator* (const DirectX::XMMATRIX& m)	const;
	_tagMatrix operator* (const Vector4 _v[4])	const;

	// operator *=
	_tagMatrix operator*= (const _tagMatrix& m);
	_tagMatrix operator*= (const DirectX::XMMATRIX& m);
	_tagMatrix operator*= (const Vector4 _v[4]);

	void Identity();
	void Transpose();
	void Inverse();

	void Scale(const union _tagVector3& _v);
	void Scale(float x, float y, float z);
	void Translation(const union _tagVector3& _v);
	void Translation(float x, float y, float z);
	void Rotate(const union _tagVector3& v);
	void Rotate(float x, float y, float z);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);
	void RotateAxis(const union _tagVector3& _v, float fAngle);

	static _tagMatrix StaticIdentity();
	static _tagMatrix StaticTranspose(const _tagMatrix& m);
	static _tagMatrix StaticInverse(const _tagMatrix& m);
	static _tagMatrix StaticScale(const union _tagVector3& _v);
	static _tagMatrix StaticScale(float x, float y, float z);
	static _tagMatrix StaticTranslation(const union _tagVector3& _v);
	static _tagMatrix StaticTranslation(float x, float y, float z);
	static _tagMatrix StaticRotate(const union _tagVector3& v);
	static _tagMatrix StaticRotate(float x, float y, float z);
	static _tagMatrix StaticRotateX(float x);
	static _tagMatrix StaticRotateY(float y);
	static _tagMatrix StaticRotateZ(float z);
	static _tagMatrix StaticRotateAxis(const union _tagVector3& _v, float fAngle);

}Matrix, *PMatrix;

