#include "Transform.h"
#include "../Device.h"
#include "../Resource/ShaderManager.h"

CTransform::CTransform()	:
	m_pScene(nullptr),
	m_pOwner(nullptr),
	m_pParent(nullptr),
	m_bInheritScale(true),
	m_bInheritRotX(true),
	m_bInheritRotY(true),
	m_bInheritRotZ(true),
	m_bUpdateScale(true),
	m_bUpdateRot(true)
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	m_vMeshSize = Vector3(1.f, 1.f, 0.f);
}

CTransform::CTransform(const CTransform& transform)
{
	*this = transform;

	m_vecChild.clear();
	m_pScene = nullptr;
	m_pOwner = nullptr;
	m_pParent = nullptr;

	m_bUpdateScale = true;
	m_bUpdateRot = true;
}

CTransform::~CTransform()
{
}

Matrix CTransform::GetMatScale() const
{
	return m_matScale;
}

Matrix CTransform::GetMatRot() const
{
	return m_matRot;
}

Matrix CTransform::GetMatPos() const
{
	return m_matPos;
}

Matrix CTransform::GetMatWorld() const
{
	return m_matWorld;
}

void CTransform::Update(float fTime)
{
}

void CTransform::PostUpdate(float fTime)
{
	if (m_bUpdateScale)
		m_matScale.Scale(m_vWorldScale);

	if (m_bUpdateRot)
		m_matRot.Rotate(m_vWorldRot);

	m_matPos.Translation(m_vWorldPos);

	m_matWorld = m_matScale * m_matRot * m_matPos;
}

void CTransform::SetTransform()
{
	Resolution tRS = RESOLUTION;

	m_tCBuffer.matWorld = m_matWorld;
	m_tCBuffer.matView = DirectX::XMMatrixIdentity();
	m_tCBuffer.matProj = DirectX::XMMatrixOrthographicOffCenterLH(-tRS.iWidth / 2.f, tRS.iWidth/2.f,
		-tRS.iHeight / 2.f, tRS.iHeight/2.f , 0.f, 3000.f);
	m_tCBuffer.matWV = m_tCBuffer.matWorld * m_tCBuffer.matView;
	m_tCBuffer.matWVP = m_tCBuffer.matWV * m_tCBuffer.matProj;
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matView.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);
}

CTransform* CTransform::Clone()
{
	return new CTransform(*this);
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

Vector3 CTransform::GetRelativeScale() const
{
	return m_vRelativeScale;
}

Vector3 CTransform::GetRelativeRot() const
{
	return m_vRelativeRot;
}

Vector3 CTransform::GetRelativePos() const
{
	return m_vRelativePos;
}

Vector3 CTransform::GetRelativeAxis(AXIS axis) const
{
	return m_vRelativeAxis[axis];
}

void CTransform::InheritScale()
{
	if (m_pParent)
	{
		if (m_bInheritScale)
			m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();
	}

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::InheritRot()
{
	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

		InheritPos();
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis->Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis->Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::InheritPos()
{
	if (m_pParent)
	{
		Matrix tmat;

		tmat.Rotate(m_pParent->GetWorldRot());

		memcpy(&tmat._41, &m_pParent->GetWorldPos(), sizeof(float)*3);

		m_vWorldPos = m_vRelativePos.TransformCoord(tmat);
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
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

Vector3 CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

Vector3 CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

Vector3 CTransform::GetWorldPos() const
{
	return m_vWorldPos;
}

Vector3 CTransform::GetWorldAxis(AXIS axis) const
{
	return m_vWorldAxis[axis];
}

Vector3 CTransform::GetPivot() const
{
	return m_vPivot;
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

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
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

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CTransform::SetWorldPos(const Vector3& v)
{
	m_vVelocity += v - m_vWorldPos;

	m_vWorldPos = v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat;

		tmat.Rotate(m_pParent->GetWorldRot());

		memcpy(&tmat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CTransform::SetWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z) - m_vWorldPos;

	m_vWorldPos = Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat;

		tmat.Rotate(m_pParent->GetWorldRot());

		memcpy(&tmat._41, &m_pParent->GetWorldPos(), sizeof(float)*3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
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

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z) - m_vWorldRot;

	m_vWorldRot = Vector3(x, y, z);

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

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRotX(float x)
{
	m_vVelocityRot.x += x - m_vWorldRot.x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRotY(float y)
{
	m_vVelocityRot.y += y - m_vWorldRot.y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CTransform::SetWorldRotZ(float z)
{
	m_vVelocityRot.z += z - m_vWorldRot.z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
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

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritScale();
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

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritScale();
}

void CTransform::AddWorldPos(const Vector3& v)
{
	m_vVelocity += v;

	m_vWorldPos += v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat;

		tmat.Rotate(m_pParent->GetWorldRot());

		memcpy(&tmat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritPos();
}

void CTransform::AddWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x,y,z);

	m_vWorldPos += Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tmat;

		tmat.Rotate(m_pParent->GetWorldRot());

		memcpy(&tmat._41, &m_pParent->GetWorldPos(), sizeof(float)*3);

		tmat.Inverse();

		m_vRelativePos = m_vWorldPos.TransformCoord(tmat);
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritPos();
}

void CTransform::AddWorldRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vWorldRot += v;

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

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x,y,z);

	m_vWorldRot += Vector3(x, y, z);

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

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vWorldRot.x += x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vWorldRot.y += y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vWorldRot.z += z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix tmat;

	tmat.Rotate(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vRelativeAxis[i].Normalize();
	}

	tmat.Rotate(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(tmat);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->InheritRot();
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
