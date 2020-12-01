// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �L�����N�^�[���� [character.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CMeshobit;
class CCollision;
class CStencilshadow;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCharacter : public CScene
{
public:
	/* �񋓌^ */
	// �X�e�[�^�X
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DIE,
		STATE_MAX
	} STATE;
	// ��{���[�V�����^�C�v
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// �ҋ@���[�V����
		MOTIONTYPE_MOVE,			// �ړ����[�V����
		MOTIONTYPE_DASH,			// �_�b�V�����[�V����
		MOTIONTYPE_JUMP,			// �W�����v���[�V����
		MOTIONTYPE_BANZAI,			// �o���U�C���[�V����
		MOTIONTYPE_VICTORY_01,		// �������[�V����_01
		MOTIONTYPE_VICTORY_02,		// �������[�V����_02
		MOTIONTYPE_VICTORY_03,		// �������[�V����_03
		MOTIONTYPE_VICTORY_04,		// �������[�V����_04
		MOTIONTYPE_MAX				// �ő吔
	} MOTIONTYPE;
	// �L�����N�^�[
	typedef enum
	{
		CHARACTER_PLAYER_0 = 0,	// �v���C���[0
		CHARACTER_PLAYER_1,		// �v���C���[1
		CHARACTER_PLAYER_2,		// �v���C���[2
		CHARACTER_PLAYER_3,		// �v���C���[3
		CHARACTER_MAX,			// �L�����N�^�[�S�̐�
	} CHARACTER;

	/* �\���� */

	/* �֐� */
	// �R���X�g���N�^
	CCharacter(CHARACTER const &character);
	// �f�X�g���N�^
	virtual ~CCharacter();
	// ����������
	virtual void Init(void);
	// �X�V����
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);
	// �L�����N�^�[�����񂾂Ƃ�
	virtual void Die(void);
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
	) = 0;
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) = 0;
	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return &m_posold; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return &m_move; };
	// �����o�������蔻��
	//	Obj		: �I�u�W�F�^�C�v
	//	pos		: �ʒu
	//	posOld	: �O��̈ʒu
	//	move	: �ړ���
	//	size	: �T�C�Y
	virtual COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// �I�u�W�F�^�C�v
		D3DXVECTOR3 * pos,								// �ʒu
		D3DXVECTOR3 * posOld,							// �O��̈ʒu
		D3DXVECTOR3 * move,								// �ړ���
		D3DXVECTOR3 * size,								// �T�C�Y
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// �I�t�Z�b�g�ʒu
	) = 0;
	// �����蔻��
	//	Obj		: �I�u�W�F�^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	OffsetPos	: �I�t�Z�b�g�ʒu
	virtual COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// �I�u�W�F�^�C�v
		D3DXVECTOR3 * pos,								// �ʒu
		D3DXVECTOR3 * posOld,							// �O��̈ʒu
		D3DXVECTOR3 * size,								// �T�C�Y
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// �I�t�Z�b�g�ʒu
	) = 0;
	// �K�v�ɉ��������� //
	// �d��
	void AddGravity(float const &fGravity)					{ m_move.y -= fGravity; };
	// �ݒ� //
	// �ʒu
	void SetPos(D3DXVECTOR3 const &pos)						{ m_pos = pos; };
	// �O��̈ʒu
	void SetPosOld(D3DXVECTOR3 const &Posold)				{ m_posold = Posold; };
	// �ړ���
	void SetMove(D3DXVECTOR3 const &move)					{ m_move = move; };
	// ��]
	void SetRot(D3DXVECTOR3 const &rot)						{ m_rot = rot; };
	// �W�����v�ł��邩�ǂ����̃t���O�̐ݒ�
	void SetJumpAble(bool const &jump)						{ m_bJumpable = jump; };
	//  ���S���Ă��邩�ǂ����̃t���O�̐ݒ�
	void SetDie(bool const &die)							{ m_bDie = die; };

	// �����x�N�g���ݒ�
	void SetDirectionVec(D3DXVECTOR3 const &direct)			{ m_Directvector = direct; };
	// �擾 //
	// �ʒu
	D3DXVECTOR3 &GetPos(void)								{ return m_pos; };
	// �O��̈ʒu�擾
	D3DXVECTOR3 &GetPosOld(void)							{ return m_posold; };
	// �ړ���
	D3DXVECTOR3 &GetMove(void)								{ return m_move; };
	// ��]
	D3DXVECTOR3 &GetRot(void)								{ return m_rot; };
	// �W�����v�ł��邩�ǂ����̃t���O�̎擾
	bool		&GetJumpAble(void)							{ return m_bJumpable; };
	// ���S���Ă��邩�ǂ����̃t���O�̎擾
	bool		&GetDie(void)								{ return m_bDie; };
	// �e�Ǝq�̉�]��
	D3DXVECTOR3 *GetPartsRot(int const nModelID);
	// �e�Ǝq�̈ʒu
	D3DXVECTOR3 * GetPartsPos(int const nModelID);
	// �e�Ǝq�̍s��
	D3DXMATRIX *GetMatrix(int const nModelID);
	// ���
	STATE GetState(void) const								{ return m_State; };
	// �L�����N�^�[
	CHARACTER	GetCharacter(void) const					{ return m_character; };
	// �O�������x�N�g���擾
	D3DXVECTOR3 GetDirectionVec(void)						{ return m_Directvector; };
	// ���̍���
	bool GetFloorHeight(void);
	// ���[�V�����̃t���[�����擾����
	int GetMaxFrame(
		CHARACTER character,					// �L�����N�^�[
		int nMotionID,							// ���[�V����ID
		int nNowKeyCnt = -1						// ���݂̃L�[�J�E���g
	);
	// �o�����Ă���l���擾
	int GetAllCharacter(void) { return m_nAllCharacter; };
	// �L�����N�^�[�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �L�����N�^�[�̏��ǂݍ���
	static HRESULT Load_Character(void);
	// �L�����N�^�[�S�\�[�X�̊J��
	static void UnLoad(void);
	// �L�����N�^�[�̐ÓI�ϐ��̏�����
	static void InitStatic(void);
	// �ڕW��]�ʐݒ�
	void SetRotDest(D3DXVECTOR3 const &rotDest)		{ m_rotLast = rotDest; };
	// �ڕW��]�ʎ擾
	D3DXVECTOR3 GetRotDest(void) const				{ return m_rotLast; };
