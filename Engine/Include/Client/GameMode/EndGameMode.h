#pragma once

#include "TileMode.h"

class CEndGameMode	:
	CTileMode
{
	friend class CScene;

private:
	CEndGameMode();
	~CEndGameMode();

private:
	float		m_fTime;
	int			m_iStage;
	float		m_fFadeInLimit;
	float		m_fFadeOutLimit;
	bool		m_bFadeIn;
	float		m_fStayTimeLimit;

public:
	virtual bool Init();
	virtual void Update(float fTime);

};

