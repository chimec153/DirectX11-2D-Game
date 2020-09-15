#pragma once

#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Flag.h"

#define PI	3.14159f

static float DegToRad(float fDeg)
{
	return fDeg * PI / 180.f;
}

static float RadToDeg(float fRad)
{
	return fRad * 180.f / PI;
}