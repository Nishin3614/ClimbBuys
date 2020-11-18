// ------------------------------------------
//
// �R���g���[���[�w�b�_�[���� [joypad.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
#include "main.h"
#include "input.h"

// ------------------------------------------
//
// �}�N����`
//
// ------------------------------------------
#define MAX_GAMEPAD (1)

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CJoypad : public CInput
{
public:
	/* �񋓌^ */
	// �L�[���
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
	// �������
	typedef enum
	{
		DIRECTION_LEFT = 0,
		DIRECTION_RIGHT,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_MAX,
	} DIRECTION;

	/* �֐� */
	CJoypad();
	~CJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	// ������Ă���Ƃ��̏��擾
	bool GetPress(
		int const &nJoy,	// �W���C�p�b�h�̔ԍ�
		KEY const &key		// �L�[���
	);
	// �������Ƃ��̏��擾
	bool GetTrigger(
		int const &nJoy,	// �W���C�p�b�h�̔ԍ�
		KEY const &key		// �L�[���
	);
	// �b�����Ƃ��̏��擾
	bool GetRelease(
		int const &nJoy,	// �W���C�p�b�h�̔ԍ�
		KEY const &key		// �L�[���
	);
	bool GetRepeat(
		int const &nJoy,	// �W���C�p�b�h�̔ԍ�
		KEY const &key		// �L�[���
	);
	// L�{�^����������Ă�����擾
	int GetTriggerLeft(
		int const &nJoy,	// �W���C�p�b�h�̔ԍ�
		KEY const &key		// �L�[���
	);
	// R�{�^���������ꂢ�Ă�����擾
	int GetTriggerRight(
		int const &nJoy,	// �W���C�p�b�h�̔ԍ�
		KEY const &key		// �L�[���
	);
	// ���X�e�B�b�N�̏��擾
	void GetStickLeft(
		int const &nJoy,	// �W���C�p�b�h�ԍ�
		int & pValueH,		// ���������̗�
		int & pValueV		// ���������̗�
	);
	// �E�X�e�B�b�N�̏��擾
	void GetStickRight(
		int const &nJoy,	// �W���C�p�b�h�ԍ�
		int & pValueH,		// ���������̗�
		int & pValueV		// ���������̗�
	);
	// ���X�e�B�b�N��bool���
	bool const &GetBoolStickLeft(
		int const &nJoy,			// �W���C�p�b�h�ԍ�
		DIRECTION const &direction	// �������
	)
	{
		return m_abStickTrigger[nJoy][direction];
	};
protected:
private:
	/* �ϐ��錾 */
	static LPDIRECTINPUTDEVICE8 m_pJoyDevice[MAX_GAMEPAD];	// ���̓f�o�C�X�ւ̃|�C���^
	DIJOYSTATE2 m_aState[MAX_GAMEPAD];						// �Q�[���p�b�h��Ԃ��󂯎�郏�[�N
	bool m_abStick[MAX_GAMEPAD][DIRECTION_MAX];							// �X�e�B�b�N���
	bool m_abStickTrigger[MAX_GAMEPAD][DIRECTION_MAX];					// �ߋ��̃X�e�B�b�N���
	int m_nCntWaitRepeat;									// �J�E���g�}�b�N�X
	int m_nIntervalRepeat;									// �����̊�
	static int m_nCntpad;									// �J�E���g
	int m_aStateRepeatCnt[MAX_GAMEPAD][KEY_MAX];			// ���s�[�g��
	bool m_abState[MAX_GAMEPAD][KEY_MAX];					// �Q�[���p�b�h��Ԃ̕ۑ��ꏊ
	bool m_abStateTrigger[MAX_GAMEPAD][KEY_MAX];			// �Q�[���p�b�h��Ԃ̃I���E�I�t
	bool m_abStateRelease[MAX_GAMEPAD][KEY_MAX];			// �Q�[���p�b�h��Ԃ̕Ԃ�
	bool m_abStateRepeat[MAX_GAMEPAD][KEY_MAX];				// �Q�[���p�b�h��Ԃ��J��Ԃ�
	/* �֐� */
	// �R�[���o�b�N�֐�
	// �\�͎擾
	static BOOL CALLBACK EnumCallback(
		const DIDEVICEINSTANCE *pdidInstance,
		VOID *pContext
	);
	// �Q�[���p�b�h�̏�Ԑݒ�
	void SetState(int const & nJoy);
};
#endif
