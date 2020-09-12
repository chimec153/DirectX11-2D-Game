#pragma once

#include "../GameEngine.h"

class CSceneManager
{
private:
	class CScene*	m_pScene;

public:
	bool Init();

	DECLARE_SINGLE(CSceneManager)
};

