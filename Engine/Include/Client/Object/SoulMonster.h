#pragma once
#include "Object\Obj.h"

enum class BOSS_TYPE
{
    NONE,
    COLOSSUS,
    EYECUBE = 1<<1,
    BRAINFREEZE = 1<<2,
    END
};

class CSoulMonster :
    public CObj
{
    friend class CScene;

protected:
    CSoulMonster();
    CSoulMonster(const CSoulMonster& obj);
    virtual ~CSoulMonster();

private:
    class CState* m_pState;
    BOSS_TYPE   m_eBossType;

public:
    enum class State GetState()    const;
    virtual void SetState(State eStat);
    void SetBossType(BOSS_TYPE eType)
    {
        m_eBossType = eType;
    }

    BOSS_TYPE GetBossType() const
    {
        return m_eBossType;
    }

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CSoulMonster* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

