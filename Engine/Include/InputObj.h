#pragma once

#include "Input.h"

typedef struct _tagInputAxis
{
	std::function<void(float, float)>	pFunc;
}InputAxis, *PInputAxis;

typedef struct _tagInputAction
{
	std::function<void(float, float)>	pFunc[KT_END];
}InputAction, *PInputAction;

class CInputObj
{
	friend class CObj;

private:
	CInputObj();
	~CInputObj();

private:
	std::unordered_map<std::string, PInputAxis>		m_mapInputAxis;
	std::unordered_map<std::string, PInputAction>	m_mapInputAction;

public:
	template <typename T>
	void SetAxisFunc(const std::string& strTag, T* pObj, void(T::* pFunc)(float, float))
	{
		PInputAxis pAxis = FindInputAxis(strTag);

		if (!pAxis)
		{
			pAxis = new InputAxis;
			m_mapInputAxis.insert(std::make_pair(strTag, pAxis));			
		}

		pAxis->pFunc = std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2);

		GET_SINGLE(CInput)->AddAxisBind(strTag, this, &CInputObj::AxisFunc);
	}

	template <typename T>
	void SetActionFunc(const std::string& strTag, KEY_TYPE eType, T* pObj, void(T::* pFunc)(float, float))
	{
		PInputAction pAction = FindInputAction(strTag);

		if (!pAction)
		{
			pAction = new InputAction;
			m_mapInputAction.insert(std::make_pair(strTag, pAction));
		}

		pAction->pFunc[eType] = std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2);

		GET_SINGLE(CInput)->AddActionBind(strTag,eType, this, &CInputObj::ActionFunc);
	}

public:
	void AxisFunc(const std::string& strTag, float fScale, float fTime);
	void ActionFunc(const std::string& strTag, KEY_TYPE eType, float fPushTime, float fTime);

private:
	PInputAxis FindInputAxis(const std::string& strTag);
	PInputAction FindInputAction(const std::string& strTag);
};

