#pragma once

#include "../GameEngine.h"

class CAnimation2DNotify
{
	friend class CAnimation2DSequence;

private:
	CAnimation2DNotify();
	~CAnimation2DNotify();

private:
	std::string		m_strTag;
	int				m_iFrame;
	float			m_fTime;
	std::vector<std::function<void(float)>>	m_vecCallBack;
	std::vector<std::function<void(int, float)>>	m_vecFrameFunc;
	bool			m_bCall;
	float			m_fAccTime;

public:
	void CreateNotify(const std::string& strTag, int iFrame);
	void CreateNotify(const std::string& strTag, float fTime);
	template <typename T>
	void AddFunc(T* pObj, void (T::* pFunc)(float))
	{
		m_vecCallBack.push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}
	template <typename T>
	void AddFunc(T* pObj, void (T::* pFunc)(int, float))
	{
		m_vecFrameFunc.push_back(std::bind(pFunc, pObj, std::placeholders::_1,std::placeholders::_2));
	}

	void AddFunc(void (*pFunc)(float));
	void AddFunc(void (*pFunc)(int, float));
	void Update(float fTime, int iFrame, float fDeltaTime);
	void Call(float fTime);
	void Clear();

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

};

