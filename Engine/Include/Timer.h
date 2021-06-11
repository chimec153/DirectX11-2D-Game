#pragma once

#include "Ref.h"

class CTimer	:
	public CRef
{
	friend class CTimerManager;

private:
	CTimer();
	virtual ~CTimer();

private:
	float			m_fDeltaTime;
	float			m_fFPS;
	int				m_iFrame;
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tPrevTime;
	float			m_fFrameTime;
	float			m_fTimeScale;

public:
	float GetDeltaTime()	const;
	float GetFPS()	const;
	void SetTimeScale(float fScale);
	float GetTimeScale()	const;

public:
	bool Init();
	void Update();
};

