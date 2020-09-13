#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <tchar.h>
#include <crtdbg.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Macro.h"
#include "resource.h"
#include "EngineMath.h"

#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

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