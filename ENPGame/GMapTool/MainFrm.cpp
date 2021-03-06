
// MainFrm.cpp : CMainFrame 클래스의 구현
//
#define Test_save

#include "stdafx.h"
#include "MainFrm.h"
#include "MapTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_CREATEMAP, &CMainFrame::OnCreatemap)
	ON_COMMAND(ID_SAVEMAP, &CMainFrame::OnSavemap)
	ON_COMMAND(ID_LOADMAP, &CMainFrame::OnLoadmap)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("메뉴 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	//추가된 메뉴가 축소되서 보이지 않아서 추가함[S]
	m_wndMenuBar.SetRecentlyUsedMenus(FALSE);
	//추가된 메뉴가 축소되서 보이지 않아서 추가함[E]


	// 메뉴 모음을 활성화해도 포커스가 이동하지 않게 합니다.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}




	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 사용자 정의 도구 모음 작업을 허용합니다.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: 도구 모음 및 메뉴 모음을 도킹할 수 없게 하려면 이 다섯 줄을 삭제하십시오.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	/*
	if (TRUE == m_wndSampleCtrl.CreateEx(
		NULL, _T("GSampleCtrl"), this, CRect(0,0,300,300	),
		TRUE,1234,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | WS_CLIPSIBLINGS
		| WS_CLIPCHILDREN | CBRS_FLOAT_MULTI))
	{
		m_wndSampleCtrl.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndSampleCtrl);
	}
	*/

	if (TRUE == m_wndObjCtrl.CreateEx(
		NULL, _T("GObjCtrl"), this, CRect(0, 0, 300, 300),
		TRUE, 5678,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | WS_CLIPSIBLINGS
		| WS_CLIPCHILDREN | CBRS_FLOAT_MULTI))
	{
		m_wndObjCtrl.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndObjCtrl);
	}


	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 도킹 창을 만듭니다.
	if (!CreateDockingWindows())
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
		return -1;
	}

	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);

	// 보관된 값에 따라 비주얼 관리자 및 스타일을 설정합니다.
	OnApplicationLook(theApp.m_nAppLook);

	// 도구 모음 및 도킹 창 메뉴 바꾸기를 활성화합니다.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 빠른(<Alt> 키를 누른 채 끌기) 도구 모음 사용자 지정을 활성화합니다.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 사용자 정의 도구 모음 이미지를 로드합니다.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 메뉴 개인 설정을 활성화합니다(가장 최근에 사용한 명령).
	// TODO: 사용자의 기본 명령을 정의하여 각 풀다운 메뉴에 하나 이상의 기본 명령을 포함시킵니다.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 클래스 뷰를 만듭니다.
	/*
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("클래스 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}
	*/

	// 파일 뷰를 만듭니다.
	/*
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("파일 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}
	*/

	// 출력 창을 만듭니다.
	/*
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}
	*/

	// 속성 창을 만듭니다.
	/*
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("속성 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}
	*/

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	//m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}

// 지형생성 다이얼로그 호출
void CMainFrame::OnCreatemap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	GCreateMapDlg dlg;
	dlg.DoModal();

}
void CMainFrame::PrintToStatusbar(CString strMsg)
{
	m_wndStatusBar.SetPaneText(0, strMsg);
}

