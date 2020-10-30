// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���ꏈ�� [stand.h]
// Author : FujiwaraMasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _STAND_H_
#define _STAND_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_x.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CStand : public CScene_X
{
public:
	/* �񋓌^ */

	// �u���b�N�̎��
	typedef enum
	{
		TYPE_BLOCK_NORMAL = 0,	// �����̑���u���b�N
		TYPE_BLOCK_MAP1,		// �}�b�v1�Ɉړ����邽�߂̑���u���b�N
		TYPE_BLOCK_MAP2,		// �}�b�v2�Ɉړ����邽�߂̑���u���b�N
		TYPE_BLOCK_MAP3,		// �}�b�v3�Ɉړ����邽�߂̑���u���b�N
		TYPE_MAX,				// �^�C�v�S�̐�
	} TYPE;

	/* �\���� */

	/* �֐� */
	// �R���X�g���N�^
	CStand();
	// �f�X�g���N�^
	~CStand();
	// ����������
	void Init(void);
	// �X�V����
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
	// �����o�������蔻��
	//	Obj		: �I�u�W�F�^�C�v
	//	pos		: �ʒu
	//	posOld	: �O��̈ʒu
	//	move	: �ړ���
	//	size	: �T�C�Y
	COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// �I�u�W�F�^�C�v
		D3DXVECTOR3 * pos,								// �ʒu
		D3DXVECTOR3 * posOld,							// �O��̈ʒu
		D3DXVECTOR3 * move,								// �ړ���
		D3DXVECTOR3 * size,								// �T�C�Y
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// �I�t�Z�b�g�ʒu
	);
	// �����蔻��
	//	Obj		: �I�u�W�F�^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	OffsetPos	: �I�t�Z�b�g�ʒu
	COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// �I�u�W�F�^�C�v
		D3DXVECTOR3 * pos,								// �ʒu
		D3DXVECTOR3 * posOld,							// �O��̈ʒu
		D3DXVECTOR3 * size,								// �T�C�Y
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// �I�t�Z�b�g�ʒu
	);


	// �x�[�X�u���b�N
	void SetType(TYPE const type) { m_type = type; };
	// �x�[�X�u���b�N
	TYPE	GetType(void) const { return m_type; };
	// �x�[�X�u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �x�[�X�u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);
	// �쐬(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	//	layer		: ���C���[
	static CStand * Create(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId,								// ���f���ԍ�
		CScene::LAYER	const & layer = CScene::LAYER_3DSTAND			// ���C���[
	);
	// �쐬(�l�Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	static CStand * Create_Self(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId								// ���f���ԍ�
	);
	// unique_ptr�쐬(�l�Ǘ�unique)
	// ���߂�l��std::move�Ŏ󂯎��
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	static std::unique_ptr<CStand> Creat_Unique(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId								// ���f���ԍ�
	);

	static void CreateStand_Tutorial();									// �`���[�g���A���p���ꐶ��

	// �������Ԑݒ�
	void SetFall(bool const & bFall) { m_bFall = bFall; };
	// �������Ԏ擾
	bool & GetFall(void) { return m_bFall; };

	// �X�e�[�W����ݒ�
	static void SetDetermination(bool flag) { m_bDetermination = flag; };
	// �X�e�[�W����擾
	static bool &GetDetermination(void) { return m_bDetermination; };

	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
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
	static bool		m_bDetermination;	// �X�e�[�W����܂ł̃J�E���g

	TYPE	m_type;		// �x�[�X�u���b�N
	bool	m_bFall;	// ��������
};

#endif