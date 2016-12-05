#include "mesh.h"


Mesh::Mesh()
{
	pMesh = NULL;
	
}

Mesh::~Mesh()
{
	pMesh->Release();
}

void Mesh::DrawMatrice(D3DXMATRIXA16& mat_transform, D3DXMATRIXA16& mat_scale, D3DXMATRIXA16& mat_rotate )
{
	D3DXMATRIXA16 matWorld;

	//���W�@�g��@��]�@��ێ�����s��
	D3DXMATRIXA16 mat_temp;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&matWorld);
	
	D3DXMatrixIdentity(&mat_temp);

	////���f���̔z�u
	//static float testx = 0.0f;
	////testx += 0.01f;

	////���f���̊g�嗦
	//static float testScaleY = 0;
	//testScaleY += 0.005f;
	

	D3DXMatrixMultiply(&mat_temp, &mat_scale, &mat_rotate);   //�ꎞ�s��Ɋg�嗦 ��]�������������̂���
	D3DXMatrixMultiply(&matWorld, &mat_temp, &mat_transform); //�ꎞ�s��ƍ��W������


	
	Direct3D::GetInstance().DrawMatrix(&pMesh, matWorld);

}



void Mesh::Load(TCHAR* path)
{
	Direct3D::GetInstance().LoadMesh(&pMesh, path);

}