#ifdef _DEBUG
	virtual void  Debug(void);
	static void AllDebug(void);
#endif // _DEBUG
protected:
	/* �֐� */
	// �ݒ� //
	// �L�����N�^�[
	void SetCharacter(CHARACTER const character)	{ m_character = character; };
	// ���[�V�����ݒ�
	void SetMotion(int const nMotiontype);
	// �������[�V�����ݒ�
	void ComplusionSetMotion(int const nMotiontype);
	// �d��
	void FagGravity(void);
	// �����x�̐ݒ�
	static void SetAlpha(float fAlpha)				{ m_fAlpha = fAlpha; }
	// �ݒ� //
	// ���݂̃��[�V����
	int GetMotion(void) const						{ return m_nMotiontype; };
	// �ߋ��̃��[�V����
	int GetOldMotion(void) const					{ return m_nMotiontypeOld; };
	// �L�[�J�E���g
	int GetKeyInfoCnt(void) const					{ return m_keyinfoCnt; };
	// ���[�V�����J�����̍X�V
	void MotionCamera(void);
	/* �ϐ� */
	D3DXVECTOR3									m_pos;							// �ʒu
	D3DXVECTOR3									m_move;							// �ړ���
	D3DXVECTOR3									m_rot;							// ���݉�]��
	// ��
	STATE										m_State;						// ����̃X�e�[�^�X
	int											m_nCntState;					// �J�E���g�X�e�[�^�X



	/* �v���g�^�C�v�p */
	void Limit(void);										// �������
private:
	/* �֐� */
	void Update_Normal(void);								// �ʏ펞�̍X�V
	void NextKeyMotion(void);								// ���̃��[�V����
	void Move(void);										// �ړ�
	void Motion(void);										// ���[�V����
	void ModelUpdate(void);									// ���f���̍X�V
	void Motion_Effect(void);								// ���[�V�����G�t�F�N�g
	void Motion_Obit(void);									// ���[�V�����O��
	/* �ϐ� */
	/* �\���̂̃X�^�e�B�b�N�ɂ��� */
	static MODEL_ALL							*m_modelAll[CHARACTER_MAX];		// ���f���S�̂̏��
	static std::vector<int>						m_modelId[CHARACTER_MAX];		// ���f���ԍ�
	static CModel_info							*m_model_info[CHARACTER_MAX];	// ���f�����
	static D3DXVECTOR3							m_CharacterSize[CHARACTER_MAX];	// �L�����N�^�[�̃T�C�Y
	static int									m_NumModel[CHARACTER_MAX];		// �ő僂�f����
	static int									m_NumParts[CHARACTER_MAX];		// ���������f����
	static int									m_nAllCharacter;				// �o�����Ă���L�����N�^�[�l��
	static float								m_fAlpha;						// �A���t�@�l
	CMeshobit									*m_pMeshobit;					// �O��
	CModel 										*m_pModel;						// ���f��
	CHARACTER									m_character;					// �L�����N�^�[
	D3DXMATRIX									m_mtxWorld;						// �s��
	D3DXVECTOR3									m_posold;						// �O�̈ʒu
	D3DXVECTOR3									m_rotLast;						// ������������
	D3DXVECTOR3									m_rotbetween;					// ��]�̍���
	int											m_nMotiontype;					// ���[�V�����^�C�v
	int											m_nMotiontypeOld;				// �O��̃��[�V�����^�C�v
	int											m_nMaxMotion;					// �ő僂�[�V������
	int											m_keyinfoCnt;					// �L�[���̃J�E���g
	int											m_nFrame;						// �t���[���J�E���g
	int											m_nMotionFrame;					// ��̃��[�V�����̃J�E���g
	float										m_fLength;						// �U���̓�����͈�
	bool										m_bJumpable;					// �W�����v�\���ǂ���
	bool										m_bDie;							// ���S�t���O
	D3DXVECTOR3									m_Directvector;					// �����̃x�N�g��
	std::vector<std::unique_ptr<CMeshobit>>		m_vec_pMeshObit;				// ���
	CStencilshadow								* m_pStencilshadow;				// �X�e���V���V���h�E
};

#endif