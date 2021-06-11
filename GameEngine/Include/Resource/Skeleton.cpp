#include "Skeleton.h"
#include "../Device.h"
#include "../PathManager.h"

CSkeleton::CSkeleton()	:
	m_pBoneTex(nullptr)
	, m_pSRV(nullptr)
	, m_pBuffer(nullptr)
#ifdef _DEBUG
	, m_pDebugBuffer(nullptr)
#endif
{
}

CSkeleton::CSkeleton(const CSkeleton& skel)	:
	CRef(skel)
	, m_pBoneTex(nullptr)
	, m_pSRV(nullptr)
	, m_pBuffer(nullptr)
#ifdef _DEBUG
	, m_pDebugBuffer(nullptr)
#endif
{	
	size_t iSize = skel.m_vecBone.size();
	std::vector<Matrix> vecOff(iSize);	

	for (size_t i = 0; i < iSize; i++)
	{
		PBONE p = new BONE;

		*p = *skel.m_vecBone[i];

		memcpy_s(&vecOff[i], 64, &p->matOffset, 64);

		m_vecBone.push_back(p);
	}

	CreateBoneTexture(&vecOff[0]);
}

CSkeleton::~CSkeleton()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pBoneTex);
	SAFE_RELEASE(m_pBuffer);

	size_t iSize = m_vecBone.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (--m_vecBone[i]->iRef == 0)
			delete m_vecBone[i];
	}

	m_vecBone.clear();
#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugBuffer);
#endif
}

size_t CSkeleton::GetBoneCount() const
{
	return m_vecBone.size();
}

int CSkeleton::GetBoneIndex(const std::string& strKey)	const
{
	size_t iSize = m_vecBone.size();

	for (size_t i = 0; i < iSize; i++)
	{
		if (m_vecBone[i]->strName == strKey)
		{
			return static_cast<int>(i);
		}
	}

	return -1;
}

PBONE CSkeleton::GetBone(int iIdx) const
{
	return m_vecBone[iIdx];
}

PBONE CSkeleton::GetBone(const std::string& strKey)	const
{
	int iIdx = GetBoneIndex(strKey);

	if (iIdx < 0)
		return nullptr;

	return m_vecBone[iIdx];
}

const Matrix& CSkeleton::GetBoneMatrix(int iIdx) const
{
	return m_vecBone[iIdx]->matBone;
}

const Matrix CSkeleton::GetBoneMatrix(const std::string& strKey) const
{
	PBONE pBone = GetBone(strKey);

	if (pBone)
		return pBone->matBone;

	return Matrix();
}

void CSkeleton::AddBone(const PBONE bone)
{
	m_vecBone.push_back(bone);
}

bool CSkeleton::CreateBoneTexture(void* pData)
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pBuffer);
	UINT iSize = static_cast<UINT>(m_vecBone.size());
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.ByteWidth = 64 * iSize;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = 64;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA tData = {};
	tData.pSysMem = pData;
	
	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_pBuffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSrvDesc = {};

	tSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSrvDesc.BufferEx.FirstElement = 0;
	tSrvDesc.BufferEx.Flags = 0;
	tSrvDesc.BufferEx.NumElements = iSize;
	tSrvDesc.Format = DXGI_FORMAT_UNKNOWN;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer, &tSrvDesc, &m_pSRV)))
		return false;

#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugBuffer);
	D3D11_BUFFER_DESC tDebug = {};

	tDebug.ByteWidth = sizeof(Matrix) * iSize;
	tDebug.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tDebug.Usage = D3D11_USAGE_STAGING;

	DEVICE->CreateBuffer(&tDebug, NULL, &m_pDebugBuffer);
#endif

	return true;
}

bool CSkeleton::UpdateBoneTexture(const std::vector<Matrix>& vecMat)
{
	return true;
}

bool CSkeleton::Save(const char* pFilePath, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};
	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	return SaveFullPath(strFullPath);
}

bool CSkeleton::SaveFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		int iCount = static_cast<int>(m_vecBone.size());
		fwrite(&iCount, 4, 1, pFile);
		for (int i = 0; i < iCount; i++)
		{
			int iLength = static_cast<int>(m_vecBone[i]->strName.length());
			fwrite(&iLength, 4, 1, pFile);
			if (iLength > 0)
				fwrite(m_vecBone[i]->strName.c_str(), 1, iLength, pFile);

			fwrite(&m_vecBone[i]->iDepth, 4, 1, pFile);
			fwrite(&m_vecBone[i]->iParentIndex, 4, 1, pFile);
			fwrite(&m_vecBone[i]->matOffset, 64, 1, pFile);
			fwrite(&m_vecBone[i]->matBone, 64, 1, pFile);
		}
		fclose(pFile);
		return true;
	}

	return false;
}

