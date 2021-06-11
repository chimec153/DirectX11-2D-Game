#pragma once
#include "State.h"
class CSleepState :
    public CState
{
public:
    CSleepState();
    CSleepState(const CSleepState& stat);
    virtual ~CSleepState();

public:
    virtual void Attack(class CSceneComponent* pCom);
};

