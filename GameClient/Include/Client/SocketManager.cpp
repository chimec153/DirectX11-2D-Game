#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SocketManager.h"
#include "Engine.h"

DEFINITION_SINGLE(CSocketManager)

SOCKADDR_IN	CSocketManager::m_tAddr = {};
char CSocketManager::m_pText[BUF_SIZE] = {};
char* CSocketManager::m_pRecvText[BUF_SIZE] = {};
int	CSocketManager::m_iText = 0;
bool CSocketManager::m_bExit = false;

CSocketManager::CSocketManager()
{

}

CSocketManager::~CSocketManager()
{
	m_bExit = true;
	closesocket(m_tClnt);
	WSACleanup();
}

bool CSocketManager::Init()
{
	WSAData tData = {};
	WSAStartup(MAKEWORD(2, 2), &tData);

	memset(&m_tAddr, 0, sizeof(m_tAddr));
	m_tAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_tAddr.sin_port = htons(atoi("9190"));
	m_tAddr.sin_family = AF_INET;

	m_tClnt = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(m_tClnt, (sockaddr*)&m_tAddr, sizeof(m_tAddr)) == SOCKET_ERROR)
	{
		return false;
	}

	uintptr_t hThread1 = _beginthreadex(NULL, 0, Send, (void*)&m_tClnt, 0, NULL);
	uintptr_t hThread2 = _beginthreadex(NULL, 0, Recv, (void*)&m_tClnt, 0, NULL);

	return true;
}

void CSocketManager::Update(float fTime)
{
}

unsigned __stdcall CSocketManager::Send(void* pData)
{
	SOCKET tClnt = *((SOCKET*)pData);

	while (!m_bExit)
	{
		std::cin >> m_pText;
		//GET_SINGLE(CImguiManager)->PreRender(0.f);
		//if (ImGui::Begin("Chat"))
		//{
			//ImGui::InputText("Text", m_pText, BUF_SIZE);

		if (strcmp(m_pText, ""))
		{
			char strText[256] = {};
			sprintf_s(strText, "[%s]: %s ",inet_ntoa(m_tAddr.sin_addr),m_pText);
			send(tClnt, strText, BUF_SIZE, 0);
		}
		//}
		//ImGui::End();
		//GET_SINGLE(CImguiManager)->Render(0.f);
	}

	return 0;
}

unsigned __stdcall CSocketManager::Recv(void* pData)
{
	SOCKET tClnt = *((SOCKET*)pData);

	//GET_SINGLE(CImguiManager)->PreRender(0.f);
	//if (ImGui::Begin("Chat"))
	//{
		//ImGui::InputText("Text", m_pText, BUF_SIZE);
	while (!m_bExit)
	{

		char strText[256] = {};
		if (recv(tClnt, strText, BUF_SIZE, 0) != 0)
		{
			std::cout << strText << std::endl;
			//++m_iText;

			/*if (m_iText > BUF_SIZE)
			{
				for (int i = 0; i < BUF_SIZE - 1; ++i)
				{
					m_pRecvText[i] = m_pRecvText[i + 1];
				}

				m_iText = BUF_SIZE;
			}*/
		}
	}

		//ImGui::BeginChild("Msg");

		//for (int i = 0; i < m_iText; ++i)
		//{
		//	ImGui::Text(m_pRecvText[i], i);
		//}

		//ImGui::EndChild();
	//}
	//ImGui::End();

	return 0;
}
