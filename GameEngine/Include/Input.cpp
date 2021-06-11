#include "Input.h"
#include "UI/MouseObj.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "Component/ColliderRay.h"
#include "Engine.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_pInput(nullptr),
	m_pKeyBoard(nullptr),
	m_pMouse(nullptr),
	m_eType(KEY_INPUT_TYPE::KIT_DINPUT),
	m_vMousePos(),
	m_vMouseMove(),
	m_hWnd(0),
	m_hInst(0),
	m_pMouseObj(nullptr),
	m_bShowCS(true),
	m_vWorldMousePos()
{
	memset(m_cState, 0, sizeof(char) * 256);
	memset(m_pMouseDown, 0, (int)MOUSE_BUTTON_TYPE::END);
	memset(m_pMouseUp, 0, (int)MOUSE_BUTTON_TYPE::END);
	memset(m_pMousePress, 0, (int)MOUSE_BUTTON_TYPE::END);
}

CInput::~CInput()
{
	SAFE_DELETE_VECLIST(m_KeyList);
	SAFE_DELETE_MAP(m_mapBindAction);
	SAFE_DELETE_MAP(m_mapBindAxis);

	if (m_pKeyBoard)
		m_pKeyBoard->Unacquire();

	if (m_pMouse)
		m_pMouse->Unacquire();

	SAFE_RELEASE(m_pKeyBoard);
	SAFE_RELEASE(m_pMouse);
	SAFE_RELEASE(m_pMouseObj);
	SAFE_RELEASE(m_pInput);
}

const Vector3& CInput::GetWorldMousePos() const
{
	return m_vWorldMousePos;
}

bool CInput::IsMouseDown(MOUSE_BUTTON_TYPE eType) const
{
	return m_pMouseDown[(int)eType];
}

bool CInput::IsMouseUp(MOUSE_BUTTON_TYPE eType) const
{
	return m_pMouseUp[(int)eType];
}

bool CInput::IsMousePress(MOUSE_BUTTON_TYPE eType) const
{
	return m_pMousePress[(int)eType];
}

DIMOUSESTATE CInput::GetMouseState() const
{
	return m_tState;
}

const Vector2& CInput::GetMousePos() const
{
	return m_vMousePos;
}

const Vector2& CInput::GetMouseMove() const
{
	return m_vMouseMove;
}

CMouseObj* CInput::GetMouse() const
{
	return m_pMouseObj;
}

void CInput::DeleteActionCallBack(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAction>::iterator iter = m_mapBindAction.find(strTag);

	if (iter == m_mapBindAction.end())
		return;

	iter->second->vecDFunc->clear();
}

void CInput::DeleteAxisCallBack(const std::string& strTag)
{
}

bool CInput::IsKeyPressed(unsigned char cKey)
{
	return m_cState[cKey] != 0;
}

bool CInput::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	POINT pt;

	GetCursorPos(&pt);

	ScreenToClient(m_hWnd, &pt);

	m_vMousePos = pt;

	m_eType = KEY_INPUT_TYPE::KIT_DINPUT;

	HRESULT lResult = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& m_pInput, NULL);

	if (FAILED(lResult))
		m_eType = KEY_INPUT_TYPE::KIT_WIN;

	switch (m_eType)
	{
	case KEY_INPUT_TYPE::KIT_WIN:
		return InitWin();
	case KEY_INPUT_TYPE::KIT_DINPUT:
		return InitDInput();
	}

	return false;
}

bool CInput::InitWin()
{
	return true;
}

bool CInput::InitDInput()
{
	//	키보드 장치 초기화
	if (FAILED(m_pInput->CreateDevice(GUID(GUID_SysKeyboard), &m_pKeyBoard, NULL)))
		return false;

	//	키보드 포맷 설정
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	//	마우스 장치 초기화
	if (FAILED(m_pInput->CreateDevice(GUID(GUID_SysMouse), &m_pMouse, NULL)))
		return false;

	//	마우스 포맷 설정
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	return true;
}

void CInput::Update(float fTime)
{
	switch (m_eType)
	{
	case KEY_INPUT_TYPE::KIT_WIN:
		UpdateWin(fTime);
		break;
	case KEY_INPUT_TYPE::KIT_DINPUT:
		ReadKeyBoard(fTime);
		ReadMouse(fTime);
		UpdateDInput(fTime);
		break;
	}

	UpdateAxis(fTime);
	UpdateAction(fTime);
	UpdateMouse(fTime);
}

