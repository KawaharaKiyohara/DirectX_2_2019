/*!
 *@brief	スケルトン
 */
#include "stdafx.h"
#include "graphics/skeleton.h"

#include <comdef.h> 

void Bone::CalcWorldTRS(CVector3& trans, CQuaternion& rot, CVector3& scale)
{
	CMatrix mWorld = m_worldMatrix;
	//行列から拡大率を取得する。
	scale.x = mWorld.v[0].Length();
	scale.y = mWorld.v[1].Length();
	scale.z = mWorld.v[2].Length();
	m_scale = scale;
	//行列から平行移動量を取得する。
	trans.Set(mWorld.v[3]);
	m_positoin = trans;
	//行列から拡大率と平行移動量を除去して回転量を取得する。
	mWorld.v[0].Normalize();
	mWorld.v[1].Normalize();
	mWorld.v[2].Normalize();
	mWorld.v[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	rot.SetRotation(mWorld);
	m_rotation = rot;
}
Skeleton::Skeleton()
{
	//リザーブ。
	m_bones.reserve(BONE_MAX);
}
Skeleton::~Skeleton()
{
	//ちゃんとメモリは解放しましょう。
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++) {
		delete m_bones[boneNo];
	}
}
void Skeleton::UpdateBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix)
{
	//ワールド行列を計算する。
	CMatrix mBoneWorld;
	CMatrix localMatrix = bone.GetLocalMatrix();
	mBoneWorld.Mul(localMatrix, parentMatrix);

	bone.SetWorldMatrix(mBoneWorld);

	//子供のワールド行列も計算する。
	std::vector<Bone*>& children = bone.GetChildren();
	for (int childNo = 0; childNo < children.size(); childNo++ ) {
		//UpdateBoneWorldMatrixを再帰呼び出し。
		UpdateBoneWorldMatrix(*children[childNo], mBoneWorld);
	}
}
bool Skeleton::Load(const wchar_t* filePath)
{
	//tksファイルをオープン。
	FILE* fp = _wfopen(filePath, L"rb");

	if (fp == nullptr) {
		return false;
	}
	//骨の数を取得。
	int numBone = 0;
	//tksファイルの先頭から、4バイト読み込む。
	//先頭４バイトに骨の数のデータが入っている。
	fread(&numBone, sizeof(numBone), 1, fp);
	//骨を一本一本読み込んでいく。
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		int nameCount = 0;
		//骨の名前の文字数を読み込む。
		fread(&nameCount, 1, 1, fp);
		//骨の名前を読み込めるだけのメモリを確保する。
		char* name = new char[nameCount + 1];
		//骨の名前を読み込み。+1は終端文字列を読み込むため
		fread(name, nameCount+1, 1, fp);
		//親の骨番号を取得。
		int parentNo;
		fread(&parentNo, sizeof(parentNo),  1, fp);
		//バインドポーズを取得。
		CVector3 bindPose[4];
		fread(&bindPose, sizeof(bindPose), 1, fp);
		//バインドポーズの逆行列を取得。
		CVector3 invBindPose[4];
		fread(&invBindPose, sizeof(invBindPose), 1, fp);
		
		//バインドポーズを表す行列を作成する。
		CMatrix bindPoseMatrix;
		memcpy(bindPoseMatrix.m[0], &bindPose[0], sizeof(bindPose[0]));
		memcpy(bindPoseMatrix.m[1], &bindPose[1], sizeof(bindPose[1]));
		memcpy(bindPoseMatrix.m[2], &bindPose[2], sizeof(bindPose[2]));
		memcpy(bindPoseMatrix.m[3], &bindPose[3], sizeof(bindPose[3]));
		bindPoseMatrix.m[0][3] = 0.0f;
		bindPoseMatrix.m[1][3] = 0.0f;
		bindPoseMatrix.m[2][3] = 0.0f;
		bindPoseMatrix.m[3][3] = 1.0f;

		//バインドポーズの逆行列。
		CMatrix invBindPoseMatrix;
		memcpy(invBindPoseMatrix.m[0], &invBindPose[0], sizeof(invBindPose[0]));
		memcpy(invBindPoseMatrix.m[1], &invBindPose[1], sizeof(invBindPose[1]));
		memcpy(invBindPoseMatrix.m[2], &invBindPose[2], sizeof(invBindPose[2]));
		memcpy(invBindPoseMatrix.m[3], &invBindPose[3], sizeof(invBindPose[3]));
		invBindPoseMatrix.m[0][3] = 0.0f;
		invBindPoseMatrix.m[1][3] = 0.0f;
		invBindPoseMatrix.m[2][3] = 0.0f;
		invBindPoseMatrix.m[3][3] = 1.0f;
		
		//マルチバイト文字列をワイド文字列に変換する。
		wchar_t boneName[256];
		mbstowcs(boneName, name, 256);
		//tksファイルからロードしたボーン情報を使って、新しいボーンを作成。
		Bone* bone = new Bone(
			boneName,			//ボーンの名前。
			bindPoseMatrix,		//バインドポーズを表す行列。
			invBindPoseMatrix,	//バインドポーズを表す行列の逆行列。
			parentNo,			//親の番号。
			boneNo				//ボーン番号。
		);
		//ちゃんと不要になったら削除。
		delete[] name;
		//ファイルから読み込んで、作成した骨の情報をリストにプッシュバック。
		m_bones.push_back(bone);
	}

	//ファイルは開いたら、ちゃんと閉じる。。
	fclose(fp);

	//スケルトンの読み込みが完了したので、親の座標系での行列を求める。
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++)  {
		Bone* bone = m_bones[boneNo];
		if (bone->GetParentId() != -1) {
			//親がいる。
			m_bones.at(bone->GetParentId())->AddChild(bone);
			//親の逆行列を取得する。h
			const CMatrix& parentMatrix = m_bones.at(bone->GetParentId())->GetInvBindPoseMatrix();
			//骨のバインドポーズの行列に、親の逆行列を乗算して、親の座標系での行列を求める。
			CMatrix localMatrix;
			localMatrix.Mul(bone->GetBindPoseMatrix(), parentMatrix);
			bone->SetLocalMatrix(localMatrix);
		}
		else {
			//親がいない。
			bone->SetLocalMatrix(bone->GetBindPoseMatrix());
		}
	}

	//ボーン行列を確保。これは後々必要になってくる行列。
	//まだ使ってないので無視してください。
	m_boneMatrixs.resize(m_bones.size());

	return true;
}


void Skeleton::Update(const CMatrix& mWorld)
{
	//ワールド行列を構築していく
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++) {
		Bone* bone = m_bones[boneNo];
		if (bone->GetParentId() != -1) {
			continue;
		}
		//ルートが見つかったので、ボーンのワールド行列を計算していく。
		UpdateBoneWorldMatrix(*bone, mWorld);
	}

	//ボーン行列を計算。これは後々必要になってくる行列。
	//まだ使ってないので無視してください。
	for (int boneNo = 0; boneNo < m_bones.size(); boneNo++) {
		Bone* bone = m_bones[boneNo];
		CMatrix mBone;
		mBone.Mul(bone->GetInvBindPoseMatrix(), bone->GetWorldMatrix());
		m_boneMatrixs[boneNo] = mBone;
		boneNo++;
	}
	
}
