#pragma once
#include "Object\Obj.h"
class CMonster3D :
	public CObj
{
	friend class CScene;

private:
	CMonster3D();
	CMonster3D(const CMonster3D& mon);
	virtual ~CMonster3D() override;

	class CMesh2DComponent* m_pCom;
	class CMesh2DComponent* m_pChild;
	class CAnimation* m_pAni;

public:
	class CMesh2DComponent* GetCom()	const;
	class CAnimation* GetAni()	const;

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual CMonster3D* Clone() override;
public:
	virtual void Save(FILE* pFile) override; 
	virtual void Load(FILE* pFile) override;
};