// 지형 저장 다이얼로그 호출
void CMainFrame::OnSavemap()
{

	if (theApp.m_MapMgr.m_iMapSelected == -1) {
		AfxMessageBox(
			L"선택된 맵이 없습니다.\n"
			L"맵을 우선 선택해주시겠습니까?", MB_OK);
		return;
	}


	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();


	TCHAR current_path[MAX_PATH] = _T("");
	GetCurrentDirectory(MAX_PATH, current_path); // 현재 경로 저장

	CFileDialog dlg(false, _T("map"), _T("*.map"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("MAP File(*.map)|*.map||"));

	if (dlg.DoModal() == IDOK) { // OK 를 하는 순간 현재 경로가 변경된다.
								 // 여기서 파일 저장 작업.
								 // 여기서 얻는 dlg.GetFileName() 함수는 상대 경로로의 파일 이름을 반환해 준다.

								 // 선택한 파일의 이름..
								 // dlg.GetFileName();

								 // 선택한 파일의 이름을 포함한 경로..
		CString strFile = dlg.GetPathName();

		{

			FILE* fp = fopen((CStringA)strFile, "wt+");

			CString str;
			CString strNum;
			TCHAR strTex[MAX_PATH];
			TCHAR strHeight[MAX_PATH];

			

			_tcsncpy_s(strTex, theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_strTex, MAX_PATH);
			_tcsncpy_s(strHeight, theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_strHeight, MAX_PATH);

			//Texture 이름 출력
			str = (LPCTSTR)strTex;
			str.Append(_T("\n"));
			fprintf(fp, (CStringA)str);

			//HeightMap 이름 출력
			str = (LPCTSTR)strHeight;
			str.Append(_T("\n"));
			fprintf(fp, (CStringA)str);

			for (int iMapObj = 0; iMapObj<theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj.size(); iMapObj++)
			{
				TCHAR strObj[MAX_PATH] = L"";// = L"data\\object\\building\\";
				//오브젝트 gbs 이름 출력
				_tcsncat(strObj, theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_strName, _tcsclen(theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_strName));

				str = strObj;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);

				//오브젝트 조명 reverse 여부 값 출력 true: 1, false: 0
				if (theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_bLightReverse == TRUE)
					strNum.Format(_T("%d"), 1);
				else
					strNum.Format(_T("%d"), 0);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);

				//오브젝트 Specular 여부 값 출력 true: 1, false: 0
				if (theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_bLightSpecular == TRUE)
					strNum.Format(_T("%d"), 1);
				else
					strNum.Format(_T("%d"), 0);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);


				//오브젝트 scl 값 출력
				strNum.Format(_T("%f"), theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_fScl);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);

				//오브젝트 rot 값 출력
				strNum.Format(_T("%f"), theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_fRotY);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);

				//오브젝트 translation x,y,z 값 출력
				strNum.Format(_T("%f"), theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_matObjTrans._41);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);

				strNum.Format(_T("%f"), theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_matObjTrans._42);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);

				strNum.Format(_T("%f"), theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[iMapObj]->m_matObjTrans._43);
				str = strNum;
				str.Append(_T("\n"));
				fprintf(fp, (CStringA)str);


			}

			fclose(fp);
			strFile.Append(_T(" 저장완료"));

			AfxMessageBox( strFile, MB_OK);
		}

	}
	SetCurrentDirectory(current_path); // 원래 경로로 돌아 간다.

}


void CMainFrame::OnObjectformview()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


}


void CMainFrame::OnLoadmap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if (!theApp.LoadFileDlg(_T("map"), _T("MAP Load")))
	{
		return;
	}

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_wndObjCtrl.m_wndForm->m_listObj.ResetContent();
	pFrame->m_wndObjCtrl.m_wndForm->m_listObj.UpdateData(FALSE);
	pFrame->m_wndObjCtrl.m_wndForm->m_listMap.ResetContent();
	pFrame->m_wndObjCtrl.m_wndForm->m_listMap.UpdateData(FALSE);

	int iLoad = theApp.m_LoadFiles.size() - 1;

	//Map생성과 Obj 생성. 파일 이름을 넘겨서 함수에서 해당 파일 읽어서 처리한다.
	if (theApp.m_MapMgr.LoadMap(&theApp.m_LoadFiles[iLoad], theApp.m_pMainCamera.get(),&theApp)) {
		//로드가 끝난후 오브젝트 리스트 컨트롤과 맵 리스트 컨트롤을 업데이트 해준다.

		theApp.m_MapMgr.m_iMapSelected = theApp.m_MapMgr.m_vecMapGroup.size() - 1;

		for (int i = 0; i < theApp.m_MapMgr.m_vecMapGroup.size(); i++) {
			TCHAR szRet[30] = { 0 }; // "10"의 NULL 처리를 위한 3 count
			_stprintf_s(szRet, _countof(szRet), _T("%d"), i);
			pFrame->m_wndObjCtrl.m_wndForm->m_listMap.AddString(szRet);
		}



		pFrame->m_wndObjCtrl.m_wndForm->m_listObj.ResetContent();

		for (int i = 0; i < theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj.size(); i++) {

			TCHAR szRet[30] = { 0 }; // "10"의 NULL 처리를 위한 3 count

			TCHAR strObjName[MAX_PATH];

			_tcsncpy_s(strObjName, theApp.m_MapMgr.m_vecMapGroup[theApp.m_MapMgr.m_iMapSelected]->m_vecObj[i]->m_strName, MAX_PATH);

			theApp.m_MapMgr.GetStringFileName(strObjName, strObjName);

			_stprintf_s(szRet, _countof(szRet), _T("%d - %s"), i, strObjName);
			pFrame->m_wndObjCtrl.m_wndForm->m_listObj.AddString(szRet);

		}
		pFrame->m_wndObjCtrl.m_wndForm->m_listObj.UpdateData(FALSE);
	}


	
}
