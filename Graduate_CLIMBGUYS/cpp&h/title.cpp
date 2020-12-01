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
#include "bg.h"
#include "connectblock.h"
#include "stagingblock.h"
//#include "3Deffect.h"
//#include "2Deffect.h"

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
	m_Phase = TITLE_PHASE::PHASE_ONE;
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

	// 3Dエフェクトの生成
	C2DEffect::Create();

	// タイトルUIの生成
	CUi::LoadCreate(CUi::UITYPE_TILTE);

	// ブロックの生成
	//CBaseblock::CreateInBulkBlock();

	// とりあえずの仮
	static D3DXVECTOR3 pos[MAX_STAGINGBLOCK];
	pos[0] = STAGINGBLOCK_POS_C;
	pos[1] = STAGINGBLOCK_POS_L;
	pos[2] = STAGINGBLOCK_POS_I;
	pos[3] = STAGINGBLOCK_POS_M;
	pos[4] = STAGINGBLOCK_POS_B;
	pos[5] = STAGINGBLOCK_POS_G;
	pos[6] = STAGINGBLOCK_POS_U;
	pos[7] = STAGINGBLOCK_POS_Y;
	pos[8] = STAGINGBLOCK_POS_S;

	// ブロックの最大数分生成しポインタを保存
	for (int nCnt = 0; nCnt < MAX_STAGINGBLOCK; nCnt++)
	{
		CStagingBlock::Create(
			pos[nCnt],
			D3DVECTOR3_ZERO,
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CScene_X::TYPE_OBJECT_BLOCK_C + nCnt,
			false);
	}

	// 背景生成
	CBg::Create(CTexture_manager::TYPE_BG_TITLE);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
{
	CStagingBlock::SetEnd(false);
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

	// 一定時間操作していなったら自動でオープニングへ移行する
	//if (CBaseMode::GetTransitionCnt() <= 0)
	//{
	//	// フェード状態が何も起こっていない状態なら
	//	if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
	//	{
	//		CManager::GetFade()->SetFade(CManager::MODE_OPENING);
	//	}
	//}
	//else
	//{
	//	CBaseMode::SetTransitionCnt(CBaseMode::GetTransitionCnt()-1);
	//}

	switch (m_Phase)
	{
	case CTitle::TITLE_PHASE::PHASE_ONE:
		// ゲーム遷移
		if (CCalculation::PressAnyButton())
		{
			m_Phase = TITLE_PHASE::PHASE_TWO;
		}
		break;
	case CTitle::TITLE_PHASE::PHASE_TWO:
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			// 決定音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_DETERMINATION);
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		}

		break;
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