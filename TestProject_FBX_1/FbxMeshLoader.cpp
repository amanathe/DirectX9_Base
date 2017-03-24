#include "FbxMeshLoader.h"

#include "fbxUtil.h"

FbxMeshLoader::FbxMeshLoader()
{
	pVertexPoints_DX = NULL;
	
	controlNum=0;

	polygonNum = 0;
	polygonVertexNum = 0;
	pIndexArray = NULL;
	pPolygonVertexCount = NULL;
	ppPolygonVertexIndex = NULL;
}

FbxMeshLoader::~FbxMeshLoader()
{
	Release();
}

void FbxMeshLoader::Release()
{
	if (pVertexPoints_DX != NULL)
	{
		delete[] pVertexPoints_DX;
		pVertexPoints_DX = NULL;
	}

	if (ppPolygonVertexIndex != NULL)
	{
		for (int i = 0; i < polygonNum; i++)
		{
			if (ppPolygonVertexIndex[i] != NULL)
			{
				delete[] ppPolygonVertexIndex[i];
			}
		}

		delete[] ppPolygonVertexIndex;
		ppPolygonVertexIndex = NULL;
	}

	if (pPolygonVertexCount != NULL)
	{
		delete[] pPolygonVertexCount;
		pPolygonVertexCount = NULL;;
	}
}

void FbxMeshLoader::Load(FbxNode* pNode)
{

	if (Loaded == false)
	{
		if (IsMesh(pNode))
		{
			//�m�[�h���烁�b�V���f�[�^�擾
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != NULL)
			{
				Loaded = true;

				std::cout << " mesh load "<<std::endl;

				//���b�V�����\������|���S����
				polygonNum = pMesh->GetPolygonCount();

				//���b�V���̑����_
				polygonVertexNum = pMesh->GetPolygonVertexCount();
				pIndexArray = pMesh->GetPolygonVertices();

				//�|���S�����\�����钸�_�̐�
				pPolygonVertexCount = new int[polygonNum];
				ppPolygonVertexIndex = new int*[polygonNum];

				for (int i = 0; i < polygonNum; i++)
				{
					pPolygonVertexCount[i] = 0;
					ppPolygonVertexIndex[i] = NULL;

					//�|���S��[i]���\�����钸�_��
					pPolygonVertexCount[i] = pMesh->GetPolygonSize(i);

					//�|���S��[i]���\�����钸�_�̃C���f�b�N�X���i�[����
					ppPolygonVertexIndex[i] = new int[pPolygonVertexCount[i]];

					for (int j = 0; j < pPolygonVertexCount[i]; j++)
					{
						ppPolygonVertexIndex[i][j] = pMesh->GetPolygonVertex(i, j);
					}
				}
				

				//���_���W�̐�
				controlNum = pMesh->GetControlPointsCount();

				FbxVector4* pControllPoints_FBX = pMesh->GetControlPoints();

				pVertexPoints_DX = new D3DXVECTOR4[controlNum];

				//FbxVector4 ���� D3DXVECTOR4��
				for (int i = 0; i < controlNum; i++)
				{
					pVertexPoints_DX[i].x = pControllPoints_FBX[i][0];
					pVertexPoints_DX[i].y = pControllPoints_FBX[i][1];
					pVertexPoints_DX[i].z = pControllPoints_FBX[i][2];
					pVertexPoints_DX[i].w = pControllPoints_FBX[i][3];
				}
				

				//�ǂݍ��񂾏���\��
				std::cout << "polygonNum : " << polygonNum << std::endl;
				std::cout << "polygonVertexList" << std::endl;
				
				for (int i = 0; i < polygonNum; i++)
				{
					std::cout << "PolygonVertexCount [ " << i << " ] : " << pPolygonVertexCount[i] << std::endl;
					std::cout << "vertexIndex";
					for (int j = 0; j < pPolygonVertexCount[i]; j++)
					{
						std::cout << ppPolygonVertexIndex[i][j]<<",";
					}
					std::cout << std::endl;
				}

				pMesh->Destroy();
			}
		}
	}
}