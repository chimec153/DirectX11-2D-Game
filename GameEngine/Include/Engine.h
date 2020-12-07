#pragma once

#include "GameEngine.h"
#include "CreateInstance.h"

class CEngine
{
private:
	HDC						m_hDC;
	HWND					m_hWnd;
	HINSTANCE				m_hInst;
	Resolution				m_tRS;
	static bool				m_bLoop;
	class CCreateInstance*	m_pCInst;
	class CUIFont*			m_pFont;
	class CObj*				m_pFontObj;

private:
	std::function<void(float)>		m_EditorUpdate;

public:
	template <typename T>
	void SetEditorUpdate(T* pObj, void(T::* pFunc)(float))
	{
		m_EditorUpdate = std::bind(pFunc, pObj, std::placeholders::_1);
	}

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
	int PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

private:
	int Create(const TCHAR* pClass, const TCHAR* pTitle, int iWidth, int iHeight);
	int Register(const TCHAR* pClass);

public:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

public:
	template <typename T>
	void SetInstance()
	{
		SAFE_DELETE(m_pCInst);

		m_pCInst = new T;

		if (!m_pCInst->Init())
		{
			SAFE_DELETE(m_pCInst);
			return;
		}
	}

	DECLARE_SINGLE(CEngine)
};

