#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
void Bullet::Init()
{
	m_model.Init(L"Resource/modelData/bullet.cmo");
}
void Bullet::Update()
{
	m_position.z -= 100.0f;
	m_model.UpdateWorldMatrix(
		m_position,
		CQuaternion::Identity(),
		CVector3::One()
	);
}
void Bullet::Render(CMatrix viewMatrix, CMatrix projMatrix)
{
	m_model.Draw(viewMatrix, projMatrix);
}
