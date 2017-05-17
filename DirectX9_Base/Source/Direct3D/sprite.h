﻿#pragma once

#include "Direct3D.h"

struct AnimationNum
{
	unsigned int numU;
	unsigned int numV;
};

//板ポリゴン頂点情報　
//MSDN頂点フォーマットを参照 FVFの設定によって型、数、順番が決まる
struct SpriteVertex
{
	float x, y, z;	//3次元座標
	float rhw;		//2D変換済みフラグ スクリーン座標

	DWORD colorDefuse;

	float u, v;		//UV座標

};

class Sprite
{
	friend class Direct3D;
public:
	
	//FVF(柔軟な頂点構造体宣言)
	static const DWORD SPRITE_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

private:
	D3DXVECTOR2 pos;

	//スプライトサイズ
	int width;
	int height;
	
	//回転値(ラジアン)
	float rotate;

	float alpha;
public :

	//コンストラクタ
	Sprite();
	//~デストラクタ
	~Sprite();

	void SetPos(float x, float y);
	void SetSize(int Width, int Height);
	void SetRotate(float Rotate);

	float GetAlpha(){ return alpha; }
	void SetAlpha(float a);

	//void Draw(IDirect3DDevice9* pDevice3D, IDirect3DTexture9* pTexture, bool isTurn = false);

};