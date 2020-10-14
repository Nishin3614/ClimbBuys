// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// タイトル処理 [title.cpp]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "title.h"
/* 描画 */
#include "fade.h"
#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "manager.h"
#include "ui.h"
#include "2Deffect.h"
#include "3Dparticle.h"
#include "title_select.h"

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
	// 2Dエフェクトの生成
	C2DEffect::Create();
	// 3Dパーティクル生成
	C3DParticle::Create(
		C3DParticle::PARTICLE_ID_UNKNOWN,
		//D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DVECTOR3_ZERO,
		true);
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