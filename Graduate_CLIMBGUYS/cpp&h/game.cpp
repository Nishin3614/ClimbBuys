// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ゲーム処理 [game.cpp]
// Author : Nishiyama Koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "game.h"
/* 描画 */
#include "fade.h"
#include "floor.h"
#include "player.h"
#include "meshobit.h"
#include "meshdome.h"
#include "meshsphere.h"
#include "meshwall.h"
#include "3Deffect.h"
#include "time.h"
#include "collision.h"
#include "camera.h"
#include "3Dparticle.h"
#include "keyboard.h"
#include "ui_group.h"
#include "3Dmap.h"
#include "player.h"
#include "baseblock.h"
#include "connectblock.h"
#include "connect_fieldblock.h"
#include "damagefloor.h"
#include "bg.h"
#include "XInputPad.h"


#include "normalblock.h"
#include "fieldblock.h"

/* ポーズ */
//#include "pause.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CGame::CGame()
{
	m_pause = NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CGame::~CGame()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::Init(void)
{
	// モードの初期化
	CBaseMode::Init();

	/* 初期化 */
	// 静的変数の初期化
	StaticInit();
	/* 作成 */
	// 3Dエフェクトの生成
	C3DEffect::Create();
	// 試験的背景の生成
	CBg::Create();
	// プレイヤー
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// プレイヤーの生成	試験的
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1,D3DXVECTOR3(0.0,300.0f,0.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(100.0f, 300.0f, 0.0f));

	// 結合されたフィールドブロックの生成
	CConnect_fieldblock::Create(CConnect_fieldblock::STAGE_1);
	// ダメージ床の生成
	CDamageFloor::Create();
	/*
	// 球の設定
	CMeshsphere::Create(D3DXVECTOR3(0.0f, 0.0f, 3000.0f),
		10000.0f);
		*/
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// モードの終了処理
	CBaseMode::Uninit();
	/*
	// ポーズ
	if (m_pause != NULL)
	{
		m_pause->Uninit();
		delete m_pause;
		m_pause = NULL;
	}
	*/
	// シーンの静的変数の初期化
	StaticInit();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::Update(void)
{
	// モードの更新
	CBaseMode::Update();

	// 結合されたブロックの更新ブロック生成
	CConnectblock::Update_CreateBlock();
	/*
	// ポーズ状態ならば
	if (m_state == STATE_PAUSE)
	{
		if (m_pause != NULL)
		{
			m_pause->Update();
		}
	}
	*/

#ifdef _DEBUG

	CXInputPad *InpudPad[(int)PLAYER_TAG::PLAYER_MAX] = {};

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		InpudPad[nCnt] = CManager::GetPad((PLAYER_TAG)nCnt);

		// リザルト遷移
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN) || InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_START, 1))
		{
			// フェード状態が何も起こっていない状態なら
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			}
		}
	}
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画描画
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::Draw(void)
{
	// モードの描画
	CBaseMode::Draw();
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::Debug(void)
{
	// ベースモードのデバッグ処理
	CBaseMode::Debug();
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CGame * CGame::Create(void)
{
	// 変数宣言
	CGame * pGame = new CGame;
	// 初期化処理
	pGame->Init();
	// 生成したオブジェクトを返す
	return pGame;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的変数の初期化処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::StaticInit(void)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ポーズ状態処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGame::PauseState(void)
{
	/*
	// ゲーム状態をポーズに
	if (m_state != STATE_PAUSE)
	{
		m_state = STATE_PAUSE;
		// 更新を止める
		CScene::UpdateStop(true);
		// メニュー音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_MENU);
	}
	*/
}