// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f������ [player.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
class CMeshBox;		// ���b�V���{�b�N�X�N���X
class CPlayerUI;	// �v���C���[UI

class C3DLine;	// 3D���N���X

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
	// �X�e�[�^�X�^�C�v
	typedef enum
	{
		STATUSTYPE_BLOCK,		// �u���b�N�ɉ����ꂽ�Ƃ�
		STATUSTYPE_JUMP,		// ���܂ꂽ�Ƃ�
		STATUSTYPE_ELECTRIC,	// �d�C��H������Ƃ�
		STATUSTYPE_MAX
	} STATUSTYPE;

	// ----- �v���C���[�̃X�e�[�^�X ----- //
	typedef struct _PLAYER_STATUS
	{
		_PLAYER_STATUS()
		{
			fMove				= 0;				// �ړ���
			fJump				= 0;				// �W�����v��
			fNormalInertia		= 0;				// �ʏ펞�̊���
			fJumpInertia		= 0;				// �W�����v���̊���
			PlayerSize			= D3DVECTOR3_ZERO;	// �v���C���[�T�C�Y
			PlayerOffSet		= D3DVECTOR3_ZERO;	// �v���C���[�I�t�Z�b�g
			PushSize			= 0;				// �����o���p�̃T�C�Y
			PushOffSet			= D3DVECTOR3_ZERO;	// �����o���p�̃I�t�Z�b�g
			nMaxPowerTime		= 0;				// �ő�p���[�^�C��
			nMaxPanicTime		= 0;				// �ő�p�j�b�N�^�C��
			for (int nCnt = 0; nCnt < STATUSTYPE_MAX; nCnt++)
			{
				nMaxStanTime[nCnt]		 = 0;		// �ő�X�^���^�C��
				nMaxInvincibleTime[nCnt] = 0;		// �ő喳�G�^�C��
			}
		}
		float				fMove;								// �ړ���
		float				fJump;								// �W�����v��
		float				fNormalInertia;						// �ʏ펞�̊���
		float				fJumpInertia;						// �W�����v���̊���
		D3DXVECTOR3			PlayerSize;							// �v���C���[�T�C�Y
		D3DXVECTOR3			PlayerOffSet;						// �v���C���[�I�t�Z�b�g
		float				PushSize;							// �����o���p�̃T�C�Y
		D3DXVECTOR3			PushOffSet;							// �����o���p�̃I�t�Z�b�g
		int					nMaxPowerTime;						// �ő�p���[�^�C��
		int					nMaxStanTime[STATUSTYPE_MAX];		// �ő�X�^���^�C��
		int					nMaxInvincibleTime[STATUSTYPE_MAX];	// �ő喳�G�^�C��
		int					nMaxPanicTime;						// �ő�p�j�b�N�^�C��
	}PLAYER_STATUS;
	// ----- �͗��ߏ�� ----- //
	typedef struct _POWER
	{
		// �R���X�g���N�^
		_POWER()
		{
			bCharge = false;		// �`���[�W���
			bDashFlag = false;		// �_�b�V���t���O
			bTackleFrag = false;	// �^�b�N���t���O
			nCntDashTime = 0;		// �_�b�V�����̐؂�ւ��J�E���g
			nPushPower = 1;			// ������
			nCntTime = 0;			// �͗��߃J�E���g
		}
		// ����������
		void Init()
		{
			bCharge = false;		// �`���[�W���
			bDashFlag = false;		// �_�b�V���t���O
			bTackleFrag = false;	// �^�b�N���t���O
			nCntDashTime = 0;		// �_�b�V�����̐؂�ւ��J�E���g
			nPushPower = 1;			// ������
			nCntTime = 0;			// �͗��߃J�E���g
		}
		bool		bCharge;		// �`���[�W���
		bool		bDashFlag;		// �_�b�V���t���O
		bool		bTackleFrag;	// �^�b�N���t���O
		int			nCntDashTime;	// �_�b�V�����̐؂�ւ��J�E���g
		int			nPushPower;		// ������
		int			nCntTime;		// �͗��߃J�E���g
	} POWER;

	typedef struct _STATUSCHANGE
	{
		// �R���X�g���N�^
		_STATUSCHANGE()
		{
			bChange = false;  // �X�e�[�^�X�ϓ����
			nChangeTime = 0;	// �X�e�[�^�X�ϓ��p������
			nMaxTime = 0;		// �ő�p������
		}
		// ����������
		void Init()
		{
			bChange = false;	// �X�e�[�^�X�ϓ����
			nChangeTime = 0;	// �X�e�[�^�X�ϓ��p������
			nMaxTime = 0;		// �ő�p������
		}
		// �X�e�[�^�X�ϓ��ݒ�
		void Set(bool const & bSouceChange,int const & nSouceMaxTime)
		{
			bChange = bSouceChange;			// �X�e�[�^�X�ϓ����
			nMaxTime = nSouceMaxTime;		// �ő�p������
		}
		bool		bChange;			// �X�e�[�^�X�ϓ����
		int			nChangeTime;		// �X�e�[�^�X�ϓ��p������
		int			nMaxTime;			// �ő�p������
	} STATUSCHANGE;
	// ���p��
	// ���S���
	union DIESTATUS
	{
		DIESTATUS() {}
		// ����������
		void Init(void)
		{
			for (int nStatus = 0; nStatus < 6; nStatus++)
			{
				bStatus[nStatus] = false;
			}
		}
		struct
		{
			bool	bUp;		// ��̓����蔻��
			bool	bDown;		// ���̓����蔻��
			bool	bRight;		// �E�̓����蔻��
			bool	bLeft;		// ���̓����蔻��
			bool	bFront;		// �O�̓����蔻��
			bool	bBack;		// ��̓����蔻��
		};
		bool	bStatus[6];
	};

	// ��
	// ���񂾌���
	typedef enum
	{
		LIVE = 0,			// �����Ă���
		DIECAUSE_PRESS,		// ����
		DIECAUSE_FALL,		// ������
		DIECAUSEMAX			// �ő吔
	} DIECAUSE;
	// ----- �L�^ ----- //
	typedef struct _RECORD
	{
		// �R���X�g���N�^
		_RECORD()
		{
			nRanking = 1;  					// �����L���O
			nTime = -1;						// �^�C��
			nPushCnt = 0;					// ��������
			DieCause = DIECAUSE::LIVE;		// ���񂾌���
		}
		int			nRanking;		// �����L���O
		int			nTime;			// �^�C��
		int			nPushCnt;		// ��������
		DIECAUSE	DieCause;		// ���񂾌���
	} RECORD;
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
#if IMGUI_DEBUG

	static void AllDebugImGui(void);

