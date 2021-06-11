#include "Client.h"
#include "Input.h"

DEFINITION_SINGLE(CClient)

CClient::CClient()
{

}

CClient::~CClient()
{

}

bool CClient::GlobalSettings()
{
	GET_SINGLE(CInput)->AddAxisKey("MoveV", DIK_W, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveV", DIK_S, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveH", DIK_D, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveH", DIK_A, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("RotZ", DIK_Q, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("RotZ", DIK_E, -1.f);
	GET_SINGLE(CInput)->AddAxisKey("Z", DIK_Z, 1.f);
	GET_SINGLE(CInput)->AddAxisKey("1", DIK_1, 30.f);
	GET_SINGLE(CInput)->AddAxisKey("2", DIK_2, 30.f);
	GET_SINGLE(CInput)->AddAxisKey("3", DIK_3, 30.f);
	GET_SINGLE(CInput)->AddAxisKey("4", DIK_4, -30.f);
	GET_SINGLE(CInput)->AddAxisKey("5", DIK_5, -30.f);
	GET_SINGLE(CInput)->AddAxisKey("6", DIK_6, -30.f);
	GET_SINGLE(CInput)->AddActionKey("Fire1", DIK_SPACE);
	GET_SINGLE(CInput)->AddActionKey("Fire2", DIK_F1);
	GET_SINGLE(CInput)->AddActionKey("Fire3", DIK_F2);
	GET_SINGLE(CInput)->AddActionKey("Shift", DIK_LSHIFT);
	GET_SINGLE(CInput)->AddActionKey("O", DIK_O);
	GET_SINGLE(CInput)->AddActionKey("TAB", DIK_TAB);
	GET_SINGLE(CInput)->AddActionKey("F1", DIK_F1);
	GET_SINGLE(CInput)->AddActionKey("F2", DIK_F2);
	GET_SINGLE(CInput)->AddActionKey("F3", DIK_F3);
	GET_SINGLE(CInput)->AddActionKey("C", DIK_C);
	GET_SINGLE(CInput)->AddActionKey("X", DIK_X);
	GET_SINGLE(CInput)->AddActionKey("P", DIK_P);
	GET_SINGLE(CInput)->AddActionKey("Z", DIK_Z);
	GET_SINGLE(CInput)->AddActionKey("ESC", DIK_ESCAPE);

	return true;
}
