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
#include "camera.h"
#include "3Dparticle.h"
#include "keyboard.h"
#include "ui_group.h"
#include "player.h"
#include "baseblock.h"
#include "connectblock.h"
#include "connect_fieldblock.h"
#include "damagefloor.h"
#include "bg.h"
#include "XInputPad.h"
#include "gameUI.h"

#include "normalblock.h"
#include "fieldblock.h"
#include "springblock.h"

#include "sound.h"

/* ポーズ */
//#include "pause.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define FINISH_TIME		(120)			// 終了する時間

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CGame::STAGE	CGame::m_Stage = CGame::STAGE_1;	// ステージ
int				CGame::m_nCntTime = 0;				// カウントタイム

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CGame::CGame()
{
	// 初期化
	m_pause			= NULL;
	m_pGameUI		= nullptr;		// ゲームUI
	m_nCntFinish	= 0;			// 終了のカウント
	m_bFinishFlag	= false;		// 終了フラグ
	m_bBgm			= false;		// 一度だけ処理を通す
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
	// 死亡人数の初期化
	CPlayer::InitDieCount();
	/* 初期化 */
	// 静的変数の初期化
	StaticInit();
	/* 作成 */
	// 3Dエフェクトの生成
	C3DEffect::Create();
	// 試験的背景の生成
	CBg::Create(CTexture_manager::TYPE_BG);
	// ゲームUIの生成
	m_pGameUI = CGameUI::Create();
	// オブジェクト生成
	CScene_X::Create(D3DXVECTOR3(0.0f,0.0f,0.0f),
		D3DVECTOR3_ZERO,
		CScene_X::TYPE_OBJECT_VOLCANO);
	// プレイヤー
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};
	// プレイヤーの生成	試験的
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(-50.0, 300.0f, 50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(50.0, 300.0f, 50.0f));

	// 結合されたフィールドブロックの生成
	CConnect_fieldblock::Create(m_Stage);
	// 結合されたブロックの更新ブロック生成
	//CConnectblock::TestCreate();
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
	// 静的変数の初期化
	CPlayer::InitDieCount();

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

	if (m_pGameUI)
	{
		// ゲームUIの終了
		m_pGameUI->Uninit();
		delete m_pGameUI;
		m_pGameUI = nullptr;
	}

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
	// ブロックのフェーズ状態設定
	if (m_nCntTime > 0 &&
		m_nCntTime % DERAY_TIME(CBaseblock::GetBlockStatus().nChangeTime) == 0)
	{
		int nPhase = CBaseblock::GetPhase();
		nPhase++;
		CBaseblock::SetPhase(nPhase);
	}
	// タイムカウント更新
	m_nCntTime++;
	// NULLチェック
	if (m_pGameUI)
	{
		// ゲームUIの更新
		m_pGameUI->Update();
	}

	// スタートの表示が出た後に生成
	if (m_pGameUI->GetStartFlag())
	{
		m_bOperatable = true;
		if (m_bOperatable && !m_bBgm)
		{
			// ゲームスタート
			CManager::GetSound()->PlaySound(CSound::LABEL_BGM_GAME);
			m_bBgm = true;
		}

		// 結合されたブロックの更新ブロック生成
		CConnectblock::Update_CreateBlock();
	}
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

	// 終了フラグがオンになったとき
	if (m_bFinishFlag)
	{
		m_bOperatable = false;
		if (m_nCntFinish <= 0)
		{
			// カウントダウン
			CManager::GetSound()->StopSound(CSound::LABEL_BGM_GAME);
		}
		// カウントアップ
		m_nCntFinish++;

		// 終了の表示を出す
		m_pGameUI->FinishSignal();

		// 一定時間経過後
		if (m_nCntFinish >= FINISH_TIME)
		{
			// リザルトに遷移する
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			}
		}
	}
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
				//CManager::GetFade()->SetFade(CManager::MODE_RESULT);
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

	if (m_pGameUI)
	{
		// ゲームUIの描画
		m_pGameUI->Draw();
	}
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
	m_nCntTime = 0;
	CCharacter::InitStatic();
	CBaseblock::BlockStaticValue();
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