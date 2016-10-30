#include "DirectSound.h"

//�R���X�g���N�^
DirectSound::DirectSound()
{
	pDirectSound8 = NULL;
}

//�f�X�g���N�^
DirectSound::~DirectSound()
{
	Release();
}

//�J��
void DirectSound::Release()
{
	if (pDirectSound8 != NULL)
	{
		pDirectSound8->Release();
		pDirectSound8 = NULL;
	}
}

//�T�E���h�f�o�C�X�̍쐬�Ƌ������x���̐ݒ�
bool DirectSound::Create(HWND hWnd)
{ 
	//�f�o�C�X�쐬
	if(FAILED(DirectSoundCreate8(NULL, &pDirectSound8, NULL)))
	{
		//�쐻���s
		Release();
		return false;
	}

	//�������x���̐ݒ�@
	//�܂��@����炷�E�B���h�E�̐ݒ�
	if (FAILED(pDirectSound8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		Release();
		return false;

	}

	return true;

}

HRESULT DirectSound::CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer** ppBuffer)
{
	if (pDirectSound8 == NULL)
	{
		//�f�o�C�X���쐬����Ă��Ȃ����ߎ��s
		return S_FALSE;
	}

	
	return pDirectSound8->CreateSoundBuffer(pDesc, ppBuffer,NULL);

}
