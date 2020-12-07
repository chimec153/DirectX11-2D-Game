#pragma once
#include "Object\Obj.h"
class CBow :
	public CObj
{
public:
	CBow();
	CBow(const CBow& bow);
	virtual ~CBow();

private:
	class CSpriteComponent* m_pMesh;

public:
	void ChangeClip(const std::string& strTag);
	void SetDefaultClip(const std::string& strTag);
	void ReturnClip();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CBow* Clone();
};

