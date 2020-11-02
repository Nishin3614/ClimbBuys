// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f������ [player.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"
#include "baseblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_FRONTFORCE		(10)
#define PLAYER_G				(0.5f)								// �d��
#define PLAYER_RESISTANCE		(0.5f)								// ��R��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CMeshBox;	// ���b�V���{�b�N�X�N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayer : public CCharacter, public CPlayerTAG
{
public:
	// ---------���[�V�����^�C�v---------- //
	typedef enum
	{
		MOTIONTYPE_MAX = CCharacter::MOTIONTYPE_MAX
	} MOTIONTYPE;
	// �����蔻��
	typedef enum
	{
		COLLISIONTYPE_CHARACTER = 0,
		COLLISIONTYPE_PUSH,
		COLLISIONTYPE_MAX
	} COLLISIONTYPE;
	// �����蔻��
	typedef enum
	{
		DIESTATE_UP		= 0b000001,	// ��
		DIESTATE_UNDER	= 0b000010,	// ��
		DIESTATE_LEFT	= 0b000100,	// ��
		DIESTATE_RIGHT	= 0b001000,	// �E
		DIESTATE_FRONT	= 0b010000,	// �O
		DIESTATE_BACK	= 0b100000,	// ���
		DIESTATE_MAX
	} DIESTATE;

	// ----- �v���C���[�̃X�e�[�^�X ----- //
	typedef struct
	{
		float				fMove;				// �ړ���
		float				fJump;				// �W�����v��
		float				fDash;				// �_�b�V���̈ړ���
		float				fNormalInertia;		// �ʏ펞�̊���
		float				fJumpInertia;		// �W�����v���̊���
		D3DXVECTOR3			PlayerSize;			// �v���C���[�T�C�Y
		D3DXVECTOR3			PlayerOffSet;		// �v���C���[�I�t�Z�b�g
		D3DXVECTOR3			PushSize;			// �����o���p�̃T�C�Y
		D3DXVECTOR3			PushOffSet;			// �����o���p�̃I�t�Z�b�g
	}PLAYER_STATUS;

	/* �֐� */
	// �R���X�g���N�^
	CPlayer(CCharacter::CHARACTER const &character);
	// �f�X�g���N�^
	virtual ~CPlayer();
	// ����������
	virtual void Init(void);
	// �I�͏���
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);
	// �v���C���[�����񂾂Ƃ��̊֐�
	virtual void Die(void);
	void OtherDie(void);
	// ���S�t���O�̎擾
	bool GetDieFlag(void) { return m_bDieFlag; };

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
	);
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
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
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// ��������
	//	tag	: �v���C���[�^�O
	//	pos	: �ʒu
	static CPlayer * Create(
		PLAYER_TAG tag,								// �v���C���[�^�O
		D3DXVECTOR3 const & pos = D3DVECTOR3_ZERO	// �ʒu
);
	// ���\�[�X�̓ǂݍ��ݏ���
	static HRESULT Load(void);
	// ���\�[�X�̊J������
	static void UnLoad(void);
	// �_�b�V�����Ă��邩�ǂ����̃t���O�̐ݒ�
	void SetDashFlag(bool const &bDashFlag) { m_bDashFlag = bDashFlag; };
	// �_�b�V�����Ă��邩�ǂ����̃t���O�̎擾
	bool		&GetDashFlag(void) { return m_bDashFlag; };
protected:
private:
	/* �\���� */
	/* �֐� */
	// ���L�����ړ�����
	void MyMove(void);
	// ���L�����s������
	void MyAction(void);
	// ��Ԃɂ���Ẵ��[�V�����ݒ菈��
	void StatusMotion(void);
	// ����ɏ���Ă��锻��
	void StandJudg(
		CScene_X * pScene_X,
		bool const & bJudg
	);
	// �u���b�N�̉����o������
	void PushBlock(
		CScene_X * pScene_X,			// �V�[��X���
		CBaseblock::GRID const & Grid	// ����
	);
	// �����蔻�菈��
	void Collision(void);
	// ����u���b�N�Ƃ̔���
	void StandCollision(void);
	// �u���b�N�Ƃ̓����蔻��
	void BlockCollision(void);
	// �L�����N�^�[���m�̓����蔻��
	void CharacterCollision(void);
	// �����o������̃v���C���[�̓����蔻��
	void PushAfter_Collision(
		CBaseblock * pBaseBlock,
		CBaseblock::GRID Grid
		);
	// �v���C���[�̃X�e�[�^�X�̃��[�h
	static void PlayerStatusLoad(void);
	// �v���C���[�̃X�e�[�^�X�̃Z�[�u
	void PlayerStatusSave(void);
	// �v���C���[�̃X�e�[�^�X�̏����l�̃��[�h
	void PlayerStatusInitLoad(void);

	/* �ϐ� */
	CXInputPad				*m_pPad;						// �p�b�h�̃|�C���^
	bool					m_bDieFlag;						// ���S�t���O
	bool					m_bDashFlag;					// �_�b�V���t���O
	int						m_nCntDashTime;					// �_�b�V�����̐؂�ւ��J�E���g
	static PLAYER_STATUS	m_PlayerStatus;					// �v���C���[�̃X�e�[�^�X
	PLAYER_STATUS			m_PlayerStatusInit;				// �v���C���[�̏����X�e�[�^�X

#ifdef _DEBUG
	CMeshBox * pCollisionBox[COLLISIONTYPE_MAX];
#endif // _DEBUG

};
#endif