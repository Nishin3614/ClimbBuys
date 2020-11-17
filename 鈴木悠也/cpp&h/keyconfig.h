// ------------------------------------------
//
// �L�[�ڍ׏��� [keyconfig.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _KEYCONFIG_H_
#define _KEYCONFIG_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// �}�N����`
//
// ------------------------------------------

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
class CKeyConfig
{
public:
	/* �񋓌^ */
	typedef enum
	{
		CONFIG_DECISION = 0,
		CONFIG_BACK,
		CONFIG_POUSE,
		CONFIG_UP,
		CONFIG_DOWN,
		CONFIG_LEFT,
		CONFIG_RIGHT,
		CONFIG_CAMERAUP,
		CONFIG_CAMERADOWN,
		CONFIG_CAMERALEFT,
		CONFIG_CAMERARIGHT,
		CONFIG_JUMP,
		CONFIG_BALLOONCREATE,
		CONFIG_TRICK1,
		CONFIG_TRICK2,
		CONFIG_MAX
	} CONFIG;
	/* �֐� */
	CKeyConfig();
	~CKeyConfig();
	void Init(void);
	void Uninit(void);
	void Update(void);
	bool const GetKeyConfigPress(CONFIG const &config);	// �L�[�ڍ�(�����Ă����)���擾
	bool const GetKeyConfigTrigger(CONFIG const &config);	// �L�[�ڍ�(�����ꂽ�Ƃ�)���擾
protected:
private:
	/* �֐� */
	/* �ϐ� */
	int m_nKeyboardId[CONFIG_MAX];			// �L�[�{�[�hID
	int m_nPadID[CONFIG_MAX];					// �W���C�p�b�hID
};
#endif