void CInput::UpdateWin(float fTime)
{
	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (GetAsyncKeyState((*iter)->cKey) & 0x8000)
		{
			(*iter)->bPush = true;
			(*iter)->fPushTime += fTime;
		}

		else
		{
			(*iter)->bPush = false;
			(*iter)->fPushTime = 0.f;
		}
	}
}

void CInput::UpdateDInput(float fTime)
{
	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (m_cState[(*iter)->cKey] & 0x80)
		{
			(*iter)->bPush = true;
			(*iter)->fPushTime += fTime;
		}

		else
		{
			(*iter)->bPush = false;
			(*iter)->fPushTime = 0.f;
		}
	}
}

void CInput::UpdateAxis(float fTime)
{
	std::unordered_map < std::string, PBindAxis>::iterator iter = m_mapBindAxis.begin();
	std::unordered_map < std::string, PBindAxis>::iterator iterEnd = m_mapBindAxis.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->fScale = 0.f;

		size_t iSize = iter->second->vecKey.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if (iter->second->vecKey[i].pInfo->bPush)
				iter->second->fScale += iter->second->vecKey[i].fScale;
		}

		size_t iFuncSize = iter->second->vecFunc.size();

		for (size_t i = 0; i < iFuncSize; ++i)
			iter->second->vecFunc[i].pFunc(iter->first, iter->second->fScale, fTime);
	}
}

void CInput::UpdateAction(float fTime)
{
	bool bUnion[(int)KEY_UNION::KU_END] = {};
	char cKey[(int)KEY_UNION::KU_END] = {VK_CONTROL, VK_MENU, VK_SHIFT};

	switch (m_eType)
	{
	case KEY_INPUT_TYPE::KIT_WIN:

		for (int i = 0; i < (int)KEY_UNION::KU_END; ++i)
		{
			if (GetAsyncKeyState(cKey[i]) & 0x8000)
				bUnion[i] = true;
		}

		break;
	case KEY_INPUT_TYPE::KIT_DINPUT:

		cKey[(int)KEY_UNION::KU_CTRL] = DIK_LCONTROL;
		cKey[(int)KEY_UNION::KU_ALT] = DIK_LALT;
		cKey[(int)KEY_UNION::KU_SHIFT] = DIK_LSHIFT;
/*
		for (int i = 0; i < KU_END; ++i)
		{
			if (m_cState[cKey[i]] & 0x80)
				bUnion[i] = true;
		}
*/
		break;
	}

	std::unordered_map < std::string, PBindAction>::iterator iter = m_mapBindAction.begin();
	std::unordered_map < std::string, PBindAction>::iterator iterEnd = m_mapBindAction.end();

	for (; iter != iterEnd; ++iter)
	{
		size_t iSize = iter->second->vecKey.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			bool bEnable = true;

			for (int j = 0; j < (int)KEY_UNION::KU_END; ++j)
			{
				if (iter->second->vecKey[i].bUnion[j] != bUnion[j])
				{
					bEnable = false;
					break;
				}
			}

			if (!bEnable)
				continue;

			if (iter->second->vecKey[i].pInfo->bPush)
			{
				if (!iter->second->vecKey[i].bPush)
				{
					iter->second->vecKey[i].bPush = true;

					size_t iFuncSize = iter->second->vecFunc[(int)KEY_TYPE::KT_DOWN].size();

					for (size_t j = 0; j < iFuncSize; ++j)
						iter->second->vecFunc[(int)KEY_TYPE::KT_DOWN][j].pFunc(iter->first, KEY_TYPE::KT_DOWN,
							iter->second->vecKey[i].pInfo->fPushTime, fTime);

					iFuncSize = iter->second->vecDFunc[(int)KEY_TYPE::KT_DOWN].size();

					for (size_t j = 0; j < iFuncSize; ++j)
					{
						iter->second->vecDFunc[(int)KEY_TYPE::KT_DOWN][j](fTime);
					}
				}

				else
				{
					size_t iFuncSize = iter->second->vecFunc[(int)KEY_TYPE::KT_PRESS].size();

					for (size_t j = 0; j < iFuncSize; ++j)
						iter->second->vecFunc[(int)KEY_TYPE::KT_PRESS][j].pFunc(iter->first, KEY_TYPE::KT_PRESS,
							iter->second->vecKey[i].pInfo->fPushTime, fTime);

					iFuncSize = iter->second->vecDFunc[(int)KEY_TYPE::KT_PRESS].size();

					for (size_t j = 0; j < iFuncSize; ++j)
					{
						iter->second->vecDFunc[(int)KEY_TYPE::KT_PRESS][j](fTime);
					}
				}
			}

			else if (iter->second->vecKey[i].bPush)
			{
				iter->second->vecKey[i].bPush = false;

				size_t iFuncSize = iter->second->vecFunc[(int)KEY_TYPE::KT_UP].size();

				for (size_t j = 0; j < iFuncSize; ++j)
					iter->second->vecFunc[(int)KEY_TYPE::KT_UP][j].pFunc(iter->first, KEY_TYPE::KT_UP,
						iter->second->vecKey[i].pInfo->fPushTime, fTime);

				iFuncSize = iter->second->vecDFunc[(int)KEY_TYPE::KT_UP].size();

				for (size_t j = 0; j < iFuncSize; ++j)
				{
					iter->second->vecDFunc[(int)KEY_TYPE::KT_UP][j](fTime);
				}

				iter->second->vecKey[i].pInfo->fPushTime = 0.f;
			}
		}
	}
}

