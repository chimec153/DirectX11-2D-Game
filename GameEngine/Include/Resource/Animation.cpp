#include "Animation.h"
#include "Skeleton.h"
#include "AnimationSequence.h"
#include "../Device.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "ComputeShader.h"
#include "BoneSocket.h"
#include "../Component/Transform.h"
#include "../Component/SceneComponent.h"
#include "../Render/RenderInstance.h"

CAnimation::CAnimation()	:
	m_pSkeleton(nullptr)
	, m_pScene(nullptr)
	, m_fTime(0.f)
	, m_pDefault(nullptr)
	, m_pCurrent(nullptr)
	, m_pChange(nullptr)
	, m_pShader(nullptr)
	, m_tCBuffer()
	, m_pOwner(nullptr)
	, m_fGlobalTime(0.f)
	, m_pBuffer(nullptr)
	, m_pBoneBuffer(nullptr)
	, m_pBoneUAV(nullptr)
	, m_pBoneSrcBuffer(nullptr)
#ifdef _DEBUG
	, m_pDebugBuffer(nullptr)
#endif
{
}

CAnimation::CAnimation(CAnimation& ani)	:
	CRef(ani)
	, m_pSkeleton(ani.m_pSkeleton)
	, m_pScene(nullptr)
	, m_fTime(ani.m_fTime)
	, m_pDefault(nullptr)
	, m_pCurrent(nullptr)
	, m_pChange(nullptr)
	, m_pShader(ani.m_pShader)
	, m_tCBuffer()
	, m_pOwner(ani.m_pOwner)
	, m_fGlobalTime(ani.m_fGlobalTime)
	, m_pBuffer(new STRUCTUREDBUFFER)
	, m_pBoneBuffer(nullptr)
	, m_pBoneUAV(nullptr)
	, m_pBoneSrcBuffer(nullptr)
#ifdef _DEBUG
	, m_pDebugBuffer(nullptr)
#endif
{
	*m_pBuffer = *ani.m_pBuffer;
	if (m_pSkeleton)
		m_pSkeleton = m_pSkeleton->Clone();

	std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapClip.begin();
	std::unordered_map<size_t, CAnimationSequence*>::iterator iterEnd = m_mapClip.end();

	for (;iter!=iterEnd;++iter)
	{
		CAnimationSequence* pSeq = new CAnimationSequence;
		pSeq = iter->second->Clone();

		if (ani.m_pCurrent == iter->second)
			m_pCurrent = pSeq;

		if (ani.m_pDefault == iter->second)
			m_pDefault = pSeq;

		m_mapClip.insert(std::make_pair(iter->first, pSeq));
	}

	if (m_pShader)
		m_pShader->AddRef();
}

CAnimation::~CAnimation()
{
	SAFE_RELEASE(m_pSkeleton);
	SAFE_DELETE(m_pBuffer);
	SAFE_RELEASE_MAP(m_mapClip);
	SAFE_RELEASE(m_pShader);
	SAFE_DELETE_VECLIST(m_vecNotify);
	SAFE_RELEASE_VECLIST(m_SocketList);
	SAFE_RELEASE(m_pBoneBuffer);
	SAFE_RELEASE(m_pBoneUAV);
	SAFE_RELEASE(m_pBoneSrcBuffer);
#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugBuffer);
#endif
}

void CAnimation::SetOwner(CSceneComponent* pCom)
{
	m_pOwner = pCom;
}

CSkeleton* CAnimation::GetSkeleton() const
{
	return m_pSkeleton;
}

void CAnimation::SetInstIndex(int iIndex)
{
	m_tCBuffer.iInstIndex = iIndex;
}

void CAnimation::SetFrame(int iFrame)
{
	m_tCBuffer.iFrame = iFrame;
}

