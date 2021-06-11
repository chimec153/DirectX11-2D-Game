#pragma once
#include "GameEngine.h"
#ifdef _DEBUG
#ifdef _WIN64
#pragma comment(lib, "GameEngine_Debug.lib")
#else
#pragma comment(lib, "GameEngine32_Debug.lib")
#endif
#else
#ifdef _WIN64
#pragma comment(lib, "GameEngine.lib")
#else
#pragma comment(lib, "GameEngine32.lib")
#endif
#endif
