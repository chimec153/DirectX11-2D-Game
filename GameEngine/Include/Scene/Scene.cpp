#include "Scene.h"
#include "../Object/Obj.h"
#include "SceneResource.h"

CScene::CScene()	:
	m_bStart(false)
{
	m_pGameMode = new CGameMode;
	m_pResource = new CSceneResource;
}

CScene::~CScene()
{
	SAFE_DELETE(m_pGameMode);
	SAFE_DELETE(m_pResource);
	SAFE_RELEASE_VECLIST(m_ObjList);
}

bool CScene::Init()
{
	return true;
}

void CScene::Start()
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();
}

void CScene::Input(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Input(fTime);
		++iter;
	}
}

void CScene::Update(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Update(fTime);
		++iter;
	}
}

void CScene::PostUpdate(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->PostUpdate(fTime);
		++iter;
	}
}

void CScene::Collision(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Collision(fTime);
		++iter;
	}
}

void CScene::PreRender(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->PreRender(fTime);
		++iter;
	}
}

void CScene::Render(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Render(fTime);
		++iter;
	}
}

void CScene::PostRender(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->PostRender(fTime);
		++iter;
	}
}