void CAnimation::CreateNotify(const std::string& strKey, const std::string& strSeq, int iFrame)
{
	PNOTIFY pNot = FindNotify(strKey);

	if (pNot)
		return;

	CAnimationSequence* pSeq = FindSequence(strSeq);

	if (!pSeq)
		return;

	pNot = new NOTIFY;

	pNot->iFrame = iFrame;
	pNot->pSequence = pSeq;
	pNot->strKey = strKey;
	pNot->bCall = false;

	m_vecNotify.push_back(pNot);
}

void CAnimation::AddCallBack(const std::string& strKey, void(*pFunc)(float))
{
	PNOTIFY pNot = FindNotify(strKey);

	if (!pNot)
		return;

	pNot->vecFunc.push_back(std::bind(pFunc, std::placeholders::_1));
}

void CAnimation::AddSequence(const std::string& strKey, const std::string& strSeq)
{
	CAnimationSequence* pSeq = FindSequence(strKey);

	if (pSeq)
		return;

	pSeq = GET_SINGLE(CResourceManager)->FindSequence(strSeq);

	if (!pSeq)
		return;

	pSeq->m_pAni = this;
	pSeq->m_strKey = strKey;

	if (!m_pCurrent)
	{
		m_pCurrent = pSeq;
		m_tCBuffer.iFrameMax = m_pCurrent->m_iFrameLength;
	}

	if (!m_pDefault)
		m_pDefault = pSeq;

	std::hash<std::string> hs;
	size_t iKey = hs(strKey);
	m_mapClip.insert(std::make_pair(iKey, pSeq));

}

void CAnimation::ChangeSequence(const std::string& strKey)
{
	CAnimationSequence* pSeq = FindSequence(strKey);

	if (pSeq)
	{
		if (m_pCurrent == pSeq)
			return;

		m_pChange = pSeq;
		m_tCBuffer.iChange = true;
		m_tCBuffer.iChangeMax = m_pChange->m_iFrameLength;
	}
}

void CAnimation::SetSkeleton(CSkeleton* pSkel)
{
	SAFE_RELEASE(m_pSkeleton);

	m_pSkeleton = pSkel;
	if (m_pSkeleton)
		m_pSkeleton->AddRef();
}

void CAnimation::SetSkeleton(const std::string& strKey)
{
	SAFE_RELEASE(m_pSkeleton);

	m_pSkeleton = GET_SINGLE(CResourceManager)->FindSkeleton(strKey);
}

void CAnimation::LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey)
{
	GET_SINGLE(CResourceManager)->LoadSkeleton(strKey, pFilePath, strPathKey);
	CSkeleton* pSkel = GET_SINGLE(CResourceManager)->FindSkeleton(strKey);

	if(pSkel)
		m_pSkeleton = pSkel->Clone();

	SAFE_RELEASE(pSkel);
}

CAnimationSequence* CAnimation::FindSequence(const std::string& strKey) const
{
	std::hash<std::string> hs;
	size_t iKey = hs(strKey);

	std::unordered_map<size_t, CAnimationSequence*>::const_iterator iter = m_mapClip.find(iKey);

	if (iter == m_mapClip.end())
		return nullptr;

	return iter->second;
}

PNOTIFY CAnimation::FindNotify(const std::string& strKey)
{
	size_t iSize = m_vecNotify.size();

	for (size_t i = 0; i < iSize; i++)
	{
		if (m_vecNotify[i]->strKey == strKey)
			return m_vecNotify[i];
	}

	return nullptr;
}

void CAnimation::CreateSocket(const std::string& strKey, const std::string& strBone)
{
	CBoneSocket* pSocket = FindSocket(strKey);

	if (pSocket)
	{
		SAFE_RELEASE(pSocket);
		return;
	}

	pSocket = new CBoneSocket;

	pSocket->m_strName = strKey;
	pSocket->SetName(strKey);

	if (m_pSkeleton)
	{
		int iIndex = m_pSkeleton->GetBoneIndex(strBone);

		pSocket->m_iBoneIndex = iIndex;
	}

	m_SocketList.push_back(pSocket);
}

