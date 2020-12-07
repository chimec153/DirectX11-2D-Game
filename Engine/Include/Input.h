#pragma once

#include "GameEngine.h"

#define MOUSE_LEFT	0xfd
#define MOUSE_RIGHT	0xfe
#define MOUSE_WHEEL	0xff

enum KEY_TYPE
{
	KT_DOWN,
	KT_PRESS,
	KT_UP,
	KT_END
};

enum KEY_UNION
{
	KU_CTRL,
	KU_ALT,
	KU_SHIFT,
	KU_END
};

enum class KEY_INPUT_TYPE
{
	KIT_WIN,
	KIT_DINPUT
};

enum class MOUSE_BUTTON_TYPE
{
	LEFT,
	RIGHT,
	WHEEL,
	END
};

enum class MOUSE_DIR
{
	X,
	Y,
	Z,
	END
};

typedef struct _tagKeyInfo
{
	char	cKey;
	bool	bPush;
	float	fPushTime;
	int		iRef;

	_tagKeyInfo()
	{
		cKey = 0;
		bPush = false;
		fPushTime = 0.f;
		iRef = 0;
	}
}KeyInfo, *PKeyInfo;

typedef struct _tagActionKey
{
	PKeyInfo	pInfo;
	bool		bUnion[KU_END];
	bool		bPush;
	float		fPushTime;

	_tagActionKey()
	{
		pInfo = nullptr;
		memset(bUnion, 0, sizeof(bool) * KU_END);
		bPush = false;
	}
}ActionKey, *PActionKey;

typedef struct _tagAxisFunc
{
	std::function<void(const std::string&, float, float)>	pFunc;
	class CInputObj*										pObj;
}AxisFunc, *PAxisFunc;

typedef struct _tagActionFunc
{
	std::function<void(const std::string&,KEY_TYPE, float, float)>	pFunc;
	class CInputObj*								pObj;
}ActionFunc, *PActionFunc;

typedef struct _tagBindAction
{
	std::string								strTag;
	std::vector<ActionKey>					vecKey;
	std::vector<ActionFunc>					vecFunc[KT_END];
}BindAction, *PBindAction;

typedef struct _tagAxisKey
{
	PKeyInfo	pInfo;
	float		fScale;

	_tagAxisKey()
	{
		pInfo = nullptr;
		fScale = 0.f;
	}
}AxisKey,*PAxisKey;

typedef struct _tagBindAxis
{
	std::string										strTag;
	std::vector<AxisKey>							vecKey;
	std::vector<AxisFunc>							vecFunc;
	float											fScale;

	_tagBindAxis()
	{
		fScale = 0.f;
	}
}BindAxis, *PBindAxis;

class CInput
{
private:
	std::list<PKeyInfo>								m_KeyList;
	std::unordered_map<std::string, PBindAction>	m_mapBindAction;
	std::unordered_map<std::string, PBindAxis>		m_mapBindAxis;
	IDirectInput8*									m_pInput;
	IDirectInputDevice8*							m_pKeyBoard;
	IDirectInputDevice8*							m_pMouse;
	KEY_INPUT_TYPE									m_eType;
	Vector2											m_vMousePos;
	Vector2											m_vMouseMove;
	HWND											m_hWnd;
	HINSTANCE										m_hInst;
	unsigned char									m_cState[256];
	class CMouseObj*								m_pMouseObj;
	bool											m_bShowCS;
	Vector2											m_vWorldMousePos;
	bool											m_pMouseDown[(int)MOUSE_BUTTON_TYPE::END];
	bool											m_pMouseUp[(int)MOUSE_BUTTON_TYPE::END];
	bool											m_pMousePress[(int)MOUSE_BUTTON_TYPE::END];
	DIMOUSESTATE									m_tState;

public:
	const Vector2& GetWorldMousePos()	const;
	bool IsMouseDown(MOUSE_BUTTON_TYPE eType)	const;
	bool IsMouseUp(MOUSE_BUTTON_TYPE eType)	const;
	bool IsMousePress(MOUSE_BUTTON_TYPE eType)	const;
	DIMOUSESTATE GetMouseState()	const;
	const Vector2& GetMousePos()	const;
	const Vector2& GetMouseMove()	const;
	class CMouseObj* GetMouse()	const;

public:
	bool Init(HWND hWnd, HINSTANCE hInst);
	bool InitWin();
	bool InitDInput();
	void Update(float fTime);
	void UpdateWin(float fTime);
	void UpdateDInput(float fTime);
	void UpdateAxis(float fTime);
	void UpdateAction(float fTime);
	void UpdateMouse(float fTime);
	void ReadKeyBoard(float fTime);
	void ReadMouse(float fTime);
	void CreateMouse();
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render();

public:
	void AddActionKey(const std::string& strTag, char cKey);
	void AddActionKeyUnion(const std::string& strTag, char cKey, KEY_UNION eUnion);
	void AddActionBind(const std::string& strTag, KEY_TYPE eType, void(*pFunc)(const std::string&, KEY_TYPE,float));
	template <typename T>
	void AddActionBind(const std::string& strTag, KEY_TYPE eType, T* pObj, void(T::*pFunc)(const std::string&, KEY_TYPE, float, float))
	{
		PBindAction pAction = FindAction(strTag);

		if (!pAction)
		{
			pAction = new BindAction;
			m_mapBindAction.insert(std::make_pair(strTag, pAction));
			pAction->strTag = strTag;
		}

		ActionFunc tFunc = {};

		tFunc.pFunc = std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		tFunc.pObj = pObj;

		pAction->vecFunc[eType].push_back(tFunc);
	}
	void DeleteActionKey(const std::string& strTag);
	void DeleteActionKey(const std::string& strTag, class CInputObj* pInput);

public:
	void AddAxisKey(const std::string& strTag, char cKey, float fScale);
	void AddAxisBind(const std::string& strTag, void(*pFunc)(const std::string&, float, float));
	template <typename T>
	void AddAxisBind(const std::string& strTag, T* pObj, void(T::* pFunc)(const std::string&, float, float))
	{
		PBindAxis pAxis = FindAxis(strTag);

		if (!pAxis)
		{
			pAxis = new BindAxis;
			m_mapBindAxis.insert(std::make_pair(strTag, pAxis));
			pAxis->strTag = strTag;
		}

		AxisFunc tFunc;

		tFunc.pFunc = std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		tFunc.pObj = pObj;

		pAxis->vecFunc.push_back(tFunc);
	}
	void DeleteAxisKey(const std::string& strTag);
	void DeleteAxisKey(const std::string& strTag, class CInputObj* pInput);

public:
	PBindAction FindAction(const std::string& strTag);
	PBindAxis FindAxis(const std::string& strTag);

	DECLARE_SINGLE(CInput)
};

