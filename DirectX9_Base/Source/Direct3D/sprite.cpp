#include "sprite.h"


//�R���X�g���N�^
Sprite::Sprite()
{
	pos.x = pos.y = 0.0f;
	width = height = 0;

	rotate = 0.0f;
}

//�f�X�g���N�^

Sprite::~Sprite()
{

}

//------------------------

void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}


//------------------------

void Sprite::SetSize(int Width, int Height)
{
	width = Width;
	height = Height;
}


//------------------------



//��]��ݒ� ���W�A��
void Sprite::SetRotate(float Rotate)
{
	rotate = Rotate;
}