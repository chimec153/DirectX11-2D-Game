#include "Timer.h"

CTimer::CTimer()	:
	m_fDeltaTime(0.f),
	m_fFPS(0.f),
	m_iFrame(0),
	m_tSecond(),
	m_tPrevTime(),
	m_fFrameTime(0.f),
	m_fTimeScale(1.f)
{

}

CTimer::~CTimer()
{

}

float CTimer::GetDeltaTime() const
{
	return m_fDeltaTime * m_fTimeScale;
}

float CTimer::GetFPS() const
{
	return m_fFPS;
}

void CTimer::SetTimeScale(float fScale)
{
	m_fTimeScale = fScale;
}

float CTimer::GetTimeScale() const
{
	return m_fTimeScale;
}

bool CTimer::Init()
{
	QueryPerformanceCounter(&m_tPrevTime);
	QueryPerformanceFrequency(&m_tSecond);

	return true;
}

void CTimer::Update()
{
	LARGE_INTEGER tTime = {};

	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tPrevTime.QuadPart) / (float)m_tSecond.QuadPart;

	m_tPrevTime = tTime;

	++m_iFrame;

	m_fFrameTime += m_fDeltaTime;

	if (m_fFrameTime >= 1.f)
	{
		m_fFPS = m_iFrame / m_fFrameTime;

		m_fFrameTime -= 1.f;

		m_iFrame = 0;/*

		char strFPS[64] = {};

		sprintf_s(strFPS, "FPS: %.5f, DT: %.5f\n", m_fFPS, m_fDeltaTime);

		OutputDebugStringA(strFPS);*/
	}
}
