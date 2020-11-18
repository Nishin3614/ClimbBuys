//-----------------------------------------------------------------------------
//
// ���b�V���X�t�B�A���� [meshsphere.h]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define RADIUS_SIZE		(450.0f)
#define SEPARATE		(15)

//-----------------------------------------------------------------------------
// �N���X��`
//-----------------------------------------------------------------------------
class CMeshsphere :public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TEXTYPE_NONE = 0,
		TEXTYPE_WORLD,
		TEXTYPE_STAR,
		TEXTYPE_MAX
	} TEXTYPE;
	/* �\���� */
	typedef struct MESHSPHERE
	{
		MESHSPHERE()
		{
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
			col = D3DXCOLOR_INI;
			fRadius = 0;
			fRot = 0;
			fRot2 = 0;
			nHeightBlock = 0;
			nWidthBlock = 0;
			nMaxVtx = 0;
			nMaxIndex = 0;
			nMaxPolygon = 0;
			nMaxFrame = 0;
			nCntFrame = 0;
			bUse = true;
			Textype = TEXTYPE_WORLD;
			pVtxBuffMeshSphere = NULL;
			pIdxBuffMeshSphere = NULL;
		}
		D3DXVECTOR3		pos;								// ���W
		D3DXVECTOR3		rot;								// ��]
		D3DXCOLOR		col;								// �F
		D3DXMATRIX		mtxWorldMeshSphere;					// ���[���h�}�g���b�N�X
		float			fRadius;							// ���a
		float			fRot;								// �e���_�̊p�x
		float			fRot2;								// �e���_�̊p�x
		int				nHeightBlock;						// �����̕�����
		int				nWidthBlock;						// ���̒��_�̌�
		int				nMaxVtx;							// ���_�̌�
		int				nMaxIndex;							// �C���f�b�N�X�̌�
		int				nMaxPolygon;						// �|���S���̖���
		int				nMaxFrame;							// �ő�t���[����
		int				nCntFrame;							// �t���[����
		bool			bUse;								// �g�p���
		TEXTYPE			Textype;							//  �e�N�X�`���[�^�C�v
		LPDIRECT3DVERTEXBUFFER9	pVtxBuffMeshSphere;			// ���_�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshSphere;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	}	MESHSPHERE;
	/* �֐� */
	CMeshsphere();
	~CMeshsphere();
	// ����(�V�[���Ǘ�)
	static CMeshsphere *Create(
		D3DXVECTOR3 const &pos,
		float const &fRadius,
		int const &nWidthBlock = 10,
		int const &nHeightBlock = 10,
		D3DXCOLOR const &col = D3DXCOLOR_INI,
		TEXTYPE	const &textype = TEXTYPE_WORLD,
		CScene::LAYER const &layer = CScene::LAYER_WORLD,
		int const &nMaxFrame = 0
	);
	// ����(�l�Ǘ�)
	static CMeshsphere * Create_Self(
		D3DXVECTOR3 const &pos,
		float const &fRadius,
		int const &nWidthBlock = 10,
		int const &nHeightBlock = 10,
		D3DXCOLOR const &col = D3DXCOLOR_INI,
		TEXTYPE	const &textype = TEXTYPE_WORLD,
		int const &nMaxFrame = 0
	);
	// �쐬����(�l_unique�Ǘ�)
	static std::unique_ptr<CMeshsphere> Create_Unique(
		D3DXVECTOR3 const &pos,
		float const &fRadius,
		int const &nWidthBlock = 10,
		int const &nHeightBlock = 10,
		D3DXCOLOR const &col = D3DXCOLOR_INI,
		TEXTYPE	const &textype = TEXTYPE_WORLD,
		int const &nMaxFrame = 0
	);
	void Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��
#ifdef _DEBUG
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_MeshSphere.pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	static HRESULT Load(void);	// �ǂݍ���
	static void Unload(void);	// �j��
	// �擾 //
	bool const &GetUse(void) { return m_MeshSphere.bUse; };
	// �ݒ� //
	// �ʒu�ݒ�
	void SetPosition(D3DXVECTOR3 const &pos);
	// ���a�ݒ�
	void SetRadius(float const &fRadius);
	// �g�p��Ԑݒ�
	void SetUse(bool const &bUse) { m_MeshSphere.bUse = bUse; };
	// �ݒ�
	void Set(
		D3DXVECTOR3 const & pos,
		int const & nMaxFrame,
		bool const & bUse
	);
	// �F�̐ݒ�
	void SetCol(D3DXCOLOR const &col);
	// ���_�J���[�̐ݒ�
	void Set_Vtx_Col(void);

private:
	/* �֐� */
	void FrameDelete(void);	// ���Ԃɂ���ď�����
	/* �ϐ� */
	static int m_nTexId[TEXTYPE_MAX];	// �e�N�X�`���[ID
	MESHSPHERE	m_MeshSphere;			// ���\����
};

#endif
