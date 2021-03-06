#pragma once

class GAIColAttack : public GAIColSeq
{
public:
	D3DXVECTOR3 m_vADestLook;
	D3DXVECTOR3 m_vARight;
	D3DXVECTOR3 m_vAUp;
	void AttackMove(int i, D3DXVECTOR3 vBoxPosition, D3DXVECTOR3 vZombiePosition);

	bool Init(GAICol* iMyIndex);
	bool Frame(GAICol* iMyIndex, D3DXMATRIX matHeroWorld, D3DXMATRIX matHeroWorld2, GHero* pHero);
	bool Render();
	bool Release();

	int	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	//----------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//----------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	virtual ~GAIColAttack();
	GAIColAttack();
protected:

};


