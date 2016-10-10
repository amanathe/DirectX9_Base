#pragma once

#include "Direct3d.h"

class Texture
{
	friend class Direct3D;
private:
	IDirect3DTexture9* pTexture;

public:
	
	//�R���X�g���N�^ �f�X�g���N�^
	Texture();
	~Texture();

	//UV�̕�����
	unsigned int divU;
	unsigned int divV;
	//UV�̔ԍ�
	unsigned int numU;
	unsigned int numV;

	void SetDivide(unsigned int DivU, unsigned int DivV);
	void SetUVNum(unsigned int NumU, unsigned int NumV);

	bool Load(TCHAR* FileName);


};