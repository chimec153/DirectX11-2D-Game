#pragma once

#include "../Ref.h"



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
	PINSTANCINGBUFFER		m_pInstancingBuffer;
	std::vector<PMESHCONTAINER>	m_vecContainer;
	MESH_TYPE					m_eType;

public:
	const Vector3& GetMax()	const;
	const Vector3& GetMin()	const;
	const std::vector<PMESHCONTAINER>* GetMeshContainer()	const;
	MESH_TYPE GetMeshType()	const;

public:
	bool Init();
	virtual bool CreateInstancingBuffer(int iCount, int iSize);
	virtual bool CreateMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg,
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	bool CreateParticleMesh(D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg,
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);

public:
	virtual void Render(float fTime, int iConatiner = 0, int iSubset = 0);
	virtual void RenderInstancing(void* pData, int iCount ,int iSize, int iConatiner = 0, int iSubset = 0);
	virtual void RenderParticle(int iCount, int iConatiner = 0, int iSubset = 0);
	virtual bool LoadMesh(const std::string& strName,
		const TCHAR* pFileName, const std::string& strPath = MESH_PATH);
	virtual bool LoadMeshFullPath(const std::string& strName,
		const TCHAR* pFullPath);

protected:
	bool ConvertFBX(class CFBXLoader* pLoader, const char* pFullPath);
	virtual bool LoadMesh(const char* pFullPath);
	virtual bool SaveMesh(const char* pFullPath);

public:
	virtual bool AddVertexBuffer(const void* pVertices, int iSize, int iCount, D3D11_USAGE eUsg, D3D11_PRIMITIVE_TOPOLOGY eTop, PMESHCONTAINER pContainer);
	virtual bool AddIndexBuffer(const void* pIndices, int iSize, int iCount, D3D11_USAGE, DXGI_FORMAT eFmt, PMESHCONTAINER pContainer);
	void UpdateVertexBuffer(const void* pData, int iContainer = 0);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

