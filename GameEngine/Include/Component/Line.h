#pragma once
#include "SceneComponent.h"
class CLine :
    public CSceneComponent
{
    friend class CObj;

private:
    CLine();
    CLine(const CLine& com);
    virtual ~CLine();

private:
    float   m_fAngle;
    float   m_fAngleSpeed;
    ANGLE   m_tCBuffer;

public:
    void SetAngleSpeed(float fSpeed);
    void SetAngle(float fAngle);

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CLine* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

