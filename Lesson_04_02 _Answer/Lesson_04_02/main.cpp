#include "stdafx.h"
#include "graphics/SkinModel.h"
#include "System.h"


GraphicsEngine* g_graphicsEngine = NULL;	//�O���t�B�b�N�X�G���W���B
CMatrix g_viewMatrix = CMatrix::Identity();		//�r���[�s��B
CMatrix g_projMatrix = CMatrix::Identity();		//�v���W�F�N�V�����s��B

///////////////////////////////////////////////
//���̃N���X�B
///////////////////////////////////////////////
class Floor {
private:
	CVector3 position = { 0, 0, 0 };	//���W�B
	SkinModel model;				//���f���B
	int timer = 0;
	CVector3 moveSpeed = { 4, 0, 0 };
	CMatrix worldMatrix;			//���[���h�s��B
public:
	void Init()
	{
		//CMO�t�@�C�������[�h����B
		model.Init(L"Assets/modelData/floor.cmo");
	}
	CMatrix GetWorldMatrix()
	{
		//model�����[���h�s���ێ����Ă���̂ŁA�����Ԃ��B
		return model.GetWorldMatrix();
	}
	void Upate()
	{
		timer++;
		if (timer == 120) {
			//120�t���[�����o�߂�����A�ړ����x�𔽓]������B
			moveSpeed *= -1.0f;
			//�^�C�}�[�̓��Z�b�g�B
			timer = 0;
		}
		//�ړ�������B
		position += moveSpeed;
		//���̃��[���h�s����X�V����B
		model.UpdateWorldMatrix(position, CQuaternion::Identity(), CVector3::One());
	}
	void Draw()
	{
		model.Draw(g_viewMatrix, g_projMatrix);
	}
};


///////////////////////////////////////////////
//�v���C���[�N���X�B
///////////////////////////////////////////////
class Player {
private:
	CVector3 position = CVector3::Zero();		//���W�B
	SkinModel model;							//���f���B
	Floor* parent = nullptr;					//�e�B
public:
	void Init(Floor* p)
	{
		//���f�������[�h�B
		model.Init(L"Assets/modelData/unityChan.cmo");
		//�e���֘A�t������B
		parent = p;
	}
	void Update()
	{
		//Hands-On-1 �܂��͕��ʂɍ��W�𓮂����Ă݂�B
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

		//Hands-On-2 ���ʂɃ��[���h�s������ƁE�E�E
		CMatrix mWorld;
		mWorld.MakeTranslation(position);

		//Hands-On-3 ���̃��[���h�s�����Z���āA�q���ɉe����^���Ă݂�B
		mWorld.Mul(mWorld, parent->GetWorldMatrix());
		
		//Hands-On-4 �s������f���ɐݒ肷��B
		model.SetWorldMatrix(mWorld);

	}
	void Draw()
	{
		model.Draw(g_viewMatrix, g_projMatrix);
	}
};



//�v���C���[�̃C���X�^���X�B
Player g_player;
//���̃C���X�^���X�B
Floor g_floor;

///////////////////////////////////////////////////////////////////
// ���t���[���Ă΂��Q�[���̍X�V�����B
///////////////////////////////////////////////////////////////////
void Update()
{
	//���̍X�V���������s�B
	g_floor.Upate();

	//�v���C���[�̍X�V���������s�B
	g_player.Update();
}
///////////////////////////////////////////////////////////////////
// ���t���[���Ă΂��Q�[���̕`�揈���B
///////////////////////////////////////////////////////////////////
void Render()
{
	
	g_graphicsEngine->BegineRender();
	///////////////////////////////////////////
	//�������烂�f���\���̃v���O�����B
	//3D���f����`�悷��B

	//����\������B
	g_floor.Draw();
	//�v���C���[��\������B
	g_player.Draw();

	//�����܂Ń��f���\���Ɋ֌W����v���O�����B
	///////////////////////////////////////////
	g_graphicsEngine->EndRender();
	
}

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�̏������B
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	//DirectX�̏������B
	g_graphicsEngine = new GraphicsEngine();
	g_graphicsEngine->Init(g_hWnd);
	
	//�J�����s����쐬�B
	g_viewMatrix.MakeLookAt(
		{ 0.0f, 400.0f, 800.0f },	//���_�B
		{ 0.0f, 70.0f, 0.0f },		//�����_�B
		{ 0.0f, 1.0f, 0.0f }		//������B
	);
	//�v���W�F�N�V�����s����쐬�B
	g_projMatrix.MakeProjectionMatrix(
		CMath::DegToRad(60.0f),				//��p�B
		FRAME_BUFFER_W/FRAME_BUFFER_H ,		//�A�X�y�N�g��B
		1.0f,								//�ߕ��ʁB
		10000.0f								//�����ʁB
	);

	//�v���C���[�Ə�������������B
	g_floor.Init();
	g_player.Init(&g_floor);

	//���b�Z�[�W�\���̂̕ϐ�msg���������B
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)	//���b�Z�[�W���[�v
	{
		//�E�B���h�E����̃��b�Z�[�W���󂯎��B
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//�X�V�����B
			Update();
			//�`�揈���B
			Render();
		}
	}
}