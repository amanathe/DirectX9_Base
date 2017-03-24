#include "fbxUtil.h"
#include "fbxMeshLoader.h"
#include <iomanip>

void ProbeNode(FbxNode* pNode,int depth)
{
	//�m�[�h�T���֐�
	if (pNode)
	{
		int childCount = pNode->GetChildCount();
		for (int i = 0; i < depth; i++)
		{
			std::cout << " " ;
		}
		
		std::cout << "|- ("<<std::setw(4)<<depth<<") " ;		

		std::cout << pNode->GetName() << " : " << childCount << " children. ";

		if (IsMesh(pNode))
		{
			std::cout << " [ Mesh ] ";
		}			

		if (IsMesh(pNode))
		{	
			FbxMeshLoader fbxMeshLoader;
			fbxMeshLoader.Load(pNode);
		}

		std::cout << std::endl;

		if (childCount > 0)
		{			
			//std::cout <<">>>>  In childNode" << std::endl;
		
			for (int i = 0; i < childCount; i++)
			{
				ProbeNode(pNode->GetChild(i), depth + 1);
			}
			//std::cout << "<<<<  back ParentNode" << std::endl;
		}
	}
}



bool IsMesh(FbxNode* pNode)
{
	//�m�[�h�����b�V���Ɍq�����Ă��邩

	//�m�[�h�ɂ͕����̃A�g���r���[�g���q�����Ă��邽�߁A���̌����`�F�b�N����

	if (pNode)
	{
		int attrCount = pNode->GetNodeAttributeCount();

		for (int i = 0; i < attrCount; i++)
		{

			FbxNodeAttribute::EType attrType = pNode->GetNodeAttributeByIndex(i)->GetAttributeType();

			if (attrType == FbxNodeAttribute::EType::eMesh)
			{
				return true;
			}			
		}
	}	

	return false;
}