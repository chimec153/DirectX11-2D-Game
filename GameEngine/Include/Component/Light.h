#pragma once
#include "SceneComponent.h"
class Light :
	public CSceneComponent
{
private:
	Light();
	Light(const Light& light);
	virtual ~Light() override;

private:
	LIGHTCBUFFER	m_tCBuffer;
	Matrix			m_matView;
	Matrix			m_matProj;
	Matrix			m_matVP;

public:
	const Matrix& GetView()	const;
	const Matrix& GetProj()	const;
	const Matrix& GetVP()	const;

public:
	void SetRange(float fRange);
	void SetAngleIn(float fAngleIn);
	void SetAngleOut(float fAngleOut);
	void SetAttn(float c, float a, float b);
	void SetAttn(const Vector3& vAttn);
	void SetDif(const Vector4& vDir);
	void SetAmb(const Vector4& vAmb);
	void SetSpc(const Vector4& vSpc);
	void SetEmv(const Vector4& vEmv);
	void SetDif(float r, float g, float b, float a);
	void SetAmb(float r, float g, float b, float a);
	void SetSpc(float r, float g, float b, float a);
	void SetEmv(float r, float g, float b, float a);
	void SetDif(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetAmb(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetSpc(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetEmv(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetLightType(LIGHT_TYPE eType);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual Light* Clone() override;

public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	virtual void SpawnWindow() override;

public:
	void SetShader();

private:
	static std::unordered_map<std::string, Light*> m_mapLight;
	static Light* m_pMainLight;

public:
	template <typename T>
	static T* CreateLight(const std::string& strKey)
	{
		T* pLight = new T;

		pLight->SetName(strKey);

		if (!pLight->Init())
		{
			SAFE_RELEASE(pLight);
			return nullptr;
		}

		m_mapLight.insert(std::make_pair(strKey, pLight));

		pLight->AddRef();

		if (!m_pMainLight)
		{
			m_pMainLight = pLight;

			m_pMainLight->AddRef();
		}

		return pLight;
	}

public:
	static class Light* FindLight(const std::string& strKey);
	static void SetLight();
	static void Destroy();
	static void RenderAll(float fTime);
	static void SetMainLight(Light* pLight);
	static Light* GetMainLight();
};

