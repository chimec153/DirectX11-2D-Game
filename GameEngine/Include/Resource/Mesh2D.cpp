#include "Mesh2D.h"
#include "../Device.h"

CMesh2D::CMesh2D() :
	m_ePrimitive(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
{
	memset(&m_tVB, 0, sizeof(VertexBuffer));
	memset(&m_tIB, 0, sizeof(IndexBuffer));
}

CMesh2D::~CMesh2D()
{
	SAFE_RELEASE(m_tVB.pBuffer);
	SAFE_RELEASE(m_tIB.pBuffer);
}

bool CMesh2D::Init()
{
	return true;
}

bool CMesh2D::CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, 
	void* pIdxData, int iIdxSz, int iIdxCnt, D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	m_ePrimitive = eTop;

	m_tVB.iSize = iVtxSz;
	m_tVB.iCount = iVtxCnt;
	m_tVB.eUsage = eVtxUsg;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = m_tVB.iSize * m_tVB.iCount;
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

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_tVB.pBuffer)))
		return false;

	m_tMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_tMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

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
		m_tIB.iSize = iIdxSz;
		m_tIB.iCount = iIdxCnt;
		m_tIB.eUsage = eIdxUsg;
		m_tIB.eFmt = eFmt;

		D3D11_BUFFER_DESC tIndexDesc = {};

		tIndexDesc.ByteWidth = m_tIB.iSize * m_tIB.iCount;
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

		D3D11_SUBRESOURCE_DATA tIndexData = {};

		tIndexData.pSysMem = pIdxData;

		if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &m_tIB.pBuffer)))
			return false;
	}

	return true;
}

bool CMesh2D::CreateMesh()
{
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexColor v[4] = {};

	v[0].vPos = Vector3(0.f, 1.f, 0.f);
	v[0].vUV = Vector2(0.f, 0.f);
	v[0].vColor = Vector4::Red;

	v[1].vPos = Vector3(1.f, 1.f, 0.f);
	v[1].vUV = Vector2(1.f, 0.f);
	v[1].vColor = Vector4::Green;

	v[2].vPos = Vector3(0.f, 0.f, 0.f);
	v[2].vUV = Vector2(0.f, 1.f);
	v[2].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

	v[3].vPos = Vector3(1.f, 0.f, 0.f);
	v[3].vUV = Vector2(1.f, 1.f);
	v[3].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

	m_tVB.iSize = sizeof(VertexColor);
	m_tVB.iCount = 4;
	m_tVB.eUsage = D3D11_USAGE_IMMUTABLE;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = m_tVB.iSize * m_tVB.iCount;
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = v;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_tVB.pBuffer)))
		return false;

	m_tMax = Vector3(1.f, 1.f, 0.f);
	m_tMin = Vector3(0.f, 0.f, 0.f);

	unsigned short iIdx[6] = {
		0, 1, 3,
		0, 3, 2
	};

	m_tIB.iSize = sizeof(unsigned short);
	m_tIB.iCount = 6;
	m_tIB.eUsage = D3D11_USAGE_IMMUTABLE;
	m_tIB.eFmt = DXGI_FORMAT_R16_UINT;

	D3D11_BUFFER_DESC tIndexDesc = {};

	tIndexDesc.ByteWidth = m_tIB.iSize * m_tIB.iCount;
	tIndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIndexDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA tIndexData = {};

	tIndexData.pSysMem = iIdx;

	if (FAILED(DEVICE->CreateBuffer(&tIndexDesc, &tIndexData, &m_tIB.pBuffer)))
		return false;

	return true;
}

void CMesh2D::Render(float fTime)
{
	UINT	iStride = m_tVB.iSize;
	UINT	iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_ePrimitive);
	CONTEXT->IASetVertexBuffers(0, 1, &m_tVB.pBuffer, &iStride, &iOffset);

	if (m_tIB.pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_tIB.pBuffer, m_tIB.eFmt, 0);
		CONTEXT->DrawIndexed(m_tIB.iCount, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->Draw(m_tVB.iCount, 0);
	}
}

void CMesh2D::RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize)
{
	UINT iStride[2] = { sizeof(VertexColor), (UINT)iSize };
	UINT iOffset[2] = { 0,0 };

	ID3D11Buffer* ppBuffer[2] = { m_tVB.pBuffer, (ID3D11Buffer*)pInstancedBuffer };

	CONTEXT->IASetVertexBuffers(0, 2, ppBuffer, iStride, iOffset);
	CONTEXT->IASetPrimitiveTopology(m_ePrimitive);

	if (m_tIB.pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_tIB.pBuffer, m_tIB.eFmt, 0);
		CONTEXT->DrawIndexedInstanced(m_tIB.iCount, iCnt, 0, 0, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->DrawInstanced(m_tVB.iCount, iCnt, 0, 0);
	}
}

void CMesh2D::Save(FILE* pFile)
{
	CMesh::Save(pFile);

	fwrite(&m_ePrimitive, 4, 1, pFile);
}

void CMesh2D::Load(FILE* pFile)
{
	CMesh::Load(pFile);

	fread(&m_ePrimitive, 4, 1, pFile);

	CreateMesh();
}
