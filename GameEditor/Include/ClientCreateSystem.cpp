#include "ClientCreateSystem.h"
#include "Client/Object/Player.h"
#include "Client/Object/Bullet.h"
#include "Client/GameMode/MainGameMode.h"
#include "Scene/SceneManager.h"

DEFINITION_SINGLE(CClientCreateSystem)

CClientCreateSystem::CClientCreateSystem()
{

}

CClientCreateSystem::~CClientCreateSystem()
{

}

bool CClientCreateSystem::Init()
{
	GET_SINGLE(CSceneManager)->SetEditorObjCreate(this, &CClientCreateSystem::CreateObj);
	GET_SINGLE(CSceneManager)->SetEditorGameModeCreate(this, &CClientCreateSystem::CreateGameMode);

	return true;
}

CObj* CClientCreateSystem::CreateObj(int iType)
{
	switch (iType)
	{
	case (int)OBJ_CLASS_TYPE::OCT_PLAYER:
		return new CPlayer;
	case (int)OBJ_CLASS_TYPE::OCT_BULLET:
		return new CBullet;
	}

	return nullptr;
}

CGameMode* CClientCreateSystem::CreateGameMode(int iType)
{
	switch (iType)
	{
	case (int)GAMEMODE_CLASS_TYPE::GCT_MAIN:
		return new CMainGameMode;
	}

	return nullptr;
}
