#include "pch.h"
#include "IdleState.h"

CIdleState::CIdleState()
{
	SetState(State::IDLE);
}

CIdleState::CIdleState(const CIdleState& stat) :
	CState(stat)
{
}

CIdleState::~CIdleState()
{
}

CIdleState* CIdleState::Clone()
{
	return new CIdleState(*this);
}

void CIdleState::Save(FILE* pFile)
{
	CState::Save(pFile);
}

void CIdleState::Load(FILE* pFile)
{
	CState::Load(pFile);
}
