#include "ParticleSystem.h"
#include "../Device.h"
#include "ComputeShader.h"

CParticleSystem::CParticleSystem()	:
	m_bDynamic(false)
	, m_tDesc()
	, m_pBuffer(nullptr)
	, m_iCount(0)
	, m_iSize(0)
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_iRegister(0)
	, m_iType(0)
	, m_tCBuffer()
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& system)	:
	CRef(system)
	, m_bDynamic(system.m_bDynamic)
	, m_tDesc(system.m_tDesc)
	, m_iCount(system.m_iCount)
	, m_iSize(system.m_iSize)
	, m_iRegister(system.m_iRegister)
	, m_iType(system.m_iType)
	, m_tCBuffer(system.m_tCBuffer)
{
	m_pBuffer = nullptr;
	m_pSRV = nullptr;
	m_pUAV = nullptr;

	Init(m_bDynamic, m_iCount, m_iSize, m_iRegister, m_iType);
}

CParticleSystem::~CParticleSystem()
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
}

bool CParticleSystem::Init(bool bDynamic, int iCount,int iSize, int iRegister, int iType)
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);

	m_iCount = iCount;
	m_bDynamic = bDynamic;
	m_iSize = iSize;
	m_iRegister = iRegister;
	m_iType = iType;

	if (m_bDynamic)
	{
		m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	else
	{
		m_tDesc.Usage = D3D11_USAGE_DEFAULT;
		m_tDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	}

	m_tDesc.ByteWidth = iSize * iCount;
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_tDesc.StructureByteStride = iSize;

	if (FAILED(DEVICE->CreateBuffer(&m_tDesc, nullptr, &m_pBuffer)))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = iCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBuffer, &tSRVDesc, &m_pSRV)))
	{
		return false;
	}

	if (!bDynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};

		tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tUAVDesc.Buffer.FirstElement = 0;
		tUAVDesc.Buffer.Flags = 0;
		tUAVDesc.Buffer.NumElements = iCount;

		if (FAILED(DEVICE->CreateUnorderedAccessView(m_pBuffer, &tUAVDesc, &m_pUAV)))
		{
			return false;
		}
	}

	return true;
}

