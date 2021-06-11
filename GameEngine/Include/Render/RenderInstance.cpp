#include "RenderInstance.h"
#include "../Device.h"
#include "../Resource/Mesh.h"
#include "../Resource/Shader.h"
#include "../Resource/Material.h"
#include "../Resource/ShaderManager.h"

CRenderInstance::CRenderInstance()	:
	m_pMesh(nullptr)
	, m_pShader(nullptr)
	, m_iCount(0)
	, m_pInstBuffer(nullptr)
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_iBoneCount(0)
	, m_pShadowShader(nullptr)
{
}

CRenderInstance::~CRenderInstance()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pShadowShader);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);

	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		SAFE_RELEASE_VECLIST(m_vecMaterial[i]);
	}

	if (m_pInstBuffer)
	{
		SAFE_RELEASE(m_pInstBuffer->pBuffer);
		SAFE_DELETE_ARRAY(m_pInstBuffer->pData);
	}
	SAFE_DELETE(m_pInstBuffer);
}

void CRenderInstance::AddData(void* pData)
{
	memcpy_s(
		static_cast<char*>(m_pInstBuffer->pData) + m_pInstBuffer->iSize * m_iCount, 
		(m_pInstBuffer->iCount - m_iCount) * m_pInstBuffer->iSize,
		pData, m_pInstBuffer->iSize);
	++m_iCount;

	if (m_iCount == m_pInstBuffer->iCount)
	{
		m_pInstBuffer->iCount = static_cast<size_t>(m_pInstBuffer->iCount * 1.5f);

		size_t iTotalSize = static_cast<size_t>(m_pInstBuffer->iSize) * m_pInstBuffer->iCount;
		char* pData = new char[iTotalSize];

		memset(pData, 0, iTotalSize);

		memcpy_s(pData, m_iCount * m_pInstBuffer->iSize, m_pInstBuffer->pData, m_iCount * m_pInstBuffer->iSize);

		SAFE_DELETE_ARRAY(m_pInstBuffer->pData);

		CreateInstBuffer(m_pInstBuffer->iSize, m_pInstBuffer->iCount);

		m_pInstBuffer->pData = pData;

		CreateBoneBuffer(m_pInstBuffer->iCount);
	}
}

void CRenderInstance::AddComponent(CSceneComponent* pCom)
{
	m_ComponentList.push_back(pCom);
}

void CRenderInstance::DeleteAllComponent()
{
	m_ComponentList.clear();
}

const std::list<class CSceneComponent*>& CRenderInstance::GetComponentList() const
{
	return m_ComponentList;
}

int CRenderInstance::GetCount() const
{
	return static_cast<int>(m_iCount);
}

ID3D11UnorderedAccessView* CRenderInstance::GetBoneUav() const
{
	return m_pUAV;
}

bool CRenderInstance::Init(CMesh* pMesh, CShader* pShader, 
	size_t iSize, size_t iMaxCount, int iBoneCount)
{
	m_pShader = pShader;

	if (!CreateInstBuffer(static_cast<int>(iSize), static_cast<int>(iMaxCount)))
		return false;

	m_pInstBuffer->pData = new char[iSize * iMaxCount];

	memset(m_pInstBuffer->pData, 0, iSize * iMaxCount);

	if (m_pShader)
	{
		m_pShader->AddRef();
	}

	m_pMesh = pMesh;

	if (m_pMesh)
	{
		m_pMesh->AddRef();

		const std::vector<PMESHCONTAINER>* pvecMeshContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecMeshContainer->size();
		m_vecMaterial.resize(iSize);
		for (size_t i = 0; i < iSize; ++i)
		{
			size_t iSub = (*pvecMeshContainer)[i]->vecSubSet.size();

			m_vecMaterial[i] = std::vector<CMaterial*>(iSub);

			for (size_t j = 0; j < iSub; ++j)
			{
				m_vecMaterial[i][j] = (*pvecMeshContainer)[i]->vecSubSet[j]->pMaterial->Clone();
			}
		}
	}

	m_iBoneCount = iBoneCount;
	if (m_iBoneCount > 0)
	{
		CreateBoneBuffer(m_pInstBuffer->iCount);
	}

	SAFE_RELEASE(m_pShadowShader);
	m_pShadowShader = GET_SINGLE(CShaderManager)->FindShader("InstShadowShader");

	return true;
}

bool CRenderInstance::CreateInstBuffer(int iSize, int iCount)
{
	if (m_pInstBuffer)
	{
		SAFE_RELEASE(m_pInstBuffer->pBuffer);
		SAFE_DELETE_ARRAY(m_pInstBuffer->pData);
	}
	SAFE_DELETE(m_pInstBuffer);

	m_pInstBuffer = new INSTANCINGBUFFER;


	m_pInstBuffer->iSize = iSize;
	m_pInstBuffer->iCount = iCount;
	m_pInstBuffer->eUsg = D3D11_USAGE_DYNAMIC;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = m_pInstBuffer->iSize * m_pInstBuffer->iCount;
	tDesc.Usage = m_pInstBuffer->eUsg;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstBuffer->pBuffer)))
	{
		SAFE_DELETE(m_pInstBuffer);
		return false;
	}

	return true;
}

