#pragma once
#include "PostProcess.h"
class CDistortion :
	public CPostProcess
{
	friend class CSceneComponent;

private:
	CDistortion();
	CDistortion(const CDistortion& dis);
	virtual ~CDistortion();

private:
	DISTORTIONCBUFFER	m_tCBuffer;

public:
	void SetSpeed(float fSpeed);
	void SetAmp(float fAmp);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CDistortion* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

