#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "../Device.h"
#include "../Resource/Shader.h"
#include "FBXLoader.h"
#include "Skeleton.h"
#include "AnimationSequence.h"
#include "../PathManager.h"

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

CMesh::CMesh()	:
	m_pInstancingBuffer(nullptr)
	, m_eType(MESH_TYPE::MT_2D)
{
	//memset(&m_tVB, 0, sizeof(VertexBuffer));
	//memset(&m_tIB, 0, sizeof(IndexBuffer));

	m_tMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_tMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
}

CMesh::~CMesh()
{
	size_t iSize = m_vecContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY(m_vecContainer[i]->tVB.pData);
		SAFE_RELEASE(m_vecContainer[i]->tVB.pBuffer);

		size_t iIBSize = m_vecContainer[i]->vecSubSet.size();

		for (size_t j = 0; j < iIBSize; j++)
		{
			SAFE_DELETE_ARRAY(m_vecContainer[i]->vecSubSet[j]->tIB.pData);
			SAFE_RELEASE(m_vecContainer[i]->vecSubSet[j]->tIB.pBuffer);
			SAFE_RELEASE(m_vecContainer[i]->vecSubSet[j]->pMaterial);
			SAFE_DELETE(m_vecContainer[i]->vecSubSet[j]);
		}

		SAFE_DELETE(m_vecContainer[i]);
	}

	m_vecContainer.clear();
}

const Vector3& CMesh::GetMax()	const
{
	return m_tMax;
}
const Vector3& CMesh::GetMin()	const
{
	return m_tMin;
}

const std::vector<PMESHCONTAINER>* CMesh::GetMeshContainer() const
{
	return &m_vecContainer;
}

MESH_TYPE CMesh::GetMeshType() const
{
	return m_eType;
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
	size_t iSize = m_vecContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY(m_vecContainer[i]->tVB.pData);
		SAFE_RELEASE(m_vecContainer[i]->tVB.pBuffer);

		size_t iIBSize = m_vecContainer[i]->vecSubSet.size();

		for (size_t j = 0; j < iIBSize; j++)
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

bool CMesh::CreateParticleMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, 
	int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, void* pIdxData, 
	int iIdxSz, int iIdxCnt, D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	size_t iSize = m_vecContainer.size();

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_DELETE_ARRAY(m_vecContainer[i]->tVB.pData);
		SAFE_RELEASE(m_vecContainer[i]->tVB.pBuffer);

		size_t iIBSize = m_vecContainer[i]->vecSubSet.size();

		for (size_t j = 0; j < iIBSize; j++)
		{
			SAFE_DELETE_ARRAY(m_vecContainer[i]->vecSubSet[j]->tIB.pData);
			SAFE_RELEASE(m_vecContainer[i]->vecSubSet[j]->tIB.pBuffer);
			SAFE_DELETE(m_vecContainer[i]->vecSubSet[j]);
		}

		SAFE_DELETE(m_vecContainer[i]);
	}

	m_vecContainer.clear();

	PMESHCONTAINER pContainer = new MESHCONTAINER;

	pContainer->tVB = VertexBuffer();
	pContainer->eTopo = eTop;

	pContainer->tVB.iSize = iVtxSz;
	pContainer->tVB.iCount = iVtxCnt;
	pContainer->tVB.eUsage = eVtxUsg;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = pContainer->tVB.iSize * pContainer->tVB.iCount;
	tDesc.Usage = eVtxUsg;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	tDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = pVtxData;
	pContainer->tVB.pData = new char[tDesc.ByteWidth];
	memcpy(pContainer->tVB.pData, pVtxData, tDesc.ByteWidth);

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pContainer->tVB.pBuffer)))
		return false;

	return true;
}

