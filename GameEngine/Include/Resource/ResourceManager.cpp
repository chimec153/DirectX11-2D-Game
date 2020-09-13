#include "ResourceManager.h"
#include "Mesh2D.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()	:
	m_pDefaultMesh(nullptr)
{

}

CResourceManager::~CResourceManager()
{
	SAFE_RELEASE_MAP(m_mapMesh);
	SAFE_RELEASE(m_pDefaultMesh);
}

CMesh2D* CResourceManager::GetDefaultMesh() const
{
	if (m_pDefaultMesh)
		m_pDefaultMesh->AddRef();

	return m_pDefaultMesh;
}

bool CResourceManager::Init()
{
	m_pDefaultMesh = new CMesh2D;

	m_pDefaultMesh->CreateMesh();

	return true;
}

bool CResourceManager::CreateMesh(const std::string& strName)
{
	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::ReleaseMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return;
		
	if (iter->second->Release() == 0)
		m_mapMesh.erase(iter);
}