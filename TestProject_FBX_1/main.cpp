#include <iostream>
#include <fbxsdk.h>

#include "fbxUtil.h"

using namespace fbxsdk;

void WaitKey(char* message)
{
	std::cout << message << std::endl;
	std::cout << "Return �L�[�������Ă�������" << std::endl;
	getchar();
}

int main(int argc, char* argv[])
{
	//------------------------------------------------------------------
	//FBX Manager�̍쐬
	std::cout << "Create FbxManager ... ";
	FbxManager* pManager = FbxManager::Create();

	if (pManager == NULL)
	{
		std::cout << "Failed" << std::endl;

		WaitKey("�v���O�����̏I��");
		return 0;
	}
	else
	{
		std::cout << "Succeded" << std::endl;
	}

	//------------------------------------------------------------------
	//FBX Iporter�̍쐬
	std::cout << "Create FbxImporter ... ";
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");
	
	if (pImporter == NULL)
	{
		std::cout << "Failed" << std::endl;
		pManager->Destroy();
		WaitKey("�v���O�����̏I��");
	}
	else
	{
		std::cout << "Succeded" << std::endl;
	}
	
	//------------------------------------------------------------------
	//�C���|�[�^�[�Ńt�@�C�����J��
	const char* filePath = "FBX_FILES/cube.fbx";

	std::cout << "Open File [ " << filePath << " ] ... ";
	if (pImporter->Initialize(filePath) == false)
	{
		std::cout << "Failed " << std::endl;	
		pImporter->Destroy();
		pManager->Destroy();

		WaitKey("�v���O�����̏I��");
		return 0;
	}
	else
	{
		std::cout << "Succeded " << std::endl;
	}

	//------------------------------------------------------------------
	//�V�[���̍쐬
	std::cout << "CreateScene ...";
	FbxScene* pScene = FbxScene::Create(pManager, "");

	if (pScene == NULL)
	{
		std::cout << "Failed " << std::endl;
		pImporter->Destroy();
		pManager->Destroy();
		
		WaitKey("�v���O�����̏I��");
		return 0;
	}
	else
	{
		std::cout << "Succeded " << std::endl;
	}
	
	
	//------------------------------------------------------------------
	//�C���|�[�^�[����V�[����ǂݍ���
	std::cout << "Import Scene ... " << std:: endl;

	if(!pImporter->Import(pScene))
	{
		std::cout << "Failed " << std::endl;
		pScene->Destroy();
		pImporter->Destroy();
		pManager->Destroy();

		WaitKey("�v���O�����̏I��");
		return 0;
	}
	else
	{
		std::cout << "Succeded " << std::endl;
	}


	//�V�[����ǂ񂾂�K�v�Ȃ��̂ŃC���|�[�^�[�͔j������
	pImporter->Destroy();
	pImporter = NULL;
	
	//------------------------------------------------------------------
	//�m�[�h�T��
	FbxNode* pRootNode = pScene->GetRootNode();

	if (pRootNode)
	{		
		ProbeNode(pRootNode);		
	}

	pRootNode->Destroy();

	//------------------------------------------------------------------
	std::cout << "�I�������ɓ���܂�" <<std::endl;

	
	pScene->Destroy();
	pManager->Destroy();

	WaitKey("�v���O�����̏I��");
	return 1;
		

}