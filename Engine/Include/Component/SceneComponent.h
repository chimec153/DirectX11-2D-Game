#pragma once
#include "Component.h"
class CSceneComponent :
	public CComponent
{
	friend class CObj;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent() = 0;

protected:
	class CTransform*				m_pTransform;
	CSceneComponent*				m_pParent;
	std::vector<CSceneComponent*>	m_vecChild;
	SCENE_COMPONENT_TYPE			m_eSceneComponentType;

public:
	bool AddChild(CSceneComponent* pChild);
	bool DeleteChild(CSceneComponent* pChild);
	bool Detach(CSceneComponent* pChild);
	SCENE_COMPONENT_TYPE GetSceneComponentType()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSceneComponent* Clone() = 0;

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
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

public:
	Vector3 GetRelativeScale()			const;
	Vector3 GetRelativeRot()			const;
	Vector3 GetRelativePos()			const;
	Vector3 GetRelativeAxis(AXIS axis)	const;

public:
	Vector3 GetWorldScale()				const;
	Vector3 GetWorldRot()				const;
	Vector3 GetWorldPos()				const;
	Vector3 GetWorldAxis(AXIS axis)		const;
	Vector3 GetPivot()					const;

public:
	Matrix GetMatScale()				const;
	Matrix GetMatRot()					const;
	Matrix GetMatPos()					const;
	Matrix GetMatWorld()				const;

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
};

