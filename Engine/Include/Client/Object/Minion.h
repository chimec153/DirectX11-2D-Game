#pragma once
#include "Object\Obj.h"
class CMinion :
	public CObj
{
	friend class CScene;

private:
	CMinion();
	CMinion(const CMinion& min);
	virtual ~CMinion();

private:
	class CSpriteComponent* m_pMesh;
	float					m_fFireLimitTime;
	float					m_fTime;
	int						m_iPhase;
	bool					m_bIdleEnable;
	bool					m_bRolling;
	float					m_fDist;
	Vector3					m_vDir;
	float					m_fSpeed;
	float					m_fTargetAngle;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMinion* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void AttackEnd();
	virtual void Notify(const std::string& strTag);
	void Fire();
	void RollToPlayer();
};

