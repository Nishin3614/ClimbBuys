// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �����u���b�N���� [connectblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CONNECTBLOCK_H_
#define _CONNECTBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseblock;	// �x�[�X�u���b�N�N���X

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CConnectblock : public CScene
{
public:
	/* �񋓌^ */
	// �`���
	typedef enum
	{
		SHAPE_RECT = 0,
		SHAPE_MAX
	} SHAPE;
	// �t�B�[���h�^�C�v���
	typedef enum
	{
		FIELDTYPE_RECT = 0,
		FIELDTYPE_MAX
	} FIELDTYPE;
	/* �\���� */

	/* �֐� */
	// �R���X�g���N�^
	CConnectblock();
	// �f�X�g���N�^
	virtual ~CConnectblock();
	// ����������
	virtual void Init(void);
	// �X�V����
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);

	// ����������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return &m_move; };
	// �����u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �����u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);

	/* �����Ă���u���b�N�̐��� */
	// �쐬(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	shape		: �`���
	//	layer		: ���C���[
	static CConnectblock * Create(
		D3DXVECTOR3		const & pos,									// �ʒu
		SHAPE			const & shape,									// �`���
		CScene::LAYER	const & layer = CScene::LAYER_3DOBJECT			// ���C���[
	);
	// �쐬(�l�Ǘ�)
	//	pos			: �ʒu
	//	shape		: �`���
	static CConnectblock * Create_Self(
		D3DXVECTOR3		const & pos,									// �ʒu
		SHAPE			const & shape									// �`���
		);
	// unique_ptr�쐬(�l�Ǘ�unique)
	// ���߂�l��std::move�Ŏ󂯎��
	//	pos			: �ʒu
	//	shape		: �`���
	static std::unique_ptr<CConnectblock> Creat_Unique(
		D3DXVECTOR3		const & pos,									// �ʒu
		SHAPE			const & shape									// �`���
	);

	/* �t�B�[���h�p�̃u���b�N���� */
	// �쐬(�V�[���Ǘ�)
	//	layer		: ���C���[
	static CConnectblock * Create_Field(
		FIELDTYPE		const & fieldtype = FIELDTYPE_RECT,				// �t�B�[���h�^�C�v
		CScene::LAYER	const & layer = CScene::LAYER_3DOBJECT			// ���C���[
	);
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void  Debug(void);
#endif // _DEBUG
protected:
	/* �֐� */
	// �ݒ� //
private:
	/* �֐� */

	/* �ϐ� */
	std::vector<CBaseblock *>				m_vec_pBaseBlock;		// �x�[�X�u���b�N���
	D3DXVECTOR3								m_pos;					// �ʒu
	D3DXVECTOR3								m_move;					// �ړ�
	D3DXCOLOR								m_col;				// �܂Ƃ܂�̐F���
};

#endif