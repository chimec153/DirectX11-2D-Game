#include "Animation2DNotify.h"

CAnimation2DNotify::CAnimation2DNotify()	:
	m_strTag(),
	m_iFrame(-1),
	m_fTime(-1.f),
	m_bCall(false),
	m_fAccTime(0.f)
{
}

CAnimation2DNotify::~CAnimation2DNotify()
{
}

void CAnimation2DNotify::CreateNotify(const std::string& strTag, int iFrame)
{
	m_strTag = strTag;
	m_iFrame = iFrame;
}

void CAnimation2DNotify::CreateNotify(const std::string& strTag, float fTime)
{
	m_strTag = strTag;
	m_fTime = fTime;
}

void CAnimation2DNotify::AddFunc(void(*pFunc)(float))
{
	m_vecCallBack.push_back(std::bind(pFunc, std::placeholders::_1));
}

void CAnimation2DNotify::AddFunc(void(*pFunc)(int, float))
{
	m_vecFrameFunc.push_back(std::bind(pFunc, std::placeholders::_1, std::placeholders::_2));
}

void CAnimation2DNotify::Update(float fTime, int iFrame, float fDeltaTime)
{
	if (m_bCall)
		return;

	m_fAccTime += fDeltaTime;

	if (m_iFrame == -1)
	{

		if (m_fTime <= m_fAccTime)
		{
			Call(fDeltaTime);
		}
	}

	else
	{
		if (m_iFrame <= iFrame)
		{
			Call(fDeltaTime);
#ifdef _DEBUG
			char pTime[256] = {};

			sprintf_s(pTime, "%d\n", iFrame);

			OutputDebugStringA(pTime);
#endif
		}
	}
}

void CAnimation2DNotify::Call(float fTime)
{
	m_bCall = true;

	size_t iSz = m_vecCallBack.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecCallBack[i](fTime);
	}
	
	iSz = m_vecFrameFunc.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_iFrame != -1)
		{
			m_vecFrameFunc[i](m_iFrame, fTime);
		}
		else
		{
			m_vecFrameFunc[i]((int)m_fTime, fTime);
		}
	}
}

void CAnimation2DNotify::Clear()
{
	m_bCall = false;
	m_fAccTime = 0.f;
}

void CAnimation2DNotify::Save(FILE* pFile)
{
}

void CAnimation2DNotify::Load(FILE* pFile)
{
}
