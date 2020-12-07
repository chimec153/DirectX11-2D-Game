#pragma once

#include "Mesh.h"

class CMesh2D :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CMesh2D();
	~CMesh2D();

private:
	D3D_PRIMITIVE_TOPOLOGY	m_ePrimitive;
	VertexBuffer		m_tVB;
	IndexBuffer			m_tIB;

public:
	bool Init();
	bool CreateMesh();
	virtual bool CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg,
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	virtual void Render(float fTime);
	virtual void RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

