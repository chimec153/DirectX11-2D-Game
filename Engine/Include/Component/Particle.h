#pragma once
#include "SceneComponent.h"

class CParticle :
    public CSceneComponent
{
    friend class CObj;

private:
    CParticle();
    CParticle(const CParticle& com);
    virtual ~CParticle();

private:
    class CComputeShader* m_pUpdateShader;
    std::vector<class CParticleSystem*>   m_vecParticle;
    float           m_fSpawnTime;
    float           m_fSpawnTimeLimit;
    PARTICLE        m_tParticle;
    PARTICLESHARE   m_tParticleShare;
    PARTICLECBUFFER m_tCBuffer;
    class CTexture* m_pNoiseTex;   
    SpriteCBuffer   m_tSpriteCBuffer;
    int             m_iFrame;
    int             m_iMaxFrame;
    float           m_fFrameTime;
    float           m_fFrameTimeMax;
    Vector2         m_vStart;
    Vector2         m_vEnd;
    bool            m_bStop;
    int             m_iSpawnCount;

public:
    void AddParticle(const std::string& strTag);
    void SetSpawnLimit(float fTime);
    void SetUVStart(const Vector2& vStart);
    void SetUVEnd(const Vector2& vEnd);
    void SetUVSize(const Vector2& vSize);
    void SetUVStart(float x, float y);
    void SetUVEnd(float x, float y);
    void SetUVSize(float x, float y);
    void SetAnim(int iMaxFrame, float fFrameTimeMax);
    void Stop();
    void Resume();
    void SetSpawnCount(int iCount);

public:
    virtual bool Init();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CParticle* Clone();
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void SpawnWindow();
};

