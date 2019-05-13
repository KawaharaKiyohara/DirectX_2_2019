/*!
 *@brief	�X�P���g��
 */
#include "stdafx.h"
#include "graphics/skeleton.h"

#include <comdef.h> 

void Bone::CalcWorldTRS(CVector3& trans, CQuaternion& rot, CVector3& scale)
{
	CMatrix mWorld = m_worldMatrix;
	//�s�񂩂�g�嗦���擾����B
	scale.x = mWorld.v[0].Length();
	scale.y = mWorld.v[1].Length();
	scale.z = mWorld.v[2].Length();
	m_scale = scale;
	//�s�񂩂畽�s�ړ��ʂ��擾����B
	trans.Set(mWorld.v[3]);
	m_positoin = trans;
	//�s�񂩂�g�嗦�ƕ��s�ړ��ʂ��������ĉ�]�ʂ��擾����B
	mWorld.v[0].Normalize();
	mWorld.v[1].Normalize();
	mWorld.v[2].Normalize();
	mWorld.v[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	rot.SetRotation(mWorld);
	m_rotation = rot;
}
Skeleton::Skeleton()
{
	//���U�[�u�B
	m_bones.reserve(BONE_MAX);
}
Skeleton::~Skeleton()
{
	//�����ƃ������͉�����܂��傤�B
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++) {
		delete m_bones[boneNo];
	}
}
void Skeleton::UpdateBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix)
{
	//���[���h�s����v�Z����B
	CMatrix mBoneWorld;
	CMatrix localMatrix = bone.GetLocalMatrix();
	mBoneWorld.Mul(localMatrix, parentMatrix);

	bone.SetWorldMatrix(mBoneWorld);

	//�q���̃��[���h�s����v�Z����B
	std::vector<Bone*>& children = bone.GetChildren();
	for (int childNo = 0; childNo < children.size(); childNo++ ) {
		//UpdateBoneWorldMatrix���ċA�Ăяo���B
		UpdateBoneWorldMatrix(*children[childNo], mBoneWorld);
	}
}
bool Skeleton::Load(const wchar_t* filePath)
{
	//tks�t�@�C�����I�[�v���B
	FILE* fp = _wfopen(filePath, L"rb");

	if (fp == nullptr) {
		return false;
	}
	//���̐����擾�B
	int numBone = 0;
	//tks�t�@�C���̐擪����A4�o�C�g�ǂݍ��ށB
	//�擪�S�o�C�g�ɍ��̐��̃f�[�^�������Ă���B
	fread(&numBone, sizeof(numBone), 1, fp);
	//������{��{�ǂݍ���ł����B
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		int nameCount = 0;
		//���̖��O�̕�������ǂݍ��ށB
		fread(&nameCount, 1, 1, fp);
		//���̖��O��ǂݍ��߂邾���̃��������m�ۂ���B
		char* name = new char[nameCount + 1];
		//���̖��O��ǂݍ��݁B+1�͏I�[�������ǂݍ��ނ���
		fread(name, nameCount+1, 1, fp);
		//�e�̍��ԍ����擾�B
		int parentNo;
		fread(&parentNo, sizeof(parentNo),  1, fp);
		//�o�C���h�|�[�Y���擾�B
		CVector3 bindPose[4];
		fread(&bindPose, sizeof(bindPose), 1, fp);
		//�o�C���h�|�[�Y�̋t�s����擾�B
		CVector3 invBindPose[4];
		fread(&invBindPose, sizeof(invBindPose), 1, fp);
		
		//�o�C���h�|�[�Y��\���s����쐬����B
		CMatrix bindPoseMatrix;
		memcpy(bindPoseMatrix.m[0], &bindPose[0], sizeof(bindPose[0]));
		memcpy(bindPoseMatrix.m[1], &bindPose[1], sizeof(bindPose[1]));
		memcpy(bindPoseMatrix.m[2], &bindPose[2], sizeof(bindPose[2]));
		memcpy(bindPoseMatrix.m[3], &bindPose[3], sizeof(bindPose[3]));
		bindPoseMatrix.m[0][3] = 0.0f;
		bindPoseMatrix.m[1][3] = 0.0f;
		bindPoseMatrix.m[2][3] = 0.0f;
		bindPoseMatrix.m[3][3] = 1.0f;

		//�o�C���h�|�[�Y�̋t�s��B
		CMatrix invBindPoseMatrix;
		memcpy(invBindPoseMatrix.m[0], &invBindPose[0], sizeof(invBindPose[0]));
		memcpy(invBindPoseMatrix.m[1], &invBindPose[1], sizeof(invBindPose[1]));
		memcpy(invBindPoseMatrix.m[2], &invBindPose[2], sizeof(invBindPose[2]));
		memcpy(invBindPoseMatrix.m[3], &invBindPose[3], sizeof(invBindPose[3]));
		invBindPoseMatrix.m[0][3] = 0.0f;
		invBindPoseMatrix.m[1][3] = 0.0f;
		invBindPoseMatrix.m[2][3] = 0.0f;
		invBindPoseMatrix.m[3][3] = 1.0f;
		
		//�}���`�o�C�g����������C�h������ɕϊ�����B
		wchar_t boneName[256];
		mbstowcs(boneName, name, 256);
		//tks�t�@�C�����烍�[�h�����{�[�������g���āA�V�����{�[�����쐬�B
		Bone* bone = new Bone(
			boneName,			//�{�[���̖��O�B
			bindPoseMatrix,		//�o�C���h�|�[�Y��\���s��B
			invBindPoseMatrix,	//�o�C���h�|�[�Y��\���s��̋t�s��B
			parentNo,			//�e�̔ԍ��B
			boneNo				//�{�[���ԍ��B
		);
		//�����ƕs�v�ɂȂ�����폜�B
		delete[] name;
		//�t�@�C������ǂݍ���ŁA�쐬�������̏������X�g�Ƀv�b�V���o�b�N�B
		m_bones.push_back(bone);
	}

	//�t�@�C���͊J������A�����ƕ���B�B
	fclose(fp);

	//�X�P���g���̓ǂݍ��݂����������̂ŁA�e�̍��W�n�ł̍s������߂�B
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++)  {
		Bone* bone = m_bones[boneNo];
		if (bone->GetParentId() != -1) {
			//�e������B
			m_bones.at(bone->GetParentId())->AddChild(bone);
			//�e�̋t�s����擾����Bh
			const CMatrix& parentMatrix = m_bones.at(bone->GetParentId())->GetInvBindPoseMatrix();
			//���̃o�C���h�|�[�Y�̍s��ɁA�e�̋t�s�����Z���āA�e�̍��W�n�ł̍s������߂�B
			CMatrix localMatrix;
			localMatrix.Mul(bone->GetBindPoseMatrix(), parentMatrix);
			bone->SetLocalMatrix(localMatrix);
		}
		else {
			//�e�����Ȃ��B
			bone->SetLocalMatrix(bone->GetBindPoseMatrix());
		}
	}

	//�{�[���s����m�ہB����͌�X�K�v�ɂȂ��Ă���s��B
	//�܂��g���ĂȂ��̂Ŗ������Ă��������B
	m_boneMatrixs.resize(m_bones.size());

	return true;
}


void Skeleton::Update(const CMatrix& mWorld)
{
	//���[���h�s����\�z���Ă���
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++) {
		Bone* bone = m_bones[boneNo];
		if (bone->GetParentId() != -1) {
			continue;
		}
		//���[�g�����������̂ŁA�{�[���̃��[���h�s����v�Z���Ă����B
		UpdateBoneWorldMatrix(*bone, mWorld);
	}

	//�{�[���s����v�Z�B����͌�X�K�v�ɂȂ��Ă���s��B
	//�܂��g���ĂȂ��̂Ŗ������Ă��������B
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++) {
		Bone* bone = m_bones[boneNo];
		CMatrix mBone;
		mBone.Mul(bone->GetInvBindPoseMatrix(), bone->GetWorldMatrix());
		m_boneMatrixs[boneNo] = mBone;
		boneNo++;
	}
	
}
