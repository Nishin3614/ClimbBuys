// ----------------------------------------
//
// 円形キャラクター処理の説明[circleshadow.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "circleshadow.h"
#include "character.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define CIRCLESHADOWCOLLISION_SIZE (30.0f)

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CCircleshadow::CCircleshadow() : CScene_THREE::CScene_THREE()
{
	m_bShadow = true;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CCircleshadow::~CCircleshadow()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CCircleshadow::Init(void)
{
	// オフセットタイプ設定
	CScene_THREE::SetOffsetType(OFFSET_TYPE_SIDE_CENTER);
	// 色の設定
	CScene_THREE::SetCol(D3DXCOLOR_CA(1.0f,1.0f));
	// シーン3Dの初期化
	CScene_THREE::Init();
	//CScene_THREE::SetLighting(true);		// ライティング
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CCircleshadow::Uninit(void)
{
	CScene_THREE::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CCircleshadow::Update(void)
{
	CScene_THREE::Update();
#ifdef _DEBUG
	D3DXVECTOR3 pos = CScene_THREE::GetPos();
	/*
	CDebugproc::Print("影の場所:(%.1f,%.1f,%.1f)\n", pos.x, pos.y, pos.z);
	*/
#endif // _DEBUG

}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CCircleshadow::Draw(void)
{
	// シャドウ状態がfalseなら
	// ->関数を抜ける
	if (!m_bShadow) return;
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	/*
	// 減算
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	*/
	// 描画
	CScene_THREE::Draw();
	/*
	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	*/
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CCircleshadow * CCircleshadow::Create(
	D3DXVECTOR3 const &	pos,						// 位置
	D3DXVECTOR3 const &	size,						// サイズ
	int const &			nTexType					// テクスチャータイプ
)
{
	// 変数宣言
	CCircleshadow * pCircleshadow;		// シーン3Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pCircleshadow = new CCircleshadow();
	// シーンマネージャ設定
	pCircleshadow->ManageSetting(CScene::LAYER_3DBLOCKSHADOW);
	// 位置設定
	pCircleshadow->SetPos(pos);
	// サイズ設定
	pCircleshadow->SetSize(D3DXVECTOR3(size.x,0.0f,size.z));
	// テクスチャータイプ設定
	pCircleshadow->SetTexType(nTexType);
	// 初期化処理
	pCircleshadow->Init();
	// 生成したオブジェクトを返す
	return pCircleshadow;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CCircleshadow::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// unload処理
// ----------------------------------------
void CCircleshadow::UnLoad(void)
{
}