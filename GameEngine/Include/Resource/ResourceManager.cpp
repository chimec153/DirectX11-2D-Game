#include "ResourceManager.h"
#include "Mesh2D.h"
#include "ShaderManager.h"
#include "Material.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()	:
	m_pDefaultMesh(nullptr)
{

}

CResourceManager::~CResourceManager()
{
	SAFE_RELEASE_MAP(m_mapMesh);
	SAFE_RELEASE_MAP(m_mapMaterial);
	SAFE_RELEASE(m_pDefaultMesh);
	DESTROY_SINGLE(CShaderManager);
}

CMesh2D* CResourceManager::GetDefaultMesh() const
{
	if (m_pDefaultMesh)
		m_pDefaultMesh->AddRef();

	return m_pDefaultMesh;
}

bool CResourceManager::Init()
{
	// Shader Manager ÃÊ±âÈ­
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	m_pDefaultMesh = new CMesh2D;

	if (!m_pDefaultMesh->CreateMesh())
		return false;

	CMaterial* pMaterial = CreateMaterial("Color");

	pMaterial->SetShader("Standard2D");

	m_pDefaultMesh->SetMaterial(pMaterial);

	SAFE_RELEASE(pMaterial);

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

CMaterial* CResourceManager::CreateMaterial(const std::string& strName)
{
	CMaterial* pMaterial = FindMaterial(strName);
	
	if (pMaterial)
		return pMaterial;

	pMaterial = new CMaterial;

	pMaterial->SetName(strName);

	pMaterial->AddRef();

	m_mapMaterial.insert(std::make_pair(strName, pMaterial));

	return pMaterial;
}

CMaterial* CResourceManager::FindMaterial(const std::string& strName)
{
	std::unordered_map<std::string, CMaterial*>::iterator iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::ReleaseMaterial(const std::string& strName)
{
	std::unordered_map<std::string, CMaterial*>::iterator iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
		return;

	if (iter->second->Release() == 0)
		m_mapMaterial.erase(iter);
}
