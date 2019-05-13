#include "stdafx.h"
#include "Airplane.h"


Airplane::Airplane()
{
	
}

Airplane::~Airplane()
{
}

void Airplane::Init()
{
	m_model.Init(L"Resource/modelData/World War II - Plane - Flying.cmo");
}
void Airplane::Update()
{
	if (GetAsyncKeyState('A')) {
		//キーボードのAが押されたら弾丸を生成。
		Bullet* bullet = new Bullet();
		bullet->Init();
		m_bulletList.push_back(bullet);
	}
	if (GetAsyncKeyState(VK_LEFT) ){
		m_position.x += 50.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		m_position.x -= 50.0f;
	}
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	//Bulletの更新関数を呼び出す。
	for (int i = 0; i < m_bulletList.size(); i++) {
		m_bulletList[i]->Update();
	}
}
void Airplane::Render(CMatrix viewMatrix, CMatrix projMatrix)
{
	m_model.Draw(viewMatrix, projMatrix);
	//弾丸の描画関数を呼び出す。
	for (int i = 0; i < m_bulletList.size(); i++) {
		m_bulletList[i]->Render(viewMatrix, projMatrix);
	}
}