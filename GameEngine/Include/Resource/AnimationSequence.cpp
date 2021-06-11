#include "AnimationSequence.h"
#include "FBXLoader.h"
#include "../PathManager.h"
#include "../Device.h"
#include "Animation.h"

CAnimationSequence::CAnimationSequence()	:
	m_pAni(nullptr)
	, m_bLoop(false)
	, m_fPlayTime(1.f)
	, m_fPlayScale(1.f)
	, m_fFrameTime(0.f)
	, m_fStartTime(0.f)
	, m_fEndTime(0.f)
	, m_fTimeLength(0.f)
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_iFrameLength(0)
	, m_iFrameMode(0)
	, m_iChangeFrame(0)
	, m_bEnd(false)
	, m_pFullPath()
	, m_pBuffer(nullptr)
	, m_pSRV(nullptr)
{
}

CAnimationSequence::CAnimationSequence(const CAnimationSequence& seq)
{
	*this = seq;

	m_pAni = nullptr;
	m_pBuffer = nullptr;
	m_pSRV = nullptr;

	size_t iSize = seq.m_vecKeyFrame.size();

	std::vector<ANIFRAME> vecFrame(iSize * m_iFrameLength);
	for (size_t i = 0; i < iSize; i++)
	{
		m_vecKeyFrame[i]->iRef++;

		size_t iCount = m_vecKeyFrame[i]->vecKeyFrame.size();

		for (size_t j = 0; j < iCount; j++)
		{
			if (j >= m_iFrameLength)
				break;

			memcpy_s(&vecFrame[i * m_iFrameLength + j].vPos.x, 16, &m_vecKeyFrame[i]->vecKeyFrame[j]->vPos.x, 12);
			memcpy_s(&vecFrame[i * m_iFrameLength + j].vScale.x, 16, &m_vecKeyFrame[i]->vecKeyFrame[j]->vScale.x, 12);
			memcpy_s(&vecFrame[i * m_iFrameLength + j].vRot.x, 16, &m_vecKeyFrame[i]->vecKeyFrame[j]->vRot.x, 16);
		}
	}

	CreateBuffer(static_cast<int>(iSize) * m_iFrameLength, static_cast<int>(sizeof(ANIFRAME)), &vecFrame[0]);
}

CAnimationSequence::~CAnimationSequence()
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);

	size_t iSize = m_vecKeyFrame.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (--m_vecKeyFrame[i]->iRef == 0)
		{
			delete m_vecKeyFrame[i];
		}
	}

	m_vecKeyFrame.clear();
}

void CAnimationSequence::SetPlayScale(float fScale)
{
	m_fPlayScale = fScale;
}

void CAnimationSequence::SetPlayTime(float fTime)
{
	m_fPlayTime = fTime;
	m_fTimeLength = m_fPlayTime;
	m_fEndTime = m_fPlayTime;

	m_fFrameTime = m_fPlayTime / m_iFrameLength;

	size_t iSize = m_vecKeyFrame.size();
	for (size_t i = 0; i < iSize; i++)
	{
		size_t iFrame = m_vecKeyFrame[i]->vecKeyFrame.size();
		for (size_t j = 0; j < iFrame; j++)
		{
			m_vecKeyFrame[i]->vecKeyFrame[j]->dTime = j* m_fFrameTime;
		}
	}
}

