#pragma once
class CGameMode
{
	friend class CScene;

private:
	CGameMode();
	~CGameMode();

public:
	bool Init();
};

