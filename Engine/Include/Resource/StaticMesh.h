#pragma once
#include "Mesh.h"
class CStaticMesh :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CStaticMesh();
	~CStaticMesh();

public:
	bool Init();
	virtual bool CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg = D3D11_USAGE_IMMUTABLE,
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	virtual void Render(float fTime, int iContainer = 0, int iSubset = 0);
	virtual void RenderInstancing(void* pInstancedBuffer, int iCnt, int iSize, int iContainer = 0, int iSubset = 0);
	virtual void RenderParticle(int iCount, int iContainer = 0, int iSubset = 0);

	virtual bool LoadMesh(const std::string& strName,
		const TCHAR* pFileName, const std::string& strPath = MESH_PATH);
	virtual bool LoadMeshFullPath(const std::string& strName,
		const TCHAR* pFullPath);

protected:
	bool ConvertFBX(class CFBXLoader* pLoader, const char* pFullPath);
	virtual bool LoadMesh(const char* pFullPath);
	virtual bool SaveMesh(const char* pFullPath);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

