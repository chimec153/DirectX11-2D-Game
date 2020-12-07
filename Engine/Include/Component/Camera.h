#pragma once
#include "SceneComponent.h"
class CCamera :
	public CSceneComponent
{
	friend class CObj;
	friend class CCameraManager;

private:
	CCamera();
	CCamera(const CCamera& cam);
	virtual ~CCamera();

private:
	Matrix			m_matView;
	Matrix			m_matProj;
	CAMERA_TYPE		m_eType;
	float			m_fDist;
	float			m_fAngle;
	bool			m_bZoom;
	class CObj*		m_pTarget;
	class CObj*		m_pFocus;
	float			m_fMaxRange;
	float			m_fMinRange;
	Vector3			m_vPos;

public:
	Matrix GetViewMat()	const;
	Matrix GetProjMat()	const;
	void SetCameraType(CAMERA_TYPE eType);
	void SetAngle(float fAngle);
	void SetDist(float fDist);
	float GetAngle()	const;
	float GetDist()	const;
	void SetZoom(bool bZoom);
	void SetTarget(class CObj* pObj);
	void SetFocus(class CObj* pObj);
	void SetMax(float fMax);
	void SetMin(float fMin);
	void SetMovePos(const Vector3 vPos);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CCamera* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

