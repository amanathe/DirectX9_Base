#define _CRT_SECURE_NO_WARNINGS
//fopen�ł̌x�����Ȃ�������

#include "WaveFile.h"
#include <string>

using namespace std;

//�R���X�g���N�^
WaveFile::WaveFile()
{
	//waveFormat��0�ŏ�����
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	waveData = NULL;
	dataSize = 0;

}

//�f�X�g���N�^
WaveFile::~WaveFile()
{
	Release();
}


//�J������
void WaveFile::Release()
{
	if (waveData != NULL)
	{
		delete[]waveData;

		waveData = NULL;
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));		
		dataSize = 0;
	}
}

//Wave�t�@�C���̓ǂݍ���
bool WaveFile::Load(TCHAR* FilePath)
{
	FILE* fp;

	//�t�@�C���I�[�v��
	if (!(fp = _tfopen(FilePath, _T("rb"))))
	{
		return false;
	}

	char chunkId[5] = {};
	char type[5] = {};
	unsigned int chunkSize = 0;

	
	//RFF�w�b�_�[�̍\��
	//riff "RIFF"��4�����@							4byte
	//size ���t�@�C���T�C�Y-8�ibyte�P�ʁj			4byte
	//type "WAVE"��4����(�����f�[�^�̏ꍇ"WAVE")	4byte 

	//RIFF�w�b�_�[�`�����N�ǂݍ���
	fread(chunkId, sizeof(char) * 4, 1, fp);
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	fread(type, sizeof(char) * 4, 1, fp);

	string sChunkId(chunkId);
	string sType(type);
	//riff�`�� �^�C�v��
	if (!(sChunkId == "RIFF" && sType == "WAVE"))
	{
		//Wave�t�@�C���ł͂Ȃ�
		return false;
	}

	//�q�`�����N�̓ǂ݂���
	bool fmtChunk = false;
	bool dataChunk = false;

	while (true)
	{
		fread(chunkId, sizeof(char) * 4, 1, fp);
		fread(&chunkSize, sizeof(unsigned int), 1, fp);

		sChunkId = chunkId;

		//�t�H�[�}�b�g�`�����N�̓ǂݍ���
		if (sChunkId == "fmt ")
		{
			//�t�H�[�}�b�g�`�����N�̍\��
			//id			4byte	�`�����N���ʎq
			//size			4byte	id��size�������`�����N�T�C�Y
			//format		2byte	�g�`�f�[�^�̃t�H�[�}�b�g
			//channels		2byte	�g�p�`�����l����
			//samplerate	4byte	�T���v�����O���g��
			//bytepersec	4byte	���σ��f�[�^����
			//blockalign	2byte	�f�[�^�̃u���b�N�T�C�Y
			//bitswidth		2byte	1�T���v��������̃r�b�g��
			//extended_size	2byte	�g���f�[�^�̃T�C�Y
			//extended		nbyte	�g���f�[�^

			
			if (chunkSize >= sizeof(WAVEFORMATEX))
			{
				//WAVEFORMATEX�̂����������ǂݍ���
				fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);		

				//�`�����N�̎c��̈�̂����������v�Z
				int diff = chunkSize - sizeof(WAVEFORMATEX);

				//���̃`�����N�̐擪�܂ŃV�[�N
				fseek(fp, diff, SEEK_CUR);
			
			}
			else
			{			
				//�`�����N�̏I���܂œǂݍ���
				fread(&waveFormat, chunkSize, 1, fp);
			}

			fmtChunk = true;//�t�H�[�}�b�g�`�����N�ǂݍ��ݍς݂�����
		}
		else if(sChunkId =="data")//�f�[�^�`�����N�̓ǂݍ���
		{
			//�f�[�^�`�����N
			//id				4byte	�`�����N���ʎq
			//size				4byte	id��size�������`�����N�T�C�Y(�g�`�f�[�^�̃T�C�Y)
			//waveformatData	nbyte	�g�`�f�[�^

			//�f�[�^�T�C�Y�����E�z
			dataSize = chunkSize;
			waveData = new byte[chunkSize];
		
			//�f�[�^�ǂݍ���
			unsigned long readCount = fread(waveData, sizeof(byte), chunkSize, fp);
			if (readCount!= chunkSize)
			{
				fclose(fp);

				Release();
				return false;
			}
			dataChunk = true;


		}
		else
		{
			fseek(fp, chunkSize, SEEK_CUR);
		}

		//�t�H�[�}�b�g�`�����N�ƃf�[�^�`�����N�̗������ǂݍ��ݍς݂Ȃ烋�[�v�𔲂���
		if (fmtChunk && dataChunk)
		{
			break;
		}

	}
	fclose(fp);

	return true;
}