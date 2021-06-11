#pragma once
#include "TileMode.h"

class CStartGameMode :
	public CTileMode
{
	friend class CScene;

private:
	CStartGameMode();
	virtual ~CStartGameMode();

private:
	class CObj* m_pTitle;
	class CObj* m_pAn;
	class CObj* m_pGamePad;
	class CObj* m_pLogo;
	float		m_fTime;
	int			m_iStage;
	float		m_fFadeInLimit;
	float		m_fFadeOutLimit;
	bool		m_bFadeIn;
	class CSpriteComponent* m_pArrowL;
	class CSpriteComponent* m_pArrowR;
	int			m_iSelect;
	class CObj* m_pExitMenu;
	int			m_iDepth;
	int			m_iMenu;
	class CSpriteComponent* m_pArrowL2;
	class CSpriteComponent* m_pArrowR2;
	class CObj* m_pStartMenu;
	class CSpriteComponent* m_pArrowL3;
	class CSpriteComponent* m_pArrowR3;
	class CObj* m_pHelp;
	float		m_fStayTimeLimit;
	class CSound* m_pBGM;
	class CSound* m_pEffect;
	static bool	m_bDelete;
	static bool m_bStay;

public:
	bool Init();
	void Update(float fTime);

public:
	bool SetUI();
	bool LoadResources();
	void MainGameMode(float);
	void ExitGame(float);

public:
	bool LoadSequance(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	bool LoadFrame(const char* pFileName, const std::string& strPathKey = DATA_PATH);

public:
	void DownX(float fTime);
	void DownC(float fTime);
	void DownUpArrow(float fTime);
	void DownDownArrow(float fTime);

public:
	void Option();
	void ExitMenu();
	void StartMenu();

public:
	void UpdateSlot();
};

