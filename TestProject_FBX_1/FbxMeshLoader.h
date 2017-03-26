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

class FbxMeshLoader
{
private:

	//���[�h�ς݂��ǂ���
	bool loaded;

	//���_���W�̐�
	int vertexPointCount;

	//���_���W
	D3DXVECTOR4* pVertexPoints_DX;

	//�|���S����
	int polygonCount;

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


};