void CAnimation::SetSocketComponent(const std::string& strKey, CSceneComponent* pCom)
{
	CBoneSocket* pSocket = FindSocket(strKey);

	if (!pSocket)
	{
		return;
	}

	pSocket->SetInfo(strKey, pCom);

	m_SocketList.push_back(pSocket);
}

CBoneSocket* CAnimation::FindSocket(const std::string& strKey)
{
	std::list<CBoneSocket*>::iterator iter = m_SocketList.begin();
	std::list<CBoneSocket*>::iterator iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strKey)
		{
			(*iter)->AddRef();
			return (*iter);
		}
	}

	return nullptr;
}

void CAnimation::DeleteSocket(const std::string& strKey)
{
	std::list<CBoneSocket*>::iterator iter = m_SocketList.begin();
	std::list<CBoneSocket*>::iterator iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strKey)
		{
			SAFE_RELEASE((*iter));
			m_SocketList.erase(iter);
			return;
		}
	}
	return;
}

void CAnimation::SetSocketOffset(const std::string& strKey, const Vector3& vOffset)
{
	CBoneSocket* pBone = FindSocket(strKey);

	if (!pBone)
		return;

	pBone->SetOffset(vOffset);

	SAFE_RELEASE(pBone);
}

void CAnimation::SetSocketRotation(const std::string& strKey, const Vector4& vRot)
{
	CBoneSocket* pBone = FindSocket(strKey);

	if (!pBone)
		return;

	pBone->SetRotation(vRot);

	SAFE_RELEASE(pBone);
}

bool CAnimation::Init()
{
	m_pShader = static_cast<CComputeShader*>(GET_SINGLE(CShaderManager)->FindShader("AniUpdate"));

	return true;
}

void CAnimation::Start()
{
	if (!m_pBuffer)
	{
		m_pBuffer = new STRUCTUREDBUFFER;
		UINT iCount = static_cast<UINT>(m_pSkeleton->GetBoneCount());
		D3D11_BUFFER_DESC tDesc = {};
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		tDesc.ByteWidth = static_cast<UINT>(sizeof(Matrix) * iCount);
		tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		tDesc.StructureByteStride = sizeof(Matrix);
		tDesc.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pBuffer->pBuffer)))
			return;

		D3D11_SHADER_RESOURCE_VIEW_DESC tSrv = {};
		tSrv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		tSrv.BufferEx.FirstElement = 0;
		tSrv.BufferEx.Flags = 0;
		tSrv.BufferEx.NumElements = iCount;
		tSrv.Format = DXGI_FORMAT_UNKNOWN;

		if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer->pBuffer, &tSrv, &m_pBuffer->pSRV)))
			return;

		D3D11_UNORDERED_ACCESS_VIEW_DESC tUav = {};
		tUav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tUav.Buffer.FirstElement = 0;
		tUav.Buffer.Flags = 0;
		tUav.Buffer.NumElements = iCount;
		tUav.Format = DXGI_FORMAT_UNKNOWN;
		if (FAILED(DEVICE->CreateUnorderedAccessView(m_pBuffer->pBuffer, &tUav, &m_pBuffer->pUAV)))
			return;

#ifdef _DEBUG
		D3D11_BUFFER_DESC tDebug = {};
		tDebug.ByteWidth = iCount * static_cast<int>(sizeof(Matrix));
		tDebug.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tDebug.Usage = D3D11_USAGE_STAGING;
		if (FAILED(DEVICE->CreateBuffer(&tDebug, nullptr, &m_pDebugBuffer)))
			return;
