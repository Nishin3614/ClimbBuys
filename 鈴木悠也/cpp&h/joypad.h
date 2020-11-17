// ------------------------------------------
//
// コントローラーヘッダー処理 [joypad.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

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
#define MAX_GAMEPAD (1)

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
class CJoypad : public CInput
{
public:
	/* 列挙型 */
	// キー情報
	typedef enum
	{
		KEY_X = 0,
		KEY_Y,
		KEY_A,
		KEY_B,
		KEY_LEFT_1,
		KEY_RIGHT_1,
		KEY_LEFT_2,
		KEY_RIGHT_2,
		KEY_LEFT_STICK,
		KEY_RIGHT_STICK,
		KEY_BACK,
		KEY_START,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_MAX,
	} KEY;
	// 方向情報
	typedef enum
	{
		DIRECTION_LEFT = 0,
		DIRECTION_RIGHT,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_MAX,
	} DIRECTION;

	/* 関数 */
	CJoypad();
	~CJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	// 押されているときの情報取得
	bool GetPress(
		int const &nJoy,	// ジョイパッドの番号
		KEY const &key		// キー情報
	);
	// 押したときの情報取得
	bool GetTrigger(
		int const &nJoy,	// ジョイパッドの番号
		KEY const &key		// キー情報
	);
	// 話したときの情報取得
	bool GetRelease(
		int const &nJoy,	// ジョイパッドの番号
		KEY const &key		// キー情報
	);
	bool GetRepeat(
		int const &nJoy,	// ジョイパッドの番号
		KEY const &key		// キー情報
	);
	// Lボタンが押されている情報取得
	int GetTriggerLeft(
		int const &nJoy,	// ジョイパッドの番号
		KEY const &key		// キー情報
	);
	// Rボタンが押されいている情報取得
	int GetTriggerRight(
		int const &nJoy,	// ジョイパッドの番号
		KEY const &key		// キー情報
	);
	// 左スティックの情報取得
	void GetStickLeft(
		int const &nJoy,	// ジョイパッド番号
		int & pValueH,		// 水平方向の力
		int & pValueV		// 垂直方向の力
	);
	// 右スティックの情報取得
	void GetStickRight(
		int const &nJoy,	// ジョイパッド番号
		int & pValueH,		// 水平方向の力
		int & pValueV		// 垂直方向の力
	);
	// 左スティックのbool情報
	bool const &GetBoolStickLeft(
		int const &nJoy,			// ジョイパッド番号
		DIRECTION const &direction	// 方向情報
	)
	{
		return m_abStickTrigger[nJoy][direction];
	};
protected:
private:
	/* 変数宣言 */
	static LPDIRECTINPUTDEVICE8 m_pJoyDevice[MAX_GAMEPAD];	// 入力デバイスへのポインタ
	DIJOYSTATE2 m_aState[MAX_GAMEPAD];						// ゲームパッド状態を受け取るワーク
	bool m_abStick[MAX_GAMEPAD][DIRECTION_MAX];							// スティック状態
	bool m_abStickTrigger[MAX_GAMEPAD][DIRECTION_MAX];					// 過去のスティック状態
	int m_nCntWaitRepeat;									// カウントマックス
	int m_nIntervalRepeat;									// 往復の間
	static int m_nCntpad;									// カウント
	int m_aStateRepeatCnt[MAX_GAMEPAD][KEY_MAX];			// リピート回数
	bool m_abState[MAX_GAMEPAD][KEY_MAX];					// ゲームパッド状態の保存場所
	bool m_abStateTrigger[MAX_GAMEPAD][KEY_MAX];			// ゲームパッド状態のオン・オフ
	bool m_abStateRelease[MAX_GAMEPAD][KEY_MAX];			// ゲームパッド状態の返し
	bool m_abStateRepeat[MAX_GAMEPAD][KEY_MAX];				// ゲームパッド状態を繰り返し
	/* 関数 */
	// コールバック関数
	// 能力取得
	static BOOL CALLBACK EnumCallback(
		const DIDEVICEINSTANCE *pdidInstance,
		VOID *pContext
	);
	// ゲームパッドの状態設定
	void SetState(int const & nJoy);
};
#endif
