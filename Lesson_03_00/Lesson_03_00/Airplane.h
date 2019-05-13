#pragma once
#include "SkinModel.h"
#include <vector>
#include "Bullet.h"

class Airplane
{
public:
	Airplane();
	~Airplane();
	void Init();
	void Update();
	void Render(CMatrix viewMatrix, CMatrix projMatrix);
private:
	SkinModel	m_model;						//!<モデル。
	CVector3	m_position = CVector3::Zero();	//!<座標。
	std::vector<Bullet*>	m_bulletList;		//!<弾丸の可変長配列。
};

