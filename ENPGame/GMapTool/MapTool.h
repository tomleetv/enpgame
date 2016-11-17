
// MapTool.h : MapTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "GCoreLibV2.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "GMapCore32d.lib" )
#else
#pragma comment( lib, "GMapCore32.lib" )
#endif


//#include "map/GNoiseMap.h"
//#include "map/GQuadTreeIndex.h"
//#include "map/GMapObject.h"
#include "map/GMap.h"
#include "map/GQuadTree.h"
#include "GCamera.h"



// CMapToolApp:
// 이 클래스의 구현에 대해서는 MapTool.cpp을 참조하십시오.
//

class CMapToolApp : public CWinAppEx, public GCoreLibV2
{
public:
	shared_ptr<GCamera >	m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// 맵
	//--------------------------------------------------------------------------------------
	GMap	 m_CustomMap;
	TMapDesc m_MapDesc;
	//--------------------------------------------------------------------------------------
	// 쿼드트리
	//--------------------------------------------------------------------------------------
	GQuadTree m_QuadTree;
	
	//--------------------------------------------------------------------------------------
	// 디버깅 용도
	//--------------------------------------------------------------------------------------
	GLineShape		m_DrawLine;
	bool			m_bDebugRender;
	int				m_iDrawDepth;
	ComPtr<ID3D11PixelShader>   m_pPixelShader;// 프로스텀 전용 픽쉘쉐이더
public:
	bool			CreateInit(int Width, int Height, float Distance, CString strTex);
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
	
	bool			DrawQuadLine(GNode* pNode);
	bool			DrawDebug();
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT			CreateResource();
	HRESULT			DeleteResource();
	
public:
	CMapToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CMapToolApp theApp;
