//=============================================================================
//
// リザルトの処理 [result.cpp]
// Author : Ryouma Inoue
//
//=============================================================================
#include "result.h"
#include "fade.h"

#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "player.h"
#include "manager.h"
#include "ui.h"
#include "keyboard.h"
#include "3Deffect.h"
#include "bg.h"
#include "connect_fieldblock.h"
#include "resultUI.h"

//=============================================================================
//
// コンストラクタ
//
//=============================================================================
CResult::CResult()
{
	// 初期化
	m_pResultUI				= nullptr;		// リザルトUI
	m_nCntPressButton		= 0;			// ボタンを押した回数
}

//=============================================================================
//
// デストラクタ
//
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//
// 初期化処理
//
//=============================================================================
void CResult::Init()
{
	// モード初期化
	CBaseMode::Init();

	// 3Dエフェクトの生成
	C3DEffect::Create();

	// 試験的背景の生成
	CBg::Create();

	// 結合されたフィールドブロックの生成
	CConnect_fieldblock::Create(CGame::STAGE_1);

	// プレイヤー
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// プレイヤーの生成	試験的
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(-50.0, 300.0f, 50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(50.0, 300.0f, 50.0f));
}

//=============================================================================
//
// 終了処理
//
//=============================================================================
void CResult::Uninit(void)
{
	// モード終了
	CBaseMode::Uninit();

	if (m_pResultUI)
	{
		// リザルトUIの終了
		m_pResultUI->Uninit();
		m_pResultUI = nullptr;
	}
}

//=============================================================================
//
// 更新処理
//
//=============================================================================
void CResult::Update(void)
{
	// モード更新
	CBaseMode::Update();

	// NULLチェック
	if (m_pResultUI)
	{
		// リザルトUIの更新
		m_pResultUI->Update();
	}

	// ボタンを押した回数を加算
	if (CCalculation::PressAnyButton())
	{
		m_nCntPressButton++;
	}

	// ボタンを押した回数によって起きる処理
	switch (m_nCntPressButton)
	{
	case 1:
		// リザルトUIが生成されていないとき
		if (!m_pResultUI)
		{
			// リザルトUIの生成
			m_pResultUI = CResultUI::Create();
		}
		break;

	case 2:
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
		break;
	}
}

//=============================================================================
//
// 描画処理
//
//=============================================================================
void CResult::Draw(void)
{
	// モード描画
	CBaseMode::Draw();

	if (m_pResultUI)
	{
		// リザルトUIの描画
		m_pResultUI->Draw();
	}
}

//=============================================================================
//
// リザルトの生成
//
//=============================================================================
CResult * CResult::Create(void)
{
	// 変数宣言
	CResult *pResult = new CResult;

	// 初期化
	pResult->Init();

	return pResult;
}