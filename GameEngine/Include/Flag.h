#pragma once

enum class WORLD_AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_END
};

enum class SHADER_TYPE
{
	ST_COMPUTE,
	ST_GRAPHIC
};

enum class COMPONENT_TYPE
{
	CT_SCENE,
	CT_OBJECT
};

enum class SHADER_CBUFFER_TYPE
{
	CBUFFER_VERTEX = 0x1,
	CBUFFER_PIXEL = 0x2,
	CBUFFER_HULL = 0x4,
	CBUFFER_DOMAIN = 0x8,
	CBUFFER_GEOMETRY = 0x10,
	CBUFFER_COMPUTE = 0x20
};

enum class SCENE_COMPONENT_TYPE
{
	SCT_2D,
	SCT_3D,
	SCT_UI,
};

enum class OBJ_CLASS_TYPE
{
	OCT_PLAYER,
	OCT_BULLET
};

enum class GAMEMODE_CLASS_TYPE
{
	GCT_MAIN
};

enum class SCENECOMPONENT_CLASS_TYPE
{
	SCT_MESH2D,
	SCT_STATICMESH,
	SCT_CAMERA,
	SCT_SPRITE,
	SCT_COLLIDERRECT,
	SCT_COLLIDERLINE,
	SCT_COLLIDERCIRCLE,
	COLLIDEROBB2D,
	COLLIDERPIXEL,
	COLLIDER_POINT,
	UI_CONTROL,
	UI_IMAGE,
	UI_SPRITE,
	UI_BUTTON,
	UI_BAR,
	UI_TITLEBAR,
	UI_PANEL,
	SOUND,
	SCT_END
};

enum class CAMERA_TYPE
{
	CT_3D,
	CT_2D,
	CT_UI
};

enum class IMAGE_TYPE
{
	IT_ATLAS,
	IT_ARRAY
};

enum class REGISTER_TYPE
{
	RT_DIF,
	RT_NRM,
	RT_SPE,
	RT_CUSTOM
};

enum class SCENE_TYPE
{
	SC_CURRENT,
	SC_NEXT,
	SC_END
};

enum class COLLIDER_TYPE
{
	CT_RECT,
	CT_SPHERE,
	CT_OBB,
	CT_PIXEL,
	CT_POINT,
	CT_LINE,
	CT_END
};

enum class MESH_TYPE
{
	MT_2D,
	MT_STATIC,
	MT_ANIMATION
};

enum class COLLISION_CHANNEL
{
	BASE,
	PLAYER,
	MONSTER,
	NPC,
	CUSTOM1,
	CUSTOM2,
	CUSTOM3,
	CUSTOM4,
	CUSTOM5,
	CUSTOM6,
	CUSTOM7,
	CUSTOM8,
	CUSTOM9,
	CUSTOM10,
	CUSTOM11,
	CUSTOM12,
	CUSTOM13,
	CUSTOM14,
	CUSTOM15,
	CUSTOM16,
	CUSTOM17,
	CUSTOM18,
	CUSTOM19,
	CUSTOM20,
	END
};

enum class COLLISION_TYPE
{
	BLK,
	IGN
};

enum class COLLISION_STATE
{
	INIT,
	STAY,
	END,
	MAX
};

enum class DIR_8
{
	U,
	RU,
	R,
	RD,
	D,
	LD,
	L,
	LU,
	END
};

enum class PIXEL_COLLISION_TYPE
{
	COLOR_IGN,
	LESS_EQUAL_ALPHA,
	COLOR_CONFIRM
};

enum class BUTTON_STATE
{
	OFF,
	ON,
	PUSH,
	DISABLE,
	END
};

enum class SOUND_TYPE
{
	EFFECT,
	BGM,
	END
};

enum class TILE_TYPE
{
	RECT,
	ISOMETRIC,
	END
};

enum class TILE_OPTION
{
	NONE,
	NOMOVE,
	END
};

enum class TEXT_ALIGN_VERTICAL
{
	TOP,
	MID,
	BOT,
	END
};

enum class TEXT_ALIGN_HORISONTAL
{
	LEFT,
	CENTER,
	RIGHT,
	END
};

enum class TILE_TEX_TYPE
{
	TEX,
	COL,
	END
};