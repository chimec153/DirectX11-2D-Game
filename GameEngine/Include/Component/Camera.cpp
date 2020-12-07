#include "Camera.h"
#include "../Device.h"
#include "../Camera/CameraManager.h"
#include "../Scene/Scene.h"

CCamera::CCamera()	:
	m_fAngle(90.f),
	m_fDist(3000.f),
	m_bZoom(false),
	m_pTarget(nullptr),
	m_pFocus(nullptr),
	m_fMaxRange(0.f),
	m_fMinRange(0.f),
	m_vPos()
{
	SetCameraType(CAMERA_TYPE::CT_3D);
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA;
}

CCamera::CCamera(const CCamera& cam)	:
	CSceneComponent(cam)
{
	m_matView = cam.m_matView;
	m_matProj = cam.m_matProj;
	m_eType = cam.m_eType;
	m_fDist = cam.m_fDist;
	m_fAngle = cam.m_fAngle;
	m_pTarget = cam.m_pTarget;
	m_pFocus = cam.m_pFocus;
	m_fMaxRange = cam.m_fMaxRange;
	m_fMinRange = cam.m_fMinRange;
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pFocus);
}

Matrix CCamera::GetViewMat() const
{
	return m_matView;
}

Matrix CCamera::GetProjMat() const
{
	return m_matProj;
}

void CCamera::SetCameraType(CAMERA_TYPE eType)
{
	Resolution tRS = RESOLUTION;

	m_eType = eType;

	switch (m_eType)
	{
	case CAMERA_TYPE::CT_3D:
		m_matProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fAngle), tRS.iWidth / (float)tRS.iHeight, 0.3f, m_fDist);
		break;
	case CAMERA_TYPE::CT_2D:
	case CAMERA_TYPE::CT_UI:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)tRS.iWidth, 0.f, (float)tRS.iHeight, 0.f, m_fDist);
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

void CCamera::SetZoom(bool bZoom)
{
	m_bZoom = bZoom;
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

	Vector3 vPos = GetWorldPos();

	if (m_bZoom)
	{
		Resolution tRS = RESOLUTION;

		vPos += GetPivot() * Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);

		m_vPos.z = -100.f;

		Vector3 vMove = m_vPos - vPos + Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * GetPivot();

		float fLength = vMove.Length();

		if (fLength > 5.f)
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
		Resolution tRS = RESOLUTION;

		vPos += GetPivot() * Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);

		if (m_pTarget)
		{
			Resolution tRS = RESOLUTION;

			Vector3 vTarPos = m_pTarget->GetWorldPos();

			vTarPos.z = vPos.z;

			if (m_pFocus)
			{
				Vector3 vFocusPos = m_pFocus->GetWorldPos();

				vFocusPos.z = vPos.z;

				float fDist = vFocusPos.Distance(vTarPos);

				if (fDist >= m_fMinRange && fDist <= m_fMaxRange)
				{
					Vector3 vNew = vTarPos + (vFocusPos - vTarPos)* (m_fMaxRange - fDist) / (m_fMaxRange - m_fMinRange);

						SetWorldPos(vNew);
					//m_vPos = vFocusPos;
				}

				else if (fDist < m_fMinRange)
				{
					m_vPos = vFocusPos;
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

			Vector3 _vPos = m_vPos - vPos + Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * GetPivot();

			float fLength = _vPos.Length();

			if (fLength <= 50.f)
			{
				AddWorldPos(_vPos * fTime * 5.f);
			}

			else
			{
				_vPos /= fLength;

				AddWorldPos(_vPos * fTime * 500.f);
			}
		}
	}

}

void CCamera::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	Resolution tRS = RESOLUTION;

	m_matView.Identity();

	Vector3 vPos = GetWorldPos();

	if (m_eType == CAMERA_TYPE::CT_2D)
		vPos -= GetPivot() * Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f);

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_END; ++i)
	{
		Vector3 vAxis = GetWorldAxis(WORLD_AXIS(i));

		memcpy(&m_matView.v[i], &vAxis, sizeof(float) * 3);

		m_matView.v[i].w = vPos.Dot(vAxis) * -1.f;
	}

	m_matView.Transpose();
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
