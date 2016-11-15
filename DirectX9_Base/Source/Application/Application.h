#pragma once

#include <Windows.h>

#include <tchar.h>
#include <windowsx.h>
#include <commctrl.h>
#include <WinUser.h>

#pragma comment (lib,"comctl32.lib")
#pragma comment (lib,"winmm.lib")

#include "../Template/Singleton.h"


static const TCHAR * WC_BASIC = _T("Basic");	//��{�E�B���h�E�N���X��

class Application :public Singleton<Application>
{
	friend class Singleton<Application>;

public:
	typedef void(*FuncPointer)();//�֐��|�C���^�̌^��`

//�ϐ�
private:
	static HINSTANCE hInstance;	//�C���X�^���X�n���h��
	HWND hWnd;					//�E�B���h�E�n���h��

	static const DWORD FPS;
	static const DWORD miriSecondPerFrame;
	static DWORD ExcessTime;
	static bool pathed;

	FuncPointer updateFunc;

//�֐�
public :

	static bool RegistBasicWindowClass();		//�E�B���h�E�N���X�̒�`�ƍ쐬

	bool CreateWind(							//�@�߂�l�@true ����   false �쐬���s
		const TCHAR	*Title,						//�@�^�C�g��
		int		Width = CW_USEDEFAULT,			//�@��
		int		Height = CW_USEDEFAULT,			//�@����
		int		X = CW_USEDEFAULT,				//�@X���W
		int		Y = CW_USEDEFAULT,				//�@Y���W
		HWND	hParentWnd = NULL,				//�@�e�E�B���h�E�n���h��
		HMENU	hMenu = NULL,					//	���j���[�n���h���@�������̓��\�[�XID
		const	TCHAR* ClassName = WC_BASIC,	//	�E�B���h�E�N���X��
		DWORD	Style = (WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_VISIBLE,		//	�E�B���h�E�X�^�C��
		DWORD	ExStyle = 0						//	�E�B���h�E�g���X�^�C��
		);

	HWND GetWindowHandle(){ return hWnd; }

	void MessageLoop();
	bool Wait(DWORD wait_time);

	void SetUpdateFuncPointer(FuncPointer fp){ updateFunc = fp; };
	
protected:



private:
	Application();		//�R���X�g���N�^
	~Application();		//�f�X�g���N�^�̃I�[�o�[���[�h




};
