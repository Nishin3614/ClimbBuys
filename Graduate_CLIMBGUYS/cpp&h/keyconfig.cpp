// ------------------------------------------
//
// 入力処理 [keyconfig.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "keyconfig.h"
#include "manager.h"

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
CKeyConfig::CKeyConfig()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CKeyConfig::~CKeyConfig()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
void CKeyConfig::Init(void)
{
	// キーボードIDの設定
	m_nKeyboardId[CONFIG_DECISION] = DIK_RETURN;
	m_nKeyboardId[CONFIG_BACK] = DIK_BACKSPACE;
	m_nKeyboardId[CONFIG_POUSE] = DIK_P;
	m_nKeyboardId[CONFIG_UP] = DIK_W;
	m_nKeyboardId[CONFIG_DOWN] = DIK_S;
	m_nKeyboardId[CONFIG_LEFT] = DIK_A;
	m_nKeyboardId[CONFIG_RIGHT] = DIK_D;
	m_nKeyboardId[CONFIG_CAMERAUP] = DIK_UP;
	m_nKeyboardId[CONFIG_CAMERADOWN] = DIK_DOWN;
	m_nKeyboardId[CONFIG_CAMERALEFT] = DIK_LEFT;
	m_nKeyboardId[CONFIG_CAMERARIGHT] = DIK_RIGHT;
	m_nKeyboardId[CONFIG_JUMP] = DIK_SPACE;
	m_nKeyboardId[CONFIG_BALLOONCREATE] = DIK_RETURN;
	m_nKeyboardId[CONFIG_TRICK1] = DIK_U;
	m_nKeyboardId[CONFIG_TRICK2] = DIK_I;
	// ジョイパッドIDの設定
	m_JoypadId[CONFIG_DECISION] = CJoypad::KEY_B;
	m_JoypadId[CONFIG_BACK] = CJoypad::KEY_A;
	m_JoypadId[CONFIG_POUSE] = CJoypad::KEY_START;
	m_JoypadId[CONFIG_UP] = CJoypad::KEY_UP;
	m_JoypadId[CONFIG_DOWN] = CJoypad::KEY_DOWN;
	m_JoypadId[CONFIG_LEFT] = CJoypad::KEY_LEFT;
	m_JoypadId[CONFIG_RIGHT] = CJoypad::KEY_RIGHT;
	m_JoypadId[CONFIG_CAMERAUP] = CJoypad::KEY_UP;
	m_JoypadId[CONFIG_CAMERADOWN] = CJoypad::KEY_DOWN;
	m_JoypadId[CONFIG_CAMERALEFT] = CJoypad::KEY_LEFT;
	m_JoypadId[CONFIG_CAMERARIGHT] = CJoypad::KEY_RIGHT;
	m_JoypadId[CONFIG_JUMP] = CJoypad::KEY_A;
	m_JoypadId[CONFIG_BALLOONCREATE] = CJoypad::KEY_B;
	m_JoypadId[CONFIG_TRICK1] = CJoypad::KEY_LEFT_1;
	m_JoypadId[CONFIG_TRICK2] = CJoypad::KEY_RIGHT_1;
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CKeyConfig::Uninit(void)
{
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CKeyConfig::Update(void)
{
}

// ------------------------------------------
// キー詳細(押している間)情報取得
// ------------------------------------------
bool const CKeyConfig::GetKeyConfigPress(CONFIG const & config)
{
	// キーボードが押されていたらtrueを返す
	if (CManager::GetKeyboard()->GetKeyboardPress(m_nKeyboardId[config])) return true;
	// ジョイパッドがNULLではないのなら
	else if (CManager::GetJoy() != NULL)
	{
		// ジョイパッドが押されていたらtrueを返す
		if (CManager::GetJoy()->GetPress(0, (CJoypad::KEY)m_JoypadId[config])) return true;
	}
	return false;
}

// ------------------------------------------
// キー詳細(押したとき)情報取得
// ------------------------------------------
bool const CKeyConfig::GetKeyConfigTrigger(CONFIG const & config)
{
	// キーボードが押されていたらtrueを返す
	if (CManager::GetKeyboard()->GetKeyboardTrigger(m_nKeyboardId[config])) return true;
	// ジョイパッドがNULLではないのなら
	else if (CManager::GetJoy() != NULL)
	{
		// ジョイパッドが押されていたらtrueを返す
		if (CManager::GetJoy()->GetTrigger(0, (CJoypad::KEY)m_JoypadId[config])) return true;
	}
	return false;
}