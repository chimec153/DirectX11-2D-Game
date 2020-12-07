#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{

}

CPathManager::~CPathManager()
{
	SAFE_DELETE_MAP(m_mapPath);
}

bool CPathManager::Init()
{
	TCHAR strRootPath[MAX_PATH] = {};

	GetModuleFileName(0, strRootPath, MAX_PATH);

	for (int i = lstrlen(strRootPath) - 1; i > 0; --i)
	{
		if (strRootPath[i] == '\\' || strRootPath[i] == '/')
		{
			memset(strRootPath + i + 1, 0, (lstrlen(strRootPath) - i - 1) * sizeof(TCHAR));

			break;
		}
	}

	PPath pPath = new Path;

	lstrcat(pPath->pPath, strRootPath);

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, pPath->pPath, -1, pPath->pMultibytePath, lstrlen(pPath->pPath), NULL, NULL);
#else
	strcpy_s(pPath->pMultibytePath, pPath->pPath);
#endif

	m_mapPath.insert(std::make_pair(ROOT_PATH, pPath));

	AddPath(RESOURCE_PATH, TEXT("Resource\\"));
	AddPath(TEXTURE_PATH, TEXT("Data\\IMG\\"));
	AddPath(DATA_PATH, TEXT("Data\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(SHADER_PATH, TEXT("Resource\\Shader\\"));
	AddPath(MAP_PATH, TEXT("Data\\MAPS\\"));

	return true;
}

void CPathManager::AddPath(const std::string strName, const TCHAR* pPath, const std::string strRootPath)
{
	PPath pNewPath = new Path;

	const TCHAR* pRootPath = FindPath(strRootPath);

	if (pRootPath)
		lstrcat(pNewPath->pPath, pRootPath);

	lstrcat(pNewPath->pPath, pPath);

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, pNewPath->pPath, -1, pNewPath->pMultibytePath, lstrlen(pNewPath->pPath), NULL, NULL);
#else
	strcpy_s(pNewPath->pPath, pNewPath->pMultibytePath);
#endif

	m_mapPath.insert(std::make_pair(strName, pNewPath));
}

const TCHAR* CPathManager::FindPath(const std::string strName)
{
	std::unordered_map<std::string, PPath>::iterator iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second->pPath;
}

const char* CPathManager::FindMultibytePath(const std::string strName)
{
	std::unordered_map<std::string, PPath>::iterator iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second->pMultibytePath;
}
