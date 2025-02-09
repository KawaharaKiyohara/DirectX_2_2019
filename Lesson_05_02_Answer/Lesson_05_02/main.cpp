#include "stdafx.h"
#include "graphics/SkinModel.h"
//Hands-On 1 skeleton.hをインクルード。
#include "graphics/Skeleton.h"

///////////////////////////////////////////////////////////////////
// グローバル変数。
///////////////////////////////////////////////////////////////////
HWND			g_hWnd = NULL;				//ウィンドウハンドル。
GraphicsEngine* g_graphicsEngine = NULL;	//グラフィックスエンジン。

SkinModel g_unityChanModel;						//スキンモデル。

CMatrix g_viewMatrix = CMatrix::Identity;		//ビュー行列。
CMatrix g_projMatrix = CMatrix::Identity;		//プロジェクション行列。
CMatrix g_worldMatrix = CMatrix::Identity;	//ワールド行列。

///////////////////////////////////////////////////////////////////
// DirectXの終了処理。
///////////////////////////////////////////////////////////////////
void ReleaseDirectX()
{
	
}
///////////////////////////////////////////////////////////////////
//メッセージプロシージャ。
//hWndがメッセージを送ってきたウィンドウのハンドル。
//msgがメッセージの種類。
//wParamとlParamは引数。今は気にしなくてよい。
///////////////////////////////////////////////////////////////////
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//送られてきたメッセージで処理を分岐させる。
	switch (msg)
	{
	case WM_DESTROY:
		ReleaseDirectX();
		PostQuitMessage(0);
		break;	
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////
// ウィンドウの初期化。
///////////////////////////////////////////////////////////////////
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		//構造体のサイズ。
		CS_CLASSDC,				//ウィンドウのスタイル。
								//ここの指定でスクロールバーをつけたりできるが、ゲームでは不要なのでCS_CLASSDCでよい。
		MsgProc,				//メッセージプロシージャ(後述)
		0,						//0でいい。
		0,						//0でいい。
		GetModuleHandle(NULL),	//このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
								//何も気にしなくてよい。
		NULL,					//アイコンのハンドル。アイコンを変えたい場合ここを変更する。とりあえずこれでいい。
		NULL,					//マウスカーソルのハンドル。NULLの場合はデフォルト。
		NULL,					//ウィンドウの背景色。NULLの場合はデフォルト。
		NULL,					//メニュー名。NULLでいい。
		TEXT("Sample_01"),		//ウィンドウクラスに付ける名前。
		NULL					//NULLでいい。
	};
	//ウィンドウクラスの登録。
	RegisterClassEx(&wc);

	// ウィンドウの作成。
	g_hWnd = CreateWindow(
		TEXT("Sample_01"),		//使用するウィンドウクラスの名前。
								//先ほど作成したウィンドウクラスと同じ名前にする。
		TEXT("Sample_01"),		//ウィンドウの名前。ウィンドウクラスの名前と別名でもよい。
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい、
		0,						//ウィンドウの初期X座標。
		0,						//ウィンドウの初期Y座標。
		(UINT)FRAME_BUFFER_W,	//ウィンドウの幅。
		(UINT)FRAME_BUFFER_H,	//ウィンドウの高さ。
		NULL,					//親ウィンドウ。ゲームでは基本的にNULLでいい。
		NULL,					//メニュー。今はNULLでいい。
		hInstance,				//アプリケーションのインスタンス。
		NULL
	);

	ShowWindow(g_hWnd, nCmdShow);

}

