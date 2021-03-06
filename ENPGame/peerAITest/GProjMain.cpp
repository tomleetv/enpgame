#include "_stdafx.h"
GProjMain* g_pMain;


bool GProjMain::Init()
{

	m_GAIZombMgr.Init(G_DEFINE_MAX_AI_ZOMBIE);
	//--------------------------------------------------------------------------------------
	// 카메라  행렬 
	//--------------------------------------------------------------------------------------	
	m_pMainCamera = make_shared<GCamera>();

	float fAspectRatio = g_pMain->m_iWindowWidth / (FLOAT)g_pMain->m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(D3DX_PI / 4, fAspectRatio, 0.1f, 100000.0f);
	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0.0f, 200.0f, -10.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	m_pMainCamera->SetWindow(g_pMain->m_iWindowWidth, g_pMain->m_iWindowHeight);

	SAFE_NEW(m_Box, GBoxShape);
	m_Box->Create(m_pd3dDevice, L"data/shader/box.hlsl", L"data_test/flagstone.bmp");
	D3DXMatrixIdentity(&m_matBoxWorld);
	D3DXMatrixScaling(&m_matBoxWorld, 10.0f, 10.0f, 10.0f);
	m_matBoxWorld._42 = 40.0f;

	SAFE_NEW(m_Box2, GBoxShape);
	m_Box2->Create(m_pd3dDevice, L"data/shader/box.hlsl", L"data_test/flagstone.bmp");
	D3DXMatrixIdentity(&m_matBoxWorld2);
	D3DXMatrixScaling(&m_matBoxWorld2, 10.0f, 10.0f, 10.0f);
	m_matBoxWorld._42 = 40.0f;





	return true;
}
bool GProjMain::Frame()
{
	//박스  움직임
	if (I_Input.KeyCheck(DIK_UP) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld._43 += G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}

	if (I_Input.KeyCheck(DIK_DOWN) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld._43 -= G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}

	if (I_Input.KeyCheck(DIK_LEFT) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld._41 -= G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}

	if (I_Input.KeyCheck(DIK_RIGHT) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld._41 += G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}


	//박스 2 움직임
	if (I_Input.KeyCheck(DIK_Y) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld2._43 += G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}

	if (I_Input.KeyCheck(DIK_H) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld2._43 -= G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}

	if (I_Input.KeyCheck(DIK_G) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld2._41 -= G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}

	if (I_Input.KeyCheck(DIK_J) == KEY_HOLD)
	{
		g_pMain->m_matBoxWorld2._41 += G_DEFINE_AI_TEST_HERO_SPEED * g_fSecPerFrame;
	}


	m_pMainCamera->Frame();


	m_GAIZombMgr.Frame(m_Box->m_matWorld);

	return true;
}
bool GProjMain::Render()
{
	m_Box->SetMatrix(&m_matBoxWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Box->Render(m_pImmediateContext);

	m_Box2->SetMatrix(&m_matBoxWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Box2->Render(m_pImmediateContext);

	m_GAIZombMgr.Render(m_pMainCamera.get());
	return true;
}
bool GProjMain::Release()
{
	m_GAIZombMgr.Release();
	return true;
}
HRESULT GProjMain::CreateResource()
{
	HRESULT hr;
	if (m_pMainCamera != nullptr)
		m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f,
			m_SwapChainDesc.BufferDesc.Width / (FLOAT)m_SwapChainDesc.BufferDesc.Height, 1.0f, 10000.0f);

	m_GAIZombMgr.CreateResource();

	return S_OK;
}
HRESULT GProjMain::DeleteResource()
{
	HRESULT hr;
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	return S_OK;
}

GProjMain::GProjMain(void)
{


}

GProjMain::~GProjMain(void)
{

}

int GProjMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera != nullptr)
	{
		m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
	}
	return -1;

}
GCORE_RUN(FSM TEST);
