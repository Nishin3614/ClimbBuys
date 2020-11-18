// ----------------------------------------------------------------------------------------------------
//
// ColumnCollision�w�b�_�[�����̐���[columncollision.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _COLUMNCOLLISION_H_
#define _COLUMNCOLLISION_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------
#include "collision.h"

// ----------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------
#define COLUMNCOLLISION_MAX (128)

// ----------------------------------------------------------------------------------------------------
//
// ColumnCollision���
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���O��`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
class CMeshdome;
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CColumnCollision : public CCollision
{
public:
	/* �񋓌^ */
	/* �֐� */
	// �R���X�g���N�^
	CColumnCollision();
	// �f�X�g���N�^
	virtual ~CColumnCollision() {};
	// ����������
	virtual void Init(void) {};
	// �I������
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void) {};
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void Debug(void);
	// �����蔻����J�̐ݒ�
	virtual void Collision_Visible_Set(void);
#endif // _DEBUG
	// ����������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// �������瓖����Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// ���L���Ă���`�N���X�擾
	CShape * const GetShape(void) { return m_pColumnShape.get(); };
	//
	// ��`�N���X�̓����蔻���r
	bool Judg(CRectShape * const RectShape);
	// ��`�N���X�̓����蔻���r(�����o������)
	bool Judg(CRectShape * const RectShape, D3DXVECTOR3 * pPos);
	// ���N���X�̓����蔻���r
	bool Judg(CSphereShape * const SphereShape);
	// �~���N���X�̓����蔻���r
	bool Judg(CColumnShape * const ColumnShape);
	// �쐬����(�V�[���Ǘ�)
	static CColumnCollision *Create(
		float const &fRadius,
		float const &fVertical,
		D3DXVECTOR3 const offset = D3DVECTOR3_ZERO,
		OBJTYPE const &obj = OBJTYPE_ACTOR,
		CScene * pScene	= NULL,
		CScene * pParent = NULL,
		bool const &bPush = false,
		bool const &bOpponent = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// �쐬����(�l�Ǘ�)
	static std::unique_ptr<CColumnCollision> Create_Self(
		float const &fRadius,
		float const &fVertical,
		D3DXVECTOR3 const offset = D3DVECTOR3_ZERO,
		OBJTYPE const &obj = OBJTYPE_ACTOR,
		CScene * pScene = NULL,
		CScene * pParent = NULL,
		bool const &bPush = false,
		bool const &bOpponent = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);

protected:

private:
	// �ϐ��錾
	std::unique_ptr<CColumnShape> m_pColumnShape;	// ��`
#ifdef _DEBUG
	// �����蔻������̕ϐ�
	CMeshdome * m_pDebugdome;
#endif // _DEBUG

};

// ----------------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ----------------------------------------------------------------------------------------------------

#endif