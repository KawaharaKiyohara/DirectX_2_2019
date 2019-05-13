#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"

void SkinModel::Init(const wchar_t* filePath)
{
	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath);
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation( position );
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion( rotation );
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);
}
void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
{
	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
	
	CMatrix mWorld;
	mWorld.MakeRotationX(CMath::PI * -0.5f);
	mWorld.Mul(mWorld, m_worldMatrix);

	m_modelDx->Draw(
		g_graphicsEngine->GetD3DDeviceContext(),
		state,
		mWorld,
		viewMatrix,
		projMatrix
	);
}