#pragma once
#include "GameMode.h"
class CMainMode :
	public CGameMode
{
	friend class CScene;

private:
	CMainMode();
	virtual ~CMainMode();

public:
	bool Init();
	bool LoadTexture();
};

