// ------------------------------------------
//
// チュートリアル処理 [tutorial.cpp]
// Author : Nishiyama Koki
//
// ------------------------------------------
#include "tutorial.h"
#include "manager.h"
#include "keyboard.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "collision.h"
#include "XInputPad.h"

/* 描画 */
#include "fade.h"
#include "character.h"
#include "3Deffect.h"
#include "floor.h"
#include "meshobit.h"
#include "meshdome.h"
#include "meshsphere.h"
#include "meshwall.h"
#include "player.h"
#include "ui_group.h"
#include "3Dmap.h"
#include "stand.h"
#include "debugproc.h"
#include "meshbox.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CTutorial::CTutorial()
{
	// ステージ決定カウントを設定
	m_nDeterminationCnt = 120;
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CTutorial::~CTutorial()
{
}

// ------------------------------------------
// 初期化処理
// ------------------------------------------
void CTutorial::Init(void)
{
	// モード初期化
	CBaseMode::Init();

	// 床の生成
	CFloor::Create(D3DVECTOR3_ZERO,D3DXVECTOR3(1000.0f,0.0f, 1000.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DVECTOR3_ZERO,2,2,0);

	// 足場の生成
	CStand::CreateStand_Tutorial();

	// プレイヤーの生成
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// プレイヤーの生成	試験的
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(0.0, 300.0f, 0.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(100.0f, 300.0f, 0.0f));
	//// 足場の生成
	CStand::CreateStand_Tutorial();
}

// ------------------------------------------
// 終了処理
// ------------------------------------------
void CTutorial::Uninit(void)
{
	// モード終了
	CBaseMode::Uninit();
}

// ------------------------------------------
// 更新処理
// ------------------------------------------
void CTutorial::Update(void)
{
#ifdef _DEBUG
	CXInputPad *InpudPad[(int)PLAYER_TAG::PLAYER_MAX] = {};

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		InpudPad[nCnt] = CManager::GetPad((PLAYER_TAG)nCnt);

		// ゲーム遷移
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN) || InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_START, 1))
		{
			// フェード状態が何も起こっていない状態なら
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
		}
	}

	CDebugproc::Print("チュートリアルカウント < %d\n" ,m_nDeterminationCnt);
#endif // _DEBUG

	// カウントが0以下になったらゲームに移動
	if (m_nDeterminationCnt <= 0)
	{
		m_nDeterminationCnt = 0;
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
	}

	if (CStand::GetDetermination() && m_nDeterminationCnt >= 0)
	{
		m_nDeterminationCnt--;
	}
	else
	{
		m_nDeterminationCnt = 120;
	}

	// モード更新
	CBaseMode::Update();
}

// ------------------------------------------
// 描画処理
// ------------------------------------------
void CTutorial::Draw(void)
{
	// モード描画
	CBaseMode::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTutorial * CTutorial::Create(void)
{
	// 変数宣言
	CTutorial * pTutorial = new CTutorial;
	// 初期化処理
	pTutorial->Init();
	// 生成したオブジェクトを返す
	return pTutorial;
}
