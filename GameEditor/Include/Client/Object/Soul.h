#pragma once
#include "Object\Obj.h"
class CSoul :
    public CObj
{
    friend class CScene;

private:
    CSoul();
    CSoul(const CSoul& obj);
    virtual ~CSoul();

private:
    class CObj* m_pTarget;
    Vector2 m_vInitVel;
    Vector2 m_vTermVel;
    Vector2 m_vSpeed;
    float   m_fTime;
    Vector3 m_vTargetPos;
    bool    m_bArrive;
    float     m_fOffset;
    Vector3 m_vPrevPos;
    static float   m_fScale;

public:
    void SetTarget(class CObj* pObj);
    void SetInitVel(const Vector2& vVel)
    {
        m_vInitVel = vVel;
    }

    void SetTargetPos(const Vector3& vPos)
    {
        m_vTargetPos = vPos;
    }

    void SetOffset(float fOffset)
    {
        m_fOffset = fOffset;
    }

    float GetScale()    const
    {
        return m_fScale;
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
    virtual CSoul* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

