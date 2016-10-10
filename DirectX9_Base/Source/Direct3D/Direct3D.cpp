#include "direct3d.h"

//�R���X�g���N�^
Direct3D::Direct3D()
{
	pD3D9 = NULL;
	pDevice3D = NULL;
	isDeviceCreated = false;
}

//�f�X�g���N�^
Direct3D::~Direct3D()
{
	pDevice3D->Release();
	pD3D9->Release();
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (isDeviceCreated == false)//����Ă��Ȃ��ꍇ�̂�
	{
		isDeviceCreated = Create(hWnd);
	}

	return isDeviceCreated;
}

bool Direct3D::Create(HWND hWnd)
{
	RECT rec;
	GetClientRect(hWnd,&rec);
	int Width=rec.right-rec.left;
	int Height=rec.bottom-rec.top;

	//Direct3D9�I�u�W�F�N�g�̍쐬
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	//�f�B�X�v���C���擾
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//�X���b�v�`�F�C���ݒ�
	//�X���b�v�`�F�C���@�o�b�N�o�b�t�@���t�����g�o�b�t�@�ɏ���������
	//DISCARD�̓X���b�v�`�F�C���̕��@��DIrectX�ɔC����Ƃ����ݒ�
	D3DPRESENT_PARAMETERS D3DParam =
	{
		Width, Height, Display.Format, 1, D3DMULTISAMPLE_NONE, 0,
		D3DSWAPEFFECT_DISCARD, hWnd, TRUE, TRUE, D3DFMT_D24S8, 0, 0, D3DPRESENT_INTERVAL_DEFAULT
	};

	//����̐ݒ�Ńf�o�C�X�쐬�����݂�
	//HAL���[�h��3D�f�o�C�X�쐬
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
	{
		if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
		{
			//Ref���[�h��3D�f�o�C�X�쐬
			if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
			{
				if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				{
					// 3D�f�o�C�X�쐬���s
					pD3D9->Release();
					return false;
				}
			}
		}
	}
	return true;
}

//�`��̔��f���Ɏ��s
void Direct3D::Render()
{
	if (Singleton<Direct3D>::IsInstatnceCreated())
	{
		Direct3D & d3d = Direct3D::GetInstance();

		if (d3d.IsDeviceCreated())
		{
			DWORD ClearColor =/* (pathed) ? 0x11111111 :*/ 0xFFFF0000;//�w�i�N���A�F

			//�w�i�N���A
			d3d.pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, ClearColor, 1.0f, 0);

			//�����Ɋ֐��|�C���^�ŕ`�揈�����s���l�ɂ���



			d3d.pDevice3D->EndScene();
		
			d3d.pDevice3D->Present(NULL, NULL, NULL, NULL);
		}
		else
		{
			MessageBox(NULL, "�f�o�C�X���쐬����Ă��Ȃ��̂Ŏ��s�ł��܂���", TEXT("Direct3D Render"), MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, "�C���X�^���X���쐬����Ă��Ȃ��̂Ŏ��s�ł��܂���", TEXT("Direct3D Render"), MB_OK);
	}
}
