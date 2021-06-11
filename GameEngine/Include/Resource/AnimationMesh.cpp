#include "AnimationMesh.h"

CAnimationMesh::CAnimationMesh()
{
	m_eType = MESH_TYPE::MT_ANIMATION;
}

CAnimationMesh::~CAnimationMesh()
{
}

bool CAnimationMesh::Init()
{
	return true;
}

bool CAnimationMesh::CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, void* pIdxData, int iIdxSz, int iIdxCnt, D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	return CMesh::CreateMesh(eTop, pVtxData, iVtxSz, iVtxCnt, eVtxUsg, pIdxData,iIdxSz, iIdxCnt, eIdxUsg, eFmt);
}

void CAnimationMesh::Render(float fTime, int iContainer, int iSubset)
{
	CMesh::Render(fTime, iContainer, iSubset);
}

void CAnimationMesh::RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize, int iContainer, int iSubset)
{
	CMesh::RenderInstancing(pInstancedBuffer, iCnt, iSize, iContainer, iSubset);
}

void CAnimationMesh::RenderParticle(int iCount, int iContainer, int iSubset)
{
	CMesh::RenderParticle(iCount, iContainer, iSubset);
}

void CAnimationMesh::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CAnimationMesh::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
