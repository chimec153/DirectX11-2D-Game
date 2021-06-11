#pragma once
#include "../Ref.h"
class CBoneSocket :
	public CRef
{
	friend class CAnimation;
private:
	CBoneSocket();
	CBoneSocket(const CBoneSocket& socket);
	virtual ~CBoneSocket();

private:
	class CObj*				m_pObj;
	Vector3					m_vOffset;
	Vector4					m_vRot;
	Matrix					m_matBone;
	class CSceneComponent*	m_pCom;
	int						m_iBoneIndex;

public:
	void SetOffset(const Vector3& vOffset);
	void SetRotation(const Vector4& vRot);
	void SetBone(const Matrix& mat);
	void Update(float fTime, const Matrix& mat);
	void SetInfo(const std::string& strKey, class CSceneComponent* pCom);
};

