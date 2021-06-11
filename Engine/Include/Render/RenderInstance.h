#pragma once
#include "../Ref.h"

typedef struct _tagInstMatrixies
{
    Matrix  matWV;
    Matrix  matWVP;
}INSTMATRIXIES, *PINSTMATRIXIES;

class CRenderInstance :
    public CRef
{
    friend class CRenderManager;

private:
    CRenderInstance();
    virtual ~CRenderInstance();

private:
    class CMesh*                                    m_pMesh;
    class CShader*                                  m_pShader;
    std::vector<std::vector<class CMaterial*>>      m_vecMaterial;
    std::vector<std::vector<class CRenderState*>>   m_vecRenderState[static_cast<int>(RENDER_STATE::END)];
    size_t                  m_iCount;
    PINSTANCINGBUFFER       m_pInstBuffer;
    std::list<class CSceneComponent*> m_ComponentList;
    ID3D11ShaderResourceView* m_pSRV;
    ID3D11UnorderedAccessView* m_pUAV;
    int                         m_iBoneCount;
    class CShader* m_pShadowShader;

public:
    void AddData(void* pData);
    void AddComponent(class CSceneComponent* pCom);
    void DeleteAllComponent();
    const std::list<class CSceneComponent*>& GetComponentList() const;
    int GetCount()  const;
    ID3D11UnorderedAccessView* GetBoneUav() const;

public:
    bool Init(class CMesh* pMesh,class CShader* pShader, size_t iSize, size_t iMaxCount = 1000, int iBoneCount = 0);
    bool CreateInstBuffer(int iSize, int iCount);
    void Render(float fTime);
    void RenderShadow(float fTime);

    void Clear();
    bool CreateBoneBuffer(int iCount);
};

