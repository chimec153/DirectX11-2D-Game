#pragma once

#include "TileMode.h"

class CMainGameMode :
	public CTileMode
{
public:
	CMainGameMode();
	~CMainGameMode();

private:
	bool	m_bOpen;
	class CSound* m_pEft;

public:
	bool Init();
	void Update(float fTime);

public:
	void LoadResource();
	void LoadSound();
	void LoadParticle();
	void StartScene(float);
	void CreateUI();

public:
	bool LoadXml(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	void searchXMLData(class TiXmlElement* pElem);
	bool LoadSequance(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	bool LoadFrame(const char* pFileName, const std::string& strPathKey = DATA_PATH);

public:
	void Open(float);

public:
	void Col(class CCollider* pSrc, class CCollider* pDst, float fTime);
};

void SetImgui(float);
