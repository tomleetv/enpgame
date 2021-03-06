#pragma once

#include <atlstr.h>

#include "map/GMap.h"
#include "map/GQuadTree.h"
#include "GMapMgr.h"
#include "GInput.h"
#include "map/GTileMap.h"
#include "GModel.h"
#include "GGbsObj.h"
#include "GObjMgr.h"
#include "tchar.h"

#define G_SHA_OBJ_DIFFUSE L"data/shader/Obj_Diffuse.hlsl"
#define G_SHA_OBJ_DIFFUSE_REVERSE L"data/shader/Obj_Diffuse_Rev.hlsl"

#define G_SHA_OBJ_DIFFUSE_SHADOW L"data/shader_shadow/Obj_Diffuse_shadow.hlsl"
#define G_SHA_OBJ_DIFFUSE_SHADOW_REVERSE L"data/shader_shadow/Obj_Diffuse_Rev_shadow.hlsl"

#define G_SHA_OBJ_SPECULAR L"data/shader/Obj_Specular.hlsl"
#define G_SHA_OBJ_SPECULAR_REVERSE L"data/shader/Obj_Specular_Rev.hlsl"

#define G_SHA_OBJ_SPECULAR_SHADOW L"data/shader_shadow/Obj_Specular_shadow.hlsl"
#define G_SHA_OBJ_SPECULAR_SHADOW_REVERSE L"data/shader_shadow/Obj_Specular_Rev_shadow.hlsl"

#define G_SHA_MAP_DIFFUSE_SHADOW L"data/shader_shadow/CustomizeMap_shadow.hlsl"

#define G_DEFINE_MAP_DIR L"data/map/"
#define G_DEFINE_MAP_SHADER L"data/shader/CustomizeMap_light.hlsl"

#define G_DEFINE_STAGE_FENCE_OBJ_NAME L"data\\object\\stage_fence\\stage_fence.GBS"

class GObjData {
public:
	TCHAR			m_strName[MAX_PATH];
	GGbsObj*		m_pObj;
	D3DXMATRIX		m_matObjWld;
	//D3DXMATRIX	m_matObjScl;
	//D3DXMATRIX	m_matObjRot;
	D3DXMATRIX		m_matObjTrans;
	float			m_fScl;
	float			m_fRotY;
	bool			m_bLightReverse;
	bool			m_bLightSpecular;

	GObjData() {
		m_bLightReverse = true;
		m_bLightSpecular = false;
		D3DXMatrixIdentity(&m_matObjWld);
		D3DXMatrixIdentity(&m_matObjTrans);
		m_fScl = 1.0;
		m_fRotY = 0.0f;
	};
	~GObjData() {};
};

class GMapGroup {
public:
	TCHAR			m_strTex[MAX_PATH];
	TCHAR			m_strHeight[MAX_PATH];


	int				m_iDrawDepth;
	//--------------------------------------------------------------------------------------
	// ����� �뵵
	//--------------------------------------------------------------------------------------
	GLineShape		m_DrawLine;
	//--------------------------------------------------------------------------------------
	// ��
	//--------------------------------------------------------------------------------------
	//GMap	 m_CustomMap;
	TMapDesc m_MapDesc;
	//--------------------------------------------------------------------------------------
	// ����Ʈ��
	//--------------------------------------------------------------------------------------
	GQuadTree m_QuadTree;

	GTileMap		m_HeightMap;
	vector<shared_ptr<GObjData>>	m_vecObj;
	vector<bool>	m_vecObjRender;//���� ���� �ؼ� 1�̸� ���� 0�̸� ���� ����.

	bool			DrawQuadLine(GNode* pNode, GCamera* pCamera);
	bool			DrawDebug(GCamera* pCamera);
	bool			CreateInit(GCoreLibV2* pMain, int Width, int Height, float Distance, TCHAR* pStr, GCamera* pCamera);


	bool			Init();
	bool			Frame(GCamera* pCamera, bool bDebug,GInput* pInput = NULL);
	bool			Render(GCamera* pCamera, bool bDebug, GCoreLibV2* pMain = NULL);
	bool			Release();
	HRESULT			CreateResource();
	HRESULT			DeleteResource();

	GMapGroup() {
		m_iDrawDepth = 0;
	};
	~GMapGroup() {};
};
