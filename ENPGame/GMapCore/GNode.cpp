#include "GNode.h"

GNode::GNode()
{
	m_dwLodType	= 0;
	m_iDepth	= 0;
	m_isLeaf	= FALSE;
	m_tBox.vCenter	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_tBox.vMax		= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	m_tBox.vMin		= D3DXVECTOR3( -1.0f, -1.0f, -1.0f );
	m_tBox.vAxis[0] = D3DXVECTOR3( 1.0f,0.0f, 0.0f );
	m_tBox.vAxis[1] = D3DXVECTOR3( 0.0f,1.0f, 0.0f );
	m_tBox.vAxis[2] = D3DXVECTOR3( 0.0f,0.0f, 1.0f );
	m_tBox.fExtent[0] = 1;
	m_tBox.fExtent[1] = 1;
	m_tBox.fExtent[2] = 1;


	iNumPolygon = 0;
	iNumMtrl = 0;
	m_pSplitter = NULL;
}
GNode::~GNode()
{
	// 노드의 코너 정점 인덱스 및 위치
	if (!m_CornerList.empty()) m_CornerList.clear();
	if (!m_CornerIndex.empty()) m_CornerIndex.clear();	// 추가

	for (int i = 0; i < m_ChildList.size(); i++)
	{
		SAFE_DEL(m_ChildList[i]);
	}
	//////////// bsp 
	// 노드의 분할면 및 리프노드의 폴리곤 리스트 소멸
	/*GPolygon *pDelSplitter = m_pSplitter;
	while (pDelSplitter)
	{
		GPolygon *pPolyDel = pDelSplitter;
		pDelSplitter = pPolyDel->pNext;
		SAFE_DEL(pPolyDel);
	}*/

	/*if (m_ChildList.size() > 0)
	{
		for (int ibuffer = 0; ibuffer < iNumMtrl; ibuffer++)
		{
			SAFE_DEL(m_pVBList[ibuffer]);
			SAFE_DEL(m_pIBList[ibuffer]);
		}
	}*/

	if (!m_pVBList.empty()) m_pVBList.clear();
	if (!m_pIBList.empty()) m_pIBList.clear();
	if (!m_MtrlList.empty()) m_MtrlList.clear();
}
