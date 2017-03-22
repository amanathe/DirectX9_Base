#include "fbxUtil.h"

void ProbeNode(FbxNode* pNode)
{
	//�m�[�h�T���֐�
	if (pNode)
	{
		int childCount = pNode->GetChildCount();
		std::cout << pNode->GetName() << " : " << childCount << " children. ";

		if (IsMesh(pNode))
		{
			std::cout << " [ Mesh ] ";
		}
		
		std::cout << std::endl;

		if (IsMesh(pNode))
		{	
			FbxMesh *pMesh = pNode->GetMesh();

			if (pMesh != NULL)
			{
				std::cout << "mesh load" << std::endl;
			}
		}


		if (childCount > 0)
		{			
			std::cout <<">>>>  childNode" << std::endl;
		
			for (int i = 0; i < childCount; i++)
			{
				ProbeNode(pNode->GetChild(i));
			}
			std::cout << "<<<<  backParent" << std::endl;
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