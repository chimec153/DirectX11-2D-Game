#pragma once

#include "GameEngine.h"

class CRef
{
protected:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef() = 0;

protected:
	int			m_iRef;
	std::string	m_strName;
	bool		m_bActive;
	bool		m_bEnable;

public:
	bool IsActive()	const
	{
		return m_bActive;
	}

	bool IsEnable()	const
	{
		return m_bEnable;
	}

	void Enable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void Destroy()
	{
		m_bActive = false;
	}

	std::string GetName()	const
	{
		return m_strName;
	}

	void SetName(const std::string& strName)
	{
		m_strName = strName;
	}

public:
	void AddRef();
	int Release();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

