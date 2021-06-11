#pragma once
#include "Ref.h"

enum class State
{
    SLEEP,
    IDLE,
    ATTACK,
    STOP,
    DIE,
    END
};

class CState :
    public CRef
{
protected:
    CState();
    CState(const CState& st);
    virtual ~CState() = 0;

private:
    State       m_eStat;
    class CObj* m_pObj;

public:
    State GetState()    const
    {
        return m_eStat;
    }

    void SetState(State eStat)
    {
        m_eStat = eStat;
    }

    void SetObj(class CObj* pObj)
    {
        m_pObj = pObj;
    }

public:
    virtual void Attack(class CScenecomponent* pCom);
    virtual CState* Clone() = 0;

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