void CMesh::Render(float fTime, int iContainer, int iSubset )
{
	UINT iStride = static_cast<UINT>(m_vecContainer[iContainer]->tVB.iSize);
	UINT iOffset = 0;

	CONTEXT->IASetPrimitiveTopology(m_vecContainer[iContainer]->eTopo);
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecContainer[iContainer]->tVB.pBuffer, &iStride, &iOffset);

	if (m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.pBuffer)
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

void CMesh::RenderInstancing(void* pData, int iCount, int iSize, int iContainer, int iSubset)
{
	UINT pStrides[2] = {static_cast<UINT>(m_vecContainer[iContainer]->tVB.iSize),static_cast<UINT>(iSize)};
	UINT pOffsets[2] = { 0,0 };

	ID3D11Buffer* pBuffers[2] = {m_vecContainer[iContainer]->tVB.pBuffer, static_cast<ID3D11Buffer*>(pData)};	

	CONTEXT->IASetVertexBuffers(0, 2, pBuffers, pStrides, pOffsets);
	CONTEXT->IASetPrimitiveTopology(m_vecContainer[iContainer]->eTopo);

	if (m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.pBuffer)
	{
		CONTEXT->IASetIndexBuffer(m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.pBuffer, m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.eFmt, 0);
		CONTEXT->DrawIndexedInstanced(m_vecContainer[iContainer]->vecSubSet[iSubset]->tIB.iCount, iCount, 0, 0, 0);
	}
	else
	{
		CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CONTEXT->DrawInstanced(m_vecContainer[iContainer]->tVB.iCount, iCount, 0, 0);
	}
}

void CMesh::RenderParticle(int iCount, int iContainer, int iSubset)
{
}

bool CMesh::LoadMesh(const std::string& strName, const TCHAR* pFileName, const std::string& strPath)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPath);

	if (pPath)
		wcscpy_s(strFullPath, pPath);

	wcscat_s(strFullPath, pFileName);

	return LoadMeshFullPath(strName, strFullPath);
}

bool CMesh::LoadMeshFullPath(const std::string& strName, const TCHAR* pFullPath)
{
	SetName(strName);

	char strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath), NULL, NULL);
#else 
	strcpy_s(strFullPath, MAX_PATH, pFullPath, MAX_PATH);
#endif 
	char pExt[_MAX_EXT] = {};
	_splitpath_s(strFullPath, nullptr, 0, nullptr, 0, nullptr, 0, pExt, _MAX_EXT);

	_strupr_s(pExt);

	if (!strcmp(pExt, ".FBX"))
	{
		CFBXLoader loader;

		if (loader.LoadFbx(strFullPath, m_eType != MESH_TYPE::MT_ANIMATION))
		{
			return ConvertFBX(&loader, strFullPath);
		}
	}

	return LoadMesh(strFullPath);
}

