#pragma once
class GAIFollow : public GSeq
{
private:
	static GAIFollow *pInstance_;
public:
	static GSeq* CreateInstance()
	{
		if (pInstance_ == 0) pInstance_ = new GAIFollow;
		return pInstance_;
	}
	shared_ptr<GCamera > m_pMainCamera;
	D3DXMATRIX  m_matWorld;

	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	void FollowMove();
	GNewZombie* Zombie;
	int hp;
	D3DXVECTOR3 B_Look[ZombieNum];
	D3DXMATRIX B_Trans[ZombieNum];

	int	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	D3DXVECTOR3 RandomPoint[ZombieNum];
	D3DXMATRIX m_BoxRotation[ZombieNum];

	D3DXVECTOR3 vBoxPosition[ZombieNum];
	D3DXVECTOR3 vZombiePosition[ZombieNum];
	D3DXVECTOR3 vBDestLook1[ZombieNum];
	D3DXVECTOR3 vBDestLook[ZombieNum];
	D3DXVECTOR3 vBRight[ZombieNum];
	D3DXVECTOR3 vBUp[ZombieNum];

	float	ZombieDistance[ZombieNum];
	D3DXVECTOR3 vDistance[ZombieNum];

	//----------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//----------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();
public:
	virtual ~GAIFollow();
protected:
	GAIFollow();
};


