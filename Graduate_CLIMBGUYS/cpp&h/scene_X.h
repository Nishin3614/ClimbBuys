// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// X�V�[�������̐���[scene_X.h]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _SCENE_X_H_
#define _SCENE_X_H_	 // �t�@�C������������߂�

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model_info.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���錾
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_THREE;		// �V�[��3D�N���X
class CStencilshadow;	// �X�e���V���V���h�E�N���X

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_X : public CScene
{
public:
	/* �񋓌^ */
	// ���f���^�C�v
	typedef enum
	{
		TYPE_NONE = -1,					// NONE
		TYPE_CASTLE,					// ��H
		TYPE_PLAYER,					// �v���C���[�H
		TYPE_BLOCK,						// �u���b�N
		TYPE_STAND,						// ����
		TYPE_PLAYER00_ASTOLFO,			// �v���C���[00 �A�X�g���t�H
		TYPE_PLAYER01_TANJIRO,			// �v���C���[01 ���񂶂낤
		TYPE_PLAYER00_WAIST,			// �v���C���[00 ��
		TYPE_PLAYER00_BODY,				// �v���C���[00 ����
		TYPE_PLAYER00_HEAD,				// �v���C���[00 ��
		TYPE_PLAYER00_HAND_RIGHT,		// �v���C���[00 �E��
		TYPE_PLAYER00_HAND_LEFT,		// �v���C���[00 ����
		TYPE_PLAYER00_THIGH_RIGHT,		// �v���C���[00 �E��
		TYPE_PLAYER00_LEG_RIGHT,		// �v���C���[00 �E��
		TYPE_PLAYER00_THIGH_LEFT,		// �v���C���[00 ����
		TYPE_PLAYER00_LEG_LEFT,			// �v���C���[00 ����
		TYPE_PLAYER01_WAIST,			// �v���C���[01 ��
		TYPE_PLAYER01_BODY,				// �v���C���[01 ����
		TYPE_PLAYER01_HEAD,				// �v���C���[01 ��
		TYPE_PLAYER01_HAND_RIGHT,		// �v���C���[01 �E��
		TYPE_PLAYER01_HAND_LEFT,		// �v���C���[01 ����
		TYPE_PLAYER01_THIGH_RIGHT,		// �v���C���[01 �E��
		TYPE_PLAYER01_LEG_RIGHT,		// �v���C���[01 �E��
		TYPE_PLAYER01_THIGH_LEFT,		// �v���C���[01 ����
		TYPE_PLAYER01_LEG_LEFT,			// �v���C���[01 ����
		TYPE_PLAYER02_WAIST,			// �v���C���[02 ��
		TYPE_PLAYER02_BODY,				// �v���C���[02 ����
		TYPE_PLAYER02_HEAD,				// �v���C���[02 ��
		TYPE_PLAYER02_HAND_RIGHT,		// �v���C���[02 �E��
		TYPE_PLAYER02_HAND_LEFT,		// �v���C���[02 ����
		TYPE_PLAYER02_THIGH_RIGHT,		// �v���C���[02 �E��
		TYPE_PLAYER02_LEG_RIGHT,		// �v���C���[02 �E��
		TYPE_PLAYER02_THIGH_LEFT,		// �v���C���[02 ����
		TYPE_PLAYER02_LEG_LEFT,			// �v���C���[02 ����
		TYPE_PLAYER03_WAIST,			// �v���C���[03 ��
		TYPE_PLAYER03_BODY,				// �v���C���[03 ����
		TYPE_PLAYER03_HEAD,				// �v���C���[03 ��
		TYPE_PLAYER03_HAND_RIGHT,		// �v���C���[03 �E��
		TYPE_PLAYER03_HAND_LEFT,		// �v���C���[03 ����
		TYPE_PLAYER03_THIGH_RIGHT,		// �v���C���[03 �E��
		TYPE_PLAYER03_LEG_RIGHT,		// �v���C���[03 �E��
		TYPE_PLAYER03_THIGH_LEFT,		// �v���C���[03 ����
		TYPE_PLAYER03_LEG_LEFT,			// �v���C���[03 ����
		TYPE_TUTORIALBLOCK,				// �`���[�g���A���u���b�N
		TYPE_OBJECT_MAP,				// �}�b�v
		TYPE_OBJECT_BLOCK_C,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_L,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_I,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_M,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_B,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_G,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_U,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_Y,			// �����蔻�薳�����o�p�u���b�N
		TYPE_OBJECT_BLOCK_S,			// �����蔻�薳�����o�p�u���b�N
		TYPE_BLOCK_SPRING,				// ����u���b�N_�o�l
		TYPE_BLOCK_BOMB,				// ����u���b�N_�{��
		TYPE_OBJECT_VOLCANO,			// �ΎR
		TYPE_BLOCKS_INVERTER,			// �d�C�u���b�N
		TYPE_BLOCKS_HARD,				// �d���u���b�N
		TYPE_BLOCKS_HATENA,				// �n�e�i�u���b�N
		TYPE_MAX						// �ő吔
	} TYPE;

	/* �\���� */
	// ���f�����
	typedef struct MODEL_LOAD
	{
		// �R���X�g���N�^
		MODEL_LOAD()
		{
			pMesh = NULL;
			pBuffMat = NULL;
			nNumMat = 0;
			vtxMinMaterials = D3DVECTOR3_ZERO;
			vtxMaxMaterials = D3DVECTOR3_ZERO;
			size = D3DVECTOR3_ZERO;
		}
		LPD3DXMESH			pMesh;						// ���b�V�����ւ̃|�C���^
		LPD3DXBUFFER		pBuffMat;					// �}�e���A�����ւ̃|�C���^
		std::vector<LPDIRECT3DTEXTURE9> vec_pTexture;	// �e�N�X�`���[���
		std::vector<D3DCOLORVALUE>		vec_Diffuse;	// ���̃J���[���
		DWORD				nNumMat;					// �}�e���A���̐�
		D3DXVECTOR3			vtxMinMaterials;			// ���f�����̈ʒu�̍ŏ��l
		D3DXVECTOR3			vtxMaxMaterials;			// ���f�����̈ʒu�̍ő�l
		D3DXVECTOR3			size;						// �T�C�Y
	} MODEL_LOAD;

	/* �֐� */
	// �R���X�g���N�^
	CScene_X();
	// �f�X�g���N�^
	~CScene_X();
	// ����������
	void Init(void);
	// �I������
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
#ifdef _DEBUG
	// �f�o�b�O����
	void Debug(void);
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

	// �����蔻��������I�ɍ폜
	void CollisionDelete(void);
	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	// �쐬����(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	rot			: ��]
	//	nModelId	: ���f���ԍ�
	//	bShadowMap	: �V���h�E�}�b�s���O���
	static CScene_X * Create(
		D3DXVECTOR3 const &pos,						// �ʒu
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,	// ��]
		int const &nModelId = 0,					// ���f���ԍ�
		bool const &bShadowMap = false				// �V���h�E�}�b�s���O���
	);

	// �쐬����(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	rot			: ��]
	//	size		: �T�C�Y�{�� 1.0���W��
	//	nModelId	: ���f���ԍ�
	//	bShadowMap	: �V���h�E�}�b�s���O���
	static CScene_X * Create(
		D3DXVECTOR3 const &pos,						// �ʒu
		D3DXVECTOR3 const &rot,						// ��]
		D3DXVECTOR3 const &size,					// �T�C�Y�{��
		D3DXCOLOR color,							// �J���[
		int const &nModelId = 0,					// ���f���ԍ�
		bool const &bShadowMap = false				// �V���h�E�}�b�s���O���
	);

	// �쐬����(�l�Ǘ�)
	//	pos			: �ʒu
	//	rot			: ��]
	//	nModelId	: ���f���ԍ�
	//	bShadowMap	: �V���h�E�}�b�s���O���
	static CScene_X * Create_Self(
		D3DXVECTOR3 const &pos,						// �ʒu
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,	// ��]
		int const &nModelId = 0,					// ���f���ԍ�
		bool const &bShadowMap = false				// �V���h�E�}�b�s���O���
	);
	// �쐬����(�l�Ǘ�)
	//	pos			: �ʒu
	//	rot			: ��]
	//	nModelId	: ���f���ԍ�
	//	bShadowMap	: �V���h�E�}�b�s���O���
	static std::unique_ptr<CScene_X> Create_Uni(
		D3DXVECTOR3 const &pos,						// �ʒu
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,	// ��]
		int const &nModelId = 0,					// ���f���ԍ�
		bool const &bShadowMap = false				// �V���h�E�}�b�s���O���
	);

	// ���f�����̓ǂݍ���
	static HRESULT LoadModel(void);
	// ���f�����̊J��
	static HRESULT UnLoadModel(void);

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
	) {
		return COLLISIONDIRECTION::NONE;
	};
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
	) {
		return COLLISIONDIRECTION::NONE;
	};

	// �ݒ�
	// �ʒu�ݒ�
	void SetPos(D3DXVECTOR3 const &pos)			{ m_pos = pos; };
	// ��]�ݒ�
	void SetRot(D3DXVECTOR3 const &rot)			{ m_rot = rot; };
	// �T�C�Y�ݒ�
	void SetSize(D3DXVECTOR3 const &size)		{ m_size = size; };
	// ���f���ԍ��ݒ�
	void SetModelId(int const &nModelId)		{ m_nModelId = nModelId; };
	// �V���h�E�}�b�s���O�ݒ�
	void SetShadowMap(bool const &bShadow)		{ m_bShadowMap = bShadow; };
	// �e�̍s��ݒ�
	void SetParentMtx(D3DXMATRIX * ParentMax)	{ m_pParentMtx = ParentMax; };
	// ���f���̃��l�ݒ�
	void SetModelAlpha(float fModelAlpha)		{ m_fModelAlpha = fModelAlpha; };
	// �X�e���V���V���h�E�ݒ�
	//	Size	: �T�C�Y
	//	Type	: �^�C�v
	void SetStencilshadow(
		D3DXVECTOR3 const & Size,			// �T�C�Y
		int const & nType	// �^�C�v
	);
	// �X�e���V���V���h�E�̎g�p��Ԑݒ�
	void SetUseStencillShadow(bool const & bUse);
	// �X�e���V���V���h�E�̎g�p��Ԃ��擾
	bool GetUseStencillShadow(void);
	// �����蔻��ݒ�
	//	nShapeType	: 0:��`�A1:���A2:�~��
	//	Obj			: �I�u�W�F�N�g�^�C�v
	//	bPush		: �����o������
	//	pParent		: �e���
	//	Offset_pos	: �I�t�Z�b�g�ʒu
	void SetCollision(
		int const &nShapeType,
		int const &Obj,
		bool const &bPush = false,
		bool const &bOpponent = false,
		CScene * pParent = NULL,
		D3DXVECTOR3 const &Offset_pos = D3DVECTOR3_ZERO
	);									// �����蔻��ݒ�
	// ���f���J���[���̐ݒ�
	//	col	: �J���[
	void SetModelColor(
		D3DXCOLOR const &Col	// �J���[
	);
	// �擾
	// ���f�����擾
	CScene_X::MODEL_LOAD * GetModel(void);
	// �ʒu�擾
	D3DXVECTOR3 &GetPos(void) { return m_pos; };
	// ��]�擾
	D3DXVECTOR3 &GetRot(void) { return m_rot; };
	// �T�C�Y�擾
	D3DXVECTOR3 &GetSize(void) { return m_size; };
	// �s��擾
	D3DXMATRIX &GetMatrix(void) { return m_mtxWorld; };
	// �X�e���V���V���h�E���擾
	CStencilshadow * GetStencillShadow(void) { return m_pStencilshadow; };
	// �����蔻���Ԏ擾
	bool const GetbCollision(void);
	// ���f�����̎擾
	static MODEL_LOAD * GetModelLoad(int const &nModelId);
	// ���f��id�̎擾
	int GetModelId() { return m_nModelId; };
protected:
private:
	/* �֐� */
	// ���f���ݒ�
	static void ModelSetting(MODEL_LOAD * pModel_load);
	/* �ϐ� */
	static std::vector<std::unique_ptr<MODEL_LOAD>> m_pModelLoad;		// ���f�����̓ǂݍ��ݗp
	D3DXVECTOR3										m_pos;				// �ʒu���
	D3DXVECTOR3										m_rot;				// ��]���
	D3DXVECTOR3										m_size;				// �T�C�Y���
	D3DXMATRIX										*m_pParentMtx;		// �e�}�g���b�N�X
	D3DXMATRIX										m_mtxWorld;			// ���[���h�}�g���b�N�X
	int												m_nModelId;			// ���f���ԍ�
	bool											m_bShadowMap;		// �V���h�E�}�b�s���O�ɂ��邩���Ȃ���
	float											m_fShadowAlpha;		// �e�̃��l
	float											m_fModelAlpha;		// ���f���̃A���t�@�l
	D3DXCOLOR										* m_pModelCol;		// ���f���J���[���
	CScene_THREE									* m_pShadow;		// �܂�e
	CStencilshadow									* m_pStencilshadow;	// �X�e���V���V���h�E
};

#endif