#pragma once
#include "SoulMonster.h"

enum class ATTACK_STAGE
{
	IDLE,
	FIND,
	READY,
	DOWN,
	END
};

class CColossus :
	public CSoulMonster
{
	friend class CScene;
	friend class CLayer;
	friend class CBossManager;

private:
	CColossus();
	CColossus(const CColossus& obj);
	virtual ~CColossus();

private:
	class CSpriteComponent* m_pHead;
	class CSpriteComponent* m_pBody;
	class CSpriteComponent* m_pLeftHand;
	class CSpriteComponent* m_pRightHand;
	class CSpriteComponent* m_pLight;
	class CSpriteComponent* m_pLeftHandShadow;
	class CSpriteComponent* m_pRightHandShadow;
	bool		m_bRising;
	bool		m_bLeft;
	ATTACK_STAGE	m_eAttackStage;
	float		m_fReadyTime;
	float		m_fDownDist;
	static float		m_fLimitDist;
	static float		m_fLimitDist2;
	static float		m_fHandSpeed;
	float		m_fInertiaTime;
	static float		m_fInertiaLimit;
	static float		m_fMoveSpeed;
	class CParticle* m_pParticleLeft;
	class CParticle* m_pParticleRight;
	bool		m_bSleep;
	Vector3		m_vTargetPos;
	class CSound* m_pBGM;
	class CSound* m_pBGM2;
	static float		m_fAccel;
	float			m_fSpeed;
	const static float	m_fReturnSpeed;
	bool		m_bFirst;
	const static float	m_fLiftSpeed;

public:
	const ATTACK_STAGE GetStage()	const
	{
		return m_eAttackStage;
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
	virtual CColossus* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColStart(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColLast(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
	virtual void SetState(State eState);

public:
	void Cry(float fTime);
	void ReturnCam(float fTime);
	void StopShaking(float fTime);

public:
	void Cheat(float, float);
	void SpawnWindow();
};

