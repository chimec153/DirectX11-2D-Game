#include "Ref.h"

CRef::CRef()	:
	m_iRef(1),
	m_bActive(true),
	m_bEnable(true)
{
}

CRef::CRef(const CRef& ref)
{
	*this = ref;

	m_iRef = 1;
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_iRef;
}

int CRef::Release()
{
	--m_iRef;

	if (m_iRef == 0)
	{
		delete this;
		return 0;
	}

	return m_iRef;
}

void CRef::Save(FILE* pFile)
{
	int iLength = (int)m_strName.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strName.c_str(), 1, iLength, pFile);
	fwrite(&m_bEnable, 1, 1, pFile);
}

void CRef::Load(FILE* pFile)
{
	m_iRef = 1;
	m_bActive = true;
	
	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	if (iLength > 0)
	{
		fread(strTag, 1, iLength, pFile);
	}

	m_strName = strTag;

	fread(&m_bEnable, 1, 1, pFile);
}

void CRef::SpawnWindow()
{
	const char* pText = GetName().c_str();

	if (!strcmp(pText, ""))
	{
		pText = "None";
	}

	if (ImGui::Begin(pText))
	{
		ImGui::Checkbox("Enable", &m_bEnable);
	}

	ImGui::End();
}
