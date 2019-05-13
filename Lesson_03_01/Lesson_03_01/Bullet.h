#pragma once
#include "SkinModel.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void Init();
	void Update();
	void Render(CMatrix viewMatrix, CMatrix projMatrix);
private:
	SkinModel	m_model;							//モデル。
	CVector3	m_position = CVector3::Zero();		//座標。
};

