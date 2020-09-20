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

typedef struct _tagTransformCBuffer
{
	Matrix		matWorld;
	Matrix		matView;
	Matrix		matProj;
	Matrix		matWV;
	Matrix		matWVP;
	Vector3		vPivot;
	Vector3		vMeshSize;
}TransformCBuffer, *PTransformCBuffer;

typedef struct _tagShaderCBuffer
{
	Vector4	vDif;
	Vector4 vAmp;
	Vector4	vSpe;
}ShaderCBuffer, *PShaderCBuffer;


typedef struct _tagCBuffer
{
	ID3D11Buffer*	pBuffer;
	int				iSize;
	int				iRegister;
	int				iType;
}CBuffer, *PCBuffer;
