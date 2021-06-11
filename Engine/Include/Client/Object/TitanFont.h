#pragma once
#include "Object\Obj.h"
class CTitanFont :
	public CObj
{
	friend class CScene;

private:
	CTitanFont();
	CTitanFont(const CTitanFont& obj);
	virtual ~CTitanFont();

private:
	std::string m_strText;
	int			m_iComCount;

public:
	void SetText(const std::string& strText, float fWidth, const Vector4& vColor = Vector4(1.f,1.f,1.f,1.f));
	void SetAlpha(float fAlpha);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CTitanFont* Clone();
};