void CInput::UpdateMouse(float fTime)
{
	POINT pt;

	GetCursorPos(&pt);

	ScreenToClient(m_hWnd, &pt);

	RECT tRect = {};

	GetClientRect(m_hWnd, &tRect);

	Resolution tRS = RESOLUTION;

	Vector2 vRatio = RATIO;

	bool bImgui = GET_SINGLE(CEngine)->IsImgui();

	if (pt.x < 0 || pt.y < 0 ||
		pt.x > tRect.right - tRect.left || pt.y > tRect.bottom - tRect.top)
	{
		if (!m_bShowCS)
		{
			m_bShowCS = true;
			ShowCursor(TRUE);
		}
	}

	else
	{
		if (m_bShowCS)
		{
			if (!bImgui)
			{
				m_bShowCS = false;
				ShowCursor(FALSE);
			}
		}

		else if(bImgui)
		{
			m_bShowCS = true;
			ShowCursor(TRUE);
		}
	}

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	RectInfo tInfo = pCam->GetRect();

	Vector2 vPos = pt;

	vPos *= GET_SINGLE(CDevice)->GetRatio();

	vPos.x *= (tInfo.fR - tInfo.fL);
	vPos.y *= (tInfo.fT - tInfo.fB);

	vPos.y = (float)tRS.iHeight - vPos.y;

	m_vMouseMove = vPos - m_vMousePos;

	m_vMousePos = vPos;

	Vector3 vCamPos = pCam->GetWorldPos();

	Vector3 vPivot = pCam->GetPivot();

	SAFE_RELEASE(pCam);

	m_vWorldMousePos = Vector3(m_vMousePos.x, m_vMousePos.y, 0.f) + Vector3(vCamPos.x, vCamPos.y, 0.f) - Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * vPivot;

	m_pMouseObj->SetRelativePos(m_vMousePos.x, m_vMousePos.y, 0.f);

	m_pMouseObj->Update(fTime);
	m_pMouseObj->PostUpdate(fTime);
}

void CInput::ReadKeyBoard(float fTime)
{
	HRESULT lResult = m_pKeyBoard->GetDeviceState(256, m_cState);

	if (FAILED(lResult))
	{
		if (lResult == DIERR_INPUTLOST || lResult == DIERR_NOTACQUIRED)
			m_pKeyBoard->Acquire();
	}
}

void CInput::ReadMouse(float fTime)
{
	DIMOUSESTATE tState;

	HRESULT lResult = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &tState);

	if (FAILED(lResult))
	{
		if (lResult == DIERR_INPUTLOST || lResult == DIERR_NOTACQUIRED)
			m_pMouse->Acquire();
	}

	m_tState = tState;

	m_vMouseMove.x = (float)m_tState.lX;
	m_vMouseMove.y = (float)m_tState.lY;

	m_cState[MOUSE_LEFT] = 0;
	m_cState[MOUSE_RIGHT] = 0;

	for (int i = 0; i < (int)MOUSE_BUTTON_TYPE::END; ++i)
	{
		switch (i)
		{
		case (int)MOUSE_BUTTON_TYPE::LEFT:
			m_cState[MOUSE_LEFT] = 0xff;
			break;
		case (int)MOUSE_BUTTON_TYPE::RIGHT:
			m_cState[MOUSE_RIGHT] = 0xff;
			break;
		}

		if (m_tState.rgbButtons[i] & 0x80)
		{
			if (!m_pMouseDown[i] &&
				!m_pMousePress[i])
			{
				m_pMouseUp[i] = false;
				m_pMouseDown[i] = true;
			}

			else if (!m_pMousePress[i])
			{
				m_pMouseDown[i] = false;
				m_pMousePress[i] = true;
			}
		}

		else
		{
			if (m_pMouseDown[i] ||
				m_pMousePress[i])
			{
				m_pMouseUp[i] = true;
				m_pMouseDown[i] = false;
				m_pMousePress[i] = false;
			}

			else if(m_pMouseUp[i])
			{
				m_pMouseUp[i] = false;
			}				
		}
	}
}

