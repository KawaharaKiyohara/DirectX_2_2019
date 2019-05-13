#include "stdafx.h"
#include "SkinModel.h"

void SkinModel::Init(const wchar_t* filePath)
{
	//�G�t�F�N�g�t�@�N�g���B
	DirectX::EffectFactory effectFactory(g_graphicsEngine->GetD3DDevice());
	//�e�N�X�`��������t�H���_��ݒ肷��B
	effectFactory.SetDirectory(L"Resource/modelData");
	//CMO�t�@�C���̃��[�h�B
	m_modelDx = DirectX::Model::CreateFromCMO(	//CMO�t�@�C�����烂�f�����쐬����֐��́ACreateFromCMO�����s����B
		g_graphicsEngine->GetD3DDevice(),			//��������D3D�f�o�C�X�B
		filePath,									//�������͓ǂݍ���CMO�t�@�C���̃t�@�C���p�X�B
		effectFactory,								//��O�����̓G�t�F�N�g�t�@�N�g���B
		false										//��l������Cull���[�h�B���͋C�ɂ��Ȃ��Ă悢�B
	);
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//���s�ړ��s����쐬����B
	transMatrix.MakeTranslation( position );
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion( rotation );
	//�g��s����쐬����B
	scaleMatrix.MakeScaling(scale);
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);
}
void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
{
	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
	m_modelDx->Draw(
		g_graphicsEngine->GetD3DDeviceContext(),
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}