#include "Application.h"

#include "../Direct3D/Direct3D.h"

#include "../Input/directInput.h"
#include "../Input/XInput.h"


//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	//���b�Z�[�W�����̂��߂̊֐�
	//DispatchMessage �ɂ���ČĂяo�����

	//�E�B���h�E�N���X�o�^���Ɋe�E�B���h�E�ɐݒ肳���
	//�A�v���P�[�V���������b�Z�[�W���擾������Ăяo�����悤�ɂ���

	//���b�Z�[�W�̗�
	//�E�B���h�E�̉E��́~�������ꂽ WM_DESTROY
	//�E�B���h�E�̈ړ� WM_MOVE
	//�E�B���h�E�T�C�Y�̕ύX WM_SIZE
	//��


	//���ʂȏ�����K�v�Ƃ��郁�b�Z�[�W�́@�Ƃ肠�����I���������������Ă���
	switch (msg)
	{
	case WM_DESTROY: //�E��̃o�c�������ꂽ�AAltF4�������ꂽ���ȂǂɎ�M���郁�b�Z�[�W
		PostQuitMessage(0);//���b�Z�[�W�L���[��WM_QUIT���b�Z�[�W�𑗂�
		break;

	}

	//���ʂȏ�����v���Ȃ����b�Z�[�W�̓f�t�H���g�̃E�B���h�E�v���V�[�W������������
	return DefWindowProc(hWnd, msg, wParam, lParam);//�ړ��Ƃ��T�C�Y�ύX�Ƃ�
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
	updateFunc = NULL;
}

Application::~Application()
{
	
};


bool Application::RegistBasicWindowClass()
{
	
	//-----------
	//�E�B���h�E�N���X�̒�`�ƍ쐬
	//----------

	hInstance = GetModuleHandle(NULL);

	//�V���v���E�B���h�E�N���X�ݒ�
	//WNDCLASSEX wcex =
	//{
	//	sizeof(WNDCLASSEX),						// cbSize : �\���̂̃T�C�Y
	//	CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,	// style : �E�B���h�E�X�^�C��
	//	(WNDPROC)WndProc,							// ipfnWndProc : �E�B���h�E�v���V�[�W���̃A�h���X
	//	0,										// cbClsExtra : 0�Œ�
	//	0,										// cbWndExtra : 0�Œ�
	//	hInstance,								// hInstance : WinMain�̃C���X�^���X�n���h��
	//	
	//	(HICON)LoadImage(NULL,					// hIcon : �g�p����A�C�R��
	//	MAKEINTRESOURCE(IDI_APPLICATION),
	//	IMAGE_ICON,
	//	0,
	//	0,
	//	LR_DEFAULTSIZE | LR_SHARED),

	//	(HCURSOR)LoadImage(NULL,					// hCursor :  �}�E�X�J�[�\��
	//	MAKEINTRESOURCE(IDC_ARROW),
	//	IMAGE_CURSOR,
	//	0,
	//	0,
	//	LR_DEFAULTSIZE | LR_SHARED),
	//	(HBRUSH)(COLOR_WINDOW + 1)
	//	/*(HBRUSH)GetStockObject(WHITE_BRUSH)*/,	// hbrBackground : �E�B���h�E�N���C�A���g�̈�̔w�i�F
	//	NULL,										// lpszMeueName : ���j���[
	//	WC_BASIC,									// lpszClassName : �E�B���h�E�N���X�̖��O CreateWindow()�̑������Ɠ���
	//	NULL										// hIconSm : �g�p����A�C�R��(��)
	//};

	//��̃R�����g�A�E�g�̕������킩��₷���悤�ɐ���
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));						//�������̊m��

	wcex.cbSize			=sizeof(WNDCLASSEX);					// cbSize : �\���̂̃T�C�Y
	wcex.cbClsExtra		= 0;									// cbClsExtra : 0�Œ�
	wcex.cbWndExtra		= 0;									// cbWndExtra : 0�Œ�
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// style : �E�B���h�E�X�^�C��
	wcex.hInstance		= hInstance;							// hInstance : WinMain�̃C���X�^���X�n���h��
	wcex.lpfnWndProc	= (WNDPROC)WndProc;						// ipfnWndProc : �E�B���h�E�v���V�[�W���̃A�h���X�@�֐��|�C���^
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	// hbrBackground : �E�B���h�E�N���C�A���g�̈�̔w�i�F
	wcex.lpszMenuName	= NULL;									// lpszMeueName : ���j���[
	wcex.lpszClassName  = WC_BASIC;								// lpszClassName : �E�B���h�E�N���X�̖��O
	
	//�A�C�R��
	wcex.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm =NULL;
	//�J�[�\��
	wcex.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	//�V���v���E�B���h�E�N���X�쐬
	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	return true;


	//�E�B���h�E�N���X�X�^�C�� http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html
	/*�@CS_HREDRAW	�F	���T�C�Y���ς�����Ƃ��C�E�C���h�E�S�̂��ĕ`�悷��
		CS_VREDRAW	�F	�c�T�C�Y���ς�����Ƃ��C�E�C���h�E�S�̂��ĕ`�悷��
		CS_DBLCLKS	�F	�_�u���N���b�N�������������Ƃ��E�C���h�E�ɒʒm����
		CS_NOCLOSE	�F	�V�X�e�����j���[�́m����n�R�}���h���g�p�s�ɂ���
		CS_CLASSDC	�F	���̃N���X�̃E�C���h�E���m�łP�̃f�o�C�X�R���e�L�X�g�����L����
		CS_OWNDC	�F	�E�C���h�E���Ƀf�o�C�X�R���e�L�X�g���P�Â���
		CS_PARENTDC	�F	�e�E�C���h�E�̃f�o�C�X�R���e�L�X�g���g�p����
		CS_GLOBALCLASS	�F	���̃N���X���쐬�����A�v���P�[�V���������s����Ă���΁C ���̃A�v���P�[�V��������ł����̃N���X�𗘗p�ł���悤�ɂȂ�D ��ʓI��DLL�ŃJ�X�^���R���g���[�����쐬����Ƃ��Ɏg���炵���D*/
}

