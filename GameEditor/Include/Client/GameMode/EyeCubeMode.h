#pragma once
#include "TileMode.h"
class CEyeCubeMode :
    public CTileMode
{
    friend class CScene;

private:
    CEyeCubeMode();
    virtual ~CEyeCubeMode();

private:
    static bool m_bClear;

public:
    void Clear()
    {
        m_bClear = true;
    }

public:
    virtual bool Init();
    virtual void Start();

};

