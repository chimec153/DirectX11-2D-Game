#pragma once

#include "GameEngine.h"

class CClientCreateSystem
{
public:
	bool Init();

public:
	class CObj* CreateObj(int iType);
	class CGameMode* CreateGameMode(int iType);

	DECLARE_SINGLE(CClientCreateSystem)
};

