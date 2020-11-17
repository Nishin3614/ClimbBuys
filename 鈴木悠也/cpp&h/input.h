// ----------------------------------------------------------------------------------------------------
//
// �C���v�b�g�����̐���[input.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------
//
// �}�N���֐�
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------
class CInput
{
public:
	/* �񋓌^ */
	typedef enum
	{
		DEVICETYPE_KEYBOARD = 0,
		DEVICETYPE_JOYPAD
	} DEVICETYPE;
	// �{�^���z�u
	typedef enum
	{
		BUTTON_NONE = 0,
		BUTTON_DECIDE,
		BUTTON_CHANCEL,
		BUTTON_ATTACK,
		BUTTON_TRICKER,
		BUTTON_MAX
	} BUTTON;

	/* �֐� */
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	static void ReleaseAll(void);										// �e�q�����[�X
	static void InitState(void) { m_sbState = false; };					// �{�^�������ꂽ���ǂ����̏�����
	static bool GetState(void) { return m_sbState; };					// �{�^�������ꂽ���ǂ����̎擾
	static void SetState(bool const &bState) { m_sbState = bState; };	// �{�^�������ꂽ���ǂ����̎擾
protected:
	static LPDIRECTINPUT8 m_pInput;	// ���̓f�o�C�X
	static bool m_sbState;			// �{�^���������ꂽ���ǂ���
private:
};

#endif // !_INPUT_H_