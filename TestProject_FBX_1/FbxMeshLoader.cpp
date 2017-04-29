#include "FbxMeshLoader.h"

#include "fbxUtil.h"

#include "Grobal.h"


UvSet::UvSet()
{
	pUvBuffer = nullptr;



}


UvSet::~UvSet()
{
	if (pUvBuffer != nullptr)
	{
		delete[] pUvBuffer;
		pUvBuffer = nullptr;
	}
}



FbxMeshLoader::FbxMeshLoader()
{
	pControlPoints_DX = nullptr;
	
    controlPointCount =0;

	polygonCount = 0;
	/*polygonVertexNum = 0;
	pIndexArray = NULL;*/
	pPolygonVertexCount = nullptr;
	ppPolygonVertexIndex = nullptr;

	pNormalCounts = nullptr;


	ppNormalVector = nullptr;


	uvSetCount = 0;
	
	pUvSetArray = nullptr;

	loaded = false;
}

FbxMeshLoader::~FbxMeshLoader()
{
	Release();
}

void FbxMeshLoader::Release()
{
	if (pControlPoints_DX != nullptr)
	{
		delete[] pControlPoints_DX;
		pControlPoints_DX = nullptr;
	}


	//�|���S���Ɏg�����������̉��
	if (ppPolygonVertexIndex != nullptr)
	{
		for (int i = 0; i < polygonCount; i++)
		{
			if (ppPolygonVertexIndex[i] != nullptr)
			{
				delete[] ppPolygonVertexIndex[i];
			}
		}

		delete[] ppPolygonVertexIndex;
		ppPolygonVertexIndex = nullptr;
	}

	if (pPolygonVertexCount != nullptr)
	{
		delete[] pPolygonVertexCount;
		pPolygonVertexCount = nullptr;
	}



	//�@���Ɏg�����������̉��
	if(ppNormalVector!= nullptr)
	{ 
		for (int i = 0; i < normalLayerCount; i++)
		{
			if (ppNormalVector[i] != nullptr)
			{
				delete[] ppNormalVector[i];
				ppNormalVector[i] = nullptr;
			}
		}
		delete[] ppNormalVector;
		ppNormalVector = nullptr;
	}

	if (pNormalCounts != nullptr)
	{
		delete[]pNormalCounts;
		pNormalCounts = nullptr;
	}

	//���_�F�Ɏg�����������̉��
	if (ppVertexColor != nullptr)
	{
		for (int i = 0; i <  VertexColorSetMax; i++)
		{
			if (ppVertexColor[i] != nullptr)
			{
				delete[] ppVertexColor[i];
				ppVertexColor[i] = nullptr;
			}
		}
		delete[]ppVertexColor;
		ppVertexColor = nullptr;
	}

	if (pColorCount_ByVertexColorSet != nullptr)
	{
		delete[]pColorCount_ByVertexColorSet;
		pColorCount_ByVertexColorSet = nullptr;
	}

	//UV�Z�b�g�̉��
	if (pUvSetArray != nullptr)
	{
		delete[] pUvSetArray;
		pUvSetArray = nullptr;
	}

	loaded = false;
}


//���[�h
void FbxMeshLoader::Load(FbxNode* pNode)
{

	if (loaded == false)
	{
		if (IsMesh(pNode))
		{
			//�m�[�h���烁�b�V���f�[�^�擾
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != nullptr)
			{
				loaded = true;

				std::cout << std::endl;
				std::cout << "mesh load "<<std::endl;	
				

				
				//���_���W
				LoadVertexPosition(pMesh);

				//���_�F�̎擾
				LoadVertexColor(pMesh);

				//�@���̎擾
				LoadNormal(pMesh);

				//UV�̎擾
				LoadVertexUV_Buffer(pMesh);
				
				//UVSEt�ƃ}�e���A���̊֘A�t��
				Asociate_UVSetAndMaterial(pNode);		

				////�ǂݍ��񂾏���\��
				//std::cout << "polygonCount : " << polygonCount << std::endl;
				//std::cout << "polygonVertexList" << std::endl;
				//
				//for (int i = 0; i < polygonCount; i++)
				//{
				//	std::cout << "PolygonVertexCount [ " << i << " ] : " << pPolygonVertexCount[i] <<" - ";
				//	std::cout << "vertexIndex ";
				//	for (int j = 0; j < pPolygonVertexCount[i]; j++)
				//	{
				//		std::cout << ppPolygonVertexIndex[i][j]<<",";
				//	}
				//	std::cout << std::endl;
				//}

				pMesh->Destroy();
			}
		}
	}
}


