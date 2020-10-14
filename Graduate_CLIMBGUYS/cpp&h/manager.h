// ------------------------------------------
//
// �}�l�[�W���[�����̐���[manager.h]
// Author : Koki Nishiyama
//
// ------------------------------------------
#ifndef _MANAGER_H_
#define _MANAGER_H_	 // �t�@�C��������T�����߂�

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
/* ��{ */
#include "main.h"
#include "Calculation.h"

/* �`�� */
#include "renderer.h"

/* ���̓f�o�C�X */
#include "keyboard.h"
#include "joypad.h"
#include "mouse.h"
#include "keyconfig.h"

/* �T�E���h */
#include "sound.h"

/* �f�o�b�O�\�� */
#include "debugproc.h"

// ------------------------------------------
//
// �}�N���֐�
//
// ------------------------------------------
#define ONEPLAYER_STAGE		(5)
#define DOUBLEPLAYER_STAGE	(5)
#define DERAY_TIME(X)		(X * 60)
#define MAX_PLAYER			(4)

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------
class CFade;		// ��ʑJ��
class CBaseMode;	// �x�[�X���[�h�N���X
class CLoadScreen;	// �ǂݍ��݉��

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CManager
{
public:
	/* �񋓌^ */
	// ���[�h
	typedef enum
	{
		MODE_TITLE = 0,	// �^�C�g��
		MODE_TUTORIAL,	// �`���[�g���A��
		MODE_GAME,		// �Q�[��
		MODE_RESULT,	// ���U���g
		MODE_RANKING,	// �����L���O
		MODE_MAX		// �ő吔
	} MODE;
	/* �֐� */
	// �R���X�g���N�^
	CManager();
	// �f�X�g���N�^
	~CManager();
	// ����������
	//	hWnd		: �E�B���h�E�n���h��
	//	bWindow		: �E�B���h�E���
	//	hInstance	: �C���X�^���X�n���h��
	HRESULT  Init(
		HWND hWnd,			// �E�B���h�E�n���h��
		BOOL bWindow,		// �E�B���h�E���
		HINSTANCE hInstance	// �C���X�^���X�n���h��
	);
	// �I������
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
	// ���[�h�ݒ�
	//	mode	: ���[�h
	static void SetMode(
		MODE const &mode	// ���[�h
	);
	// ���[�h�擾
	static MODE GetMode(void) { return m_mode; };
	// �����_���[���擾
	static CRenderer * GetRenderer(void) { return m_renderer; };
	// �L�[�{�[�h���擾
	static CKeyboard * GetKeyboard(void) { return m_keyboard; };
	// �W���C�p�b�h���擾
	static CJoypad * GetJoy(void) { return m_joypad; };
	// �}�E�X���擾
	static CMouse * GetMouse(void) { return m_mouse; };
	// �L�[�ڍ׎擾
	static CKeyConfig * GetKeyConfig(void) { return m_keyconfig; };
	// �T�E���h���擾
	static CSound * GetSound(void) { return m_sound; };
	// ��ʑJ�ڏ��擾
	static CFade * GetFade(void) { return m_fade; };
	// ���[�h���擾
	static CBaseMode * GetBaseMode(void) { return m_BaseMode; };
	// �ǂݍ��݉�ʎ擾
	static CLoadScreen * GetLoadScreen(void) { return m_pLoadScreen; };
protected:

private:
	/* �ϐ� */
	static CRenderer * m_renderer;			// �����_�����O
	static CSound * m_sound;				// �T�E���h
	static CKeyboard * m_keyboard;			// �L�[�{�[�h
	static CJoypad * m_joypad;				// �W���C�p�b�h
	static CMouse * m_mouse;				// �}�E�X
	static CKeyConfig * m_keyconfig;		// �L�[�ڍ�
	static CFade * m_fade;					// ��ʑJ��
	static CBaseMode * m_BaseMode;			// ���݂̃��[�h
	static CLoadScreen * m_pLoadScreen;		// �ǂݍ��݉��
	static MODE m_mode;						// ���[�h
	static bool	m_bWire;					// ���C���[���[�h
};

#endif // !_MANAGER_H_