// ----------------------------------------
//
// �ԍ������̐���[number.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _NUMBER_H_
#define _NUMBER_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_two.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CNumber : public CScene_TWO
{
public:
	/* �񋓌^ */
	// �e�N�X�`���[�^�C�v
	typedef enum
	{
		TEX_TIME,
		TEX_SCORE,
		TEX_KNOCK,
		TEX_MAX
	} TEX;

	/* �֐� */
	CNumber();
	~CNumber();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void) {};
#endif // _DEBUG
	// ����������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// �������瓖����Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	void SetNum(int const &nNum);											// �ԍ��ݒ�
	void SetTex(TEX const &tex);											// �e�N�X�`���[�ԍ��ݒ�
	void SetAnim(int const &nRandamTime) {
		m_nRandamTime = nRandamTime;
		m_bRand = true;
	};	// �����_�������؂�ւ����o���Ԑݒ�
	bool &GetAnim(void) { return m_bRand; };
	// �����A�j���[�V�����I��
	void Complusion(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	// �쐬
	static CNumber * Create(
		int			const & nScore,								// �X�R�A
		D3DXVECTOR3 const & pos,								// �ʒu
		TEX			const & tex = TEX_SCORE,					// �e�N�X�`���[�^�C�v
		D3DXVECTOR2 const & size = D3DXVECTOR2(100.0f,100.0f),	// �T�C�Y
		D3DXCOLOR	const & col = D3DXCOLOR_INI					// �F
	);	// �쐬
protected:

private:
	static int		m_TexId[TEX_MAX];	// �e�N�X�`���[ID
	TEX				m_texID;			// �e�N�X�`���[�ԍ�
	int				m_nNum;				// �n���ꂽ����
	int				m_nRandamTime;		// �����_���ɐ������؂�ւ�鉉�o����
	int				m_nFram;			// �t���[���^�C��
	bool			m_bRand;			// �����_����
};

#endif