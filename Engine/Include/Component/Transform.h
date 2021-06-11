#pragma once

#include "SceneComponent.h"

class CTransform
{
	friend class CSceneComponent;

private:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	class CScene*				m_pScene;
	class CSceneComponent*		m_pOwner;
	std::list<CTransform*>		m_ChildList;
	CTransform*					m_pParent;
	TransformCBuffer			m_tCBuffer;

public:
	void SetScene(class CScene* pScene);

private:
	Vector3						m_vVelocityScale;
	Vector3						m_vVelocityRot;
	Vector3						m_vVelocity;
	Vector3						m_vRelativeScale;
	Vector3						m_vRelativeRot;
	Vector3						m_vRelativePos;
	Vector3						m_vRelativeAxis[(int)WORLD_AXIS::AXIS_END];
	bool						m_bInheritScale;
	bool						m_bInheritRotX;
	bool						m_bInheritRotY;
	bool						m_bInheritRotZ;
	bool						m_bInheritPos;
	bool						m_bUpdateScale;
	bool						m_bUpdateRot;

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void SetInheritPos(bool bIn);
	void SetUpdateScale(bool bScale);
	void SetUpdateRot(bool bRot);
	const Vector3& GetVelocityScale()			const;
	const Vector3& GetVelocityRot()			const;
	const Vector3& GetVelocity()				const;
	float GetVelocityAmt()				const;
	const Vector3& GetRelativeScale()			const;
	const Vector3& GetRelativeRot()			const;
	const Vector3& GetRelativePos()			const;
	const Vector3& GetRelativeAxis(WORLD_AXIS axis)	const;

public:
	void InheritScale();
	void InheritRot();
	void InheritPos();

public:
	void SetRelativeScale(const Vector3& v);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativePos(const Vector3& v);
	void SetRelativePos(float x, float y, float z);
	void SetRelativeRot(const Vector3& v);
	void SetRelativeRot(float x, float y, float z);
	void SetRelativeRotX(float x);
	void SetRelativeRotY(float y);
	void SetRelativeRotZ(float z);
	void AddRelativeScale(const Vector3& v);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativePos(const Vector3& v);
	void AddRelativePos(float x, float y, float z);
	void AddRelativeRot(const Vector3& v);
	void AddRelativeRot(float x, float y, float z);
	void AddRelativeRotX(float x);
	void AddRelativeRotY(float y);
	void AddRelativeRotZ(float z);

private:
	Vector3						m_vWorldScale;
	Vector3						m_vWorldRot;
	Vector3						m_vWorldPos;
	Vector3						m_vWorldAxis[(int)WORLD_AXIS::AXIS_END];
	Vector3						m_vPivot;
	Vector3						m_vMeshSize;
	Vector4						m_vQuaternion;
	Vector4						m_vRelativeQuat;

public:
	const Vector3& GetWorldScale()			const;
	const Vector3& GetWorldRot()			const;
	const Vector3& GetWorldPos()			const;
	const Vector3& GetWorldAxis(WORLD_AXIS axis)	const;
	const Vector3& GetPivot()				const;
	const Vector3& GetMeshSize()			const;
	const Vector4& GetQuarternion()	const;

public:
	void SetWorldScale(const Vector3& v);
	void SetWorldScale(float x, float y, float z);
	void SetWorldPos(const Vector3& v);
	void SetWorldPos(float x, float y, float z);
	void SetWorldRot(const Vector3& v);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void AddWorldScale(const Vector3& v);
	void AddWorldScale(float x, float y, float z);
	void AddWorldPos(const Vector3& v);
	void AddWorldPos(float x, float y, float z);
	void AddWorldRot(const Vector3& v);
	void AddWorldRot(float x, float y, float z);
	void AddWorldRotX(float x);
	void AddWorldRotY(float y);
	void AddWorldRotZ(float z);
	void SetPivot(const Vector3& v);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& v);
	void Slerp(const Vector4& p, const Vector4& q, float s);
	void Slerp(const Vector4& q, float s);
	void SetQuaternionRot(const Vector4& vAxis, float fAngle);
	void AddQuaternionRot(const Vector4& vAxis, float fAngle);
	void SetQuaternionRotNorm(const Vector4& vAxis, float fAngle);
	void AddQuaternionRotNorm(const Vector4& vAxis, float fAngle);

private:
	Matrix						m_matScale;
	Matrix						m_matRot;
	Matrix						m_matPos;
	Matrix						m_matWorld;
	Matrix						m_matBone;
	float						m_fFlipTex;

public:
	const Matrix& GetMatScale()	const;
	const Matrix& GetMatRot()		const;
	const Matrix& GetMatPos()		const;
	const Matrix& GetMatWorld()	const;
	void SetBoneMatrix(const Matrix& mat);

public:
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void SetTransform();
	void SetShadow();
	void SetWorld();
	CTransform* Clone();

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	void SpawnWindow();
};

