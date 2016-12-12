#include "direct3d.h"
#include "texture.h"
#include "sprite.h"
#include "mesh.h"

//�R���X�g���N�^
Direct3D::Direct3D()
{
	pD3D9 = NULL;
	pDevice3D = NULL;
	isDeviceCreated = false;
	DrawFunc = NULL;
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
	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = Width;					//�o�b�N�o�b�t�@�̕�
	D3DParam.BackBufferHeight = Height;					//�o�b�N�o�b�t�@�̍���
	D3DParam.BackBufferFormat = Display.Format;			//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
	D3DParam.BackBufferCount = 1;						//�o�b�N�o�b�t�@��
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;		//�}���`�T���v�����O�̐ݒ� �Ȃ�
	D3DParam.MultiSampleQuality = 0;					//�}���`�T���v�����O�̃N�I���e�B
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;		//�X���b�v�`�F�C���̕��@�@DirectX�܂���
	D3DParam.hDeviceWindow = hWnd;						//�Ώۂ̃E�B���h�E�̃n���h��
	D3DParam.Windowed = TRUE;							//�E�B���h�E
	D3DParam.EnableAutoDepthStencil = TRUE;				//�k�x�X�e���V���o�b�t�@�̍쐬
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;		//�k�x�X�e���V���̃t�H�[�}�b�g
	D3DParam.Flags = 0;									//
	D3DParam.FullScreen_RefreshRateInHz = 0;			//�X�N���[���̃��t���b�V�����[�g�@�E�B���h�E���[�h���ƕK��0
	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�A�_�v�^���t���b�V�����[�g��present���������s���郌�[�g�̊֌W


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
	return true;//�ǂꂩ�ō쐬���������true���Ԃ�

	/*HRESULT CreateDevice
		(UINT Adapter,			�f�B�X�v���C �A�_�v�^����������
		D3DDEVTYPE DeviceType,	�ړI�̃f�o�C�X �^�C�v
		HWND hFocusWindow,		�f�o�C�X�Ńt�H�[�J�X��ݒ肷��E�B���h�E�̃n���h��
		DWORD BehaviorFlags,	�f�o�C�X�쐬�̃I�v�V����
		D3DPRESENT_PARAMETERS *pPresentationParameters,
		IDirect3DDevice9** ppReturnedDeviceInterface
		);*/




}

//�`��̔��f���Ɏ��s
void Direct3D::Render()
{
	if (Singleton<Direct3D>::IsInstatnceCreated())
	{
		Direct3D & d3d = Direct3D::GetInstance();

		if (d3d.IsDeviceCreated())
		{
			if (SUCCEEDED(d3d.pDevice3D->BeginScene()))
			{
				DWORD ClearColor =/* (pathed) ? 0x11111111 :*/ 0xFFFF0000;//�w�i�N���A�F

				//�w�i�N���A
				d3d.pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, ClearColor, 1.0f, 0);

				//�����Ɋ֐��|�C���^�ŕ`�揈�����s���l�ɂ���
				d3d.TryCallDrawFunc();

				d3d.pDevice3D->EndScene();

				d3d.pDevice3D->Present(NULL, NULL, NULL, NULL);
			}
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

			case RENDER_MESH:
			{
				d3d.pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				d3d.pDevice3D->SetRenderState(D3DRS_LIGHTING, TRUE);
				d3d.pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);


				d3d.SetupRrojectionMatrix();

				

				D3DLIGHT9 light;
				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 1.0f;
				light.Diffuse.g = 0.0f;
				light.Diffuse.b = 0.0f;
				light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);
				light.Range = 1000.0f;

				d3d.pDevice3D->SetLight(0, &light);

				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 0.0f;
				light.Diffuse.g = 1.0f;
				light.Diffuse.b = 0.0f;
				light.Direction = D3DXVECTOR3(0.5f, -1.0f, 0.5f);
				light.Range = 1000.0f;

				d3d.pDevice3D->SetLight(1, &light);

				d3d.pDevice3D->LightEnable(0, TRUE);
				d3d.pDevice3D->LightEnable(1, TRUE);

				d3d.pDevice3D->SetRenderState(D3DRS_AMBIENT, 0x00444444);
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

bool Direct3D::LoadTexture(Texture& texture,TCHAR* FileName)
{
	//�摜�ǂݍ���
	//DirectX��WindowsAPI�̊֐���HRESULT�����ʂɕԂ��֐�������
	//FAILED�}�N���Ŏ��s�������̔��f
	//SUCEEDED�}�N���Ŋ֐��������������̔��f
	if (IsDeviceCreated())
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice3D, FileName, &texture.pTexture)))
		{
			//�ǂݍ��ݎ��s�i�t�@�C���������\���������j
			return false;
		}

		//�ǂݍ��ݐ���
		return true;
	}
	else
	{
		return false;
	}
}