#endif

		D3D11_BUFFER_DESC tBoneDesc = {};

		tBoneDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		tBoneDesc.ByteWidth = static_cast<UINT>(sizeof(Matrix) * m_pSkeleton->GetBoneCount());
		tBoneDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		tBoneDesc.StructureByteStride = static_cast<UINT>(sizeof(Matrix));
		tBoneDesc.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(DEVICE->CreateBuffer(&tBoneDesc, nullptr, &m_pBoneSrcBuffer)))
		{
			return;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC tBoneUAVDesc = {};

		tBoneUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tBoneUAVDesc.Buffer.FirstElement = 0;
		tBoneUAVDesc.Buffer.NumElements = static_cast<UINT>(m_pSkeleton->GetBoneCount());
		tBoneUAVDesc.Buffer.Flags = 0;
		tBoneUAVDesc.Format = DXGI_FORMAT_UNKNOWN;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_pBoneSrcBuffer, &tBoneUAVDesc, &m_pBoneUAV)))
		{
			return;
		}

		D3D11_BUFFER_DESC tCopyDesc = {};

		tCopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tCopyDesc.ByteWidth = static_cast<UINT>(sizeof(Matrix) * m_pSkeleton->GetBoneCount());
		tCopyDesc.Usage = D3D11_USAGE_STAGING;

		if (FAILED(DEVICE->CreateBuffer(&tCopyDesc, nullptr, &m_pBoneBuffer)))
		{
			return;
		}
		m_vecFrames.resize(m_pSkeleton->GetBoneCount());
	}

}

