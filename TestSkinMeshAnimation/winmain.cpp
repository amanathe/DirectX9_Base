


#if _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#pragma comment(lib,"winmm.lib")
#include <windows.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <tchar.h>
#include <d3dx9.h>
#include <d3dx9.h>

#if _DEBUG
#define new ::new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

//�Q�l�@�v���O����
//  HP    : http://marupeke296.com

//�Œ���̒��_���
//���W�Ɗe�{�[���̏d�݂ƃ{�[���s��ԍ�
//��3������΃X�L�����b�V���͂ł���
struct Vertex
{
	D3DXVECTOR3 coord;
	D3DXVECTOR3 weight;
	unsigned char matrixIndex[4];
};

//�{�[���\����
struct Bone
{
	int id;					//�{�[��id
	Bone* firstChild;		//���q�{�[��
	Bone* sibling;			//���̌Z��{�[��
	D3DXMATRIX offsetMat;	//�{�[���I�t�Z�b�g�s��
	D3DXMATRIX initMat;		//�����p���s��
	D3DXMATRIX boneMat;		//�{�[���p���s��
	D3DXMATRIX *combMatAry;	//�����p���s��z��ւ̃|�C���^

	Bone() :id(), firstChild(), sibling(), combMatAry()
	{
		D3DXMatrixIdentity(&initMat);
		D3DXMatrixIdentity(&offsetMat);
		D3DXMatrixIdentity(&boneMat);
	}

};


int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev);

//�E�B���h�E�v���V�[�W���\����
struct WP
{
	static LRESULT CALLBACK WndProc
	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{//���ʂȏ�����K�v�Ƃ��郁�b�Z�[�W�́@�Ƃ肠�����I���������������Ă���
		switch (msg)
		{
		case WM_DESTROY: //�E��̃o�c�������ꂽ�AAltF4�������ꂽ���ȂǂɎ�M���郁�b�Z�[�W
			PostQuitMessage(0);//���b�Z�[�W�L���[��WM_QUIT���b�Z�[�W�𑗂�
			break;
		}
		//���ʂȏ�����v���Ȃ����b�Z�[�W�̓f�t�H���g�̃E�B���h�E�v���V�[�W������������
		return DefWindowProc(hWnd, msg, wParam, lParam);//�ړ��Ƃ��T�C�Y�ύX�Ƃ�
	}
};

_TCHAR gName[100] = _T("���S�z���C�g�{�b�N�X�ȃX�L�����b�V���A�j���[�V�����e�X�g�v���O����");


// �Œ���̃V�F�[�_
//  �d�݌W���ƒ��_�𓮂������߂̃��[���h�ϊ��s��̔z���n���܂�
const char *vertexShaderStr =
"float4x4 view : register(c0);"
"float4x4 proj : register(c4);"
"float4x4 world[12] : register(c8);"
"  "
"struct VS_IN {"
"    float3 pos : POSITION;"
"    float3 blend : BLENDWEIGHT;"
"    int4 idx : BLENDINDICES;"
"};"
"struct VS_OUT {"
"    float4 pos : POSITION;"
"};"
"VS_OUT main( VS_IN In ) {"
"    VS_OUT Out = (VS_OUT)0;"
"    float w[3] = (float[3])In.blend;"
"    float4x4 comb = (float4x4)0;"
"    for ( int i = 0; i < 3; i++ )"
"        comb += world[In.idx[i]] * w[i];"
"    comb += world[In.idx[3]] * (1.0f - w[0] - w[1] - w[2]);"
"    "
"    Out.pos = mul( float4(In.pos, 1.0f), comb );"
"    Out.pos = mul( Out.pos, view );"
"    Out.pos = mul( Out.pos, proj );"
"    return Out;"
"}";

