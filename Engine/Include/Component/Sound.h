#pragma once
#include "SceneComponent.h"
class CSound :
	public CSceneComponent
{
	friend class CObj;

private:
	CSound();
	CSound(const CSound& snd);
	virtual ~CSound();

private:
	std::string		m_strName;
	PSOUNDINFO		m_pSound;
	float			m_fMax;
	float			m_fMin;
	float			m_fMaxVol;
	float			m_fMinVol;

public:
	void SetVol(float fVol);
	void Play(float fTime);
	void Stop();
	void SetSound(const std::string& strKey);
	void SetMax(float fMax);
	void SetMin(float fMin);
	void SetMaxVol(float fMax);
	void SetMinVol(float fMin);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSound* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

