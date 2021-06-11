#pragma once
#include "Collider.h"
class CColliderRay :
    public CCollider
{
    friend class CObj;

private:
    CColliderRay();
    CColliderRay(const CColliderRay& ray);
    virtual ~CColliderRay();

private:
    RAYINFO m_tInfo;
    RAYINFO m_tUIInfo;

public:
    const RAYINFO& GetInfo()    const;
    const RAYINFO& GetUIInfo()    const;
    void SetDir(const Vector3& vDir);
    void SetOrigin(const Vector3& vPos);

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CColliderRay* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    virtual bool Collision(class CCollider* pDest);
};

