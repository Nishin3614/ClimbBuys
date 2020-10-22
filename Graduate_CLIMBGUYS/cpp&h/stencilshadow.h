// ----------------------------------------
//
// �X�e���V���V���h�E�����̐���[stencilshadow.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_	 // �t�@�C������������߂�

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


// ----------------------------------------
//
// �O���錾
//
// ----------------------------------------
class CScene_TWO;	// �V�[��2D�N���X

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CStencilshadow : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_CYLINDER = 0,	// �~��
		TYPE_RECT,			// ��`
		TYPE_MAX			// �ő吔
	} TYPE;
	/* �֐� */
	// �R���X�g���N�^
	CStencilshadow();
	// �f�X�g���N�^
	~CStencilshadow();
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
	// �X�e���V���`���Ԃ̐ݒ�
	static void SetStencil(bool const &bStencil) { m_bStencil = bStencil; };
	// �X�e���V���`���Ԃ̎擾
	static bool & GetStencil(void) { return m_bStencil; };
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

	static HRESULT Load(void);
	static void UnLoad(void);
	// �쐬����(�V�[���Ǘ�)
	static CStencilshadow * Create(
		D3DXVECTOR3 const &pos,							// �ʒu
		D3DXVECTOR3 const &size,						// �T�C�Y
		TYPE const &type = TYPE_CYLINDER,				// �^�C�v
		int const &nWidth = 10,							// ����
		int const &nDepth = 1,							// �c��
		D3DXCOLOR	const &col = D3DXCOLOR_INI,			// �J���[
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO		// ��]
	);
	// �쐬����(�l�Ǘ�)
	static CStencilshadow * Create_Self(
		D3DXVECTOR3 const &pos,							// �ʒu
		D3DXVECTOR3 const &size,						// �T�C�Y
		TYPE const &type = TYPE_CYLINDER,				// �^�C�v
		int const &nWidth = 10,							// ����
		int const &nDepth = 5,							// �c��
		D3DXCOLOR	const &col = D3DXCOLOR_INI,			// �J���[
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO		// ��]
	);
	// �擾
	// �g�p���
	bool GetUse(void);
	// �ݒ�
	// �ʒu�ݒ�
	void SetPos(D3DXVECTOR3 const &pos);
	// ��]�ݒ�
	void SetRot(D3DXVECTOR3 const &rot);
	// �F�ݒ�
	void SetCol(D3DXCOLOR const &col);
	// �d�l�ݒ�
	void SetUse(bool const bUse);
protected:

private:
	/* �֐� */
	// �~���̐ݒ�
	void SetCylinder(LPDIRECT3DDEVICE9 pDevice);
	// ��`�̐ݒ�
	void SetRect(LPDIRECT3DDEVICE9 pDevice);
	/* �ϐ� */
	static int CStencilshadow::m_nTexId[TYPE_MAX];	// �e�N�X�`���[ID
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIndex;				// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_OriginBlock;						// �����z�u
	D3DXMATRIX  m_mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXVECTOR3 m_size;								// �T�C�Y
	D3DXVECTOR3 m_rot;								// �p�x
	D3DXCOLOR	m_col;								// �F
	int		m_nNumberVertex;						// �����_��
	int		m_nNumIndex;							// ���C���f�b�N�X
	int		m_nNumPolygon;							// ���|���S��
	int		m_nBlock_Depth;							// �c�u���b�N��
	int		m_nBlock_Width;							// ���u���b�N��
	bool	m_bUse;									// �g�p
	TYPE	m_type;									// �^�C�v
	std::unique_ptr<CScene_TWO> m_pSceneTwo;		// 2D�|���S��
#ifdef _DEBUG
	static bool	m_bStencil;							// �X�e���V���`�悷�邩���Ȃ���
#endif // _DEBUG
};

#endif