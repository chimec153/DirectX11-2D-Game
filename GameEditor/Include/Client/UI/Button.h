#pragma once

#include "UI/UIObject.h"

	class CClientButton :
		public CObj
	{
		friend class CScene;
		friend class CViewPort;

	private:
		CClientButton();
		CClientButton(const CClientButton& ui);
		virtual ~CClientButton();

	private:
		class CUIButton*		m_pBtn;
		class CPanel*			m_pPanel;

	public:
		virtual bool Init();
		virtual void Start();
		virtual void Update(float fTime);
		virtual void PostUpdate(float fTime);
		virtual void Collision(float fTime);
		virtual void PreRender(float fTime);
		virtual void Render(float fTime);
		virtual void PostRender(float fTime);
		virtual CClientButton* Clone();

	public:
		virtual void Save(FILE* pFile);
		virtual void Load(FILE* pFile);

	public:
		void Close(float);
	};


