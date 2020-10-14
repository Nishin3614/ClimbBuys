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
class CExtrusion;
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
		MOTIONTYPE_MAX
	} MOTIONTYPE;
	// �L�����N�^�[
	typedef enum
	{
		CHARACTER_NPC = 0,	// NPC
		CHARACTER_MAX,							// �L�����N�^�[�S�̐�
	} CHARACTER;

	/* �\���� */
	// �X�e�[�^�X���
	typedef struct STATUS
	{
		/* �֐� */
		// ������
		STATUS()
		{
			nMaxBalloon = 0;			// �ő啗�D��
			nMaxPopBalloon = 0;			// �ő�o�����D��
			fMaxInertia = 0;			// ������
			fMaxJump = 0;				// �W�����v��
			fMaxMove = 0;				// �ړ���
			nMaxMp = 0;					// �ő�MP��
			nMaxMpUp_Every = 0;			// �}�C�t���[���ő�����MP��
			nMaxMpUp_KnockDown = 0;		// �|�����Ƃ���MP��
			nMaxMpDown = 0;				// �X�L���g�p���̌����
			fMaxSkill = 0;				// �\�͂̔{��
		}
		/* �ϐ� */
		int			nMaxBalloon;		// �ő啗�D��
		int			nMaxPopBalloon;		// �ő�o�����D��
		float		fMaxInertia;		// ������
		float		fMaxJump;			// �W�����v��
		float		fMaxMove;			// �ړ���
		int			nMaxMp;				// �ő�MP��
		int			nMaxMpUp_Every;		// �}�C�t���[���ő�����MP��
		int			nMaxMpUp_KnockDown;	// �|�����Ƃ���MP��
		int			nMaxMpDown;			// �X�L���g�p���̌����
		float		fMaxSkill;			// �\�͂̔{��
		// �|�C���g�]�[�� �|�C���g���Z(100)
		// �X�R�A���Ƃ������̃Q�[�W�̏㏸��
		// �|�C���g�̔{��
	} STATUS, *PSTATUS;
	/* �֐� */
	CCharacter(CHARACTER const &character);
	virtual ~CCharacter();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
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
	// �K�v�ɉ��������� //
	// �d��
	void AddGravity(float const &fGravity)					{ m_move.y -= fGravity; };
	// �ݒ� //
	// �ʒu
	void SetPos(D3DXVECTOR3 const &pos)						{ m_pos = pos; };
	// �ړ���
	void SetMove(D3DXVECTOR3 const &move)					{ m_move = move; };
	// ��]
	void SetRot(D3DXVECTOR3 const &rot)						{ m_rot = rot; };
	// ���n��Ԑݒ�
	void SetbLanding(bool const &bLanding)					{ m_bLanding = bLanding; };
	// �ړ���Ԑݒ�
	void SetbMove(bool const &bMove)						{ m_bMove = bMove; };
	// �����x�N�g���ݒ�
	void SetDirectionVec(D3DXVECTOR3 const &direct)			{ m_Directvector = direct; };
	// �擾 //
	// �ʒu
	D3DXVECTOR3 &GetPos(void)								{ return m_pos; };
	// �ړ���
	D3DXVECTOR3 &GetMove(void)								{ return m_move; };
	// ��]
	D3DXVECTOR3 &GetRot(void)								{ return m_rot; };
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
	// �X�e�[�^�X(�L�����N�^�[�̃N���X�����N���X�Ɏ����Ă���N���X)
	STATUS &GetStatus(void) { return m_sStatus[m_character]; };
	// �X�e�[�^�X(�L�����N�^�[��)
	// ����:�L�����N�^�[�ԍ�
	static STATUS &GetStatus(CHARACTER const &character)	{ return m_sStatus[character]; };
	// ���n��Ԏ擾
	bool GetbLanding(void)									{ return m_bLanding; };
	// �ړ���Ԏ擾
	bool GetbMove(void)										{ return m_bMove; };
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
	// �J�����ǔ����Ă���ID
	static int GetCameraCharacter(void);
	// �L�����N�^�[�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �L�����N�^�[�̏��ǂݍ���
	static HRESULT Load_Character(void);
	// �X�e�[�^�X���ǂݍ���
	static HRESULT LoadStatus(void);
	// �L�����N�^�[�S�\�[�X�̊J��
	static void UnLoad(void);
	// �L�����N�^�[�̐ÓI�ϐ��̏�����
	static void InitStatic(void);
	// �ڕW��]�ʐݒ�
	void SetRotDest(D3DXVECTOR3 const &rotDest) { m_rotLast = rotDest; };
	// �ڕW��]�ʎ擾
	D3DXVECTOR3 GetRotDest(void) const { return m_rotLast; };
