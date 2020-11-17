// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�V�[�������̐���[scene_three.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _SCENE_THREE_H_
#define _SCENE_THREE_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_THREE : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		OFFSET_TYPE_VERTICAL_CENTER = 0,	// �I�t�Z�b�g�c�̒��S
		OFFSET_TYPE_VERTICAL_LEFT,			// �I�t�Z�b�g�c�̍�����
		OFFSET_TYPE_VERTICAL_UNDER,			// �I�t�Z�b�g�c�̉�����
		OFFSET_TYPE_SIDE_CENTER,			// �I�t�Z�b�g���̒��S����
		OFFSET_TYPE_MAX
	} OFFSET_TYPE;
	/* �֐� */
	CScene_THREE();
	virtual ~CScene_THREE();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
#ifdef _DEBUG
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
	// �ݒ�
	// ���_���W
	void Set_Vtx_Pos(
		OFFSET_TYPE const &offsettype = OFFSET_TYPE_VERTICAL_CENTER,	// �I�t�Z�b�g�^�C�v
		VERTEX_3D * pVtx = NULL											// ���_���
	);
	// ���_�J���[
	void Set_Vtx_Col(VERTEX_3D * pVtx = NULL);
	// �T�C�Y
	void SetSize(D3DXVECTOR3 const &size)	{ m_size = size; };
	// �ʒu
	void SetPos(D3DXVECTOR3 const &pos)		{ m_pos = pos; };
	// ��]
	void SetRot(D3DXVECTOR3 const &rot)		{ m_rot = rot; };
	// �F
	void SetCol(D3DXCOLOR const &col)		{ m_col = col; };
	// �e�N�X�`���[
	void SetTex(
		D3DXVECTOR2 const &first,									// �ŒZ�l
		D3DXVECTOR2 const &last										// �Œ��l
	);
	// �e�N�X�`���[�^�C�v
	void SetTexType(
		int const & nTexType
	)
	{
		m_nTexType = nTexType;
	};
	// �A�j���[�V�����ݒ�
	void SetAnimation(
		float fTexX,
		float fTexY,
		float fTexY2,
		int nPatternAnim
	);
	// �r���{�[�h���
	void SetBillboard(bool const &bBillboard) { m_bBillboard = bBillboard; };
	// Z�����
	void SetZEneble(bool const &bEneble) { m_bZEneble = bEneble; };
	// ���C�e�B���O���
	void SetLighting(bool const &bLight) { m_bLighting = bLight; };
	// ���u�����h���
	void SetAlphaBlend(bool const &bAlpha) { m_bAlpha = bAlpha; };
	// �I�t�Z�b�g�^�C�v
	void SetOffsetType(OFFSET_TYPE const &type) { m_offsetType = type; };
	// �擾
	// �T�C�Y
	D3DXVECTOR3 GetSize(void) const { return m_size; };
	// �ʒu
	D3DXVECTOR3 &GetPos(void)		{ return m_pos; };
	// ��]
	D3DXVECTOR3 GetRot(void) const	{ return m_rot; };
	// �e�N�X�`���[�^�C�v
	int const &GetTexType(void) const { return m_nTexType; };
	// �V�[���Ǘ�
	static CScene_THREE * Create(
		OFFSET_TYPE	const & type,									// �^�C�v
		D3DXVECTOR3 const & pos,									// �ʒu
		D3DXVECTOR3 const & size,									// �T�C�Y
		int			const & nTexType,								// �e�N�X�`���[�^�C�v
		D3DXVECTOR3	const & rot = D3DVECTOR3_ZERO,					// �p�x
		bool		const & bBillboard = false,						// �r���{�[�h
		bool		const & bZEneble = false,						// Z�o�b�t�@
		bool		const & bLighting = false,						// ���C�e�B���O
		bool		const & bAlpha = false,							// �A���t�@�u�����h
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// �F
	);

	// �l�Ǘ�
	static CScene_THREE * Create_Self(
		OFFSET_TYPE	const & type,									// �^�C�v
		D3DXVECTOR3 const & pos,									// �ʒu
		D3DXVECTOR3 const & size,									// �T�C�Y
		int			const & nTexType,								// �e�N�X�`���[�^�C�v
		D3DXVECTOR3	const & rot = D3DVECTOR3_ZERO,					// �p�x
		bool		const & bBillboard = false,						// �r���{�[�h
		bool		const & bZEneble = false,						// Z�o�b�t�@
		bool		const & bLighting = false,						// ���C�e�B���O
		bool		const & bAlpha = false,							// �A���t�@�u�����h
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// �F
	);
	// �l�Ǘ�
	static std::unique_ptr<CScene_THREE> Create_Uni(
		OFFSET_TYPE	const & type,									// �^�C�v
		D3DXVECTOR3 const & pos,									// �ʒu
		D3DXVECTOR3 const & size,									// �T�C�Y
		int			const & nTexType,								// �e�N�X�`���[�^�C�v
		D3DXVECTOR3	const & rot = D3DVECTOR3_ZERO,					// �p�x
		bool		const & bBillboard = false,						// �r���{�[�h
		bool		const & bZEneble = false,						// Z�o�b�t�@
		bool		const & bLighting = false,						// ���C�e�B���O
		bool		const & bAlpha = false,							// �A���t�@�u�����h
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// �F
	);

	float GetHeight(D3DXVECTOR3 pos);								// �����擾
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
protected:
private:
	/* �֐� */
	void Offset_Virtical_Center(VERTEX_3D *pVtx);					// �I�t�Z�b�g���c�̃Z���^�[
	void Offset_Virtical_Left(VERTEX_3D *pVtx);						// �I�t�Z�b�g���c�̍�
	void Offset_Virtical_Under(VERTEX_3D *pVtx);					// �I�t�Z�b�g���c�̉�
	void Offset_Side_Center(VERTEX_3D *pVtx);						// �I�t�Z�b�g�����̃Z���^�[
	/* �ϐ� */
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;												// ���S���W
	D3DXVECTOR3 m_OriginBlock;										// �����z�u
	D3DXVECTOR3 m_size;												// �T�C�Y
	D3DXVECTOR3 m_rot;												// �p�x
	D3DXMATRIX  m_mtxWorld;											// ���[���h�}�g���b�N�X
	D3DXCOLOR	m_col;												// �J���[
	OFFSET_TYPE	m_offsetType;										// �I�t�Z�b�g�^�C�v
	float m_fLengh;													// ����
	float m_fAngle;													// �p�x
	bool m_bBillboard;												// �r���{�[�h���
	bool m_bZEneble;												// Z�����Ȃ��ɂ��邩�ǂ���
	bool m_bLighting;												// ���C�e�B���O�I���I�t
	bool m_bAlpha;													// ���u�����h
	int m_nTexType;													// �e�N�X�`���[�^�C�v
};

#endif