void CInput::CreateMouse()
{
	SAFE_RELEASE(m_pMouseObj)

	m_pMouseObj = new CMouseObj;

	//m_pMouseObj->m_pScene = GET_SINGLE(CSceneManager)->GetScene();

	if (!m_pMouseObj->Init())
	{
		SAFE_RELEASE(m_pMouseObj);
		return;
	}

	m_pMouseObj->SetRelativePos(m_vMousePos.x, m_vMousePos.y, 0.f);

	CColliderRay* pCom = m_pMouseObj->FindComByType<CColliderRay>();

	pCom->AddRenderState("DepthNoWrite");
	pCom->AddRenderState("NoCullBack");

	SAFE_RELEASE(pCom);
/*
	ShowCursor(FALSE);

	m_bShowCS = false;*/
}

void CInput::Collision(float fTime)
{
	m_pMouseObj->Collision(fTime);
}

void CInput::PreRender(float fTime)
{
	//m_pMouseObj->PreRender(fTime);
}

void CInput::Render()
{
	m_pMouseObj->Render(0.f);

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		if (ImGui::Begin("Mouse"))
		{
			Resolution tRS = RESOLUTION;

			ImGui::SliderFloat3("RelativePos", &m_vMousePos.x, 0.f, (float)tRS.iWidth);
			ImGui::SliderFloat3("WorldPos", &m_vWorldMousePos.x, -5000.f, 5000.f);
		}
		ImGui::End();
	}
}

void CInput::AddActionKey(const std::string& strTag, unsigned char cKey)
{
	PBindAction pAction = FindAction(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	PKeyInfo pInfo = nullptr;

	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = (*iter);
			pInfo->iRef++;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		pInfo->iRef = 1;
		m_KeyList.push_back(pInfo);
	}

	size_t iSize = pAction->vecKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAction->vecKey[i].pInfo->cKey == cKey)
			return;
	}

	ActionKey tKey = {};
	tKey.pInfo = pInfo;
	pAction->vecKey.push_back(tKey);
}

void CInput::AddActionKeyUnion(const std::string& strTag, unsigned char cKey, KEY_UNION eUnion)
{
	PBindAction pAction = FindAction(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	PKeyInfo pInfo = nullptr;

	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = (*iter);
			pInfo->iRef++;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		pInfo->iRef = 1;
		m_KeyList.push_back(pInfo);
	}

	if (pAction->vecKey.empty())
	{
		ActionKey tKey = {};
		tKey.pInfo = pInfo;
		pAction->vecKey.push_back(tKey);
	}

	size_t iSize = pAction->vecKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAction->vecKey[i].pInfo->cKey == cKey)
			pAction->vecKey[i].bUnion[(int)eUnion] = true;
	}
}

void CInput::AddActionBind(const std::string& strTag, KEY_TYPE eType, void(*pFunc)(const std::string&,KEY_TYPE, float))
{
	PBindAction pAction = FindAction(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	ActionFunc tFunc;

	tFunc.pFunc = std::bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	tFunc.pObj = nullptr;

	pAction->vecFunc[(int)eType].push_back(tFunc);
}

void CInput::AddActionBind(const std::string& strTag, KEY_TYPE eType, void(*pFunc)(float))
{
	PBindAction pAction = FindAction(strTag);

	if (!pAction)
	{
		pAction = new BindAction;
		pAction->strTag = strTag;
		m_mapBindAction.insert(std::make_pair(strTag, pAction));
	}

	pAction->vecDFunc[(int)eType].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CInput::DeleteActionKey(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAction>::iterator iter = m_mapBindAction.find(strTag);

	if (iter == m_mapBindAction.end())
		return;

	size_t iSize = iter->second->vecKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (--iter->second->vecKey[i].pInfo->iRef == 0)
		{
			std::list<PKeyInfo>::iterator iterKey = m_KeyList.begin();
			std::list<PKeyInfo>::iterator iterKeyEnd = m_KeyList.end();

			for (; iterKey != iterKeyEnd; ++iterKey)
			{
				if ((*iterKey)->cKey == iter->second->vecKey[i].pInfo->cKey)
				{
					SAFE_DELETE((*iterKey));
					m_KeyList.erase(iterKey);
					break;
				}
			}
		}
	}

	SAFE_DELETE(iter->second);

	m_mapBindAction.erase(iter->first);
}

void CInput::DeleteActionKey(const std::string& strTag, CInputObj* pInput)
{
	PBindAction pAction = FindAction(strTag);

	if (!pAction)
		return;

	for (int j = 0; j< (int)KEY_TYPE::KT_END; ++j)
	{
		size_t iSize = pAction->vecFunc[j].size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if (pAction->vecFunc[j][i].pObj == pInput)
			{
				std::vector<ActionFunc>::iterator iter = pAction->vecFunc[j].begin() + i;

				pAction->vecFunc[j].erase(iter);

				break;
			}
		}
	}
}

