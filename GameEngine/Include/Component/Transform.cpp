#include "Transform.h"
#include "../Device.h"
#include "../Resource/ShaderManager.h"
#include "../Scene/Scene.h"
#include "../Camera/CameraManager.h"
#include "Camera.h"
#include "Light.h"

CTransform::CTransform()	:
	m_pScene(nullptr),
	m_pOwner(nullptr),
	m_pParent(nullptr),
	m_tCBuffer(),
	m_vVelocityScale(),
	m_vVelocityRot(),
	m_vVelocity(),
	m_vRelativeScale(1.f, 1.f, 1.f),
	m_vRelativeRot(),
	m_vRelativePos(),
	m_bInheritScale(true),
	m_bInheritRotX(true),
	m_bInheritRotY(true),
	m_bInheritRotZ(true),
	m_bInheritPos(true),
	m_bUpdateScale(true),
	m_bUpdateRot(true),
	m_vWorldScale(1.f, 1.f, 1.f),
	m_vWorldRot(),
	m_vWorldPos(),
	m_vPivot(),
	m_vMeshSize(),
	m_vQuaternion(0.f, 0.f, 0.f, 1.f),
	m_vRelativeQuat(m_vQuaternion),
	m_matScale(),
	m_matRot(),
	m_matPos(),
	m_matWorld(),
	m_matBone(),
	m_fFlipTex(0.f)
{
	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	m_vMeshSize = Vector3(1.f, 1.f, 0.f);
}

CTransform::CTransform(const CTransform& transform)
{
	*this = transform;

	m_ChildList.clear();
	m_pScene = nullptr;
	m_pOwner = nullptr;
	m_pParent = nullptr;

	m_bUpdateScale = true;
	m_bUpdateRot = true;
}

CTransform::~CTransform()
{
}

void CTransform::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}

const Matrix& CTransform::GetMatScale() const
{
	return m_matScale;
}

const Matrix& CTransform::GetMatRot() const
{
	return m_matRot;
}

const Matrix& CTransform::GetMatPos() const
{
	return m_matPos;
}

const Matrix& CTransform::GetMatWorld() const
{
	return m_matWorld;
}

void CTransform::SetBoneMatrix(const Matrix& mat)
{
	m_matBone = mat;
}

void CTransform::Start()
{
	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	if (pCam)
	{
		m_tCBuffer.matProjInv = pCam->GetProjMat();
		m_tCBuffer.matProjInv.Inverse();
		m_tCBuffer.matProjInv.Transpose();
		pCam->Release();
	}
}

void CTransform::Update(float fTime)
{
	if (m_vWorldRot.z > 360.f)
		m_vWorldRot.z -= 360.f;

	else if(m_vWorldRot.z < -360.f)
		m_vWorldRot.z += 360.f;
}

void CTransform::PostUpdate(float fTime)
{
	if (m_bUpdateScale)
		m_matScale.Scale(m_vWorldScale);

	if (m_bUpdateRot)
	{
		if (m_vQuaternion != Vector4(0.f, 0.f, 0.f, 1.f))
		{
			m_matRot = DirectX::XMMatrixRotationQuaternion(m_vQuaternion.Convert());
			
		}

		else
		{
			m_matRot.Rotate(m_vWorldRot);
		}
	}

	m_matPos.Translation(m_vWorldPos);

	m_matWorld = m_matScale * m_matRot * m_matPos * m_matBone;	//	크기 자전 이동 공전 부모
}

void CTransform::SetTransform()
{
	Resolution tRS = RESOLUTION;

	m_tCBuffer.matWorld = m_matWorld;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (m_pOwner->GetSceneComponentType() == SCENE_COMPONENT_TYPE::SCT_UI)
	{
		SAFE_RELEASE(pCam);

		pCam = GET_SINGLE(CCameraManager)->GetUICam();
	}		

	if (pCam)
	{
		m_tCBuffer.matView = pCam->GetViewMat();
		m_tCBuffer.matProj = pCam->GetProjMat();
		m_tCBuffer.matInvViewProj = pCam->GetVP();
		m_tCBuffer.matInvViewProj.Inverse();
	}

	SAFE_RELEASE(pCam);

	m_tCBuffer.matWV = m_tCBuffer.matWorld * m_tCBuffer.matView;
	m_tCBuffer.matWVP = m_tCBuffer.matWV * m_tCBuffer.matProj;
 	Light* pLight = Light::GetMainLight();
	if (pLight)
	{
		m_tCBuffer.matLightWVP = m_matWorld * pLight->GetVP();
	}
	SAFE_RELEASE(pLight);
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matView.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();
	m_tCBuffer.matLightWVP.Transpose();
	m_tCBuffer.matInvViewProj.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
}

