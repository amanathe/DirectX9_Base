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
					pVertexPoints_DX[i].x = static_cast<float>(pControllPoints_FBX[i][0]);
					pVertexPoints_DX[i].y = static_cast<float>(pControllPoints_FBX[i][1]);
					pVertexPoints_DX[i].z = static_cast<float>(pControllPoints_FBX[i][2]);
					pVertexPoints_DX[i].w = static_cast<float>(pControllPoints_FBX[i][3]);
				}			
			



				//�@���̎擾
				GetNormal(pMesh);

				//UV�̎擾
				GetVertexUV_Buffer(pMesh);

				
				//UVSEt�ƃ}�e���A���̊֘A�t��
				Asociate_UVSetAndMaterial(pNode);
				



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






void FbxMeshLoader::GetVertexUV_Buffer(FbxMesh* pMesh)
{
	//http://shikemokuthinking.blogspot.jp/

	//UVSet�̐����擾
	int UvLayerCount = pMesh->GetElementUVCount();
	
	UvSet uvSet;
	for (int i = 0; i<UvLayerCount; i++)
	{
		//Uv�o�b�t�@���擾
		FbxGeometryElementUV*UV = pMesh->GetElementUV(i);


		//�}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();

		//���t�@�����X���[�h�̎擾
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();


		//UV���̎擾
		int uvCount = UV->GetDirectArray().GetCount();

		switch (mapping)
		{
		
		case fbxsdk::FbxLayerElement::eByControlPoint:

			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex:
		{
			switch (reference)
			{
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* pUvIndex = &UV->GetIndexArray();
				int uvIndexCount = pUvIndex->GetCount();

				Point2DF temp;
				for (int i = 0; i < uvIndexCount; i++)
				{
					temp.x = (float)UV->GetDirectArray().GetAt(pUvIndex->GetAt(i))[0];
					temp.y = 1.0f - (float)UV->GetDirectArray().GetAt(pUvIndex->GetAt(i))[1];//�u�����_�[�ō�����t�@�C���͏㉺�t

					uvSet.uvBuffer.push_back(temp);
				}

				//UvSet�����擾
				uvSet.uvSetName = UV->GetName();

				list_uvSet.push_back(uvSet);
			}
			break;

			}

		}
			break;
		
		case fbxsdk::FbxLayerElement::eByPolygon:
			break;
		case fbxsdk::FbxLayerElement::eByEdge:
			break;
		case fbxsdk::FbxLayerElement::eAllSame:
			break;
		default:
			break;
		}
		UV->Destroy();

	}
}

void FbxMeshLoader::GetNormal(FbxMesh* pMesh)
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

						for (UVSetList::iterator it = list_uvSet.begin(); it != list_uvSet.end(); it++)
						{
							if ((*it).uvSetName == UVSetName)
							{
								(*it).texture = textureName;
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
						for (UVSetList::iterator it = list_uvSet.begin(); it != list_uvSet.end(); it++)
						{
							if ((*it).uvSetName == UVSetName)
							{
								(*it).texture = textureName;
							}
						}
					}
				}
			}
		}
		pMaterial->Destroy();
	}
}