bool CAnimationSequence::CreateSequence(_tagFbxAnimationClip* pClip, bool bLoop)
{
	m_bLoop = bLoop;
	SetName(pClip->strName);

	switch (pClip->eTimeMode)
	{
	case fbxsdk::FbxTime::EMode::eFrames24:
		m_iFrameMode = 24;
		break;
	case fbxsdk::FbxTime::EMode::eFrames30:
		m_iFrameMode = 30;
		break;
	case fbxsdk::FbxTime::EMode::eFrames60:
		m_iFrameMode = 60;
		break;
	}

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_iStartFrame = static_cast<int>(pClip->tStart.GetFrameCount(pClip->eTimeMode));
	m_iEndFrame = static_cast<int>(pClip->tEnd.GetFrameCount(pClip->eTimeMode));
	m_iFrameLength = m_iEndFrame - m_iStartFrame + 1;
	m_fPlayTime = static_cast<float>(pClip->tEnd.GetSecondDouble());

	// 시간 정보를 저장해준다.
	m_fStartTime = 0.f;
	m_fEndTime = m_fPlayTime;
	m_fTimeLength = m_fPlayTime;

	m_fFrameTime = m_fPlayTime / m_iFrameLength;

	std::vector<ANIFRAME>	vecFrameTrans;

	size_t iSize = pClip->vecBoneKeyFrame.size();
	vecFrameTrans.resize(iSize * m_iFrameLength);
	int iIdx = 0;
	for (size_t i = 0; i < iSize; i++)
	{
		PBONEKEYFRAME pBone = new BONEKEYFRAME;

		m_vecKeyFrame.push_back(pBone);

		size_t iFrame = pClip->vecBoneKeyFrame[i].vecKeyFrame.size();

		for (size_t j = 0; j < iFrame; j++)
		{
			PKEYFRAME pFrame = new KEYFRAME;

			fbxsdk::FbxAMatrix mat=  pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4 vT = mat.GetT();
			FbxVector4 vS = mat.GetS();
			FbxQuaternion q = mat.GetQ();

			pFrame->vPos = Vector3(static_cast<float>(vT.mData[0]), static_cast<float>(vT.mData[1]), static_cast<float>(vT.mData[2]));
			pFrame->vScale = Vector3(static_cast<float>(vS.mData[0]), static_cast<float>(vS.mData[1]), static_cast<float>(vS.mData[2]));
			pFrame->vRot = Vector4(static_cast<float>(q.mData[0]), static_cast<float>(q.mData[1]), static_cast<float>(q.mData[2]), static_cast<float>(q.mData[3]));

			pBone->vecKeyFrame.push_back(pFrame);

			if (j < m_iFrameLength)
			{
				memcpy_s(&vecFrameTrans[iIdx].vPos, 16, &pFrame->vPos, 12);
				memcpy_s(&vecFrameTrans[iIdx].vScale, 16, &pFrame->vScale, 12);
				memcpy_s(&vecFrameTrans[iIdx].vRot, 16, &pFrame->vRot, 16);
				++iIdx;
			}
			else
			{
				int i = 0;
			}
		}
	}

	CreateBuffer(static_cast<int>(iSize) * m_iFrameLength, 48, &vecFrameTrans[0]);

	int iLength = static_cast<int>(strlen(m_pFullPath));

	if (iLength)
	{
		char strPath[MAX_PATH] = {};
		char strName[_MAX_FNAME] = {};

		_splitpath_s(m_pFullPath, NULL, 0, NULL, 0, strName, _MAX_FNAME, NULL, 0);
		strcpy_s(strPath, m_pFullPath);

		for (int i = iLength - 1; i >= 0; i--)
		{
			// aa/bb.txt
			if (strPath[i] == '/' || strPath[i] == '\\')
			{
				memset(strPath + i + 1, 0, iLength - i - 1);
				break;
			}
		}

		strcat_s(strPath, strName);
		strcat_s(strPath, ".sqc");
		SaveFullPathMultiByte(strPath);
	}

	return true;
}

bool CAnimationSequence::CreateSequence(const std::string& strKey, bool bLoop, int iStartFrame, int iEndFrame, float fPlayTime, const std::vector<_tagBoneKeyFrame*>& vecFrame)
{
	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	m_bLoop = bLoop;
	SetName(strKey);

	m_iChangeFrame = 0;

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_iStartFrame = 0;
	m_iEndFrame = iEndFrame - iStartFrame;
	m_iFrameLength = m_iEndFrame - m_iStartFrame + 1;

	m_fPlayTime = fPlayTime;
	m_fFrameTime = fPlayTime / m_iFrameLength;

	// 시간 정보를 저장해준다.
	m_fStartTime = 0.f;
	m_fEndTime = fPlayTime;
	m_fTimeLength = fPlayTime;

	std::vector<ANIFRAME>	vecFrameTrans;
	size_t iSize = vecFrame.size();
	vecFrameTrans.resize( iSize* m_iFrameLength);
	int iIdx = 0;
	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < iSize; ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = vecFrame[i]->iBoneIndex;

		m_vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(m_iFrameLength);

		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = iStartFrame; j <= iEndFrame; ++j)
			{
				PKEYFRAME	pKeyFrame = new KEYFRAME;

				pKeyFrame->dTime = (j - iStartFrame) * m_fFrameTime;
				pKeyFrame->vScale = vecFrame[i]->vecKeyFrame[j]->vScale;
				pKeyFrame->vRot = vecFrame[i]->vecKeyFrame[j]->vRot;
				pKeyFrame->vPos = vecFrame[i]->vecKeyFrame[j]->vPos;

				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);


				if (j < m_iFrameLength)
				{
					memcpy_s(&vecFrameTrans[iIdx].vPos, 16, &pKeyFrame->vPos, 12);
					memcpy_s(&vecFrameTrans[iIdx].vScale, 16, &pKeyFrame->vScale, 12);
					memcpy_s(&vecFrameTrans[iIdx].vRot, 16, &pKeyFrame->vRot, 16);
					++iIdx;
				}
			}
		}
	}
	CreateBuffer(static_cast<int>(iSize) * m_iFrameLength, 48, &vecFrameTrans[0]);

	return true;
}

