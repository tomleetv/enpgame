// GObjectControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GObjectControl.h"


// GObjectControl

IMPLEMENT_DYNAMIC(GObjectControl, CDockablePane)

GObjectControl::GObjectControl()
{

}

GObjectControl::~GObjectControl()
{
}


BEGIN_MESSAGE_MAP(GObjectControl, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// GObjectControl 메시지 처리기입니다.




void GObjectControl::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int GObjectControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


int GObjectControl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
