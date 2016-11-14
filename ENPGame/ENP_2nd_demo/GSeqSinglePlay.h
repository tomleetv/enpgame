#pragma once
class GSeqSinglePlay : public GSeq
{
private:
	static GSeqSinglePlay *pInstance_;
public:
	static GSeq* CreateInstance() {
		if (pInstance_ == 0) pInstance_ = new GSeqSinglePlay;
		return pInstance_;
	}
#ifdef G_MACRO_EFFECT_ADD
public:
	GPlaneShape						m_BigPlane;
	shared_ptr<GSprite>				m_pSprite;
	ComPtr<ID3D11PixelShader>		m_pPS;
	D3DXVECTOR4						m_vMaterial;
	D3DXMATRIX						m_matPlaneWorld;
	D3DXMATRIX						m_mPlanWorld;
	//D3DXMATRIX				m_matWorld;
	shared_ptr<GPlaneShape>			m_pPlane;
	shared_ptr<GPlaneShape>			m_pScreen;
	//public:
	//	bool check; // play button check
	//	ComPtr<ID3D11BlendState>		m_pAlphaBlendFactor;
	//	ComPtr<ID3D11BlendState>		m_pAlphaBlend;
#endif
#ifdef G_MACRO_MAP_ADD
	//--------------------------------------------------------------------------------------
	// ������Ʈ
	//--------------------------------------------------------------------------------------
	//GMapObject*			m_pMapObj;
	//GBoxShape*			m_pBoxs;
	//--------------------------------------------------------------------------------------
	// ����Ʈ��
	//--------------------------------------------------------------------------------------
	GMap m_CustomMap;
	//--------------------------------------------------------------------------------------
	// ����Ʈ��
	//--------------------------------------------------------------------------------------
	GQuadTree	m_QuadTree;
	
	//--------------------------------------------------------------------------------------
	// ����� �뵵
	//--------------------------------------------------------------------------------------
	GLineShape		m_DrawLine;
	bool			m_bDebugRender;
	int				m_iDrawDepth;
	ComPtr<ID3D11PixelShader>   m_pPixelShader;// ���ν��� ���� �Ƚ����̴�


	void		DrawSelectTreeLevel(D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool		DrawQuadLine(GNode* pNode);
#endif
	shared_ptr<GCamera > m_pMainCamera;
	D3DXMATRIX  m_matWorld;
#ifdef G_MACRO_CHAR_ADD	
public:
	//--------------------------------------------------------------------------------------
	// ���� �����Ͽ� �ε�( ����� : O )
	//--------------------------------------------------------------------------------------
	vector<shared_ptr<GHeroObj>>	m_HeroObj;
	T_STR_VECTOR m_LoadFiles;
	bool		Load();
	bool		LoadFileDlg(TCHAR* szExt, TCHAR* szTitle);
#endif
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//----------------------------------------------------
	// ����� Ŭ���̾�Ʈ ������ �缳���� ���� �Ҹ� �� ����
	//----------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();

public:
	virtual ~GSeqSinglePlay();
protected:
	GSeqSinglePlay();

};


