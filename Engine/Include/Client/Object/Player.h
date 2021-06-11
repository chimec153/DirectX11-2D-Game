#pragma once

#include "Object/Obj.h"

enum class STATE
{
	IDLE,
	WALK,
	ROLL,
	SWIM,
	ATTACK,
	CALL,
	DIE,
	RUN,
	END
};

enum class MAT_TYPE
{
	GRASS,
	DIRT,
	WATER_2,
	PURPLE,
	CROSS,
	FOREST,
	RHOMBUS,
	SNOW,
	ICE,
	MUD,
	SOIL,
	TILE,
	L,
	R,
	O,
	WATER,
	MAGMA,
	GREEN_GRASS,
	TILE_BLUE,
	RIGHT_UP,
	LEFT_UP,
	UP_DOWN,
	UP,
	DOWN,
	ICE_UP,
	RAIN_W,
	UP_DOWN_BLUE,
	LIFT_UP,
	LIFT_UP_BLUE,
	END
};

class CPlayer :
	public CObj
{
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	virtual ~CPlayer();

private:
	class CSpriteComponent* m_pMesh;
	float						m_fSpeed;
	bool						m_bIdleEnable;
	float						m_fScaleX;
	float						m_fScaleY;
	DIR_8						m_eDir;
	bool						m_bCharge;
	bool						m_bRolling;
	class CArrow* m_pArrow;
	class CBow* m_pBow;
	class CBullet* m_pBullet;
	float			m_fBowAngle;
	class CTileMap* m_pTileMap;
	class CUIFont*	m_pCoord;
	class CSpriteComponent* m_pShadow;
	class CColliderRect* m_pRC;
	STATE						m_eState;
	float						m_fPushTime;
	float						m_fRollTime;
	float						m_fPrevSpeed;
	class CSound* m_pSnd;
	class CTileMap* m_pMtrlMap;
	float						m_fBackTime;
	bool						m_bBack;
	float						m_fBackLimit;
	class CTileMap* m_pAirMap;
	bool	m_bWalkIn;
	float	m_fWalkTime;
	std::string	m_strMap;
	float		m_fDieDelay;
	bool		m_bBurn;
	class CSpriteComponent* m_pShadowDown;
	float		m_fFlyDist;
	bool		m_bFly;
	int			m_iSouls;
	float		m_fLightTime;
	float		m_fLightTimeLimit;
	int			m_iLights;
	float		m_fWhiteTime;
	float		m_fWhiteTimeLimit;
	class CParticle* m_pGrass;
	MAT_TYPE	m_eMatType;
	bool		m_bReady;
	char		m_cCol;
	class CParticle* m_pTileDust;
	class CParticle* m_pWave;
	class CParticle* m_pSplash;
	class CParticle* m_pBlood;
	static float	m_fPlayTime;
	bool		m_bImpact;
	class CSound* m_pBGM;
	static int		m_iDeath;
	static bool	m_bMenu;
	static int	m_iMenu;
	static const float m_fWalkSpeed;
	static bool		m_bSlow;
	static float	m_fSlowTime;
	class CTileMap* m_pDebugMap;
	//bool		m_bPushC;
	bool		m_bStair;
	Vector3		m_vMovePos;
	
public:
	void SetMeshComponent(class CSpriteComponent* pMesh);
	virtual void Notify(const std::string& strTag);
	void Charge();
	bool IsCharged()	const
	{
		return m_bCharge;
	}

	void SetBurn()
	{
		m_bBurn = true;
	}

	void SetFly(bool bFly);

	void AddSoul();
	class CSpriteComponent* GetMesh()	const;

	float GetPlayTime()	const
	{
		return m_fPlayTime;
	}

	void SetPlayTime(float fTime)
	{
		m_fPlayTime = fTime;
	}

	void SetImpact(bool bImpact)
	{
		m_bImpact = bImpact;
	}
	void SetBow(class CBow* pObj);
	void SetArrow(class CArrow* pObj);

	int GetDeath()	const
	{
		return m_iDeath;
	}
	void SetDeath(int iDeath)
	{
		m_iDeath = iDeath;
	}
	void SetDir(DIR_8 eDir);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CPlayer* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void MoveV(float fScale, float fTime);
	void MoveH(float fScale, float fTime);
	float GetAngle();

public:
	void CloseUp(float fScale, float fTime);
	void CloseUpEnd(float fScale, float fTime);
	void ReturnArrow(float fPushTime, float fTime);
	void Attack(float fPushTime, float fTime);
	void AttackFire(float fPushTime, float fTime);

public:
	void ShiftDown(float fPushTIme, float fTime);
	void ShiftUp(float fPushTIme, float fTime);
	void RollEnd();

public:
	void ColStart(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);

public:
	void Option(float fPushTime, float fTime);

public:
	void Stage1(float fScale, float fTime);
	void Stage2(float fScale, float fTime);
	void Stage3(float fScale, float fTime);
	void ChangeStage(const char* pStage);

public:
	char TileMapCol(class CTileMap* pMap);
	void TileMatToggle(float fScale, float fTime);

public:
	void ChangeSequenceAll(const char* pSeq);
	void SetDefaultClipAll(const char* pSeq);
	void SetWorldRotYAll(float fRot);

public:
	void SetState(STATE eState);
	void Water(float fTime);
	void LookBack();
	void Die(float fTime);
	void Step(int iFrame, float fTime);

public:
	void MenuLeft(float fPushTime, float fTime);
	void MenuRight(float fPushTime, float fTime);
	void MenuUpdate();

public:
	void SpawnWindow();

public:
	void Mucus();
};

