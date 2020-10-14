//=============================================================================
//
// マスク処理 [MaskShader.fx]
// Author : masayasu wakita
//
//=============================================================================
//=================================================================================
//型定義
//=================================================================================
struct VS_OUT
{
    float4 Position	: POSITION;
    float4 Color	: COLOR0;
    float2 Texture : TEXCOORD0;
    float2 MaskTex : TEXCOORD1;
};

//=================================================================================
//グローバル
//=================================================================================
float4x4 WVP; //ワールド行列
float4x4 WIT;  //ワールド行列の逆行列の転置行列
float4 LightDir;  //ｒライトの方向ベクトル
float4 LightIntensity; //入射光の強度

float4 Diffuse = {1.0f, 1.0f, 1.0f, 1.0f}; //拡散反射率
float4 Ambient = {1.0f, 1.0f, 1.0f, 1.0f};	// 環境光

float fDivision;
float uv_left;
float uv_top;
float uv_width;
float uv_height;

texture texDecal; //メッシュのテクスチャ
texture texMask;    // マスク用テクスチャ

sampler Sampler = sampler_state //サンプラー
{
    Texture = <texDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler Sampler_Mask = sampler_state //サンプラー
{
    Texture = <texMask>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

//=================================================================================
//バーテックス・シェーダー
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
//ピクセル・シェーダー
//=================================================================================
float4 PS(VS_OUT In) : COLOR
{   
    // テクスチャ座標系の計算
    float2 work = In.Texture;

    // テクスチャ座標系の計算
    In.MaskTex = In.Texture * fDivision;

    // 10倍にする
    work.x *= 10;
    work.y *= 10;

    work.x = work.x % 10;
    work.y = work.y % 10;

    // 0.1倍する
    work.x *= 0.1f;
    work.y *= 0.1f;

    In.Texture = work;

    // 座標の色を取得
    float4 output = In.Color * tex2D(Sampler, In.Texture);
    float4 mask = tex2D(Sampler_Mask, In.MaskTex);
    output.a = mask.r;

    return output;
}

//=================================================================================
// テクニック
//=================================================================================
technique tecLambert
{
    pass Lambert
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}