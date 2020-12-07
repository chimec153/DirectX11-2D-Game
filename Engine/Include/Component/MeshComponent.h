#pragma once
#include "PrimitiveComponent.h"
class CMeshComponent :
	public CPrimitiveComponent
{
	friend class CObj;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent& com);
	virtual ~CMeshComponent();

protected:
	class CMaterial*		m_pMaterial;

public:
	class CMaterial* GetMaterial()	const;
	void SetMaterial(class CMaterial* pMaterial);

public:
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTexture(REGISTER_TYPE eType, class CTexture* pTex, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
	virtual CMeshComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

