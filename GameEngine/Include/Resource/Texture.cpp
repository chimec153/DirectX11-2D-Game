#include "Texture.h"
#include "../PathManager.h"
#include "../Device.h"

CTexture::CTexture()	:
	m_eImageType(IMAGE_TYPE::IT_ATLAS)
	, m_pArraySRV(nullptr)
{
}

CTexture::~CTexture()
{
	SAFE_DELETE_VECLIST(m_vecTextureInfo);
	SAFE_RELEASE(m_pArraySRV);
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

IMAGE_TYPE CTexture::GetImageType() const
{
	return m_eImageType;
}

ID3D11ShaderResourceView* CTexture::GetArraySRV() const
{
	return m_pArraySRV;
}

ScratchImage* CTexture::GetImage(int idx) const
{
	if (m_vecTextureInfo.size() <= idx)
		return nullptr;

	return m_vecTextureInfo[idx]->pImage;
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

	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	return CreateResource();
}

bool CTexture::LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath)
{
	SetName(strTag);

	PTextureInfo pInfo = new TextureInfo;

	m_vecTextureInfo.push_back(pInfo);

	lstrcpy(pInfo->pFullPath, pFullPath);

	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
		{
			SAFE_DELETE(pInfo->pImage);
			return false;
		}
	}

	return CreateResource();
}

bool CTexture::LoadTexture(const std::string& strTag, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName)
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

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

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

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		bool bResult = CreateResource((int)i);

		if (!bResult)
			return false;
	}

	return true;
}

bool CTexture::LoadTextureArray(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName)
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

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}
	}

	return CreateAarryResource();
}

