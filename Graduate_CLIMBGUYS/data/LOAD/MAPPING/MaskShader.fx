//=============================================================================
//
// �}�X�N���� [MaskShader.fx]
// Author : masayasu wakita
//
//=============================================================================
//=================================================================================
//�^��`
//=================================================================================
struct VS_OUT
{
    float4 Position	: POSITION;
    float4 Color	: COLOR0;
    float2 Texture : TEXCOORD0;
    float2 MaskTex : TEXCOORD1;
};

//=================================================================================
//�O���[�o��
//=================================================================================
float4x4 WVP; //���[���h�s��
float4x4 WIT;  //���[���h�s��̋t�s��̓]�u�s��
float4 LightDir;  //�����C�g�̕����x�N�g��
float4 LightIntensity; //���ˌ��̋��x

float4 Diffuse = {1.0f, 1.0f, 1.0f, 1.0f}; //�g�U���˗�
float4 Ambient = {1.0f, 1.0f, 1.0f, 1.0f};	// ����

float fDivision;
float uv_left;
float uv_top;
float uv_width;
float uv_height;

texture texDecal; //���b�V���̃e�N�X�`��
texture texMask;    // �}�X�N�p�e�N�X�`��

sampler Sampler = sampler_state //�T���v���[
{
    Texture = <texDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler Sampler_Mask = sampler_state //�T���v���[
{
    Texture = <texMask>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

//=================================================================================
//�o�[�e�b�N�X�E�V�F�[�_�[
//=================================================================================
VS_OUT VS(float4 Position : POSITION, float3 Normal : NORMAL, float2 Texture : TEXCOORD0, float2 TexMask : TEXCOORD0)
{
	VS_OUT Out = (VS_OUT)0;   
	
	Out.Position=mul(Position,WVP);
	
	float3 WNormal=normalize(mul(Normal,(float3x3)WIT));

    // float4 col = {1.0f, 1.0f, 1.0f, 1.0f};

	// col.xyz = Ambient.xyz * LightIntensity.xyz * Diffuse.xyz * dot(WNormal,-LightDir.xyz);
    // col.w = Diffuse.w;

    // float4 BaseCol = {1.0f, 1.0f, 1.0f, 0.0f};
    // // Out.Color = col * BaseCol;
    Out.Texture = Texture;
    Out.MaskTex = TexMask;
    Out.Color = LightIntensity * Diffuse * max(Ambient, dot(WNormal,-LightDir.xyz));
    // Out.Texture = Texture * float2(uv_width, uv_height) + float2(uv_left, uv_top);
	return Out;
}
//=================================================================================
//�s�N�Z���E�V�F�[�_�[
//=================================================================================
float4 PS(VS_OUT In) : COLOR
{   
    // �e�N�X�`�����W�n�̌v�Z
    float2 work = In.Texture;

    // �e�N�X�`�����W�n�̌v�Z
    In.MaskTex = In.Texture * fDivision;

    // 10�{�ɂ���
    work.x *= 10;
    work.y *= 10;

    work.x = work.x % 10;
    work.y = work.y % 10;

    // 0.1�{����
    work.x *= 0.1f;
    work.y *= 0.1f;

    In.Texture = work;

    // ���W�̐F���擾
    float4 output = In.Color * tex2D(Sampler, In.Texture);
    float4 mask = tex2D(Sampler_Mask, In.MaskTex);
    output.a = mask.r;

    return output;
}

//=================================================================================
// �e�N�j�b�N
//=================================================================================
technique tecLambert
{
    pass Lambert
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}