void FbxMeshLoader::LoadVertexPosition(FbxMesh* pMesh)
{
	//���b�V�����\������|���S����
	polygonCount = pMesh->GetPolygonCount();

	//���b�V���̑����_		

	//�|���S�����\�����钸�_�̐�
	pPolygonVertexCount = new int[polygonCount];
	ppPolygonVertexIndex = new int*[polygonCount];

	bool flag = false;
	for (int i = 0; i < polygonCount; i++)
	{
		pPolygonVertexCount[i] = 0;
		ppPolygonVertexIndex[i] = nullptr;

		//�|���S��[i]���\�����钸�_��
		pPolygonVertexCount[i] = pMesh->GetPolygonSize(i);

		//�|���S��[i]���\�����钸�_�̃C���f�b�N�X���i�[����
		ppPolygonVertexIndex[i] = new int[pPolygonVertexCount[i]];

		for (int j = 0; j < pPolygonVertexCount[i]; j++)
		{
			if (pPolygonVertexCount[i] > 3)
			{
				flag = true;
			}
			ppPolygonVertexIndex[i][j] = pMesh->GetPolygonVertex(i, j);
		}
	}

	if (flag)
	{
		std::cout << "���b�V�����[�h��" << std::endl;
		WaitKey("���p�`�|���S�����܂܂�Ă��܂���");
	}

	//���_���W�̐�
	controlPointCount = pMesh->GetControlPointsCount();

	FbxVector4* pControllPoints_FBX = pMesh->GetControlPoints();

	pControlPoints_DX = new D3DXVECTOR4[controlPointCount];

	//FbxVector4 ���� D3DXVECTOR4��
	for (int i = 0; i < controlPointCount; i++)
	{
		pControlPoints_DX[i].x = static_cast<float>(pControllPoints_FBX[i][0]);
		pControlPoints_DX[i].y = static_cast<float>(pControllPoints_FBX[i][1]);
		pControlPoints_DX[i].z = static_cast<float>(pControllPoints_FBX[i][2]);
		pControlPoints_DX[i].w = static_cast<float>(pControllPoints_FBX[i][3]);
	}
}

//UV���W�擾�֐�
void FbxMeshLoader::LoadVertexUV_Buffer(FbxMesh* pMesh)
{
	//http://shikemokuthinking.blogspot.jp/

	//UVSet�̐����擾
	
	uvSetCount = pMesh->GetElementUVCount();
	pUvSetArray = new UvSet[uvSetCount];

	for (int i = 0; i<uvSetCount; i++)
	{
		//Uv�o�b�t�@���擾
		FbxGeometryElementUV*UV = pMesh->GetElementUV(i);


		//�}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//���t�@�����X���[�h�̎擾
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();


		//UV���̎擾
		
		FbxLayerElementArrayTemplate<int>* pUvIndex = &UV->GetIndexArray();
		int uvIndexCount = pUvIndex->GetCount();

		pUvSetArray[i].pUvBuffer = new Point2DF[uvIndexCount];
	

		switch (mapping)
		{
			case fbxsdk::FbxLayerElement::eByControlPoint:
				{
					int BreakPoint = 0;
				}

				break;
			case fbxsdk::FbxLayerElement::eByPolygonVertex:

				switch (reference)
				{
					case FbxGeometryElement::eDirect:
						{
							int BreakPoint = 0;
						}
						break;

					case FbxGeometryElement::eIndexToDirect:
						{
							for (int j = 0; j < uvIndexCount; j++)
							{
								Point2DF temp;

								int index = pUvIndex->GetAt(j);
								FbxVector2 v = UV->GetDirectArray().GetAt(index);

								temp.x =static_cast<float>( v[0] );
								temp.y =static_cast<float>( 1.0f- v[1]);//�u�����_�[�ō�����t�@�C���͏㉺�t

								pUvSetArray[i].pUvBuffer[j] = temp;
							}

							//UvSet�����擾
							pUvSetArray[i].uvSetName = UV->GetName();

							{
								int BreakPoint = 0;
							}

						}
				
						break;
				}

				break;

			case fbxsdk::FbxLayerElement::eByPolygon:
				{
					int BreakPoint = 0;
				}
				break;
			case fbxsdk::FbxLayerElement::eByEdge:

				{
					int BreakPoint = 0;
				}
				break;

			case fbxsdk::FbxLayerElement::eAllSame:

				{
					int BreakPoint = 0;
				}
				break;

			default:
				{
					int BreakPoint = 0;
				}
				break;
		}

		UV->Destroy();
	}
}


