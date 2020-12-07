#pragma once

#include "../Component/SceneComponent.h"

class CUIControl :
	public CSceneComponent
{
	friend class CUIObject;

protected:
	CUIControl();
	CUIControl(const CUIControl& con);
	virtual ~CUIControl() = 0;

protected:
	bool				m_bStart;
	class CViewPort*	m_pViewPort;
	class CTexture*		m_pTexture;
	class CMesh2D*		m_pImage;
	int					m_iZOrder;

public:
	void SetTexture(const std::string& strTex);
	void SetTexture(class CTexture* pTex);
	void SetZOrder(int iZ);
	int GetZOrder()	const;

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
	virtual CUIControl* Clone() = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

