#pragma once

#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

#include "Flag.h"

#define PI		3.14159f
#define PI_DIV2	1.57080f
#define PI_DIV4	0.785398f

static float DegToRad(float fDeg)
{
	return fDeg * PI / 180.f;
}

static float RadToDeg(float fRad)
{
	return fRad * 180.f / PI;
}