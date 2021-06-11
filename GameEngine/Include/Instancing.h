#pragma once

#include "GameEngine.h"

class CInstancing
{
	friend class CRenderManager;
	friend class CLayer;

private:
	CInstancing();
	~CInstancing();

private:
	PINSTANCINGBUFFER	m_pBuffer;
	int					m_iCount;
	bool				m_bAnimation;
	class CMesh*		m_pMesh;
	class CMaterial*	m_pMaterial;
	class CShader*		m_pShader;
	class CLayer*		m_pLayer;
	class CUIFont*		m_pFont;
	std::vector<class CSceneComponent*>		m_vecCom;
	std::vector<class CRenderState*>	m_vecState;
	class CRenderer*	m_pRenderer;

public:
	void SetLayer(class CLayer* pLayer);
	int GetCount()	const;
	void AddCom(class CSceneComponent* pCom);
	const std::vector<class CSceneComponent*>* GetComs()	const;
	void AddState(const std::string& strKey);
	void AddState(class CRenderState* pState);

public:
	bool Init(CMesh* pMesh, CMaterial* pMtrl, class CShader* pShader, int iCount , int iSize);
	void SetFontRect(float l, float t, float r, float b);
	void SetFontPos(const Vector3& vPos);
	bool CreateInstancingBuffer(int iCount, int iSize);
	void AddInstanceData(const void* pData);
	void ResizeBuffer(int iCount, int iSize);
	void Update(float fTime);
	void Render(float fTime);
	void Clear();
};