void CParticleSystem::SetShader(int iRegister)
{
	if (m_bDynamic)
	{
		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		{
			CONTEXT->VSSetShaderResources(iRegister, 1, &m_pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
		{
			CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
		{
			CONTEXT->HSSetShaderResources(iRegister, 1, &m_pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
		{
			CONTEXT->DSSetShaderResources(iRegister, 1, &m_pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
		{
			CONTEXT->GSSetShaderResources(iRegister, 1, &m_pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
		{
			CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
		}

	}
	else
	{
			UINT iCount = 0;

			CONTEXT->CSSetUnorderedAccessViews(iRegister, 1, &m_pUAV, &iCount);
	}
}

void CParticleSystem::ResetShader(int iRegister)
{
	if (m_bDynamic)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		{
			CONTEXT->VSSetShaderResources(iRegister , 1, &pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
		{
			CONTEXT->PSSetShaderResources(iRegister , 1, &pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
		{
			CONTEXT->HSSetShaderResources(iRegister, 1, &pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
		{
			CONTEXT->DSSetShaderResources(iRegister, 1, &pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
		{
			CONTEXT->GSSetShaderResources(iRegister, 1, &pSRV);
		}

		if (m_iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
		{
			CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
		}
	}

	else
	{
		ID3D11UnorderedAccessView* pUAV = nullptr;

		UINT iCount = 0;

		CONTEXT->CSSetUnorderedAccessViews(iRegister, 1,&pUAV, &iCount);
	}
}

void CParticleSystem::SetShader(int iRegister, int iType)
{
	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
	{
		CONTEXT->VSSetShaderResources(iRegister, 1, &m_pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
	{
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
	{
		CONTEXT->HSSetShaderResources(iRegister, 1, &m_pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
	{
		CONTEXT->DSSetShaderResources(iRegister, 1, &m_pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
	{
		CONTEXT->GSSetShaderResources(iRegister, 1, &m_pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
	{
		CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);
	}
}

void CParticleSystem::ResetShader(int iRegister, int iType)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
	{
		CONTEXT->VSSetShaderResources(iRegister, 1, &pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
	{
		CONTEXT->PSSetShaderResources(iRegister, 1, &pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
	{
		CONTEXT->HSSetShaderResources(iRegister, 1, &pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
	{
		CONTEXT->DSSetShaderResources(iRegister, 1, &pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
	{
		CONTEXT->GSSetShaderResources(iRegister, 1, &pSRV);
	}

	if (iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
	{
		CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
	}
}

CParticleSystem* CParticleSystem::Clone()
{
	return new CParticleSystem(*this);
}

void CParticleSystem::SetStartColor(const Vector4& vColor)
{
	m_tCBuffer.vStartColor = vColor;
}

void CParticleSystem::SetEndColor(const Vector4& vColor)
{
	m_tCBuffer.vEndColor = vColor;
}

void CParticleSystem::SetStartScale(const Vector3& vScale)
{
	m_tCBuffer.vStartScale = vScale;
}

void CParticleSystem::SetEndScale(const Vector3& vScale)
{
	m_tCBuffer.vEndScale = vScale;
}

void CParticleSystem::SetMinSpeed(float fSpeed)
{
	m_tCBuffer.fMinSpeed = fSpeed;
}

void CParticleSystem::SetMaxSpeed(float fSpeed)
{
	m_tCBuffer.fMaxSpeed = fSpeed;
}

void CParticleSystem::SetMoveDir(const Vector3& vDir)
{
	m_tCBuffer.vMoveDir = vDir;
}

void CParticleSystem::SetMoveAngle(const Vector3& vAngle)
{
	m_tCBuffer.vMoveAngle = vAngle;
}

void CParticleSystem::Set2D(bool b2D)
{
	m_tCBuffer.i2D = b2D;
}

void CParticleSystem::SetMove(bool bMove)
{
	m_tCBuffer.iMove = bMove;
}

void CParticleSystem::SetMaxLifeTime(float fTime)
{
	m_tCBuffer.fMaxLifeTime = fTime;
}

void CParticleSystem::SetMinLifeTime(float fTime)
{
	m_tCBuffer.fMinLifeTime = fTime;
}

void CParticleSystem::SetMaxCount(unsigned int iCount)
{
	m_tCBuffer.iMaxCount = iCount;
}

void CParticleSystem::SetRange(const Vector3& vRange)
{
	m_tCBuffer.vRange = vRange;
}

void CParticleSystem::SetAngle(float fRad)
{
	m_tCBuffer.fTurnAngle = fRad;
	//m_tCBuffer.vMoveAngle = Vector3(0.f, 0.f, 1.f);
}

void CParticleSystem::SetAnim(int iFrameMax, float fFrameTimeMax)
{
	m_tCBuffer.iFrameMax = iFrameMax;
	m_tCBuffer.fFrameTime = fFrameTimeMax;
}

void CParticleSystem::SetAccel(const Vector3& vAcc)
{
	m_tCBuffer.vAcc = vAcc;
}

void CParticleSystem::CloneCBuffer(PARTICLECBUFFER& tBuffer)
{
	tBuffer = m_tCBuffer;
}

void CParticleSystem::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_bDynamic, 1, 1, pFile);
	fwrite(&m_tDesc, sizeof(D3D11_BUFFER_DESC), 1, pFile);
	fwrite(&m_iCount, 4, 1, pFile);
	fwrite(&m_iSize, 4, 1, pFile);
	fwrite(&m_iRegister, 4, 1, pFile);
	fwrite(&m_iType, 4, 1, pFile);
	fwrite(&m_tCBuffer, sizeof(PARTICLECBUFFER), 1, pFile);
}

void CParticleSystem::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_bDynamic, 1, 1, pFile);
	fread(&m_tDesc, sizeof(D3D11_BUFFER_DESC), 1, pFile);
	fread(&m_iCount, 4, 1, pFile);
	fread(&m_iSize, 4, 1, pFile);
	fread(&m_iRegister, 4, 1, pFile);
	fread(&m_iType, 4, 1, pFile);
	fread(&m_tCBuffer, sizeof(PARTICLECBUFFER), 1, pFile);
	Init(m_bDynamic, m_iCount, m_iSize, m_iRegister, m_iType);
}
