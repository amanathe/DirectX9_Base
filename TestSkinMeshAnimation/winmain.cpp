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

	g_pD3DDEV->Release();
	g_pD3D->Release();



	return 0;
}


//�X�L�����b�V��
int SkinMeshAppMain(LPDIRECT3DDEVICE9 g_pD3DDev)
{
	//�|���S���̒��_��`
	//���_����15��
	Vertex vtx[15]=
	{
		{ D3DXVECTOR3(-0.5000f,-2.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } },//0
		{ D3DXVECTOR3(-0.5000f,-1.2887f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,2,0,0 } },//1
		{ D3DXVECTOR3(-0.5000f,-0.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//2
		{ D3DXVECTOR3(-1.3660f, 0.2113f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 3,4,0,0 } },//3
		{ D3DXVECTOR3(-2.2321f, 0.7113f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 4,0,0,0 } },//4
		{ D3DXVECTOR3(-1.7321f, 1.5774f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 4,0,0,0 } },//5
		{ D3DXVECTOR3(-0.8660f, 1.0774f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 3,4,0,0 } },//6
		{ D3DXVECTOR3( 0.0000f, 0.5774f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//7
		{ D3DXVECTOR3( 0.8660f, 1.0774f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 5,6,0,0 } },//8
		{ D3DXVECTOR3( 1.7321f, 1.5774f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 6,0,0,0 } },//9
		{ D3DXVECTOR3( 2.2321f, 0.7113f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 6,0,0,0 } },//10
		{ D3DXVECTOR3( 1.3660f, 0.2113f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 5,6,0,0 } },//11
		{ D3DXVECTOR3( 0.5000f,-2.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 0,0,0,0 } },//12
		{ D3DXVECTOR3( 0.5000f,-1.2887f,0.0f),D3DXVECTOR3(0.50f,0.50f,0.00f),{ 1,2,0,0 } },//13
		{ D3DXVECTOR3( 0.5000f,-2.2887f,0.0f),D3DXVECTOR3(1.00f,0.00f,0.00f),{ 2,0,0,0 } } //14
	};

	//���_�C���f�b�N�X
	//�|���S����13���Ł@�O�p�`�Ȃ̂Ł@13*3 39��
	WORD idx[39] =
	{
		0,1,14,
		1,13,14,
		1,2,13,
		2,12,13,
		2,7,12,
		2,6,7,
		2,3,6,
		3,5,6,
		3,4,5,
		7,8,12,
		8,11,12,
		8,9,11,
		9,10,11,
	};

	//�C���f�b�N�X�����ǂ��ĎO�p�`���X�g���쐬
	Vertex v[39];
	for (int i = 0; i < 39; i++)
	{
		v[i] = vtx[idx[i]];
	}

	//���_�錾�@FVF�쐬 
	//���_�\���̂̍\�����@
	D3DVERTEXELEMENT9 declAry[]=
	{
		{0,0,D3DDECLTYPE_FLOAT3 ,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDWEIGHT,0},
		{0,24,D3DDECLTYPE_UBYTE4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDINDICES,0},
		D3DDECL_END()	
	};
	IDirect3DVertexDeclaration9 *decl = 0;
	g_pD3DDev->CreateVertexDeclaration(declAry, &decl);


	return 0;

}