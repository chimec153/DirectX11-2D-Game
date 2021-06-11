#pragma once

#include "GameEngine.h"

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 256

class CSocketManager
{
private:
	SOCKET	m_tClnt;
	SOCKET	m_tServ;
	static SOCKADDR_IN	m_tAddr;
	static char m_pText[BUF_SIZE];
	static char* m_pRecvText[BUF_SIZE];
	static int	m_iText;
	static bool m_bExit;

public:
	SOCKET GetClientSock()	const
	{
		return m_tClnt;
	}
	SOCKET GetServerSock()	const
	{
		return m_tServ;
	}

public:
	bool Init();
	void Update(float fTime);

private:
	static unsigned WINAPI Send(void*);
	static unsigned WINAPI Recv(void*);

	DECLARE_SINGLE(CSocketManager)
};

