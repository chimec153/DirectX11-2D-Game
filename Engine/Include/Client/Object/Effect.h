#pragma once
#include "Object\Obj.h"
class CEffect :
	public CObj
{
	friend class CScene;

private:
	CEffect();
	CEffect(const CEffect& eft);
	virtual ~CEffect();

private:
	float	m_fSpeed;
	Vector3	m_vScaleSpeed;
	float	m_fAlphaSpeed;

public:
	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	void SetScaleSpeed(const Vector3& vSpeed)
	{
		m_vScaleSpeed = vSpeed;
	}

	void SetAlphaSpeed(float fSpeed)
	{
		m_fAlphaSpeed = fSpeed;
	}

	void SetScaleCallBack(float);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CEffect* Clone();
	virtual void Notify(const std::string& strTag);
	void Destroy(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

