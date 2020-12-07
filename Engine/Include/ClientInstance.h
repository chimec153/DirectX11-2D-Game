#pragma once

#include "CreateInstance.h"

class CClientInstance :
	public CCreateInstance
{
	friend class CEngine;

private:
	CClientInstance();
	~CClientInstance();

public:
	bool Init();
};

