#include "BoneSocket.h"
#include "../Component/SceneComponent.h"
#include "../Component/Transform.h"
#include "../Object/Obj.h"

CBoneSocket::CBoneSocket() :
	m_pObj(nullptr)
	, m_vOffset()
	, m_vRot()
	, m_matBone()
	, m_pCom(nullptr)
	, m_iBoneIndex(0)
{
}

CBoneSocket::CBoneSocket(const CBoneSocket& socket)	:
	m_pObj(socket.m_pObj)
	, m_vOffset(socket.m_vOffset)
	, m_vRot(socket.m_vRot)
	, m_matBone(socket.m_matBone)
	, m_pCom(socket.m_pCom)
	, m_iBoneIndex(socket.m_iBoneIndex)
{
	if (m_pCom)
		m_pCom->AddRef();
}

CBoneSocket::~CBoneSocket()
{
	SAFE_RELEASE(m_pCom);
	SAFE_RELEASE(m_pObj);
}

void CBoneSocket::SetOffset(const Vector3& vOffset)
{
	m_vOffset = vOffset;
}

void CBoneSocket::SetRotation(const Vector4& vRot)
{
	m_vRot = vRot;
}

void CBoneSocket::SetBone(const Matrix& mat)
{
	m_matBone = mat;
}

void CBoneSocket::Update(float fTime, const Matrix& mat)
{
	if (m_pObj)
	{
		CSceneComponent* pCom = m_pObj->GetRootComponent();
		CTransform* pTrans = pCom->GetTransform();

		SAFE_RELEASE(pCom);

		if (pTrans)
		{
			Matrix matOffset;
			matOffset.Translation(m_vOffset);
			Matrix matRot;
			matRot = XMMatrixRotationQuaternion(m_vRot.Convert());

			pTrans->SetBoneMatrix(matRot * matOffset * mat);
			pTrans->SetWorld();
		}
	}
}

void CBoneSocket::SetInfo(const std::string& strKey, CSceneComponent* pCom)
{
	SetName(strKey);
	SAFE_RELEASE(m_pCom);
	m_pCom = pCom;

	if (m_pCom)
	{
		m_pCom->AddRef();

		SAFE_RELEASE(m_pObj);
		m_pObj = m_pCom->GetObj();
	}
}

