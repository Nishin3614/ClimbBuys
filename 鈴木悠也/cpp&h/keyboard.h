// ------------------------------------------
//
// ���̓w�b�_�[���� [keyboard.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

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
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔

#ifdef _DEBUG
#define TESTPLAY_NUMBER1 (DIK_1)	// �`���[�g���A���ɋ����J��
#define TESTPLAY_NUMBER2 (DIK_2)
#define TESTPLAY_NUMBER3 (DIK_3)
#define TESTPLAY_NUMBER4 (DIK_4)
#define TESTPLAY_NUMBER5 (DIK_5)
#define TESTPLAY_NUMBER6 (DIK_6)
#define TEXTPLAY_NUMBER7 (DIK_7)
#define TEXTPLAY_NUMBER8 (DIK_8)
#define TEXTPLAY_NUMBER9 (DIK_9)
#define TEXTPLAY_NUMBER0 (DIK_0)

#endif // _DEBUG

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
class CKeyboard : public CInput
{
public:
	/* �֐� */
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);	// ������Ă�����擾
	bool GetKeyboardTrigger(int nKey);	// ���������擾

protected:
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;	// ���̓f�o�C�X�ւ̃|�C���^
	BYTE m_aState[NUM_KEY_MAX];		// ���͂��Ă�����
	BYTE m_aTrigger[NUM_KEY_MAX];	// ���͎����
};
#endif
