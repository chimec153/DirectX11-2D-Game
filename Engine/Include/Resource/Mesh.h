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
	virtual void Render(float fTime) = 0;
};

