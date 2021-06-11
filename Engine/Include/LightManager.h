#pragma once

#include "GameEngine.h"

class CLightManager
{
private:
	std::unordered_map<std::string, class Light*> m_mapLight;

public:
	bool Init();
	void SetShader();
	class Light* FindLight(const std::string& strKey);
	void AddLight(class Light* pLight);

	DECLARE_SINGLE(CLightManager)
};