#endif // IMGUI_DEBUG

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
	void SetDashFlag(bool const &bDashFlag) { m_Power.bDashFlag = bDashFlag; };
	// �_�b�V�����Ă��邩�ǂ����̃t���O�̎擾
	bool		&GetDashFlag(void)			{ return m_Power.bDashFlag; };
	// �L�^���̎擾
	RECORD & GetRecord(void)				{ return m_Record; };
	// �o�l�p�W�����v����
	void SpringJump(void);

	// �Q�[���p�b�h�̎擾
	CXInputPad *GetPad()					{ return m_pPad; };

	static void InitDieCount() { m_nDieCnt = 0; };
	static int GetDieCount() { return m_nDieCnt; };
	// �v���C���[�̃X�e�[�^�X���擾
	static PLAYER_STATUS GetPlayerStatus(void) { return m_PlayerStatus; };
	// �v���C���[�̎��S��Ԃ̎擾
	DIESTATUS &GetPlayerDieStatus(void) { return m_DieStatus; };
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
	// �u���b�N�̉����o������
	void PushBlock(
		CBaseblock * pBlock,			// �u���b�N���
		CBaseblock::GRID const & Grid	// ����
	);
	// �����蔻�菈��
	void Collision(void);
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
	static void PlayerStatusSave(void);
	// �v���C���[�̃X�e�[�^�X�̏����l�̃��[�h
	static void PlayerStatusInitLoad(void);

	// �v���C���[�̈ړ������ݒ�
	void PlayerMoveSet(
		D3DXVECTOR3 & Vec,
		D3DXVECTOR3 const & Rot,
		D3DXVECTOR3 & Move
	);

	// �v���C���[�̏�ԍX�V
	void StateUpdate(void);
	// �͗��߂̍X�V
	void PowerUpdate(void);
	// �X�^����Ԃ̍X�V
	void StanUpdate(void);
	// ���G��Ԃ̍X�V
	void InvincibleUpdate(void);
	// �p�j�b�N��Ԃ̍X�V
	void PanicUpdate(void);
	// �d�C�u���b�N�g�p��
	void ElectricUse(void);

	/* �ϐ� */
	CXInputPad					*m_pPad;						// �p�b�h�̃|�C���^
	bool						m_bDieFlag;						// ���S�t���O
	static PLAYER_STATUS		m_PlayerStatus;					// �v���C���[�̃X�e�[�^�X
	static PLAYER_STATUS		m_PlayerStatusInit;				// �v���C���[�̏����X�e�[�^�X
	POWER						m_Power;						// �͗���
	STATUSCHANGE				m_Stan;							// �X�^�����
	STATUSCHANGE				m_Invincible;					// ���G���
	STATUSCHANGE				m_Panic;						// �p�j�b�N���
	CPlayerUI					*m_pPlayerUI;					// �v���C���[UI
	bool						m_bSpringFlag;					// �΂˂̔������񂾂��ʂ�
	RECORD						m_Record;						// �L�^���

	static int					m_nDieCnt;						// ���S�l�� ��
	DIESTATUS					m_DieStatus;					// ���S���

#ifdef _DEBUG
	CMeshBox * pCollisionBox;
	C3DLine *	pCollisionLine;
#endif // _DEBUG

};
#endif