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
#include "connectblock.h"

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

	// チュートリアルUIの生成
	CUi::LoadCreate(CUi::UITYPE_TUTORIAL);

	// 3Dエフェクトの生成
	C3DEffect::Create();

	// 試験的背景の生成
	CBg::Create();

	// 結合されたフィールドブロックの生成
	CConnect_fieldblock::Create(CGame::STAGE_1);
	// 結合されたブロックの更新ブロック生成
	CConnectblock::TestCreate();

	//// 床の生成
	//CFloor::Create(D3DVECTOR3_ZERO,D3DXVECTOR3(1000.0f,0.0f, 1000.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DVECTOR3_ZERO,2,2,0);
	////// 足場の生成
	//CStand::CreateStand_Tutorial();

	// プレイヤー
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// プレイヤーの生成	試験的
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-100.0, 300.0f, -100.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(100.0f, 300.0f, 0.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(0.0, 300.0f, 100.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(100.0f, 300.0f, 100.0f));
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
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
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