// ------------------------------------------
//
// �`���[�g���A���w�b�_�[���� [tutorial.h]
// Author : Nishiyama Koki
//
// ------------------------------------------
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
#include "basemode.h"

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
class CPlayer;				// �v���C���[�N���X

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CTutorial : public CBaseMode
{
public:
	/* �񋓌^ */

	/* �֐� */
	// �R���X�g���N�^
	CTutorial();
	// �f�X�g���N�^
	~CTutorial();
	// ����������
	void Init(void);
	// �I������
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
	// ��������
	static CTutorial * Create(void);
//#ifdef _DEBUG
//	// �f�o�b�O����
//	void Debug(void);
//#endif // _DEBUG
protected:
private:
};
#endif
