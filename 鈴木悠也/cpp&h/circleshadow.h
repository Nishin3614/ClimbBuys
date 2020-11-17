// ----------------------------------------
//
// �~�`�L�����N�^�[�����̐���[circleshadow.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _CIRCLESHADOW_H_
#define _CIRCLESHADOW_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_three.h"

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
class CCircleshadow : public CScene_THREE
{
public:
	/* �񋓌^ */

	/* �֐� */
	CCircleshadow();
	~CCircleshadow();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
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
	// �쐬(�V�[���p������)
	static CCircleshadow * Create(
		D3DXVECTOR3 const &	pos,						// �ʒu
		D3DXVECTOR3 const &	size,						// �T�C�Y
		int const &			nTexType					// �e�N�X�`���[�^�C�v
	);					// �쐬
	static HRESULT Load(void);							// �ǂݍ���
	static void UnLoad(void);							// �j��
	// �V���h�E��Ԃ�ݒ�
	void SetShadow(bool const & bShadow) { m_bShadow = bShadow; };
	// �V���h�E��Ԃ��擾
	bool GetShadow(void) { return m_bShadow; };
protected:
private:
	/* �֐� */
	/* �ϐ� */
	bool	m_bShadow;	// �V���h�E���
};
#endif