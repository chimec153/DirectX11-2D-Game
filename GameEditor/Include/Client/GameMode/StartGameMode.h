#pragma once
#include "GameMode.h"
class CStartGameMode :
	public CGameMode
{
	friend class CScene;

private:
	CStartGameMode();
	virtual ~CStartGameMode();

public:
	bool Init();
	bool SetUI();
	bool LoadResources();
	void MainGameMode(float);
	void ExitGame(float);
};