bool CMesh::ConvertFBX(CFBXLoader* pLoader, const char* pFullPath)
{
	const std::vector<PFBXMESHCONTAINER>* pvecContainer = pLoader->GetMeshContainers();
	const std::vector<std::vector<PFBXMATERIAL>>* pvecMaterials = pLoader->GetMaterials();

	std::vector<PFBXMESHCONTAINER>::const_iterator	iter;
	std::vector<PFBXMESHCONTAINER>::const_iterator	iterEnd = pvecContainer->end();

	std::vector<std::vector<bool>>	vecEmptyIndex;

	bool	bBump = false;
	bool	bAnimation = false;
	int	iContainerNumber = 0;

	for (iter = pvecContainer->begin(); iter != iterEnd; ++iter, ++iContainerNumber)
	{
		std::vector<bool>	vecEmpty;
		vecEmptyIndex.push_back(vecEmpty);

		PMESHCONTAINER	pContainer = new MESHCONTAINER;

		m_vecContainer.push_back(pContainer);

		// 범프가 있을 경우
		if ((*iter)->bBump)
		{
			bBump = true;
		}

		if ((*iter)->bAnimation)
		{
			bAnimation = true;
		}

		std::vector<VERTEX3D>	vecVtx;

		for (size_t i = 0; i < (*iter)->vecPos.size(); ++i)
		{
			VERTEX3D	tVtx = {};

			tVtx.vPos = (*iter)->vecPos[i];
			tVtx.vNormal = (*iter)->vecNormal[i];
			tVtx.vUV = (*iter)->vecUV[i];

			if (bBump)
			{
				if (!(*iter)->vecTangent.empty())
					tVtx.vTangent = (*iter)->vecTangent[i];

				if (!(*iter)->vecBinormal.empty())
					tVtx.vBinormal = (*iter)->vecBinormal[i];
			}

			if (!(*iter)->vecBlendWeight.empty() && bAnimation)
			{
				tVtx.vBlendWeight = (*iter)->vecBlendWeight[i];
				tVtx.vBlendIndex = (*iter)->vecBlendIndex[i];
			}

			vecVtx.push_back(tVtx);
		}

		if (!AddVertexBuffer(&vecVtx[0], sizeof(VERTEX3D),
			(int)vecVtx.size(), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			pContainer))
			return false;

		// 인덱스버퍼 생성
		for (size_t i = 0; i < (*iter)->vecIndices.size(); ++i)
		{
			if ((*iter)->vecIndices[i].empty())
			{
				vecEmptyIndex[iContainerNumber].push_back(false);
				continue;
			}

			vecEmptyIndex[iContainerNumber].push_back(true);

			if (!AddIndexBuffer(&(*iter)->vecIndices[i][0],
				4, (int)(*iter)->vecIndices[i].size(),
				D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
				pContainer))
				return false;
		}
	}

	// 재질 정보를 읽어온다.
	const std::vector<std::vector<PFBXMATERIAL>>* pMaterials = pLoader->GetMaterials();

	std::vector<std::vector<PFBXMATERIAL>>::const_iterator	iterM;
	std::vector<std::vector<PFBXMATERIAL>>::const_iterator	iterMEnd = pMaterials->end();

	int	iContainer = 0;
	for (iterM = pMaterials->begin(); iterM != iterMEnd; ++iterM, ++iContainer)
	{
		for (size_t i = 0; i < (*iterM).size(); ++i)
		{
			// 인덱스 버퍼가 비어있을 경우에는 재질을 추가하지 않는다.
			if (!vecEmptyIndex[iContainer][i])
				continue;

			// 재질 정보를 얻어온다.
			PFBXMATERIAL	pMtrl = (*iterM)[i];

			CMaterial* pMaterial = new CMaterial;

			pMaterial->SetDiffuseColor(pMtrl->vDif);
			pMaterial->SetAmbientColor(pMtrl->vAmb);
			pMaterial->SetSpecularColor(pMtrl->vSpc);
			pMaterial->SetEmissiveColor(pMtrl->vEmv);
			pMaterial->SetSpecularPower(pMtrl->fShininess);

			// Shader는 일단 기본 3D Shader로 설정한다.
			m_vecContainer[iContainer]->vecSubSet[i]->strShader="Standard3DShader";
			
			m_vecContainer[iContainer]->vecSubSet[i]->pMaterial = pMaterial;

			// 이름을 불러온다.
			char	strName[MAX_PATH] = {};
			_splitpath_s(pMtrl->strDifTex.c_str(), NULL, 0, NULL, 0,
				strName, MAX_PATH, NULL, 0);

			TCHAR	strPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, pMtrl->strDifTex.c_str(),
				-1, strPath, (int)pMtrl->strDifTex.length());
#else
			strcpy_s(strPath, pMtrl->strDifTex.c_str());
#endif // UNICODE

			pMaterial->SetTextureFromFullPath(REGISTER_TYPE::RT_DIF, strName, strPath, 
				(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

			if (!pMtrl->strBumpTex.empty())
			{
				memset(strName, 0, MAX_PATH);
				_splitpath_s(pMtrl->strBumpTex.c_str(), NULL, 0, NULL, 0,
					strName, MAX_PATH, NULL, 0);

				memset(strPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, pMtrl->strBumpTex.c_str(),
					-1, strPath, (int)pMtrl->strBumpTex.length());
#else
				strcpy_s(strPath, pMtrl->strBumpTex.c_str());
#endif // UNICODE

				pMaterial->SetTextureFromFullPath(REGISTER_TYPE::RT_NRM, strName, strPath, 
					(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}

			if (!pMtrl->strSpcTex.empty())
			{
				memset(strName, 0, MAX_PATH);
				_splitpath_s(pMtrl->strSpcTex.c_str(), NULL, 0, NULL, 0,
					strName, MAX_PATH, NULL, 0);

				memset(strPath, 0, sizeof(TCHAR) * MAX_PATH);

#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0, pMtrl->strSpcTex.c_str(),
					-1, strPath, (int)pMtrl->strSpcTex.length());
#else
				strcpy_s(strPath, pMtrl->strSpcTex.c_str());
#endif // UNICODE

				pMaterial->SetTextureFromFullPath(REGISTER_TYPE::RT_SPE, strName, strPath, 
					(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
			}

			pMaterial->SetBump(bBump);

			pMaterial->SetAnimation(bAnimation);
		}
	}

	// 텍스쳐가 저장된 폴더명을 키로 변경한다.
	char	strFullName[MAX_PATH] = {};
	iterM = pMaterials->begin();
	strcpy_s(strFullName, (*iterM)[0]->strDifTex.c_str());

	int	iLength = static_cast<int>(strlen(strFullName));
	for (int i = iLength - 1; i >= 0; --i)
	{
		if (strFullName[i] == '\\' || strFullName[i] == '/')
		{
			memset(strFullName + (i + 1), 0, sizeof(char) * (iLength - (i + 1)));
			strFullName[i] = '\\';
			//strFullName[i] = 0;
			break;
		}
	}

	char	strChange[MAX_PATH] = {};
	strcpy_s(strChange, strFullName);
	iLength = static_cast<int>(strlen(strChange));
	for (int i = iLength - 2; i >= 0; --i)
	{
		if (strChange[i] == '\\' || strChange[i] == '/')
		{
			memset(strChange + (i + 1), 0, sizeof(char) * (iLength - (i + 1)));
			break;
		}
	}

	strcat_s(strChange, m_strName.c_str());
	strcat_s(strChange, "\\");

	MoveFileA(strFullName, strChange);

	// Mesh\\ 까지의 경로를 제거한다.
	iLength = static_cast<int>(strlen(strChange));
	for (int i = iLength - 2; i >= 0; --i)
	{
		char	cText[5] = {};
		memcpy(cText, &strChange[i - 4], 4);
		_strupr_s(cText);

		if (strcmp(cText, "MESH") == 0)
		{
			memset(strChange, 0, sizeof(char) * (i + 1));
			memcpy(strChange, &strChange[i + 1], sizeof(char) * (iLength - (i + 1)));
			memset(strChange + (i + 1), 0, sizeof(char) * (iLength - (i + 1)));
			break;
		}
	}

	for (size_t i = 0; i < m_vecContainer.size(); ++i)
	{
		PMESHCONTAINER	pContainer = m_vecContainer[i];

		for (size_t j = 0; j < pContainer->vecSubSet.size(); ++j)
		{
			pContainer->vecSubSet[j]->pMaterial->SetTexturePathKey(MESH_PATH);
			pContainer->vecSubSet[j]->pMaterial->ChangeTexturePath(strChange);
		}
	}

	/*m_vLength = m_vMax - m_vMin;

	m_vCenter = (m_vMax + m_vMin) / 2.f;
	m_fRadius = m_vLength.Length() / 2.f;*/

	char	strFullPath[MAX_PATH] = {};
	strcpy_s(strFullPath, pFullPath);
	int	iPathLength = (int)strlen(strFullPath);
	memcpy(&strFullPath[iPathLength - 3], "msh", 3);

	SaveMesh(strFullPath);

	// 애니메이션은 일단 주석.
	// 애니메이션 처리
	const std::vector<PFBXBONE>* pvecBone = pLoader->GetBones();

	if (pvecBone->empty())
		return true;

	std::string	strSkeletonName = m_strName + "_Skeleton";

	CSkeleton* pSkeleton = new CSkeleton;

	// 본 수만큼 반복한다.
	std::vector<PFBXBONE>::const_iterator	iterB;
	std::vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();
	std::vector<Matrix> vecOff;

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		PBONE	pBone = new BONE;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}
		vecOff.push_back(pBone->matOffset);

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
			}
		}

		pSkeleton->AddBone(pBone);
	}

	pSkeleton->CreateBoneTexture(&vecOff[0]);

	CResourceManager::GetInst()->SetSkeleton(strSkeletonName, pSkeleton);

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = pLoader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char	strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, strFullPath);

	iLength = (int)strlen(strAnimPath);
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		CResourceManager::GetInst()->LoadSequence((*iterC)->strName, false, *iterC);
		CAnimationSequence* pSequence = CResourceManager::GetInst()->FindSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, strAnimPath);
		strcat_s(strAnimFullPath, (*iterC)->strName.c_str());
		strcat_s(strAnimFullPath, ".sqc");

		pSequence->SaveFullPathMultiByte(strAnimFullPath);

		SAFE_RELEASE(pSequence);
	}

	if (pSkeleton)
	{
		memcpy(&strFullPath[iPathLength - 3], "bne", 3);
		
		pSkeleton->SaveFullPath(strFullPath);
	}

	SAFE_RELEASE(pSkeleton);

	return true;
}