void CInput::AddAxisKey(const std::string& strTag, unsigned char cKey, float fScale)
{
	PBindAxis pAxis = FindAxis(strTag);

	if (!pAxis)
	{
		pAxis = new BindAxis;
		m_mapBindAxis.insert(std::make_pair(strTag, pAxis));
		pAxis->strTag = strTag;
	}

	PKeyInfo pInfo = nullptr;

	std::list<PKeyInfo>::iterator iter = m_KeyList.begin();
	std::list<PKeyInfo>::iterator iterEnd = m_KeyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->cKey == cKey)
		{
			pInfo = (*iter);
			pInfo->iRef++;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new KeyInfo;
		pInfo->cKey = cKey;
		pInfo->iRef = 1;
		m_KeyList.push_back(pInfo);
	}

	size_t iSize = pAxis->vecKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAxis->vecKey[i].pInfo->cKey == cKey)
			return;
	}

	AxisKey tKey = {};
	tKey.pInfo = pInfo;
	tKey.fScale = fScale;
	pAxis->vecKey.push_back(tKey);
}

void CInput::AddAxisBind(const std::string& strTag, void(*pFunc)(const std::string&,float,float))
{
	PBindAxis pAxis = FindAxis(strTag);

	if (!pAxis)
	{
		pAxis = new BindAxis;
		m_mapBindAxis.insert(std::make_pair(strTag, pAxis));
		pAxis->strTag = strTag;
	}

	AxisFunc tFunc = {};

	tFunc.pFunc = std::bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	pAxis->vecFunc.push_back(tFunc);
}

void CInput::DeleteAxisKey(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAxis>::iterator iter = m_mapBindAxis.find(strTag);

	if (iter == m_mapBindAxis.end())
		return;

	size_t iSize = iter->second->vecKey.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (--iter->second->vecKey[i].pInfo->iRef == 0)
		{
			std::list<PKeyInfo>::iterator iterKey = m_KeyList.begin();
			std::list<PKeyInfo>::iterator iterKeyEnd = m_KeyList.end();

			for (; iterKey != iterKeyEnd; ++iterKey)
			{
				if ((*iterKey)->cKey == iter->second->vecKey[i].pInfo->cKey)
				{
					SAFE_DELETE((*iterKey));
					m_KeyList.erase(iterKey);
					break;
				}
			}
		}
	}

	SAFE_DELETE(iter->second);

	m_mapBindAxis.erase(iter->first);
}

void CInput::DeleteAxisKey(const std::string& strTag, CInputObj* pInput)
{
	PBindAxis pAxis = FindAxis(strTag);

	if (!pAxis)
		return;

	size_t iSize = pAxis->vecFunc.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pAxis->vecFunc[i].pObj == pInput)
		{
			std::vector<AxisFunc>::iterator iter = pAxis->vecFunc.begin() + i;

			pAxis->vecFunc.erase(iter);

			break;
		}
	}
}

PBindAction CInput::FindAction(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAction>::iterator iter = m_mapBindAction.find(strTag);

	if (iter == m_mapBindAction.end())
		return nullptr;

	return iter->second;
}

PBindAxis CInput::FindAxis(const std::string& strTag)
{
	std::unordered_map<std::string, PBindAxis>::iterator iter = m_mapBindAxis.find(strTag);

	if (iter == m_mapBindAxis.end())
		return nullptr;

	return iter->second;
}
