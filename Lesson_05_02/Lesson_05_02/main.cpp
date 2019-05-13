#include "stdafx.h"
#include "graphics/SkinModel.h"
//Hands-On 1 skeleton.h���C���N���[�h�B
#include "graphics/Skeleton.h"

///////////////////////////////////////////////////////////////////
// �O���[�o���ϐ��B
///////////////////////////////////////////////////////////////////
HWND			g_hWnd = NULL;				//�E�B���h�E�n���h���B
GraphicsEngine* g_graphicsEngine = NULL;	//�O���t�B�b�N�X�G���W���B

SkinModel g_unityChanModel;						//�X�L�����f���B

CMatrix g_viewMatrix = CMatrix::Identity;		//�r���[�s��B
CMatrix g_projMatrix = CMatrix::Identity;		//�v���W�F�N�V�����s��B
CMatrix g_worldMatrix = CMatrix::Identity;	//���[���h�s��B

///////////////////////////////////////////////////////////////////
// DirectX�̏I�������B
///////////////////////////////////////////////////////////////////
void ReleaseDirectX()
{
	
}
///////////////////////////////////////////////////////////////////
//���b�Z�[�W�v���V�[�W���B
//hWnd�����b�Z�[�W�𑗂��Ă����E�B���h�E�̃n���h���B
//msg�����b�Z�[�W�̎�ށB
//wParam��lParam�͈����B���͋C�ɂ��Ȃ��Ă悢�B
///////////////////////////////////////////////////////////////////
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�����Ă������b�Z�[�W�ŏ����𕪊򂳂���B
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
// �E�B���h�E�̏������B
///////////////////////////////////////////////////////////////////
void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�N���X�̃p�����[�^��ݒ�(�P�Ȃ�\���̂̕ϐ��̏������ł��B)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		//�\���̂̃T�C�Y�B
		CS_CLASSDC,				//�E�B���h�E�̃X�^�C���B
								//�����̎w��ŃX�N���[���o�[��������ł��邪�A�Q�[���ł͕s�v�Ȃ̂�CS_CLASSDC�ł悢�B
		MsgProc,				//���b�Z�[�W�v���V�[�W��(��q)
		0,						//0�ł����B
		0,						//0�ł����B
		GetModuleHandle(NULL),	//���̃N���X�̂��߂̃E�C���h�E�v���V�[�W��������C���X�^���X�n���h���B
								//�����C�ɂ��Ȃ��Ă悢�B
		NULL,					//�A�C�R���̃n���h���B�A�C�R����ς������ꍇ������ύX����B�Ƃ肠��������ł����B
		NULL,					//�}�E�X�J�[�\���̃n���h���BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					//�E�B���h�E�̔w�i�F�BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					//���j���[���BNULL�ł����B
		TEXT("Sample_01"),		//�E�B���h�E�N���X�ɕt���閼�O�B
		NULL					//NULL�ł����B
	};
	//�E�B���h�E�N���X�̓o�^�B
	RegisterClassEx(&wc);

	// �E�B���h�E�̍쐬�B
	g_hWnd = CreateWindow(
		TEXT("Sample_01"),		//�g�p����E�B���h�E�N���X�̖��O�B
								//��قǍ쐬�����E�B���h�E�N���X�Ɠ������O�ɂ���B
		TEXT("Sample_01"),		//�E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢�B
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C���B�Q�[���ł͊�{�I��WS_OVERLAPPEDWINDOW�ł����A
		0,						//�E�B���h�E�̏���X���W�B
		0,						//�E�B���h�E�̏���Y���W�B
		(UINT)FRAME_BUFFER_W,	//�E�B���h�E�̕��B
		(UINT)FRAME_BUFFER_H,	//�E�B���h�E�̍����B
		NULL,					//�e�E�B���h�E�B�Q�[���ł͊�{�I��NULL�ł����B
		NULL,					//���j���[�B����NULL�ł����B
		hInstance,				//�A�v���P�[�V�����̃C���X�^���X�B
		NULL
	);

	ShowWindow(g_hWnd, nCmdShow);

}

///////////////////////////////////////////////////////////////////
// ���t���[���Ă΂��Q�[���̍X�V�����B
///////////////////////////////////////////////////////////////////
void Update()
{
	//���[���h�s��̍X�V�B
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisX, -CMath::PI * 0.5f);
	//Hands-On Unity������Y������ɉ�]������B
	
	//static�ϐ��͉i���I�Ȏ��������B
/*
	static float angleY = 0.0f;	
	//Y������ɉ�]������s����쐬�B
	CQuaternion qRotY;
	qRotY.SetRotation(CVector3::AxisY, angleY);
	angleY += 0.03f;
	//X������ɉ񂵂Ă���AY������ŉ񂷁B
	//�N�H�[�^�j�I���͍s��Ƃ͊|���Z�̏��Ԃ��t�ɂȂ�̂Œ��ӁI�I�I
	qRot.Multiply(qRotY, qRot);
*/

	g_unityChanModel.UpdateWorldMatrix(CVector3::Zero, qRot, CVector3::One);
	//Hands-On �낭���Unity�����I
	
	//���K-1�@�L�[�{�[�h�̉E�����͂��ꂽ�Ƃ��ɁA����E�ɓ������B
	
	//���K-2�@�L�[�{�[�h�̏オ���͂��ꂽ�Ƃ��ɁA�����ɓ������B
	//        ���q���g�@3dsMax�ł͏������Z���B�܂�E�E�E
	
	//���K-3�@�L�[�{�[�h�̉������͂��ꂽ�Ƃ��ɁA������ɓ������B
	//        ���q���g�@3dsMax�ł͏������Z���B�܂�E�E�E
	
		
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
	g_unityChanModel.Draw(g_viewMatrix, g_projMatrix);

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
		{ 0.0f, 100.0f, 300.0f },	//���_�B
		{ 0.0f, 100.0f, 0.0f },		//�����_�B
		{ 0.0f, 1.0f, 0.0f }		//������B
	);
	//�v���W�F�N�V�����s����쐬�B
	g_projMatrix.MakeProjectionMatrix(
		CMath::DegToRad(60.0f),				//��p�B
		FRAME_BUFFER_W/FRAME_BUFFER_H ,		//�A�X�y�N�g��B
		1.0f,								//�ߕ��ʁB
		10000.0f							//�����ʁB
	);

	g_unityChanModel.Init(L"Assets/modelData/unityChan.cmo");

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