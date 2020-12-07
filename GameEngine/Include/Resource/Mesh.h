#pragma once

#include "../Ref.h"

typedef struct _tagVertexBuffer
{
	ID3D11Buffer*			pBuffer;
	int						iSize;
	int						iCount;
	D3D11_USAGE				eUsage;
}VertexBuffer,*PVertexBuffer;

typedef struct _tagIndexBuffer
{
	ID3D11Buffer*			pBuffer;
	int						iSize;
	int						iCount;
	D3D11_USAGE				eUsage;
	DXGI_FORMAT				eFmt;
}IndexBuffer,*PIndexBuffer;

class CMesh :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	Vector3					m_tMax;
	Vector3					m_tMin;
	class CMaterial*		m_pMaterial;
	PINSTANCINGBUFFER		m_pInstancingBuffer;

public:
	Vector3 GetMax()	const
	{
		return m_tMax;
	}

	Vector3 GetMin()	const
	{
		return m_tMin;
	}

	void SetMaterial(class CMaterial* pMaterial);
	class CMaterial* GetMaterial()	const;

public:
	bool Init();
	virtual bool CreateInstancingBuffer(int iCount, int iSize);
	virtual bool CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg,
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	virtual void Render(float fTime);
	virtual void RenderInstancing(void* pData, int iCount ,int iSize);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

