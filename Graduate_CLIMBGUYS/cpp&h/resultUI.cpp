// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// リザルトUI[resultUI.cpp]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "resultUI.h"
#include "basemode.h"
#include "debugproc.h"
#include "scene_two.h"
#include "multinumber.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
/* D3DXVECTOR3((基準値 + 幅 * nCnt), ((サイズY / 2 + サイズY[下の画面外に出すため]) + (移動量 * ずらすフレーム数) * nCnt), 0.0f) */
#define RESULT_UI_FRAME_POS								(D3DXVECTOR3((163.0f + 320.0f * nCnt), (1080.0f + 210.0f * nCnt), 0.0f))		// 枠の位置
#define RESULT_UI_FRAME_SIZE							(D3DXVECTOR2(326.0f, SCREEN_HEIGHT))	// 枠のサイズ
#define RESULT_UI_FRAME_MOVE_STOP_POS_Y					(360.0f)								// 枠の移動が止まる位置Y

#define RESULT_UI_RANK_POS								(D3DXVECTOR3((265.0f + 320.0f * (nCnt - (int)RESULT_UI::RANK_01)), (740.0f + 210.0f * (nCnt - (int)RESULT_UI::RANK_01)), 0.0f))		// 順位の位置
#define RESULT_UI_RANK_SIZE								(D3DXVECTOR2(100.0f, 100.0f))			// 順位のサイズ
#define RESULT_UI_RANK_MOVE_STOP_POS_Y					(55.0f)									// 順位の移動が止まる位置Y

#define RESULT_UI_DEATH_POS								(D3DXVECTOR3((160.0f + 320.0f * (nCnt - (int)RESULT_UI::DEATH_01)), (1080.0f + 210.0f * (nCnt - (int)RESULT_UI::DEATH_01)), 0.0f))	// 死因の位置
#define RESULT_UI_DEATH_SIZE							(D3DXVECTOR2(200.0f, 80.0f))			// 死因のサイズ
#define RESULT_UI_DEATH_MOVE_STOP_POS_Y					(685.0f)								// 死因の移動が止まる位置Y

#define RESULT_SCORE_SURVIVAL_TIME_POS					(D3DXVECTOR3((230.0f + 320.0f * nCnt), (485.0f + SCREEN_HEIGHT), 0.0f))	// 生存時間の位置
#define RESULT_SCORE_SURVIVAL_TIME_MOVE_STOP_POS_Y		(485.0f)																// 生存時間の移動が止まる位置Y

#define RESULT_SCORE_PRESS_BLOCK_POS					(D3DXVECTOR3((230.0f + 320.0f * nCnt), (585.0f + SCREEN_HEIGHT), 0.0f))	// ブロックを押した回数の位置
#define RESULT_SCORE_PRESS_BLOCK_MOVE_STOP_POS_Y		(585.0f)																// ブロックを押した回数の移動が止まる位置Y

