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

typedef struct _tagInstancedData2D
{
	Matrix	matWVP;
	Vector3	vPivot;
	Vector3 vSize;
	Vector2 vTexSize;
	Vector2 vStart;
	Vector2 vEnd;
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