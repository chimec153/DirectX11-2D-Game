#pragma once

#include "../Ref.h"

class CParticleSystem :
    public CRef
{
    friend class CResourceManager;

private:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& system);
    virtual ~CParticleSystem();

private:
    bool            m_bDynamic;
    D3D11_BUFFER_DESC   m_tDesc;
    ID3D11Buffer*   m_pBuffer;
    int             m_iCount;
    int             m_iSize;
    ID3D11ShaderResourceView* m_pSRV;
    ID3D11UnorderedAccessView*  m_pUAV;

    int             m_iRegister;
    int             m_iType;

    PARTICLECBUFFER m_tCBuffer;

public:
    bool Init(bool bDynamic, int iCount, int iSize, int iRegister, int iType);
    void SetShader(int iRegister);
    void ResetShader(int iRegister);
    void SetShader(int iRegister,int iType);
    void ResetShader(int iRegister, int iType);
    CParticleSystem* Clone();
    void SetStartColor(const Vector4& vColor);
    void SetEndColor(const Vector4& vColor);
    void SetStartScale(const Vector3& vScale);
    void SetEndScale(const Vector3& vScale);
    void SetMinSpeed(float fSpeed);
    void SetMaxSpeed(float fAddSpeed);
    void SetMoveDir(const Vector3& vDir);
    void SetMoveAngle(const Vector3& vAngle);
    void Set2D(bool b2D);
    void SetMove(bool bMove);
    void SetMaxLifeTime(float fTime);
    void SetMinLifeTime(float fTime);
    void SetMaxCount(unsigned int iCount);
    void SetRange(const Vector3& vRange);
    void SetAngle(float fRad);
    void SetAnim(int iFrameMax, float fFrameTimeMax);
    void SetAccel(const Vector3& vAcc);
    void CloneCBuffer(PARTICLECBUFFER& tBuffer);

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

