#pragma once
#include "GCoreStd.h"
#include "GDxHelperEx.h"
#include "GEnumeration.h"

using namespace DX;

class GDevice 
{
public:
	DXGI_MODE_DESC			m_FindBufferDesc;
	GEnumeration			m_Enumeration;
	GDxRT					m_DefaultRT;
	//--------------------------------------------
	// Global Variables
	//--------------------------------------------	
	ID3D11Device*           m_pd3dDevice;		// 디바이스 객체
	IDXGISwapChain*         m_pSwapChain;		// 스왑체인 객체
	D3D_DRIVER_TYPE         m_driverType;		// 디바이스 타입( 디폴트:하드웨어 가속 )
	IDXGIFactory*			m_pGIFactory;		// DXGI 객체
	ID3D11DeviceContext*    m_pImmediateContext;
	D3D_FEATURE_LEVEL       m_FeatureLevel;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;	
	BOOL					m_IsFullScreenMode;	// 풀스크린 디바이스 여부판단
public:
	BOOL					GetFullScreenFlag();
	void					SetFullScreenFlag( BOOL bFlag );
	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetContext();
	IDXGISwapChain*			GetSwapChain();
	ID3D11RenderTargetView*	    GetRenderTargetView();
	ID3D11RenderTargetView**	GetRenderTargetViewAddress() {return  m_DefaultRT.m_pRenderTargetView.GetAddressOf(); }
	ID3D11DepthStencilView*		GetDepthStencilView() {		return m_DefaultRT.m_pDepthStencilView.Get();	}
	ID3D11DepthStencilView**	GetDepthStencilViewAddress() { return  m_DefaultRT.m_pDepthStencilView.GetAddressOf(); }
	IDXGIFactory*			GetGIFactory();
public:
	//--------------------------------------------
	// for HW info 출력
	//--------------------------------------------
	bool Start();
	DXGI_MODE_DESC FindClosestMatchingMode(DXGI_MODE_DESC& Desc, ID3D11Device* pd3dDevice);

	//--------------------------------------------
	// 디바이스 및 스왑체인 생성 : InitDevice()
	//--------------------------------------------
	HRESULT		InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen= FALSE);
	// 0, InitDevice()
	// 1, CreateGIFactory()
	// 2, CreateDevice()
	// 3, CreateSwapChain()
	// 4, SetRenderTargetView()
	// 5, SetViewPort()
	//--------------------------------------------
	HRESULT		CreateDevice( );
	HRESULT		CreateGIFactory();
	HRESULT		CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeigh, BOOL IsFullScreen = FALSE);
	HRESULT		CreateDeviceAndSwapChain(HWND hWnd, UINT iWidth, UINT iHeigh, BOOL IsFullScreen = FALSE);
	//--------------------------------------------	
	bool		CleanupDevice();
	///--------------------------------------------
	// 변경된 클라이언트 영역를 재설정한다.
	//--------------------------------------------
	HRESULT		ResizeDevice( UINT iWidth, UINT iHeight);
	virtual		HRESULT		SetRenderTargetView();
	virtual		HRESULT		SetViewPort();
	virtual		HRESULT		DeleteDxResource();
	virtual		HRESULT		CreateDxResource();

public:
	GDevice(void);
	virtual ~GDevice(void);
};
