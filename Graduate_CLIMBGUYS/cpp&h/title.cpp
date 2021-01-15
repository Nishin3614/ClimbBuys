// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// タイトル処理 [title.cpp]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "keyboard.h"
#include "sound.h"
#include "manager.h"
#include "camera.h"

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
#include "damagefloor.h"
//#include "3Deffect.h"
//#include "2Deffect.h"

#include "stagingObj_info.h"
#include "stagingObj.h"
#include "light.h"

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
	m_nTime = 0;
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

	// 背景生成
	CBg::Create(CTexture_manager::TYPE_BG_TITLE);

	// 設定
	CStagingObj_Info::SetCondition(30, 6000, 1, CStagingObj::STAGING_OBJTYPE::NORMAL);
	CStagingObj_Info::SetCondition(30, 6000, 1, CStagingObj::STAGING_OBJTYPE::LEVITATION);
	CStagingObj_Info::SetCondition(30, 6000, 1, CStagingObj::STAGING_OBJTYPE::ASCENT);
	CStagingObj_Info::SetCondition(30, 6000, 1, CStagingObj::STAGING_OBJTYPE::DESCENT);

	// タイトルオブジェクトを生成
	CStagingObj_Info::Create_TitleObj();

	// ライトの設定
	CManager::GetRenderer()->GetLight()->ModeSetLight();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
{
	//CStagingBlock::SetEnd(false);
	// モード終了
	CBaseMode::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTitle::Update(void)
{
	// 一定時間毎にエフェクトを出現
	if (m_nTime++ % 10 == 0)
	{
		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::PARTICLE_ID_AIRLINE,
			SCREEN_CENTER_POS,
			true
		);
	}

	// 演出ブロックの生成
	//CStagingObj_Info::Create_StagingObj(D3DXVECTOR3(1280 * 0.5f, 0.0f, 0.0f), D3DXVECTOR3(1280 * 0.5f, 0.0f, 0.0f), CScene_X::TYPE_PLAYER00_ALL, CStagingObj::STAGING_OBJTYPE::ASCENT,true);
	CStagingObj_Info::Create_StagingObj_RangeSelect(D3DXVECTOR3(1280 * 0.5f, 0.0f, 0.0f), D3DXVECTOR3(1280 * 0.5f, 0.0f, 0.0f), CScene_X::TYPE_PLAYER00_ALL, CScene_X::TYPE_PLAYER03_ALL, CStagingObj::STAGING_OBJTYPE::ASCENT, true);


	// チュートリアルに画面遷移
	if (CCalculation::PressAnyButton())
	{
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			// 決定音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_DETERMINATION);
			CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
		}
	}

	// モード更新
	CBaseMode::Update();
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