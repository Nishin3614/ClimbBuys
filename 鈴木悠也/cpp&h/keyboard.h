// ------------------------------------------
//
// 入力ヘッダー処理 [keyboard.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"
#include "input.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------
#define	NUM_KEY_MAX			(256)	// キーの最大数

#ifdef _DEBUG
#define TESTPLAY_NUMBER1 (DIK_1)	// チュートリアルに強制遷移
#define TESTPLAY_NUMBER2 (DIK_2)
#define TESTPLAY_NUMBER3 (DIK_3)
#define TESTPLAY_NUMBER4 (DIK_4)
#define TESTPLAY_NUMBER5 (DIK_5)
#define TESTPLAY_NUMBER6 (DIK_6)
#define TEXTPLAY_NUMBER7 (DIK_7)
#define TEXTPLAY_NUMBER8 (DIK_8)
#define TEXTPLAY_NUMBER9 (DIK_9)
#define TEXTPLAY_NUMBER0 (DIK_0)

#endif // _DEBUG

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CKeyboard : public CInput
{
public:
	/* 関数 */
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);	// 押されている情報取得
	bool GetKeyboardTrigger(int nKey);	// 押した情報取得

protected:
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;	// 入力デバイスへのポインタ
	BYTE m_aState[NUM_KEY_MAX];		// 入力している情報
	BYTE m_aTrigger[NUM_KEY_MAX];	// 入力時情報
};
#endif
