#pragma once

#include "Object\Obj.h"

class CFire :
    public CObj
{
    friend class CScene;

private:
    CFire();
    CFire(const CFire& fire);
    virtual ~CFire();

private:
    class CSpriteComponent* m_pFire;
    class CColliderRect* m_pRC;
    class CSpriteComponent* m_pLight;
    class CSpriteComponent* m_pLightBig;
    bool                m_bOn;
    float               m_fScaleSpeed;
    bool                m_bOff;
    float               m_fScale;
    float               m_fScaleMax;
    class CSound* m_pBGM;
    class CSound* m_pEft;

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CFire* Clone();

    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void ColInit(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
    void StartFire();
    void EndFire();

};

