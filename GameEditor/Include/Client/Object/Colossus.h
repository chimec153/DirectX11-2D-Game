#pragma once
#include "Object\Obj.h"
class CColossus :
	public CObj
{
	friend class CScene;
	friend class CLayer;

private:
	CColossus();
	CColossus(const CColossus& obj);
	virtual ~CColossus();

private:
	class CSpriteComponent* m_pHead;
	class CSpriteComponent* m_pBody;
	class CSpriteComponent* m_pLeftHand;
	class CSpriteComponent* m_pRightHand;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColossus* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

