﻿#pragma once


#include <windows.h>
#pragma comment(lib, "winmm.lib")
#include <comdef.h>
#include <mmsystem.h>

//directX
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

//list
#include <list>

//基本入出力
//#include <iostream>

using namespace std;



//fbx
#include <fbxsdk.h>
using namespace fbxsdk;

#include "../Direct3D/texture.h"


struct ColorRGBA;

class Direct3D;
class UvSet;

class TriangleList
{
	friend class Direct3D;

public:
	//インナークラス(構造体)
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DCOLOR color;

		float u, v;
	};

	//静的定数

	//頂点構造体宣言
	static const DWORD FVF_TRIANGLE_LIST_VERTEX = D3DFVF_XYZ |D3DFVF_NORMAL| D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const Vertex DEFAULT_VERTEX;

	struct LoadParamator
	{
		//頂点
		int indexCount;
		D3DXVECTOR4* pVertexPoints_DX;
		//法線
		int normalLayerCount;
		int* pNormalCount;
		D3DXVECTOR4 ** ppNormalVector;

		//頂点色
		int VertexColorSetNum;
		int* pColorCount_ByVerTexColorSet;
		ColorRGBA** ppVertexColor;

		//UVSEt
		int uvSetCount;
		UvSet*pUvSetArray;
	};

public:
	//コンストラクタ
	TriangleList();

	//デストラクタ
	~TriangleList();

private:
	//メンバ変数 private


	Vertex* pVertices;

	int triangleCount;
	int vertexCount;

	Texture* pTexture;

public:
	//メンバ変数 public 



private:
	//メンバ関数 private



public:
	//メンバ関数 public 
	//アクセサ　ミューテータなど
	TriangleList* GetThisPointer() { return this; }
	
	
	//通常のメンバ関数
	void Release();

	//vertcies用のメモリ領域を確保
	bool TryAllocMem_Verticies(int vertexCount);

	bool LoadVerticies(LoadParamator param);


	//テスト用のデバッグ関数
	void Debug_TestShow();
};

