#include "stdafx.h"
#include "graphics/SkinModel.h"
#include "System.h"


GraphicsEngine* g_graphicsEngine = NULL;	//グラフィックスエンジン。
CMatrix g_viewMatrix = CMatrix::Identity();		//ビュー行列。
CMatrix g_projMatrix = CMatrix::Identity();		//プロジェクション行列。

///////////////////////////////////////////////
//床のクラス。
///////////////////////////////////////////////
class Floor {
private:
	CVector3 position = { 0, 0, 0 };	//座標。
	SkinModel model;				//モデル。
	int timer = 0;
	CVector3 moveSpeed = { 4, 0, 0 };
	CMatrix worldMatrix;			//ワールド行列。
public:
	void Init()
	{
		//CMOファイルをロードする。
		model.Init(L"Assets/modelData/floor.cmo");
	}
	CMatrix GetWorldMatrix()
	{
		//modelがワールド行列を保持しているので、それを返す。
		return model.GetWorldMatrix();
	}
	void Upate()
	{
		timer++;
		if (timer == 120) {
			//120フレームが経過したら、移動速度を反転させる。
			moveSpeed *= -1.0f;
			//タイマーはリセット。
			timer = 0;
		}
		//移動させる。
		position += moveSpeed;
		//床のワールド行列を更新する。
		model.UpdateWorldMatrix(position, CQuaternion::Identity(), CVector3::One());
	}
	void Draw()
	{
		model.Draw(g_viewMatrix, g_projMatrix);
	}
};


///////////////////////////////////////////////
//プレイヤークラス。
///////////////////////////////////////////////
class Player {
private:
	CVector3 position = CVector3::Zero();		//座標。
	SkinModel model;							//モデル。
	Floor* parent = nullptr;					//親。
public:
	void Init(Floor* p)
	{
		//モデルをロード。
		model.Init(L"Assets/modelData/unityChan.cmo");
		//親を関連付けする。
		parent = p;
	}
	void Update()
	{
		//Hands-On-1 まずは普通に座標を動かしてみる。
		if (GetAsyncKeyState('A')) {
			position.x += 2.0f;
		}
		if (GetAsyncKeyState('D')) {
			position.x -= 2.0f;
		}
		if (GetAsyncKeyState('S')) {
			position.z += 2.0f;
		}
		if (GetAsyncKeyState('W')) {
			position.z -= 2.0f;
		}

		//Hands-On-2 普通にワールド行列を作ると・・・
		CMatrix mWorld;
		mWorld.MakeTranslation(position);

		//Hands-On-3 床のワールド行列を乗算して、子供に影響を与えてみる。
		mWorld.Mul(mWorld, parent->GetWorldMatrix());
		
		//Hands-On-4 行列をモデルに設定する。
		model.SetWorldMatrix(mWorld);

	}
	void Draw()
	{
		model.Draw(g_viewMatrix, g_projMatrix);
	}
};



//プレイヤーのインスタンス。
Player g_player;
//床のインスタンス。
Floor g_floor;

///////////////////////////////////////////////////////////////////
// 毎フレーム呼ばれるゲームの更新処理。
///////////////////////////////////////////////////////////////////
void Update()
{
	//床の更新処理を実行。
	g_floor.Upate();

	//プレイヤーの更新処理を実行。
	g_player.Update();
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

	//床を表示する。
	g_floor.Draw();
	//プレイヤーを表示する。
	g_player.Draw();

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
		{ 0.0f, 400.0f, 800.0f },	//視点。
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

	//プレイヤーと床を初期化する。
	g_floor.Init();
	g_player.Init(&g_floor);

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