#include "Light.h"
#include "../Resource/ShaderManager.h"
#include "../Camera/CameraManager.h"
#include "../Component/Camera.h"
#include "../Device.h"

std::unordered_map<std::string, Light*> Light::m_mapLight = {};
Light* Light::m_pMainLight = nullptr;

Light::Light()	:
	m_tCBuffer()
	, m_matView()
	, m_matProj()
	, m_matVP()
{
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vAmb = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	m_tCBuffer.vSpc = Vector4::White;
	m_tCBuffer.vEmv = Vector4::White;
	m_tCBuffer.fAngleOut = PI_DIV2 / 3.f;
	m_tCBuffer.fAngleIn = PI_DIV4 / 2.f;
	m_tCBuffer.fRange = 50.f;
	m_tCBuffer.vAttn.y = 5.f;
	m_tCBuffer.vAttn.z = 7.f;
}

Light::Light(const Light& light)	:
	m_tCBuffer(light.m_tCBuffer)
	, m_matView(light.m_matView)
	, m_matProj(light.m_matProj)
	, m_matVP(light.m_matVP)
{
}

Light::~Light()
{
}

const Matrix& Light::GetView() const
{
	return m_matView;
}

const Matrix& Light::GetProj() const
{
	return m_matProj;
}

const Matrix& Light::GetVP() const
{
	return m_matVP;
}

void Light::SetRange(float fRange)
{
	m_tCBuffer.fRange = fRange;
}

void Light::SetAngleIn(float fAngleIn)
{
	m_tCBuffer.fAngleIn = fAngleIn;
}

void Light::SetAngleOut(float fAngleOut)
{
	m_tCBuffer.fAngleOut = fAngleOut;
}

void Light::SetAttn(float c, float a, float b)
{
	SetAttn(Vector3(c, a, b));
}

void Light::SetAttn(const Vector3& vAttn)
{
	m_tCBuffer.vAttn = vAttn;
}

void Light::SetDif(const Vector4& vDif)
{
	m_tCBuffer.vDif = vDif;
}

void Light::SetAmb(const Vector4& vAmb)
{
	m_tCBuffer.vAmb = vAmb;
}

void Light::SetSpc(const Vector4& vSpc)
{
	m_tCBuffer.vSpc = vSpc;
}

void Light::SetEmv(const Vector4& vEmv)
{
	m_tCBuffer.vEmv = vEmv;
}

void Light::SetDif(float r, float g, float b, float a)
{
	SetDif(Vector4(r, g, b, a));
}

void Light::SetAmb(float r, float g, float b, float a)
{
	SetAmb(Vector4(r, g, b, a));
}

void Light::SetSpc(float r, float g, float b, float a)
{
	SetSpc(Vector4(r, g, b, a));
}

void Light::SetEmv(float r, float g, float b, float a)
{
	SetEmv(Vector4(r, g, b, a));
}

void Light::SetDif(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetDif(Vector4(r/255.f, g / 255.f, b / 255.f, a / 255.f));
}

void Light::SetAmb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetAmb(Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f));
}

void Light::SetSpc(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetSpc(Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f));
}

void Light::SetEmv(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SetEmv(Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f));
}

void Light::SetLightType(LIGHT_TYPE eType)
{
	m_tCBuffer.eType = eType;

	switch (m_tCBuffer.eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
	{
		Resolution tRS = RESOLUTION;

		m_matProj = XMMatrixOrthographicLH(
			static_cast<float>(tRS.iWidth), static_cast<float>(tRS.iHeight),
			0.3f, 5000.f);
	}
		break;
	case LIGHT_TYPE::POINT:
	{

	}
		break;
	case LIGHT_TYPE::SPOT:
	{

	}
		break;
	}
}

bool Light::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetLightType(LIGHT_TYPE::DIRECTIONAL);

	return true;
}

void Light::Start()
{
	CSceneComponent::Start();
}

