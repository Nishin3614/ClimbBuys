// ------------------------------------------
//
// ���͏��� [keyconfig.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "keyconfig.h"
#include "manager.h"

// ------------------------------------------
//
// �}�N����`
//
// ------------------------------------------

// ------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------

// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CKeyConfig::CKeyConfig()
{
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CKeyConfig::~CKeyConfig()
{
}

// ------------------------------------------
// ������
// ------------------------------------------
void CKeyConfig::Init(void)
{
	// �L�[�{�[�hID�̐ݒ�
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
	// �W���C�p�b�hID�̐ݒ�
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
// �I��
// ------------------------------------------
void CKeyConfig::Uninit(void)
{
}

// ------------------------------------------
// �X�V
// ------------------------------------------
void CKeyConfig::Update(void)
{
}

// ------------------------------------------
// �L�[�ڍ�(�����Ă����)���擾
// ------------------------------------------
bool const CKeyConfig::GetKeyConfigPress(CONFIG const & config)
{
	// �L�[�{�[�h��������Ă�����true��Ԃ�
	if (CManager::GetKeyboard()->GetKeyboardPress(m_nKeyboardId[config])) return true;
	// �W���C�p�b�h��NULL�ł͂Ȃ��̂Ȃ�
	else if (CManager::GetJoy() != NULL)
	{
		// �W���C�p�b�h��������Ă�����true��Ԃ�
		if (CManager::GetJoy()->GetPress(0, (CJoypad::KEY)m_JoypadId[config])) return true;
	}
	return false;
}

// ------------------------------------------
// �L�[�ڍ�(�������Ƃ�)���擾
// ------------------------------------------
bool const CKeyConfig::GetKeyConfigTrigger(CONFIG const & config)
{
	// �L�[�{�[�h��������Ă�����true��Ԃ�
	if (CManager::GetKeyboard()->GetKeyboardTrigger(m_nKeyboardId[config])) return true;
	// �W���C�p�b�h��NULL�ł͂Ȃ��̂Ȃ�
	else if (CManager::GetJoy() != NULL)
	{
		// �W���C�p�b�h��������Ă�����true��Ԃ�
		if (CManager::GetJoy()->GetTrigger(0, (CJoypad::KEY)m_JoypadId[config])) return true;
	}
	return false;
}