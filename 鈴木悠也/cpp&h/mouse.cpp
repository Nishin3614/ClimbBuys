//=============================================================================
//
// 入力処理 [mouse.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "mouse.h"
#include "debugproc.h"

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CMouse::CMouse()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CMouse::~CMouse()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 変数宣言
	DIPROPDWORD diprop;

	// 入力デバイスの初期化
	CInput::Init(hInstance, hWnd);

	// 入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// マウス用のデータフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}
	// 協調設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	/* ----------dipropの設定---------- */
	diprop.diph.dwSize = sizeof(diprop);			// diprop構造体のメモリサイズを格納
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);	// diph構造体のメモリサイズを格納
	diprop.diph.dwObj = 0;							// マウス情報の初期化
	diprop.diph.dwHow = DIPH_DEVICE;
	//diprop.dwData = DIPROPAXISMODE_REL;			// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	// マウスデバイスの設定更新
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	// 入力制御開始    
	m_pDevice->Acquire();

	// Window情報取得
	m_hWnd = hWnd;
	return S_OK;
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouse::Update(void)
{
	// 変数宣言
	POINT point;

	// カーソルの位置を取得
	GetCursorPos(&point);
	// スクリーン上の位置情報に変換
	ScreenToClient(m_hWnd, &point);

	for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
	{
		// 値の初期化    
		m_mouse.rgbButtons[nCntButton] = 0;
	}
	// マウスの情報を取得出来たら
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouse)))
	{
		/* マウスの位置情報格納 */
		m_mouse.lX = point.x;	// x座標
		m_mouse.lY = point.y;	// y座標
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// 排他的論理和かどうか判断
			m_aTrigger[nCntButton] = (m_aPress[nCntButton] ^ m_mouse.rgbButtons[nCntButton]) & m_mouse.rgbButtons[nCntButton];
			m_aDouble[nCntButton] = (m_aPress[nCntButton] ^ m_mouse.rgbButtons[nCntButton]);
			m_aPress[nCntButton] = m_mouse.rgbButtons[nCntButton];	// キープレス情報保存
		}
	}
	// 出来なかったら
	else
	{
		m_pDevice->Acquire(); // １発目や２発目にエラーが出るが無視してよい。 
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouse::Uninit(void)
{
	// 入力デバイスの終了
	CInput::Uninit();
	// マウスデバイスの終了
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// マウスへのアクセス権を開放
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//=============================================================================
// 押されている情報取得処理
//=============================================================================
bool CMouse::GetPress(int nButton)
{
	return (m_mouse.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// 押した情報取得処理
//=============================================================================
bool CMouse::GetTrigger(int nButton)
{
	return (m_aTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// ダブルクリック時取得処理
//=============================================================================
bool CMouse::GetDouble(int nButton)
{
	return (m_aDouble[nButton] & 0x80) ? true : false;
}

//=============================================================================
// X座標取得処理
//=============================================================================
LONG CMouse::GetX(void)
{
	return m_mouse.lX;
}

//=============================================================================
// Y座標取得処理
//=============================================================================
LONG CMouse::GetY(void)
{
	return m_mouse.lY;
}