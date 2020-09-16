#pragma once

#include "../GameEngine.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	std::unordered_map<std::string, class CMesh*>			m_mapMesh;
	std::unordered_map<std::string, class CShader*>			m_mapShader;
	std::unordered_map<std::string, class CMaterial*>		m_mapMaterial;

public:
	bool Init();

	class CMesh* FindMesh(const std::string& strName);

	class CShader* FindShader(const std::string& strName);

public:
	class CMaterial* FindMaterial(const std::string& strName);
};

