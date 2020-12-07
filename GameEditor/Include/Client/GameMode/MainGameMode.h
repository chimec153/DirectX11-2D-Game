#pragma once

#include "TileMode.h"

class CMainGameMode :
	public CTileMode
{
public:
	CMainGameMode();
	~CMainGameMode();

public:
	bool Init();

public:
	void LoadResource();
	void StartScene(float);
	void CreateUI();
	bool LoadXml(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	void searchXMLData(class TiXmlElement* pElem);

};
