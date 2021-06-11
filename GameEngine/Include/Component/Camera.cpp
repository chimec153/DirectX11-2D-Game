#include "Camera.h"
#include "../Device.h"
#include "../Camera/CameraManager.h"
#include "../Scene/Scene.h"

CCamera::CCamera()	:
	m_fDist(3000.f),
	m_fAngle(90.f),
	m_bZoom(false),
	m_pTarget(nullptr),
	m_pFocus(nullptr),
	m_fMaxRange(0.f),
	m_fMinRange(0.f),
	m_vPos(),
	m_fZoom(-100.f),
	m_matVP(),
	m_tRS(),
	m_fZoomLimit(0.25f),
	m_vZoomCenter(0.5f, 0.5f)
{
	m_tRS = RESOLUTION;
	m_tRect.fL = 0.f;
	m_tRect.fR = 1.f;
	m_tRect.fB = 0.f;
	m_tRect.fT = 1.f;
	SetCameraType(CAMERA_TYPE::CT_3D);
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA;
	
}

CCamera::CCamera(const CCamera& cam)	:
	CSceneComponent(cam)
	, m_matView(cam.m_matView)
	, m_matProj(cam.m_matProj)
	, m_eType(cam.m_eType)
	, m_fDist(cam.m_fDist)
	, m_fAngle(cam.m_fAngle)
	, m_bZoom(cam.m_bZoom)
	, m_pTarget(cam.m_pTarget)
	, m_pFocus(cam.m_pFocus)
	, m_fMaxRange(cam.m_fMaxRange)
	, m_fMinRange(cam.m_fMinRange)
	, m_vPos(cam.m_vPos)
	, m_fZoom(cam.m_fZoom)
	, m_matVP(cam.m_matVP)
	, m_tRS(cam.m_tRS)
	, m_tRect(cam.m_tRect)
	, m_fZoomLimit(cam.m_fZoomLimit)
	, m_vZoomCenter(cam.m_vZoomCenter)
{
	if (m_pFocus)
		m_pFocus->AddRef();
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pFocus);
}

const Matrix& CCamera::GetViewMat() const
{
	return m_matView;
}

const Matrix& CCamera::GetProjMat() const
{
	return m_matProj;
}

const Matrix& CCamera::GetVP() const
{
	return m_matVP;
}

void CCamera::SetCameraType(CAMERA_TYPE eType)
{
	m_eType = eType;

	switch (m_eType)
	{
	case CAMERA_TYPE::CT_3D:
		m_matProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fAngle), 
			m_tRS.iWidth / (float)m_tRS.iHeight, 0.3f, m_fDist);
		break;
	case CAMERA_TYPE::CT_2D:
	case CAMERA_TYPE::CT_UI:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(m_tRect.fL * m_tRS.iWidth, m_tRect.fR * m_tRS.iWidth,
			m_tRect.fB * m_tRS.iHeight, m_tRect.fT * m_tRS.iHeight, -1000.f, m_fDist);
		break;	
	}
}

void CCamera::SetAngle(float fAngle)
{
	m_fAngle = fAngle;

	if (m_eType == CAMERA_TYPE::CT_3D)
		SetCameraType(m_eType);
}

void CCamera::SetDist(float fDist)
{
	m_fDist = fDist;

	SetCameraType(m_eType);
}

float CCamera::GetAngle() const
{
	return m_fAngle;
}

float CCamera::GetDist() const
{
	return m_fDist;
}

void CCamera::SetZoom(bool bZoom, float fZoom)
{
	m_bZoom = bZoom;
	m_fZoomLimit = fZoom;

	if (!m_bZoom)
	{
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(
			m_tRect.fL  * m_tRS.iWidth,
			m_tRect.fR  * m_tRS.iWidth,
			m_tRect.fB  * m_tRS.iHeight,
			m_tRect.fT  * m_tRS.iHeight,
			-1000.f, m_fDist);
	}
}

void CCamera::SetTarget(CObj* pObj)
{
	m_pTarget = pObj;
}

void CCamera::SetFocus(CObj* pObj)
{
	SAFE_RELEASE(m_pFocus);

	m_pFocus = pObj;

	if (m_pFocus)
		m_pFocus->AddRef();
}

void CCamera::SetMax(float fMax)
{
	m_fMaxRange = fMax;
}