//�@���擾�֐�
void FbxMeshLoader::LoadNormal(FbxMesh* pMesh)
{
	//------�@��----

	//�@���Z�b�g�̐�
	normalLayerCount = pMesh->GetElementNormalCount();

	pNormalCounts = new int[normalLayerCount];
	ppNormalVector = new D3DXVECTOR4*[normalLayerCount];

	for (int layer = 0; layer < normalLayerCount; layer++)
	{

		FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(layer);

		//�}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mappingMode = pNormal->GetMappingMode();
		//���t�@�����X���[�h�̎擾
		FbxGeometryElement::EReferenceMode referenceMode = pNormal->GetReferenceMode();

		//�}�b�s���O���[�h�̔���
		switch (mappingMode)
		{
			case FbxGeometryElement::eByControlPoint:

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
		pNormal->Destroy();
	}
}

//UV�ƃ}�e���A���̃e�N�X�`�����֘A�t����֐�
void  FbxMeshLoader::Asociate_UVSetAndMaterial(FbxNode* pNode)
{
	//���Ӎ�肩���@2017_4_18

	//UVSet�ƃ}�e���A���̊֘A�t��
	//uv�Z�b�g�̃e�N�X�`�����ƃ}�e���A���̃e�N�X�`�����Ŋ֘A�t��

	//�}�e���A���̐����擾
	int materialCount = pNode->GetMaterialCount();

	for (int i = 0; i < materialCount; i++)
	{
		//�}�e���A���̎擾
		FbxSurfaceMaterial * pMaterial = pNode->GetMaterial(i);

		//�}�e���A������Deffuse���擾
		FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);


		//�v���p�e�B�ɃA�^�b�`����Ă���e�N�X�`���̎�ʂ𔻒f
		int LayerdTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

		//FbxLayerdTexture :�����̃e�N�X�`�����u�����h�g�p����Ă���ꍇ�ɗL���ɂȂ�
		//FbxFileTexture :�@�P��̃e�N�X�`���Ƃ��Ďg�p����Ă���ƗL���ɂȂ�
		//FbxProceduralTexture : �s������͍l�����Ȃ�

		if (LayerdTextureCount>0)
		{
			for (int j = 0; j < LayerdTextureCount; j++)
			{
				//�e�N�X�`�����擾
				FbxLayeredTexture *pLayerdTexture = prop.GetSrcObject<FbxLayeredTexture>(j);

				//���C���[�����擾
				int textureCount = pLayerdTexture->GetSrcObjectCount<FbxFileTexture>();

				for (int k = 0; k < textureCount; k++)
				{
					FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(k);


					if (pTexture)
					{
						//�e�N�X�`�������擾
						std::string textureName = pTexture->GetRelativeFileName();

						//UVSet�����擾
						std::string UVSetName = pTexture->UVSet.Get().Buffer();

						for (int k=0;k<uvSetCount;k++)
						{
							if (pUvSetArray[k].uvSetName == UVSetName)
							{
								pUvSetArray[k].texture = textureName;
							}
						}
					}
				}
			}
		}
		else
		{
			//--- �e�N�X�`�������擾 ---//
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 < fileTextureCount)
			{
				//--- �e�N�X�`���̐������J��Ԃ� ---//
				for (int j = 0; fileTextureCount > j; j++)
				{
					//--- �e�N�X�`�����擾 ---//
					FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>(j);
					if (pTexture)
					{
						//--- �e�N�X�`�������擾 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = pTexture->GetRelativeFileName();

						//--- UVSet�����擾 ---//
						std::string UVSetName = pTexture->UVSet.Get().Buffer();

						//--- UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ� ---//
						for (int k = 0; k<uvSetCount; k++)
						{
							if (pUvSetArray[k].uvSetName == UVSetName)
							{
								pUvSetArray[k].texture = textureName;
							}
						}
					}
				}
			}
		}
		pMaterial->Destroy();
	}
}


