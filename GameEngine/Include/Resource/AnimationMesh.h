#pragma once
#include "Mesh.h"
class CAnimationMesh :
	public CMesh
{
	friend class CResourceManager;

private:
	CAnimationMesh();
	virtual ~CAnimationMesh();

public:
	bool Init();
	virtual bool CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg = D3D11_USAGE_IMMUTABLE,
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	virtual void Render(float fTime, int iContainer = 0, int iSubset = 0);
	virtual void RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize, int iContainer = 0, int iSubset = 0);
	virtual void RenderParticle(int iCount, int iContainer = 0, int iSubset = 0);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

