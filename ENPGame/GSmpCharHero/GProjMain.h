#pragma once
#include "GCoreLibV2.h"
#include "GZombie.h"
#include "GHero.h"


//오브젝트 위치
#define G_OBJ_LOC_GUN_SHOTGUN _T("data/object/shotgun/shotgun.GBS")
//오브젝트 매니저 등록 이름
#define G_OBJ_NAME_GUN_SHOTGUN L"shotgun.GBS"



//#define G_MACRO_MODELVIEW 1

#ifdef G_MACRO_MODELVIEW
#include "GModelViewCamera.h"
#else
#include "GCamera.h"
#endif

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "GCharCore32d.lib" )	
#else
#pragma comment( lib, "GCharCore32.lib" )
#endif

class GProjMain : public GCoreLibV2
{
#ifdef G_MACRO_MODELVIEW
	shared_ptr<GModelViewCamera > m_pMainCamera;
#else
	shared_ptr<GCamera > m_pMainCamera;
#endif
	/*
	GModel*		m_ObjGun;
	D3DXMATRIX	m_matObjGunWld, m_matObjGunScl, m_matObjGunRot, m_matObjGunTrans;
	*/
	D3DXMATRIX  m_matWorld;
	vector<shared_ptr<GHero>>	m_HeroObj;
public:	
	bool		Init();
	bool		Frame();
	bool		Render();	
	bool		Release();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//----------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//----------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	//--------------------------------------------------------------------------------------
	// 파일 선택하여 로드( 단축기 : O )
	//--------------------------------------------------------------------------------------
	bool		Load();
public:
	GProjMain(void);
	virtual ~GProjMain(void);
};

extern GProjMain*	g_pMain; //외부 연결 범위 선언

