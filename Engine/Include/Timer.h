#pragma once

#include "GameEngine.h"

class CTimer
{
private:
	float			m_fDeltaTime;
	float			m_fFPS;
	int				m_iFrame;
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tPrevTime;
	float			m_fFrameTime;
	float			m_fTimeScale;

public:
	float GetDeltaTime()	const
	{
		return m_fDeltaTime * m_fTimeScale;
	}

	float GetFPS()	const
	{
		return m_fFPS;
	}

public:
	bool Init();
	void Update();

	DECLARE_SINGLE(CTimer)
};

