//=============================================================================
//
// マウス処理 [mouse.h]
// Author : koki nishiyama
//
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// マクロ定義
//=============================================================================
#define	NUM_BUTTON_MAX (8)		// ボタン(マウス)の最大数

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CMouse : public CInput
{
public:
	/* 関数 */
	CMouse();
	~CMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void Uninit(void);
	bool GetPress(int nButton);		// 押されている情報取得
	bool GetTrigger(int nButton);	// 押した情報取得
	bool GetDouble(int nButton);	// ダブルクリック情報取得
	LONG GetX(void);				// X座標取得
	LONG GetY(void);				// Y座標取得

protected:
private:
	DIMOUSESTATE2			m_mouse;
	LPDIRECTINPUTDEVICE8	m_pDevice;					// 入力デバイス(マウス)へのポインタ
	BYTE					m_aPress[NUM_BUTTON_MAX];	// マウスの入力情報ワーク
	BYTE					m_aTrigger[NUM_BUTTON_MAX];	// マウスのトリガー
	BYTE					m_aDouble[NUM_BUTTON_MAX];	// マウスのオンオフ
	HWND					m_hWnd;						// ウィンドウハンドル
};

#endif
