#include "Material.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "../Device.h"

CMaterial::CMaterial()	:
	m_pShader(nullptr)
{
	memset(&m_tCBuffer, 0, sizeof(ShaderCBuffer));
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vAmp = Vector4::White;
	m_tCBuffer.vSpe = Vector4::White;
}

CMaterial::CMaterial(const CMaterial& material)	:
	CRef(material)
{
	m_tCBuffer = material.m_tCBuffer;

	m_pShader = material.m_pShader;

	if (m_pShader)
		m_pShader->AddRef();

	m_vecTexture.clear();

	size_t iSize = material.m_vecTexture.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PMaterialTexture pMatTex = new MaterialTexture;
		
		*pMatTex = *material.m_vecTexture[i];

		if (pMatTex->pTexture)
			pMatTex->pTexture->AddRef();

		m_vecTexture.push_back(pMatTex);
	}
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_pShader);

	size_t iSize = m_vecTexture.size();

	for (size_t i = 0; i < iSize; ++i)
		SAFE_RELEASE(m_vecTexture[i]->pTexture);
		
	SAFE_DELETE_VECLIST(m_vecTexture);
}

void CMaterial::SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister, int iCount, unsigned int iType)
{
	CTexture* pTex = GET_SINGLE(CResourceManager)->FindTexture(strTag);

	if (!pTex)
		return;

	PMaterialTexture pMatTex = new MaterialTexture;

	m_tCBuffer.iDifTex = 1;
	pMatTex->pTexture = pTex;
	pMatTex->iCount = iCount;
	pMatTex->iType = iType;
	pMatTex->iRegister = (int)eType;

	switch (eType)
	{
	case REGISTER_TYPE::RT_DIF:
		m_tCBuffer.iDifTex = 1;
		break;
	case REGISTER_TYPE::RT_NRM:
		m_tCBuffer.iNrmTex = 1;
		break;
	case REGISTER_TYPE::RT_SPE:
		m_tCBuffer.iSpeTex = 1;
		break;
	case REGISTER_TYPE::RT_CUSTOM:
		pMatTex->iRegister = iRegister;
		break;
	}

	m_vecTexture.push_back(pMatTex);
}

void CMaterial::SetTexture(REGISTER_TYPE eType, CTexture* pTex, int iRegister, int iCount, unsigned int iType)
{
	if (!pTex)
		return;

	PMaterialTexture pMatTex = new MaterialTexture;

	m_tCBuffer.iDifTex = 1;
	pMatTex->pTexture = pTex;
	pMatTex->iCount = iCount;
	pMatTex->iType = iType;
	pMatTex->iRegister = (int)eType;

	switch (eType)
	{
	case REGISTER_TYPE::RT_DIF:
		m_tCBuffer.iDifTex = 1;
		break;
	case REGISTER_TYPE::RT_NRM:
		m_tCBuffer.iNrmTex = 1;
		break;
	case REGISTER_TYPE::RT_SPE:
		m_tCBuffer.iSpeTex = 1;
		break;
	case REGISTER_TYPE::RT_CUSTOM:
		pMatTex->iRegister = iRegister;
		break;
	}

	m_vecTexture.push_back(pMatTex);
}

void CMaterial::SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName, int iRegister, int iCount, unsigned int iType)
{
	if (!GET_SINGLE(CResourceManager)->LoadTexture(strTag, pFileName, strPathName))
		return;

	PMaterialTexture pMatTex = new MaterialTexture;

	m_tCBuffer.iDifTex = 1;
	pMatTex->pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTag);
	pMatTex->iCount = iCount;
	pMatTex->iType = iType;
	pMatTex->iRegister = (int)eType;

	switch (eType)
	{
	case REGISTER_TYPE::RT_DIF:
		m_tCBuffer.iDifTex = 1;
		break;
	case REGISTER_TYPE::RT_NRM:
		m_tCBuffer.iNrmTex = 1;
		break;
	case REGISTER_TYPE::RT_SPE:
		m_tCBuffer.iSpeTex = 1;
		break;
	case REGISTER_TYPE::RT_CUSTOM:
		pMatTex->iRegister = iRegister;
		break;
	}

	m_vecTexture.push_back(pMatTex);
}

