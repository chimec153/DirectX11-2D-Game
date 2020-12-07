#include "Instancing.h"
#include "Resource/Material.h"
#include "Resource/Mesh.h"
#include "Device.h"
#include "UI/UIFont.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "RenderManager.h"
#include "Render/RenderState.h"

CInstancing::CInstancing()	:
	m_pBuffer(nullptr),
	m_iCount(0),
	m_bAnimation(false),
	m_pMesh(nullptr),
	m_pMaterial(nullptr),
	m_pLayer(nullptr),
	m_pFont(nullptr)
{
}

CInstancing::~CInstancing()
{
	if (m_pBuffer)
	{
		SAFE_RELEASE(m_pBuffer->pBuffer);
		SAFE_DELETE_ARRAY(m_pBuffer->pData);
		delete m_pBuffer;
	}

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pFont);
}

void CInstancing::SetLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;
}

int CInstancing::GetCount() const
{
	return m_iCount;
}

void CInstancing::AddCom(CSceneComponent* pCom)
{
	m_vecCom.push_back(pCom);
}

const std::vector<class CSceneComponent*>* CInstancing::GetComs() const
{
	return &m_vecCom;
}

void CInstancing::AddState(const std::string& strKey)
{
	CRenderState* pState = GET_SINGLE(CRenderManager)->FindState(strKey);

	if (!pState)
		return;

	m_vecState.push_back(pState);
}

bool CInstancing::Init(CMesh* pMesh, CMaterial* pMtrl, int iCount , int iSize)
{
	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (pMtrl)
		m_pMaterial = pMtrl->Clone();

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CLayer* pUI = pScene->FindLayer("UI");

	CObj* pObj = pScene->CreateObject<CObj>("InstCount", pUI);

	m_pFont = pObj->CreateComponent<CUIFont>("InstCount");

	pObj->SetRootComponent(m_pFont);

	SAFE_RELEASE(pObj);

	m_pFont->SetRect(0.f, 600.f, 300.f, 500.f);

	m_pFont->SetWorldPos(0.f, 100.f, 0.f);

	return CreateInstancingBuffer(iCount, iSize);
}

void CInstancing::SetFontRect(float l, float t, float r, float b)
{
	m_pFont->SetRect(l, t, r, b);
}

void CInstancing::SetFontPos(const Vector3& vPos)
{
	if(m_pFont)
		m_pFont->SetWorldPos(vPos);
}

bool CInstancing::CreateInstancingBuffer(int iCount, int iSize)
{
	m_pBuffer = new INSTANCINGBUFFER;

	m_pBuffer->eUsg = D3D11_USAGE_DYNAMIC;
	m_pBuffer->iCount = iCount;
	m_pBuffer->iSize = iSize;
	m_pBuffer->pData = new char[iCount * iSize];
	
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = iCount * iSize;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &m_pBuffer->pBuffer)))
	{
		delete m_pBuffer;
		return false;
	}

	return true;
}

void CInstancing::AddInstanceData(const void* pData)
{
	if (m_iCount == m_pBuffer->iCount)
	{
		ResizeBuffer(m_iCount * 2, m_pBuffer->iSize);
	}

	memcpy(((char*)m_pBuffer->pData + (m_iCount++) * m_pBuffer->iSize), pData, m_pBuffer->iSize);
}

void CInstancing::ResizeBuffer(int iCount, int iSize)
{
	if (m_pBuffer)
	{
		if (m_pBuffer->pData)
		{
			char* pData = new char[iCount * iSize];

			memset(pData, 0, iCount * iSize);

			memcpy(pData, m_pBuffer->pData, m_iCount * m_pBuffer->iSize);

			delete[] m_pBuffer->pData;

			m_pBuffer->pData = pData;
		}

		else
			m_pBuffer->pData = new char[iCount * iSize];

		if (m_pBuffer->pBuffer)
			m_pBuffer->pBuffer->Release();

		m_pBuffer->iSize = iSize;
		m_pBuffer->iCount = iCount;
		
		D3D11_BUFFER_DESC tDesc = {};

		tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tDesc.ByteWidth = iCount * iSize;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = m_pBuffer->eUsg;

		DEVICE->CreateBuffer(&tDesc, nullptr, &m_pBuffer->pBuffer);
	}

	else
		CreateInstancingBuffer(iCount, iSize);
}

void CInstancing::Update(float fTime)
{
	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(m_pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, m_pBuffer->pData, m_iCount * m_pBuffer->iSize);

	CONTEXT->Unmap(m_pBuffer->pBuffer, 0);

	TCHAR strCount[32] = {};

	swprintf_s(strCount, TEXT(" Inst Count: %d"), m_iCount);

	std::string strKey = m_pLayer->GetName();

	TCHAR strName[64] = {};

#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP, 0, strKey.c_str(), -1, strName, (int)strKey.length());
#else
	strcpy_s(strName, strKey.c_str());
#endif

	wcscat_s(strName, strCount);

	m_pFont->SetText(strName);
}

void CInstancing::Render(float fTime)
{
	size_t iSz = m_vecState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecState[i]->SetState();
	}

	m_pMaterial->SetMaterial();

	m_pMesh->RenderInstancing(m_pBuffer->pBuffer, m_iCount, m_pBuffer->iSize);

	iSz = m_vecState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecState[i]->ResetState();
	}
}

void CInstancing::Clear()
{
	m_iCount = 0;
	m_vecCom.clear();
}
