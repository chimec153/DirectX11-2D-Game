#pragma once

#include "../GameEngine.h"

class CResourceManager
{
private:
	class CMesh2D*			m_pDefaultMesh;
	class CMesh2D*			m_pDefaultUIMesh;
	class CTexture*			m_pDefaultTex;
	std::unordered_map<std::string, class CMesh*>					m_mapMesh;
	std::unordered_map<std::string, class CMaterial*>				m_mapMaterial;
	std::unordered_map<std::string, class CTexture*>				m_mapTexture;
	std::unordered_map<std::string, class CAnimation2DSequence*>	m_mapAni2DSeq;
	ID3D11SamplerState*		m_pPoint;
	ID3D11SamplerState*		m_pLinear;
	ID3D11SamplerState*		m_pAnisotropic;

public:
	class CMesh2D* GetDefaultMesh()	const;
	class CMesh2D* GetUIMesh()	const;
	class CTexture* GetDTex()	const;

public:
	bool Init();
	bool CreateMesh(MESH_TYPE eType, const std::string& strName, D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, 
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	class CMesh* FindMesh(const std::string& strName);
	void ReleaseMesh(const std::string& strName);

public:
	class CMaterial* CreateMaterial(const std::string& strName);
	class CMaterial* FindMaterial(const std::string& strName);
	void ReleaseMaterial(const std::string& strName);

public:
	bool LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTexture(const std::string& strTag, const char* pFileName, const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath);
	bool LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath);
	void ReleaseTexture(const std::string& strTag);
	class CTexture* FindTexture(const std::string& strName);
	class CTexture* LoadTexture(FILE* pFile);

public:
	bool CreateAni2DSeq(const std::string& strTag, class CTexture* pTex, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool CreateAni2DSeq(const std::string& strTag, const std::string& strKey, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool LoadAni2DSeq(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath = RESOURCE_PATH, float fMaxTime = 1.f, float fPlayRate = 1.f);
	void SaveAni2DSeqFromFullPath(const std::string& strTag, const char* pFullPath);
	void AddFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd);
	void AddNotify(const std::string& strTag, const std::string& strNot, int iFrame);
	class CAnimation2DSequence* FindAni2DSeq(const std::string& strTag);
	void ReleaseAni2DSeq(const std::string& strTag);

private:
	ID3D11SamplerState* CreateSample(D3D11_FILTER eFilter);

	DECLARE_SINGLE(CResourceManager)
};