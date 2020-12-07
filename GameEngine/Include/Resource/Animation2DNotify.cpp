#include "Animation2DNotify.h"

CAnimation2DNotify::CAnimation2DNotify()
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

void CAnimation2DNotify::Save(FILE* pFile)
{
}

void CAnimation2DNotify::Load(FILE* pFile)
{
}
