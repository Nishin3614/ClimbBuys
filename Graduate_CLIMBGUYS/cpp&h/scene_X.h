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
class CCollision;		// �����蔻��N���X
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
	CScene_X::MODEL_LOAD * GetModel(int const & nModelId);
	// �ʒu�擾
	D3DXVECTOR3 &GetPos(void) { return m_pos; };
	// ��]�擾
	D3DXVECTOR3 &GetRot(void) { return m_rot; };
	// �T�C�Y�擾
	D3DXVECTOR3 &GetSize(void) { return m_size; };
	// �s��擾
	D3DXMATRIX &GetMatrix(void) { return m_mtxWorld; };
	// �����蔻���Ԏ擾
	bool const GetbCollision(void);
	// �����蔻��̏����擾
	CCollision * GetCollision(void);
	// ���f�����̎擾
	static MODEL_LOAD * GetModelLoad(int const &nModelId);
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
	CCollision										* m_Collision;		// �����蔻��
	CStencilshadow									* m_pStencilshadow;	// �X�e���V���V���h�E
};

#endif