bool CAnimationSequence::CreateSequence(bool bLoop, const TCHAR* pFilePath, const std::string& strPathKey)
{
	m_bLoop = bLoop;

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		wcscpy_s(strFullPath, pPath);

	wcscat_s(strFullPath, pFilePath);	

	return CreateSequence(strFullPath);
}

bool CAnimationSequence::CreateSequence(const TCHAR* pFullPath)
{
	char strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath), NULL, NULL);
#else
	strcpy_s(strFullPath, pFullPath);
#endif

	return CreateSequenceMultiByte(strFullPath);
}

bool CAnimationSequence::CreateSequenceMultiByte(const char* pFullPath)
{
	char pExt[_MAX_EXT] = {};

	_splitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, pExt, _MAX_EXT);

	_strupr_s(pExt);

	if (!strcmp(pExt, ".FBX"))
	{
		return LoadFbxAnimation(pFullPath, m_bLoop);
	}

	return LoadFullPathMultiByte(pFullPath);
}

void CAnimationSequence::SetShader(int iRegister)
{
	CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
}

void CAnimationSequence::ResetShader(int iRegister)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
}

void CAnimationSequence::SpawnWindow()
{
	CRef::SpawnWindow();

	const char* pText = GetName().c_str();

	if (!strcmp(pText,""))
	{
		pText = "None";
	}

	if (ImGui::Begin(pText))
	{
		int iSize = static_cast<int>(m_vecKeyFrame.size());

		const char** pText = new const char* [iSize];

		for (int i = 0; i < iSize; i++)
		{
			if (m_pAni)
			{
				CSkeleton* pSkel = m_pAni->GetSkeleton();

				if (pSkel)
				{
					PBONE pBone = pSkel->GetBone(i);

					if (pBone)
					{
						pText[i] = pBone->strName.c_str();
					}
				}
			}
		}

		static int iSel = -1;

		ImGui::ListBox("Bone Key Frames", &iSel, pText, iSize);

		delete[] pText;

		if (iSel != -1 && iSel < iSize)
		{
			ImGui::Text("Bone Index: %d", m_vecKeyFrame[iSel]->iBoneIndex);
			int iFrame = static_cast<int>(m_vecKeyFrame[iSel]->vecKeyFrame.size());

			char** strText = new char* [iFrame];

			for (int i = 0; i < iFrame; i++)
			{
				char* _pText = new char[MAX_PATH];
				memset(_pText, 0, MAX_PATH);
				sprintf_s(_pText,MAX_PATH, "%d", i);
				strText[i] = _pText;
			}

			static int iItem = -1;

			ImGui::ListBox("Frames", &iItem, strText, iFrame);

			if (iItem != -1 && iItem < iFrame)
			{
				ImGui::Text("Time: %.2d", m_vecKeyFrame[iSel]->vecKeyFrame[iItem]->dTime);
				ImGui::InputFloat3("Position", &m_vecKeyFrame[iSel]->vecKeyFrame[iItem]->vPos.x);
				ImGui::InputFloat3("Scale", &m_vecKeyFrame[iSel]->vecKeyFrame[iItem]->vScale.x);
				ImGui::InputFloat4("Rotation", &m_vecKeyFrame[iSel]->vecKeyFrame[iItem]->vRot.x);
			}

			if (ImGui::Button("Update"))
			{
				std::vector<ANIFRAME> vecFrames(iSize * iFrame);

				for (int i = 0; i < iSize ; i++)
				{
					for (int j = 0; j < iFrame; j++)
					{
						if (j == m_iFrameLength)
							break;

						memcpy_s(&vecFrames[i * m_iFrameLength + j].vPos.x, 16, &m_vecKeyFrame[i]->vecKeyFrame[j]->vPos.x, 12);
						memcpy_s(&vecFrames[i * m_iFrameLength + j].vScale.x, 16, &m_vecKeyFrame[i]->vecKeyFrame[j]->vScale.x, 12);
						memcpy_s(&vecFrames[i * m_iFrameLength + j].vRot.x, 16, &m_vecKeyFrame[i]->vecKeyFrame[j]->vRot.x, 16);
					}
				}

				CreateBuffer(iSize * iFrame, static_cast<int>(sizeof(ANIFRAME)), &vecFrames[0]);
			}

			for (int i = 0; i < iFrame; i++)
			{
				delete[] strText[i];
			}

			delete[] strText;
		}
	}
	ImGui::End();
}

