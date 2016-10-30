#include "SoundBuffer.h"
#include "WaveFile.h"
#include "DirectSound.h"

//�R���X�g���N�^
SoundBuffer::SoundBuffer()
{
	pSecondaryBuffer = NULL;

}
//�f�X�g���N�^
SoundBuffer::~SoundBuffer()
{
	Release();
}

//�J������
void SoundBuffer::Release()
{
	if (pSecondaryBuffer != NULL)
	{
		Stop();//�J������ۂɂ͍Đ���~����

		pSecondaryBuffer->Release();
		pSecondaryBuffer = NULL;
	}
}

//�Đ��J�n
void SoundBuffer::Play(bool isLoop)
{
	if (pSecondaryBuffer != NULL)
	{
		DWORD LoopFlag = (isLoop ? DSBPLAY_LOOPING : 0);
		pSecondaryBuffer->Play(0, 0, LoopFlag);
	}
}

//�Đ���~
void SoundBuffer::Stop()
{
	if (pSecondaryBuffer != NULL)
	{
		pSecondaryBuffer->Stop();
	}
}



//�o�b�t�@�쐬

bool SoundBuffer::Create(WaveFile& waveFile)
{
	//desc : description
	DSBUFFERDESC desc = {};//�Z�J���_���o�b�t�@�쐬�p�ݒ�

	//�`�����l�����ł̕���
	//���m������1�`�����l��
	//�X�e���I��2�`�����l��

	if (waveFile.waveFormat.nChannels==1)
	{//���m����
		desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;

		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else
	{//�X�e���I
		desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC | DSBCAPS_CTRLPAN;
		// |DSBCAPS_CTRFX; �G�t�F�N�g��ǉ������Duplicate�ł��Ȃ�

		desc.guid3DAlgorithm = GUID_NULL;
	}

	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwBufferBytes = waveFile.dataSize;//���f�[�^�T�C�Y
	desc.lpwfxFormat = &waveFile.waveFormat;//�t�H�[�}�b�g�w��

	//�f�o�C�X�擾
	DirectSound & directSound = DirectSound::GetInstance();

	//�T�E���h�o�b�t�@
	IDirectSoundBuffer* pBuffer = NULL;

	//�v���C�}���o�b�t�@�쐻
	if (directSound.CreateSoundBuffer(&desc,&pBuffer))
	{
		//�n���ꂽwave�f�[�^�ɖ�肪����
		//����������Ă��Ȃ���
		//���[�h���s
		//�̉\��

		return false;
	}

	//�T�E���h�o�b�t�@����Z�J���_���o�b�t�@�쐬
	//IDirectSoundBuffer���� IDirectSoundBuffer8�ɕϊ�����
	pBuffer->QueryInterface(IID_IDirectSoundBuffer8,(void **)&pSecondaryBuffer);
	pBuffer->Release();//�Z�J���_���o�b�t�@���쐬�o����΃o�b�t�@�͔j�����č\��Ȃ�


	unsigned char *block1 = NULL;
	unsigned char *block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;

	//�Z�J���_���o�b�t�@�����b�N���ăf�[�^����������
	pSecondaryBuffer->Lock(0, waveFile.dataSize, (void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);

	//�Z�J���_���o�b�t�@�ɉ��f�[�^���R�s�[
	memcpy(block1, waveFile.waveData, waveFile.dataSize);

	//�Z�J���_���o�b�t�@�̃��b�N����
	pSecondaryBuffer->Unlock(block1, blockSize1, block2, 0);

	return true;
}