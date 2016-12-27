#pragma once

#include "map/GMap.h"
#include "map/GQuadTree.h"
#include "GMapMgr.h"
#include "GInput.h"

class GMapMgr
{
public:
	bool			DrawQuadLine(GNode* pNode,GCamera* pCamera);
	bool			DrawDebug(GCamera* pCamera);

public:
	bool			CreateInit(int Width, int Height, float Distance, CString strTex,GCamera* pCamera);

	//--------------------------------------------------------------------------------------
	// ��
	//--------------------------------------------------------------------------------------
	//GCreateMapDlg GCreateDlg;
	//GSaveMapDlg GSaveDlg;
	//--------------------------------------------------------------------------------------
	// ��
	//--------------------------------------------------------------------------------------
	GMap	 m_CustomMap;
	TMapDesc m_MapDesc;
	//--------------------------------------------------------------------------------------
	// ����Ʈ��
	//--------------------------------------------------------------------------------------
	GQuadTree m_QuadTree;

	//--------------------------------------------------------------------------------------
	// ����� �뵵
	//--------------------------------------------------------------------------------------
	GLineShape		m_DrawLine;
	bool			m_bDebugRender;
	int				m_iDrawDepth;



	bool			Init();
	bool			Frame(GInput* pInput = NULL);
	bool			Render(GCamera* pCamera);
	bool			Release();
	HRESULT			CreateResource();
	HRESULT			DeleteResource();



	GMapMgr();
	virtual ~GMapMgr();
};

