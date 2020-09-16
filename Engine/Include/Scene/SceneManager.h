#pragma once

#include "../GameEngine.h"
#include "Scene.h"

class CSceneManager
{
private:
	class CScene*	m_pScene;	
	class CScene*	m_pSceneNext;

public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int PostUpdate(float fTime);
	int Collision(float fTime);
	int PreRender(float fTime);
	int Render(float fTime);
	int PostRender(float fTime);

public:
	template <typename T>
	bool SetGameMode(bool bCurrent = true)
	{
		if (bCurrent)
			return m_pScene->SetGameMode<T>();

		return m_pSceneNext->SetGameMode<T>();
	}

	DECLARE_SINGLE(CSceneManager)
};

