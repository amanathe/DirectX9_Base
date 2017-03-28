#include "FbxMeshLoader.h"

#include "fbxUtil.h"

FbxMeshLoader::FbxMeshLoader()
{
	pVertexPoints_DX = NULL;
	
	vertexPointCount =0;

	polygonCount = 0;
	/*polygonVertexNum = 0;
	pIndexArray = NULL;*/
	pPolygonVertexCount = NULL;
	ppPolygonVertexIndex = NULL;

	pNormalCounts = NULL;


	ppNormalVector =NULL;

	loaded = false;
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
		for (int i = 0; i < polygonCount; i++)
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


	if(ppNormalVector!=NULL)
	{ 
		for (int i = 0; i < normalLayerCount; i++)
		{
			if (ppNormalVector[i] != NULL)
			{
				delete[] ppNormalVector[i];
				ppNormalVector[i] = NULL;
			}
		}
		delete[] ppNormalVector;
		ppNormalVector = NULL;
	}

	if (pNormalCounts != NULL)
	{
		delete[]pNormalCounts;
		pNormalCounts = NULL;
	}


	loaded = false;
}

void FbxMeshLoader::Load(FbxNode* pNode)
{

	if (loaded == false)
	{
		if (IsMesh(pNode))
		{
			//�m�[�h���烁�b�V���f�[�^�擾
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != NULL)
			{
				loaded = true;

				std::cout << std::endl;
				std::cout << "mesh load "<<std::endl;

				

				//���b�V�����\������|���S����
				polygonCount = pMesh->GetPolygonCount();

				//���b�V���̑����_
				/*polygonVertexNum = pMesh->GetPolygonVertexCount();
				pIndexArray = pMesh->GetPolygonVertices();*/

				//�|���S�����\�����钸�_�̐�
				pPolygonVertexCount = new int[polygonCount];
				ppPolygonVertexIndex = new int*[polygonCount];

				for (int i = 0; i < polygonCount; i++)
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
				vertexPointCount = pMesh->GetControlPointsCount();

				FbxVector4* pControllPoints_FBX = pMesh->GetControlPoints();

				pVertexPoints_DX = new D3DXVECTOR4[vertexPointCount];

				//FbxVector4 ���� D3DXVECTOR4��
				for (int i = 0; i < vertexPointCount; i++)
				{
					pVertexPoints_DX[i].x = (float)pControllPoints_FBX[i][0];
					pVertexPoints_DX[i].y = (float)pControllPoints_FBX[i][1];
					pVertexPoints_DX[i].z = (float)pControllPoints_FBX[i][2];
					pVertexPoints_DX[i].w = (float)pControllPoints_FBX[i][3];
				}			
				
				//------�@��----

				//�@���Z�b�g�̐�
				int normalLayerCount = pMesh->GetElementNormalCount();

				pNormalCounts = new int[normalLayerCount];
				ppNormalVector = new D3DXVECTOR4*[normalLayerCount];				

				for (int layer = 0; layer < normalLayerCount; layer++)
				{

					FbxGeometryElementNormal* pNormal= pMesh->GetElementNormal(layer);

					//�}�b�s���O���[�h�̎擾
					FbxGeometryElement::EMappingMode mappingMode = pNormal->GetMappingMode();
					//���t�@�����X���[�h�̎擾
					FbxGeometryElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();

					//�}�b�s���O���[�h�̔���
					switch (mappingMode)
					{
					case FbxGeometryElement::eByControlPoint:

						//���t�@�����X���[�h�̔���
						switch(referenceMode)
						{ 
						case FbxGeometryElement::eDirect:					

							//eDirect�̏ꍇ�f�[�^�͏��ԂɊi�[����Ă��邽�߂��̂܂ܕێ�

							pNormalCounts[layer] = pNormal->GetDirectArray().GetCount();

							ppNormalVector[layer] = new D3DXVECTOR4[pNormalCounts[layer]];

							for (int i = 0; i < pNormalCounts[layer]; i++)
							{
								FbxVector4 vec = pNormal->GetDirectArray().GetAt(i);

								ppNormalVector[layer][i].x = static_cast<float>( vec[0]);
								ppNormalVector[layer][i].y = static_cast<float>(vec[1]);
								ppNormalVector[layer][i].z = static_cast<float>(vec[2]);
								ppNormalVector[layer][i].w = static_cast<float>(vec[3]);
							}

							break;

						case FbxGeometryElement::eIndexToDirect:
					
							break;
						}

						break;

					case FbxGeometryElement::eByPolygonVertex:
						//�@�����|���S���̊e���_���Ƃɐݒ肳��Ă���ꍇ						

						//���t�@�����X���[�h�̔���
						switch (referenceMode)
						{
						case FbxGeometryElement::eDirect:
					
							//eDirect�̏ꍇ�f�[�^�͏��ԂɊi�[����Ă��邽�߂��̂܂ܕێ�				

							pNormalCounts[layer] = pNormal->GetDirectArray().GetCount();

							ppNormalVector[layer] = new D3DXVECTOR4[pNormalCounts[layer]];

							for (int i = 0; i < pNormalCounts[layer]; i++)
							{
								FbxVector4 vec = pNormal->GetDirectArray().GetAt(i);

								ppNormalVector[layer][i].x = static_cast<float>(vec[0]);
								ppNormalVector[layer][i].y = static_cast<float>(vec[1]);
								ppNormalVector[layer][i].z = static_cast<float>(vec[2]);
								ppNormalVector[layer][i].w = static_cast<float>(vec[3]);
							}
													
							break;

						case FbxGeometryElement::eIndexToDirect:
					
							break;
						}
						
						break;
					}
				}



				//�ǂݍ��񂾏���\��
				std::cout << "polygonCount : " << polygonCount << std::endl;
				std::cout << "polygonVertexList" << std::endl;
				
				for (int i = 0; i < polygonCount; i++)
				{
					std::cout << "PolygonVertexCount [ " << i << " ] : " << pPolygonVertexCount[i] <<" - ";
					std::cout << "vertexIndex ";
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