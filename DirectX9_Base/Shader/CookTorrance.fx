float4x4 matWorldViewProj;
float4x4 matWorldInverseTranspose;

float3 vLightDir;
float3 vEyePos;


float4 I_a = {0.2f,0.2f,0.2f,1.0f};
float4 I_d = {0.8f,0.8f,0.8f,1.0f};


float4 K_a = {1.0f,1.0f,1.0f,1.0f};
float4 K_d = {1.0f,1.0f,1.0f,1.0f};

texture Tex;
sampler Samp =sampler_state
{
	Texture = <Tex>;
	MinFilter  = LINEAR;
	MagFilter  = LINEAR;
	MipFilter  = NONE;

	AddressU =Clamp;
	AddressV =Clamp;
};

struct VS_OUTPUT
{
	float4 Pos 	: POSITION;
	float2 uv	: TEXCOORD0;
	float3 N	: TEXCOORD1;
	float3 X	: TEXCOORD2;

	float4 Color : 	COLOR0;
};

VS_OUTPUT VS( 
	float4 LocalPos : POSITION 	,
	float4 Normal	: NORMAL	,
	float2 Tex      : TEXCOORD0
) 
{
	VS_OUTPUT Out =(VS_OUTPUT)0;
	
	//���W�ϊ�
	Out.Pos=mul( LocalPos, matWorldViewProj );

	float3 E =normalize(vEyePos-LocalPos.xyz);
	float3 L = -vLightDir;
	float3 N = normalize(Normal.xyz);

	//�g�U��
	Out.Color =(I_a*K_a)+ (K_d * I_d*(max(0,dot(N,L))));

	//���ʔ��˗p�̃x�N�g��
	Out.X= LocalPos.xyz;
	Out.N= N;
	
	//�e�N�X�`�����W
	Out.uv =Tex;	

   	return Out;
}

float4 PS( VS_OUTPUT  In) : COLOR0
{
float4 color = tex2D(Samp,In.uv);

	//�N�b�N�E�g�����X���f�� (�������ʔ���)

	//���C�g�x�N�g��
	float3 L = -vLightDir;

	//�@���x�N�g��
	float3  N = normalize(In.N);
	//���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn����ߒ��ŕ�Ԃ����
	//������1�łȂ��Ȃ��Ă���̂ōēx�W�����x�N�g���ɒ���
	
	//�����x�N�g��
	//���_�̍��W�@-�@���̃s�N�Z���ɑΉ����郍�[�J�����W
	//���W���王�_�ւ̃x�N�g��
	float3 E = normalize(vEyePos - In.X);//�Q�l������E�łȂ�V�Ƃ��Ă���

	//�����x�N�g���ƃ��C�g�x�N�g���̃n�[�t�x�N�g��
	float3 H = normalize(E + L);

	//�v�Z�Ɏg�p����x�N�g�����m�̂Ȃ��p�̊p�x(cos)���v�Z
	float NE = dot(N, E);
	float NH = dot(N, H);
	float EH = dot(E, H);
	float NL = dot(N, L);
	float LH = dot(L, H);

	//�N�b�N�g�����X�̔��˃��f��
	// �F *((D * G * F )/ (N�EV));

	//D �}�C�N���t�@�Z�b�g�̕��z�֐�
	//�����̔��˃x�N�g�������C�g�̕�������������
	//Becmann���z���g�p
	const float m = 0.35;//�\�ʂ̑e���@0�ɋ߂��ق������
	float NH2 = NH * NH;
	float D = exp(-(1 - NH2) / (NH2 * m * m)) / (4 * m * m * NH2 * NH2);
	//exp (exponential) �w���֐� ���R�ΐ��̒�̉���ł��邩
	//e^(X) �ƕ\������镨�ƈꏏ

	//G �􉽊w������
	//�}�C�N���t�@�Z�b�g�ɓ��˂��������ʂ̃}�C�N���t�@�Z�b�g�ɉe��������
	//���ˌ����Օ������ꍇ�Ɠ��ˌ����Օ������ꍇ�ɓ������
	float Ga = 2 * (NH*NE) / (EH); //���ˌ��������ɓ͂�����								   
	float Gb = 2 * (NH*NL) / (EH); //���ˌ����Ղ��Ȃ���������
	//Ga,Gb�͎Օ����ꂽ������1����}�C�i�X����Ă���
	float G = min(1, min(Ga, Gb));//Ga,Gb,1�̓��̍Œ�l���̗p����

	//F �t���l�� (���������̂܂܎g�p)
	//�d���C�w�I�Ȍ���
	//�ʂƌ��̓��˂Ƌ��ܗ��ɂ���Ĕ��˂�����̋������ς��
	float n = 3.0f;//(���f)���ܗ��̎����� //�����̏ꍇ�͋��ܗ��͕��f���ɂȂ�炵��
	float g = sqrt( n * n + LH * LH - 1);
	float gpc = g + LH;
	float gnc = g - LH;
	float cgpc = LH*gpc - 1;
	float cgnc = LH*gnc + 1;
	float F = 0.5f * gnc*gnc * (1 + (cgpc*cgpc) / (cgnc*cgnc)) / (gpc*gpc);

	//�����̐F
	float4 ks = color;
	//���̐F
	float4 light_color = { 1.0f,1.0f,1.0f,1.0f };

	float4 color0 = { ks.x*light_color.x,
					 ks.y*light_color.y,
					 ks.z*light_color.z,
					 1.0f};

	//�J���[�V�t�g�@
	//���ˌ��ɋ����̐F���ӂ��܂�邱��
	//���������˕����Ǝ����̕������^���΂̏ꍇ�͑S���˂��N���邽��
	//���ˌ��̐F�����̂܂ܕԂ�
	//�����̂قڐ��ʂ���Ƃ炵���Ƃ��Ƃ̒��Ԃ���`��ԂŌv�Z����

	//�����ƌ��̃x�N�g�����d�Ȃ��Ă���Ƃ��̔��ˌ��F
	//���F�̎���LH��1�ƌ������̎� (cos(0)=1���)
	float F0 = ((n - 1) / (n + 1))*((n - 1) / (n + 1));

	float4 colorR = color0;	

	//t ����Ԃ̕ϐ�
	float t = max(0, F - F0) / (1 - F0);
	colorR = (1 - t)*color0 + (t*light_color);
	
	
	//4�̍����܂Ƃ߂ċ��ʔ��ˌv�Z�̊���
    colorR = colorR * max(0, F * G * D / (NE));

	// *  1 / NV ; �ϑ��҂��猩����ʂ̌����ڏ�̃}�C�N���t�@�Z�b�g�̔Z�x
	//             ���̕����ɋ��ʔ��˂��o�₷���Ȃ����

	//�����@�g�U��
	color = color* In.Color;

	//���ʔ���
	color = color+colorR;

	return color;
}

technique BasicTec
{
   pass P0
   {
      	VertexShader 	= compile vs_2_0 VS();
				PixelShader 	= compile ps_3_0 PS();
   }
}