bool Application::CreateWind(
	const TCHAR	*Title,
	int		Width,
	int		Height,
	int		X,
	int		Y,
	HWND	hParentWnd,
	HMENU	hMenu,
	const	TCHAR* ClassName,//�f�t�H���g�� �ϐ� WC_BASIC
	DWORD	Style,
	DWORD	ExStyle
	)
{
	//�w�b�_�[�t�@�C���̃f�t�H���g�������`�F�b�N
	
	//�N���C�A���g�̈�T�C�Y����ɂ��ăE�B���h�E�̑傫�������߂�
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = Width;
	rect.bottom = Height;

	//�w�肵���N���C�A���g�̈��ێ�����傫���̃E�B���h�E�T�C�Y���v�Z
	AdjustWindowRect(&rect, Style, TRUE);

	//�E�B���h�E�̍쐬
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

	//hWnd�@�E�B���h�E�n���h���@�����̃E�B���h�E����ʂ��邽�߂̂���

	//�E�B���h�E�쐬���s
	if (hWnd == NULL)
	{
		return false;
	}


	//// �E�C���h�E�\�� //0623�ǉ�
	ShowWindow(hWnd, SW_SHOWNORMAL);//�E�B���h�E���A�N�e�B�u�ɂ��ĕ\��
	UpdateWindow(hWnd);				//�N���C�A���g�̈�̍X�V

	return true;

	//CreateWindowEx�́@�X�^�C���ڍ� http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html
	/*�@
	WS_OVERLAPPED	�F	�^�C�g���o�[�C���E�����I�[�o�[���b�v�E�C���h�E
	WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIMZEBOX
	WS_CHILD	�F	�q�E�C���h�E�DWS_POPUP�Ƃ͕��p�ł��Ȃ�
	WS_POPUP	�F	�|�b�v�A�b�v�E�C���h�E�DWS_CHILD�Ƃ͕��p�ł��Ȃ�
	WS_POPUPWINDOW = WS_BORDER | WS_POPUP | WS_SYSMENU
	WS_CLIPCHILDREN	�F	�e�̕`��Ŏq�E�C���h�E�̕������N���b�s���O
	WS_CLIPSIBLINGS	�F	�q�̕`��ŌZ��E�C���h�E�̕������N���b�s���O
	WS_CAPTION	�F	�^�C�g���o�[(WS_DLGFRAME���ꏏ�ɂ��Ă���)
	WS_SYSMENU	�F	�^�C�g���o�[�ɃV�X�e�����j���[(����{�^���ƃA�C�R��)����
	WS_MINIMIZEBOX	�F	�V�X�e�����j���[�ɍŏ����{�^��������
	WS_MAXIMIZEBOX	�F	�V�X�e�����j���[�ɍő剻�{�^��������
	WS_BORDER	�F	�ׂ����E��
	WS_DLGFRAME	�F	�T�C�Y�ύX�ł��Ȃ����E��
	WS_THICKFRAME	�F	�T�C�Y�ύX�ł��鋫�E��
	WS_HSCROLL	�F	horizon scroll�C�܂萅�������X�N���[��
	WS_VSCROLL	�F	vertical scroll�C�܂萂�������X�N���[��
	WS_VISIBLE	�F	������Ԃŕ\�������
	WS_DISABLED	�F	������Ԃ��g�p�s��
	WS_MAXIMIZE	�F	������Ԃōő剻
	WS_MINIMIZE	�F	������Ԃōŏ���
	WS_TABSTOP	�F	�_�C�A���O�̃R���g���[���ɂ����DTab�Ńt�H�[�J�X�𓖂Ă���悤�ɂ���D
	WS_GROUP	�F	��Ƀ_�C�A���O�̃��W�I�{�^���̃O���[�v�ݒ�Ɏg���D
	*/
}

