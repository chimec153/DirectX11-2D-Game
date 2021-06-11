#include "Mesh2D.h"
#include "../Device.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"

CMesh2D::CMesh2D()
{
	m_eType = MESH_TYPE::MT_2D;
}

CMesh2D::~CMesh2D()
{
}

bool CMesh2D::Init()
{
	return true;
}

bool CMesh2D::CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, 
	void* pIdxData, int iIdxSz, int iIdxCnt, D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	size_t iSize = m_vecContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY(m_vecContainer[i]->tVB.pData);
		SAFE_RELEASE(m_vecContainer[i]->tVB.pBuffer);

		size_t iIBSize = m_vecContainer[i]->vecSubSet.size();

		for (size_t j = 0;j < iIBSize; j++)
		{
			SAFE_DELETE_ARRAY(m_vecContainer[i]->vecSubSet[j]->tIB.pData);
			SAFE_RELEASE(m_vecContainer[i]->vecSubSet[j]->tIB.pBuffer);
			SAFE_DELETE(m_vecContainer[i]->vecSubSet[j]);
		}

		SAFE_DELETE(m_vecContainer[i]);
	}

	m_vecContainer.clear();

	PMESHCONTAINER pContainer = new MESHCONTAINER;

	m_vecContainer.push_back(pContainer);

	pContainer->tVB = VertexBuffer();

	pContainer->eTopo = eTop;

	pContainer->tVB.iSize = iVtxSz;
	pContainer->tVB.iCount = iVtxCnt;
	pContainer->tVB.eUsage = eVtxUsg;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = pContainer->tVB.iSize * pContainer->tVB.iCount;
	tDesc.Usage = eVtxUsg;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	switch (tDesc.Usage)
	{
	case D3D11_USAGE_DYNAMIC:
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE |
			D3D11_CPU_ACCESS_READ;
		break;
	}

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = pVtxData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pContainer->tVB.pBuffer)))
		return false;

	m_tMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_tMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	pContainer->tVB.pData = new char[tDesc.ByteWidth];
	memcpy(pContainer->tVB.pData, pVtxData, tDesc.ByteWidth);

	char* pData = (char*)pVtxData;

	for (int i = 0; i < iVtxCnt; ++i)
	{
		PVector3 pVtx = (PVector3)pData;

		pData += iVtxSz;

		if (pVtx->x > m_tMax.x)
			m_tMax.x = pVtx->x;

		if (pVtx->y > m_tMax.y)
			m_tMax.y = pVtx->y;

		if (pVtx->z > m_tMax.z)
			m_tMax.z = pVtx->z;

		if (pVtx->x < m_tMin.x)
			m_tMin.x = pVtx->x;

		if (pVtx->y < m_tMin.y)
			m_tMin.y = pVtx->y;

		if (pVtx->z < m_tMin.z)
			m_tMin.z = pVtx->z;
	}

	if (pIdxData)
	{
		PSUBSET pSub = new SUBSET;
		pContainer->vecSubSet.push_back(pSub);
		IndexBuffer tIB = {};
		pSub->tIB = tIB;

		pSub->tIB.iSize = iIdxSz;
		pSub->tIB.iCount = iIdxCnt;
		pSub->tIB.eUsage = eIdxUsg;
		pSub->tIB.eFmt = eFmt;
		
		pSub->pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("Color");
		pSub->strShader = "Standard2D";

		D3D11_BUFFER_DESC tIndexDesc = {};

		tIndexDesc.ByteWidth = pSub->tIB.iSize * pSub->tIB.iCount;
		tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIndexDesc.Usage = eIdxUsg;

		switch (tIndexDesc.Usage)
		{
		case D3D11_USAGE_DYNAMIC:
			tIndexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case D3D11_USAGE_STAGING:
			tIndexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE |
				D3D11_CPU_ACCESS_READ;
			break;
		}
		pSub->tIB.pData = new char[tIndexDesc.ByteWidth];
		memcpy(pSub->tIB.pData, pIdxData, tIndexDesc.ByteWidth);

		D3D11_SUBRESOURCE_DATA tIndexData = {};

		tIndexData.pSysMem = pIdxData;

		if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &pSub->tIB.pBuffer)))
			return false;
	}

	return true;
}

void CMesh2D::Render(float fTime, int iContainer, int iSubset)
{
	UINT	iStride = m_vecContainer[iContainer]->tVB.iSize;
	UINT	iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_vecContainer[iContainer]->eTopo);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecContainer[iContainer]->tVB.pBuffer, &iStride, &iOffset);

	if (!m_vecContainer[iContainer]->vecSubSet.empty())
	{
		CONTEXT->IASetIndexBuffer(m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.pBuffer, m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.eFmt, 0);
		CONTEXT->DrawIndexed(m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.iCount, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(m_vecContainer[iContainer]->tVB.iCount, 0);
	}
}

void CMesh2D::RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize, int iContainer, int iSubset )
{
	UINT iStride[2] = { (UINT)m_vecContainer[iContainer]->tVB.iSize, (UINT)iSize };
	UINT iOffset[2] = { 0,0 };

	ID3D11Buffer* ppBuffer[2] = { m_vecContainer[iContainer]->tVB.pBuffer, (ID3D11Buffer*)pInstancedBuffer };

	CONTEXT->IASetVertexBuffers(0, 2, ppBuffer, iStride, iOffset);
	CONTEXT->IASetPrimitiveTopology(m_vecContainer[iContainer]->eTopo);

	if (m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.pBuffer, m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.eFmt, 0);
		CONTEXT->DrawIndexedInstanced(m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.iCount, iCnt, 0, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->DrawInstanced(m_vecContainer[iContainer]->tVB.iCount, iCnt, 0, 0);
	}
}

void CMesh2D::RenderParticle(int iCount, int iContainer, int iSubset)
{
	UINT iOffset = 0;
	UINT iStride = m_vecContainer[iContainer]->tVB.iSize;

	CONTEXT->IASetPrimitiveTopology(m_vecContainer[iContainer]->eTopo);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecContainer[iContainer]->tVB.pBuffer, &iStride, &iOffset);

	CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CONTEXT->DrawInstanced(m_vecContainer[iContainer]->tVB.iCount, iCount, 0, 0);
}

void CMesh2D::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CMesh2D::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
