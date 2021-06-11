#include "pch.h"
#include "StopState.h"

CStopState::CStopState()
{
    SetState(State::STOP);
}

CStopState::CStopState(const CStopState& stat) :
    CState(stat)
{
}

CStopState::~CStopState()
{
}

void CStopState::Attack(CSceneComponent* pCom)
{
}

CStopState* CStopState::Clone()
{
    return new CStopState(*this);
}

void CStopState::Save(FILE* pFile)
{
    CState::Save(pFile);
}

void CStopState::Load(FILE* pFile)
{
    CState::Load(pFile);
}
