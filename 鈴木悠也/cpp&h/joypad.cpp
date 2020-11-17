// ------------------------------------------
//
// コントローラー処理 [joypad.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "joypad.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------
#define GAMEPAD_MAX_RANGE (1024)
#define MAX_TRUN (32768.0f)
#define COUNT_WAIT_REPEAT (20)
#define INTERVAL_REPEAT (1)

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------
LPDIRECTINPUTDEVICE8 CJoypad::m_pJoyDevice[MAX_GAMEPAD] = {};	// 入力デバイスへのポインタ
int CJoypad::m_nCntpad = 0;								// カウント

// ------------------------------------------
//
// グローバル変数
//
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CJoypad::CJoypad()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CJoypad::~CJoypad()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
HRESULT CJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイスの初期化
	if (!CInput::Init(hInstance, hWnd) == S_OK)
	{
		return E_FAIL;
	}

	// 時間
	m_nCntWaitRepeat = COUNT_WAIT_REPEAT;
	m_nIntervalRepeat = INTERVAL_REPEAT;

	for (int nCntPad = 0; nCntPad < MAX_GAMEPAD; nCntPad++)
	{
		// 入力デバイス（ゲームパッド）の作成
		if (FAILED(m_pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumCallback,
			NULL,
			DIEDFL_ATTACHEDONLY)) ||
			m_pJoyDevice[m_nCntpad] == NULL)
		{
			return E_FAIL;
		}
		// データフォーマットを設定
		if (FAILED(m_pJoyDevice[m_nCntpad]->SetDataFormat(&c_dfDIJoystick2)))
		{
			return E_FAIL;
		}
		// 協調モードを設定（フォアグラウンド＆非排他モード）
		if (FAILED(m_pJoyDevice[m_nCntpad]->SetCooperativeLevel(
			hWnd,
			(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		/* -------------------パッドの設定------------------- */
		// 変数宣言
		DIPROPRANGE diprg;			// デバイスの設定
									// 構造体の中身をNULLにする
		ZeroMemory(&diprg, sizeof(diprg));

		/* ----------diprgの設定---------- */
		/* diphの中身 */
		diprg.diph.dwSize = sizeof(diprg);				// diprg構造体のメモリを格納
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// diph構造体のメモリを格納
		diprg.diph.dwHow = DIPH_BYOFFSET;				// 何の情報なのじゃ

														/* パッドの最小値・最大値*/
		diprg.lMin = -GAMEPAD_MAX_RANGE;
		diprg.lMax = GAMEPAD_MAX_RANGE;

		/* それぞれのパッド情報を設定 */
		// X軸の設定
		diprg.diph.dwObj = DIJOFS_X;	// パッドのxの情報
		m_pJoyDevice[m_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の設定
		diprg.diph.dwObj = DIJOFS_Y;	// パッドのyの情報
		m_pJoyDevice[m_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の設定
		diprg.diph.dwObj = DIJOFS_Z;	// パッドのzの情報
		m_pJoyDevice[m_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ軸の設定
		diprg.diph.dwObj = DIJOFS_RZ;	// パッドのrzの情報
		m_pJoyDevice[m_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 入力制御開始
		m_pJoyDevice[m_nCntpad]->Acquire();

		// クリア
		memset(&m_aState[m_nCntpad], 0, sizeof(m_aState[m_nCntpad]));
		memset(&m_abState[m_nCntpad], 0, sizeof(m_abState[m_nCntpad]));
		memset(&m_abStateTrigger[m_nCntpad], 0, sizeof(m_abStateTrigger[m_nCntpad]));
		memset(&m_abStateRelease[m_nCntpad], 0, sizeof(m_abStateRelease[m_nCntpad]));
		memset(&m_abStateRepeat[m_nCntpad], 0, sizeof(m_abStateRepeat[m_nCntpad]));
		//// 入力デバイス(キーボード)の開放
		if (m_pJoyDevice[nCntPad] != NULL)
		{
			m_aState[nCntPad].lX = 0;
			m_aState[nCntPad].lY = 0;
		}
	}
	// 初期化完了
	return S_OK;
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CJoypad::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		// 入力デバイス(キーボード)の開放
		if (m_pJoyDevice[nCnt] != NULL)
		{
			m_pJoyDevice[nCnt]->Unacquire();	// キーボードへのアクセス権を開放
			m_pJoyDevice[nCnt]->Release();
			m_pJoyDevice[nCnt] = NULL;
		}
	}

	// コントローラーデバイスの初期化
	CInput::Uninit();
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CJoypad::Update(void)
{
	bool aJoyState[KEY_MAX];	// キーボードの入力情報
	bool abLeftStick[4];		// 左スティックの情報
	int nCntPad;
	int nCntJoy;

	for (nCntPad = 0; nCntPad < MAX_GAMEPAD; nCntPad++)
	{
		if (m_pJoyDevice[nCntPad] == NULL)
		{
			return;
		}

		// スティックの情報保存
		for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
		{
			abLeftStick[nCntDirection] = m_abStick[nCntPad][nCntDirection];
		}
		// キー情報保存
		for (nCntJoy = 0; nCntJoy < KEY_MAX; nCntJoy++)
		{
			// キープレス情報保存
			aJoyState[nCntJoy] = m_abState[nCntPad][nCntJoy];
		}
		// デバイスからデータを取得
		if (SUCCEEDED(m_pJoyDevice[nCntPad]->GetDeviceState(sizeof(m_aState[nCntPad]), &m_aState[nCntPad])))
		{
			// キー情報設定
			SetState(nCntPad);
			// スティック情報の設定
			// スティックのトリガー・リリース・リピート情報の計算
			m_abStick[nCntPad][DIRECTION_LEFT] = m_aState[nCntPad].lX < 0;	// 左の情報
			m_abStick[nCntPad][DIRECTION_RIGHT] = m_aState[nCntPad].lX > 0;	// 右の情報
			m_abStick[nCntPad][DIRECTION_UP] = -m_aState[nCntPad].lY > 0;	// 上の情報
			m_abStick[nCntPad][DIRECTION_DOWN] = -m_aState[nCntPad].lY < 0;	// 下の情報

			// スティック情報計算
			for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
			{
				// スティックトリガー情報の計算
				m_abStickTrigger[nCntPad][nCntDirection] =
					(abLeftStick[nCntDirection] ^ m_abStick[nCntPad][nCntDirection]) &
					m_abStick[nCntPad][nCntDirection];
			}
			for (nCntJoy = 0; nCntJoy < KEY_MAX; nCntJoy++)
			{
				if (!m_sbState)
				{
					CInput::m_sbState = m_abState[nCntPad][nCntJoy];
				}
				/*
				pValueH = m_aState[nJoy].lX;
				pValueV = -m_aState[nJoy].lY;
				*/

				// トリガー・リリース・リピート情報の計算
				m_abStateTrigger[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ m_abState[nCntPad][nCntJoy]) & m_abState[nCntPad][nCntJoy];
				m_abStateRelease[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ m_abState[nCntPad][nCntJoy]) & !m_abState[nCntPad][nCntJoy];
				m_abStateRepeat[nCntPad][nCntJoy] = m_abStateTrigger[nCntPad][nCntJoy];

				// ONの状態
				if (m_abStateRepeat[nCntPad][nCntJoy])
				{
					m_aStateRepeatCnt[nCntPad][nCntJoy]++;
					if (m_aStateRepeatCnt[nCntPad][nCntJoy] < m_nCntWaitRepeat)
					{
						if (m_abStateRepeat[nCntPad][nCntJoy] == 1)
						{
							m_abStateRepeat[nCntPad][nCntJoy] = m_abState[nCntPad][nCntJoy];
						}

						else
						{
							m_abStateRepeat[nCntPad][nCntJoy] = 0;
						}
					}

					else
					{
						if ((m_abStateRepeat[nCntPad][nCntJoy] - m_nCntWaitRepeat) % m_nIntervalRepeat == 0)
						{
							m_abStateRepeat[nCntPad][nCntJoy] = m_abState[nCntPad][nCntJoy];
						}

						else
						{
							m_abStateRepeat[nCntPad][nCntJoy] = 0;
						}
					}
				}

				else
				{
					m_aStateRepeatCnt[nCntPad][nCntJoy] = 0;
					m_abStateRepeat[nCntPad][nCntJoy] = 0;
				}
			}
		}
		else
		{
			m_pJoyDevice[nCntPad]->Acquire();
		}
	}
}

// ------------------------------------------
// ジョイパッド能力取得
// ------------------------------------------
BOOL CALLBACK CJoypad::EnumCallback(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext)
{
	// 変数宣言
	static GUID pad_discrimination;
	DIDEVCAPS diDevCaps;

	if (FAILED(m_pInput->CreateDevice(pdidInstance->guidInstance, &m_pJoyDevice[m_nCntpad], NULL)))
	{
		return DIENUM_CONTINUE;
	}

	// デバイスの能力取得
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(m_pJoyDevice[m_nCntpad]->GetCapabilities(&diDevCaps)))
	{
		m_pJoyDevice[m_nCntpad]->Release();
		m_pJoyDevice[m_nCntpad] = NULL;
		return DIENUM_CONTINUE;
	}

	// デバイスの識別子を保存
	pad_discrimination = pdidInstance->guidInstance;
	return DIENUM_STOP;
}

// ------------------------------------------
// ボタンの設定
// ------------------------------------------
void CJoypad::SetState(int const &nJoy)
{
	// 変数宣言
	int nKey;

	if (m_aState[nJoy].rgdwPOV[0] >= 225 * 100 && m_aState[nJoy].rgdwPOV[0] <= 315 * 100)
	{
		// 十字キー[左]が押されている
		m_abState[nJoy][KEY_LEFT] = true;
	}

	else
	{
		// 十字キー[左]が押されている
		m_abState[nJoy][KEY_LEFT] = false;
	}

	if (m_aState[nJoy].rgdwPOV[0] >= 45 * 100 && m_aState[nJoy].rgdwPOV[0] <= 135 * 100)
	{
		// 十字キー[右]が押されている
		m_abState[nJoy][KEY_RIGHT] = true;
	}

	else
	{
		// 十字キー[右]が押されている
		m_abState[nJoy][KEY_RIGHT] = false;
	}

	if ((m_aState[nJoy].rgdwPOV[0] >= 315 * 100 && m_aState[nJoy].rgdwPOV[0] <= 360 * 100) ||
		(m_aState[nJoy].rgdwPOV[0] >= 0 * 100 && m_aState[nJoy].rgdwPOV[0] <= 45 * 100))
	{
		// 十字キー[上]が押されている
		m_abState[nJoy][KEY_UP] = true;
	}

	else
	{
		// 十字キー[上]が押されている
		m_abState[nJoy][KEY_UP] = false;
	}

	if (m_aState[nJoy].rgdwPOV[0] >= 135 * 100 && m_aState[nJoy].rgdwPOV[0] <= 225 * 100)
	{
		// 十字キー[下]が押されている
		m_abState[nJoy][KEY_DOWN] = true;
	}

	else
	{
		// 十字キー[下]が押されている
		m_abState[nJoy][KEY_DOWN] = false;
	}

	for (nKey = KEY_X; nKey <= KEY_START; nKey++)
	{
		if (m_aState[nJoy].rgbButtons[nKey])
		{
			// ボタンが押されている
			m_abState[nJoy][nKey] = true;
		}

		else
		{
			m_abState[nJoy][nKey] = false;
		}
	}
}

// ------------------------------------------
// 押した処理
// ------------------------------------------
bool CJoypad::GetPress(
	int const &nJoy,	// ジョイパッドの番号
	KEY const &key		// キー情報
)
{
	return m_abState[nJoy][key];
}

// ------------------------------------------
// 押されたときの処理
// ------------------------------------------
bool CJoypad::GetTrigger(
	int const &nJoy,	// ジョイパッドの番号
	KEY const &key		// キー情報
)
{
	return m_abStateTrigger[nJoy][key];
}

// ------------------------------------------
// リリース処理
// ------------------------------------------
bool CJoypad::GetRelease(
	int const &nJoy,	// ジョイパッドの番号
	KEY const &key		// キー情報
)
{
	return m_abStateRelease[nJoy][key];
}

// ------------------------------------------
// 往復処理
// ------------------------------------------
bool CJoypad::GetRepeat(
	int const &nJoy,	// ジョイパッドの番号
	KEY const &key		// キー情報
)
{
	return m_abStateRepeat[nJoy][key];
}

// ------------------------------------------
// Lボタン押された処理
// ------------------------------------------
int CJoypad::GetTriggerLeft(
	int const &nJoy,	// ジョイパッドの番号
	KEY const &key		// キー情報
)
{
	return m_aState[nJoy].rgbButtons[KEY_LEFT_2];
}

// ------------------------------------------
// Rボタン押された処理
// ------------------------------------------
int CJoypad::GetTriggerRight(
	int const &nJoy,	// ジョイパッドの番号
	KEY const &key		// キー情報
)
{
	return m_aState[nJoy].rgbButtons[KEY_RIGHT_2];
}

// ------------------------------------------
// 左スティック押された処理
// ------------------------------------------
void CJoypad::GetStickLeft(
	int const &nJoy,	// ジョイパッド番号
	int & pValueH,		// 水平方向の力
	int & pValueV		// 垂直方向の力
)
{
	pValueH = m_aState[nJoy].lX;
	pValueV = -m_aState[nJoy].lY;
}

// ------------------------------------------
// 右スティック押された処理
// ------------------------------------------
void CJoypad::GetStickRight(
	int const &nJoy,	// ジョイパッド番号
	int & pValueH,		// 水平方向の力
	int & pValueV		// 垂直方向の力
)
{
	pValueH = m_aState[nJoy].lZ;
	pValueV = -m_aState[nJoy].lRz;
}