void CTransform::SetShadow()
{
	Light* pLight = Light::GetMainLight();

	m_tCBuffer.matLightWVP = m_matWorld * pLight->GetVP();
	m_tCBuffer.matLightWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

	SAFE_RELEASE(pLight);
}

void CTransform::SetWorld()
{
	m_matWorld = m_matScale * m_matRot * m_matPos * m_matBone;
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Save(FILE* pFile)
{
	fwrite(&m_vRelativeScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativeRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativePos, sizeof(Vector3), 1, pFile);
	fwrite(m_vRelativeAxis, sizeof(Vector3), (int)WORLD_AXIS::AXIS_END, pFile);
	fwrite(&m_bInheritScale, 1, 1, pFile);
	fwrite(&m_bInheritRotX, 1, 1, pFile);
	fwrite(&m_bInheritRotY, 1, 1, pFile);
	fwrite(&m_bInheritRotZ, 1, 1, pFile);

	fwrite(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fwrite(m_vWorldAxis, sizeof(Vector3), (int)WORLD_AXIS::AXIS_END, pFile);
	fwrite(&m_vPivot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMeshSize, sizeof(Vector3), 1, pFile);
	fwrite(&m_vQuaternion, sizeof(Vector4), 1, pFile);
}

void CTransform::Load(FILE* pFile)
{
	fread(&m_vRelativeScale, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativeRot, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativePos, sizeof(Vector3), 1, pFile);
	fread(m_vRelativeAxis, sizeof(Vector3), (int)WORLD_AXIS::AXIS_END, pFile);
	fread(&m_bInheritScale, 1, 1, pFile);
	fread(&m_bInheritRotX, 1, 1, pFile);
	fread(&m_bInheritRotY, 1, 1, pFile);
	fread(&m_bInheritRotZ, 1, 1, pFile);

	fread(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fread(m_vWorldAxis, sizeof(Vector3), (int)WORLD_AXIS::AXIS_END, pFile);
	fread(&m_vPivot, sizeof(Vector3), 1, pFile);
	fread(&m_vMeshSize, sizeof(Vector3), 1, pFile);
	fread(&m_vQuaternion, sizeof(Vector4), 1, pFile);

	m_bUpdateRot = true;
	m_bUpdateScale = true;
}

void CTransform::SpawnWindow()
{
	if (ImGui::Begin("Transform"))
	{
		ImGui::Text("Pos");
		ImGui::InputFloat3("Pos", &m_vWorldPos.x);
		ImGui::Text("Scale");
		ImGui::InputFloat3("Scale", &m_vWorldScale.x);
		ImGui::Text("Rot");
		ImGui::InputFloat3("Rot", &m_vWorldRot.x);
		ImGui::Text("Quternion");
		if (ImGui::InputFloat4("Quternion", &m_vQuaternion.x))
		{
			m_vQuaternion = Vector4(XMQuaternionNormalize(m_vQuaternion.Convert()));
		}
	}
	ImGui::End();
}

void CTransform::SetInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
}
void CTransform::SetInheritRotX(bool bInherit)
{
	m_bInheritRotX = bInherit;
}

void CTransform::SetInheritRotY(bool bInherit)
{
	m_bInheritRotY = bInherit;
}

void CTransform::SetInheritRotZ(bool bInherit)
{
	m_bInheritRotZ = bInherit;
}

void CTransform::SetInheritPos(bool bIn)
{
	m_bInheritPos = bIn;
}

void CTransform::SetUpdateScale(bool bScale)
{
	m_bUpdateScale = bScale;
}

void CTransform::SetUpdateRot(bool bRot)
{
	m_bUpdateRot = bRot;
}

const Vector3& CTransform::GetVelocityScale() const
{
	return m_vVelocityScale;
}

const Vector3& CTransform::GetVelocityRot() const
{
	return m_vVelocityRot;
}

const Vector3& CTransform::GetVelocity() const
{
	return m_vVelocity;
}

float CTransform::GetVelocityAmt() const
{
	return m_vVelocity.Length();
}

const Vector3& CTransform::GetRelativeScale() const
{
	return m_vRelativeScale;
}

const Vector3& CTransform::GetRelativeRot() const
{
	return m_vRelativeRot;
}

const Vector3& CTransform::GetRelativePos() const
{
	return m_vRelativePos;
}

const Vector3& CTransform::GetRelativeAxis(WORLD_AXIS axis) const
{
	return m_vRelativeAxis[(int)axis];
}

void CTransform::InheritScale()
{
	if (m_pParent)
	{
		if (m_bInheritScale)
			m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();
	}

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::InheritRot()
{
	Matrix tmat = GetMatRot();

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x;// +m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y;// +m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z;// +m_pParent->GetWorldRot().z;

		m_vQuaternion = XMQuaternionMultiply(m_vRelativeQuat.Convert(), m_pParent->GetQuarternion().Convert());


		InheritPos();
	}

	tmat = XMMatrixRotationQuaternion(m_vRelativeQuat.Convert());
	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis->Normalize();
	}

	tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());
	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis->Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::InheritPos()
{
	if (m_pParent)
	{
		if (m_bInheritPos)
		{
			if (m_bInheritRotX || m_bInheritRotY || m_bInheritRotZ)
			{
				Matrix tmat = XMMatrixRotationQuaternion(m_pParent->GetQuarternion().Convert());

				//tmat.Rotate(m_pParent->GetWorldRot());

				Vector3 vPos = m_pParent->GetWorldPos();

				memcpy(&tmat._41, &vPos, sizeof(float) * 3);

				m_vWorldPos = m_vRelativePos.TransformCoord(tmat);
			}
			else
			{
				m_vWorldPos = m_vRelativePos + m_pParent->GetWorldPos();
			}
		}
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vRelativeScale;

	m_vRelativeScale = v;

	m_vWorldScale = m_vRelativeScale;

	InheritScale();
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z) - m_vRelativeScale;

	m_vRelativeScale = Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	InheritScale();
}

void CTransform::SetRelativePos(const Vector3& v)
{
	m_vVelocity += v - m_vRelativePos;

	m_vRelativePos = v;

	m_vWorldPos = m_vRelativePos;

	InheritPos();
}

void CTransform::SetRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vRelativePos;

	m_vRelativePos = Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	InheritPos();
}

