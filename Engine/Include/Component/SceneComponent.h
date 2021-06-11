#pragma once
#include "Component.h"
#include "../Render/PostProcess.h"
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
	std::list<CSceneComponent*>		m_ChildList;
	SCENE_COMPONENT_TYPE			m_eSceneComponentType;
	SCENECOMPONENT_CLASS_TYPE		m_eSceneComponentClassType;
	class CRenderer*				m_pRenderer;
	bool							m_bInst;
	bool							m_bPostProcess;
	class CPostProcess*				m_pPostProcess;
	class CRenderInstance*			m_pInstancing;
	bool							m_bInFrustum;
	bool							m_bCulling;

public:
	class CMesh* GetMesh()	const;
	void SetMesh(const std::string& strName);
	void SetMesh(class CMesh* pMesh);
	class CMaterial* GetMaterial(size_t iContainer = 0, size_t iSubset = 0)	const;
	void SetMaterial(class CMaterial* pMaterial, size_t iContainer = 0, size_t iSubset = 0);
	void SetMaterial(const std::string& strMtrl, size_t iContainer = 0, size_t iSubset = 0);
	bool IsInstanced()	const;
	void SetInstance(bool bInst = true);
	void SetLayer(class CLayer* pLayer);
	class CRenderer* GetRenderer()	const;
	class CTransform* GetTransform()	const;
	void SetShader(const std::string& strShader, size_t iContainer = 0, size_t iSubset = 0);
	void SetShader(class CShader* pShader, size_t iContainer = 0, size_t iSubset = 0);
	class CShader* GetShader(size_t iContainer = 0, size_t iSubset = 0)	const;
	bool IsPostProcess()	const;
	class CPostProcess* GetPostProcess()	const;
	void SetInstIndex(int iIndex);
	void SetInstancing(class CRenderInstance* pInst);
	class CRenderInstance* GetInstancing()	const;
	bool IsInFrustum()	const;
	void SetCulling(bool bCull);

public:
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
		size_t iContainer = 0, size_t iSubset = 0);
	void SetTexture(REGISTER_TYPE eType, class CTexture* pTex, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
		size_t iContainer = 0, size_t iSubset = 0);
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
		size_t iContainer = 0, size_t iSubset = 0);
	void SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX,
		size_t iContainer = 0, size_t iSubset = 0);

public:
	bool AddChild(CSceneComponent* pChild);
	bool DeleteChild(CSceneComponent* pChild);
	bool Detach(CSceneComponent* pChild);
	SCENE_COMPONENT_TYPE GetSceneComponentType()	const;
	CSceneComponent* FindComponent(const std::string& strTag);
	template <typename T>
	T* FindComByType(const std::string& strKey = "")
	{
		if (typeid(T) == typeid(*this))
		{
			if (strKey == "" ||
				strKey == GetName())
			{
				AddRef();

				return (T*)this;
			}
		}

		else
		{
			std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
			std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

			for (; iter != iterEnd; ++iter)
			{
				T* pCom = (*iter)->FindComByType<T>(strKey);

				if (pCom)
					return pCom;
			}
		}

		return nullptr;
	}
	SCENECOMPONENT_CLASS_TYPE GetSceneComponentClassType()	const;
	void SetScene(class CScene* pScene);
	void SetSceneComType(SCENE_COMPONENT_TYPE eType);
	CSceneComponent* GetParent()	const;
	void SetObj(class CObj* pObj);
	void AddRenderState(const std::string& strKey);
	class CRenderState* FindRenderState(const std::string& strKey);
	void DeleteRenderState(const std::string& strKey);
	void SetState();
	void ResetState();
	
	template <typename T>
	T* CreatePostProcess(const std::string& strKey)
	{
		if (m_pPostProcess)
			return nullptr;		

		m_pPostProcess = new T;

		m_pPostProcess->m_pCom = this;

		m_bPostProcess = true;

		if (!m_pPostProcess->Init())
		{
			SAFE_RELEASE(m_pPostProcess);
			return nullptr;
		}

		m_pPostProcess->AddRef();

		return static_cast<T*>(m_pPostProcess);
	}
public:
	virtual bool Init();
	virtual bool Init(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	virtual bool Init(FILE* pFile);
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSceneComponent* Clone() = 0;
	virtual void RenderShadow(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void CheckFrustum();

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void SetInheritPos(bool bIn);
	void SetUpdateScale(bool bScale);
	void SetUpdateRot(bool bRot);
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
	const Vector3& GetVelocityScale()			const;
	const Vector3& GetVelocityRot()			const;
	const Vector3& GetVelocity()				const;
	float GetVelocityAmt()				const;
	const Vector3& GetRelativeScale()			const;
	const Vector3& GetRelativeRot()			const;
	const Vector3& GetRelativePos()			const;
	const Vector3& GetRelativeAxis(WORLD_AXIS axis)	const;

public:
	const Vector3& GetWorldScale()				const;
	const Vector3& GetWorldRot()				const;
	const Vector3& GetWorldPos()				const;
	const Vector3& GetWorldAxis(WORLD_AXIS axis)		const;
	const Vector3& GetPivot()					const;
	const Vector3& GetMeshSize()				const;

public:
	const Matrix& GetMatScale()				const;
	const Matrix& GetMatRot()					const;
	const Matrix& GetMatPos()					const;
	const Matrix& GetMatWorld()				const;

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

public:
	void GetAllComponentName(std::vector<Hierarchy>& vecHierarchy);
	void GetAllComponent(std::vector<CSceneComponent*>& vecCom);

public:
	virtual void SpawnWindow();
	void ShowNode();
};