//���_�F�擾�֐�
void FbxMeshLoader::LoadVertexColor(FbxMesh* pMesh)
{
	//���_�J���[�Z�b�g�����擾
	VertexColorSetMax = pMesh->GetElementVertexColorCount();

	ppVertexColor = new ColorRGBA*[VertexColorSetMax];
	pColorCount_ByVertexColorSet = new int[VertexColorSetMax];

	for (int i = 0; i < VertexColorSetMax; i++)
	{
		ppVertexColor[i] = nullptr;
		pColorCount_ByVertexColorSet[i] = 0;
	}

	//�J���[�Z�b�g�̐������J��Ԃ�
	for (int i = 0; i <VertexColorSetMax; i++)
	{
		//���_�J���[�Z�b�g���擾
		FbxGeometryElementVertexColor * pColor = pMesh->GetElementVertexColor(i);

		FbxGeometryElement::EMappingMode mappingMode = pColor->GetMappingMode();
		FbxGeometryElement::EReferenceMode referenceMode = pColor->GetReferenceMode();

		switch (mappingMode)
		{

			case FbxGeometryElement::eByControlPoint:
				{
					int BreakPoint = 0;
				}
				break;

			case FbxGeometryElement::eByPolygon:
				{
					int BreakPoint = 0;
				}
				break;

			case FbxGeometryElement::eByPolygonVertex:

				switch (referenceMode)
				{

					case fbxsdk::FbxLayerElement::eIndexToDirect:
						{
							//����͂܂� mapping =eByPolygonVertex reference=eIndexToCirect
							//�̑g�ݍ��킹���珑�� 
							//�����p�̃t�@�C�������̕ۑ��`���Ȃ̂�

							FbxLayerElementArrayTemplate<int>* pIndex = &pColor->GetIndexArray();


							pColorCount_ByVertexColorSet[i] = pIndex->GetCount();


							ppVertexColor[i] = new ColorRGBA[pColorCount_ByVertexColorSet[i]];

							//���_�̐��������_�J���[���擾
							for (int j = 0; j < pColorCount_ByVertexColorSet[i]; j++)
							{

								int index = pIndex->GetAt(j);
								FbxColor color = pColor->GetDirectArray().GetAt(index);

								ppVertexColor[i][j].r = static_cast<float>(color[0]);
								ppVertexColor[i][j].g = static_cast<float>(color[1]);
								ppVertexColor[i][j].b = static_cast<float>(color[2]);
								ppVertexColor[i][j].a = static_cast<float>(color[3]);
							}
							{
								int BreakPoint = 0;
							}

						}
						break;

					case fbxsdk::FbxLayerElement::eDirect:
						{
							int BreakPoint = 0;
						}
						break;

					case fbxsdk::FbxLayerElement::eIndex:
						{
							int BreakPoint = 0;
						}
						break;

					default:
						{
							int BreakPoint = 0;
						}
						break;
				}

				break;

			default:

				{
					int BreakPoint = 0;
				}

				break;
		}
	}
	

}


