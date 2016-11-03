
// GciCharTool.h : GciCharTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "GCoreLibV2.h"
#include "GHeroObj.h"

#define G_MACRO_MODELVIEW 1

#ifdef G_MACRO_MODELVIEW
//#include "GModelViewCamera.h"
#include "GModelCamera.h"
#else
#include "GCamera.h"
#endif

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "GCharCore32d.lib" )	
#else
#pragma comment( lib, "GCharCore32.lib" )
#endif

// CAIToolApp:
// 이 클래스의 구현에 대해서는 GciCharTool.cpp을 참조하십시오.
//




class CGciCharToolApp : public CWinAppEx, public GCoreLibV2
{
public:
#ifdef G_MACRO_MODELVIEW
	BOOL				InitCamera();

	//--------------------------------------------------------------------------------------
	// ViewStyle
	//--------------------------------------------------------------------------------------
	T_STR ViewStyle[4] =
	{
		_T("Top g_matView"),
		_T("Front g_matView"),
		_T("Side g_matView"),
		_T("User g_matView"),
	};

	//shared_ptr<GModelViewCamera > m_pMainCamera;

	//GBoxShape					m_pBox;
	//GPlaneShape					m_pPlane;
	GDirectionLineShape			m_pDirectionLine;
	//--------------------------------------------------------------------------------------
	// 각종 지원 객체 
	//--------------------------------------------------------------------------------------
	shared_ptr<GModelCamera >			m_pCamera[4];
	shared_ptr<GModelCamera >			m_pMainCamera;
	//--------------------------------------------------------------------------------------
	// Matrix
	//--------------------------------------------------------------------------------------
	D3DXMATRIX					m_World[2];
	//--------------------------------------------------------------------------------------
	// 뷰포트 설정
	//--------------------------------------------------------------------------------------	
	GDxRT						m_ViewPort[4];
	HRESULT						ScreenViewPort(UINT iWidth, UINT iHeight);
	//--------------------------------------------------------------------------------------
	// 카메라 
	//--------------------------------------------------------------------------------------	
	UINT						m_iCameraType;
	//--------------------------------------------------------------------------------------
	// 카메라 회전 정보
	//--------------------------------------------------------------------------------------
	float						m_fCameraYaw;
	float						m_fCameraPitch;
	float						m_fCameraRoll;
	float						m_fRadius;
	//--------------------------------------------------------------------------------------
	// 오브젝트 회전 정보
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3					m_vObjectPosition;
	float						m_fYaw;
	float						m_fPitch;
	float						m_fRoll;
#else
	shared_ptr<GCamera > m_pMainCamera;
#endif
	D3DXMATRIX  m_matWorld;
	vector<shared_ptr<GHeroObj>>	m_HeroObj;

	T_STR_VECTOR m_LoadFiles;
	bool		Load();
	bool		LoadFileDlg(TCHAR* szExt, TCHAR* szTitle);

	HRESULT CreateResource();
	HRESULT DeleteResource();
public:
	CGciCharToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

public:
	virtual bool Frame();
	virtual bool Render();

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnCharload();
	afx_msg void OnCharsave();
};

extern CGciCharToolApp theApp;
