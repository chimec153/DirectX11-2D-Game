#include "Mesh2D.h"
#include "../Device.h"

CMesh2D::CMesh2D() :
	m_tVB(),
	m_tIB(),
	m_eTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
{
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

bool CMesh2D::CreateMesh()
{
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexColor v[4] = {};

	v[0].vPos = Vector3(0.f, 1.f, 0.f);
	v[0].vColor = Vector4::Red;

	v[1].vPos = Vector3(1.f, 1.f, 0.f);
	v[1].vColor = Vector4::Green;

	v[2].vPos = Vector3(0.f, 0.f, 0.f);
	v[2].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

	v[3].vPos = Vector3(1.f, 0.f, 0.f);
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
		0, 1, 2,
		0, 2, 3
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
	//CONTEXT->IASetInputLayout();
}