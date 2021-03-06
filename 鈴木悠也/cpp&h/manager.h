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
#include "renderer.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------
#define DERAY_TIME(X)		(X * 60)									// 秒をフレームに変換
#define FIELD_SIZE			(D3DXVECTOR3(1000.0, 0.0f, 1000.0f))		// フィールドサイズ

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------
class CRenderer;	// レンダラークラス
class CFade;		// 画面遷移
class CBaseMode;	// ベースモードクラス
class CKeyboard;	// キーボードクラス
class CXInputPad;	// ゲームパッドクラス
class CMouse;		// マウスクラス
class CKeyConfig;	// キー詳細
class CSound;		// サウンドクラス

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
		MODE_OPENING = 0,// オープニング
		MODE_TITLE,		// タイトル
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
	// ゲームパッド情報取得
	static CXInputPad * GetPad(PLAYER_TAG Tag) { return m_pPad[(int)Tag]; };
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
protected:

private:
	/* 変数 */
	static CRenderer * m_renderer;							// レンダリング
	static CSound * m_sound;								// サウンド
	static CKeyboard * m_keyboard;							// キーボード
	static CXInputPad *m_pPad[(int)PLAYER_TAG::PLAYER_MAX];	// ゲームパッド( XInput )
	static CMouse * m_mouse;								// マウス
	static CKeyConfig * m_keyconfig;						// キー詳細
	static CFade * m_fade;									// 画面遷移
	static CBaseMode * m_BaseMode;							// 現在のモード
	static MODE m_mode;										// モード
	static bool	m_bWire;									// ワイヤーモード
};

#endif // !_MANAGER_H_