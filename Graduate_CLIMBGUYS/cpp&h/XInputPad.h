// =====================================================================================================================================================================
//
// �Q�[���p�b�h���� [Xinput.h]
// Author : Sato Yoshiki
//
// =====================================================================================================================================================================
#ifndef _GAMEPAD_XINPUT_H_
#define _GAMEPAD_XINPUT_H_

// =====================================================================================================================================================================
// �C���N���[�h�t�@�C��
// =====================================================================================================================================================================
#include "main.h"

// =====================================================================================================================================================================
// �}�N����`
// =====================================================================================================================================================================
#define MAX_SAVE_KEYSTATE (60)		// �L�[��ۑ�����ő�l
#define STICK_MAX_RANGE (32768.0f)	// �X�e�B�b�N�̍ő�͈�

// =====================================================================================================================================================================
//�N���X��`
// =====================================================================================================================================================================
class CXInputPad
{
public:
	CXInputPad();		// �R���X�g���N�^
	~CXInputPad();		// �f�X�g���N�^

	//���̓L�[
	enum XINPUT_KEY
	{
		JOYPADKEY_UP	 = XINPUT_GAMEPAD_DPAD_UP,			// �\���L�[ ��
		JOYPADKEY_DOWN	 = XINPUT_GAMEPAD_DPAD_DOWN,		// �\���L�[ ��
		JOYPADKEY_LEFT	 = XINPUT_GAMEPAD_DPAD_LEFT,		// �\���L�[ ��
		JOYPADKEY_RIGHT	 = XINPUT_GAMEPAD_DPAD_RIGHT,		// �\���L�[ ��
		JOYPADKEY_START	 = XINPUT_GAMEPAD_START,			// START
		JOYPADKEY_BACK	 = XINPUT_GAMEPAD_BACK,				// BACK
		JOYPADKEY_L1	 = XINPUT_GAMEPAD_LEFT_THUMB,		// L1
		JOYPADKEY_R1	 = XINPUT_GAMEPAD_RIGHT_THUMB,		// R1
		JOYPADKEY_L2	 = XINPUT_GAMEPAD_LEFT_SHOULDER,	// L2
		JOYPADKEY_R2	 = XINPUT_GAMEPAD_RIGHT_SHOULDER,	// R2
		JOYPADKEY_A		 = XINPUT_GAMEPAD_A,				// A
		JOYPADKEY_B		 = XINPUT_GAMEPAD_B,				// B
		JOYPADKEY_X		 = XINPUT_GAMEPAD_X,				// X
		JOYPADKEY_Y		 = XINPUT_GAMEPAD_Y,				// Y
	};

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, PLAYER_TAG PlayerTag);	// ������
	void Uninit(void);								// �I��
	void Update(void);								// �X�V

	bool GetTrigger(XINPUT_KEY Button, size_t Frame);		// �{�^�����������Ƃ� ( �P�� )
	bool GetPress(XINPUT_KEY Button);						// �{�^��������������Ƃ� ( ������ )
	void GetStickLeft(float *pValueX, float *pValueZ);		// ���X�e�B�b�N�l�擾
	void GetStickRight(float *pValueX, float *pValueZ);		// �E�X�e�B�b�N�l�擾
	bool GetConnect() { return m_bConnect; };				// �R���g���[�����ڑ�����Ă��邩�ǂ���

	/* �o�C�u���[�V�����֌W */
	void StartVibration(int nCnt) { m_bVibration = true, m_nCntVibration = nCnt; };		// �o�C�u���[�V�����J�n
	void EndVibration() { m_bVibration = false; };										// �o�C�u���[�V�����I��

private:
	XINPUT_STATE	m_pState;							// ���݂̃R���g���[���̏��
	WORD			m_OldState;							// �O��̃R���g���[���[�̏��
	bool			m_bVibration;						// �o�C�u���[�V�������Ă邩�ǂ���
	bool			m_bConnect;							// �R���g���[���[���ڑ�����Ă��邩�ǂ���
	int				m_nCntVibration;					// �o�C�u���[�V�����p�̃J�E���g
	PLAYER_TAG		m_PlayerTag;						// �v���C���[�^�O
};

#endif