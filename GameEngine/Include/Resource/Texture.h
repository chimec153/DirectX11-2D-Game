#pragma once

#include "../Ref.h"

typedef struct _tagTextureInfo
{
	ScratchImage*				pImage;
	TCHAR*						pFileName;
	char*						pPathName;
	TCHAR*						pFullPath;
	ID3D11ShaderResourceView*	pSRV;
	unsigned int				iWidth;
	unsigned int				iHeight;
	int							iFileNameLength;
	int							iPathNameLength;
	int							iFullPathLength;

	_tagTextureInfo()	:
		pSRV(nullptr),
		iWidth(0),
		iHeight(0)
	{
		pFileName = new TCHAR[MAX_PATH];
		pPathName = new char[MAX_PATH];
		pFullPath = new TCHAR[MAX_PATH];

		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		memset(pPathName, 0, sizeof(char) * MAX_PATH);
		memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

		pImage = new ScratchImage;
	}

	~_tagTextureInfo()
	{
		SAFE_DELETE(pImage);
		SAFE_RELEASE(pSRV);
		delete[] pFileName;
		delete[] pPathName;
		delete[] pFullPath;
	}
}TextureInfo, *PTextureInfo;

class CTexture :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CTexture();
	virtual ~CTexture();

private:
	std::vector<PTextureInfo>	m_vecTextureInfo;
	IMAGE_TYPE					m_eImageType;

public:
	unsigned int GetWidth(int idx = 0)	const;
	unsigned int GetHeigth(int idx = 0)	const;
	void SetImageType(IMAGE_TYPE eType);
	const Vector2 GetSize(int idx = 0)	const;

	IMAGE_TYPE GetImageType()	const
	{
		return m_eImageType;
	}

public:
	bool Init();

public:
	void SetResource(ID3D11ShaderResourceView* pSRV, int idx = 0);
	bool LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath);
	bool LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath);
	void SetTexture(int iRegister, int iType, int idx = 0);
	ID3D11ShaderResourceView* CreateRandomTexture1DSRV();

private:
	bool CreateResource(int idx = 0);
	bool CreateAarryResource();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

