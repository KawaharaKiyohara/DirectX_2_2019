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
	SkinModel	m_model;							//���f���B
	CVector3	m_position = CVector3::Zero();		//���W�B
};

