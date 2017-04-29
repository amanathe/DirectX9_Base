#include "texture.h"

//�R���X�g���N�^
Texture::Texture()
{
	pTexture = nullptr;

	divU = 1;
	divV = 1;
	numU = 0;
	numV = 0;
}

//�f�X�g���N�^
Texture::~Texture()
{
	//�ǂݍ��܂�Ă�����j��
	if (pTexture != nullptr)
	{
		pTexture->Release();
	}
}

bool Texture::Load(TCHAR* FileName)
{
	Direct3D& d3d =Direct3D::GetInstance();

	return d3d.LoadTexture(*this, FileName);//�ǂݍ���
}

//�e�N�X�`�����c�����������ė��p���邩��ݒ�
void Texture::SetDivide(unsigned int DivU, unsigned int DivV)
{
	if (DivU <= 0 || DivV <= 0)
	{
		return;
	}

	divU = DivU;
	divV = DivV;
}

//���������e�N�X�`���̏c�����Ԗڂ��g�������w��
void Texture::SetUVNum(unsigned int NumU, unsigned int NumV)
{
	if (NumU >= divU)return;
	if (NumV >= divV)return;

	numU = NumU;
	numV = NumV;
}