#define RESULT_SCORE_SIZE								(D3DXVECTOR2(35.0f, 50.0f))			// 数字のサイズ
#define RESULT_UI_MOVE									(D3DXVECTOR3(0.0f, -70.0f, 0.0f))	// リザルトUIの移動量

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::RECORD		CResultUI::m_Record[(int)PLAYER_TAG::PLAYER_MAX] = {};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CResultUI::CResultUI()
{
	// 初期化
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		m_pScene2D[nCnt]	= nullptr;			// シーン2D
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_ResultScore[nCnt] = {};				// リザルトスコア
	}
	m_move					= D3DVECTOR3_ZERO;	// 移動量
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CResultUI::~CResultUI()
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_Record[nCnt] = {};				// 記録情報
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Init(void)
{
	// 初期化
	m_move = RESULT_UI_MOVE;	// 移動量

	// 枠の初期設定
	InitSettingFrame();
	// 各スコアの初期設定
	InitSettingScore();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			m_pScene2D[nCnt] = nullptr;			// シーン2D
		}
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- リザルトスコア --- */
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			m_ResultScore[nCnt].m_pSurvivalTime = nullptr;	// 生存時間
		}
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			m_ResultScore[nCnt].m_pPressBlock = nullptr;	// ブロックを押した回数
		}
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Update(void)
{
	// 枠の更新設定
	UpdateSettingFrame();

	// 各スコアの更新設定
	UpdateSettingScore();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Draw(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			// 描画
			m_pScene2D[nCnt]->Draw();
		}
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- リザルトスコアの描画 --- */
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			m_ResultScore[nCnt].m_pSurvivalTime->Draw();	// 生存時間
		}
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			m_ResultScore[nCnt].m_pPressBlock->Draw();		// ブロックを押した回数
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CResultUI * CResultUI::Create(void)
{
	// 変数宣言
	CResultUI * pResultUI;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pResultUI = new CResultUI();

	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		// シーン2Dの生成
		pResultUI->m_pScene2D[nCnt] = CScene_TWO::Create(CScene_TWO::OFFSET_TYPE_CENTER, D3DVECTOR3_ZERO, D3DVECTOR2_ZERO, (CTexture_manager::TYPE_RESULT_UI_FRAME_1P + nCnt));
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		// リザルトスコアの生成
		pResultUI->m_ResultScore[nCnt].m_pSurvivalTime	= CMultiNumber::Create(D3DVECTOR3_ZERO, D3DVECTOR2_ZERO, 0, 3, 20.0f);
		pResultUI->m_ResultScore[nCnt].m_pPressBlock	= CMultiNumber::Create(D3DVECTOR3_ZERO, D3DVECTOR2_ZERO, 0, 2, 20.0f);
	}
	// 初期化処理
	pResultUI->Init();

	// 生成したオブジェクトを返す
	return pResultUI;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CResultUI::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// unload処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 記録情報の設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::SetRecord(PLAYER_TAG PlayerNum, CPlayer::RECORD record)
{
	m_Record[(int)PlayerNum] = record;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 枠の初期設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::InitSettingFrame(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			// 枠に共通する設定
			if (nCnt <= (int)RESULT_UI::FRAME_4P)
			{
				// 位置の設定
				m_pScene2D[nCnt]->SetPosition(RESULT_UI_FRAME_POS);
				// サイズの設定
				m_pScene2D[nCnt]->SetSize(RESULT_UI_FRAME_SIZE);
			}
			// 順位に共通する設定
			else if (nCnt <= (int)RESULT_UI::RANK_04)
			{
				// 位置の設定
				m_pScene2D[nCnt]->SetPosition(RESULT_UI_RANK_POS);
				// サイズの設定
				m_pScene2D[nCnt]->SetSize(RESULT_UI_RANK_SIZE);
				// テクスチャの設定
				m_pScene2D[nCnt]->BindTexture(CTexture_manager::TYPE_RESULT_UI_RANK_01 + m_Record[nCnt - (int)RESULT_UI::RANK_01].nRanking - 1);
			}
			// 死因に共通する設定
			else if (nCnt <= (int)RESULT_UI::DEATH_04)
			{
				// 位置の設定
				m_pScene2D[nCnt]->SetPosition(RESULT_UI_DEATH_POS);
				// サイズの設定
				m_pScene2D[nCnt]->SetSize(RESULT_UI_DEATH_SIZE);
				// テクスチャの設定
				m_pScene2D[nCnt]->BindTexture(CTexture_manager::TYPE_RESULT_UI_DEATH_01 + m_Record[nCnt - (int)RESULT_UI::DEATH_01].DieCause);
			}

			// 初期化
			m_pScene2D[nCnt]->Init();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 各スコアの初期設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::InitSettingScore(void)
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- リザルトスコアの初期設定 --- */
		// 生存時間
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			// 位置の設定
			m_ResultScore[nCnt].m_pSurvivalTime->SetPos(RESULT_SCORE_SURVIVAL_TIME_POS);
			// サイズの設定
			m_ResultScore[nCnt].m_pSurvivalTime->SetSize(RESULT_SCORE_SIZE);
			// スコアの設定
			m_ResultScore[nCnt].m_pSurvivalTime->SetScore(m_Record[nCnt].nTime);
		}
		// ブロックを押した回数
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			// 位置の設定
			m_ResultScore[nCnt].m_pPressBlock->SetPos(RESULT_SCORE_PRESS_BLOCK_POS);
			// サイズの設定
			m_ResultScore[nCnt].m_pPressBlock->SetSize(RESULT_SCORE_SIZE);
			// スコアの設定
			m_ResultScore[nCnt].m_pPressBlock->SetScore(m_Record[nCnt].nPushCnt);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 枠の更新設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::UpdateSettingFrame(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			// 位置
			D3DXVECTOR3 pos = D3DVECTOR3_ZERO;

			// 位置の取得
			pos = m_pScene2D[nCnt]->GetPosition();

			// 移動
			pos.y += m_move.y;

			// 枠に共通する設定
			if (nCnt <= (int)RESULT_UI::FRAME_4P)
			{
				// 移動を止める
				if (pos.y <= RESULT_UI_FRAME_MOVE_STOP_POS_Y)
				{
					pos.y = RESULT_UI_FRAME_MOVE_STOP_POS_Y;
				}
			}
			// 順位に共通する設定
			else if (nCnt <= (int)RESULT_UI::RANK_04)
			{
				// 移動を止める
				if (pos.y <= RESULT_UI_RANK_MOVE_STOP_POS_Y)
				{
					pos.y = RESULT_UI_RANK_MOVE_STOP_POS_Y;
				}
			}
			// 死因に共通する設定
			else if (nCnt <= (int)RESULT_UI::DEATH_04)
			{
				// 移動を止める
				if (pos.y <= RESULT_UI_DEATH_MOVE_STOP_POS_Y)
				{
					pos.y = RESULT_UI_DEATH_MOVE_STOP_POS_Y;
				}
			}

			// 位置の設定
			m_pScene2D[nCnt]->SetPosition(pos);
			m_pScene2D[nCnt]->Set_Vtx_Pos();
			// 更新
			m_pScene2D[nCnt]->Update();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 各スコアの更新設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::UpdateSettingScore(void)
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- リザルトスコアの更新 --- */
		// 生存時間
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			// 位置
			D3DXVECTOR3 SurvivalTimePos = m_ResultScore[nCnt].m_pSurvivalTime->GetPos();
			SurvivalTimePos.y += m_move.y;

			// 移動を止める
			if (SurvivalTimePos.y <= RESULT_SCORE_SURVIVAL_TIME_MOVE_STOP_POS_Y)
			{
				SurvivalTimePos.y = RESULT_SCORE_SURVIVAL_TIME_MOVE_STOP_POS_Y;
			}
			// 位置の設定
			m_ResultScore[nCnt].m_pSurvivalTime->SetPos(SurvivalTimePos);

			// 更新
			m_ResultScore[nCnt].m_pSurvivalTime->Update();
		}
		// ブロックを押した回数
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			// 位置
			D3DXVECTOR3 PressBlockPos = m_ResultScore[nCnt].m_pPressBlock->GetPos();
			PressBlockPos.y += m_move.y;

			// 移動を止める
			if (PressBlockPos.y <= RESULT_SCORE_PRESS_BLOCK_MOVE_STOP_POS_Y)
			{
				PressBlockPos.y = RESULT_SCORE_PRESS_BLOCK_MOVE_STOP_POS_Y;
			}
			// 位置の設定
			m_ResultScore[nCnt].m_pPressBlock->SetPos(PressBlockPos);

			// 更新
			m_ResultScore[nCnt].m_pPressBlock->Update();
		}
	}
}