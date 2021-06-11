#pragma once
#include "State.h"
class CStopState :
    public CState
{
public:
    CStopState();
    CStopState(const CStopState& stat);
    virtual ~CStopState();

public:
    virtual void Attack(class CSceneComponent* pCom);
    virtual CStopState* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

