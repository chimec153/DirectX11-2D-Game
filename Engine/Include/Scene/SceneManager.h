#pragma once

#include "../GameEngine.h"

class CSceneManager
{
private:
	class CScene*	m_pScene;

public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int PostUpdate(float fTime);
	int Collision(float fTime);
	int PreRender(float fTime);
	int Render(float fTime);
	int PostRender(float fTime);

	DECLARE_SINGLE(CSceneManager)
};

