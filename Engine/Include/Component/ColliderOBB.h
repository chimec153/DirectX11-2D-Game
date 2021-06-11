#pragma once
#include "Collider.h"
class CColliderOBB :
    public CCollider
{
    friend class CObj;

private:
    CColliderOBB();
    CColliderOBB(const CColliderOBB& obb);
    virtual ~CColliderOBB();

private:
    OBBInfo m_tInfo;

public:
    void SetExtent(float x, float y, float z);
    void SetExtent(const Vector3& vExtent);
    const OBBInfo& GetInfo()    const;

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CColliderOBB* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    virtual bool Collision(class CCollider* pDest);
    virtual bool MouseCollision(const Vector2& vPos);
};

