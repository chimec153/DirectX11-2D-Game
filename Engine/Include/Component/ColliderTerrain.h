#pragma once
#include "Collider.h"
class CColliderTerrain :
    public CCollider
{
    friend class CObj;

private:
    CColliderTerrain();
    CColliderTerrain(const CColliderTerrain& col);
    virtual ~CColliderTerrain();

private:
    TERRAININFO     m_tInfo;

public:
    void SetPos(const std::vector<Vector3>& vecPos);
    void SetInfo(int iWidth, int iHeight);
    const TERRAININFO& GetInfo()    const;
#ifdef _DEBUG
    void SetDebugMesh(const std::string& strMesh);
    class CMesh* GetDebugMesh() const;
#endif

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CColliderTerrain* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    virtual bool Collision(class CCollider* pDest);
    virtual bool MouseCollision(const Vector2& vPos);
};

