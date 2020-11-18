// ------------------------------------------
//
// 入力処理 [keyboard.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "keyboard.h"
#include "debugproc.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CKeyboard::CKeyboard()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CKeyboard::~CKeyboard()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力デバイスの初期化
	if (!CInput::Init(hInstance, hWnd) == S_OK)
	{
		return E_FAIL;
	}
	// 入力デバイス（キーボード）の作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CKeyboard::Uninit(void)
{
	// 入力デバイスの終了
	CInput::Uninit();

	// キーボードのデバイスの終了
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CKeyboard::Update(void)
{
	// 変数宣言
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
	int nCntKey;

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			if (aKeyState[nCntKey] != 0)
			{
				m_sbState = true;
			}
			m_aTrigger[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aState[nCntKey] = aKeyState[nCntKey];	// キープレス情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードへのアクセス権を取得

		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
		{
			for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
			{
				if (aKeyState[nCntKey] != 0)
				{
					m_sbState = true;
				}
				m_aTrigger[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
				m_aState[nCntKey] = aKeyState[nCntKey];	// キープレス情報保存
			}
		}
	}
}

// ------------------------------------------
// 押されている状態を取得
// ------------------------------------------
bool CKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aState[nKey] & 0x80) ? true : false;
}

// ------------------------------------------
// 押した時状態を取得
// ------------------------------------------
bool CKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aTrigger[nKey] & 0x80) ? true : false;
}
