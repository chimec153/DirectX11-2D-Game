#pragma once
#include "SoulMonster.h"

class CBrainFreeze :
    public CSoulMonster
{
    friend class CScene;
    friend class CBossManager;

private:
    CBrainFreeze();
    CBrainFreeze(const CBrainFreeze& obj);
    virtual ~CBrainFreeze();

private:
    class CSpriteComponent* m_pCube;
    class CSpriteComponent* m_pBrain;
    class CSpriteComponent* m_pLight;
    class CSpriteComponent* m_pShadow;
    class CColliderOBB2D* m_pOC;
    class CMesh2DComponent* m_pIceShadow;
    class CColliderRect* m_pRC;
    float                   m_fAttackTime;
    static float                   m_fAttackTimeMax;
    DIR_8                   m_eDir;
    Vector3                   m_vAccel;
    Vector3                   m_vSpeed;
    Vector4                 m_vNextQuat;
    Vector4                 m_vPrevQuat;
    int                     m_iPhase;
    float                   m_fRot;
    float                   m_fAccelTime;
    static float                   m_fAccelTimeMax;
    bool                    m_bAccel;
    DIR_8                   m_eNextDir;
    int                     m_iRot;
    bool                    m_bFly;
    float                   m_fFlyTime;
    static float                   m_fFlyTimeMax;
    bool                   m_bFall;
    float                   m_fFlyAccel;
    float                   m_fFlySpeed;
    float                   m_fFlyDist;
    bool                    m_bMelt;
    static float                   m_fMeltSpeed;
    float                   m_fMeltTime;
    static float                   m_fMeltTimeMax;
    bool                    m_bBrain;
#ifdef _DEBUG
    class CUIFont* m_pDir;
    class CUIFont* m_pFly;
#endif
    bool        m_bLight;
    float       m_fLightTime;
    static float       m_fLightTimeMax;
    bool        m_bLightOut;
    class CSound* m_pBGM;
    class CSound* m_pEft;
public:
    bool IsFly()    const
    {
        return m_bFly;
    }

    bool IsBrain()  const
    {
        return m_bBrain;
    }

    virtual void SetState(State eStat);

public:
    bool Init();
    bool Init(const char* pFileName, const std::string& strPathKey = DATA_PATH);
    void Start();
    void Update(float fTime);
    void PostUpdate(float fTime);
    void Collision(float fTime);
    void PreRender(float fTime);
    void Render(float fTime);
    void PostRender(float fTime);
    virtual CBrainFreeze* Clone();

    void Save(FILE* pFile);
    void Load(FILE* pFile);

public:
    void RotX(float fScale, float fTime);
    void RotY(float fScale, float fTime);
    void RotZ(float fScale, float fTime);

public:
    void ColInit(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
    void NextBrain(float, float);
    void Cheat(float, float);

public:
    void SpawnWindow();
};