void CCamera::SetMin(float fMin)
{
	m_fMinRange = fMin;
}

void CCamera::SetMovePos(const Vector3 vPos)
{
	m_vPos = vPos;
}

void CCamera::SetRect(float l, float r,  float b, float t )
{
	m_tRect.fL = l;
	m_tRect.fR = r;
	m_tRect.fT = t;
	m_tRect.fB = b;

	m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(l * m_tRS.iWidth, r * m_tRS.iWidth,
		b * m_tRS.iHeight, t * m_tRS.iHeight, -1000.f, m_fDist);
}

const Resolution& CCamera::GetResolution() const
{
	return m_tRS;
}

void CCamera::SetCallBack(void(*pFunc)(float))
{
	m_pCallBack = std::bind(pFunc, std::placeholders::_1);
}

void CCamera::SpawnControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		//Vector3 vRot = GetWorldRot();
		Vector3 vPos = GetWorldPos();

		ImGui::Text("Position");
		ImGui::SliderFloat("D", &m_fDist, -3000.f, 3000.f);
		//ImGui::SliderAngle("Angle", &m_fAngle, -180.f, 180.f);
		ImGui::SliderFloat3("Pos", &vPos.x, 0.f, 10000.f);
		ImGui::Text("Orientation");
		//ImGui::SliderAngle("X", &vRot.x, -180.f, 180.f);
		//ImGui::SliderAngle("Y", &vRot.y, -180.f, 180.f);
		//ImGui::SliderAngle("Z", &vRot.z, -180.f, 180.f);
		ImGui::Text("Dimension");

		ImGui::Selectable("2D");
		ImGui::Selectable("3D");
		ImGui::Selectable("UI");

		if (m_eType !=CAMERA_TYPE::CT_3D)
		{
			ImGui::SliderFloat4("Rect", &m_tRect.fL, 0.f, 1.f);
		}

		if (m_pTarget)
		{
			char strTarget[64] = {};

			sprintf_s(strTarget, "Target: ");

			strcat_s(strTarget, m_pTarget->GetName().c_str());

			ImGui::Text(strTarget);
		}

		if (m_pFocus)
		{
			char strTarget[64] = {};

			sprintf_s(strTarget, "Focus: ");

			strcat_s(strTarget, m_pFocus->GetName().c_str());

			ImGui::Text(strTarget);

			ImGui::SliderFloat("MaxLange", &m_fMaxRange, 0.f, 1000.f);
			ImGui::SliderFloat("MinLange", &m_fMinRange, 0.f, 1000.f);
		}

		SetWorldPos(vPos);
		//SetWorldRot(vRot);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}

		SetCameraType(m_eType);
	}
	ImGui::End();
}

void CCamera::Reset()
{
	m_fDist = 500.f;
	m_fAngle = 0.f;
}

void CCamera::SetZoomDist(float fDist)
{
	m_fZoom = fDist;
}

void CCamera::SetZoomCenter(const Vector2& vCenter)
{
	m_vZoomCenter = vCenter;
}

void CCamera::SetZoomCenter(float fX, float fY)
{
	m_vZoomCenter = Vector2(fX, fY);
}

bool CCamera::IsInFrustum(const Vector3& vPos, float fRadius) const
{
	for (int i = 0; i < static_cast<int>(FRUSTUM_PLANE::END); ++i)
	{
		if (XMPlaneDotCoord(m_pPlanes[i].Convert(), vPos.Convert()).m128_f32[0]> fRadius)
		{
			return false;
		}
	}

	return true;
}

const RectInfo& CCamera::GetRect() const
{
	return m_tRect;
}

bool CCamera::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CCamera::Start()
{
	CSceneComponent::Start();

	GET_SINGLE(CCameraManager)->SetMainCam(this);
}