bool CMesh::LoadMesh(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		fread(&m_tMax, sizeof(Vector3), 1, pFile);
		fread(&m_tMin, sizeof(Vector3), 1, pFile);

		bool bInst = false;

		fread(&bInst, 1, 1, pFile);

		if (bInst)
		{
			m_pInstancingBuffer = new INSTANCINGBUFFER;
			fread(&m_pInstancingBuffer->eUsg, 4, 1, pFile);
			fread(&m_pInstancingBuffer->iCount, 4, 1, pFile);
			fread(&m_pInstancingBuffer->iSize, 4, 1, pFile);
			fread(&m_pInstancingBuffer->pData, 1, m_pInstancingBuffer->iCount * m_pInstancingBuffer->iSize, pFile);
		}
		size_t iSize = 0;

		fread(&iSize, 4, 1, pFile);

		for (size_t i = 0; i < iSize; i++)
		{
			PMESHCONTAINER pContainer = new MESHCONTAINER;
			m_vecContainer.push_back(pContainer);
			fread(&pContainer->eTopo, 4, 1, pFile);
			fread(&pContainer->tVB.eUsage, 4, 1, pFile);
			fread(&pContainer->tVB.iCount, 4, 1, pFile);
			fread(&pContainer->tVB.iSize, 4, 1, pFile);
			//pContainer->tVB.pData = new char[pContainer->tVB.iCount * pContainer->tVB.iSize];
			std::vector<VERTEX3D> vecVtx(pContainer->tVB.iCount);
			fread(&vecVtx[0], 1, pContainer->tVB.iSize * pContainer->tVB.iCount, pFile);

			AddVertexBuffer(&vecVtx[0], pContainer->tVB.iSize, pContainer->tVB.iCount, pContainer->tVB.eUsage, pContainer->eTopo, pContainer);
			VERTEX3D vtx = *(--vecVtx.end());
			size_t iSubSize = 0;

			fread(&iSubSize, 4, 1, pFile);

			for (size_t j = 0; j < iSubSize; j++)
			{
				SUBSET tSub = {};
				fread(&tSub.tIB.eFmt, 4, 1, pFile);
				fread(&tSub.tIB.eUsage, 4, 1, pFile);
				fread(&tSub.tIB.iCount, 4, 1, pFile);
				fread(&tSub.tIB.iSize, 4, 1, pFile);
				//pSub->tIB.pData = new char[pSub->tIB.iCount * pSub->tIB.iSize];
				std::vector<UINT> vecIdx(tSub.tIB.iCount);
				fread(&vecIdx[0], 1, tSub.tIB.iSize * tSub.tIB.iCount, pFile);

				AddIndexBuffer(&vecIdx[0], tSub.tIB.iSize, tSub.tIB.iCount, tSub.tIB.eUsage, tSub.tIB.eFmt, pContainer);
				PSUBSET pSub = pContainer->vecSubSet.back();
				pSub->pMaterial = new CMaterial;
				pSub->pMaterial->Load(pFile);
				int iLength = 0;
				fread(&iLength, 4, 1, pFile);
				if (iLength > 0)
				{
					char strShader[MAX_PATH] = {};
					fread(strShader, 1, iLength, pFile);
					pSub->strShader = strShader;
				}
			}

		}

		fclose(pFile);

		return true;
	}

	return false;
}

