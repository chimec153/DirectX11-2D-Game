#pragma once
class CCreateInstance
{
	friend class CEngine;

protected:
	CCreateInstance();
	~CCreateInstance();

public:
	bool Init();
};

