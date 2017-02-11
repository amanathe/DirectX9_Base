
#include <Windows.h>
#pragma comment(lib, "winmm.lib")


#include "Template/Singleton.h"
#include "Application/Application.h"
#include "Direct3D/Direct3D.h"
#include "DirectSound/DirectSound.h"
#include "Game/GameState.h"

#include "Scene/Manager/SceneManager.h"

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�⃁�C�����[�v�̎��s���Ǘ�����N���X�̃C���X�^���X���쐻
	Application& App = Application::GetInstance();
	
	App.RegistBasicWindowClass();							//�E�B���h�E�N���X�̓o�^
	App.CreateWind(_T("�E�B���h�E�^�C�g��"), 1024, 780);	//�o�^�����E�B���h�E�N���X����ɃE�B���h�E���쐬

	//�f�o�C�X���Ǘ�����N���X�̃C���X�^���X���쐬
	Direct3D& D3d = Direct3D::GetInstance();

	//�`��f�o�C�X�̍쐬
	//Application�ō쐬�����E�B���h�E�̃n���h������A�`��̈�̐ݒ���s��
	if (D3d.TryCreate(App.GetWindowHandle()))
	{
		MessageBox(NULL, "3D�f�o�C�X�쐬����", TEXT("WinMain"), MB_OK);
	}
	else
	{
		MessageBox(NULL, "3D�f�o�C�X�쐬���s", TEXT("WinMain"), MB_OK);
	}

	DirectSound& DSound = DirectSound::GetInstance();
	DSound.Create(App.GetWindowHandle());


	GameState& GameState = GameState::GetInstance();

	App.SetUpdateFuncPointer(GameState::Update_Static);
	D3d.SetDrawFunc(GameState::Draw_Static);

	GameState.Init();

	App.MessageLoop();//���b�Z�[�W���[�v

	//���b�Z�[�W���[�v���I�������̂ŏI�����������ăv���O�������I����
	
	SceneManager::Finalize();
	
	SingletonFinalizer::Finalize();//�V���O���g���ȃN���X�̏I������

	

}