bool CMesh::SaveMesh(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		fwrite(&m_tMax, sizeof(Vector3), 1, pFile);
		fwrite(&m_tMin, sizeof(Vector3), 1, pFile);

		bool bInst = false;

		if (m_pInstancingBuffer)
			bInst = true;

		fwrite(&bInst, 1, 1, pFile);

		if (m_pInstancingBuffer)
		{
			fwrite(&m_pInstancingBuffer->eUsg, 4, 1, pFile);
			fwrite(&m_pInstancingBuffer->iCount, 4, 1, pFile);
			fwrite(&m_pInstancingBuffer->iSize, 4, 1, pFile);
			fwrite(&m_pInstancingBuffer->pData, 1, m_pInstancingBuffer->iCount * m_pInstancingBuffer->iSize, pFile);
		}
		size_t iSize = m_vecContainer.size();

		fwrite(&iSize, 4, 1, pFile);

		for (size_t i = 0; i < iSize; i++)
		{
			fwrite(&m_vecContainer[i]->eTopo, 4, 1, pFile);
			fwrite(&m_vecContainer[i]->tVB.eUsage, 4, 1, pFile);
			fwrite(&m_vecContainer[i]->tVB.iCount, 4, 1, pFile);
			fwrite(&m_vecContainer[i]->tVB.iSize, 4, 1, pFile);
			fwrite(m_vecContainer[i]->tVB.pData, 1, m_vecContainer[i]->tVB.iSize * m_vecContainer[i]->tVB.iCount, pFile);

			size_t iSubSize = m_vecContainer[i]->vecSubSet.size();

			fwrite(&iSubSize, 4, 1, pFile);

			for (size_t j = 0; j < iSubSize; j++)
			{
				fwrite(&m_vecContainer[i]->vecSubSet[j]->tIB.eFmt, 4, 1, pFile);
				fwrite(&m_vecContainer[i]->vecSubSet[j]->tIB.eUsage, 4, 1, pFile);
				fwrite(&m_vecContainer[i]->vecSubSet[j]->tIB.iCount, 4, 1, pFile);
				fwrite(&m_vecContainer[i]->vecSubSet[j]->tIB.iSize, 4, 1, pFile);
				fwrite(m_vecContainer[i]->vecSubSet[j]->tIB.pData, 1, m_vecContainer[i]->vecSubSet[j]->tIB.iSize * m_vecContainer[i]->vecSubSet[j]->tIB.iCount, pFile);

				m_vecContainer[i]->vecSubSet[j]->pMaterial->Save(pFile);
				int iLength = static_cast<int>(m_vecContainer[i]->vecSubSet[j]->strShader.length());
				fwrite(&iLength, 4, 1, pFile);
				if (iLength > 0)
				{
					fwrite(m_vecContainer[i]->vecSubSet[j]->strShader.c_str(), 1, iLength, pFile);
				}
			}

		}

		fclose(pFile);
	}

	return true;
}

