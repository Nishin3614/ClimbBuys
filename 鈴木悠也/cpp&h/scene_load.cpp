// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// シーン処理の説明[scene_load.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_load.h"
/* 描画 */
#include "number.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "floor.h"
#include "meshwall.h"
#include "meshdome.h"
#include "meshsphere.h"
#include "collision.h"
#include "camera.h"
#include "character.h"
#include "circleshadow.h"
#include "time.h"
#include "fade.h"
#include "texture_manager.h"
#include "ui.h"
#include "scene_X.h"
#include "3Dmap.h"
#include "connect_fieldblock.h"
#include "baseblock.h"
#include "player.h"
#include "stencilshadow.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_load::CScene_load()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_load::~CScene_load()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// シーン親子作成処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_load::LoadAll(void)
{
	/* ----------テクスチャー読み込み---------- */
	// テクスチャー情報
	if (!CTexture_manager::Load() == S_OK)
	{
		CCalculation::Messanger("テクスチャー情報読み取り失敗");
	}
	// モデルの情報
	if (!CScene_X::LoadModel() == S_OK)
	{
		CCalculation::Messanger("モデル情報読み取り失敗");
	}
	/* UI */
	// UI
	if (!CUi::Load() == S_OK)
	{
		CCalculation::Messanger("UI読み取り失敗");
	}
	// フェード
	if (!CFade::Load() == S_OK)
	{
		CCalculation::Messanger("フェード読み取り失敗");
	}

	/* 効果 */
	// 3Deffect
	if(!C3DEffect::Load()== S_OK)
	{
		CCalculation::Messanger("3Dエフェクト読み取り失敗");
	}
	// 3Dparticle
	if(!C3DParticle::Load()== S_OK)
	{
		CCalculation::Messanger("3Dパーティクル読み取り失敗");
	}
	/*
	/* ゲームに必要なもの */
	// カメラ
	if (!CCamera::Load() == S_OK)
	{
		CCalculation::Messanger("カメラ読み取り失敗");
	}
	/* 2Dオブジェクト */
	// 番号
	if (!CNumber::Load() == S_OK)
	{
		CCalculation::Messanger("番号読み取り失敗");
	}

	// ステンシルシャドウ
	if (!CStencilshadow::Load() == S_OK)
	{
		CCalculation::Messanger("ステンシルシャドウ読み取り失敗");
	}

	/* 3Dオブジェクト*/

	/* メッシュ */
	// 床
	if (!CFloor::Load() == S_OK)
	{
		CCalculation::Messanger("床読み取り失敗");
	}
	// メッシュドーム
	if (!CMeshdome::Load() == S_OK)
	{
		CCalculation::Messanger("メッシュドーム読み取り失敗");
	}
	// メッシュスフィア
	if (!CMeshsphere::Load() == S_OK)
	{
		CCalculation::Messanger("メッシュスフィア読み取り失敗");
	}
	// メッシュウォール
	if (!CMeshwall::Load() == S_OK)
	{
		CCalculation::Messanger("メッシュウォール読み取り失敗");
	}

	/* 3Dモデル*/
	// キャラクター情報
	if (!CCharacter::Load() == S_OK)
	{
		CCalculation::Messanger("すべてキャラクター情報読み取り失敗");
	}
	// 円形シャドウ
	if (!CCircleshadow::Load() == S_OK)
	{
		CCalculation::Messanger("円形シャドウ読み取り失敗");
	}
	/*
	// 3Dマップ
	if (!C3DMap::Load() == S_OK)
	{
		CCalculation::Messanger("3Dマップ読み取り失敗");
	}
	*/
	// 結合されたフィールドブロック
	if (!CConnect_fieldblock::Load() == S_OK)
	{
		CCalculation::Messanger("結合されたフィールドブロック読み取り失敗");
	}
	// ベースブロックの情報
	if (!CBaseblock::Load() == S_OK)
	{
		CCalculation::Messanger("ベースブロック情報読み取り失敗");
	}
	// プレイヤーの情報
	if (!CPlayer::Load() == S_OK)
	{
		CCalculation::Messanger("プレイヤーのステータス読み取り失敗");
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだものを破棄する処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_load::UnLoadAll(void)
{
	/* 読み込んだものの破棄*/
	// 番号
	CTexture_manager::UnLoad();
	// 番号
	CNumber::UnLoad();
	// UI
	CUi::UnLoad();
	// 床
	CFloor::UnLoad();
	// キャラクター
	CCharacter::UnLoad();
	// 3Deffect
	C3DEffect::Unload();
	// 3Dparticle
	C3DParticle::Unload();
	// メッシュドーム
	CMeshdome::UnLoad();
	// メッシュスフィア
	CMeshsphere::Unload();
	// 円形シャドウ
	CCircleshadow::UnLoad();
	// ステンシルシャドウ
	CStencilshadow::UnLoad();
	// メッシュウォール
	CMeshwall::UnLoad();
	// フェード
	CFade::UnLoad();
	// 3Dマップ
	//C3DMap::UnLoad();
	// 結合されたフィールドブロック
	CConnect_fieldblock::UnLoad();
	// ベースブロック
	CBaseblock::UnLoad();
	// シーンX
	CScene_X::UnLoadModel();
	// プレイヤー
	CPlayer::UnLoad();
}