///////////////////////////////////////////////////////////////////
// 毎フレーム呼ばれるゲームの更新処理。
///////////////////////////////////////////////////////////////////
void Update()
{
	//ワールド行列の更新。
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisX, -CMath::PI * 0.5f);
	//Hands-On UnityちゃんをY軸周りに回転させる。
	
	//static変数は永続的な寿命を持つ。
	static float angleY = 0.0f;	
	//Y軸周りに回転させる行列を作成。
	CQuaternion qRotY;
	qRotY.SetRotation(CVector3::AxisY, angleY);
	angleY += 0.03f;
	//X軸周りに回してから、Y軸周りで回す。
	//クォータニオンは行列とは掛け算の順番が逆になるので注意！！！
	qRot.Multiply(qRotY, qRot);
	
	g_unityChanModel.UpdateWorldMatrix(CVector3::Zero, qRot, CVector3::One);
	//Hands-On ろくろ首Unityちゃん！
	//ボーンを名前で検索。
	Bone* bone = g_unityChanModel.FindBone(L"Character1_Neck");
	//ローカルマトリクスを取得。
	CMatrix localMatrix = bone->GetLocalMatrix();
	if (GetAsyncKeyState(VK_LEFT)) {
		//左に1.0平行移動する行列を作成。
		CMatrix mTrans;
		mTrans.MakeTranslation({ 1.0f, 0.0f, 0.0f });
		//平行移動する行列をローカル行列に乗算する。
		localMatrix.Mul(localMatrix, mTrans);
		//変更したローカル行列をボーンに反映。
		bone->SetLocalMatrix(localMatrix);
	}
	//実習-1　キーボードの右が入力されたときに、首を右に動かす。
	if (GetAsyncKeyState(VK_RIGHT)) {
		//右に1.0平行移動する行列を作成。
		CMatrix mTrans;
		mTrans.MakeTranslation({ -1.0f, 0.0f, 0.0f });
		//平行移動する行列をローカル行列に乗算する。
		localMatrix.Mul(localMatrix, mTrans);
		//変更したローカル行列をボーンに反映。
		bone->SetLocalMatrix(localMatrix);
	}
	//実習-2　キーボードの上が入力されたときに、首を上に動かす。
	//        ※ヒント　3dsMaxでは上方向はZ軸。つまり・・・
	if (GetAsyncKeyState(VK_UP)) {
		//上に1.0平行移動する行列を作成。
		CMatrix mTrans;
		mTrans.MakeTranslation({ 0.0f, 0.0f, 1.0f });
		//左に0.1平行移動する行列をローカル行列に乗算する。
		localMatrix.Mul(localMatrix, mTrans);
		//変更したローカル行列をボーンに反映。
		bone->SetLocalMatrix(localMatrix);
	}
	//実習-3　キーボードの下が入力されたときに、首を下に動かす。
	//        ※ヒント　3dsMaxでは上方向はZ軸。つまり・・・
	if (GetAsyncKeyState(VK_DOWN)) {
		//下に1.0平行移動する行列を作成。
		CMatrix mTrans;
		mTrans.MakeTranslation({ 0.0f, -0.0f, -0.1f });
		//左に1.0平行移動する行列をローカル行列に乗算する。
		localMatrix.Mul(localMatrix, mTrans);
		//変更したローカル行列をボーンに反映。
		bone->SetLocalMatrix(localMatrix);
	}
		
}
///////////////////////////////////////////////////////////////////
// 毎フレーム呼ばれるゲームの描画処理。
///////////////////////////////////////////////////////////////////
void Render()
{
	
	g_graphicsEngine->BegineRender();
	///////////////////////////////////////////
	//ここからモデル表示のプログラム。
	//3Dモデルを描画する。
	g_unityChanModel.Draw(g_viewMatrix, g_projMatrix);

	//ここまでモデル表示に関係するプログラム。
	///////////////////////////////////////////
	g_graphicsEngine->EndRender();
	
}

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウの初期化。
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	//DirectXの初期化。
	g_graphicsEngine = new GraphicsEngine();
	g_graphicsEngine->Init(g_hWnd);
	
	//カメラ行列を作成。
	g_viewMatrix.MakeLookAt(
		{ 0.0f, 100.0f, 300.0f },	//視点。
		{ 0.0f, 100.0f, 0.0f },		//注視点。
		{ 0.0f, 1.0f, 0.0f }		//上方向。
	);
	//プロジェクション行列を作成。
	g_projMatrix.MakeProjectionMatrix(
		CMath::DegToRad(60.0f),				//画角。
		FRAME_BUFFER_W/FRAME_BUFFER_H ,		//アスペクト比。
		1.0f,								//近平面。
		10000.0f							//遠平面。
	);

	g_unityChanModel.Init(L"Assets/modelData/unityChan.cmo");

	//メッセージ構造体の変数msgを初期化。
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)	//メッセージループ
	{
		//ウィンドウからのメッセージを受け取る。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//更新処理。
			Update();
			//描画処理。
			Render();
		}
	}
}