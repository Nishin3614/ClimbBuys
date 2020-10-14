// ------------------------------------------
//
// マネージャー処理の説明[manager.h]
// Author : Koki Nishiyama
//
// ------------------------------------------
#ifndef _MANAGER_H_
#define _MANAGER_H_	 // ファイル名を忌Tを決める

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
/* 基本 */
#include "main.h"
#include "Calculation.h"

/* 描画 */
#include "renderer.h"

/* 入力デバイス */
#include "keyboard.h"
#include "joypad.h"
#include "mouse.h"
#include "keyconfig.h"

/* サウンド */
#include "sound.h"

/* デバッグ表示 */
#include "debugproc.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------
#define ONEPLAYER_STAGE		(5)
#define DOUBLEPLAYER_STAGE	(5)
#define DERAY_TIME(X)		(X * 60)
#define MAX_PLAYER			(4)

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------
class CFade;		// 画面遷移
class CBaseMode;	// ベースモードクラス
class CLoadScreen;	// 読み込み画面

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CManager
{
public:
	/* 列挙型 */
	// モード
	typedef enum
	{
		MODE_TITLE = 0,	// タイトル
		MODE_TUTORIAL,	// チュートリアル
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_RANKING,	// ランキング
		MODE_MAX		// 最大数
	} MODE;
	/* 関数 */
	// コンストラクタ
	CManager();
	// デストラクタ
	~CManager();
	// 初期化処理
	//	hWnd		: ウィンドウハンドル
	//	bWindow		: ウィンドウ状態
	//	hInstance	: インスタンスハンドル
	HRESULT  Init(
		HWND hWnd,			// ウィンドウハンドル
		BOOL bWindow,		// ウィンドウ状態
		HINSTANCE hInstance	// インスタンスハンドル
	);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// モード設定
	//	mode	: モード
	static void SetMode(
		MODE const &mode	// モード
	);
	// モード取得
	static MODE GetMode(void) { return m_mode; };
	// レンダラー情報取得
	static CRenderer * GetRenderer(void) { return m_renderer; };
	// キーボード情報取得
	static CKeyboard * GetKeyboard(void) { return m_keyboard; };
	// ジョイパッド情報取得
	static CJoypad * GetJoy(void) { return m_joypad; };
	// マウス情報取得
	static CMouse * GetMouse(void) { return m_mouse; };
	// キー詳細取得
	static CKeyConfig * GetKeyConfig(void) { return m_keyconfig; };
	// サウンド情報取得
	static CSound * GetSound(void) { return m_sound; };
	// 画面遷移情報取得
	static CFade * GetFade(void) { return m_fade; };
	// モード情報取得
	static CBaseMode * GetBaseMode(void) { return m_BaseMode; };
	// 読み込み画面取得
	static CLoadScreen * GetLoadScreen(void) { return m_pLoadScreen; };
protected:

private:
	/* 変数 */
	static CRenderer * m_renderer;			// レンダリング
	static CSound * m_sound;				// サウンド
	static CKeyboard * m_keyboard;			// キーボード
	static CJoypad * m_joypad;				// ジョイパッド
	static CMouse * m_mouse;				// マウス
	static CKeyConfig * m_keyconfig;		// キー詳細
	static CFade * m_fade;					// 画面遷移
	static CBaseMode * m_BaseMode;			// 現在のモード
	static CLoadScreen * m_pLoadScreen;		// 読み込み画面
	static MODE m_mode;						// モード
	static bool	m_bWire;					// ワイヤーモード
};

#endif // !_MANAGER_H_