void CTransform::SetRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vRelativeRot;

	m_vRelativeRot = v;

	m_vWorldRot = m_vRelativeRot;

	InheritRot();
}

void CTransform::SetRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vRelativeRot;

	m_vRelativeRot = Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	InheritRot();
}

void CTransform::SetRelativeRotX(float x)
{
	m_vVelocityRot.x += x - m_vRelativeRot.x;

	m_vRelativeRot.x = x;

	m_vWorldRot.x = x;

	InheritRot();
}

void CTransform::SetRelativeRotY(float y)
{
	m_vVelocityRot.y += y - m_vRelativeRot.y;

	m_vRelativeRot.y = y;

	m_vWorldRot.y = y;

	InheritRot();
}

void CTransform::SetRelativeRotZ(float z)
{
	m_vVelocityRot.z += z - m_vRelativeRot.z;

	m_vRelativeRot.z = z;

	m_vWorldRot.z = z;

	InheritRot();
}
void CTransform::AddRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vRelativeScale += v;

	m_vWorldScale = m_vRelativeScale;

	InheritScale();
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z);

	m_vRelativeScale += Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	InheritScale();
}

void CTransform::AddRelativePos(const Vector3& v)
{
	m_vVelocity += v;

	m_vRelativePos += v;

	m_vWorldPos = m_vRelativePos;

	InheritPos();
}

void CTransform::AddRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vRelativePos += Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	InheritPos();
}

void CTransform::AddRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vRelativeRot += v;

	m_vWorldRot = m_vRelativeRot;

	InheritRot();
}

void CTransform::AddRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z);

	m_vRelativeRot += Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	InheritRot();
}

void CTransform::AddRelativeRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vRelativeRot.x += x;

	m_vWorldRot.x = m_vRelativeRot.x;

	InheritRot();
}

void CTransform::AddRelativeRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vRelativeRot.y += y;

	m_vWorldRot.y = m_vRelativeRot.y;

	InheritRot();
}

void CTransform::AddRelativeRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vRelativeRot.z += z;

	m_vWorldRot.z = m_vRelativeRot.z;

	InheritRot();
}

const Vector3& CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

const Vector3& CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

const Vector3& CTransform::GetWorldPos() const
{
	return m_vWorldPos;
}

const Vector3& CTransform::GetWorldAxis(WORLD_AXIS axis) const
{
	return m_vWorldAxis[(int)axis];
}

const Vector3& CTransform::GetPivot() const
{
	return m_vPivot;
}

const Vector3& CTransform::GetMeshSize() const
{
	return m_vMeshSize;
}

const Vector4& CTransform::GetQuarternion() const
{
	return m_vQuaternion;
}