void Application::MessageLoop()
{


	DWORD startTime, endTime, passTime;
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//�v���O������message�������Ă��邩���ׂ� 
													 //message�L���[����msg�Ƀ��b�Z�[�W����������
													 //���b�Z�[�W�L���[���烁�b�Z�[�W���폜����(PM_REMOVE���ݒ肳��Ă���Ƃ�)
		{
			//���b�Z�[�W����
			TranslateMessage(&msg); //���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ����A��������b�Z�[�W�L���[�Ƀ|�X�g����
			DispatchMessage(&msg);	//���b�Z�[�W�̊��蓖�ā@�E�B���h�E�v���V�[�W���Ń��b�Z�[�W����������
		}
		else
		{
			//���b�Z�[�W���������Ă��Ȃ��Ƃ��ɂ��鏈�� (���b�Z�[�W�L���[�ɉ��������Ă��Ȃ��Ƃ�)

			startTime = timeGetTime();//�t���[���̎��s���Ԍv���p


			//���� key and mouse
			DirectInput& di = DirectInput::GetInstance();
			if (di.Initialized() == false)
			{
				di.Init(hWnd);
			}

			//���̓Q�[���p�b�h
			XInput& xi = XInput::GetInstance();
			xi.Update();
						
			//���͂̃A�b�v�f�[�g
			di.Update();

			
			if (updateFunc != NULL)
			{
				updateFunc();//�A�b�v�f�[�g�̎��s
			}

			if (ExcessTime == 0)
			{

				Direct3D::Render();//�`��̎��s
			}

			//�t���[�����[�g�Œ艻����
			endTime = timeGetTime();
			passTime = endTime - startTime;

			if (miriSecondPerFrame >= (passTime + ExcessTime))
			{
				//wait�֐����̃��b�Z�[�W���[�v�Ł@WM_QUIT���󂯎�����ꍇ�@������̃��b�Z�[�W���[�v�����������
				bool continueMessaggeLoop =
					Wait(miriSecondPerFrame - (passTime + ExcessTime));//������1/60�b�Ɉ��s�����Ƃ�ۂ��߂ɑ҂�

				if (continueMessaggeLoop == false)
				{
					break;
				}

				ExcessTime = 0;
			}
			else
			{
				//miriSeconndPerFrame�𒴂��Ă��܂������ԁ@�҂����Ɏ��̏����ց@
				//���������̓R�}�����i�`�揈�����s��Ȃ��j
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
