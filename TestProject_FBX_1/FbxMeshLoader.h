#pragma once

#include <fbxsdk.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//#include <memory>

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#include <iostream>

#include <list>

struct Point2DF
{
	float x, y;

	bool operator==(Point2DF& val)
	{
		return(this->x == val.x &&this->y == val.y);
	}
};

class UvSet
{


public :
	std::string uvSetName;
	//std::list<std::string> textures;
	std::string texture;
	Point2DF* pUvBuffer;

	UvSet();
	~UvSet();

};





struct ColorRGBA
{
	float r;
	float g;
	float b;
	float a;

};



class FbxMeshLoader
{
private:

	//���[�h�ς݂��ǂ���
	bool loaded;

	//�R���g���[���|�C���g���W�̐�
	int controlPointCount;

	//�R���g���[���|�C���g���W
	D3DXVECTOR4* pControlPoints_DX;

	//�|���S����
	int polygonCount;

	int indexCount;
	D3DXVECTOR4* pVertexPoints_DX;


	////�����_��
	//int polygonVertexNum;
	//int * pIndexArray;

	//���ꂼ��̃|���S�����\�����钸�_�̐�
	// x�Ԗڂ̃|���S�����\�����钸�_�� = pPolygonVertexCount[x];
	int *pPolygonVertexCount;

	//���ꂼ��̃|���S�����\�����钸�_�̃C���f�b�N�X
	//���_�C���f�b�N�X = ppPolygonVertexIndex [p][x];
	//	p :�|���S���ԍ�
	//	x = 0�@�` (pPolygonVertexCount[p]-1)
	int **ppPolygonVertexIndex;

	//���_�F
	int VertexColorSetMax;
	int *pColorCount_ByVertexColorSet;
	ColorRGBA** ppVertexColor;


	//--------------
	//�@��

	//�@�����C���[��
	int normalLayerCount;

	//�@�����C���[�����Ƃ̖@����
	//�@���� = pNormalCounts[���C���[�ԍ�]
	int *pNormalCounts;

	//�@���f�[�^
	//�@�� = ppNormalVector[���C���[�ԍ�][�@���ԍ�]
	D3DXVECTOR4 **ppNormalVector;


	//�}�e���A����
	int materialCount;

	//uv�}�b�v
	int uvSetCount;	
	UvSet* pUvSetArray;


public :
	//�f�t�H���g�R���X�g���N�^
	FbxMeshLoader();
	//�f�X�g���N�^
	~FbxMeshLoader();

	//���[�h
	//���[�h���������b�V�������m�[�h
	void Load(FbxNode* pNode);

	//�������
	void Release();

private:
	void LoadVertexPosition(FbxMesh * pMesh);

	void LoadVertexUV_Buffer(FbxMesh * pMesh);

	void LoadNormal(FbxMesh * pMesh);

	void LoadVertexColor(FbxMesh * pMesh);

	void Asociate_UVSetAndMaterial(FbxNode* pNode);

};