// �s�N�Z���V�F�[�_�͎����ēK��
// �D���Ȃ悤�ɓ_������ĉ������B
const char *pixelShaderStr =
"struct VS_OUT {"
"    float4 pos : POSITION;"
"};"
"float4 main( VS_OUT In ) : COLOR {"
"    return float4(1.0f, 1.0f, 1.0f, 1.0f);"
"}"
"";


//���C��
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�A�v�����V�����̏�����

	HWND hWnd;

	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));						// �ϐ��̃����o��S��0�ŏ�����
	wcex.cbSize			= sizeof(WNDCLASSEX);					// cbSize : �\���̂̃T�C�Y
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// style : �E�B���h�E�X�^�C��
	wcex.lpfnWndProc	= (WNDPROC)WP::WndProc;					// ipfnWndProc : �E�B���h�E�v���V�[�W���̃A�h���X�@�֐��|�C���^
	wcex.cbClsExtra		= 0;									// cbClsExtra : 0�Œ�
	wcex.cbWndExtra		= 0;									// cbWndExtra : 0�Œ�
	wcex.hInstance		= hInstance;							// hInstance : WinMain�̃C���X�^���X�n���h��
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;

	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	/*GetStockObject(BLACK_BRUSH);*/							// hbrBackground : �E�B���h�E�N���C�A���g�̈�̔w�i�F
	
	wcex.lpszMenuName	= NULL;									// lpszMeueName : ���j���[
	wcex.lpszClassName	= (_TCHAR*)gName;						// lpszClassName : �E�B���h�E�N���X�̖��O
	wcex.hIconSm		= NULL;


	if (!(RegisterClassEx(&wcex)))
	{
		return 0;
	}

	RECT rect;
	const int width = 800; 
	const int height = 600;
	SetRect(&rect, 0, 0, width, height);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	if(!(hWnd =CreateWindow(gName,gName,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,hInstance,NULL)))
	{
		return 0;
	}

	//Direct3D������
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3DDEV;

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == false) { return 0; }

	//�f�B�X�v���C���擾
	D3DDISPLAYMODE Display;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);


	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;					//�o�b�N�o�b�t�@�̕�
	D3DParam.BackBufferHeight = height;					//�o�b�N�o�b�t�@�̍���
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
	D3DParam.FullScreen_RefreshRateInHz = /*0*/D3DPRESENT_RATE_DEFAULT;			//�X�N���[���̃��t���b�V�����[�g�@�E�B���h�E���[�h���ƕK��0
	D3DParam.PresentationInterval =/* D3DPRESENT_INTERVAL_DEFAULT*/ D3DPRESENT_INTERVAL_ONE;	//�A�_�v�^���t���b�V�����[�g��present���������s���郌�[�g�̊֌W

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DParam, &g_pD3DDEV)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DParam, &g_pD3DDEV)))
		{
			g_pD3D->Release();
			return 0;
		}
	}
	//�E�B���h�E�\��
	ShowWindow(hWnd, SW_SHOW);


	SkinMeshAppMain(g_pD3DDEV);

	g_pD3DDEV->Release();
	g_pD3D->Release();



	return 0;
}


