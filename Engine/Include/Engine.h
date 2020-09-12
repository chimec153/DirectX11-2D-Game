#pragma once

#include "GameEngine.h"

class CEngine
{
private:
	HDC			m_hDC;
	HWND		m_hWnd;
	HINSTANCE	m_hInst;
	Resolution	m_tRS;
	static bool	m_bLoop;

public:
	bool Init(const TCHAR* pClass, const TCHAR* pTitle, HINSTANCE hInst,
		int iWidth, int iHeight, bool bWindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, const TCHAR* pClass,
		int iWidth, int iHeight, bool bWindowMode = true);

public:
	int Run();
	void Logic();

public:
	int Input(float fTime);
	int Update(float fTime);
	int Collision(float fTime);
	int Render(float fTime);

private:
	int Create(const TCHAR* pClass, const TCHAR* pTitle, int iWidth, int iHeight);
	int Register(const TCHAR* pClass);

public:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine)
};

