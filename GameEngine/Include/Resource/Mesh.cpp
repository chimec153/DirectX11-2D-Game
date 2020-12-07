#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "../Device.h"

CMesh::CMesh()	:
	m_pMaterial(nullptr)
{

}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pMaterial);
}

void CMesh::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);

	m_pMaterial = pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}

CMaterial* CMesh::GetMaterial() const
{
	if (m_pMaterial)
		m_pMaterial->AddRef();

	return m_pMaterial;
}

bool CMesh::Init()
{
	return true;
}

bool CMesh::CreateInstancingBuffer(int iCount, int iSize)
{
	m_pInstancingBuffer = new INSTANCINGBUFFER;

	m_pInstancingBuffer->iCount = iCount;
	m_pInstancingBuffer->iSize = iSize;
	m_pInstancingBuffer->pData = new char[m_pInstancingBuffer->iCount * m_pInstancingBuffer->iSize];
	m_pInstancingBuffer->eUsg = D3D11_USAGE_DYNAMIC;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = m_pInstancingBuffer->iCount * m_pInstancingBuffer->iSize;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstancingBuffer->pBuffer)))
	{
		delete m_pInstancingBuffer;
		return false;
	}

	return true;
}

bool CMesh::CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, 
	D3D11_USAGE eVtxUsg,  void* pIdxData, int iIdxSz, int iIdxCnt, 
	D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	return false;
}

void CMesh::Render(float fTime)
{
}

void CMesh::RenderInstancing(void* pData, int iCount, int iSize)
{
}

void CMesh::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_tMax, sizeof(Vector3), 1, pFile);
	fwrite(&m_tMin, sizeof(Vector3), 1, pFile);

	int iLength = (int)m_pMaterial->GetName().length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pMaterial->GetName().c_str(), 1, iLength, pFile);

	m_pMaterial->Save(pFile);
}

void CMesh::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_tMax, sizeof(Vector3), 1, pFile);
	fread(&m_tMin, sizeof(Vector3), 1, pFile);

	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strTag, 1, iLength, pFile);

	CMaterial* pMat = GET_SINGLE(CResourceManager)->CreateMaterial(strTag);

	pMat->Load(pFile);

	SetMaterial(pMat);

	SAFE_RELEASE(pMat);
}