void CCamera::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (CAMERA_TYPE::CT_2D == m_eType)
	{
		Vector3 vPos = GetWorldPos();

		if (m_bZoom)
		{
			if (m_fZoomLimit > m_fZoom)
			{
				m_fZoom += fTime;

				m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(
					(m_tRect.fL + (m_vZoomCenter.x - m_tRect.fL) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fR + (m_vZoomCenter.x - m_tRect.fR) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fB + (m_vZoomCenter.y - m_tRect.fB) * m_fZoom) * m_tRS.iHeight,
					(m_tRect.fT + (m_vZoomCenter.y - m_tRect.fT) * m_fZoom) * m_tRS.iHeight,
					-1000.f, m_fDist);
			}

			else if (m_fZoomLimit < m_fZoom)
			{
				m_fZoom = m_fZoomLimit;

				m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(
					(m_tRect.fL + (m_vZoomCenter.x - m_tRect.fL) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fR + (m_vZoomCenter.x - m_tRect.fR) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fB + (m_vZoomCenter.y - m_tRect.fB) * m_fZoom) * m_tRS.iHeight,
					(m_tRect.fT + (m_vZoomCenter.y - m_tRect.fT) * m_fZoom) * m_tRS.iHeight,
					-1000.f, m_fDist);
			}

			vPos += GetPivot() * Vector3((float)m_tRS.iWidth, (float)m_tRS.iHeight, 0.f);

			m_vPos.z = m_fZoom;

			Vector3 vMove = m_vPos - vPos + Vector3((float)m_tRS.iWidth, (float)m_tRS.iHeight, 0.f) * GetPivot();

			float fLength = vMove.Length();

			if (fLength > 500.f)
			{
				vMove /= fLength;

				SetWorldPos(m_vPos);
			}

			else if (fLength > 5.f)
			{
				vMove /= fLength;

				AddWorldPos(vMove * fTime * 500.f);
			}

			else
			{
				AddWorldPos(vMove * fTime * 5.f);
			}
		}

		else
		{
			if (m_fZoom > 0.f)
			{
				m_fZoom -= fTime;

				m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(
					(m_tRect.fL + (m_vZoomCenter.x - m_tRect.fL) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fR + (m_vZoomCenter.x - m_tRect.fR) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fB + (m_vZoomCenter.y - m_tRect.fB) * m_fZoom) * m_tRS.iHeight,
					(m_tRect.fT + (m_vZoomCenter.y - m_tRect.fT) * m_fZoom) * m_tRS.iHeight,
					-1000.f, m_fDist);
			}

			else if (m_fZoom < 0.f)
			{
				m_fZoom = 0.f;

				m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(
					(m_tRect.fL + (m_vZoomCenter.x - m_tRect.fL) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fR + (m_vZoomCenter.x - m_tRect.fR) * m_fZoom) * m_tRS.iWidth,
					(m_tRect.fB + (m_vZoomCenter.y - m_tRect.fB) * m_fZoom) * m_tRS.iHeight,
					(m_tRect.fT + (m_vZoomCenter.y - m_tRect.fT) * m_fZoom) * m_tRS.iHeight,
					-1000.f, m_fDist);
			}

			vPos += GetPivot() * Vector3((float)m_tRS.iWidth, (float)m_tRS.iHeight, 0.f);

			if (m_pTarget)
			{
				Vector3 vTarPos = m_pTarget->GetWorldPos();

				vTarPos.z = vPos.z;

				if (m_pFocus)
				{
					Vector3 vFocusPos = m_pFocus->GetWorldPos();

					vFocusPos.z = vPos.z;

					Vector3 vMidPos = (vFocusPos + vTarPos) / 2.f;

					float fDist = vFocusPos.Distance(vTarPos);

					if (fDist >= m_fMinRange && fDist <= m_fMaxRange)
					{
						Vector3 vNew = vTarPos + (vMidPos - vTarPos) *
							(m_fMaxRange - fDist) / (m_fMaxRange - m_fMinRange);

						m_vPos = vNew;
					}

					else if (fDist < m_fMinRange)
					{
						m_vPos = vMidPos;

						if (m_pCallBack)
						{
							m_pCallBack(fTime);
						}
					}

					else
					{
						if (vTarPos.z < -500.f)
							vTarPos.z = -500.f;

						m_vPos = vTarPos;
					}
				}

				else
				{
					if (vTarPos.z < -500.f)
						vTarPos.z = -500.f;

					m_vPos = vTarPos;
				}

				m_vPos.z = -150.f;

				Vector3 _vPos = m_vPos - vPos + Vector3((float)m_tRS.iWidth, (float)m_tRS.iHeight, 0.f) * GetPivot();

				float fLength = _vPos.Length();

				if (fLength <= 50.f)
				{
					AddWorldPos(_vPos * fTime * 5.f);
				}

				else if (fLength < 500.f)
				{
					_vPos /= fLength;

					AddWorldPos(_vPos * fTime * 500.f);
				}

				else
				{
					_vPos /= fLength;

					SetWorldPos(vTarPos.x, vTarPos.y, m_vPos.z);
				}
			}
		}
	}
}

