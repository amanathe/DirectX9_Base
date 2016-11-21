#include "XInput.h"

int XInput::GamePadState::ButtonCode[14]=
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};


//�C���i�[�N���X�@�R���X�g���N�^
XInput::GamePadState::GamePadState()
{
	for (int i = 0; i < 14; i++)
	{
		buttonCount[i] = buttonCountPrev[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{

		Stick s = { 0.0f, 0.0f };
		stick[i] = s;

		trigger[i] = 0;
	}
}

//�C���i�[�N���X�@�X�V
void XInput::GamePadState::Update(int PadNum)
{
	//1�t���[���O��Counter��ۑ�
	for (int i = 0; i < 16; i++)
	{	
		buttonCountPrev[i] = buttonCount[i];
	}


	//���͏�Ԃ̎擾�����܂����������ǂ���
	DWORD funcResult;
	
	//���͏�Ԃ̎擾 (�R���g���[���ԍ�0-3 , ��Ԃ�ۑ��������\���̂̃A�h���X)
	funcResult = XInputGetState(PadNum,&state);

	if (funcResult == ERROR_SUCCESS)
	{
		//����

		conected = true;

		//�{�^���̍X�V
		for (int i = 0; i < 14; i++)
		{
			int code = ButtonCode[i];
			if ((state.Gamepad.wButtons & code) != 0)
			{
				buttonCount[i]++;
			}
			else
			{
				buttonCount[i] = 0;
			}
		}

		//�X�e�b�N�̍X�V
		for (int i = 0; i < 2; i++)
		{
			int x = (i == 0) ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX;
			int y = (i == 0) ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;

			//�f�b�h�]�[�����̐��l��0�ց@�������@xy�̂ǂ��炩���f�b�h�]�[���̊O�Ȃ�ǂ�����ύX���Ȃ�
			if (x<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&x>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
				&& y<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && y>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				x = y = 0;
			}


			//X�̓��͒l�� -1~1��
			if (x < 0)
			{
				stick[i].x = (float)x / (float)32768;
			}
			else if (x>0)
			{
				stick[i].x = (float)x / (float)32767;
			}
			else
			{
				//x=0�̏ꍇ
				stick[i].x = 0;
			}

			//Y�̓��͒l�� -1~1��
			if (y < 0)
			{
				stick[i].y = (float)y / (float)32768;
				
			}
			else if (y>0)
			{
				stick[i].y = (float)y/ (float)32767;
			}
			else
			{
				//y=0�̏ꍇ
				stick[i].y = 0;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			int x = (i == 0) ? state.Gamepad.bLeftTrigger : state.Gamepad.bRightTrigger;

			if (x > 10)//�f�b�h�]�[������O��Ă��邩
			{

				trigger[i] = (float)x / (float)255;
			}
			else
			{
				trigger[i] = 0.0f;
			}
		}

	}
	else
	{
		//���s
		conected = false;

		//�R���g���[���ɂ܂������G���Ă��Ȃ��̂Ɠ�����Ԃɂ���

		for (int i = 0; i < 16; i++)
		{
			 buttonCount[i] =0;
		}

		for (int i = 0; i < 2; i++)
		{
			stick[i].x = 0;
			stick[i].y = 0;
		}

		for (int i = 0; i < 2; i++)
		{
			trigger[i] = 0;
		}
	}
}


//�C���i�[�N���X
//�����ꂽ�u�Ԃ�
bool XInput::GamePadState::GetButtonDown(int buttonNum)
{
	if (buttonNum >= 14 || buttonNum < 0)
	{
		return false;
	}
		
	if (buttonCount[buttonNum] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


//�C���i�[�N���X
//�����ꂽ�u�Ԃ�
bool XInput::GamePadState::GetButtonRelease(int buttonNum)
{
	if (buttonNum >= 14 || buttonNum < 0)
	{
		return false;
	}

	if (buttonCount[buttonNum] == 0 && buttonCountPrev[buttonNum] >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//�C���i�[�N���X
//�����ꑱ�����t���[����
int  XInput::GamePadState::GetButtonCount(int buttonNum)
{
	if (buttonNum >= 14 || buttonNum < 0)
	{
		return 0;
	}
	
	return buttonCount[buttonNum];

}


//�C���i�[�N���X
//�X�e�B�b�N�̌X���擾
Stick XInput::GamePadState::GetStick(int stickNum)
{
	if (stickNum >= 0 && stickNum < 2)
	{
		return stick[stickNum];
	}
	else
	{
		Stick s={ 0, 0 };
		return s;
	}
}

//�g���K�[�̏�Ԏ擾�@������Ă��Ȃ���0 ���S�ɉ������܂�Ă����1  ���Ԓn��0~1�̏�����
float XInput::GamePadState::GetTrigger(int triggerNum)
{
	if (triggerNum >= 0 && triggerNum < 2)
	{
		return trigger[triggerNum];
	}
	else
	{
		
		return 0;
	}
}


//�R���X�g���N�^
XInput::XInput()
{


}

//�f�X�g���N�^
XInput::~XInput()
{

}


//�S�ẴR���g���[�����͂̏�Ԃ��X�V
void XInput::Update()
{
	for (int i = 0; i < 4; i++)
	{
		state[i].Update(i);
	}
}


//�{�^��������������u�Ԃ�
bool XInput::GetButtonDown(int padNum, int buttonNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return false;
	}

	return state[padNum].GetButtonDown(buttonNum);
}

//�{�^���������ꂽ�u�Ԃ�
bool XInput::GetButtonRelease(int padNum, int buttonNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return false;
	}
	return state[padNum].GetButtonDown(buttonNum);
}

//�{�^���������ꑱ�����t���[����
int XInput::GetButtonCount(int padNum, int buttonNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return 0;
	}
	return state[padNum].GetButtonDown(buttonNum);
}

//�X�e�B�b�N�̌X��
Stick XInput::GetStick(int padNum, int stickNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		Stick s={ 0, 0 };

		return s;
	}

	return state[padNum].GetStick(stickNum);
}

//�g���K�[�̏��
float XInput::GetTrigger(int padNum, int triggerNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return 0;
	}

	return state[padNum].GetTrigger(triggerNum);
}
