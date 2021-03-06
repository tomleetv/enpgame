#include "GSkinObj.h"
#include "GTextureMgr.h"
#include "GTimer.h"
void GSkinObj::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
bool GSkinObj::Convert(const TCHAR* strPathName )
{
	return true;
}
bool GSkinObj::Load(ID3D11Device* pDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	if (szLoadName)
	{
		_tsplitpath(szLoadName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
	}
	memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
	_stprintf_s(szFileName, _T("%s%s"), Drive, Dir);

	// ─────────────────────────────────
	// 
	// ─────────────────────────────────
	FILE	*fp;
	_tfopen_s(&fp, szLoadName, _T("rb"));
	_ASSERT(fp != NULL);

	struct tm newtime;
	fread(&newtime, sizeof(tm), 1, fp);
	T_STR today = _tasctime(&newtime);	// The date string has a \n appended.
	today[today.size() - 1] = 0;
	fread(&m_Scene, sizeof(GScene), 1, fp);

	TCHAR szBuffer[128] = { 0, };
	size_t convertedChars = 0;

	for (int iter = 0; iter < m_Scene.iNumMesh; iter++)
	{
		shared_ptr<GMesh> pMesh = make_shared<GMesh>();
		shared_ptr<tSkmMesh> pData = make_shared<tSkmMesh>();

		TCHAR szName[256] = { 0,0 };
		TCHAR szParentName[256] = { 0,0 };

		// 노드 이름
		int iLength = 0;
		fread(&iLength, sizeof(int), 1, fp);
		fread(szName, sizeof(TCHAR)*iLength, 1, fp);

		pMesh->m_strNodeName = szName;
		// 노드 부모 이름
		fread(&iLength, sizeof(int), 1, fp);
		if (iLength > 0)
		{
			fread(szParentName, sizeof(TCHAR)*iLength, 1, fp);
			pMesh->m_strParentName = szParentName;
		}
		// 월드 행렬
		fread(&pMesh->m_matWorld, sizeof(D3DXMATRIX), 1, fp);
		//m_matWorld = pMesh->m_matWorld;

		fread(&pMesh->m_iNumMtrl, sizeof(int), 1, fp);
		if (pMesh->m_iNumMtrl <= 0)
		{
			LoadMesh(fp, pMesh.get(), pData.get(), szFileName);
			//SAFE_NEW_ARRAY(pMesh->m_pDrawVertex, PNCT5_VERTEX, pMesh->m_iNumVertexs );
		}
		else
		{
			for (int iter = 0; iter < pMesh->m_iNumMtrl; iter++)
			{
				shared_ptr<GMesh>  pSubMesh = make_shared<GMesh>();
				shared_ptr<tSkmMesh>  pSubData = make_shared<tSkmMesh>();
				if (LoadMesh(fp, pSubMesh.get(), pSubData.get(), szFileName))
				{
					SAFE_NEW_ARRAY(pSubData->m_pDrawVertex, PNCT5_VERTEX, pSubMesh->m_iNumVertexs);
					pMesh->m_pSubMesh.push_back(pSubMesh);
					pData->m_pSubMesh.push_back(pSubData);
				}
			}
		}
		m_pMesh.push_back(pMesh);
		m_pData.push_back(pData);
	}

	// 드레스 포즈의 각 바이패드 행렬 로드
	int iNumMatrix;
	fread(&iNumMatrix, sizeof(int), 1, fp);
	m_matBipedList.resize(iNumMatrix);
	for (int iMat = 0; iMat < iNumMatrix; iMat++)
	{
		fread(&m_matBipedList[iMat], sizeof(D3DXMATRIX), 1, fp);
	}

	fclose(fp);
	
	m_dxobj.m_iNumVertex = m_iMaxVertex;
	m_dxobj.m_iNumIndex = m_iMaxIndex;
	m_dxobj.m_iVertexSize = sizeof(PNCT5_VERTEX);

	if (!bThread && !Create(pDevice, pLoadShaderFile))
	{
		return false;
	}
	return true;
}
bool GSkinObj::LoadMesh(FILE	*fp, GMesh* pMesh, tSkmMesh* pData, const TCHAR* szLoadFileName)
{
	char szBuffer[128] = { 0, };
	size_t convertedChars = 0;
	
	fread(&pMesh->m_iNumVertexs, sizeof(int), 1, fp);
	fread(&pMesh->m_iNumFace, sizeof(int), 1, fp);
	fread(&pMesh->m_iNumTex, sizeof(int), 1, fp);

	pData->m_VertexArray.resize(pMesh->m_iNumVertexs);
	pData->m_IndexArray.resize(pMesh->m_iNumFace * 3);

	fread(&pData->m_VertexArray.at(0), sizeof(PNCT5_VERTEX) * pMesh->m_iNumVertexs, 1, fp);
	fread(&pData->m_IndexArray.at(0), sizeof(unsigned int) * pMesh->m_iNumFace * 3, 1, fp);

	m_iMaxVertex += pData->m_VertexArray.size();
	m_iMaxIndex += pData->m_IndexArray.size();
	
	for (int itex = 0; itex < pMesh->m_iNumTex; itex++)
	{
		int iLen = _tcslen(pMesh->m_szTexName[itex]);
		fread(&iLen, sizeof(int), 1, fp);
		fread(&pMesh->m_iTexType[itex], sizeof(int), 1, fp);
		ZeroMemory(pMesh->m_szTexName[itex], sizeof(TCHAR) * 30);
		fread(pMesh->m_szTexName[itex], sizeof(TCHAR) * iLen, 1, fp);

		TCHAR szFileName[MAX_PATH];
		TCHAR Drive[MAX_PATH];
		TCHAR Dir[MAX_PATH];
		TCHAR FName[MAX_PATH];
		TCHAR Ext[MAX_PATH];
		if (szLoadFileName)
		{
			_tsplitpath(szLoadFileName, Drive, Dir, FName, Ext);
			Ext[4] = 0;
			memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		}
		T_STR loadFile = Drive;
		loadFile += Dir;
		loadFile += pMesh->m_szTexName[itex];
		// 디퓨즈 텍스쳐
		if (pMesh->m_iTexType[itex] == ID_GCORE_DI)
		{
			//EnterCriticalSection(&g_CSd3dDevice);
			pMesh->m_iDiffuseTex = I_Texture.Add(g_pd3dDevice, loadFile.c_str());
			//LeaveCriticalSection(&g_CSd3dDevice);
		}
	}
	return true;
}
//void	GSkinObj::AnimationMatrix(GMesh<PNCT5_VERTEX>* pMesh )
//{	
//	PNCT5_VERTEX*	 pDrawVertex = pMesh->m_pDrawVertex;
//	D3DXVECTOR3 vPos;	
//			
//	D3DXMATRIX		matRotate, matInverse;
//	D3DXVECTOR3		vJointPos, vWeightPos;
//
//	TAniMatrix*	 pAnimMatrix = I_AniMatrixMgr.GetPtr(m_iMatrixIndex);
//
//	int iWeightCnt = 0;
//	for( int iVertex = 0; iVertex < pMesh->m_iNumVertexs; iVertex++ )
//	{			
//		int iBipCount = pMesh->m_VertexArray[iVertex].w2[3];				
//		// 최대 바이패드 인덱스 지원보다 크면 계산 생략함.
//		if( iBipCount > 7 ) continue;
//
//		pDrawVertex[iVertex]    = pMesh->m_VertexArray[iVertex];
//		pDrawVertex[iVertex].p  = D3DXVECTOR3( 0.0f,0.0f,0.0f );			
//	
//		D3DXMATRIX matWorld;
//		int iBipedIndex;
//		for( DWORD iIndex = 0; iIndex < iBipCount; iIndex++ )
//		{				
//			if(iIndex < 4)
//			{
//				iBipedIndex = pMesh->m_VertexArray[iVertex].i1[iIndex];
//				vPos = pMesh->m_WeightArray[iWeightCnt++];				
//				D3DXVec3TransformCoord( &vPos,	&vPos, &pAnimMatrix->m_BoneMesh.m_BipedMeshList[iBipedIndex].m_matWorld );	
//				//D3DXVec3TransformCoord( &vPos,	&vPos, &pAnimMatrix->m_BoneMesh.m_pMatrix[iBipedIndex] );
//				pDrawVertex[iVertex].p +=	vPos * pMesh->m_VertexArray[iVertex].w1[iIndex];
//			}
//			else
//			{
//				iBipedIndex = pMesh->m_VertexArray[iVertex].i2[iIndex-4];
//				vPos = pMesh->m_WeightArray[iWeightCnt++];		
//				D3DXVec3TransformCoord( &vPos,	&vPos, 	&pAnimMatrix->m_BoneMesh.m_BipedMeshList[iBipedIndex].m_matWorld);	
//				//D3DXVec3TransformCoord( &vPos,	&vPos, &pAnimMatrix->m_BoneMesh.m_pMatrix[iBipedIndex] );
//				pDrawVertex[iVertex].p +=  vPos * pMesh->m_VertexArray[iVertex].w2[iIndex-4];						
//			}
//		}	
//	}		
//}
bool GSkinObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
bool GSkinObj::Release()
{
	return true;
}
HRESULT GSkinObj::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 96, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//EnterCriticalSection(&g_CSd3dDevice);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
		m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	//LeaveCriticalSection(&g_CSd3dDevice);

	return hr;
}
bool GSkinObj::UpdateBuffer()
{
	if (!CombineBuffer(m_dxobj.g_pVertexBuffer.Get(), m_dxobj.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}
bool GSkinObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
{
	size_t dstOffset = 0;
	size_t vbOffset = 0;
	size_t dstibOffset = 0;
	size_t ibOffset = 0;
	{
		for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
		{
			auto pMesh = m_pMesh[dwObject].get();
			auto pData = m_pData[dwObject].get();
			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
				{
					auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
					auto pSubData = pData->m_pSubMesh[dwSub].get();
					if (pData->m_VertexArray.size() < 3) continue;

					pSubMesh->m_dxobj.m_iNumVertex = pData->m_VertexArray.size();
					pSubMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

					pSubMesh->m_dxobj.m_BoxVB.left = dstOffset;
					pSubMesh->m_dxobj.m_BoxVB.right = dstOffset + pSubMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize;
					pSubMesh->m_dxobj.m_BoxVB.top = 0; pSubMesh->m_dxobj.m_BoxVB.bottom = 1;
					pSubMesh->m_dxobj.m_BoxVB.front = 0; pSubMesh->m_dxobj.m_BoxVB.back = 1;

					//EnterCriticalSection(&g_CSImmediateContext);
					g_pImmediateContext->UpdateSubresource(
						pVB, 0,
						&pSubMesh->m_dxobj.m_BoxVB,
						(uint8_t*)&pSubData->m_VertexArray.at(0),
						0, 0);
					//LeaveCriticalSection(&g_CSImmediateContext);

					/*g_pImmediateContext->CopySubresourceRegion(
					m_dxobj.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
					(void*)&pSubMesh->m_VertexArray.at(0),
					0, &pSubMesh->m_dxobj.m_BoxVB);*/

					pSubMesh->m_dxobj.m_iBeginVB = vbOffset;
					vbOffset += pSubMesh->m_dxobj.m_iNumVertex;
					dstOffset = pSubMesh->m_dxobj.m_BoxVB.right;

					pSubMesh->m_dxobj.m_iNumIndex = pSubData->m_IndexArray.size();
					pSubMesh->m_dxobj.m_BoxIB.left = dstibOffset;
					pSubMesh->m_dxobj.m_BoxIB.right = dstibOffset + pSubMesh->m_dxobj.m_iNumIndex*sizeof(DWORD);
					pSubMesh->m_dxobj.m_BoxIB.top = 0;		pSubMesh->m_dxobj.m_BoxIB.bottom = 1;
					pSubMesh->m_dxobj.m_BoxIB.front = 0;	pSubMesh->m_dxobj.m_BoxIB.back = 1;

					//EnterCriticalSection(&g_CSImmediateContext);
					g_pImmediateContext->UpdateSubresource(pIB, 0,
						&pSubMesh->m_dxobj.m_BoxIB, (void*)&pSubData->m_IndexArray.at(0), 0, 0);
					//LeaveCriticalSection(&g_CSImmediateContext);

					pSubMesh->m_dxobj.m_iBeginIB = ibOffset;
					ibOffset += pSubMesh->m_dxobj.m_iNumIndex;
					dstibOffset = pSubMesh->m_dxobj.m_BoxIB.right;

					//texture
					if (pSubMesh->m_iDiffuseTex < 0) continue;
					pSubMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pSubMesh->m_iDiffuseTex)->m_pTextureRV;
				}
			}
			else
			{
				if (pData->m_VertexArray.size() < 3) continue;

				pMesh->m_dxobj.m_iNumVertex = pData->m_VertexArray.size();
				pMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

				pMesh->m_dxobj.m_BoxVB.left = dstOffset;
				pMesh->m_dxobj.m_BoxVB.right = dstOffset + pMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize;
				pMesh->m_dxobj.m_BoxVB.top = 0; pMesh->m_dxobj.m_BoxVB.bottom = 1;
				pMesh->m_dxobj.m_BoxVB.front = 0; pMesh->m_dxobj.m_BoxVB.back = 1;

				//EnterCriticalSection(&g_CSImmediateContext);
				g_pImmediateContext->UpdateSubresource(pVB, 0,	&pMesh->m_dxobj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);
				//LeaveCriticalSection(&g_CSImmediateContext);

				pMesh->m_dxobj.m_iBeginVB = vbOffset;
				vbOffset += pMesh->m_dxobj.m_iNumVertex;
				dstOffset = pMesh->m_dxobj.m_BoxVB.right;

				pMesh->m_dxobj.m_iNumIndex = pData->m_IndexArray.size();
				pMesh->m_dxobj.m_BoxIB.left = dstibOffset;
				pMesh->m_dxobj.m_BoxIB.right = dstibOffset + pMesh->m_dxobj.m_iNumIndex*sizeof(DWORD);
				pMesh->m_dxobj.m_BoxIB.top = 0; pMesh->m_dxobj.m_BoxIB.bottom = 1;
				pMesh->m_dxobj.m_BoxIB.front = 0; pMesh->m_dxobj.m_BoxIB.back = 1;

				//EnterCriticalSection(&g_CSImmediateContext);
				g_pImmediateContext->UpdateSubresource(pIB, 0,	&pMesh->m_dxobj.m_BoxIB, (void*)&pData->m_IndexArray.at(0), 0, 0);
				//LeaveCriticalSection(&g_CSImmediateContext);

				pMesh->m_dxobj.m_iBeginIB = ibOffset;
				ibOffset += pMesh->m_dxobj.m_iNumIndex;
				dstibOffset = pMesh->m_dxobj.m_BoxIB.right;			

				//texture
				if (pMesh->m_iDiffuseTex <= 0) continue;
				pMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureRV;
			}
		}
	}
	return true;
}
bool GSkinObj::Draw(ID3D11DeviceContext*    pContext, GModel* pParent)
{
	//CStopwatch stopwatch;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		pParent->SetMatrix(&pParent->m_matWorld, &pParent->m_matView, &pParent->m_matProj);
		UpdateConstantBuffer(pContext, pParent);


		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1) continue;


				//조명 [Start]
				UpdateLightConstantBuffer(pSubMesh->m_matWorld);
				//조명 [End]


				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_dxobj.m_iNumIndex,
					pSubMesh->m_dxobj.m_iBeginIB,
					pSubMesh->m_dxobj.m_iBeginVB);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;


			//조명 [Start]
			UpdateLightConstantBuffer(pMesh->m_matWorld);
			//조명 [End]


			pContext->PSSetShaderResources(0, 1, pMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_dxobj.m_iNumIndex,
				pMesh->m_dxobj.m_iBeginIB,
				pMesh->m_dxobj.m_iBeginVB);
		}
	}
	//stopwatch.Output(L"\nTTbsObj::Draw");
	return true;
}
GSkinObj::GSkinObj(void)
{
	memset( &m_VersionMark, 0, sizeof(VersionMark));
	m_iMaxWeight		= 0;
	m_Scene.iBindPose	= 0;
}

GSkinObj::~GSkinObj(void)
{
}
