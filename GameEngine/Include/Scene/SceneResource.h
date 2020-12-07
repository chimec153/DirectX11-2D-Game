#pragma once

#include "../GameEngine.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	std::unordered_map<std::string, class CMesh*>					m_mapMesh;
	std::unordered_map<std::string, class CShader*>					m_mapShader;
	std::unordered_map<std::string, class CMaterial*>				m_mapMaterial;
	std::unordered_map<std::string, class CTexture*>				m_mapTexture;
	std::unordered_map<std::string, class CAnimation2DSequence*>	m_mapAni2DSeq;

public:
	bool Init();

	class CMesh* FindMesh(const std::string& strName);

	class CShader* FindShader(const std::string& strName);

public:
	class CMaterial* FindMaterial(const std::string& strName);

public:
	class CTexture* FindTexture(const std::string& strTag);
	bool LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFileName);
	bool LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strPathName = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName);

public:
	bool CreateAni2DSeq(const std::string& strTag, class CTexture* pTex, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool LoadAni2DSeq(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath = RESOURCE_PATH, float fMaxTime = 1.f, float fPlayRate = 1.f);
	void AddFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd);
	void AddNotify(const std::string& strTag, const std::string& strNot, int iFrame);
	class CAnimation2DSequence* FindAni2DSeq(const std::string& strTag);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

