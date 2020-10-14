// ==========================================================
//
// 湖[lake.cpp]
// Author : masayasu wakita
//
// ==========================================================
#include "lake.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "sound.h"
#include "character.h"
#include "spherecollision.h"
#include "light.h"
#include "camera.h"

// ==========================================================
// 静的メンバー変数の初期化
// ==========================================================
LPDIRECT3DTEXTURE9 CLake::m_pTex = NULL;
LPDIRECT3DTEXTURE9 CLake::m_pBumpTex = NULL;

// ==========================================================
//
// コンストラクタ
//
// ==========================================================
CLake::CLake() :CScene()
{
	m_fAnim = 0.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f);
}

// ==========================================================
//
// デストラクタ
//
// ==========================================================
CLake::~CLake()
{

}

// ==========================================================
// 初期化
// ==========================================================
void CLake::Init(void)
{
	MakeVertex();

	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	LPDIRECT3DTEXTURE9 pHeightTexture;			// テクスチャ
	D3DSURFACE_DESC desc;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/WaterBump.tga", &pHeightTexture)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	pHeightTexture->GetLevelDesc(0, &desc);
	if (FAILED(D3DXCreateTexture(pDevice, desc.Width, desc.Height, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pBumpTex)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	if (FAILED(D3DXComputeNormalMap(m_pBumpTex, pHeightTexture, NULL, 0, D3DX_CHANNEL_LUMINANCE, 20.0f)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}

	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/water.png", &m_pBumpTex)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}

	if (pHeightTexture != NULL)
	{
		pHeightTexture->Release();
		pHeightTexture = NULL;
	}

	//エフェクト作成
	if (FAILED(D3DXCreateEffectFromFile(CManager::GetRenderer()->GetDevice(), "data/LOAD/MAPPING/Ocean.fx", NULL, NULL, 0, NULL, &m_pEffect, NULL)))
	{
		MessageBox(NULL, "エフェクト作成失敗", "", MB_OK);
	}
}

// ==========================================================
// 終了
// ==========================================================
void CLake::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ==========================================================
// 更新
// ==========================================================
void CLake::Update(void)
{
	//m_fAnim += 0.0003f;
	//SetTex(D3DXVECTOR2(0.0f + m_fAnim, 0.0f + m_fAnim), D3DXVECTOR2(1.0f + m_fAnim, 1.0f + m_fAnim));
	//CScene_THREE::Update();
}

// ==========================================================
// 描画
// ==========================================================
void CLake::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxView;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mView, mProj;
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		m_pBumpTex
	);

	pDevice->GetTransform(D3DTS_VIEW, &mView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mProj);

	D3DXMATRIX mWVP = m_mtxWorld *mView* mProj;
	D3DXMATRIX mWorldInvTranspose;
	D3DXMatrixInverse(&mWorldInvTranspose, NULL, &m_mtxWorld);
	D3DXMatrixTranspose(&mWorldInvTranspose, &mWorldInvTranspose);

	D3DXMATRIX mViewInverse;
	D3DXMatrixInverse(&mViewInverse, NULL, &mView);
	m_pEffect->SetMatrix("mWorldViewProj", &mWVP);

	m_pEffect->SetMatrix("mWorld", &m_mtxWorld);

	m_pEffect->SetMatrixTranspose("mWorldInverseTranspose", &mWorldInvTranspose);
	m_pEffect->SetMatrix("mViewInverse", &mViewInverse);

	D3DXMATRIX m;
	D3DXVECTOR4 v;

	CLight *pLight = CManager::GetRenderer()->GetLight();
	D3DLIGHT9 Light = pLight->GetLight(CLight::TYPE_MAIN);
	D3DXVECTOR4 LightPos;
	LightPos = D3DXVECTOR4(Light.Direction.x, Light.Direction.y, Light.Direction.z, 0.0f);
	D3DXMatrixInverse(&m, NULL, &m_mtxWorld);
	D3DXVec4Transform(&v, &LightPos, &m);
	D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);
	m_pEffect->SetVector("vLightDir", &v);

	// 視線ベクトルを渡す
	m = m_mtxWorld * mView;
	D3DXMatrixInverse(&m, NULL, &m);
	D3DXVECTOR3 cameraPos = CManager::GetRenderer()->GetCamera()->GetPosV();
	v = D3DXVECTOR4(0,0,0, 1.0f);
	D3DXVec4Transform(&v, &v, &m);
	m_pEffect->SetVector("vEyePos", &v);

	static D3DXVECTOR2 vWave(0, 0);
	static float fDelta = 0.01f;

	//波の動き
	m_pEffect->SetValue("f2Wave", &vWave, sizeof(vWave));
	vWave.x += 0.001f;
	m_pEffect->SetFloat("g_fHeight", m_fHeight);
	m_fHeight += fDelta;
	if (m_fHeight <= 0.8f)
	{
		fDelta = 0.002f;
	}
	else if (m_fHeight > 1.2f)
	{
		fDelta = -0.002f;
	}
	m_pEffect->SetTechnique("tecOcean");
	UINT Passes = 0;
	m_pEffect->SetTexture("NormalMap", m_pBumpTex);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//if (SUCCEEDED(m_pEffect->Begin(&Passes, 0)))
	{
		//for (INT iPass = 0; iPass < Passes; iPass++)
		{
			//if (SUCCEEDED(m_pEffect->BeginPass(iPass)))
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP,
					0,
					2
				);

				//m_pEffect->EndPass();
			}
		}
		//m_pEffect->End();
	}
}

// ==========================================================
// 弾の生成
// ==========================================================
CLake *CLake::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CLake *pLake = NULL;

	// シーン動的に確保
	pLake = new CLake();

	if (pLake != NULL)
	{// 湖が存在していたとき
		pLake->m_pos = pos;							// 位置設定
		pLake->m_size = size;						// サイズ設定
		pLake->m_rot = D3DXVECTOR3(D3DX_PI / 2, 0.0, 0.0f);
		pLake->Init();								// 初期化
		pLake->ManageSetting(LAYER_3DOBJECT);
		return pLake;
	}

	// 値を返す
	return NULL;
}

// ==========================================================
// テクスチャロード
// ==========================================================
HRESULT CLake::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTex);
	return S_OK;
}

// ==========================================================
// 破棄
// ==========================================================
void CLake::Unload(void)
{
	// テクスチャ解放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ==========================================================
// 頂点情報の作成
// ==========================================================
void CLake::MakeVertex(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross0, Cross1_2, Cross3;	// 外積
	int nCntBlock = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);

	// 頂点座標
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ベクトル
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pVtx[2].pos - pVtx[0].pos;
	// 外積計算
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[2].pos - pVtx[3].pos;
	VecB = pVtx[1].pos - pVtx[3].pos;
	// 外積計算
	Cross3 = CCalculation::Cross_product(VecA, VecB);
	// 正規化
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross3, &Cross3);
	// 外積計算
	Cross1_2 = (Cross0 + Cross3) / 2;

	pVtx[0].nor = Cross0;
	pVtx[1].nor = Cross1_2;
	pVtx[2].nor = Cross1_2;
	pVtx[3].nor = Cross3;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtxBuff->Unlock();
}