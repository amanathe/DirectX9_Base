#pragma once

#include "GameObject.h"
#include "../Direct3D/mesh.h"
#include "../BoundingBox/OrientedBoundinngBox.h"

class GameObject3D : public GameObject
{ 
protected:

	//���W
	D3DXVECTOR3 position;

	//�e�����̃x�N�g��
	D3DXVECTOR3 forwardVector;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 upVector;

	//y���𒆐S�Ƃ�����]�x
	//���̑��̎��͍��̂Ƃ���g�p���Ȃ�
	float rotateYaw_Rad;

	//���b�V���N���X�ւ̃|�C���^
	Mesh* pMesh;

	OrientedBoundingBox obb;

public :

	bool moveF;
	bool rotateF;

	GameObject3D();

	

	virtual ~GameObject3D();

	virtual void Update();

	virtual void Draw();

	void DrawLine();

	void SetMesh(Mesh* pMesh_){ if (pMesh_ != NULL)pMesh = pMesh_; }

	//���ʁ@��@�E�����̃x�N�g�����X�V
	void UpdateVectors();

	//y���𒆐S�Ƃ�����]���Z�b�g
	void SetRotateYaw(float yawRad){ rotateYaw_Rad = yawRad; UpdateVectors(); };

	//���W��ݒ�
	void SetPosition(float x, float y, float z){ position.x = x; position.y = y; position.z = 0; }

	//��]�x�擾
	float GetRotateYaw_rad(){ return rotateYaw_Rad; }

	//���W�擾
	D3DXVECTOR3 GetPosition(){ return position; }

	//�o�E���f�B�����O�{�b�N�X�擾
	OrientedBoundingBox* GetObb(){ return &obb; }

	//���ʕ����ւ̃x�N�g��
	D3DXVECTOR3 GetForwardVector()
	{ 
		return forwardVector;
	}

	//�E�����ւ̃x�N�g��
	D3DXVECTOR3 GetRightVector()
	{		
		return rightVector;
	}

	//������ւ̃x�N�g��
	D3DXVECTOR3 GetUpVector()
	{	
		return upVector;
	}
};