bool CSkeleton::Load(const std::string& strKey, const char* pFilePath, const std::string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFilePath);

	return LoadFullPath(strKey, strFullPath);
}

bool CSkeleton::LoadFullPath(const std::string& strKey, const char* pFullPath)
{
	size_t iSize = m_vecBone.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (--m_vecBone[i]->iRef == 0)
			delete m_vecBone[i];
	}

	m_vecBone.clear();

	SetName(strKey);
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFullPath, "rb");
	if (pFile)
	{
		int iCount = 0;
		fread(&iCount, 4, 1, pFile);
		m_vecBone.resize(iCount);

		std::vector<Matrix> vecOffset(iCount);
		for (int i = 0; i < iCount; i++)
		{
			PBONE p = new BONE;
			int iLength = 0;
			fread(&iLength, 4, 1, pFile);
			if (iLength > 0)
			{
				char strKey[MAX_PATH] = {};
				fread(strKey, 1, iLength, pFile);
				p->strName = strKey;
			}
			fread(&p->iDepth, 4, 1, pFile);
			fread(&p->iParentIndex, 4, 1, pFile);
			fread(&p->matOffset, 64, 1, pFile);
			fread(&p->matBone, 64, 1, pFile);
			memcpy_s(&vecOffset[i], 64, &p->matOffset, 64);
			m_vecBone[i] = p;
		}

		CreateBoneTexture(&vecOffset[0]);
		fclose(pFile);
		return true;
	}
	return false;
}

void CSkeleton::SetShader()
{
	CONTEXT->CSSetShaderResources(14, 1, &m_pSRV);
}

void CSkeleton::ResetShader()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(14, 1, &pSRV);
}

void CSkeleton::Update(float fTime, const Matrix& matWorld)
{

}

CSkeleton* CSkeleton::Clone()
{
	return new CSkeleton(*this);
}

void CSkeleton::SpawnWindow()
{
	CRef::SpawnWindow();

	if (ImGui::Begin("Skeleton"))
	{
		int iSize = static_cast<int>(m_vecBone.size());

		const char** pText = new const char* [iSize];

		for (int i = 0; i < iSize; i++)
		{
			pText[i] = m_vecBone[i]->strName.c_str();
		}

		static int iItem = -1;

		ImGui::ListBox("Bones", &iItem, pText, iSize);

		delete[] pText;

		if (iItem != -1 && iItem < iSize)
		{
			ImGui::Text("Name: %d", m_vecBone[iItem]->strName);
			ImGui::Text("Depth: %d", m_vecBone[iItem]->iDepth);
			ImGui::Text("Parent: %d", m_vecBone[iItem]->iParentIndex);
			ImGui::Text("Reference Count: %d", m_vecBone[iItem]->iRef);
			ImGui::InputFloat4("Bone Matrix 0", &m_vecBone[iItem]->matBone[0].x);
			ImGui::InputFloat4("Bone Matrix 1", &m_vecBone[iItem]->matBone[1].x);
			ImGui::InputFloat4("Bone Matrix 2", &m_vecBone[iItem]->matBone[2].x);
			ImGui::InputFloat4("Bone Matrix 3", &m_vecBone[iItem]->matBone[3].x);
			ImGui::InputFloat4("Offset Matrix 0", &m_vecBone[iItem]->matOffset[0].x);
			ImGui::InputFloat4("Offset Matrix 1", &m_vecBone[iItem]->matOffset[1].x);
			ImGui::InputFloat4("Offset Matrix 2", &m_vecBone[iItem]->matOffset[2].x);
			ImGui::InputFloat4("Offset Matrix 3", &m_vecBone[iItem]->matOffset[3].x);
						
			if (ImGui::Button("Update Bone"))
			{
				std::vector<Matrix> vecOffset(iSize);

				for (int i = 0; i < iSize; i++)
				{
					vecOffset[i] = m_vecBone[i]->matOffset;
				}

				CreateBoneTexture(&vecOffset[0]);
			}
		}

	}
	ImGui::End();
}
