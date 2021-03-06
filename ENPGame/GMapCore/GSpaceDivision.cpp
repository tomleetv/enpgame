#include "GSpaceDivision.h"

int GSpaceDivision::CalculateBox( G_BOX *tBox, GPolygon *Polylist)
{
	int iNumPoly = 0;

	GPolygon *PolyPointer = Polylist;

	tBox->vMin = D3DXVECTOR3( 50000.0f, 50000.0f, 50000.0f ); 
	tBox->vMax = D3DXVECTOR3( -50000.0f, -50000.0f, -50000.0f ); 

	while( PolyPointer != NULL )
	{		
		iNumPoly++;
		for ( DWORD i = 0 ; i< PolyPointer->iNumVertices; i++ )
		{
			// check Minimum Bounds
			if (PolyPointer->pVertexList[i].p.x<tBox->vMin.x) tBox->vMin.x=PolyPointer->pVertexList[i].p.x;
			if (PolyPointer->pVertexList[i].p.y<tBox->vMin.y) tBox->vMin.y=PolyPointer->pVertexList[i].p.y;
			if (PolyPointer->pVertexList[i].p.z<tBox->vMin.z) tBox->vMin.z=PolyPointer->pVertexList[i].p.z;
			// check Maximum Bounds
			if (PolyPointer->pVertexList[i].p.x>tBox->vMax.x) tBox->vMax.x=PolyPointer->pVertexList[i].p.x;
			if (PolyPointer->pVertexList[i].p.y>tBox->vMax.y) tBox->vMax.y=PolyPointer->pVertexList[i].p.y;
			if (PolyPointer->pVertexList[i].p.z>tBox->vMax.z) tBox->vMax.z=PolyPointer->pVertexList[i].p.z; 
		}
		PolyPointer = PolyPointer->pNext;
	}

	tBox->vCenter = ( tBox->vMax+tBox->vMin ) *0.5f;
	D3DXVECTOR3 vHalf  = tBox->vMax-tBox->vCenter;
	tBox->fExtent[0] = D3DXVec3Dot( &tBox->vAxis[0], &vHalf );
	tBox->fExtent[1] = D3DXVec3Dot( &tBox->vAxis[1], &vHalf );
	tBox->fExtent[2] = D3DXVec3Dot( &tBox->vAxis[2], &vHalf );
	return iNumPoly;
}
GSpaceDivision::GSpaceDivision(void)
{
	memset( m_szTexturePath, 0, sizeof(TCHAR) * MAX_PATH );
}

GSpaceDivision::~GSpaceDivision(void)
{
}