bool CMesh::AddVertexBuffer(const void* pVertices, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTop, PMESHCONTAINER pContainer)
{
	pContainer->eTopo = eTop;
	pContainer->tVB.iCount = iCount;
	pContainer->tVB.iSize = iSize;
	pContainer->tVB.eUsage = eUsg;
	size_t iTotal = static_cast<size_t>(iCount) * iSize;
	pContainer->tVB.pData = new char[iTotal];

	memcpy_s(pContainer->tVB.pData, iTotal, pVertices, iTotal);

	VERTEX3D* pVtx = static_cast<VERTEX3D*>(const_cast<void*>(pVertices));

	for (int i = 0; i < iCount; ++i)
	{
		m_tMax.x = m_tMax.x < pVtx[i].vPos.x ? pVtx[i].vPos.x : m_tMax.x;
		m_tMax.y = m_tMax.y < pVtx[i].vPos.y ? pVtx[i].vPos.y : m_tMax.y;
		m_tMax.z = m_tMax.z < pVtx[i].vPos.z ? pVtx[i].vPos.z : m_tMax.z;

		m_tMin.x = m_tMin.x > pVtx[i].vPos.x ? pVtx[i].vPos.x : m_tMin.x;
		m_tMin.y = m_tMin.y > pVtx[i].vPos.y ? pVtx[i].vPos.y : m_tMin.y;
		m_tMin.z = m_tMin.z > pVtx[i].vPos.z ? pVtx[i].vPos.z : m_tMin.z;
	}

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = iSize * iCount;
	tDesc.StructureByteStride = iSize;
	tDesc.Usage = eUsg;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = pContainer->tVB.pData;
	
	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSub, &pContainer->tVB.pBuffer)))
	{
		delete[] pContainer->tVB.pData;

		return false;
	}

	return true;
}

