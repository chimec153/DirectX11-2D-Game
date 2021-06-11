#pragma once

#include "GameEngine.h"
#include <fstream>
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

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