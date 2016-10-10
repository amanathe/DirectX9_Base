#include "direct3d.h"

//�R���X�g���N�^
Direct3D::Direct3D()
{
	pD3D9 = NULL;
	pDevice3D = NULL;
	isDeviceCreated = false;
}

void Direct3D::ReleaseDevice()
{
	if (isDeviceCreated)
	{
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;

		isDeviceCreated = false;
	}
}

//�f�X�g���N�^
Direct3D::~Direct3D()
{
	ReleaseDevice();	
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

//�u�����h���[�h�̕ύX
void Direct3D::SetRenderState(RENDERSTATE RenderState)
{
	if (Singleton<Direct3D>::IsInstatnceCreated())
	{
		Direct3D & d3d = Direct3D::GetInstance();

		if (d3d.IsDeviceCreated())
		{
			switch (RenderState)
			{
			case RENDER_DEFAULT:
			{
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);	//���u�����h�̖�����
			}
			break;

			case RENDER_ALPHATEST:
			{
				//���e�X�g�ɂ�铧���̈�̐؂蔲��
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//���e�X�g�̗L����
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);					// �A���t�@�Q�ƒl
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//���e�X�g���i�

				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			//���u�����h�̖�����
			}
			break;

			case RENDER_HALFADD:
			{
				//�����Z
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//���e�X�g�̖�����
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//���u�����f�B���O�̗L����
				d3d.pDevice3D->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Z�o�b�t�@���s��Ȃ�

				//pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);			//z�e�X�g���s��Ȃ�
				//pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		//���C�e�B���O���s��Ȃ�

				d3d.pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		//�u�����f�B���O�I�v�V�������Z
				d3d.pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//SRC�̐ݒ�
				d3d.pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		//DEST�̐ݒ�

			}
			break;

			case RENDER_ADD:
			{
				//�S���Z����
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//���u�����f�B���O�̗L����
				d3d.pDevice3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//�u�����f�B���O�I�v�V�������Z
				d3d.pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);	//SRC�̐ݒ�
				d3d.pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	//DEST�̐ݒ�
				d3d.pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//���e�X�g�̖�����

			}
			break;

			}
		}
		else
		{
			MessageBox(NULL, "�f�o�C�X���쐬����Ă��Ȃ��̂Ŏ��s�ł��܂���", TEXT("Direct3D SetRenderState"), MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, "�C���X�^���X���쐬����Ă��Ȃ��̂Ŏ��s�ł��܂���", TEXT("Direct3D SetRenderState"), MB_OK);
	}

	
}