#pragma once

#include "GameEngine.h"

class CViewManager
{
private:
	class CWorldDlg*	m_pWorldDlg;
	class CDetailDlg*	m_pDetailDlg;

public:
	class CWorldDlg* GetWorldDlg()	const
	{
		return m_pWorldDlg;
	}

	class CDetailDlg* GetDetailDlg()	const
	{
		return m_pDetailDlg;
	}

public:
	bool Init(class CWorldDlg* pWorld, class CDetailDlg* pDetail);

	DECLARE_SINGLE(CViewManager)
};

