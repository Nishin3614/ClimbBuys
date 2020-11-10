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

	// タイトル遷移
	if (CCalculation::PressAnyButton())
	{
		// リザルトUIの生成
		CResultUI::Create();

		//// フェード状態が何も起こっていない状態なら
		//if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		//{
		//	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		//}
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
}

//=============================================================================
//
// タイトルの生成
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