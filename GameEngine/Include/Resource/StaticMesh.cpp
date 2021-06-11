#include "StaticMesh.h"
#include "FBXLoader.h"
#include "../PathManager.h"

CStaticMesh::CStaticMesh()
{
	m_eType = MESH_TYPE::MT_STATIC;

}

CStaticMesh::~CStaticMesh()
{
}

bool CStaticMesh::Init()
{
	return true;
}

bool CStaticMesh::CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, void* pIdxData, int iIdxSz, int iIdxCnt, D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	return CMesh::CreateMesh(eTop, pVtxData, iVtxSz, iVtxCnt, eVtxUsg, pIdxData, iIdxSz, iIdxCnt, eIdxUsg, eFmt);
}

void CStaticMesh::Render(float fTime, int iContainer, int iSubset)
{
	CMesh::Render(fTime, iContainer, iSubset);
}

void CStaticMesh::RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize, int iContainer, int iSubset)
{
	CMesh::RenderInstancing(pInstancedBuffer, iCnt, iSize, iContainer, iSubset);
}

void CStaticMesh::RenderParticle(int iCount, int iContainer, int iSubset)
{
	CMesh::RenderParticle(iCount,iContainer, iSubset);
}

bool CStaticMesh::LoadMesh(const std::string& strName, const TCHAR* pFileName, const std::string& strPath)
{
	return CMesh::LoadMesh(strName, pFileName, strPath);
}

bool CStaticMesh::LoadMeshFullPath(const std::string& strName, const TCHAR* pFullPath)
{
	return CMesh::LoadMeshFullPath(strName, pFullPath);
}

bool CStaticMesh::ConvertFBX(CFBXLoader* pLoader, const char* pFullPath)
{
	return CMesh::ConvertFBX(pLoader, pFullPath);
}

bool CStaticMesh::LoadMesh(const char* pFullPath)
{
	return CMesh::LoadMesh(pFullPath);
}

bool CStaticMesh::SaveMesh(const char* pFullPath)
{
	return CMesh::SaveMesh(pFullPath);
}


void CStaticMesh::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CStaticMesh::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
