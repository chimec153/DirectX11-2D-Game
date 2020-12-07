#pragma once

#include <DirectXMath.h>
#include <Windows.h>

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