int CAnimationSequence::GetFrameCount() const
{
	return m_iFrameLength;
}

bool CAnimationSequence::IsEnd() const
{
	return m_bEnd;
}

bool CAnimationSequence::Init()
{
	return true;
}

CAnimationSequence* CAnimationSequence::Clone()
{
	return new CAnimationSequence(*this);
}

void CAnimationSequence::GetKeyFrame(std::vector<PBONEKEYFRAME>& vecFrame)
{
	vecFrame = m_vecKeyFrame;
}

bool CAnimationSequence::LoadFbxAnimation(const char* pFullPath, bool bLoop)
{
	CFBXLoader loader;

	loader.LoadFbx(pFullPath);

	const std::vector<PFBXANIMATIONCLIP>* pClips = loader.GetClips();

	memset(m_pFullPath, 0, MAX_PATH);
	memcpy_s(m_pFullPath, MAX_PATH, pFullPath, strlen(pFullPath));

	return CreateSequence(pClips->front(), bLoop);
}

bool CAnimationSequence::Load(const TCHAR* pFileName, const std::string& strPathKey)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		wcscpy_s(strFullPath, pPath);

	wcscat_s(strFullPath, pFileName);

	return LoadFullPath(strFullPath);
}

bool CAnimationSequence::LoadMultiByte(const char* pFileName, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	return LoadFullPathMultiByte(strFullPath);
}

bool CAnimationSequence::LoadFullPath(const TCHAR* pFullPath)
{
	char strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath), NULL, NULL);
#else
	strcpy_s(strFullPath, pFullPath);
#endif

	return LoadFullPathMultiByte(strFullPath);
}

bool CAnimationSequence::LoadFullPathMultiByte(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		int iLength = 0;

		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			char strName[MAX_PATH] = {};
			fread(strName, 1, iLength, pFile);
			m_strName = strName;
		}
		fread(&m_bLoop, 1, 1, pFile);
		fread(&m_fPlayTime, 4, 1, pFile);
		fread(&m_fPlayScale, 4, 1, pFile);
		fread(&m_fFrameTime, 4, 1, pFile);
		fread(&m_fStartTime, 4, 1, pFile);
		fread(&m_fEndTime, 4, 1, pFile);
		fread(&m_fTimeLength, 4, 1, pFile);
		fread(&m_iStartFrame, 4, 1, pFile);
		fread(&m_iEndFrame, 4, 1, pFile);
		fread(&m_iFrameLength, 4, 1, pFile);
		fread(&m_iFrameMode, 4, 1, pFile);
		fread(&m_iChangeFrame, 4, 1, pFile);
		fread(&m_iChangeFrame, 4, 1, pFile);
		fread(&m_bEnd, 1, 1, pFile);

		iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
			fread(m_pFullPath, 1, iLength, pFile);

		int iCount = 0;
		fread(&iCount, 4, 1, pFile);
		m_vecKeyFrame.resize(iCount);
		std::vector<ANIFRAME>	vecAniFrame(iCount * m_iFrameLength);
		for (int i = 0; i < iCount; i++)
		{
			PBONEKEYFRAME pBone = new BONEKEYFRAME;

			fread(&pBone->iBoneIndex, 4, 1, pFile);

			int iFrameCount = 0;
			fread(&iFrameCount, 4, 1, pFile);
			pBone->vecKeyFrame.resize(iFrameCount);
			for (int j = 0; j < iFrameCount; j++)
			{
				PKEYFRAME pFrame = new KEYFRAME;

				fread(&pFrame->dTime, 8, 1, pFile);
				fread(&pFrame->vPos, 12, 1, pFile);
				fread(&pFrame->vScale, 12, 1, pFile);
				fread(&pFrame->vRot, 16, 1, pFile);

				pBone->vecKeyFrame[j] = pFrame;

				if (j < m_iFrameLength)
				{
					memcpy_s(&vecAniFrame[i * m_iFrameLength + j].vPos, 16, &pFrame->vPos, 12);
					memcpy_s(&vecAniFrame[i * m_iFrameLength + j].vScale, 16, &pFrame->vScale, 12);
					memcpy_s(&vecAniFrame[i * m_iFrameLength + j].vRot, 16, &pFrame->vRot, 16);
				}
			}

			m_vecKeyFrame[i] = pBone;
		}

		fclose(pFile);

		return CreateBuffer(m_iFrameLength * iCount, 48, &vecAniFrame[0]);
	}

	return false;
}

