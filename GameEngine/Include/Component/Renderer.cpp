#include "Renderer.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader.h"
#include "../Resource/ShaderManager.h"
#include "PaperBurn.h"
#include "../Resource/Animation.h"
#include "SceneComponent.h"

CRenderer::CRenderer()	:
	m_pMesh(nullptr)
	, m_pPaper(nullptr)
	, m_pAni(nullptr)
	, m_pOwner(nullptr)
{
}

CRenderer::CRenderer(const CRenderer& com)	:
	CComponent(com)
	, m_pPaper(com.m_pPaper)
	, m_pAni(com.m_pAni)
	, m_pOwner(com.m_pOwner)
{
	m_pMesh = com.m_pMesh;

	if (m_pMesh)
	{
		m_pMesh->AddRef();
	}

	m_vecMaterial.clear();
	m_vecShader.clear();

	size_t iMtrlSz = com.m_vecMaterial.size();

	for (size_t i = 0; i < iMtrlSz; i++)
	{
		std::vector<CMaterial*> vecMtrl;

		m_vecMaterial.push_back(vecMtrl);

		size_t iSub = com.m_vecMaterial[i].size();

		m_vecMaterial[i].resize(iSub);

		for (size_t j = 0; j < iSub; ++j)
		{
			m_vecMaterial[i][j] = com.m_vecMaterial[i][j];

			if (m_vecMaterial[i][j])
			{
				m_vecMaterial[i][j]->AddRef();
			}
		}
	}

	size_t iShaderSz = com.m_vecShader.size();

	for (size_t i = 0; i < iShaderSz; i++)
	{
		std::vector<CShader*> vecShader;

		m_vecShader.push_back(vecShader);

		size_t iSub = com.m_vecShader[i].size();

		m_vecShader[i].resize(iSub);

		for (size_t j = 0; j < iSub; ++j)
		{
			m_vecShader[i][j] = com.m_vecShader[i][j];

			if (m_vecShader[i][j])
			{
				m_vecShader[i][j]->AddRef();
			}
		}
	}

	m_vecRenderState.clear();

	size_t iSz = com.m_vecRenderState.size();

	for (int i = 0; i < iSz; ++i)
	{
		CRenderState* pState = GET_SINGLE(CRenderManager)->FindState(com.m_vecRenderState[i]->GetName());

		if (pState)
		{
			m_vecRenderState.push_back(pState);
		}
	}

	if (m_pPaper)
	{
		m_pPaper = m_pPaper->Clone();
	}

	if (m_pAni)
	{
		m_pAni = m_pAni->Clone();
	}
}

CRenderer::~CRenderer()
{
	SAFE_RELEASE_VECLIST(m_vecRenderState);
	for (int i = 0; i < m_vecMaterial.size(); ++i)
	{
		SAFE_RELEASE_VECLIST(m_vecMaterial[i]);
	}
	SAFE_RELEASE(m_pMesh);

	for (int i = 0; i < m_vecShader.size(); ++i)
	{
		SAFE_RELEASE_VECLIST(m_vecShader[i]);
	}
	SAFE_RELEASE(m_pPaper);
	SAFE_RELEASE(m_pAni);
}

void CRenderer::AddRenderState(const std::string& strKey)
{
	CRenderState* pState = FindState(strKey);

	if (pState)
	{
		SAFE_RELEASE(pState);
		return;
	}

	pState = GET_SINGLE(CRenderManager)->FindState(strKey);

	if (!pState)
		return;

	m_vecRenderState.push_back(pState);
}

CMesh* CRenderer::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CRenderer::SetMesh(const std::string& strName)
{
	CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

	SetMesh(pMesh);

	SAFE_RELEASE(pMesh);
}