void CRenderInstance::Render(float fTime)
{
	if (m_iCount < 7)
		return;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pInstBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	size_t iTotal = static_cast<size_t>(m_pInstBuffer->iCount) * m_pInstBuffer->iSize;
	memcpy_s(tSub.pData, iTotal,
		m_pInstBuffer->pData, m_iCount * m_pInstBuffer->iSize);

	CONTEXT->Unmap(m_pInstBuffer->pBuffer, 0);

	m_pShader->SetShader();


	if (m_pMesh)
	{
		if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION)
		{
			ANIMATIONCBUFFER tCBuffer = {};
			tCBuffer.iBoneCount = m_iBoneCount;

			GET_SINGLE(CShaderManager)->UpdateCBuffer("Animation", &tCBuffer);

			CONTEXT->VSSetShaderResources(104, 1, &m_pSRV);
		}

		const std::vector<PMESHCONTAINER>* pvecContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecContainer->size();
		for (size_t i = 0; i < iSize; ++i)
		{
			size_t iSub = (*pvecContainer)[i]->vecSubSet.size();

			for (size_t j = 0; j < iSub; ++j)
			{
				(*pvecContainer)[i]->vecSubSet[j]->pMaterial->SetMaterial();

				m_pMesh->RenderInstancing(m_pInstBuffer->pBuffer, static_cast<int>(m_iCount),
					m_pInstBuffer->iSize, static_cast<int>(i), static_cast<int>(j));
			}
		}

		if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION)
		{
			ID3D11ShaderResourceView* pSRV = nullptr;
			CONTEXT->VSSetShaderResources(104, 1, &pSRV);
		}
	}


	Clear();
}

void CRenderInstance::RenderShadow(float fTime)
{
	if (m_iCount <= 0)
	{
		return;
	}

	m_pShadowShader->SetShader();

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pInstBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	size_t iTotal = static_cast<size_t>(m_pInstBuffer->iSize) * m_pInstBuffer->iCount;
	memcpy_s(tSub.pData, iTotal, m_pInstBuffer->pData, m_pInstBuffer->iSize * m_iCount);

	CONTEXT->Unmap(m_pInstBuffer->pBuffer, 0);

	if (m_pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION)
	{
		ANIMATIONCBUFFER tCBuffer = {};

		tCBuffer.iBoneCount = m_iBoneCount;

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Animation", &tCBuffer);

		ShaderCBuffer tMtrlCBuffer = {};

		tMtrlCBuffer.iAnimation = true;
		GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &tMtrlCBuffer);

		CONTEXT->VSSetShaderResources(104, 1, &m_pSRV);
	}

	if (m_pMesh)
	{
		const std::vector<PMESHCONTAINER>* pvecContainer = m_pMesh->GetMeshContainer();

		int iSize = static_cast<int>(pvecContainer->size());

		for (int i = 0; i < iSize; ++i)
		{
			int iSub = static_cast<int>((*pvecContainer)[i]->vecSubSet.size());
			for (int j = 0; j < iSub; ++j)
			{
				m_pMesh->RenderInstancing(m_pInstBuffer->pBuffer, 
					static_cast<int>(m_iCount), m_pInstBuffer->iSize, i, j);
			}
		}
	}

	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(104, 1, &pSRV);
}

void CRenderInstance::Clear()
{
	m_iCount = 0;
	m_ComponentList.clear();
}

bool CRenderInstance::CreateBoneBuffer(int iCount)
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);

	ID3D11Buffer* pBuffer = nullptr;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = iCount * m_iBoneCount * static_cast<UINT>(sizeof(Matrix));
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tDesc.StructureByteStride = static_cast<UINT>(sizeof(Matrix));
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer)))
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUavDesc = {};
	tUavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUavDesc.Buffer.FirstElement = 0;
	tUavDesc.Buffer.Flags = 0;
	tUavDesc.Buffer.NumElements = iCount * m_iBoneCount;
	tUavDesc.Format = DXGI_FORMAT_UNKNOWN;

	if (FAILED(DEVICE->CreateUnorderedAccessView(pBuffer, &tUavDesc, &m_pUAV)))
	{
		SAFE_RELEASE(pBuffer);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tSrvDesc = {};

	tSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrvDesc.BufferEx.FirstElement = 0;
	tSrvDesc.BufferEx.Flags = 0;
	tSrvDesc.BufferEx.NumElements = iCount * m_iBoneCount;
	tSrvDesc.Format = DXGI_FORMAT_UNKNOWN;

	if (FAILED(DEVICE->CreateShaderResourceView(pBuffer, &tSrvDesc, &m_pSRV)))
	{
		SAFE_RELEASE(pBuffer);
		return false;
	}

	SAFE_RELEASE(pBuffer);

	return true;
}