bool CTexture::LoadTextureArrayFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath)
{
	SetName(strTag);

	size_t iSize = vecFullPath.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		PTextureInfo pInfo = new TextureInfo;

		m_vecTextureInfo.push_back(pInfo);

		lstrcpy(pInfo->pFullPath, vecFullPath[i]);

		TCHAR strExt[_MAX_EXT] = {};

		_wsplitpath_s(pInfo->pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

		char pExt[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

		_strupr_s(pExt);

		if (strcmp(pExt, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(pInfo->pFullPath, DDS_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else if (strcmp(pExt, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(pInfo->pFullPath, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(pInfo->pFullPath, WIC_FLAGS_NONE, nullptr, *pInfo->pImage)))
			{
				SAFE_DELETE(pInfo->pImage);
				return false;
			}
		}
	}

	return CreateAarryResource();
}

void CTexture::SetTexture(int iRegister, int iType, int idx)
{
	if (m_eImageType != IMAGE_TYPE::ARRAY_TEXTURE)
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
	else
	{
		if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
			CONTEXT->VSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
			CONTEXT->PSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
			CONTEXT->HSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
			CONTEXT->DSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
			CONTEXT->GSSetShaderResources(iRegister, 1, &m_pArraySRV);

		if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
			CONTEXT->CSSetShaderResources(iRegister, 1, &m_pArraySRV);
	}
}

void CTexture::SetPathKey(const std::string& strPathKey)
{
	size_t iSize = m_vecTextureInfo.size();

	for (size_t i = 0; i < iSize; i++)
	{
		strcpy_s(m_vecTextureInfo[i]->pPathName, MAX_PATH, strPathKey.c_str());
	}
}

void CTexture::ChangePath(const char* pFilePath)
{
	TCHAR strFilePath[MAX_PATH] = {};

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, pFilePath, -1, strFilePath, static_cast<int>(strlen(pFilePath)));
#else
	strcpy_s(strFilePath, MAX_PATH, pFilePath);
#endif

	size_t iSize = m_vecTextureInfo.size();

	for (size_t i = 0; i < iSize; i++)
	{
		wcscpy_s(m_vecTextureInfo[i]->pFileName, MAX_PATH, strFilePath);
	}
}

bool CTexture::CreateResource(int idx)
{
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
	m_eImageType = IMAGE_TYPE::ARRAY_TEXTURE;

	ScratchImage* pImage = new ScratchImage;

	int iMipLevel = 0;
	int iWidth = 0;
	int iHeight = 0;
	for (int i = 0; i < m_vecTextureInfo.size(); i++)
	{
		if (m_vecTextureInfo[i]->pImage->GetMetadata().mipLevels > iMipLevel)
			iMipLevel = static_cast<int>(m_vecTextureInfo[i]->pImage->GetMetadata().mipLevels);
		if (m_vecTextureInfo[i]->pImage->GetMetadata().width > iWidth)
			iWidth = static_cast<int>(m_vecTextureInfo[i]->pImage->GetMetadata().width);
		if (m_vecTextureInfo[i]->pImage->GetMetadata().height > iHeight)
			iHeight = static_cast<int>(m_vecTextureInfo[i]->pImage->GetMetadata().height);
	}
	
	HRESULT tResult = pImage->Initialize2D(
		m_vecTextureInfo[0]->pImage->GetMetadata().format,
		iWidth,
		iHeight,
		m_vecTextureInfo.size(),
		iMipLevel);

	iMipLevel = static_cast<int>(pImage->GetMetadata().mipLevels);

	for (int i = 0; i<m_vecTextureInfo.size(); i++)
	{
		const Image* _pImage = m_vecTextureInfo[i]->pImage->GetImages();

		int iMipLevel = static_cast<int>(m_vecTextureInfo[i]->pImage->GetMetadata().mipLevels);

		for (int j = 0; j < iMipLevel; j++)
		{
			const Image* pDest = &pImage->GetImages()[i * iMipLevel + j];

			const Image* pSrc = &_pImage[j];

			memcpy(pDest->pixels,pSrc->pixels, pDest->slicePitch);
		}
	}
	
	ID3D11Texture2D* pTexture = nullptr;

	if (FAILED(CreateTextureEx(DEVICE, pImage->GetImages(), 
		pImage->GetImageCount(), pImage->GetMetadata(), D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, FALSE, reinterpret_cast<ID3D11Resource**>(&pTexture))))
	{
		SAFE_DELETE(pImage);
		assert(false);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tDesc = {};

	tDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tDesc.Texture2DArray.ArraySize = static_cast<unsigned int>(m_vecTextureInfo.size());
	tDesc.Texture2DArray.FirstArraySlice = 0;
	tDesc.Texture2DArray.MipLevels = static_cast<UINT>(pImage->GetMetadata().mipLevels);
	tDesc.Texture2DArray.MostDetailedMip = 0;
	tDesc.Format = pImage->GetMetadata().format;

	SAFE_RELEASE(m_pArraySRV);
	if (FAILED(DEVICE->CreateShaderResourceView(pTexture, &tDesc, &m_pArraySRV)))
	{
		SAFE_RELEASE(pTexture);
		SAFE_DELETE(pImage);
		return false;
	}

	SAFE_RELEASE(pTexture);
	SAFE_DELETE(pImage);

	return true;
}

void CTexture::Save(FILE* pFile)
{
	CRef::Save(pFile);

	int iSz = (int)m_vecTextureInfo.size();

	fwrite(&iSz, 4, 1, pFile);

	for (int i = 0; i < iSz; ++i)
	{
		int iLength = static_cast<int>(lstrlen(m_vecTextureInfo[i]->pFullPath));
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite(m_vecTextureInfo[i]->pFullPath, sizeof(TCHAR), iLength, pFile);
		}
		iLength = static_cast<int>(lstrlen(m_vecTextureInfo[i]->pFileName));
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite(m_vecTextureInfo[i]->pFileName, sizeof(TCHAR), iLength, pFile);
		}
		iLength = static_cast<int>(strlen(m_vecTextureInfo[i]->pPathName));
		fwrite(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fwrite(m_vecTextureInfo[i]->pPathName, 1, iLength, pFile);
		}
	}

}

void CTexture::Load(FILE* pFile)
{
	CRef::Load(pFile);

	int iSz = 0;

	fread(&iSz, 4, 1, pFile);

	for (int i = 0; i < iSz; ++i)
	{
		PTextureInfo pInfo = new TextureInfo;

		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(pInfo->pFullPath, sizeof(TCHAR), iLength, pFile);
		}
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(pInfo->pFileName, sizeof(TCHAR), iLength, pFile);
		}
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(pInfo->pPathName, 1, iLength, pFile);
		}

		if (pInfo->pFullPath)
		{
			LoadTextureFromFullPath(GetName(), pInfo->pFullPath);
		}
		else
		{
			LoadTexture(GetName(), pInfo->pFileName, pInfo->pPathName);
		}

		m_vecTextureInfo.push_back(pInfo);
	}
}

