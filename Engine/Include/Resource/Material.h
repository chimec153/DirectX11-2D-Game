#pragma once

#include "../Ref.h"

typedef struct _tagMaterialTexture
{
	class CTexture*		pTexture;
	int					iRegister;
	int					iCount;
	unsigned int		iType;
}MaterialTexture,*PMaterialTexture;

class CMaterial :
	public CRef
{
	friend class CResourceManager;
	friend class CMesh;

private:
	CMaterial();
	CMaterial(const CMaterial& material);
	virtual ~CMaterial();

private:
	ShaderCBuffer					m_tCBuffer;
	std::vector<PMaterialTexture>	m_vecTexture;

public:
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTexture(REGISTER_TYPE eType, class CTexture* pTex, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	const Vector2 GetTextureSize(int idx = 0);
	size_t GetTextureCount()	const;
	const Vector4& GetDif()	const;
	const Vector4& GetAmb()	const;

public:
	void SetDiffuseColor(const Vector4& v);
	void SetDiffuseColor(float x, float y, float z, float w);
	void SetAmbientColor(const Vector4& v);
	void SetAmbientColor(float x, float y, float z, float w);
	void SetSpecularColor(const Vector4& v);
	void SetSpecularColor(float x, float y, float z, float w);
	void SetEmissiveColor(const Vector4& v);
	void SetEmissiveColor(float x, float y, float z, float w);
	void SetSpecularPower(float fPower);
	void SetAnimation(bool bGray);
	void SetBump(bool bBump);
	void SetDiffuseTexture(bool bDiffuseTexture);
	void SetNormalTexture(bool bTexture);
	void SetSpecularTexture(bool bTexture);
	void SetEmissiveTexture(bool bTexture);
	void SetMaterial();
	void SetTexturePathKey(const std::string& strPathKey);
	void ChangeTexturePath(const char* pPath);
	void SetDecal(bool bDecal = true);
	CMaterial* Clone();

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	void SpawnWindow();

};

