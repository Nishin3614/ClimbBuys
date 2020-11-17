// ------------------------------------------
//
// ���͏��� [keyconfig.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "keyconfig.h"
#include "manager.h"
#include "XInputPad.h"
#include "keyboard.h"

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
	m_nPadID[CONFIG_DECISION]			= CXInputPad::JOYPADKEY_B;
	m_nPadID[CONFIG_BACK]				= CXInputPad::JOYPADKEY_A;
	m_nPadID[CONFIG_POUSE]				= CXInputPad::JOYPADKEY_START;
	m_nPadID[CONFIG_UP]					= CXInputPad::JOYPADKEY_UP;
	m_nPadID[CONFIG_DOWN]				= CXInputPad::JOYPADKEY_DOWN;
	m_nPadID[CONFIG_LEFT]				= CXInputPad::JOYPADKEY_LEFT;
	m_nPadID[CONFIG_RIGHT]				= CXInputPad::JOYPADKEY_RIGHT;
	m_nPadID[CONFIG_CAMERAUP]			= CXInputPad::JOYPADKEY_UP;
	m_nPadID[CONFIG_CAMERADOWN]			= CXInputPad::JOYPADKEY_DOWN;
	m_nPadID[CONFIG_CAMERALEFT]			= CXInputPad::JOYPADKEY_LEFT;
	m_nPadID[CONFIG_CAMERARIGHT]		= CXInputPad::JOYPADKEY_RIGHT;
	m_nPadID[CONFIG_JUMP]				= CXInputPad::JOYPADKEY_A;
	m_nPadID[CONFIG_BALLOONCREATE]		= CXInputPad::JOYPADKEY_B;
	m_nPadID[CONFIG_TRICK1]				= CXInputPad::JOYPADKEY_L1;
	m_nPadID[CONFIG_TRICK2]				= CXInputPad::JOYPADKEY_R1;
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

	//for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	//{
	//	// �W���C�p�b�h��NULL�ł͂Ȃ��̂Ȃ�
	//	if (CManager::GetPad((PLAYER_TAG)nCnt) != NULL)
	//	{
	//		// �W���C�p�b�h��������Ă�����true��Ԃ�
	//		if (CManager::GetPad((PLAYER_TAG)nCnt)->GetPress((CXInputPad::XINPUT_KEY)m_nPadID[config])) return true;
	//	}
	//}
	return false;
}

// ------------------------------------------
// �L�[�ڍ�(�������Ƃ�)���擾
// ------------------------------------------
bool const CKeyConfig::GetKeyConfigTrigger(CONFIG const & config)
{
	// �L�[�{�[�h��������Ă�����true��Ԃ�
	if (CManager::GetKeyboard()->GetKeyboardTrigger(m_nKeyboardId[config])) return true;
	//// �W���C�p�b�h��NULL�ł͂Ȃ��̂Ȃ�
	//else if (CManager::GetPad() != NULL)
	//{
	//	// �W���C�p�b�h��������Ă�����true��Ԃ�
	//	if (CManager::GetPad()->GetTrigger((CXInputPad::XINPUT_KEY)m_nPadID[config])) return true;
	//}
	return false;
}