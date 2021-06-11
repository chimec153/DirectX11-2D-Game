#pragma once
#include "State.h"
class CDieState :
    public CState
{
public:
    CDieState();
    CDieState(const CDieState& stat);
    virtual ~CDieState();

public:
    virtual CDieState* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

