// ----------------------------------------------------------------------------------------------------
//
// マネージャー処理の説明[manager.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------
/* My */
#include "manager.h"

/* 入力デバイス */
#include "keyboard.h"
#include "XInputPad.h"
#include "mouse.h"
#include "keyconfig.h"

/* サウンド */
#include "sound.h"

/* デバッグ表示 */
#include "debugproc.h"

/* 描画 */
#include "scene.h"
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "scene_two.h"
#include "loadscreen.h"

// ----------------------------------------------------------------------------------------------------
//
// マクロ関数
//
// ----------------------------------------------------------------------------------------------------
#define STARTMODE (MODE_TITLE)


// ----------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------
CRenderer		*CManager::m_renderer								= NULL;					// レンダラー
CKeyboard		* CManager::m_keyboard								= NULL;					// キーボード
CXInputPad		* CManager::m_pPad[(int)PLAYER_TAG::PLAYER_MAX]		= {};					// ゲームパッド
CMouse			* CManager::m_mouse									= NULL;					// マウス
CKeyConfig		* CManager::m_keyconfig								= NULL;					// キー詳細
CSound			* CManager::m_sound									= NULL;					// サウンド
CFade			* CManager::m_fade									= NULL;					// フェード
CBaseMode		* CManager::m_BaseMode								= NULL;					// ベースモード
CManager::MODE	CManager::m_mode									= CManager::MODE_TITLE;	// モード
bool			CManager::m_bWire									= false;				// ワイヤー
CLoadScreen		* CManager::m_pLoadScreen							= NULL;					// 読み込み画面

// ----------------------------------------------------------------------------------------------------
// コンストラクタ
// ----------------------------------------------------------------------------------------------------
CManager::CManager()
{
}

// ----------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------
CManager::~CManager()
{
}

// ----------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------
HRESULT CManager::Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance)
{
	/* メモリの生成 */
	/* 入力デバイス */
	// キーボードの生成
	m_keyboard = new CKeyboard;
	// ジョイパッドの生成
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_pPad[nCnt] = new CXInputPad;
	}
	// マウスの生成
	m_mouse = new CMouse;
	// キー詳細の生成
	m_keyconfig = new CKeyConfig;
	/* ゲームに欠かせないもの */
	// サウンドの生成
	m_sound = new CSound;
	// レンダリングの生成
	m_renderer = new CRenderer;
	// モードの設定
	m_mode = STARTMODE;
	/* 初期化 */
	// キーボード
	if (!m_keyboard->Init(hInstance, hWnd) == S_OK)
	{
		m_keyboard->Uninit();
		delete m_keyboard;
		m_keyboard = NULL;
	}
	// ジョイパッド
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (!m_pPad[nCnt]->Init(hInstance, hWnd, (PLAYER_TAG)nCnt) == S_OK)
		{
			//m_pPad[nCnt]->Uninit();
			//delete m_pPad[nCnt];
			//m_pPad[nCnt] = nullptr;
		}
	}
	// マウス
	if (!m_mouse->Init(hInstance, hWnd) == S_OK)
	{
		m_mouse->Uninit();
		delete m_mouse;
		m_mouse = NULL;
	}
	// キー詳細
	m_keyconfig->Init();
	// サウンド
	if (!m_sound->Init(hWnd) == S_OK)
	{
		m_sound->Uninit();
		delete m_sound;
		m_sound = NULL;
		return E_FAIL;
	}
	// レンダリング
	if (!m_renderer->Init(hWnd, bWindow) == S_OK)
	{
		m_renderer->Uninit();
		delete m_renderer;
		m_renderer = NULL;
		return E_FAIL;
	}

	// カメラなどの設定
	// どのプレイヤー番号を追尾するか
	// ゲームのカメラのアップデートでそれを指定するかも

	// 読み込み画面の生成
	m_pLoadScreen = CLoadScreen::Create();

	// 画面遷移
	m_fade = CFade::Create(STARTMODE);

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	/* メモリの破棄 */
	// 入力デバイス
	CInput::ReleaseAll();
	// キーボード
	if (m_keyboard != NULL)
	{
		m_keyboard->Uninit();
		delete m_keyboard;
		m_keyboard = NULL;
	}
	// ジョイパッド
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (m_pPad[nCnt])
		{
			m_pPad[nCnt]->Uninit();
			delete m_pPad[nCnt];
			m_pPad[nCnt] = NULL;
		}
	}

	// マウス
	if (m_mouse != NULL)
	{
		m_mouse->Uninit();
		delete m_mouse;
		m_mouse = NULL;
	}
	// キー詳細
	if (m_keyconfig != NULL)
	{
		m_keyconfig->Uninit();
		delete m_keyconfig;
		m_keyconfig = NULL;
	}
	// サウンド
	if (m_sound != NULL)
	{
		m_sound->Uninit();
		delete m_sound;
		m_sound = NULL;
	}
	// レンダリング
	if (m_renderer != NULL)
	{
		m_renderer->Uninit();
		delete m_renderer;
		m_renderer = NULL;
	}
	// 画面遷移
	if (m_fade != NULL)
	{
		m_fade->Uninit();
		delete m_fade;
		m_fade = NULL;
	}
	// タイトル
	if (m_BaseMode != NULL)
	{
		m_BaseMode->Uninit();
		delete m_BaseMode;
		m_BaseMode = NULL;
	}
	// 読み込み画面
	if (m_pLoadScreen != NULL)
	{
		m_pLoadScreen->Uninit();
		delete m_pLoadScreen;
		m_pLoadScreen = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------
void CManager::Update(void)
{
	// 入力デバイスのボタンが押されているか同課の初期化
	CInput::InitState();
	// キーボード
	if (m_keyboard != NULL)
	{
		m_keyboard->Update();
	}
	// ジョイパッド
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (m_pPad[nCnt])
		{
			m_pPad[nCnt]->Update();
		}
	}
	// マウス
	if (m_mouse != NULL)
	{
		m_mouse->Update();
	}
	// レンダリング
	m_renderer->Update();
	// ベースモードのNULLチェック
	if (m_BaseMode != NULL)
	{
		// ベースモードの更新処理
		m_BaseMode->Update();
#ifdef _DEBUG
		// ベースモードのデバッグ処理
		m_BaseMode->Debug();
#endif // _DEBUG
	}
	// 前回のスティック情報
	CCalculation::SaveLastStickInfo();

	// 画面遷移
	m_fade->Update();
}

