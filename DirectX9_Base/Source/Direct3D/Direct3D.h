#pragma once 

#define D3D_DEBUG_INFO	//Direct3D�f�o�b�O�t���O

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//#include <memory>

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#include "../Template/Singleton.h"

enum RENDERSTATE
{
	RENDER_DEFAULT,		//�f�t�H���g
	RENDER_ALPHATEST,	//���e�X�g
	RENDER_HALFADD,		//�����Z����
	RENDER_ADD,		//���Z����

	RENDER_MESH
};

class Texture;
class Sprite;

//-------------------------------
class Direct3D : public Singleton<Direct3D>
{
	friend class Singleton<Direct3D>;

public :
	typedef void(*FuncPointer)();//�֐��|�C���^�̌^��`

public:	

	//�f�o�C�X�쐬�����݂�
	bool TryCreate(HWND hWmd);

	static void Render();
	static void SetRenderState(RENDERSTATE RenderrState);

	bool LoadTexture(Texture& texture, TCHAR* FileName);

	//�X�v���C�g�̕\��
	static void DrawSprite(Sprite& sprite, Texture& texture , bool isTurn = false);
	static void DrawSprite(Sprite& sprite, bool isTurn = false);
	void SetDrawFunc(FuncPointer pointer);

private:
	IDirect3D9*	pD3D9 = NULL;				//Direct3D�f�o�C�X�����p�I�u�W�F�N�g
	IDirect3DDevice9* pDevice3D = NULL;		//Direct3D�̃f�o�C�X�@�X�v���C�g�̕\����e�N�X�`���̃��[�h���ɕK�v

	//�f�o�C�X�쐬
	bool Create(HWND hWmd);

	bool isDeviceCreated ;
	bool IsDeviceCreated(){ return isDeviceCreated; }

	//�R���X�g���N�^�@�f�X�g���N�^
	Direct3D();
	~Direct3D();

	FuncPointer DrawFunc;//�֐��|�C���^
	void TryCallDrawFunc();
	

	void ReleaseDevice();
};