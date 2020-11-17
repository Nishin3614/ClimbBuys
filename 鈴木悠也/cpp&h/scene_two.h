// ----------------------------------------
//
// 2D�V�[�������̐���[scene_two.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCENE_TWO_H_
#define _SCENE_TWO_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO : public CScene
{
public:
	/* �񋓌^ */
	// �I�t�Z�b�g
	typedef enum
	{
		OFFSET_TYPE_CENTER = 0,
		OFFSET_TYPE_LEFT,
		OFFSET_TYPE_MAX
	} OFFSET_TYPE;
	/* �֐� */
	// �R���X�g���N�^
	CScene_TWO();
	// �f�X�g���N�^
	virtual ~CScene_TWO();
	// ����������
	virtual void Init(void);
	// �I������
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void Debug(void);
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// ���_���W�ݒ�
	//	offsettype	: �I�t�Z�b�g�^�C�v
	//	pVtx		: ���_���
	void Set_Vtx_Pos(
		OFFSET_TYPE const &offsettype = OFFSET_TYPE_CENTER,			// �I�t�Z�b�g�^�C�v
		VERTEX_2D * pVtx = NULL										// ���_���
	);
	// ���_�J���[�ݒ�
	//	pVtx		: ���_���
	void Set_Vtx_Col(VERTEX_2D * pVtx = NULL);
	// ���_�J���[�ݒ�(�E�̐F�ƍ��̐F)
	//	col_r		: �E�̐F
	//	col_l		: ���̐F
	//	pVtx		: ���_���
	void Set_Vtx_Col2_RL(
		D3DXCOLOR const &col_r,		// �E�̐F
		D3DXCOLOR const &col_l,		// ���̐F
		VERTEX_2D * pVtx = NULL);	// 2D���_���
	// ���_�J���[�ݒ�(��̐F�Ɖ��̐F)
	//	col_o		: ��̐F
	//	col_u		: ���̐F
	//	pVtx		: ���_���
	void Set_Vtx_Col2_UD(
		D3DXCOLOR const &col_o,		// ��̐F
		D3DXCOLOR const &col_u,		// ���̐F
		VERTEX_2D * pVtx = NULL);	// 2D���_���
	// �e�N�X�`���[�ݒ�
	//	first		: �����̈ʒu
	//	last		: �����̈ʒu
	//	pVtx		: ���_���
	void Set_Vtx_Tex(
		D3DXVECTOR2 const &first = D3DXVECTOR2(0.0f, 0.0f),	// �����̔z�u
		D3DXVECTOR2 const &last = D3DXVECTOR2(1.0f, 1.0f),	// �����̔z�u
		VERTEX_2D * pVtx = NULL								// ���_���
	);
	// �I�t�Z�b�g�ݒ�
	void SetOffset(OFFSET_TYPE const &type) { m_offsetType = type; };
	// �T�C�Y�ݒ�
	void SetSize(D3DXVECTOR2 const &size) { m_size = size; };
	// �ʒu�ݒ�
	void SetPosition(D3DXVECTOR3 const &pos) { m_pos = pos; };
	// �p�x�ݒ�
	void SetRot(float const &frot) { m_rot = frot; };
	// �F�ݒ�
	void SetCol(
		D3DXCOLOR const &col) { m_col = col; };
	// �A�j���[�V�����ݒ�
	//	nMaxCntAnim			: �ő�A�j���[�V�����J�E���g
	//	nMaxHorizonAnim		: �ő吅���A�j���[�V�����J�E���g
	//	nMaxVirticalAnim	: �ő吂���A�j���[�V�����J�E���g
	//	bLoop				: ���[�v���
	void SetTexAnim(
		int	const &nMaxCntAnim,			// �ő�A�j���[�V�����J�E���g
		int	const &nMaxHorizonAnim,		// �ő吅���A�j���[�V�����J�E���g
		int	const &nMaxVirticalAnim		// �ő吂���A�j���[�V�����J�E���g
	);
	// �e�N�X�`���[�ݒ�
	void BindTexture(int const &nTexId) { m_nTexId = nTexId; };
	// �T�C�Y�擾
	D3DXVECTOR2 const GetSize(void) { return m_size; };
	// �ʒu�擾
	D3DXVECTOR3 const GetPosition(void) { return m_pos; };
	// �F�擾
	D3DXCOLOR const GetCol(void) { return m_col; };
	// �p�x�擾
	float const GetRot(void) { return m_rot; };
	// �����擾
	float const GetLengh(void) { return m_fLengh; };

	// �쐬(�V�[���p������)
	//	offsettype	: �^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	nTextype	: �e�N�X�`���[�^�C�v
	//	frot		: �p�x
	//	col			: �F
	//	layer		: ���C���[
	static CScene_TWO * Create(
		OFFSET_TYPE		const & type,									// �^�C�v
		D3DXVECTOR3		const & pos,									// �ʒu
		D3DXVECTOR2		const & size,									// �T�C�Y
		int				const & nTextype,								// �e�N�X�`���[�^�C�v
		float			const & frot = 0.0f,							// �p�x
		D3DXCOLOR		const & col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),	// �F
		CScene::LAYER	const & layer = CScene::LAYER_UI				// ���C���[
	);
	// �쐬(�V�[���p���Ȃ�)
	//	offsettype	: �^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	nTextype	: �e�N�X�`���[�^�C�v
	//	frot		: �p�x
	//	col			: �F
	static CScene_TWO * Create_Self(
		OFFSET_TYPE	const & type,										// �^�C�v
		D3DXVECTOR3 const & pos,										// �ʒu
		D3DXVECTOR2 const & size,										// �T�C�Y
		int			const & nTextype,									// �e�N�X�`���[�^�C�v
		float		const & frot = 0.0f,								// �p�x
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �F
	);
	// unique_ptr�쐬(�V�[���p���Ȃ�)
	// ���߂�l��std::move�Ŏ󂯎��
	//	offsettype	: �^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	nTextype	: �e�N�X�`���[�^�C�v
	//	frot		: �p�x
	//	col			: �F
	static std::unique_ptr<CScene_TWO> Creat_Unique(
		OFFSET_TYPE const & type,										// �^�C�v
		D3DXVECTOR3 const & pos,										// �ʒu
		D3DXVECTOR2 const & size,										// �T�C�Y
		int			const & nTextype,									// �e�N�X�`���[�^�C�v
		float const & frot,												// �p�x
		D3DXCOLOR const & col											// �F
	);

protected:
	// �ݒ� //
private:
	/* �񋓌^ */

	/* �\���� */

	/* �֐� */
	// �A�j���[�V�����X�V����
	void Updata_Animation(void);
	void Offset_Center(VERTEX_2D *pVtx);	// �I�t�Z�b�g���Z���^�[
	void Offset_Left(VERTEX_2D *pVtx);		// �I�t�Z�b�g����

	/* �ϐ� */
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;						// ���S���W
	D3DXVECTOR2 m_size;						// �T�C�Y
	D3DXCOLOR	m_col;						// �J���[
	OFFSET_TYPE	m_offsetType;				// �I�t�Z�b�g�^�C�v
	std::unique_ptr<ANIMATION> m_unipAnim;	// �A�j���[�V�������
	float m_rot;							// �p�x
	float m_fLengh;							// ����
	float m_fAngle;							// �p�x
	int m_nTexId;							// �e�N�X�`���[�ԍ�
};

#endif