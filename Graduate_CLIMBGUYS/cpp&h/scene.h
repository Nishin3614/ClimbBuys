// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �V�[�������̐���[scene.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "manager.h"
#include "texture_manager.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene
{
public:
	/* �񋓌^ */
	// ���̔z�u����
	typedef enum
	{
		LAYER_WORLD = 0,
		LAYER_BG,
		LAYER_3DOBJECT,
		LAYER_3DSTAND,
		LAYER_3DBLOCK,
		LAYER_CHARACTER,
		LAYER_3DBLOCKSHADOW,
		LAYER_3DCHARACTERSHADOW,
		LAYER_FLOOR,
		LAYER_DEBUGCOLLISION,
		LAYER_3DPARTICLE,
		LAYER_3DOBJECT2,
		LAYER_CHARARANK,
		LAYER_DOME,
		LAYER_BGDOME,
		LAYER_PLAYER_UI,
		LAYER_UI,
		LAYER_COLLISION,
		LAYER_PARTICLE,
		LAYER_FADE,
		LAYER_RANK,
		LAYER_MAX
	} LAYER;
	// �I�u�W�F�N�g�^�C�v
	typedef enum
	{
		OBJ_NONE = 0,	// �I�u�W�F�N�g�^�C�v���ݒ肳��Ă��Ȃ�
		OBJ_PLAYER,		// �v���C���[
		OBJ_BLOCK,		// �u���b�N
		OBJ_STAND,		// ����
		OBJ_MAX			// �ő吔
	} OBJ;

	/* �֐� */
	CScene();
	virtual ~CScene();
	virtual void Init(void) = 0;										// ������
	virtual void Uninit(void) = 0;										// �I��
	virtual void Update(void) = 0;										// �X�V
	virtual void Draw(void) = 0;										// �`��
#ifdef _DEBUG
	virtual void Debug(void);											// �f�o�b�O�p
#endif // _DEBUG
	// �������瓖��������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
		) = 0;
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) = 0;
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
	virtual D3DXVECTOR3 * Scene_GetPPos(void) = 0;
	// �|�C���^�[�ߋ��̈ʒu�����擾
	virtual D3DXVECTOR3 * Scene_GetPPosold(void) = 0;
	// �|�C���^�[�ړ��ʏ��̎擾
	virtual D3DXVECTOR3 * Scene_GetPMove(void) = 0;
	static void ReleaseAll(void);										// �V�[���e�q�����[�X
	static void UpdateAll(void);										// �V�[���e�q�X�V
	static void DrawAll(void);											// �V�[���e�q�`��
	// �擾
	// �V�[�����擾(���C���[�E�N���X�w��)
	static CScene * GetScene(
		LAYER const &layer,												// ���C���[
		int const &nCntScene,											// �J�E���g�V�[��
		CScene &														// �N���X�^���
	);
	static CScene * GetScene(
		LAYER const & layer,
		int const & nCntScene,
		char const * cName										// �N���X�^���
	);
	// �V�[�����擾(���C���[�w��)
	static CScene * GetScene(
		LAYER const &layer,												// ���C���[
		int const &nCntScene											// �J�E���g�V�[��
	);
	// ���C���[���̃V�[���ő吔�擾
	static int GetMaxLayer(LAYER const &layer)
	{ return (signed)m_pScene[layer].size(); };
	static bool GetUpdateStop(void) { return m_sta_bStop; };			// �Î~���
	// �ݒ�
	// �V�[���̐Î~
	static void UpdateStop(
		bool const &bStop,												// �Î~���邩���Ȃ���
		int const &nMaxStop = 0											// �ő�Î~����
	);	// �X�V���Ƃ߂�
	// 1F�����X�V����
	static void Set1FUpdate() { m_b1FUpdateFlag = true; };
	// �V�[���Ǘ��ݒ�
	void ManageSetting(LAYER const &layer);
	// �I�u�W�F�N�g�̍X�V�X�g�b�v��Ԑݒ�
	void SetStop(bool const &bStopState)
	{
		m_bStop = bStopState;
	};
	// �I�u�W�F�N�g�̍X�V�X�g�b�v��Ԏ擾
	bool GetStop(void)
	{
		return m_bStop;
	};
	// �I�u�W�F�N�g�̕`���Ԑݒ�
	void SetDrawState(bool const &bDraw)
	{
		m_bDraw = bDraw;
	};
	// �I�u�W�F�N�g�̕`���Ԏ擾
	bool GetDrawState(void)
	{
		return m_bDraw;
	};
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	void SetObj(OBJ const & Obj) { m_Obj = Obj; };
	// �I�u�W�F�N�g�^�C�v�̎擾
	OBJ & GetObj(void) { return m_Obj; };
	// �I�u�W�F�N�g�̊J������
	void Release(void);													// �I�u�W�F�N�g�̔j��
protected:

private:
	/* �֐� */
	static void DeadFragAll(void);						// ���S�t���O�����������̂������[�X
	/* �ϐ� */
	static std::vector<CScene*> m_pScene[LAYER_MAX];	// �V�[���Ǘ��p�ϐ�
	static bool					m_sta_bStop;			// �X�V���~�߂�
	static bool					m_b1FUpdateFlag;		// ��ʒ�~����1F�����X�V����t���O
	static int					m_nMaxStop;				// �ő�Î~����
	static int					m_nCntStop;				// �Î~����
	bool						m_bDeadFrag;			// �t���O
	bool						m_bStop;				// �X�V���~�߂邩�~�߂Ȃ���
	bool						m_bDraw;				// �`����
	OBJ							m_Obj;					// �I�u�W�F�N�g�^�C�v
};

#endif