//�X�v���C�g�̕\��
void Direct3D::DrawSprite(Sprite& sprite, Texture& texture, bool isTurn)
{
	Direct3D& d3d = Direct3D::GetInstance();

	//���_�̈ʒu
	SpriteVertex vtx[4]
	{
		//{x,y,z, 2D�ϊ��ς݃t���O , u,v}
		//uv���W�̓e�N�X�`���̂ǂ̊p��\�����邩 (0,0)����	(1,0)�E��	(0,1)����	(1,1)�E��	(0.5,0.5 )�e�N�X�`���̒��S
		//���̊p�Ƀe�N�X�`���̉��p�[�Z���g�̈ʒu�����邩���w��

		//
		//�E��
		{ (float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f,
			(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//�E��
		{ (float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f,
		(isTurn ? static_cast<float>(texture.numU) / texture.divU : static_cast<float>(texture.numU + 1) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},
		//����
		{ -(float)sprite.width / 2, -(float)sprite.height / 2, 0.0f, 1.0f,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV) / texture.divV
		},
		//����
		{ -(float)sprite.width / 2, (float)sprite.height / 2, 0.0f, 1.0f,
		(isTurn ? static_cast<float>(texture.numU + 1) / texture.divU : static_cast<float>(texture.numU) / texture.divU), static_cast<float>(texture.numV + 1) / texture.divV
		},

	};

	for (int i = 0; i < 4; i++)
	{
		//��]
		float x = vtx[i].x*cosf(sprite.rotate) - vtx[i].y*sinf(sprite.rotate);
		float y = vtx[i].x*sinf(sprite.rotate) + vtx[i].y*cosf(sprite.rotate);

		//���s�ړ�
		vtx[i].x = x + sprite.pos.x;
		vtx[i].y = y + sprite.pos.y;

	}

	//�e�N�X�`���̃Z�b�g
	d3d.pDevice3D->SetTexture(0, texture.pTexture);
	//���_�\���̐錾���Z�b�g
	d3d.pDevice3D->SetFVF(Sprite::SPRITE_FVF);
	//�X�v���C�g�`��
	//TRIANGLESTRIP�Œ��_��`�悷�� 2��primitiveCount�|���S����
	if (SUCCEEDED(d3d.pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(SpriteVertex))))
	{
		int a = 0;
	}
	else

	{
		int a = 0;
	}
}

void Direct3D::SetDrawFunc(FuncPointer pointer)
{
	DrawFunc = pointer;
}

void Direct3D::TryCallDrawFunc()
{
	if (DrawFunc != NULL)
	{	
		
		DrawFunc();
		
	
	}
	else
	{
		int a = 0;
	}
}

void Direct3D::SetViewMatrix(D3DXMATRIXA16& mat)
{
	pDevice3D->SetTransform(D3DTS_VIEW, &mat);
}

void Direct3D::LoadMesh(Mesh& mesh,TCHAR* path)
{
	////LPSTR ����LPCWSTR�ɕϊ�
	////https://msdn.microsoft.com/ja-jp/library/ms235631(VS.80).aspx
	//LPSTR temp = path;
	//size_t origsize = strlen(temp)+1;
	//size_t convertedChars = 0;
	//wchar_t wcstring[1024];
	//ZeroMemory(wcstring, sizeof(wcstring));
	//mbstowcs_s(&convertedChars, wcstring, origsize, temp, _TRUNCATE);
	//
	////	wcscat_s(wcstring, L"(wchar_t *)");
	//LPCWSTR path2 = wcstring;


	//X�t�@�C���̃p�X���擾
	CHAR dir[_MAX_DIR];
	_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

	LPD3DXBUFFER pBufferMaterial;

	if (D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM, pDevice3D, NULL, &pBufferMaterial, NULL, &mesh.numMaterials, &mesh.pMesh) != D3D_OK)
	{
		return;
	}

	//�}�e���A���̏���
	if (mesh.numMaterials > 0)
	{
		mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
		mesh.ppTextures = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

		D3DXMATERIAL * d3dxMaterials = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();

		for (int i = 0; i < mesh.numMaterials; i++)
		{
			//�v�X�̃}�e���A�����o�b�t�@����R�s�[����
			mesh.pMaterials[i] = d3dxMaterials[i].MatD3D;
			mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

			mesh.ppTextures[i] = NULL;

			//�e�N�X�`���̃t�@�C���������o���ă��[�h
			if (d3dxMaterials[i].pTextureFilename != NULL)
			{			

				//�e�N�X�`���t�@�C���p�X���쐬����
				CHAR texturefile[1024];
				ZeroMemory(texturefile, sizeof(texturefile));
				lstrcat(texturefile, dir);
				lstrcat(texturefile, d3dxMaterials[i].pTextureFilename);

				if (D3DXCreateTextureFromFile(pDevice3D,texturefile, &mesh.ppTextures[i])!=D3D_OK)
				{
					mesh.ppTextures[i] = NULL;
				}

			}

		}
	}

	pBufferMaterial->Release();

}

void Direct3D::DrawMesh(Mesh& mesh, D3DXMATRIXA16& worldMat)
{
	if (mesh.pMesh != NULL)
	{
		pDevice3D->SetTransform(D3DTS_WORLD, &worldMat);

		//���_�V�F�[�_
		pDevice3D->SetVertexShader(NULL);

		//���_�t�H�[�}�b�g
		pDevice3D->SetFVF(mesh.pMesh->GetFVF());

		if (mesh.numMaterials > 0)
		{
			for (unsigned int i = 0; i < mesh.numMaterials; i++)
			{

				pDevice3D->SetMaterial(&mesh.pMaterials[i]);
				pDevice3D->SetTexture(0, mesh.ppTextures[i]);
				mesh.pMesh->DrawSubset(i);
			}
		}
		else
		{
			//�}�e���A�������������ꍇ�́i����ȃP�[�X�܂��Ȃ����j
			//�K���ɍ�����}�e���A���ŕ\��
			D3DMATERIAL9 mtrl;
			ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
			pDevice3D->SetMaterial(&mtrl);

			mesh.pMesh->DrawSubset(0);
		}
	}
}

void Direct3D::SetupRrojectionMatrix()
{
	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, 3.0f / 4.0f, 1.0f, 1.0f, 100.0f);
	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);
}
