#pragma once

#include "../Template/Singleton.h"

#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

//�{�^���Ɣz��@�Y�����̊֌W
#define GAMEPAD_DPAD_UP			0
#define GAMEPAD_DPAD_DOWN		1
#define GAMEPAD_DPAD_LEFT		2
#define GAMEPAD_DPAD_RIGHT		3	
#define GAMEPAD_START			4
#define GAMEPAD_BACK			5
#define GAMEPAD_LEFT_THUMB		6
#define GAMEPAD_RIGHT_THUMB		7	
#define GAMEPAD_LEFT_SHOULDER	8
#define GAMEPAD_RIGHT_SHOULDER	9	
#define GAMEPAD_A				10	
#define GAMEPAD_B				11
#define GAMEPAD_X				12
#define GAMEPAD_Y				13

#define STICK_LEFT		0
#define STICK_RIGHT		1

#define TRIGGER_LEFT	0
#define TRIGGER_RIGHT	1


//�X�e�B�b�N�̌X���@-1~1
struct  Stick
{
	float x;
	float y;
};


//4�̃R���g���[�����͂��܂Ƃ߂ĊǗ�
class XInput :public Singleton<XInput>
{

	//�C���i�[�N���X 
	//�ЂƂ̃R���g���[���̓��͏��
	//private�Ő錾���Ă���̂�
	//���̃N���X�O����͈�؃A�N�Z�X�ł��Ȃ�
	class  GamePadState
	{
		//�{�^���ԍ��ƃ}�X�N�̊֌W
		static int ButtonCode[14];

		//���ꂼ��̃{�^���̉����ꑱ�����t���[����
		int buttonCount[14];
		int buttonCountPrev[14];

		//�R���g���[�����ڑ�����Ă��邩
		bool conected;

		//�g���K�[�{�^���̓��͏�� 0~1
		float trigger[2];

		//�X�e�B�b�N�̌X��
		Stick stick[2];		

		//���͏�ԁ@���H�O
		XINPUT_STATE state;

	public:
		GamePadState();
		~GamePadState();

		//��Ԃ̍X�V
		void Update(int PadNum);

		//�{�^���������ꂽ�u��
		bool GetButtonDown(int buttonNum);

		//�{�^���������ꂽ�u��
		bool GetButtonRelease(int ButtunNum);

		//�{�^���������ꑱ�����t���[����
		int GetButtonCount(int ButtonNum);

		//�g���K�[�̓��͏��
		float GetTrigger(int TriggerNum);

		//�R���g���[���̐ڑ�
		bool Conected(){ return conected; }

		//�X�e�B�b�N�̌X�����
		Stick GetStick(int stickNum);

	};


	//�e�N���X����new �ł���悤�ɂ���
	friend class Singleton<XInput>;

private:

	//�R���g���[���̓��͏��
	//�R���g���[���̐�����
	GamePadState state[4];

public :

	//��Ԃ̍X�V
	void Update();


private:

	XInput();
	~XInput();
	

public:

	//�{�^���������ꂽ�u��
	bool GetButtonDown(int padNum, int buttonNum);
	//�{�^���������ꂽ�u��
	bool GetButtonRelease(int padNum, int buttonNum);

	//�{�^���������ꑱ�����t���[����
	int GetButtonCount(int padNum, int buttonNum);

	//�g���K�[�̓��͏��
	float GetTrigger(int padNum,int TriggerNum);

	//�X�e�B�b�N�̌X�����
	Stick GetStick(int padNum,int stickNum);

};