#ifdef _DEBUG
	virtual void  Debug(void);
	static void AllDebug(void);
#endif // _DEBUG
	CCollision * GetCollision(void) { return m_pCharacterCollision; };
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
	void FagGravity(void);						// �d��
	// �����x�̐ݒ�
	static void SetAlpha(float fAlpha) { m_fAlpha = fAlpha; }
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
	static int						m_nCameraCharacter;		// �L�����N�^�[�ɒǔ�����ID
	static STATUS					m_sStatus[CHARACTER_MAX];		// �L�����N�^�[���ׂẴX�^�[�^�X���
	D3DXVECTOR3						m_pos;					// �ʒu
	D3DXVECTOR3						m_move;					// �ړ���
	D3DXVECTOR3						m_rot;					// ���݉�]��

	// ��
	STATE							m_State;				// ����̃X�e�[�^�X
	int								m_nCntState;			// �J�E���g�X�e�[�^�X



	/* �v���g�^�C�v�p */
	void Limit(void);										// �������
private:
	/* �֐� */
	void Collision(void);									// ���ꂼ��̓����蔻��
	void Update_Normal(void);								// �ʏ펞�̍X�V
	void NextKeyMotion(void);								// ���̃��[�V����
	void Move(void);										// �ړ�
	void Motion(void);										// ���[�V����
	void ModelUpdate(void);									// ���f���̍X�V
	void TrackCamera(void);									// �J�����ǔ�
	void Motion_Effect(void);								// ���[�V�����G�t�F�N�g
	void Motion_Obit(void);									// ���[�V�����O��
	static void Status_Reset(void);								// �X�e�[�^�X���Z�b�g
	/* �ϐ� */
	/* �\���̂̃X�^�e�B�b�N�ɂ��� */
	static MODEL_ALL				*m_modelAll[CHARACTER_MAX];				// ���f���S�̂̏��
	static std::vector<int>				m_modelId[CHARACTER_MAX];			// ���f���ԍ�
	static CModel_info				*m_model_info[CHARACTER_MAX];			// ���f�����
	static D3DXVECTOR3				m_CharacterSize[CHARACTER_MAX];			// �L�����N�^�[�̃T�C�Y
	static int						m_NumModel[CHARACTER_MAX];				// �ő僂�f����
	static int						m_NumParts[CHARACTER_MAX];				// ���������f����
	static int						m_nAllCharacter;						// �o�����Ă���L�����N�^�[�l��
	static float					m_fAlpha;								// �A���t�@�l
	CMeshobit						*m_pMeshobit;							// �O��
	CModel 							*m_pModel;								// ���f��
	CHARACTER						m_character;							// �L�����N�^�[
	D3DXMATRIX						m_mtxWorld;								// �s��
	D3DXVECTOR3						m_posold;								// �O�̈ʒu
	D3DXVECTOR3						m_rotLast;								// ������������
	D3DXVECTOR3						m_rotbetween;							// ��]�̍���
	D3DXVECTOR3						m_size;									// �L�����N�^�[�̃T�C�Y
	int								m_nMotiontype;							// ���[�V�����^�C�v
	int								m_nMotiontypeOld;						// �O��̃��[�V�����^�C�v
	int								m_nMaxMotion;							// �ő僂�[�V������
	int								m_keyinfoCnt;							// �L�[���̃J�E���g
	int								m_nFrame;								// �t���[���J�E���g
	int								m_nMotionFrame;							// ��̃��[�V�����̃J�E���g
	int								m_nIdAttackKey;							// �U���p�̃L�[ID
	int								m_nCntDamage;							// �_���[�W�J�E���g
	float							m_fLength;								// �U���̓�����͈�
	bool							m_bMotionCamera;						// ���[�V�����J�����̐؂�ւ�ON�EOFF
	bool							m_bLanding;								// ���n���
	bool							m_bMove;								// �ړ����
	D3DXVECTOR3						m_Directvector;							// �����̃x�N�g��
	CCollision						*m_pCharacterCollision;					// �L�����N�^�[�̓����蔻��
	std::vector<std::unique_ptr<CCollision>>	m_vec_AttackCollision;		// �U�������蔻��
	std::vector<std::unique_ptr<CMeshobit>>	m_vec_pMeshObit;				// ���
	CStencilshadow					* m_pStencilshadow;						// �X�e���V���V���h�E
};

#endif