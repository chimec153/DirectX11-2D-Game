#include "pch.h"
#include "DieState.h"

CDieState::CDieState()
{
    SetState(State::DIE);
}

CDieState::CDieState(const CDieState& stat) :
    CState(stat)
{
}

CDieState::~CDieState()
{
}

CDieState* CDieState::Clone()
{
    return new CDieState(*this);
}

void CDieState::Save(FILE* pFile)
{
    CState::Save(pFile);
}

void CDieState::Load(FILE* pFile)
{
    CState::Load(pFile);
}
