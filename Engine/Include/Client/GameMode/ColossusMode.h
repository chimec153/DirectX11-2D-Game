#pragma once
#include "TileMode.h"

class CColossusMode :
	public CTileMode
{
	friend class CScene;

private:
	CColossusMode();
	virtual ~CColossusMode();

private:
	static bool m_bClear;

public:
	void Clear()
	{
		m_bClear = true;
	}

public:
	bool Init();
	void Start();
	bool LoadXml(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	void searchXMLData(class TiXmlElement* pElem);
	void LoadResource();
};

