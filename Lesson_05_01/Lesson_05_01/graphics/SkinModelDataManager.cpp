#include "stdafx.h"
#include "SkinModelDataManager.h"

//g_skinModelDataManager�̎��ԁB
SkinModelDataManager g_skinModelDataManager;

DirectX::Model* SkinModelDataManager::Load(const wchar_t* filePath)
{
	DirectX::Model* retModel = NULL;
	//�}�b�v�ɓo�^����Ă��邩���ׂ�B
	auto it = m_directXModelMap.find(filePath);
	if (it == m_directXModelMap.end()) {
		//���o�^�Ȃ̂ŁA�V�K�Ń��[�h����B
		//�G�t�F�N�g�t�@�N�g���B
		DirectX::EffectFactory effectFactory(g_graphicsEngine->GetD3DDevice());

		//CMO�t�@�C���̃��[�h�B
		auto model = DirectX::Model::CreateFromCMO(	//CMO�t�@�C�����烂�f�����쐬����֐��́ACreateFromCMO�����s����B
			g_graphicsEngine->GetD3DDevice(),			//��������D3D�f�o�C�X�B
			filePath,									//�������͓ǂݍ���CMO�t�@�C���̃t�@�C���p�X�B
			effectFactory,								//��O�����̓G�t�F�N�g�t�@�N�g���B
			false										//��l������Cull���[�h�B���͋C�ɂ��Ȃ��Ă悢�B
		);
		retModel = model.get();
		//�V�K�Ȃ̂Ń}�b�v�ɓo�^����B
		m_directXModelMap.insert({ filePath, std::move(model) });
	}
	else {
		//�o�^����Ă���̂ŁA�ǂݍ��ݍς݂̃f�[�^���擾�B
		retModel = it->second.get();
	}
	return retModel;
}

void SkinModelDataManager::Release()
{
	//map����ɂ���B
	m_directXModelMap.clear();
}