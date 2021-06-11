#pragma once
#include "Object\Obj.h"
class CFireButton :
    public CObj
{
    friend class CScene;

private:
    CFireButton();
    CFireButton(const CFireButton& obj);
    virtual ~CFireButton();

private:
    class CSpriteComponent* m_pButton;
    class CColliderRect* m_pRC;
    bool    m_bPush;
    class CSound* m_pEft;

public:
    virtual bool Init();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CFireButton* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void ColInit(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

};

