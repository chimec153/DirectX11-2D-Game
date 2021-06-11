#pragma once

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define SAFE_DELETE(p)	if(p)	{delete p; p=nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p)	{delete[] p; p=nullptr;}

#define DECLARE_SINGLE(Type)	\
	private:\
		static Type* m_pInst;\
	public:\
		static Type* GetInst()	\
		{\
			if(!m_pInst)\
				m_pInst = new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()	\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();

#define DEFINITION_SINGLE(Type)	Type* Type::m_pInst = nullptr;
#define GET_SINGLE(Type)	Type::GetInst()
#define DESTROY_SINGLE(Type)	Type::DestroyInst()

#define SAFE_RELEASE(p)	if(p)	{p->Release();p=nullptr;}

#define SAFE_DELETE_VECLIST(p)	\
	{\
		auto iter = p.begin();\
		auto iterEnd = p.end();\
		for(;iter!=iterEnd;++iter)\
			SAFE_DELETE((*iter));\
		p.clear();\
	}

#define SAFE_DELETE_ARRAY_VECLIST(p)	\
	{\
		auto iter = p.begin();\
		auto iterEnd = p.end();\
		for(;iter!=iterEnd;++iter)\
			SAFE_DELETE_ARRAY((*iter));\
		p.clear();\
	}

#define SAFE_DELETE_MAP(p)	\
	{\
		auto iter = p.begin();\
		auto iterEnd = p.end();\
		for(;iter!=iterEnd;++iter)\
			SAFE_DELETE(iter->second);\
		p.clear();\
	}

#define SAFE_RELEASE_VECLIST(p)	\
	{\
		auto iter = p.begin();\
		auto iterEnd = p.end();\
		for(;iter!=iterEnd;++iter)\
			SAFE_RELEASE((*iter));\
		p.clear();\
	}

#define SAFE_RELEASE_MAP(p)	\
	{\
		auto iter = p.begin();\
		auto iterEnd = p.end();\
		for(;iter!=iterEnd;++iter)\
			SAFE_RELEASE(iter->second);\
		p.clear();\
	}

template <typename T>
void Safe_Delete_VecList(T& p)
{
	typename T::iterator iter = p.begin();
	typename T::iterator iterEnd = p.end();

	for (; iter != iterEnd; ++iter)
		SAFE_DELETE((*iter));

	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	typename T::iterator iter = p.begin();
	typename T::iterator iterEnd = p.end();

	for (; iter != iterEnd; ++iter)
		SAFE_RELEASE((*iter));

	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	typename T::iterator iter = p.begin();
	typename T::iterator iterEnd = p.end();

	for (; iter != iterEnd; ++iter)
		SAFE_DELETE(iter->second);

	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	typename T::iterator iter = p.begin();
	typename T::iterator iterEnd = p.end();

	for (; iter != iterEnd; ++iter)
		SAFE_RELEASE(iter->second);

	p.clear();
}

template <typename T>
void Safe_Delete_Array_List(T& p)
{
	typename T::iterator iter = p.begin();
	typename T::iterator iterEnd = p.end();

	for (; iter != iterEnd; ++iter)
		SAFE_DELETE_ARRAY((*iter));

	p.clear();
}

#define RESOURCE_PATH	"Resource_Path"
#define DATA_TEXTURE_PATH	"Data_Texture_Path"
#define DATA_PATH		"Data_Path"
#define ROOT_PATH		"Root_Path"
#define SOUND_PATH		"Sound_Path"
#define SHADER_PATH		"Shader_Path"
#define MAP_PATH		"Map_Path"
#define	MESH_PATH		"Mesh_Path"
#define TEXTURE_PATH	"Texture_Path"

#define DEVICE			GET_SINGLE(CDevice)->GetDevice()
#define CONTEXT			GET_SINGLE(CDevice)->GetContext()
#define SWAPCHAIN		GET_SINGLE(CDevice)->GetSwapChain()
#define RESOLUTION		GET_SINGLE(CDevice)->GetResolution()
#define RATIO			GET_SINGLE(CDevice)->GetRatio()
#define FACTORY2D		GET_SINGLE(CDevice)->GetFactory2D()
#define RENDERTARGET2D	GET_SINGLE(CDevice)->GetRenderTarget2D()

#define WINDOWHANDLE			GET_SINGLE(CEngine)->GetHandle()

#define	ALPHA_BLEND	"AlphaBlend"
#define LIGHT_BLEND	"LightAccBlend"
#define NO_DEPTH	"DepthNoWrite"
#define DEPTH_READ	"DepthReadNoWrite"
#define WIRE_FRAME	"WireFrame"
#define CULL_NONE	"NoCullBack"
#define CULL_FRONT	"CullFront"

#define TERRAIN_LAYER	"Terrain_Layer"
#define CUBEMAP_LAYER	"Cubemap_Layer"
#define ALBEDO_LAYER	"Albedo_Layer"
#define DECAL_LAYER		"Decal_Layer"	
#define PARTICLE_LAYER	"Particle_Layer"
#define COLLIDER_LAYER	"Collider_Layer"
#define ALPHA_LAYER		"Alpha_Layer"	
#define UI_LAYER		"UI_Layer"