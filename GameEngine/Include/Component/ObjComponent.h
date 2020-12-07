#pragma once
#include "Component.h"
class CObjComponent :
	public CComponent
{
	friend class CObj;

protected:
	CObjComponent();
	CObjComponent(const CObjComponent& com);
	virtual ~CObjComponent();

public:
	void SetScene(class CScene* pScene);

public:
	virtual CObjComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

