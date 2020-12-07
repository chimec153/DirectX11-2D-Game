#include "Texture.h"
#include "../PathManager.h"
#include "../Device.h"

CTexture::CTexture()	:
	m_eImageType(IMAGE_TYPE::IT_ATLAS)
{
}

CTexture::~CTexture()
{
	SAFE_DELETE_VECLIST(m_vecTextureInfo);
}

unsigned int CTexture::GetWidth(int idx) const
{
	return m_vecTextureInfo[idx]->iWidth;
}

unsigned int CTexture::GetHeigth(int idx) const
{
	return m_vecTextureInfo[idx]->iHeight;
}

void CTexture::SetImageType(IMAGE_TYPE eType)
{
	m_eImageType = eType;
}

const Vector2 CTexture::GetSize(int idx) const
{
	return Vector2((float)m_vecTextureInfo[idx]->iWidth, (float)m_vecTextureInfo[idx]->iHeight);
}

bool CTexture::Init()
{
	return true;
}

void CTexture::SetResource(ID3D11ShaderResourceView* pSRV, int idx)
{
	SAFE_RELEASE(m_vecTextureInfo[idx]->pSRV);

	m_vecTextureInfo[idx]->pSRV = pSRV;

	if(pSRV)
		pSRV->AddRef();
}

bool CTexture::LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName)
{
	SetName(strTag);

	PTextureInfo pInfo = new TextureInfo;

	m_vecTextureInfo.push_back(pInfo);

	lstrcpy(pInfo->pFileName, pFileName);

	strcpy_s(pInfo->pPathName, strPathName.size() + 1, strPathName.c_str());

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(pInfo->pPathName);

	if (pPath)
		lstrcpy(pInfo->pFullPath, pPath);

	lstrcat(pInfo->pFullPath, pInfo->pFileName);	

	return CreateResource();
}

bool CTexture::LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath)
{
	SetName(strTag);

	PTextureInfo pInfo = new TextureInfo;

	m_vecTextureInfo.push_back(pInfo);

	lstrcpy(pInfo->pFullPath, pFullPath);

	return CreateResource();
}

bool CTexture::LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName)
{
	SetName(strTag);

	size_t iSize = vecFileName.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PTextureInfo pInfo = new TextureInfo;

		m_vecTextureInfo.push_back(pInfo);

		lstrcpy(pInfo->pFileName, vecFileName[i]);

		strcpy_s(pInfo->pPathName, strPathName.size() + 1, strPathName.c_str());

		const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(pInfo->pPathName);

		if (pPath)
			lstrcpy(pInfo->pFullPath, pPath);

		lstrcat(pInfo->pFullPath, pInfo->pFileName);

		bool bResult = CreateResource((int)i);

		if (!bResult)
			return false;
	}

	return true;
}

bool CTexture::LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath)
{
	SetName(strTag);

	size_t iSize = vecFullPath.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PTextureInfo pInfo = new TextureInfo;

		m_vecTextureInfo.push_back(pInfo);

		lstrcpy(pInfo->pFullPath, vecFullPath[i]);

		bool bResult = CreateResource((int)i);

		if (!bResult)
			return false;
	}

	return true;
}

void CTexture::SetTexture(int iRegister, int iType, int idx)
{
	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		CONTEXT->VSSetShaderResources(iRegister, 1, &m_vecTextureInfo[idx]->pSRV);

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_vecTextureInfo[idx]->pSRV);

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
		CONTEXT->HSSetShaderResources(iRegister, 1, &m_vecTextureInfo[idx]->pSRV);

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
		CONTEXT->DSSetShaderResources(iRegister, 1, &m_vecTextureInfo[idx]->pSRV);

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
		CONTEXT->GSSetShaderResources(iRegister, 1, &m_vecTextureInfo[idx]->pSRV);

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
		CONTEXT->CSSetShaderResources(iRegister, 1, &m_vecTextureInfo[idx]->pSRV);
}

ID3D11ShaderResourceView* CTexture::CreateRandomTexture1DSRV()
{
	Vector4 vRand[1024];

	for (int i = 0; i < 1024; ++i)
	{
		vRand[i].x = rand() / (RAND_MAX / 2.f) - 1.f;
		vRand[i].y = rand() / (RAND_MAX / 2.f) - 1.f;
		vRand[i].z = rand() / (RAND_MAX / 2.f) - 1.f;
		vRand[i].w = rand() / (RAND_MAX / 2.f) - 1.f;
	}

	D3D11_SUBRESOURCE_DATA tData = {};

	tData.pSysMem = vRand;
	tData.SysMemPitch = 1024 * sizeof(Vector4);

	D3D11_TEXTURE1D_DESC tDesc = {};

	tDesc.Width = 1024;
	tDesc.MipLevels = 1;
	tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.ArraySize = 1;

	ID3D11Texture1D* pTex = nullptr;

	if (FAILED(DEVICE->CreateTexture1D(&tDesc, &tData, &pTex)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tViewDesc = {};
	tViewDesc.Format = tDesc.Format;
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	tViewDesc.Texture1D.MipLevels = tDesc.MipLevels;

	ID3D11ShaderResourceView* pSRV = nullptr;

	if (FAILED(DEVICE->CreateShaderResourceView(pTex, &tViewDesc, &pSRV)))
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	SAFE_RELEASE(pTex);

	return pSRV;
}

bool CTexture::CreateResource(int idx)
{
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(m_vecTextureInfo[idx]->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(m_vecTextureInfo[idx]->pFullPath, DDS_FLAGS_NONE, nullptr, *m_vecTextureInfo[idx]->pImage)))
		{
			SAFE_DELETE(m_vecTextureInfo[idx]->pImage);
			return false;
		}
	}

	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(m_vecTextureInfo[idx]->pFullPath, nullptr, *m_vecTextureInfo[idx]->pImage)))
		{
			SAFE_DELETE(m_vecTextureInfo[idx]->pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(m_vecTextureInfo[idx]->pFullPath, WIC_FLAGS_NONE, nullptr, *m_vecTextureInfo[idx]->pImage)))
		{
			SAFE_DELETE(m_vecTextureInfo[idx]->pImage);
			return false;
		}
	}

	if (FAILED(CreateShaderResourceView(DEVICE, 
		m_vecTextureInfo[idx]->pImage->GetImages(), 
		m_vecTextureInfo[idx]->pImage->GetImageCount(), 
		m_vecTextureInfo[idx]->pImage->GetMetadata(), 
		&m_vecTextureInfo[idx]->pSRV)))
		return false;

	m_vecTextureInfo[idx]->iHeight = (unsigned int)m_vecTextureInfo[idx]->pImage->GetImages()->height;
	m_vecTextureInfo[idx]->iWidth = (unsigned int)m_vecTextureInfo[idx]->pImage->GetImages()->width;

	return true;
}

bool CTexture::CreateAarryResource()
{
	return false;
}

void CTexture::Save(FILE* pFile)
{
	CRef::Save(pFile);

	int iSz = (int)m_vecTextureInfo.size();

	fwrite(&iSz, 4, 1, pFile);

	for (int i = 0; i < iSz; ++i)
	{
		//fwrite(m_vecTextureInfo[i]->pFileName
	}

}

void CTexture::Load(FILE* pFile)
{
	CRef::Load(pFile);
}