void CRenderer::SetMesh(CMesh* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
	{
		m_pMesh->AddRef();

		const std::vector<PMESHCONTAINER>* pvecContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecContainer->size();

		for (size_t i = 0; i < iSize; i++)
		{
			std::vector<CShader*> vecShader;

			m_vecShader.push_back(vecShader);

			size_t iSubset = (*pvecContainer)[i]->vecSubSet.size();

			m_vecShader[i].resize(iSubset);

			std::vector<CMaterial*> vecMtrl;

			m_vecMaterial.push_back(vecMtrl);

			m_vecMaterial[i].resize(iSubset);//마침표, 머리이름, 경계선, 한번만 나오게 커지게 랜덤으로, 이번주 까지 마무리, 조명, 잡몹, 퀘스트 완료, fullHD 전체화면모드에서 촬영

			for (size_t j = 0; j < iSubset; j++)
			{
				SetMaterial((*pvecContainer)[i]->vecSubSet[j]->pMaterial, i,j);
				SetShader((*pvecContainer)[i]->vecSubSet[j]->strShader, i,j);
			}
		}

		m_pOwner->SetMeshSize(m_pMesh->GetMax() - m_pMesh->GetMin());
	}
}

CMaterial* CRenderer::GetMaterial(size_t iContainer, size_t iSubset) const
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return nullptr;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return nullptr;
	}

	if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset]->AddRef();
	}

	return m_vecMaterial[iContainer][iSubset];
}

void CRenderer::SetMaterial(CMaterial* pMaterial, size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return ;
	}

	SAFE_RELEASE(m_vecMaterial[iContainer][iSubset]);

	CMaterial* pMtrl = pMaterial;

	m_vecMaterial[iContainer][iSubset] = pMtrl;

	if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset] = m_vecMaterial[iContainer][iSubset]->Clone();
	}
}

void CRenderer::SetMaterial(const std::string& strTag, size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return;
	}

	SAFE_RELEASE(m_vecMaterial[iContainer][iSubset]);

	CMaterial* pMtrl = GET_SINGLE(CResourceManager)->FindMaterial(strTag);

	m_vecMaterial[iContainer][iSubset] = pMtrl;

	SAFE_RELEASE(pMtrl);

	if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset] = m_vecMaterial[iContainer][iSubset]->Clone();
	}

}

void CRenderer::SetShader(const std::string& strTag, size_t iContainer, size_t iSubset)
{
	if (m_vecShader.size() <= iContainer)
	{
		return;
	}
	else if (m_vecShader[iContainer].size() <= iSubset)
	{
		return;
	}

	SAFE_RELEASE(m_vecShader[iContainer][iSubset]);

	m_vecShader[iContainer][iSubset] = GET_SINGLE(CShaderManager)->FindShader(strTag);
}

void CRenderer::SetShader(CShader* pShader, size_t iContainer, size_t iSubset)
{
	if (m_vecShader.size() <= iContainer)
	{
		return;
	}
	else if (m_vecShader[iContainer].size() <= iSubset)
	{
		return;
	}

	SAFE_RELEASE(m_vecShader[iContainer][iSubset]);

	m_vecShader[iContainer][iSubset] = pShader;

	if (m_vecShader[iContainer][iSubset])
	{
		m_vecShader[iContainer][iSubset]->AddRef();
	}
}

CShader* CRenderer::GetShader(size_t iContainer, size_t iSubset) const
{
	if (m_vecShader.size() <= iContainer)
	{
		return nullptr;
	}
	else if (m_vecShader[iContainer].size() <= iSubset)
	{
		return nullptr;
	}

	if (m_vecShader[iContainer][iSubset])
	{
		m_vecShader[iContainer][iSubset]->AddRef();
	}

	return m_vecShader[iContainer][iSubset];
}

void CRenderer::SetState()
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRenderState[i]->SetState();
	}
}

void CRenderer::ResetState()
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRenderState[i]->ResetState();
	}
}

const std::vector<class CRenderState*>& CRenderer::GetVecRenderState() const
{
	return m_vecRenderState;
}

void CRenderer::DeleteRenderState(const std::string& strKey)
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_vecRenderState[i]->GetName() == strKey)
		{
			m_vecRenderState[i]->ResetState();

			SAFE_RELEASE(m_vecRenderState[i]);

			std::vector<CRenderState*>::iterator iter = m_vecRenderState.begin() + i;

			m_vecRenderState.erase(iter);

			return;
		}
	}
}

void CRenderer::SetPaper(CPaperBurn* pBurn)
{
	m_pPaper = pBurn;

	if (m_pPaper)
	{
		m_pPaper->AddRef();
	}
}

