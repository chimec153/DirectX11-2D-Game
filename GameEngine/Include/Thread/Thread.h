#pragma once

#include "../Ref.h"

class CThread :
	public CRef
{
	friend class CThreadManager;

private:
	CThread();
	~CThread();

protected:
	bool	m_bLoop;

private:
	HANDLE	m_hStartEvent;
	HANDLE	m_hThread;

public:
	void Start();
	virtual void Run() = 0;
	void Wait();
	void Create();
	void Stop();

private:
	static unsigned WINAPI ThreadFunc(void* pData);
};

