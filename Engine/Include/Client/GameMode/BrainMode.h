#pragma once
#include "TileMode.h"
class CBrainMode :
    public CTileMode
{
    friend class CScene;

private:
    CBrainMode();
    virtual ~CBrainMode();

private:
    class CFire* m_pFire;
    static bool m_bClear;

public:
    void Clear()
    {
        m_bClear = true;
    }

    class CFire* GetFire()  const;

public:
    virtual bool Init();
    virtual void Start();
};

