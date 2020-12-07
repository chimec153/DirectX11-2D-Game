#pragma once
#include "Object\Obj.h"
class CRadioObj :
	public CObj
{
	friend class CScene;
	friend class CViewPort;

private:
	CRadioObj();
	CRadioObj(const CRadioObj& obj);
	virtual ~CRadioObj();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CRadioObj* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);


};

