#pragma once
#include "SceneComponent.h"
class CDecal :
    public CSceneComponent
{
    friend class CObj;

private:
    CDecal();
    CDecal(const CDecal& decal);
    virtual ~CDecal();

private:
    DECALCBUFFER    m_tCBuffer;
    float           m_fInTime;
    float           m_fInTimeLimit;
    bool            m_bInComplete;
    float           m_fDuration;
    float           m_fDurationLimit;
    bool            m_bDurationComplete;
    float           m_fOutTime;
    float           m_fOutTimeLimit;
    bool            m_bLoop;
            
public:
    void SetAlpha(float fAlpha);
    void SetInTime(float fTime);
    void SetDuration(float fTime);
    void SetOutTime(float fTime);
    void SetLoop(bool bLoop);

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CDecal* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

