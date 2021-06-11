#pragma once
#include "State.h"
class CAttackState :
    public CState
{
public:
    CAttackState();
    CAttackState(const CAttackState& stat);
    virtual ~CAttackState();

public:
    virtual void Attack(class CSceneComponent* pCom);
    virtual CAttackState* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

