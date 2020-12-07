#pragma once

#include "../GameEngine.h"

class CAnimation2DNotify
{
	friend class CAnimation2DSequence;

private:
	CAnimation2DNotify();
	~CAnimation2DNotify();

private:
	std::string		m_strTag;
	int				m_iFrame;

public:
	void CreateNotify(const std::string& strTag, int iFrame);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

};