void Light::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix view = pCam->GetViewMat();

	SAFE_RELEASE(pCam);

	switch (m_tCBuffer.eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
	{
		Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Z);

		m_tCBuffer.vDir = vAxis.TransformNormal(view);
		m_tCBuffer.vDir.Normalize();

		Vector3 vPos = GetWorldPos();
		Vector3 vAxis_x = GetWorldAxis(WORLD_AXIS::AXIS_X);
		Vector3 vAxis_y = GetWorldAxis(WORLD_AXIS::AXIS_Y);

		m_matView[0][0] = vAxis_x.x;
		m_matView[1][0] = vAxis_x.y;
		m_matView[2][0] = vAxis_x.z;
		m_matView[0][1] = vAxis_y.x;
		m_matView[1][1] = vAxis_y.y;
		m_matView[2][1] = vAxis_y.z;
		m_matView[0][2] = vAxis.x;
		m_matView[1][2] = vAxis.y;
		m_matView[2][2] = vAxis.z;

		m_matView[3][0] = -vPos.Dot(vAxis_x);
		m_matView[3][1] = -vPos.Dot(vAxis_y);
		m_matView[3][1] = -vPos.Dot(vAxis);

		m_matVP = m_matView * m_matProj;

		m_tCBuffer.matVP = m_matVP;
		m_tCBuffer.matVP.Transpose();
	}
		break;
	case LIGHT_TYPE::POINT:
	{
		Vector3 vPos = GetWorldPos();

		m_tCBuffer.vPos = vPos.TransformCoord(view);
	}
		break;
	case LIGHT_TYPE::SPOT:
	{
		Vector3 vPos = GetWorldPos();

		m_tCBuffer.vPos = vPos.TransformCoord(view);
		Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Z);

		m_tCBuffer.vDir = vAxis.TransformNormal(view);
		m_tCBuffer.vDir.Normalize();
	}
		break;
	}
	
}

void Light::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void Light::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void Light::PreRender(float fTime)
{
	//CSceneComponent::PreRender(fTime);
}

void Light::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	SetShader();

	UINT iStride = 0;
	UINT iOffset = 0;
	ID3D11Buffer* pBuffer = nullptr;
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CONTEXT->IASetVertexBuffers(0, 1, &pBuffer, &iStride, &iOffset);
	CONTEXT->Draw(4, 0);
}

void Light::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

Light* Light::Clone()
{
	return new Light(*this);
}

void Light::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void Light::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}

void Light::SpawnWindow()
{
	CSceneComponent::SpawnWindow();

	const char* pName = GetName().c_str();

	if (!strcmp(pName, ""))
	{
		pName = "None";
	}

	if(ImGui::Begin(pName))
	{
		ImGui::ColorPicker4("Diffuse", &m_tCBuffer.vDif.x);
		ImGui::ColorPicker4("Ambient", &m_tCBuffer.vAmb.x);
		ImGui::ColorPicker4("Specular", &m_tCBuffer.vSpc.x);
		ImGui::ColorPicker4("Emissive", &m_tCBuffer.vEmv.x);

		static Vector3 vRot = {};
		ImGui::SliderFloat3("Rotation", &vRot.x, -180.f, 180.f);
		SetQuaternionRot(Vector4(1.f, 0.f, 0.f, 0.f), vRot.x);
		Vector3 vPos = GetWorldPos();
		ImGui::InputFloat3("Position", &vPos.x);
		SetWorldPos(vPos);
		ImGui::SliderFloat("AngleIn", &m_tCBuffer.fAngleIn, 0.f, 90.f);
		ImGui::SliderFloat("AngleOut", &m_tCBuffer.fAngleOut, 0.f, 90.f);
		ImGui::InputFloat3("Attenuation", &m_tCBuffer.vAttn.x);
		ImGui::SliderInt("Light Type", reinterpret_cast<int*>(&m_tCBuffer.eType), 0, static_cast<int>(LIGHT_TYPE::SPOT));
		ImGui::InputFloat("Light Range", &m_tCBuffer.fRange);
	}
	ImGui::End();
}

void Light::SetShader()
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Light", &m_tCBuffer);
}

Light* Light::FindLight(const std::string& strKey)
{
	std::unordered_map<std::string, Light*>::iterator iter = m_mapLight.find(strKey);

	if (iter == m_mapLight.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void Light::SetLight()
{
	std::unordered_map<std::string, Light*>::iterator iter = m_mapLight.begin();
	std::unordered_map<std::string, Light*>::iterator iterEnd = m_mapLight.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->SetShader();
	}
}

void Light::Destroy()
{
	SAFE_RELEASE_MAP(m_mapLight);
	SAFE_RELEASE(m_pMainLight);
}

void Light::RenderAll(float fTime)
{
	std::unordered_map<std::string, Light*>::iterator iter = m_mapLight.begin();
	std::unordered_map<std::string, Light*>::iterator iterEnd = m_mapLight.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Render(fTime);
	}
}

void Light::SetMainLight(Light* pLight)
{
	SAFE_RELEASE(m_pMainLight);

	m_pMainLight = pLight;

	if (m_pMainLight)
	{
		m_pMainLight->AddRef();
	}
}

Light* Light::GetMainLight()
{
	if (m_pMainLight)
	{
		m_pMainLight->AddRef();
	}

	return m_pMainLight;
}
