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
	float			m_fZoom;
	Matrix			m_matVP;
	Resolution		m_tRS;
	RectInfo		m_tRect;
	float			m_fZoomLimit;
	std::function<void(float)>	m_pCallBack;
	Vector2			m_vZoomCenter;
	Vector4			m_pPlanes[static_cast<int>(FRUSTUM_PLANE::END)];

public:
	const Matrix& GetViewMat()	const;
	const Matrix& GetProjMat()	const;
	const Matrix& GetVP()	const;
	void SetCameraType(CAMERA_TYPE eType);
	void SetAngle(float fAngle);
	void SetDist(float fDist);
	float GetAngle()	const;
	float GetDist()	const;
	void SetZoom(bool bZoom, float fDist = 0.25f);
	void SetTarget(class CObj* pObj);
	void SetFocus(class CObj* pObj);
	void SetMax(float fMax);
	void SetMin(float fMin);
	void SetMovePos(const Vector3 vPos);
	void SetRect(float l, float t, float r, float b);
	const Resolution& GetResolution()	const;
	void SetCallBack(void(*pFunc)(float));
	void SpawnControlWindow();
	void Reset();
	void SetZoomDist(float fDist);
	void SetZoomCenter(const Vector2& vCenter = Vector2(0.5f, 0.5f));
	void SetZoomCenter(float fX = 0.5f, float fY = 0.5f);
	bool IsInFrustum(const Vector3& vPos, float fRadius)	const;

	template<typename T>
	void SetCallBack(T* pObj, void(T::* pFunc)(float))
	{
		m_pCallBack = std::bind(pFunc, pObj, std::placeholders::_1);
	}
	const RectInfo& GetRect()	const;

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

