#pragma once
#include "GCoreLibV2.h"
#include "GDxHelperEx.h"
#include "GPlaneRect.h"

//#pragma comment( lib, "IMM32.lib" )
using namespace DX;
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
class GProjMain : public GCoreLibV2
{
public:
	VS_CONSTANT_BUFFER			m_cbData;
	std::vector<GControlUI*>	m_pUIList;
	GControlUI*					m_pSelectPlane;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV[3];
	GSceneUI						m_Scene;
public:	
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	//bool		PlaneRender(D3DXMATRIX matWorld, D3DXVECTOR4 vColor);
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();	
public:
	GControlUI* AddRect(GUI_TYPE type);
	GControlUI* SelectRect();
	int		 WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	GProjMain(void);
	virtual ~GProjMain(void);
};

extern GProjMain*	g_pMain; //외부 연결 범위 선언