void CRenderer::SetAnimation(CAnimation* pAni)
{
	SAFE_RELEASE(m_pAni);

	m_pAni = pAni;

	if (m_pAni)
	{
		m_pAni->SetOwner(m_pOwner);
		m_pAni->AddRef();
	}
}

void CRenderer::SetMaterial(size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return;
	}
	if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset]->SetMaterial();
	}
}

void CRenderer::SetShader(size_t iContainer, size_t iSubset)
{
	if (m_vecShader.size() <= iContainer)
	{
		return;
	}
	if (m_vecShader[iContainer].size() <= iSubset)
	{
		return;
	}
	if (m_vecShader[iContainer][iSubset])
	{
		m_vecShader[iContainer][iSubset]->SetShader();
	}
}

void CRenderer::ResizeMaterial(int iSize, int iContainer)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	m_vecMaterial[iContainer].resize(iSize);
}

void CRenderer::ResizeShader(int iSize, int iContainer)
{
	if (m_vecShader.size() <= iContainer)
		return;

	m_vecShader[iContainer].resize(iSize);
}

void CRenderer::SetInstCount(int iIndex)
{
	if (m_pAni)
	{
		m_pAni->SetInstIndex(iIndex);
	}
}

CAnimation* CRenderer::GetAnimation() const
{
	if (m_pAni)
	{
		m_pAni->AddRef();
	}

	return m_pAni;
}

void CRenderer::SetTexture(REGISTER_TYPE eType, const std::string& strTag, 
	int iRegister, int iCount, unsigned int iType, size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return;
	}

	else if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset]->SetTexture(eType, strTag, iRegister, iCount, iType);
	}
}

void CRenderer::SetTexture(REGISTER_TYPE eType, CTexture* pTex, 
	int iRegister, int iCount, unsigned int iType, size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return;
	}
	else if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset]->SetTexture(eType, pTex, iRegister, iCount, iType);
	}
}

void CRenderer::SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, 
	const std::string& strPathName, int iRegister, int iCount, unsigned int iType, 
	size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return;
	}
	else if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset]->SetTexture(eType, strTag, pFileName,
			strPathName, iRegister, iCount, iType);
	}
}

void CRenderer::SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, 
	const TCHAR* pFullPath, int iRegister, int iCount, unsigned int iType, 
	size_t iContainer, size_t iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
	{
		return;
	}
	else if (m_vecMaterial[iContainer].size() <= iSubset)
	{
		return;
	}
	else if (m_vecMaterial[iContainer][iSubset])
	{
		m_vecMaterial[iContainer][iSubset]->SetTextureFromFullPath(eType, strTag, pFullPath,
			iRegister, iCount, iType);
	}
}

CRenderState* CRenderer::FindState(const std::string& strKey)
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_vecRenderState[i]->GetName() == strKey)
		{
			m_vecRenderState[i]->AddRef();

			return m_vecRenderState[i];
		}
	}

	return nullptr;
}

bool CRenderer::Init()
{
	if (!CComponent::Init())
		return false;

	return true;
}

void CRenderer::Start()
{
	if (m_pAni)
	{
		m_pAni->Start();
	}
}

void CRenderer::Update(float fTime)
{
	CComponent::Update(fTime);

	if (m_pPaper)
	{
		m_pPaper->Update(fTime);
	}
}

void CRenderer::PostUpdate(float fTime)
{
	CComponent::PostUpdate(fTime);
	if (m_pAni)
	{
		m_pAni->Update(fTime);
	}
}

void CRenderer::Collision(float fTime)
{
	CComponent::Collision(fTime);
}

void CRenderer::PreRender(float fTime)
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRenderState[i]->SetState();
	}
}

