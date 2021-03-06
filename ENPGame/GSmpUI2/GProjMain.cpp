#include "_stdafx.h"

GProjMain* g_pMain;

bool GProjMain::Init()
{
	m_UIManager.Init();

	//T_STR strFile = L"UI_TEST4.gui";
	T_STR strFile = L"data/ui_menu.gui";
	m_UIManager.UILoad(&strFile, &m_SwapChainDesc, m_DefaultRT.m_vp.Width, m_DefaultRT.m_vp.Height);
	return true;
}
bool GProjMain::Frame()
{
	m_UIManager.Frame(&m_SwapChainDesc,&m_Timer);
	return true;
}
bool GProjMain::Render()
{
	//D3DXMATRIX matTrans, matRotation, matZ;

	// 2초당 1회전( 1 초 * D3DX_PI = 3.14 )
	//float t = m_Timer.GetElapsedTime() * D3DX_PI;

	int clicked = -1;

	m_UIManager.Render(&clicked);

	if (clicked != -1) {
		int a = 10;
	}
	return true;
}
bool GProjMain::Release()
{
	m_UIManager.Release();

	return true;
}

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GProjMain::CreateResource()
{
	HRESULT hr;

	m_UIManager.CreateResource(&m_SwapChainDesc);

	return S_OK;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT GProjMain::DeleteResource()
{
	HRESULT hr = S_OK;
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}
GProjMain::GProjMain(void)
{

}

GProjMain::~GProjMain(void)
{
}
GCORE_RUN(L"GBasisSample UI_0");