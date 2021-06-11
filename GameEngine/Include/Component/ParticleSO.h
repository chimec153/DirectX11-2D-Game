#pragma once
#include "SceneComponent.h"
class CParticleSO :
    public CSceneComponent
{
    friend class CObj;

private:
    CParticleSO();
    CParticleSO(const CParticleSO& com);
    virtual ~CParticleSO();

private:
    UINT m_iMaxParticles;
    float   m_fGameTime;
    float   m_fTimeStep;
    float   m_fAge;

    Vector3 m_vCamPosW;
    Vector3 m_vEmitPosW;
    Vector3 m_vEmitDirW;

    class CParticleSystem* m_pSystem;

    ID3D11Buffer* m_pInitVB;
    ID3D11Buffer* m_pDrawVB;
    ID3D11Buffer* m_pStreamOutVB;

    class CTexture* m_pNoiseTex;
    class CTexture* m_pTexArray;

    CBPERFRAME  m_tCBuffer;
    class CGraphicShader*  m_pShader;
    class CMesh* m_pMesh;

public:
    void Reset();
    void SetParticle(class CParticleSystem* pSystem, class CTexture* pTexArray, UINT maxParticles);

private:
    void BuildVB();

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CParticleSO* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

