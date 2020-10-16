// ------------------------------------------
//
// チュートリアル処理 [tutorial.cpp]
// Author : Nishiyama Koki
//
// ------------------------------------------
#include "tutorial.h"
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
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "manager.h"
#include "keyboard.h"

/* 描画 */
#include "fade.h"



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
	// ゲーム遷移
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// フェード状態が何も起こっていない状態なら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::SetMode(CManager::MODE_GAME);
		}
	}


#endif // _DEBUG

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
