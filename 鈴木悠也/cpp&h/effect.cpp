// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// エフェクトの処理[effect.cpp]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "effect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CEffect::CEffect() :CScene()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CEffect::~CEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::Init(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ライフ更新処理
//	pVtx	: 2D頂点情報
//	pEffect	: エフェクト情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CEffect::UpdateLife(
	EFFECT *pEffect		// エフェクト情報
)
{
	// ライフが0未満なら
	if (pEffect->nLife < 0)
	{
		// ライフが0になったとき
		pEffect->bUse = false;
		// falseを返す
		return false;
	}
	// それ以外なら
	else
	{
		// ライフを減らす
		pEffect->nLife--;
		// trueを返す
		return true;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ライフ更新処理
//	pVtx	: 2D頂点情報
//	pEffect	: エフェクト情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CEffect::UpdateVetex(
	EFFECT *pEffect		// エフェクト情報
)
{
	// 半径を変化させる
	pEffect->size += pEffect->sizeValue;
	// アルファ値を変化させる
	pEffect->col.a -= pEffect->fAlphaValue;

	/*
	// エフェクトタイプ
	switch (pEffect->EffectType)
	{
	default:
		break;
	}
	*/
	// 成功を返す
	return true;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ライフ更新処理
//	pVtx	: 2D頂点情報
//	pEffect	: エフェクト情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::UpdateMove(
	EFFECT *pEffect		// エフェクト情報
)
{
	/*
	// エフェクトタイプ
	switch (pEffect->EffectType)
	{
	default:
		break;
	}
	*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// アニメーション更新処理
//	pEffect	: エフェクト情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::Updata_Animation(
	EFFECT *pEffect		// エフェクト情報
)
{
	// アニメーション使用状態がfalseなら
	if (!pEffect->Animation.bUse) return;

	// カウントアニメアップ
	pEffect->Animation.nCntAnim++;
	// カウントアニメが最大カウントアニメで割り切れたら
	if (pEffect->Animation.nCntAnim % pEffect->Animation.nMaxCntAnim == 0)
	{
		// 水平アニメーションカウントアップ
		pEffect->Animation.nHorizonAnim++;
		if (pEffect->Animation.nHorizonAnim >= pEffect->Animation.nMaxHorizonAnim)
		{
			// 水平アニメーションカウント初期化
			pEffect->Animation.nHorizonAnim = 0;
			// 垂直アニメーションカウントアップ
			pEffect->Animation.nVirticalAnim++;
			if (pEffect->Animation.nVirticalAnim >= pEffect->Animation.nMaxVirticalAnim)
			{
				// 垂直アニメーションカウント初期化
				pEffect->Animation.nVirticalAnim = 0;
			}
		}
		// 始点位置
		pEffect->Animation.FirstPos = D3DXVECTOR2(
			pEffect->Animation.nHorizonAnim * pEffect->Animation.fHorizonSize,
			pEffect->Animation.nVirticalAnim * pEffect->Animation.fVirticalSize
		);
		// 終点位置
		pEffect->Animation.EndPos = D3DXVECTOR2(
			pEffect->Animation.nHorizonAnim * pEffect->Animation.fHorizonSize + pEffect->Animation.fHorizonSize,
			pEffect->Animation.nVirticalAnim * pEffect->Animation.fVirticalSize + pEffect->Animation.fVirticalSize
		);
		// テクスチャーの更新処理状態
		pEffect->Animation.bTexUpdate = true;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// テクスチャーアニメーションの設定
//	nMaxCntAnim			: 最大アニメーションカウント
//	nMaxHorizonAnim		: 最大水平アニメーションカウント
//	nMaxVirticalAnim	: 最大垂直アニメーションカウント
//	bLoop				: ループ状態
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::SetTexAnim(
	EFFECT * pEffect,				// エフェクト情報
	int const & nMaxCntAnim,		// カウントアニメ最大数
	int const & nMaxHorizonAnim,	// 水平アニメーション最大数
	int const & nMaxVirticalAnim	// 垂直アニメーション最大数
)
{
	// 各項目の設定
	pEffect->Animation.nMaxCntAnim = nMaxCntAnim;				// 最大アニメーションカウント
	pEffect->Animation.nMaxHorizonAnim = nMaxHorizonAnim;		// 最大水平アニメーションカウント
	pEffect->Animation.nMaxVirticalAnim = nMaxVirticalAnim;		// 最大垂直アニメーションカウント
	pEffect->Animation.fHorizonSize = 1.0f / nMaxHorizonAnim;	// 垂直アニメーションの1つのサイズ
	pEffect->Animation.fVirticalSize = 1.0f / nMaxVirticalAnim;	// 垂直アニメーションの1つのサイズ
	pEffect->Animation.bUse = true;								// 使用状態

																// 始点位置
	pEffect->Animation.FirstPos = D3DXVECTOR2(
		pEffect->Animation.nHorizonAnim * pEffect->Animation.fHorizonSize,
		pEffect->Animation.nVirticalAnim * pEffect->Animation.fVirticalSize
	);
	// 終点位置
	pEffect->Animation.EndPos = D3DXVECTOR2(
		pEffect->Animation.nHorizonAnim * pEffect->Animation.fHorizonSize + pEffect->Animation.fHorizonSize,
		pEffect->Animation.nVirticalAnim * pEffect->Animation.fVirticalSize + pEffect->Animation.fVirticalSize
	);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 値の初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::InitValues(
	EFFECT *pEffect	// エフェクト情報
)
{
	// エフェクトループ
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{
		pEffect->pos				= D3DVECTOR3_ZERO;					// 位置
		pEffect->rot				= D3DVECTOR3_ZERO;					// 回転量
		pEffect->move				= D3DVECTOR3_ZERO;					// 移動量
		pEffect->col				= D3DXCOLOR_INI;					// 色
		pEffect->size				= D3DVECTOR2_ZERO;					// サイズ
		pEffect->sizeValue			= D3DVECTOR2_ZERO;					// 半径の変化値
		pEffect->fAngle				= 0.0f;								// 角度
		pEffect->fAlphaValue		= 0.0f;								// アルファ値の変化値
		pEffect->nLife				= 0;								// 持ち時間
		pEffect->nTexType			= 0;								// テクスチャータイプ
		pEffect->bUse				= false;							// 使用しているかどうか
		pEffect->Animation.Init();										// アニメーションの初期化処理
		pEffect->BlendType			= CRenderer::BLEND_ADD_TRANSLUCENT;	// ブレンドタイプ
		pEffect->EffectType			= CEffect::EFFECT_TYPE_NONE;		// エフェクトの種類
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 値の初期化処理
//	pEffect		: エフェクト情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::Init_OneValues(
	EFFECT * pEffect
)
{
	pEffect->pos = D3DVECTOR3_ZERO;							// 位置
	pEffect->rot = D3DVECTOR3_ZERO;							// 回転量
	pEffect->move = D3DVECTOR3_ZERO;						// 移動量
	pEffect->col = D3DXCOLOR_INI;							// 色
	pEffect->size = D3DVECTOR2_ZERO;						// サイズ
	pEffect->sizeValue = D3DVECTOR2_ZERO;					// 半径の変化値
	pEffect->fAngle = 0.0f;									// 角度
	pEffect->fAlphaValue = 0.0f;							// アルファ値の変化値
	pEffect->nLife = 0;										// 持ち時間
	pEffect->nTexType = 0;									// テクスチャータイプ
	pEffect->bUse = false;									// 使用しているかどうか
	pEffect->Animation.Init();								// アニメーションの初期化処理
	pEffect->BlendType = CRenderer::BLEND_ADD_TRANSLUCENT;	// ブレンドタイプ
	pEffect->EffectType = CEffect::EFFECT_TYPE_NONE;		// エフェクトの種類
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全リソース情報の読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CEffect::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全リソース情報の開放
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::Unload(void)
{
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CEffect::Debug(void)
{
}
#endif // _DEBUG