void CMaterial::SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister, int iCount, unsigned int iType)
{
	if (!GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(strTag, pFullPath))
		return;

	PMaterialTexture pMatTex = new MaterialTexture;

	m_tCBuffer.iDifTex = 1;
	pMatTex->pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTag);
	pMatTex->iCount = iCount;
	pMatTex->iType = iType;
	pMatTex->iRegister = (int)eType;

	switch (eType)
	{
	case REGISTER_TYPE::RT_DIF:
		m_tCBuffer.iDifTex = 1;
		break;
	case REGISTER_TYPE::RT_NRM:
		m_tCBuffer.iNrmTex = 1;
		break;
	case REGISTER_TYPE::RT_SPE:
		m_tCBuffer.iSpeTex = 1;
		break;
	case REGISTER_TYPE::RT_CUSTOM:
		pMatTex->iRegister = iRegister;
		break;
	}

	m_vecTexture.push_back(pMatTex);
}

const Vector2 CMaterial::GetTextureSize(int idx)
{
	return Vector2((float)m_vecTexture[idx]->pTexture->GetWidth(), (float)m_vecTexture[idx]->pTexture->GetHeigth());
}

size_t CMaterial::GetTextureCount() const
{
	return m_vecTexture.size();
}

const Vector4& CMaterial::GetDif() const
{
	return m_tCBuffer.vDif;
}

void CMaterial::SetShader(const std::string& strName)
{
	SAFE_RELEASE(m_pShader);

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strName);
}

void CMaterial::SetDiffuseColor(const Vector4& v)
{
	m_tCBuffer.vDif = v;
}

void CMaterial::SetDiffuseColor(float x, float y, float z, float w)
{
	SetDiffuseColor(Vector4(x, y, z, w));
}

void CMaterial::SetAmbientColor(const Vector4& v)
{
	m_tCBuffer.vAmp = v;
}

void CMaterial::SetAmbientColor(float x, float y, float z, float w)
{
	SetAmbientColor(Vector4(x, y, z, w));
}

void CMaterial::SetSpecularColor(const Vector4 & v)
{
	m_tCBuffer.vSpe = v;
}

void CMaterial::SetSpecularColor(float x, float y, float z, float w)
{
	SetSpecularColor(Vector4(x, y, z, w));
}

void CMaterial::SetMaterial()
{
	if (m_pShader)
		m_pShader->SetShader();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tCBuffer);

	size_t iSize = m_vecTexture.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecTexture[i]->pTexture->SetTexture(m_vecTexture[i]->iRegister, m_vecTexture[i]->iType);
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::Save(FILE* pFile)
{
	bool bShader = false;

	if (m_pShader)
		bShader = true;

	fwrite(&bShader, 1, 1, pFile);

	if (m_pShader)
	{
		int iLength = (int)m_pShader->GetName().length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(m_pShader->GetName().c_str(), 1, iLength, pFile);
	}

	fwrite(&m_tCBuffer.vDif, sizeof(Vector4), 1, pFile);
	fwrite(&m_tCBuffer.vAmp, sizeof(Vector4), 1, pFile);
	fwrite(&m_tCBuffer.vSpe, sizeof(Vector4), 1, pFile);
}

void CMaterial::Load(FILE* pFile)
{
	bool bShader = false;

	fread(&bShader, 1, 1, pFile);

	if (bShader)
	{
		int iLength = 0;
		char strTag[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strTag, 1, iLength, pFile);

		SetShader(strTag);
	}

	fread(&m_tCBuffer.vDif, sizeof(Vector4), 1, pFile);
	fread(&m_tCBuffer.vAmp, sizeof(Vector4), 1, pFile);
	fread(&m_tCBuffer.vSpe, sizeof(Vector4), 1, pFile);
}
