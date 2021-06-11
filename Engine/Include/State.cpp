#include "pch.h"
#include "State.h"

CState::CState()	:
	m_eStat(State::IDLE)
{
}

CState::CState(const CState& st)	:
	CRef(st)
	, m_eStat(st.m_eStat)
	, m_pObj(st.m_pObj)
{
}

CState::~CState()
{
}

void CState::Attack(CScenecomponent* pCom)
{
}

void CState::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_eStat, 4, 1, pFile);
}

void CState::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_eStat, 4, 1, pFile);
}
