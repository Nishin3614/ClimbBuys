// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// タイトル処理 [title.cpp]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "keyboard.h"
#include "sound.h"

/* 描画 */
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "ui.h"
#include "2Deffect.h"
#include "3Dparticle.h"
#include "keyboard.h"
#include "baseblock.h"
#include "normalblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTitle::CTitle()
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTitle::~CTitle()
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Init(void)
{
	// モード初期化
	CBaseMode::Init();

	// タイトルUIの生成
	CUi::LoadCreate(CUi::UITYPE_TILTE);

	// ブロックの生成
	CBaseblock::CreateInBulkBlock();
	// ブロックテスト
	CNormalblock::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), 2);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
{
	// モード終了
	CBaseMode::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Update(void)
{
	// モード更新
	CBaseMode::Update();
	// ゲーム遷移
	if (CCalculation::PressAnyButton())
	{
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Draw(void)
{
	// モード描画
	CBaseMode::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTitle * CTitle::Create(void)
{
	// 変数宣言
	CTitle * pTitle = new CTitle;
	// 初期化処理
	pTitle->Init();
	// 生成したオブジェクトを返す
	return pTitle;
}