void CAnimation::Update(float fTime)
{
	m_fTime += fTime;
	if (m_pCurrent)
	{
		if (m_pChange)
		{
			m_tCBuffer.fChangeRatio += fTime;
			
			m_pChange->SetShader(15);

			if (m_tCBuffer.fChangeRatio >= 0.2f)
			{
				m_tCBuffer.iChange = false;
				m_tCBuffer.fChangeRatio = 0.f;

				m_pCurrent = m_pChange;
				m_pChange = nullptr;
				m_tCBuffer.iFrame = 0;

				m_tCBuffer.iFrameMax = m_pCurrent->m_iFrameLength;
			}
		}

		while (m_fTime >= m_pCurrent->m_fFrameTime)
		{
			m_fTime -= m_pCurrent->m_fFrameTime;
			m_tCBuffer.iFrame++;
			m_tCBuffer.fRatio = 0.f;

			if (m_tCBuffer.iFrame >= m_pCurrent->m_iFrameLength)
			{
				m_tCBuffer.iFrame = 0;

				if (!m_pCurrent->m_bLoop)
				{
					m_pChange = m_pDefault;
					m_tCBuffer.iChange = true;
					m_tCBuffer.iChangeMax = m_pChange->m_iFrameLength;
				}

				size_t iSize = m_vecNotify.size();

				for (size_t i = 0; i < iSize; i++)
				{
					m_vecNotify[i]->bCall = false;
				}
			}
		}

		m_tCBuffer.fRatio = m_fTime / m_pCurrent->m_fFrameTime;
		m_tCBuffer.iBoneCount = static_cast<int>(m_pSkeleton->GetBoneCount());

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Animation", &m_tCBuffer);

		m_pSkeleton->SetShader();
		m_pCurrent->SetShader(13);
		UINT iCount = -1;

		if (m_pOwner->IsInstanced())
		{
			CRenderInstance* pInst = m_pOwner->GetInstancing();

			ID3D11UnorderedAccessView* pUAV = pInst->GetBoneUav();

			CONTEXT->CSSetUnorderedAccessViews(0, 1, &pUAV, &iCount);
		}
		else
		{
			m_tCBuffer.iInstIndex = 0;
			CONTEXT->CSSetUnorderedAccessViews(0, 1, &m_pBuffer->pUAV, &iCount);
		}

		CONTEXT->CSSetUnorderedAccessViews(1, 1, &m_pBoneUAV, &iCount);

		int iX = static_cast<int>(m_pSkeleton->GetBoneCount()) / 256 + 1;
		m_pShader->SetShader();
		m_pShader->Dispatch(iX,1,1);
		m_pShader->Clear();

#ifdef _DEBUG
		CONTEXT->CopyResource(m_pBoneBuffer, m_pBoneSrcBuffer);

		D3D11_MAPPED_SUBRESOURCE tSub = {};
		
		CONTEXT->Map(m_pBoneBuffer, 0, D3D11_MAP_READ, 0, &tSub);

		memcpy_s(&m_vecFrames[0], sizeof(Matrix) * m_tCBuffer.iBoneCount,
			tSub.pData, sizeof(Matrix) * m_tCBuffer.iBoneCount);

		CONTEXT->Unmap(m_pBoneBuffer, 0);

#endif

		ID3D11UnorderedAccessView* pUav = nullptr;

		CONTEXT->CSSetUnorderedAccessViews(0, 1, &pUav, &iCount);
		CONTEXT->CSSetUnorderedAccessViews(1, 1, &pUav, &iCount);
		m_pCurrent->ResetShader(13);
		m_pSkeleton->ResetShader();
		if (m_pChange)
			m_pChange->ResetShader(15);

		size_t iNotSize = m_vecNotify.size();

		for (size_t i = 0; i < iNotSize; i++)
		{
			if (!m_vecNotify[i]->bCall)
			{
				if (m_vecNotify[i]->iFrame >= m_tCBuffer.iFrame)
				{
					size_t iFunc = m_vecNotify[i]->vecFunc.size();

					for (size_t j = 0; j < iFunc; j++)
					{
						m_vecNotify[i]->vecFunc[j](fTime);
					}

					m_vecNotify[i]->bCall = true;
				}
			}
		}

		m_pOwner->GetTransform()->SetWorld();

		std::list<CBoneSocket*>::iterator iter = m_SocketList.begin();
		std::list<CBoneSocket*>::iterator iterEnd = m_SocketList.end();

		for (; iter != iterEnd; ++iter)
		{
			Matrix mat = m_vecFrames[(*iter)->m_iBoneIndex];
			(*iter)->Update(fTime, mat * m_pOwner->GetMatWorld());
		}
	}

	
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::SetShader()
{
	CONTEXT->VSSetShaderResources(104, 1, &m_pBuffer->pSRV);
}

void CAnimation::ResetShader()
{
	ID3D11ShaderResourceView* pSrv = nullptr;

	CONTEXT->VSSetShaderResources(104, 1, &pSrv);
}

void CAnimation::SpawnWindow()
{
	if (ImGui::Begin("Animation"))
	{
		ImGui::SliderFloat("ChangeRatio", &m_tCBuffer.fChangeRatio, 0.f, 1.f);
		ImGui::SliderFloat("Ratio", &m_tCBuffer.fRatio, 0.f, 1.f);
		ImGui::Checkbox("Change", reinterpret_cast<bool*>(&m_tCBuffer.iChange));
		ImGui::SliderInt("Frame", &m_tCBuffer.iFrame, 0, m_tCBuffer.iFrameMax);
		ImGui::Text("MaxFrame: %d", m_tCBuffer.iFrameMax);
		ImGui::Text("Change Max: %d", m_tCBuffer.iChangeMax);

		if (m_pSkeleton)
		{
			m_pSkeleton->SpawnWindow();
		}

		int iSize = static_cast<int>(m_mapClip.size());
		const char** pText = new const char* [iSize];

		std::unordered_map<size_t, CAnimationSequence*>::iterator iter = m_mapClip.begin();
		std::unordered_map<size_t, CAnimationSequence*>::iterator iterEnd = m_mapClip.end();

		for (int i=0; iter != iterEnd; ++iter, ++i)
		{
			pText[i] = iter->second->m_strKey.c_str();
		}

		static int iSel = -1;

		ImGui::ListBox("Clips", &iSel, pText, iSize);

		if (iSel != -1 && iSel < iSize)
		{
			CAnimationSequence* pSeq = FindSequence(pText[iSel]);

			if (pSeq)
			{
				pSeq->SpawnWindow();
			}
		}

		delete[] pText;
	}
	ImGui::End();
}
