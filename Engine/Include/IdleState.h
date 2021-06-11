#pragma once
#include "State.h"
class CIdleState :
    public CState
{
public:
    CIdleState();
    CIdleState(const CIdleState& stat);
    virtual ~CIdleState();

public:
    virtual CIdleState* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