bool CAnimationSequence::Save(const TCHAR* pFileName, const std::string& strPathKey)
{
	char strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName), NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif

	return SaveMultiByte(strFileName, strPathKey);
}

bool CAnimationSequence::SaveMultiByte(const char* pFileName, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	return SaveFullPathMultiByte(strFullPath);
}

bool CAnimationSequence::SaveFullPath(const TCHAR* pFullPath)
{
	char strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath), NULL, NULL);
#else
	strcpy_s(strFullPath, pFullPath);
#endif

	return SaveFullPathMultiByte(strFullPath);
}

bool CAnimationSequence::SaveFullPathMultiByte(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		int iLength = static_cast<int>(m_strName.length());
				
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite(m_strName.c_str(), 1, iLength, pFile);
		}
		fwrite(&m_bLoop, 1, 1, pFile);
		fwrite(&m_fPlayTime, 4, 1, pFile);
		fwrite(&m_fPlayScale, 4, 1, pFile);
		fwrite(&m_fFrameTime, 4, 1, pFile);
		fwrite(&m_fStartTime, 4, 1, pFile);
		fwrite(&m_fEndTime, 4, 1, pFile);
		fwrite(&m_fTimeLength, 4, 1, pFile);
		fwrite(&m_iStartFrame, 4, 1, pFile);
		fwrite(&m_iEndFrame, 4, 1, pFile);
		fwrite(&m_iFrameLength, 4, 1, pFile);
		fwrite(&m_iFrameMode, 4, 1, pFile);
		fwrite(&m_iChangeFrame, 4, 1, pFile);
		fwrite(&m_iChangeFrame, 4, 1, pFile);
		fwrite(&m_bEnd, 1, 1, pFile);

		iLength = static_cast<int>(strlen(m_pFullPath));
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
			fwrite(m_pFullPath, 1, iLength, pFile);

		int iCount = static_cast<int>(m_vecKeyFrame.size());
		fwrite(&iCount, 4, 1, pFile);
		for (int i = 0; i < iCount; i++)
		{
			fwrite(&m_vecKeyFrame[i]->iBoneIndex, 4, 1, pFile);

			int iFrameCount = static_cast<int>(m_vecKeyFrame[i]->vecKeyFrame.size());
			fwrite(&iFrameCount, 4, 1, pFile);
			for (int j = 0; j < iFrameCount; j++)
			{
				fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->dTime, 8, 1, pFile);
				fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vPos, 12, 1, pFile);
				fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vScale, 12, 1, pFile);
				fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vRot, 16, 1, pFile);
			}
		}

		fclose(pFile);

		return true;
	}

	return false;
}

bool CAnimationSequence::CreateBuffer(int iCount, int iSize, void* pData)
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.ByteWidth = iCount * iSize;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = iSize;
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = pData;	
	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_pBuffer)))
		return false;
	D3D11_SHADER_RESOURCE_VIEW_DESC tSrvDesc = {};
	tSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrvDesc.BufferEx.FirstElement = 0;
	tSrvDesc.BufferEx.Flags = 0;
	tSrvDesc.BufferEx.NumElements = iCount;
	tSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer, &tSrvDesc, &m_pSRV)))
		return false;

	return true;
}