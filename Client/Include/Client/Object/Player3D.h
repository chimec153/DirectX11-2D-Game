#pragma once

#include "Object/Obj.h"
class CPlayer3D :
	public CObj
{
	friend class CScene;

private:
	CPlayer3D();
	CPlayer3D(const CPlayer3D& obj);
	virtual ~CPlayer3D();

private:
	class CMesh2DComponent* m_pCom;
	class CCamera* m_pCam;
	Vector3		m_vMovePos;
	class CNavigation2D* m_pNav;
	class CPaperBurn* m_pPaper;
	class CAnimation* m_pAni;
	class CTerrain* m_pTerrain;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CPlayer3D* Clone(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void MoveH(float, float);
	void MoveV(float, float);

public:
	void Space(float, float);

public:
	void Yaw(float, float);
	void Pitch(float, float);
	void Roll(float, float);
};

