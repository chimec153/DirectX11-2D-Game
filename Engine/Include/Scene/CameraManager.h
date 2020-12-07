#pragma once

#include "../GameEngine.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	class CCamera*		m_pMainCam;
	class CCamera*		m_pUICam;

public:
	void SetMainCam(class CCamera* pCam);
	void SetUICam(class CCamera* pCam);
	class CCamera* GetMainCam()	const;
	class CCamera* GetUICam()	const;

public:
	bool Init();
	void Update(float fTime);
	void PostUpdate(float fTime);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

