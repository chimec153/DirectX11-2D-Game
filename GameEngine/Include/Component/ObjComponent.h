#pragma once
#include "Component.h"
class CObjComponent :
	public CComponent
{
	friend class CObj;

protected:
	CObjComponent();
	CObjComponent(const CObjComponent& com);
	virtual ~CObjComponent() = 0;
};

