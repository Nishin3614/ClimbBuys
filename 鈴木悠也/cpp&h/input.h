// ----------------------------------------------------------------------------------------------------
//
// インプット処理の説明[input.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------
//
// マクロ関数
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------
class CInput
{
public:
	/* 列挙型 */
	typedef enum
	{
		DEVICETYPE_KEYBOARD = 0,
		DEVICETYPE_JOYPAD
	} DEVICETYPE;
	// ボタン配置
	typedef enum
	{
		BUTTON_NONE = 0,
		BUTTON_DECIDE,
		BUTTON_CHANCEL,
		BUTTON_ATTACK,
		BUTTON_TRICKER,
		BUTTON_MAX
	} BUTTON;

	/* 関数 */
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	static void ReleaseAll(void);										// 親子リリース
	static void InitState(void) { m_sbState = false; };					// ボタン押されたかどうかの初期化
	static bool GetState(void) { return m_sbState; };					// ボタン押されたかどうかの取得
	static void SetState(bool const &bState) { m_sbState = bState; };	// ボタン押されたかどうかの取得
protected:
	static LPDIRECTINPUT8 m_pInput;	// 入力デバイス
	static bool m_sbState;			// ボタンが押されたかどうか
private:
};

#endif // !_INPUT_H_