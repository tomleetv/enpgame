#pragma once
//#include"GNewZombieMgr.h"
enum ZombieNum {
	ZombieNum = G_DEFINE_MAX_AI_ZOMBIE
};
class GAIMove : public GAISeq
{
public:
	float		m_fTime;// = 0.0f;
	D3DXVECTOR3 m_vPos;

	static D3DXVECTOR3 RandomMove();

	bool Init(GNewZombie* iMyIndex);
	bool Frame(GNewZombie* iMyIndex,D3DXMATRIX matHeroWorld, D3DXMATRIX matHeroWorld2);
	bool Render();
	bool Release();
	//----------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//----------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();


	int			WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return -1; };
public:
	virtual ~GAIMove();
	GAIMove();
protected:

};