void CRenderer::Render(float fTime)
{
	if (m_pPaper)
	{
		m_pPaper->Render(fTime);
	}
	if (m_pAni)
	{
		m_pAni->SetShader();
	}

	if (m_pMesh)
	{
		const std::vector<PMESHCONTAINER>* pvecContainer = m_pMesh->GetMeshContainer();

		size_t iSize = pvecContainer->size();

		for (size_t i = 0; i < iSize; i++)
		{
			size_t iSubset = (*pvecContainer)[i]->vecSubSet.size();

			for (size_t j = 0; j < iSubset; j++)
			{
				if (m_vecMaterial.size() > i)
				{
					if (m_vecMaterial[i].size() > j)
					{
						if (m_vecMaterial[i][j])
						{
							m_vecMaterial[i][j]->SetMaterial();
						}
					}
				}

				if (m_vecShader.size() > i)
				{
					if (m_vecShader[i].size() > j)
					{
						if (m_vecShader[i][j])
						{
							m_vecShader[i][j]->SetShader();
						}
					}
				}

				m_pMesh->Render(fTime, (int)i, (int)j);
			}
		}
	}
	if (m_pAni)
	{
		m_pAni->ResetShader();
	}
}

void CRenderer::PostRender(float fTime)
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRenderState[i]->ResetState();
	}
}

void CRenderer::RenderShadow(float fTime)
{
	if (m_pAni)
	{
		m_pAni->SetShader();
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
				m_vecMaterial[i][j]->SetMaterial();

				m_pMesh->Render(fTime, i, j);
			}
		}
	}

	if (m_pAni)
	{
		m_pAni->ResetShader();
	}
}

CRenderer* CRenderer::Clone()
{
	return new CRenderer(*this);
}

void CRenderer::Save(FILE* pFile)
{
	CComponent::Save(pFile);

	int iSize = (int)m_vecRenderState.size();
	fwrite(&iSize, 4, 1, pFile);
	for (int i = 0; i < iSize; ++i)
	{
		std::string strTag = m_vecRenderState[i]->GetName();

		int iLength = (int)strTag.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strTag.c_str(), 1, iLength, pFile);
	}

	bool bMesh = false;

	if (m_pMesh)
	{
		bMesh = true;
	}

	fwrite(&bMesh, 1, 1, pFile);

	if (m_pMesh)
	{
		std::string strTag = m_pMesh->GetName();

		int iLength = (int)strTag.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strTag.c_str(), 1, iLength, pFile);
	}

	size_t iMtrlSz = m_vecMaterial.size();

	fwrite(&iMtrlSz, sizeof(size_t), 1, pFile);

	for (size_t i=0;i<iMtrlSz;i++)
	{
		size_t iSub = m_vecMaterial[i].size();
		fwrite(&iSub, sizeof(size_t), 1, pFile);
		for (size_t j = 0; j < iSub; ++j)
		{
			bool bMtrl = false;

			if (m_vecMaterial[i][j])
			{
				bMtrl = true;
			}

			fwrite(&bMtrl, 1, 1, pFile);

			if (m_vecMaterial[i][j])
			{
				std::string strTag = m_vecMaterial[i][j]->GetName();
				int iLength = (int)strTag.length();
				fwrite(&iLength, 4, 1, pFile);
				fwrite(strTag.c_str(), 1, iLength, pFile);
				m_vecMaterial[i][j]->Save(pFile);
			}
		}
	}

	size_t iShaderSz = m_vecShader.size();

	fwrite(&iShaderSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iShaderSz; i++)
	{
		size_t iSub = m_vecShader[i].size();

		fwrite(&iSub, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iSub; j++)
		{
			bool bShader = false;

			if (m_vecShader[i][j])
			{
				bShader = true;
			}
			fwrite(&bShader, 1, 1, pFile);

			if (m_vecShader[i][j])
			{
				std::string strTag = m_vecShader[i][j]->GetName();

				int iLength = (int)strTag.length();
				fwrite(&iLength, 4, 1, pFile);
				fwrite(strTag.c_str(), 1, iLength, pFile);
			}
		}
	}
}