//�X�L�����b�V��
int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev)
{
	//�|���S���̒��_��`=======================================================================
	//���_����17��
	Vertex vtx[17] =
	{
		{ D3DXVECTOR3( 5, 0, 1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//0
		{ D3DXVECTOR3( 4, 0, 1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//1
		{ D3DXVECTOR3( 3, 0, 1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 2,1,0,0 } },//2
		{ D3DXVECTOR3( 2, 0, 1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 1,0,0,0 } },//3
		{ D3DXVECTOR3( 1, 0, 1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,0,0,0 } },//4
		{ D3DXVECTOR3( 1, 0, 0)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,0,0,0 } },//5
		{ D3DXVECTOR3( 2, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 1,0,0,0 } },//6
		{ D3DXVECTOR3( 3, 0, 0)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 2,1,0,0 } },//7
		{ D3DXVECTOR3( 4, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//8
		{ D3DXVECTOR3( 5, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//9
		{ D3DXVECTOR3( 5, 0,-1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//10
		{ D3DXVECTOR3( 4, 0,-1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//11
		{ D3DXVECTOR3( 3, 0,-1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 2,1,0,0 } },//12
		{ D3DXVECTOR3( 2, 0,-1)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 1,0,0,0 } },//13
		{ D3DXVECTOR3( 1, 0,-1)	,D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,0,0,0 } },//14
		{ D3DXVECTOR3(-1, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//15
		{ D3DXVECTOR3( 0, 0, 0)	,D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//16

	};

	//���_�C���f�b�N�X
	//�|���S����20���Ł@�O�p�`�Ȃ̂Ł@20*3 60��
	WORD idx[3*20] =
	{
		 0, 1, 8,
		 1, 2, 8,
		 2, 3, 6,
		 3, 4, 6,

		 4, 5, 6,
		 2, 6, 7,
		 2, 7, 8,
		 0, 8, 9,

		 9, 8,10,
		 8, 7,12,
		 7, 6,12,
		 6, 5,14,

		14,13, 6,
		 6,13,12,
		12,11, 8,
		 8,11,10,

		 4,16, 5,
		 5,16,14,
		 4,15,16,
		16,15,14
	};

	//�C���f�b�N�X�����ǂ��ĎO�p�`���X�g���쐬
	Vertex v[3*20];
	for (int i = 0; i < 60; i++)
	{
		v[i] = vtx[idx[i]];
	}

	//���_�錾�@FVF�쐬========================================================================
	//���_�\���̂̍\����� ���o�C�g�ڂ���ǂ�ȏ��H�@
	//Declaration :�@�錾
	D3DVERTEXELEMENT9 declAry[] =
	{
		{ 0,0,D3DDECLTYPE_FLOAT3 ,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0 },
		{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDWEIGHT,0 },
		{ 0,24,D3DDECLTYPE_UBYTE4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDINDICES,0 },
		D3DDECL_END()
	};
	IDirect3DVertexDeclaration9 *decl = 0;
	g_pD3DDev->CreateVertexDeclaration(declAry, &decl);


	//�{�[�����̍쐬========================================================================
	//�K�v�Ȃ̂́@�E�{�[���I�t�Z�b�g�s��@�E�{�[���s��

	Bone * pBones = new Bone[3];

	//�{�[���̐e�q�֌W�̍\�z
	//0 --- 1 - 2

	pBones[0].firstChild = &pBones[1];
	pBones[1].firstChild = &pBones[2];


	//�����p���̌v�Z==================================================================================
	//���[�J���p����ݒ肵
	//�ŏI�I�Ɏ����̐e����̑��Ύp���ɏC��
	D3DXMatrixRotationY(&pBones[0].initMat, D3DXToRadian(0.0f));
	D3DXMatrixRotationY(&pBones[1].initMat, D3DXToRadian(0.0f));
	D3DXMatrixRotationY(&pBones[2].initMat, D3DXToRadian(0.0f));
	
	//���ꂼ��̃{�[���� x y���W�����
	pBones[0].initMat._41 = 0.0000f; 	pBones[0].initMat._42 = 0.0000f; pBones[0].initMat._43 = 0.0000f;
	pBones[1].initMat._41 = 2.0000f;	pBones[1].initMat._42 = 0.0000f; pBones[1].initMat._42 = 0.0000f;
	pBones[2].initMat._41 = 4.0000f;	pBones[2].initMat._42 = 0.0000f; pBones[1].initMat._42 = 0.0000f;

	//�{�[���@�I�t�Z�b�g�s��̌v�Z
	//�I�t�Z�b�g�s��͊e�{�[���́u���[�J���p���v�̋t�s��
	D3DXMATRIX *pCombMat = new D3DXMATRIX[7];
	for (int i = 0; i < 3; i++)
	{
		pBones[i].id = i;
		pBones[i].combMatAry = pCombMat;

		//�e��ԃx�[�X�ɏC������O�ɃI�t�Z�b�g�s������߂Ă���
		//�{�[���ƒ��_�̍��W���������߂邽�߂ɕK�v

		//�{�[���I�t�Z�b�g�s�� * �����p���̒��_���W =���_�́@�{�[�����猩�����΍��W
		D3DXMatrixInverse(&pBones[i].offsetMat, 0, &pBones[i].initMat);
	}

	//�����p����e�̎p��~���Ύp���ɒ���
	//�悸�q�̖��[�܂ō~��Ď����̃��[�J����Ԃŏ����p�����e�̃{�[���I�t�Z�b�g�s��ő��Ύp�������߂�	

	//�����p����e��ԃx�[�X�ɕϊ�����֐��̒�`
	//�e�q�֌W�����ǂ邽�ߍċA�֐����K�v
	struct CalcRelativeMat
	{
		static void run(Bone* pMe, D3DXMATRIX *pParentOffsetMat = NULL)
		{
			if (pMe->firstChild)
			{
				run(pMe->firstChild, &pMe->offsetMat);
			}
			if (pMe->sibling)
			{
				run(pMe->sibling, pParentOffsetMat);
			}

			if (pParentOffsetMat)
			{
				pMe->initMat *= *pParentOffsetMat;
			}
		}
	};
	//�����p����e��ԃx�[�X�ɕϊ�����֐��̎��s
	CalcRelativeMat::run(&pBones[0]);


	/////////////////////////////////////////
	// �V�F�[�_�̃R���p�C���ƃV�F�[�_�쐬
	//////
	ID3DXBuffer *shader, *error;
	IDirect3DVertexShader9 *vertexShader;
	IDirect3DPixelShader9 *pixelShader;
	HRESULT res = D3DXCompileShader(vertexShaderStr, (UINT)strlen(vertexShaderStr), 0, 0, "main", "vs_3_0", D3DXSHADER_PACKMATRIX_ROWMAJOR, &shader, &error, 0);
	if (FAILED(res)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return 0;
	};
	g_pD3DDev->CreateVertexShader((const DWORD*)shader->GetBufferPointer(), &vertexShader);
	shader->Release();
	res = D3DXCompileShader(pixelShaderStr, (UINT)strlen(pixelShaderStr), 0, 0, "main", "ps_3_0", D3DXSHADER_PACKMATRIX_ROWMAJOR, &shader, &error, 0);
	if (FAILED(res)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return 0;
	};
	g_pD3DDev->CreatePixelShader((const DWORD*)shader->GetBufferPointer(), &pixelShader);
	shader->Release();


	//===========================================================
	// �e��s�񏉊���
	D3DXMATRIX view, proj;
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0.0f, -20.0f,-0.1f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(30), 64.0f / 48, 1.0f, 10000.0f);




	//========================================================

	g_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pD3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);


	//d3d.SetupRrojectionMatrix();

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 0.0f;
	light.Diffuse.b = 0.0f;
	light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);
	light.Range = 1000.0f;

	g_pD3DDev->SetLight(0, &light);

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	light.Direction = D3DXVECTOR3(0.5f, -1.0f, 0.5f);
	light.Range = 1000.0f;

	g_pD3DDev->SetLight(1, &light);

	g_pD3DDev->LightEnable(0, TRUE);
	g_pD3DDev->LightEnable(1, TRUE);

	g_pD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00444444);

	//�`�惋�[�v==========================================
	//���t���[���̎p������
	MSG msg;

	float val = 0.0f;
	float a[7] = { 0 };


	do
	{
		val += 0.03f;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		g_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(40, 40, 80), 1.0f, 0);
		g_pD3DDev->BeginScene();

		//�{�[���̎p�����X�V
		//�ŏI�I�ɂ�
		//[�{�[���I�t�Z�b�g�s��] * [���[���h��Ԃł̃{�[���p��]
		//���v�Z����

		//�e�{�[���̏����p������̍����p��(�e��ԃx�[�X)���X�V
		D3DXMATRIX defBone[3];
		D3DXMatrixIdentity(&defBone[0]);

		//�{�[���̉�]��K���ɓ�����
		for (int i = 1; i < 3; i++)
		{
			D3DXMATRIX transLation;
			D3DXMATRIX rotate;
			D3DXMATRIX scale;
			D3DXMatrixIdentity(&scale);
			D3DXMatrixIdentity(&transLation);
			D3DXMatrixIdentity(&rotate);
			D3DXMatrixIdentity(&defBone[i]);

			//
			//�s��̂����鏇�Ԃ͂���Ŋm����ۂ�
			//

			D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
			D3DXMatrixTranslation(&transLation,2.0f*(i-1), 0.0f, 0.0f );
			D3DXMatrixRotationY(&rotate, D3DXToRadian(sinf(val*0.1f))*70.0f);
			D3DXMatrixMultiply(&defBone[i], &scale, &rotate);
			D3DXMatrixMultiply(&defBone[i], &defBone[i], &transLation);

		}


		//�e�{�[���s��̐e��ԃx�[�X�ł̎p�����X�V
		//��{�p��* �����p��(�Ƃ��ɐe��ԃx�[�X)
		for (int i = 0; i < 3; i++)
		{
			pBones[i].boneMat = defBone[i] * pBones[i].initMat;
		}


		//�e��ԃx�[�X�ɃAq��e�{�[���s������[�J����ԃx�[�X�̎p���ɕϊ�
		//�����͐e�q�֌W�ɂ��������čs���������K�v������
		//�����鏇�Ԃ͎q*�e
		D3DXMATRIX global;
		D3DXMATRIX tmp;
		D3DXMatrixIdentity(&global);		
		D3DXMatrixIdentity(&tmp);
		D3DXMatrixRotationY(&global,D3DXToRadian(val*1.5f)*0.0f);
		D3DXMatrixTranslation(&tmp, -5.5f-(sinf(val*0.0f)), 0, 0);

		D3DXMatrixMultiply(&global, &global, &tmp);


		struct UpdateBone
		{
			static void run(Bone* pMe, D3DXMATRIX*pParentWorldMat)
			{
				pMe->boneMat *= *pParentWorldMat;
				pMe->combMatAry[pMe->id] = pMe->offsetMat * pMe->boneMat;

				if (pMe->firstChild)
				{
					run(pMe->firstChild, &pMe->boneMat);
				}
				if (pMe->sibling)
				{
					run(pMe->sibling, pParentWorldMat);
				}
			}
		};

		UpdateBone::run(pBones, &global);


		// �V�F�[�_�ݒ�
		// �ϐ����������ރ��W�X�^�ʒu�̓V�F�[�_�ɏ����Ă���܂���B
		g_pD3DDev->SetVertexShader(vertexShader);
		g_pD3DDev->SetPixelShader(pixelShader);
		g_pD3DDev->SetVertexShaderConstantF(0, (const float*)&view, 4);
		g_pD3DDev->SetVertexShaderConstantF(4, (const float*)&proj, 4);
		g_pD3DDev->SetVertexShaderConstantF(8, (const float*)pCombMat, 4 * 3);

		g_pD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		g_pD3DDev->SetVertexDeclaration(decl);
		g_pD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 20, &v, sizeof(Vertex));



		g_pD3DDev->EndScene();
		g_pD3DDev->Present(NULL, NULL, NULL, NULL);

	} while (msg.message != WM_QUIT);

	/*for (int i = 0; i < ; i++)
		boneObj[i]->Release();*/
	vertexShader->Release();
	pixelShader->Release();
	decl->Release();


	delete[]pCombMat;
	delete[]pBones;


	return 0;

}