bool CMesh::AddIndexBuffer(const void* pIndices, int iSize, int iCount, D3D11_USAGE, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer)
{
	PSUBSET pSub = new SUBSET;

	pSub->tIB.eFmt = eFmt;
	pSub->tIB.eUsage = D3D11_USAGE_DEFAULT;
	pSub->tIB.iCount = iCount;
	pSub->tIB.iSize = iSize;
	pSub->tIB.pData = new char[iSize * iCount];

	memcpy_s(pSub->tIB.pData, iSize * iCount, pIndices, iSize * iCount);

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDesc.ByteWidth = iCount * iSize;
	tDesc.StructureByteStride = iSize;
	tDesc.Usage = pSub->tIB.eUsage;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = pSub->tIB.pData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSub, &pSub->tIB.pBuffer)))
	{
		delete[] pSub->tIB.pData;
		delete pSub;
		return false;
	}

	pContainer->vecSubSet.push_back(pSub);

	return true;
}

void CMesh::UpdateVertexBuffer(const void* pData, int iContainer)
{
	if (m_vecContainer.size() <= iContainer)
		return;

	size_t iSize = m_vecContainer[iContainer]->tVB.iSize * m_vecContainer[iContainer]->tVB.iCount;
	memcpy_s(m_vecContainer[iContainer]->tVB.pData, iSize, pData, iSize);

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_vecContainer[iContainer]->tVB.pBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy_s(tSub.pData, iSize, pData, iSize);

	CONTEXT->Unmap(m_vecContainer[iContainer]->tVB.pBuffer, 0);
}

void CMesh::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_tMax, sizeof(Vector3), 1, pFile);
	fwrite(&m_tMin, sizeof(Vector3), 1, pFile);
}

void CMesh::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_tMax, sizeof(Vector3), 1, pFile);
	fread(&m_tMin, sizeof(Vector3), 1, pFile);

}