void CTransform::SetWorldScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vWorldScale;

	m_vWorldScale = v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent)
	{
		if (m_bInheritScale)
			m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();
	}

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z) - m_vWorldScale;

	m_vWorldScale = Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent)
	{
		if (m_bInheritScale)
			m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();
	}

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::SetWorldPos(const Vector3& v)
{
	m_vVelocity += v - m_vWorldPos;

	m_vWorldPos = v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat = m_pParent->GetMatRot();

		//tmat.Rotate(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy(&tmat._41, &vPos, sizeof(float) * 3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vWorldPos;

	m_vWorldPos = Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat = m_pParent->GetMatRot();

		//tmat.Rotate(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy(&tmat._41, &vPos, sizeof(float) * 3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::SetWorldRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vWorldRot;

	m_vWorldRot = v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	Matrix tmat = {};

	tmat.Rotate(m_vWorldRot);

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z) - m_vWorldRot;

	m_vWorldRot = Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());
	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;

		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;


	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRotX(float x)
{
	m_vVelocityRot.x += x - m_vWorldRot.x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRotY(float y)
{
	m_vVelocityRot.y += y - m_vWorldRot.y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetWorldRotZ(float z)
{
	m_vVelocityRot.z += z - m_vWorldRot.z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vWorldScale += v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent)
	{
		if (m_bInheritScale)
			m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();
	}

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::AddWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x,y,z);

	m_vWorldScale += Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent)
	{
		if (m_bInheritScale)
			m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();
	}

	m_bUpdateScale = true;

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritScale();
	}
}

void CTransform::AddWorldPos(const Vector3& v)
{
	m_vVelocity += v;

	m_vWorldPos += v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat = m_pParent->GetMatRot();

		//tmat.Rotate(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy(&tmat._41, &vPos, sizeof(float) * 3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vWorldPos += Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat = m_pParent->GetMatRot();

		//tmat.Rotate(m_pParent->GetWorldRot());

		Vector3 vPos = m_pParent->GetWorldPos();

		memcpy(&tmat._41, &vPos, sizeof(float) * 3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritPos();
	}
}

void CTransform::AddWorldRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vWorldRot += v;

	m_vRelativeRot = m_vWorldRot;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z);

	m_vWorldRot += Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vWorldRot.x += x;

	m_vRelativeRot.x = m_vWorldRot.x;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vWorldRot.y += y;

	m_vRelativeRot.y = m_vWorldRot.y;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
		tmat = tmat * m_pParent->GetMatRot();
	}

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddWorldRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vWorldRot.z += z;

	m_vRelativeRot.z = m_vWorldRot.z;

	Matrix tmat = XMMatrixRotationQuaternion(m_vQuaternion.Convert());

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
		tmat = tmat * m_pParent->GetMatRot();
	}

	if (m_vWorldRot.z >= 360.f)
		m_vWorldRot.z -= 360.f;

	else if (m_vWorldRot.z <= -360.f)
		m_vWorldRot.z += 360.f;

	m_bUpdateRot = true;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	//tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetPivot(const Vector3& v)
{
	m_vPivot = v;
}

void CTransform::SetPivot(float x, float y, float z)
{
	m_vPivot = Vector3(x, y, z);
}

void CTransform::SetMeshSize(const Vector3& v)
{
	m_vMeshSize = v;
}

void CTransform::Slerp(const Vector4& p, const Vector4& q, float s)
{
	m_vQuaternion = Vector4::Slerp(p, q, s);
}

void CTransform::Slerp(const Vector4& q, float s)
{
	m_vQuaternion = Vector4::Slerp(m_vQuaternion, q, s);
}

void CTransform::SetQuaternionRot(const Vector4& vAxis, float fAngle)
{
	m_vQuaternion = XMQuaternionRotationAxis(vAxis.Convert(), DegToRad(fAngle));

	InheritRot();

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddQuaternionRot(const Vector4& vAxis, float fAngle)
{
	m_vQuaternion = XMQuaternionMultiply(m_vQuaternion.Convert(),
		XMQuaternionRotationAxis(vAxis.Convert(), DegToRad(fAngle)));


	InheritRot();

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::SetQuaternionRotNorm(const Vector4& vAxis, float fAngle)
{
	m_vQuaternion = XMQuaternionRotationNormal(vAxis.Convert(), DegToRad(fAngle));


	InheritRot();
	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}

void CTransform::AddQuaternionRotNorm(const Vector4& vAxis, float fAngle)
{
	m_vQuaternion = XMQuaternionMultiply(m_vQuaternion.Convert(),
		XMQuaternionRotationNormal(vAxis.Convert(), DegToRad(fAngle)));

	InheritRot();

	std::list<CTransform*>::iterator iter = m_ChildList.begin();
	std::list<CTransform*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->InheritRot();
	}
}