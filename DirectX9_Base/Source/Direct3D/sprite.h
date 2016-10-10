#pragma once

#include "Direct3D.h"

struct AnimationNum
{
	unsigned int numU;
	unsigned int numV;
};

//�|���S�����_���
struct SpriteVertex
{
	float x, y, z;	//3�������W
	float rhw;		//2D�ϊ��ς݃t���O
	float u, v;		//UV���W

};

class Sprite
{
	friend class Direct3D;
public:
	
	//FVF(�_��Ȓ��_�\���̐錾)
	static const DWORD SPRITE_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

private:
	D3DXVECTOR2 pos;

	//�X�v���C�g�T�C�Y
	int width;
	int height;
	
	//��]�l(���W�A��)
	float rotate;
public :

	//�R���X�g���N�^
	Sprite();
	//~�f�X�g���N�^
	~Sprite();

	void SetPos(float x, float y);
	void SetSize(int Width, int Height);
	void SetRotate(float Rotate);

	void Draw(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture, bool isTurn = false);

};