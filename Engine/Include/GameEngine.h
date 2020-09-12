#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <tchar.h>
#include <crtdbg.h>

#pragma comment(lib, "d3d11.lib")

#include "Macro.h"
#include "resource.h"

typedef struct _tagResolution
{
	int		iWidth;
	int		iHeight;
}Resolution, *PResolution;