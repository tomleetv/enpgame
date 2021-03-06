#pragma once



// GModelForm 폼 뷰입니다.

class GModelForm : public CFormView
{
	DECLARE_DYNCREATE(GModelForm)

protected:
	GModelForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~GModelForm();

	// 추가함_S
public:
	static GModelForm* CreateOne(CWnd* pParent);
	
public:
	void UpdateTextOBBInfo(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
	//추가함_E
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GMODELFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	float m_fMinX;
	float m_fMinY;
	float m_fMinZ;
	float m_fMaxX;
	float m_fMaxY;
	float m_fMaxZ;
};


