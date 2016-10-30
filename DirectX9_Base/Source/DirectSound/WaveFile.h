#pragma once


#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>
#include <tchar.h>
#include <comdef.h>



class WaveFile
{
	//soundbuffer��͕ϐ��Ɏ��R�ɃA�N�Z�X�ł���悤�ɂ���
	friend class SoundBuffer;

private:
	WAVEFORMATEX waveFormat;
	byte*		 waveData;//�g�`�f�[�^
	int			 dataSize;//�g�`�f�[�^�̃T�C�Y



public:
	//�R���X�g���N�^
	WaveFile();
	~WaveFile();

	//���[�h
	bool Load(TCHAR* FilePath);

	void Release();

};