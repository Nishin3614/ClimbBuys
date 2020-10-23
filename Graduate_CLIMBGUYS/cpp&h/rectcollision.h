// ----------------------------------------------------------------------------------------------------
//
// RectCollision�w�b�_�[�����̐���[rectcollision.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _RECTCOLLISION_H_
#define _RECTCOLLISION_H_	 // �t�@�C������������߂�

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
#define RECTCOLLISION_MAX (128)

// ----------------------------------------------------------------------------------------------------
//
// RectCollision���
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
class CMeshBox;	// ���b�V���{�b�N�X�N���X
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CRectCollision : public CCollision
{
public:
	/* �񋓌^ */
	/* �\���� */
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
	} INFO;
	/* �֐� */
	CRectCollision();
	virtual ~CRectCollision() {};
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void) {};
#ifdef _DEBUG
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
	// �`�̎擾
	CShape * const GetShape(void) { return m_pRectShape.get(); };
	// ��`�̎擾
	CRectShape * const GetRectShape(void) { return m_pRectShape.get(); };
	// ��`�N���X�̓����蔻���r
	bool Judg(CRectShape * const RectShape);
	// ���N���X�̓����蔻���r
	bool Judg(CSphereShape * const SphereShape);
	// �~���N���X�̓����蔻���r
	bool Judg(CColumnShape * const ColumnShape);
	// �ݒ�
	// �쐬����(�V�[���Ǘ�)
	static CRectCollision *Create(
		D3DXVECTOR3 const size,
		D3DXVECTOR3 const offset = D3DVECTOR3_ZERO,
		OBJTYPE const &obj = OBJTYPE_ACTOR,
		CScene * pScene = NULL,
		CScene * pParent = NULL,
		bool const &bPush = false,
		bool const &bOpponent = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// �쐬����(�l�Ǘ�)
	static std::unique_ptr<CRectCollision> Create_Self(
		D3DXVECTOR3 const size,
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
	std::unique_ptr<CRectShape> m_pRectShape;	// ��`
#ifdef _DEBUG
	// �����蔻������̕ϐ�
	CMeshBox * m_pDebugMeshBox;
#endif // _DEBUG

};

// ----------------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ----------------------------------------------------------------------------------------------------

#endif