#pragma once

#include "../GameEngine.h"

class CResourceManager
{
private:
	class CMesh2D*			m_pDefaultMesh;
	class CMesh2D*			m_pDefaultUIMesh;
	class CTexture*			m_pNoiseTex;
	std::unordered_map<std::string, class CMesh*>					m_mapMesh;
	std::unordered_map<std::string, class CMaterial*>				m_mapMaterial;
	std::unordered_map<std::string, class CTexture*>				m_mapTexture;
	std::unordered_map<std::string, class CAnimation2DSequence*>	m_mapAni2DSeq;
	std::unordered_map<std::string, class CParticleSystem*>			m_mapParticle;
	std::unordered_map<std::string, class CSkeleton*>				m_mapSkeleton;
	std::unordered_map<std::string, class CAnimationSequence*>		m_mapSequence;
	std::unordered_map<std::string, ID3D11SamplerState*>		m_mapSamplerStates;
	ID3D11SamplerState*		m_pPoint;
	ID3D11SamplerState*		m_pLinear;
	ID3D11SamplerState*		m_pAnisotropic;
	
public:
	class CMesh2D* GetDefaultMesh()	const;
	class CMesh2D* GetUIMesh()	const;
	class CTexture* GetNoiseTex()	const;

public:
	bool Init();
	bool CreateMesh(MESH_TYPE eType, const std::string& strName, D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, D3D11_USAGE eVtxUsg, 
		void* pIdxData = nullptr, int iIdxSz = 0, int iIdxCnt = 0, D3D11_USAGE eIdxUsg = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_UNKNOWN);
	class CMesh* FindMesh(const std::string& strName);
	void ReleaseMesh(const std::string& strName);
	bool LoadMesh(MESH_TYPE eType, const std::string& strKey, const TCHAR* pFilePath, const std::string& strPathKey = MESH_PATH);
	void CreateSphere(const std::string& strKey, float fRadius, UINT iSliceCount, UINT iStackCount);

public:
	class CMaterial* CreateMaterial(const std::string& strName);
	class CMaterial* FindMaterial(const std::string& strName);
	void ReleaseMaterial(const std::string& strName);

public:
	bool LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath);
	bool LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath = TEXTURE_PATH);
	bool LoadTextureArrayFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath);
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
	void AddNotify(const std::string& strTag, const std::string& strNot, float fTime);
	class CAnimation2DSequence* FindAni2DSeq(const std::string& strTag);
	void ReleaseAni2DSeq(const std::string& strTag);

public:
	void LoadSequence(const std::string& strKey, const char* pFilePath, const std::string& strPathKey = MESH_PATH);
	void LoadSequence(const std::string& strKey, bool bLoop, struct _tagFbxAnimationClip* clip);
	class CAnimationSequence* FindSequence(const std::string& strKey);
	bool CreateSequence(const std::string& strKey, bool bLoop, int iStartFrame, int iEndFrame, float fPlayTime, const std::vector<struct _tagBoneKeyFrame*>& vecFrame);
private:
	ID3D11SamplerState* CreateSample(D3D11_FILTER eFilter);
	void CreateSamplerState(const std::string& strKey,
		D3D11_TEXTURE_ADDRESS_MODE eAddressU,
		D3D11_TEXTURE_ADDRESS_MODE eAddressV,
		D3D11_TEXTURE_ADDRESS_MODE eAddressW,
		FLOAT pBorderColor[4],
		D3D11_COMPARISON_FUNC eCompareFunc,
		FLOAT fMaxLod, FLOAT fMinLod,
		D3D11_FILTER eFilter);
	ID3D11SamplerState* FindSampler(const std::string& strKey);
	void SetSamplerState(const std::string& strKey, int iRegister,
		SHADER_CBUFFER_TYPE eType = SHADER_CBUFFER_TYPE::CBUFFER_ALL);

public:
	class CParticleSystem* CreateParticle(const std::string& strTag, bool bDynamic, 
		int iCount, int iSize, int iRegister, int iType);
	class CParticleSystem* FindParticle(const std::string& strTag);

public:
	class CSkeleton* FindSkeleton(const std::string& strKey);

	void SetSkeleton(const std::string& strKey, class CSkeleton* pSkel);
	bool LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey = MESH_PATH);

	DECLARE_SINGLE(CResourceManager)
	
};