// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// タイトル処理 [basemode.cpp]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "basemode.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "keyboard.h"
#include "fade.h"
#include "renderer.h"
#include "manager.h"
#include "collision.h"

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
CBaseMode::CBaseMode()
{
	m_bSceneStop = false;	// シーンを止めるフラグ
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseMode::~CBaseMode()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseMode::Init(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseMode::Uninit(void)
{
	CScene::ReleaseAll();
	// シーンストップ解除
	CScene::UpdateStop(false);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseMode::Update(void)
{
	//CCollision::CollisionDetection();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseMode::Draw(void)
{
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseMode::Debug(void)
{
	//キーボード取得
	CKeyboard *key = CManager::GetKeyboard();

	// デバッグコマンド
	CDebugproc::Print(NEWLINE);
	CDebugproc::Print("--------- Debug Command ----------\n");
	CDebugproc::Print("[F3] : 一時停止 [ %d ] : [ OFF : 0 / ON : 1 ]\n", m_bSceneStop);
	CDebugproc::Print("[F4] : 1Fだけ更新\n");
	CDebugproc::Print("[F5] : 当たり判定の可視状態\n");
	CDebugproc::Print(NEWLINE);
	CDebugproc::Print("[Ctrl] + テンキー [0] : 現在のModeに遷移\n");
	CDebugproc::Print("[Ctrl] + テンキー [1] : Titleに遷移\n");
	CDebugproc::Print("[Ctrl] + テンキー [2] : Tutorialに遷移\n");
	CDebugproc::Print("[Ctrl] + テンキー [3] : Gameに遷移\n");
	CDebugproc::Print("[Ctrl] + テンキー [4] : Resultに遷移\n");
	CDebugproc::Print(NEWLINE);

	// 現在のモード表記
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		CDebugproc::Print("現在のモード : [ Title ]\n");
		break;

	case CManager::MODE_TUTORIAL:
		CDebugproc::Print("現在のモード : [ Tutorial ]\n");
		break;

	case CManager::MODE_GAME:
		CDebugproc::Print("現在のモード : [ Game ]\n");
		break;

	case CManager::MODE_RESULT:
		CDebugproc::Print("現在のモード : [ Result ]\n");
		break;
	}

	// 一時停止
	if (key->GetKeyboardTrigger(DIK_F3))
	{
		m_bSceneStop ^= 1;
		CScene::UpdateStop(m_bSceneStop);
	}
	// 1Fだけ更新する
	if (key->GetKeyboardTrigger(DIK_F4))
	{
		CScene::Set1FUpdate();
	}
	//当たり判定の可視状態
	if (key->GetKeyboardTrigger(DIK_F5))
	{
		CCollision::SetDispCollision(!CCollision::GetDispCollision());
	}

	// Ctrl押しながら
	if (key->GetKeyboardPress(DIK_LCONTROL))
	{
		// フェード状態が何も起こっていない状態なら
		//if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			// 現在のモード再始動
			if (key->GetKeyboardTrigger(DIK_NUMPAD0))
			{
				CManager::GetFade()->SetFade(CManager::GetMode());
			}
			//タイトル
			if (key->GetKeyboardTrigger(DIK_NUMPAD1))
			{
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			//チュートリアル
			if (key->GetKeyboardTrigger(DIK_NUMPAD2))
			{
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			}
			// ゲーム
			if (key->GetKeyboardTrigger(DIK_NUMPAD3))
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
			// リザルト
			if (key->GetKeyboardTrigger(DIK_NUMPAD4))
			{
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			}
		}
	}
}
#endif // _DEBUG