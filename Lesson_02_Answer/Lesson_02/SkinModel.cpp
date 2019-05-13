#include "stdafx.h"
#include "SkinModel.h"

void SkinModel::Init(const wchar_t* filePath)
{
	//エフェクトファクトリ。
	DirectX::EffectFactory effectFactory(g_graphicsEngine->GetD3DDevice());
	//テクスチャがあるフォルダを設定する。
	effectFactory.SetDirectory(L"Resource/modelData");
	//CMOファイルのロード。
	m_modelDx = DirectX::Model::CreateFromCMO(	//CMOファイルからモデルを作成する関数の、CreateFromCMOを実行する。
		g_graphicsEngine->GetD3DDevice(),			//第一引数はD3Dデバイス。
		filePath,									//第二引数は読み込むCMOファイルのファイルパス。
		effectFactory,								//第三引数はエフェクトファクトリ。
		false										//第四引数はCullモード。今は気にしなくてよい。
	);
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
	m_modelDx->Draw(
		g_graphicsEngine->GetD3DDeviceContext(),
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}