void CCamera::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	m_matView.Identity();

	Vector3 vPos = GetWorldPos();

	if (m_eType == CAMERA_TYPE::CT_2D)
		vPos -= GetPivot() * Vector3((float)m_tRS.iWidth, (float)m_tRS.iHeight, 0.f);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		Vector3 vAxis = GetWorldAxis(WORLD_AXIS(i));

		memcpy(&m_matView.v[i], &vAxis, sizeof(float) * 3);

		m_matView.v[i].w = vPos.Dot(vAxis) * -1.f;
	}

	m_matView.Transpose();

	Vector3 vPoints[8] = {};

	vPoints[0] = Vector3(-1.f, 1.f, 0.f);
	vPoints[1] = Vector3(-1.f, -1.f, 0.f);
	vPoints[2] = Vector3(1.f, 1.f, 0.f);
	vPoints[3] = Vector3(1.f, -1.f, 0.f);
	vPoints[4] = Vector3(-1.f, 1.f, 1.f);
	vPoints[5] = Vector3(-1.f, -1.f, 1.f);
	vPoints[6] = Vector3(1.f, 1.f, 1.f);
	vPoints[7] = Vector3(1.f, -1.f, 1.f);

	m_matVP = m_matView * m_matProj;
	Matrix matInvViewProj = m_matVP;
	matInvViewProj.Inverse();

	vPoints[0] = vPoints[0].TransformCoord(matInvViewProj);
	vPoints[1] = vPoints[1].TransformCoord(matInvViewProj);
	vPoints[2] = vPoints[2].TransformCoord(matInvViewProj);
	vPoints[3] = vPoints[3].TransformCoord(matInvViewProj);
	vPoints[4] = vPoints[4].TransformCoord(matInvViewProj);
	vPoints[5] = vPoints[5].TransformCoord(matInvViewProj);
	vPoints[6] = vPoints[6].TransformCoord(matInvViewProj);
	vPoints[7] = vPoints[7].TransformCoord(matInvViewProj);

	m_pPlanes[static_cast<int>(FRUSTUM_PLANE::_LEFT)] =
		XMPlaneFromPoints(vPoints[0].Convert(), vPoints[1].Convert(), vPoints[5].Convert());
	m_pPlanes[static_cast<int>(FRUSTUM_PLANE::_RIGHT)] =
		XMPlaneFromPoints(vPoints[3].Convert(), vPoints[2].Convert(), vPoints[6].Convert());
	m_pPlanes[static_cast<int>(FRUSTUM_PLANE::_TOP)] =
		XMPlaneFromPoints(vPoints[2].Convert(), vPoints[0].Convert(), vPoints[4].Convert());
	m_pPlanes[static_cast<int>(FRUSTUM_PLANE::_BOTTOM)] =
		XMPlaneFromPoints(vPoints[1].Convert(), vPoints[3].Convert(), vPoints[7].Convert());
	m_pPlanes[static_cast<int>(FRUSTUM_PLANE::_NEAR)] =
		XMPlaneFromPoints(vPoints[0].Convert(), vPoints[2].Convert(), vPoints[3].Convert());
	m_pPlanes[static_cast<int>(FRUSTUM_PLANE::_FAR)] =
		XMPlaneFromPoints(vPoints[6].Convert(), vPoints[4].Convert(), vPoints[5].Convert());
}

void CCamera::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CCamera::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CCamera::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CCamera::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CCamera* CCamera::Clone()
{
	return new CCamera(*this);
}

void CCamera::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_eType, 4, 1, pFile);
	fwrite(&m_fDist, 4, 1, pFile);
	fwrite(&m_fAngle, 4, 1, pFile);
}

void CCamera::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_eType, 4, 1, pFile);
	fread(&m_fDist, 4, 1, pFile);
	fread(&m_fAngle, 4, 1, pFile);

	SetCameraType(m_eType);
}
