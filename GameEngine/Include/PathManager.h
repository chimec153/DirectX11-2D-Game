#pragma once

#include "GameEngine.h"

typedef struct _tagPath
{
	TCHAR*		pPath;
	char*		pMultibytePath;

	_tagPath()
	{
		pPath = new TCHAR[MAX_PATH];
		pMultibytePath = new char[MAX_PATH];

		memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);
		memset(pMultibytePath, 0, sizeof(char) * MAX_PATH);
	}

	~_tagPath()
	{
		SAFE_DELETE_ARRAY(pPath);
		SAFE_DELETE_ARRAY(pMultibytePath);
	}

}Path, *PPath;

class CPathManager
{
private:
	std::unordered_map<std::string, PPath>		m_mapPath;

public:
	bool Init();
	void AddPath(const std::string strName, const TCHAR* pPath, const std::string strRootPath = ROOT_PATH);

	const TCHAR* FindPath(const std::string strName);
	const char* FindMultibytePath(const std::string strName);

	DECLARE_SINGLE(CPathManager)
};