void CRenderer::Load(FILE* pFile)
{
	CComponent::Load(pFile);

	int iSize = 0;
	fread(&iSize, 4, 1, pFile);
	for (int i = 0; i < iSize; ++i)
	{
		char strTag[256] = {};
		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strTag, 1, iLength, pFile);
		}

		AddRenderState(strTag);
	}

	bool bMesh = false;

	fread(&bMesh, 1, 1, pFile);

	if (bMesh)
	{
		char strTag[256] = {};
		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strTag, 1, iLength, pFile);
		}

		SetMesh(strTag);
	}

	size_t iMtrlSz = 0;

	fread(&iMtrlSz, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iMtrlSz; i++)
	{
		std::vector<CMaterial*> vecMtrl;

		m_vecMaterial.push_back(vecMtrl);

		size_t iSub = 0;

		fread(&iSub, sizeof(size_t), 1, pFile);

		m_vecMaterial[i].resize(iSub);

		for (size_t j = 0; j < iSub; ++j)
		{
			bool bMtrl = false;

			fread(&bMtrl, 1, 1, pFile);

			if (bMtrl)
			{
				int iLength = 0;
				fread(&iLength, 4, 1, pFile);
				char strTag[MAX_PATH] = {};
				if (iLength > 0)
				{
					fread(strTag, 1, iLength, pFile);
				}
				SetMaterial(strTag,i,j);
				CMaterial* pMtrl = GetMaterial(i,j);
				pMtrl->Load(pFile);
				SAFE_RELEASE(pMtrl);
			}
		}
	}

	size_t iShaderSz = 0;

	fread(&iShaderSz, sizeof(size_t), 1, pFile);

	m_vecShader.resize(iShaderSz);

	for (size_t i = 0; i < iShaderSz; i++)
	{
		size_t iSub = 0;

		fread(&iSub, sizeof(size_t), 1, pFile);

		std::vector<CShader*> vecShader;

		m_vecShader[i] = vecShader;

		m_vecShader[i].reserve(iSub);

		for (size_t j = 0; j < iSub; j++)
		{
			bool bShader = false;

			fread(&bShader, 1, 1, pFile);

			if (bShader)
			{
				int iLength = 0;
				fread(&iLength, 4, 1, pFile);
				char strTag[MAX_PATH] = {};
				if (iLength > 0)
				{
					fread(strTag, 1, iLength, pFile);
				}
				SetShader(strTag,i,j);				
			}
		}
	}
}

void CRenderer::SpawnWindow()
{
	CComponent::SpawnWindow();

	const char* pText = GetName().c_str();

	if (!strcmp(pText, ""))
	{
		pText = "None";
	}

	if (ImGui::Begin(pText))
	{
		if (m_pMesh)
		{
			const std::vector<PMESHCONTAINER>* pvecCon = m_pMesh->GetMeshContainer();

			size_t iMesh = pvecCon->size();

			char** pCon = new char* [iMesh];

			for (size_t i = 0; i < iMesh; i++)
			{
				pCon[i] = const_cast<char*>("Mesh");
			}
			static int iNum = -1;
			ImGui::ListBox("Mesh", &iNum, pCon, static_cast<int>(iMesh));

			delete[] pCon;
			if (iNum != -1 && m_vecMaterial.size() > iNum)
			{
				std::vector<const char*> vecMtrl;

				for (size_t j = 0; j < m_vecMaterial[iNum].size(); ++j)
				{
					if (m_vecMaterial[iNum][j])
					{
						vecMtrl.push_back(m_vecMaterial[iNum][j]->GetName().c_str());
					}
				}

				static int iSub = -1;
				ImGui::ListBox("Subset", &iSub, pCon, static_cast<int>(m_vecMaterial[iNum].size()));

				if (iSub != -1 && m_vecMaterial[iNum].size() > iSub)
				{
					if (m_vecMaterial[iNum][iSub])
					{
						m_vecMaterial[iNum][iSub]->SpawnWindow();
					}
				}
			}
		}

		if (m_pAni)
		{
			m_pAni->SpawnWindow();
		}


		static int iItem = 0;
		char strState[256] = {};

		size_t iSize = m_vecRenderState.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			strcat_s(strState, m_vecRenderState[i]->GetName().c_str());
			strcat_s(strState, " ");
		}

		ImGui::Combo("RenderState", &iItem, strState);
	}
	ImGui::End();
}

void CRenderer::RenderParticle(int iCount)
{
	if (m_pMesh)
	{
		m_pMesh->RenderParticle(iCount);
	}
}
