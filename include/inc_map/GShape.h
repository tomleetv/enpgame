#pragma once
#include "GModel.h"
class GShape : public GModel
{
public:
	//--------------------------------------------------------------------------------------
	// Collision Data
	//--------------------------------------------------------------------------------------
	G_BOX                       m_Box;
	G_SPHERE                    m_Sphere;
	G_PLANE                     m_Plane;
public:
	virtual void	SetShaded(bool bShaded = true) {
		if (bShaded) {
			SetColor(D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else {
			SetColor();
		}
	}
	virtual void	SetColor(D3DXVECTOR4 vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)) {
		m_cbData.Color = vColor;

		//EnterCriticalSection(&g_CSd3dDevice);
		m_dxobj.g_pConstantBuffer.Attach(DX::CreateConstantBuffer(m_pd3dDevice, &m_cbData, 1, sizeof(VS_CONSTANT_BUFFER)));
		//LeaveCriticalSection(&g_CSd3dDevice);

	}
	void	CreateOBBBox(	float fExtX = 1.0f, float fExtY = 1.0f, float fExtZ = 1.0f,
							D3DXVECTOR3 vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3 vDirX = D3DXVECTOR3(1.0f, 0.0f, 0.0f),
							D3DXVECTOR3 vDirY = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
							D3DXVECTOR3 vDirZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	void	CreateAABBBox(	D3DXVECTOR3 vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
							D3DXVECTOR3 vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
public:
	GShape() {};
	virtual ~GShape(void) {};
};
class GLineShape : public GShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT				SetInputLayout();
	bool				CreateVertexData();
	bool				CreateIndexData();
	HRESULT				CreateVertexBuffer();
	HRESULT				CreateResource();
	bool				Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 dwColor);
public:
	GLineShape(void);
	virtual ~GLineShape(void);
};

class GDirectionLineShape : public GShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT			SetInputLayout();
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateResource();
public:
	GDirectionLineShape(void);
	virtual ~GDirectionLineShape(void);
};

class GBoxShape : public GShape
{
public:
	virtual bool			CreateVertexData();
	virtual bool			CreateIndexData();
	virtual HRESULT			CreateResource();
public:
	GBoxShape(void);
	virtual ~GBoxShape(void);
};

//class GButtonShape : public GBoxShape
//{
//public:
//	bool				m_bShaded;
//public:
//	GButtonShape(void);
//	virtual ~GButtonShape(void);
//};

class GBoxHalfShape : public GBoxShape
{
public:
	virtual bool			CreateVertexData();
public:
	GBoxHalfShape(void) {};
	virtual ~GBoxHalfShape(void) {};
};

class GPlaneShape : public GShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
	bool SetScreenVertex(float x,
		float y,
		float w,
		float h,
		D3DXVECTOR2 vScreen);
public:
	GPlaneShape(void);
	virtual ~GPlaneShape(void);
};
class GSphereShape : public GShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			Render(ID3D11DeviceContext*	pImmediateContext);
	HRESULT			CreateResource();
	void CreateSphere(UINT Slices = 20, float fRadius = 1.0f);
	void MakeSpherePoint(std::vector<D3DXVECTOR3> &spherePoints,
		UINT Slices = 20, double r = 1.0f,
		D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0));
public:
	GSphereShape(void);
	virtual ~GSphereShape(void);
};
