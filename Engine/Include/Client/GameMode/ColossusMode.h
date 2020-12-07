#pragma once
#include "TileMode.h"

class CColossusMode :
	public CTileMode
{
	friend class CScene;

private:
	CColossusMode();
	virtual ~CColossusMode();

public:
	bool Init();
	bool LoadXml(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	void searchXMLData(class TiXmlElement* pElem);
	void LoadResource();
};

