#pragma once 

#define D3D_DEBUG_INFO	//Direct3D�f�o�b�O�t���O

#define DIRECTINPUT_VERSION 0x0800

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1
#define MOUSE_BUTTON_MIDDLE 2


#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>

#include "../other/MyVector.h"
#include "../Template/Singleton.h"

#define INPUT_BUFFER_SIZE 256


class DirectInput :public Singleton<DirectInput>
{
	friend class Singleton<DirectInput>;
private:

	LPDIRECTINPUT8		 pDInput;			//directinput �I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 pKeyDevice;		//�L�[�{�[�h�p�f�o�C�X
	LPDIRECTINPUTDEVICE8 pMouseDevice;		//�}�E�X�p�f�o�C�X

	//�L�[�{�[�h���
	int keyInput[INPUT_BUFFER_SIZE];
	int keyInputPreview[INPUT_BUFFER_SIZE];
	

	//�}�E�X���
	DIMOUSESTATE2 mouseInput;
	DIMOUSESTATE2 mouseInputPreview;
	int mouseButtonCounter[8];

	Vector2 mousePos;
	Vector2 mousePosPreview;
	Vector2 mousePosDelta;
	
	HWND		  hWnd;

	bool initialized;

private:

	HRESULT InitKey();//�L�[�{�[�h�̏�����
	HRESULT InitMouse();

	//�R���X�g���N�^�f�X�g���N�^
	DirectInput();
	~DirectInput();

public:


	//����������
	HRESULT Init(HWND hWnd);

	//�L�[��Ԃ̍X�V����
	HRESULT Update();

	//�������
	HRESULT Release();	

	bool Initialized(){ return initialized; }

	//�l�擾�֐�

	bool KeyState(int code);
	bool KeyStatePreview(int code);
	bool KeyDown(int code);
	bool KeyUp(int code);
	int KeyCount(int code);

	DIMOUSESTATE2 MouseState(){ return  mouseInput; }
	DIMOUSESTATE2 MoouseStatePreview(){ return mouseInputPreview; }

	Vector2 MousePosition(){ return mousePos; }
	Vector2 MousePositionPreview(){ return mousePosPreview; }
	Vector2 MousePositionDelta(){ return mousePosDelta; }

	bool MouseButton(int num);
	bool MouseButtonDown(int num);
	bool MouseButtonUP(int num);
	int MouseButtonCount(int num);
};
