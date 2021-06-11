#pragma once

#include "Types.h"

typedef struct _tagResolution
{
	int		iWidth;
	int		iHeight;
}Resolution, *PResolution;

typedef struct _tagVertexColor
{
	Vector3		vPos;
	Vector2		vUV;
	Vector4		vColor;
}VertexColor, *PVertexColor;

typedef struct _tagBasic
{
	Vector3		vPos;
	Vector2		vUV;
}Basic, *PBasic;

typedef struct _tagVertex
{
	Vector3	vInitialPosW;
	Vector3 vInitialVelW;
	Vector2 vSizeW;
	float fAge;
	unsigned int iType;
}VERTEX, *PVERTEX;

typedef struct _tagInstancedData2D
{
	Matrix	matWVP;
	Vector3	vPivot;
	Vector3 vSize;
	Vector2 vTexSize;
	Vector2 vStart;
	Vector2 vEnd;
	Vector4 vColor;
}InstancedData2D, * PInstancedData2D;

typedef struct _tagInstancedData
{
	Matrix	matWVP;
	Vector3	vPivot;
	Vector3 vSize;
}InstancedData, * PInstancedData;

typedef struct _tagInstCollider2D
{
	Matrix	matWVP;
	Vector3	vPivot;
	Vector3	vMeshSize;
	Vector4	vColor;
}INSTCOLLIDER2D, *PINSTCOLLIDER2D;

typedef struct _tagVertex3D
{
	Vector3		vPos;
	Vector2		vUV;
	Vector3		vNormal;
	Vector3		vTangent;
	Vector3		vBinormal;
	Vector4		vBlendWeight;
	Vector4		vBlendIndex;
}VERTEX3D, *PVERTEX3D;