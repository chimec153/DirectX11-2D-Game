#pragma once

#include "SoulMonster.h"

enum class EYE_STAGE
{
    NONE = -1,
    CLOSE,
    OPEN1,
    OPEN2,
    LIGHT,
    ATTACK,
    END
};

enum class CUBE_AXIS
{
    L,
    R,
    N,
    F,
    U,
    D,
    END
};

class CEyecube :
    public CSoulMonster
{
    friend class CScene;
    friend class CBossManager;

private:
    CEyecube();
    CEyecube(const CEyecube& obj);
    virtual ~CEyecube();

private:
    DIR_8           m_eDir;
    bool            m_bTurned;
    float           m_fTurnTime;
    static float           m_fTurnMax;
#ifdef _DEBUG
    class CUIFont* m_pXText;
#endif
    float           m_fDelay;
    static float           m_fDelayMax;
    float           m_fAngleX;
    Vector4         m_vNextQuat;
    Vector4         m_vPrevQuat;
    class CColliderRect* m_pRC;
    class CColliderOBB2D* m_pOC;
    class CSpriteComponent* m_pCube;
    class CMesh2DComponent* m_pCylinder;
    class CMesh2DComponent* m_pRedCylinder;
    class CSpriteComponent* m_pFace;
    class CSpriteComponent* m_pLight;
    class CMaterial*        m_pLightMtrl;
    int             m_iRotate;
    class CMesh2DComponent* m_pShadow;
    CUBE_AXIS           m_eX;
#ifdef _DEBUG
    class CUIFont* m_pStage;
    class CUIFont* m_pTurnText;
    class CUIFont* m_pX;
    class CUIFont* m_pTurn;
    class CUIFont* m_pDelay;
#endif
    bool            m_bAirRotDown;
    bool            m_bAir;
    class CColliderOBB2D* m_pCylinderBody;
    class CSound*   m_pEft;
    class CSound*   m_pBGM;
	int		m_iRot;

public:
    void SetDelayMax(float fDelay)
    {
        m_fDelayMax = fDelay;
    }

    float GetDelay()    const
    {
        return m_fDelay;
    }

    bool IsTurned() const
    {
        return m_bTurned;
    }
    virtual void SetState(State eStat);

public:
    virtual bool Init();
    virtual bool Init(const char* pFileName, const char* pPathKey = DATA_PATH);
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CEyecube* Clone();
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void ColInit(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
    void ColEyeInit(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColEyeStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
    void ColEyeEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
    void Stop(float, float);
    void RotX(float, float);
    void RotY(float, float);
    void RotZ(float, float);

public:
    void EffectOpen(float);
    void EffectClose(float);
    void EffectLaser(float);
    void EffectLaserCharge(float);

public:
    void SpawnWindow();

public:
    void DisableCollider(float);
    void EnableCollider(float);
};

