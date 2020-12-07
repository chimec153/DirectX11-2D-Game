#include "SceneResource.h"
#include "../Resource/Shader.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"
#include "../Resource/Animation2DSequence.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	std::unordered_map<std::string, CMesh*>::iterator iterMesh = m_mapMesh.begin();
	std::unordered_map<std::string, CMesh*>::iterator iterMeshEnd = m_mapMesh.end();

	for (; iterMesh != iterMeshEnd; ++iterMesh)
	{
		GET_SINGLE(CResourceManager)->ReleaseMesh(iterMesh->first);
	}
		

	std::unordered_map<std::string, CShader*>::iterator iterShader = m_mapShader.begin();
	std::unordered_map<std::string, CShader*>::iterator iterShaderEnd = m_mapShader.end();

	for (; iterShader != iterShaderEnd; ++iterShader)
	{
		GET_SINGLE(CShaderManager)->ReleaseShader(iterShader->first);
	}

	std::unordered_map<std::string, CMaterial*>::iterator iterMaterial = m_mapMaterial.begin();
	std::unordered_map<std::string, CMaterial*>::iterator iterMaterialEnd = m_mapMaterial.end();

	for (; iterMaterial != iterMaterialEnd; ++iterMaterial)
	{
		GET_SINGLE(CResourceManager)->ReleaseMaterial(iterMaterial->first);
	}

	std::unordered_map<std::string, CTexture*>::iterator iterTex = m_mapTexture.begin();
	std::unordered_map<std::string, CTexture*>::iterator iterTexEnd = m_mapTexture.end();

	for (; iterTex != iterTexEnd; ++iterTex)
	{
		GET_SINGLE(CResourceManager)->ReleaseTexture(iterTex->first);
	}

	std::unordered_map<std::string, CAnimation2DSequence*>::iterator iterSeq = m_mapAni2DSeq.begin();
	std::unordered_map<std::string, CAnimation2DSequence*>::iterator iterSeqEnd = m_mapAni2DSeq.end();

	for (; iterSeq != iterSeqEnd; ++iterSeq)
	{
		GET_SINGLE(CResourceManager)->ReleaseAni2DSeq(iterSeq->first);
	}
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

CTexture* CSceneResource::FindTexture(const std::string& strTag)
{
	std::unordered_map<std::string, CTexture*>::iterator iter = m_mapTexture.find(strTag);

	if (iter == m_mapTexture.end())
	{
		CTexture* pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

		if (!pTex)
			return nullptr;

		m_mapTexture.insert(std::make_pair(strTag, pTex));

		return pTex;
	}

	iter->second->AddRef();

	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTexture(strTag, pFileName, strPathName))
		return false;

	pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CSceneResource::LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFileName)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(strTag, pFileName))
		return false;

	pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CSceneResource::LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTexture(strTag, vecFileName, strPathName))
		return false;

	pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CSceneResource::LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	if (!GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(strTag, vecFileName))
		return false;

	pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CSceneResource::CreateAni2DSeq(const std::string& strTag, CTexture* pTex, float fMaxTime, float fPlayRate)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	if (!GET_SINGLE(CResourceManager)->CreateAni2DSeq(strTag, pTex, fMaxTime, fPlayRate))
		return false;

	pSeq = GET_SINGLE(CResourceManager)->FindAni2DSeq(strTag);

	m_mapAni2DSeq.insert(std::make_pair(strTag, pSeq));

	return true;
}

bool CSceneResource::LoadAni2DSeq(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath, float fMaxTime, float fPlayRate)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	if (!GET_SINGLE(CResourceManager)->LoadAni2DSeq(strTag, pFileName, strRootPath, fMaxTime, fPlayRate))
		return false;

	pSeq = GET_SINGLE(CResourceManager)->FindAni2DSeq(strTag);

	m_mapAni2DSeq.insert(std::make_pair(strTag, pSeq));

	return true;
}

void CSceneResource::AddFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (!pSeq)
		return;

	pSeq->AddFrame(vStart, vEnd);

	SAFE_RELEASE(pSeq);
}

void CSceneResource::AddNotify(const std::string& strTag, const std::string& strNot, int iFrame)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (!pSeq)
		return;

	pSeq->AddNotify(strNot, iFrame);

	SAFE_RELEASE(pSeq);
}

CAnimation2DSequence* CSceneResource::FindAni2DSeq(const std::string& strTag)
{
	std::unordered_map<std::string, CAnimation2DSequence*>::iterator iter = m_mapAni2DSeq.find(strTag);

	if (iter == m_mapAni2DSeq.end())
	{
		CAnimation2DSequence* pSeq = GET_SINGLE(CResourceManager)->FindAni2DSeq(strTag);

		if (!pSeq)
			return nullptr;

		pSeq->AddRef();

		m_mapAni2DSeq.insert(std::make_pair(strTag, pSeq));

		return pSeq;
	}

	iter->second->AddRef();

	return iter->second;
}

void CSceneResource::Save(FILE* pFile)
{
	int iSize = (int)m_mapMaterial.size();

	fwrite(&iSize, 4, 1, pFile);

	std::unordered_map<std::string, CMaterial*>::iterator iterMaterial = m_mapMaterial.begin();
	std::unordered_map<std::string, CMaterial*>::iterator iterMaterialEnd = m_mapMaterial.end();

	for (int i = 0; i < iSize; ++i)
	{
		int iLength = (int)iterMaterial->first.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(iterMaterial->first.c_str(), 1, iLength, pFile);

		iLength = (int)iterMaterial->second->GetName().length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(iterMaterial->second->GetName().c_str(), 1, iLength, pFile);

		iterMaterial->second->Save(pFile);
	}

	iSize = (int)m_mapMesh.size();

	fwrite(&iSize, 4, 1, pFile);

	std::unordered_map<std::string, CMesh*>::iterator iterMesh = m_mapMesh.begin();
	std::unordered_map<std::string, CMesh*>::iterator iterMeshEnd = m_mapMesh.end();

	for (int i = 0; i < iSize; ++i)
	{
		int iLength = (int)iterMesh->first.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(iterMesh->first.c_str(), 1, iLength, pFile);

		iLength = (int)iterMesh->second->GetName().length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(iterMesh->second->GetName().c_str(), 1, iLength, pFile);
	}
}

void CSceneResource::Load(FILE* pFile)
{
	int iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		int iLength = 0;
		char strTag[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strTag, 1, iLength, pFile);

		char strName[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strName, 1, iLength, pFile);

		CMaterial* pMaterial = GET_SINGLE(CResourceManager)->CreateMaterial(strName);

		pMaterial->Load(pFile);

		m_mapMaterial.insert(std::make_pair(strTag, pMaterial));

		SAFE_RELEASE(pMaterial);
	}

	iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		int iLength = 0;

		char strTag[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strTag, 1, iLength, pFile);

		iLength = 0;

		char strName[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strName, 1, iLength, pFile);
	}
}