// ----------------------------------------------------------------------------------------------------
//
// 描画関数
//
// ----------------------------------------------------------------------------------------------------
void CManager::Draw(void)
{
	// レンダリング
	m_renderer->Draw();
}

// ----------------------------------------------------------------------------------------------------
// モード設定関数
// ----------------------------------------------------------------------------------------------------
void CManager::SetMode(MODE const &mode)
{
	// ベースモードのNULLチェック
	if (m_BaseMode != NULL)
	{
		// ベースモードの終了処理
		m_BaseMode->Uninit();
	}
	// 前のモード終了
	switch (m_mode)
	{
		// タイトル
	case MODE_TITLE:
		break;
		// チュートリアル
	case MODE_TUTORIAL:
		break;
		// ゲーム
	case MODE_GAME:
		break;
		// リザルト
	case MODE_RESULT:
		break;
		// ランキング
	case MODE_RANKING:
		break;
	default:
		break;
	}

	m_mode = mode;	// 現在のモードを代入

	// 現在のモード初期化
	switch (mode)
	{
		// タイトル
	case MODE_TITLE:
		// タイトル生成
		m_BaseMode = CTitle::Create();
		break;
		// チュートリアル
	case MODE_TUTORIAL:
		// チュートリアル生成
		m_BaseMode = CTutorial::Create();
		// チュートリアルBGM
		break;
		// ゲーム
	case MODE_GAME:
		// ゲーム生成
		m_BaseMode = CGame::Create();
		// ゲームBGM
		break;
		// リザルト
	case MODE_RESULT:
		// リザルト生成
		m_BaseMode = CResult::Create();
		// リザルトBGM
		break;
		// ランキング
	case MODE_RANKING:
		// ゲーム生成
		//m_BaseMode = CRanking::Create();
		// ゲームBGM
		break;
	default:
		break;
	}
}