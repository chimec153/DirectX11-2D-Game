#include "pch.h"
#include "AttackState.h"

CAttackState::CAttackState()
{
	SetState(State::ATTACK);
}

CAttackState::CAttackState(const CAttackState& stat)	:
	CState(stat)
{
}

CAttackState::~CAttackState()
{
}

void CAttackState::Attack(CSceneComponent* pCom)
{
}

CAttackState* CAttackState::Clone()
{
	return new CAttackState(*this);
}

void CAttackState::Save(FILE* pFile)
{
	CState::Save(pFile);
}

void CAttackState::Load(FILE* pFile)
{
	CState::Load(pFile);
}
