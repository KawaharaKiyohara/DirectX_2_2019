#include "stdafx.h"
#include "graphics/SkinModel.h"
//Hands-On 1 skeleton.hをインクルード。
#include "graphics/Skeleton.h"

///////////////////////////////////////////////////////////////////
// グローバル変数。
///////////////////////////////////////////////////////////////////
HWND			g_hWnd = NULL;				//ウィンドウハンドル。
GraphicsEngine* g_graphicsEngine = NULL;	//グラフィックスエンジン。


CMatrix g_viewMatrix = CMatrix::Identity();		//ビュー行列。
CMatrix g_projMatrix = CMatrix::Identity();		//プロジェクション行列。
CMatrix g_worldMatrix = CMatrix::Identity();	//ワールド行列。

SkinModel g_model;

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

CVector3 g_modelPosition = {0.0f, 0.0f, 0.0f};
CVector3 g_modelScale = {1.0f, 1.0f, 1.0f};
CVector3 g_modelRotationAngle = {0.0f, 0.0f, 0.0f}; //今回はオイラー角の回転をつかう。
///////////////////////////////////////////////////////////////////
// 毎フレーム呼ばれるゲームの更新処理。
///////////////////////////////////////////////////////////////////
void Update()
{
	//十字キー入力でモデルの座標を動かす。
	if (GetAsyncKeyState(VK_LEFT)) {
		g_modelPosition.x += 0.6f;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		g_modelPosition.x -= 0.6f;
	}

	if (GetAsyncKeyState(VK_UP)) {
		g_modelPosition.y += 0.6f;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		g_modelPosition.y -= 0.6f;
	}

	//WASDで回転
	if (GetAsyncKeyState('W')) {
		g_modelRotationAngle.x += 0.01f;
	}
	if (GetAsyncKeyState('S')) {
		g_modelRotationAngle.x -= 0.01f;
	}

	if (GetAsyncKeyState('A')) {
		g_modelRotationAngle.z += 0.01f;
	}
	if (GetAsyncKeyState('D')) {
		g_modelRotationAngle.z -= 0.01f;
	}
	//2468で拡大
	if (GetAsyncKeyState('2')) {
		g_modelScale.y += 0.01f;
	}
	if (GetAsyncKeyState('8')) {
		g_modelScale.y += 0.01f;
	}
	if (GetAsyncKeyState('4')) {
		g_modelScale.x += 0.01f;
	}
	if (GetAsyncKeyState('6')) {
		g_modelScale.x += 0.01f;
	}

	CMatrix mTrans, mRotX, mRotY, mRotZ, mScale;

	//平行移動行列を作成。
	mTrans.MakeTranslation(g_modelPosition);
	//X軸周りの回転行列を作成。
	mRotX.MakeRotationX(g_modelRotationAngle.x);
	//Y軸周りの回転行列を作成。
	mRotY.MakeRotationY(g_modelRotationAngle.y);
	//Z軸周りの回転行列を作成。
	mRotZ.MakeRotationZ(g_modelRotationAngle.z);
	//拡大行列を作成。
	mScale.MakeScaling(g_modelScale);

	//ワールド行列を作ってみよう。
	CMatrix mWorld;

	//①　拡大→Z軸周りの回転→平行移動
	mWorld.Mul(mScale, mRotZ);
	mWorld.Mul(mWorld, mTrans);
	
	//ハンズオン-1　拡大→平行移動→Z軸周りの回転
	/*mWorld.Mul(mScale, mTrans);
	mWorld.Mul(mWorld, mRotZ);*/

	//ハンズオン-2　Z軸周りの回転→平行移動→拡大。
//	mWorld.Mul(mRotZ, mTrans);
//	mWorld.Mul(mWorld, mScale);

	//ハンズオン-3　X軸周りの回転→Z軸周りの回転
//	mWorld.Mul(mRotX, mRotZ);

	//ハンズオン-4  Z軸周りの回転→X軸周りの回転
//	mWorld.Mul(mRotZ, mRotX);

	g_model.SetWorldMatrix(mWorld);
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

	g_model.Draw(g_viewMatrix, g_projMatrix);

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
		{ 0.0f, 70.0f, 200.0f },	//視点。
		{ 0.0f, 70.0f, 0.0f },		//注視点。
		{ 0.0f, 1.0f, 0.0f }		//上方向。
	);
	//プロジェクション行列を作成。
	g_projMatrix.MakeProjectionMatrix(
		CMath::DegToRad(60.0f),				//画角。
		FRAME_BUFFER_W/FRAME_BUFFER_H ,		//アスペクト比。
		1.0f,								//近平面。
		10000.0f								//遠平面。
	);

	g_model.Init(L"Assets/modelData/unityChan.cmo");

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