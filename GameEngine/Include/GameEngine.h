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
	Vector4		vColor;
}VertexColor, *PVertexColor;