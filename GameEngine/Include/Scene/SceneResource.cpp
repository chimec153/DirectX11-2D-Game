#include "SceneResource.h"
#include "../Resource/Shader.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Material.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	std::unordered_map<std::string, CMesh*>::iterator iterMesh = m_mapMesh.begin();
	std::unordered_map<std::string, CMesh*>::iterator iterMeshEnd = m_mapMesh.end();

	for (; iterMesh != iterMeshEnd; ++iterMesh)
		GET_SINGLE(CResourceManager)->ReleaseMesh(iterMesh->first);

	std::unordered_map<std::string, CShader*>::iterator iterShader = m_mapShader.begin();
	std::unordered_map<std::string, CShader*>::iterator iterShaderEnd = m_mapShader.end();

	for (; iterShader != iterShaderEnd; ++iterShader)
		GET_SINGLE(CShaderManager)->ReleaseShader(iterShader->first);

	std::unordered_map<std::string, CMaterial*>::iterator iterMaterial = m_mapMaterial.begin();
	std::unordered_map<std::string, CMaterial*>::iterator iterMaterialEnd = m_mapMaterial.end();

	for (; iterMaterial != iterMaterialEnd; ++iterMaterial)
		GET_SINGLE(CResourceManager)->ReleaseMaterial(iterMaterial->first);
}

bool CSceneResource::Init()
{
	return true;
}

CMesh* CSceneResource::FindMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
	{
		CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

		if (!pMesh)
			return nullptr;

		pMesh->AddRef();

		m_mapMesh.insert(std::make_pair(strName, pMesh));

		return pMesh;
	}

	iter->second->AddRef();

	return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& strName)
{
	std::unordered_map<std::string, CMaterial*>::iterator iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
	{
		CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial(strName);

		if (!pMaterial)
			return nullptr;

		pMaterial->AddRef();

		m_mapMaterial.insert(std::make_pair(strName, pMaterial));

		return pMaterial;
	}

	iter->second->AddRef();

	return iter->second;
}
