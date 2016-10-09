#include "Application.h"


//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//Application�N���X�@�ÓI�ϐ��̏�����
//�C���X�^���X�n���h��
HINSTANCE Application::hInstance = NULL;

const DWORD  Application::FPS = 60;
const DWORD  Application::miriSecondPerFrame = 1000 / FPS;

DWORD  Application::ExcessTime = 0;
bool   Application::pathed = false;

//Application�N���X�֐���`

Application::Application()
{
	//�R���X�g���N�^

	char szBuff[256];
	wsprintf(szBuff, "Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("�쐬"), MB_OK);
}

Application::~Application()
{
	//�f�X�g���N�^ //overload

	char szBuff[256];
	wsprintf(szBuff, "�J��Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("�J��"), MB_OK);
};


bool Application::RegistBasicWindowClass()
{
	
	//-----------
	//�E�B���h�E�N���X�̒�`�ƍ쐬
	//----------

	hInstance = GetModuleHandle(NULL);

	//�V���v���E�B���h�E�N���X�ݒ�
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// cbSize : �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,	// style : �E�B���h�E�X�^�C��
		(WNDPROC)WndProc,							// ipfnWndProc : �E�B���h�E�v���V�[�W���̃A�h���X
		0,										// cbClsExtra : 0�Œ�
		0,										// cbWndExtra : 0�Œ�
		hInstance,								// hInstance : WinMain�̃C���X�^���X�n���h��
		
		(HICON)LoadImage(NULL,					// hIcon : �g�p����A�C�R��
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED),

		(HCURSOR)LoadImage(NULL,				// hCursor :  �}�E�X�J�[�\��
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED),
		(HBRUSH)(COLOR_WINDOW + 1)
		/*(HBRUSH)GetStockObject(WHITE_BRUSH)*/,	// hbrBackground : �E�B���h�E�N���C�A���g�̈�̔w�i�F
		NULL,										// lpszMeueName : ���j���[
		WC_BASIC,									// lpszClassName : �E�B���h�E�N���X�̖��O CreateWindow()�̑������Ɠ���
		NULL										// hIconSm : �g�p����A�C�R��(��)
	};

	//�V���v���E�B���h�E�N���X�쐬
	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	return true;

}

bool Application::CreateWind(
	const TCHAR	*Title,
	int		Width,
	int		Height,
	int		X,
	int		Y,
	HWND	hParentWnd,
	HMENU	hMenu,
	const	TCHAR* ClassName,
	DWORD	Style,
	DWORD	ExStyle
	)
{


	//--
	//�E�B���h�E�̍쐬
	//--

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = Width;
	rect.bottom = Height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	hWnd = CreateWindowEx(
		ExStyle,
		ClassName,
		Title,
		Style,
		X,
		Y,
		rect.right - rect.left,
		rect.bottom - rect.top,
		hParentWnd,
		hMenu,
		hInstance,
		NULL
		);

	//�E�B���h�E�쐬���s
	if (hWnd == NULL)
	{
		return false;
	}


	//// �E�C���h�E�\�� //0623�ǉ�
	//ShowWindow(hWnd, SW_SHOWNORMAL);
	//UpdateWindow(hWnd);

	return true;
}

void Application::MessageLoop()
{
	DWORD startTime, endTime, passTime;
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W����
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			//���b�Z�[�W���������Ă��Ȃ��Ƃ��ɂ��鏈��

			startTime = timeGetTime();//�t���[���̎��s���Ԍv���p	


			//�����ɏ���������




			//�t���[�����[�g�Œ艻����
			endTime = timeGetTime();
			passTime = endTime - startTime;

			if (miriSecondPerFrame >= (passTime + ExcessTime))
			{
				bool continueMessaggeLoop =
					Wait(miriSecondPerFrame - (passTime + ExcessTime));

				if (continueMessaggeLoop == false)
				{
					break;
				}

				ExcessTime = 0;
			}
			else
			{
				//miriSeconndPerFrame�𒴂��Ă��܂�������
				//���̓R�}����
				ExcessTime = (passTime + ExcessTime) - miriSecondPerFrame;
			}
		}
	}
}


//�t���[�����[�g�Œ菈���̍ۂ̑҂�
//�߂�l�̓��b�Z�[�W���[�v���p�����邩�ǂ���
//WM_QUIT�̎�����false��Ԃ�
bool Application::Wait(DWORD wait_time)
{
	MSG msg = {};
	DWORD startTime = timeGetTime();

	do
	{
		if (msg.message == WM_QUIT)return false;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W����
			TranslateMessage(&msg);
			DispatchMessage(&msg);	// �A�v���P�[�V�����̊e�E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W��]������
		}
		if (wait_time > 0)Sleep(1);

	} while (timeGetTime() < startTime + wait_time);

	return true;
}
