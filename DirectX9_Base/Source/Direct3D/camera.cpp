#include "camera.h"
#include <math.h>

Camera::Camera()
{
	eyePoint = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	relLookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	absLookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	upVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	cameraMode = CAMERA_MODE::LOOK_AT_REL;
	UpdateLookAtPoint();

}

Camera::~Camera()
{

}

void Camera::UpdateLookAtPoint()
{
	switch (cameraMode)
	{

	case LOOK_AT_REL:
		lookAtPoint = eyePoint + relLookAtPoint;
		break;

	case LOOK_AT_ABS:
		lookAtPoint = absLookAtPoint;
		break;
	}
}

void Camera::SetCameraMode(CAMERA_MODE camMode)
{
	cameraMode = camMode;

	UpdateLookAtPoint();
}

void Camera::SetEyePoint(float x, float y, float z)
{
	eyePoint = D3DXVECTOR3(x, y, z);

	UpdateLookAtPoint();
}

void Camera::SetAbsLookAtPoint(float x, float y, float z)
{
	absLookAtPoint = D3DXVECTOR3(x, y, z);

	UpdateLookAtPoint();
}

void Camera::SetRelLookAtPoint(float x, float y, float z)
{
	relLookAtPoint = D3DXVECTOR3(x, y, z);

	UpdateLookAtPoint();
}

void Camera::SetUpVector(float x, float y, float z)
{
	upVec = D3DXVECTOR3(x, y, z);
}

void Camera::Move(float front, float right)
{
	//�J�������[�h��RELATIVE�̂Ƃ��̂�
	if (cameraMode == CAMERA_MODE::LOOK_AT_REL)
	{
		float rad = atan2(relLookAtPoint.z, relLookAtPoint.x);
		float frontY = sin(rad);
		float frontX = cos(rad);
		rad -= static_cast<float>(3.14159265 / 2);
		float rightY = sin(rad);
		float rightX = cos(rad);

		D3DXVECTOR3 frontVec(frontX*front, 0, frontY*front);
		D3DXVECTOR3 rightVec(rightX*right, 0, rightY*right);

		D3DXVECTOR3 newVec = eyePoint + frontVec + rightVec;
		SetEyePoint(newVec.x, newVec.y, newVec.z);
	}
}

void Camera::Rotate(float radYaw, float radPitch)
{
	//�J�������[�h��RELATIVE�̂Ƃ��̂�
	if (cameraMode == CAMERA_MODE::LOOK_AT_REL)
	{
		double x = relLookAtPoint.x;
		double y = relLookAtPoint.y;
		double z = relLookAtPoint.z;

		//z��x����s�b�`��]�̊p�x�����߂�@�i���W�A���j
		double first_radYaw = atan2(relLookAtPoint.z, relLookAtPoint.x);
		//�O�p�֐��̉��@�藝
		float resultZ =static_cast<float>( sin(first_radYaw) * cos(radYaw) + cos(first_radYaw) * sin(radYaw));
		float resultX = static_cast<float>(cos(first_radYaw) * cos(radYaw) - sin(first_radYaw) * sin(radYaw));
		//�����܂Ń��[��]�̏���

		//�ȉ��s�b�`��]�̏���
		double pitchX = (sqrt(x*x + z*z));	//x*z���ʏ�ł̌��_����̋��� //�Ȃ�Ė������Ă悢���킩��Ȃ��������߂Ƃ肠�������̕ϐ�����

		//x�Az���ʂł̋�����y����p�x�����߂�
		double first_radPitch = atan2(y, pitchX);
		//�p�x�ɏ��������ݒ�
		double addRadPitch = radPitch;
		if (addRadPitch + first_radPitch >= (3.1415f * 1 / 5))
		{
			addRadPitch = (3.1415f * 1 / 5) - first_radPitch;
		}
		if (addRadPitch + first_radPitch <= -(3.1415f * 1 / 5))
		{
			addRadPitch = (-(3.1415 * 1 / 5)) - first_radPitch;
		}
		//�O�p�֐��̉��@�藝
		float resultY = static_cast<float>(sin(first_radPitch)*cos(addRadPitch) + cos(first_radPitch)*sin(addRadPitch));
		float resultPitchX = static_cast<float>(cos(first_radPitch)*cos(addRadPitch) - sin(first_radPitch)*sin(addRadPitch));

		resultX *= resultPitchX;
		resultZ *= resultPitchX;

		relLookAtPoint = D3DXVECTOR3(resultX, resultY, resultZ);
	}

}

void Camera::SetViewMatrix()
{
	D3DXMATRIXA16 matView;
	//�J���� //�r���[�s��̍쐬
	D3DXMatrixLookAtLH(&matView,GetEyePoint_p(),GetLookAtPoint_p(),GetUpVector_p());

	Direct3D::GetInstance().SetViewMatrix(matView);
}