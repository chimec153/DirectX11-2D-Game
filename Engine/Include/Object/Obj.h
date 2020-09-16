#pragma once

#include "../Ref.h"

class CObj :
	public CRef
{
	friend class CScene;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj() = 0;

protected:
	bool									m_bStart;
	class CScene*							m_pScene;

public:
	bool IsStart()	const
	{
		return m_bStart;
	}

protected:
	class CSceneComponent*					m_pRootComponent;
	std::vector<class CObjComponent*>		m_vecObjComponent;

public:
	void SetRootComponent(class CSceneComponent*);

public:
	bool Init();
	void Start();
	void Input(float fTime);
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

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

public:
	template <typename T>
	T* CreateComponent(const std::string& strName)
	{
		T* pComponent = new T;

		pComponent->SetName(strName);
		pComponent->m_pScene = m_pScene;
		pComponent->m_pObj = this;

		if (!pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return nullptr;
		}

		if (pComponent->GetType() == COMPONENT_TYPE::CT_OBJECT)
		{
			pComponent->AddRef();
			m_vecObjComponent.push_back((CObjComponent*)pComponent);
		}

		return pComponent;		
	}
};

