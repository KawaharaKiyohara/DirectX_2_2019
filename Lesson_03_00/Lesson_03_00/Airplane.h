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
	SkinModel	m_model;						//!<���f���B
	CVector3	m_position = CVector3::Zero();	//!<���W�B
	std::vector<Bullet*>	m_bulletList;		//!<�e�ۂ̉ϒ��z��B
};

