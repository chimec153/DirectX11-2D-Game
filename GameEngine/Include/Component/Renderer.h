#pragma once

#include "Component.h"

class CRenderer :
    public CComponent
{
    friend class CSceneComponent;

private:
    CRenderer();
    CRenderer(const CRenderer& com);
    virtual ~CRenderer();

private:
    std::vector<class CRenderState*>    m_vecRenderState;
    class CMesh*                        m_pMesh;
    std::vector<std::vector<class CMaterial*>>       m_vecMaterial;
    std::vector<std::vector<class CShader*>>         m_vecShader;
	class CPaperBurn*					m_pPaper;
	class CAnimation*					m_pAni;
	class CSceneComponent*				m_pOwner;

public:
    void AddRenderState(const std::string& strKey);
    class CMesh* GetMesh()	const;
    void SetMesh(const std::string& strName);
    void SetMesh(class CMesh* pMesh);
    class CMaterial* GetMaterial(size_t iContainer = 0, size_t iSubset = 0)	const;
    void SetMaterial(class CMaterial* pMaterial, size_t iContainer = 0, size_t iSubset = 0);
    void SetMaterial(const std::string& strTag, size_t iContainer = 0, size_t iSubset = 0);
    void SetShader(const std::string& strTag, size_t iContainer = 0, size_t iSubset = 0);
    void SetShader(class CShader* pShader, size_t iContainer = 0, size_t iSubset = 0);
    class CShader* GetShader(size_t iContainer = 0, size_t iSubset = 0)  const;
    void SetState();
    void ResetState();
    const std::vector<class CRenderState*>& GetVecRenderState() const;
    void DeleteRenderState(const std::string& strKey);
	void SetPaper(class CPaperBurn* pBurn);
	void SetAnimation(class CAnimation* pAni);
    void SetMaterial(size_t iContainer = 0, size_t iSubset = 0);
    void SetShader(size_t iContainer = 0, size_t iSubset = 0);
    void ResizeMaterial(int iSize, int iContainter = 0);
    void ResizeShader(int iSize, int iContainter = 0);
    void SetInstCount(int iIndex);
    class CAnimation* GetAnimation()    const;

public:
    void SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister = 0, int iCount = 1,
    unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX, 
        size_t iContainer = 0, size_t iSubset = 0);
    void SetTexture(REGISTER_TYPE eType, class CTexture* pTex, int iRegister = 0, int iCount = 1,
        unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
        size_t iContainer = 0, size_t iSubset = 0);
    void SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH, int iRegister = 0, int iCount = 1,
        unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
        size_t iContainer = 0, size_t iSubset = 0);
    void SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister = 0, int iCount = 1,
        unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
        size_t iContainer = 0, size_t iSubset = 0);

private:
    class CRenderState* FindState(const std::string& strKey);

public:
    virtual bool Init();
	virtual void Start() override;
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual void RenderShadow(float fTime);
    virtual CRenderer* Clone();
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

public:
    void SpawnWindow() override;

public:
    void RenderParticle(int iCount);
};

