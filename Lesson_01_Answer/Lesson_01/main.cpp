#include "stdafx.h"


///////////////////////////////////////////////////////////////////
// グローバル変数。
///////////////////////////////////////////////////////////////////
HWND			g_hWnd = NULL;				//ウィンドウハンドル。
GraphicsEngine* g_graphicsEngine = NULL;	//グラフィックスエンジン。

std::unique_ptr<DirectX::Model> g_model;		//モデル。
CMatrix g_viewMatrix = CMatrix::Identity();		//ビュー行列。
CMatrix g_projMatrix = CMatrix::Identity();		//プロジェクション行列。
CMatrix g_worldMatrix = CMatrix::Identity();	//ワールド行列。

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
}
///////////////////////////////////////////////////////////////////
// 毎フレーム呼ばれるゲームの描画処理。
///////////////////////////////////////////////////////////////////
void Render()
{
	
	g_graphicsEngine->BegineRender();
	//X軸周りに-180°回す。
	g_worldMatrix.MakeRotationX(CMath::PI * -0.5f);
	///////////////////////////////////////////
	//ここからモデル表示のプログラム。
	//3Dモデルを描画する。
	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());
	g_model->Draw(
		g_graphicsEngine->GetD3DDeviceContext(),//D3Dデバイスコンテキスト。
		state,									//レンダリングステート。今は気にしなくてよい。
		g_worldMatrix,							//ワールド行列。
		g_viewMatrix,							//ビュー行列。
		g_projMatrix							//プロジェクション行列。
	);
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
		{ 0.0f, 0.0f, 500.0f },		//視点。
		{ 0.0f, 0.0f, 0.0f },		//注視点。
		{ 0.0f, 1.0f, 0.0f }		//上方向。
	);
	//プロジェクション行列を作成。
	g_projMatrix.MakeProjectionMatrix(
		CMath::DegToRad(60.0f),				//画角。
		FRAME_BUFFER_W/FRAME_BUFFER_H ,		//アスペクト比。
		1.0f,								//近平面。
		1000.0f								//遠平面。
	);
	/////////////////////////////////////////////////////////
	//ここからcmoファイルのロードに関係するプログラム。
	//エフェクトファクトリ。
	DirectX::EffectFactory effectFactory(g_graphicsEngine->GetD3DDevice());
	//テクスチャがあるフォルダを設定する。
	effectFactory.SetDirectory(L"Resource/modelData");
	//CMOファイルのロード。
	g_model = DirectX::Model::CreateFromCMO(	//CMOファイルからモデルを作成する関数の、CreateFromCMOを実行する。
		g_graphicsEngine->GetD3DDevice(),			//第一引数はD3Dデバイス。
		L"Resource/modelData/unityChan.cmo",		//第二引数は読み込むCMOファイルのファイルパス。
		effectFactory,								//第三引数はエフェクトファクトリ。
		false										//第四引数はCullモード。今は気にしなくてよい。
	);
	//ここまでcmoファイルのロードに関係するプログラム。
	/////////////////////////////////////////////////////////
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