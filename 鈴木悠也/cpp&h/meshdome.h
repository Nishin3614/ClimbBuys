// ----------------------------------------
//
// ���b�V���h�[�������̐���[meshdome.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHDOME_H_
#define _MESHDOME_H_	 // �t�@�C������������߂�

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

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CMeshdome : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_NORMAL,
		TYPE_WARNING,
		TYPE_POINTCIRCLE,
		TYPE_WAVE,
		TYPE_MAX
	} TYPE;
	/* �֐� */
	CMeshdome();
	~CMeshdome();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	static HRESULT Load(void);
	static void UnLoad(void);
	// �쐬����(�V�[���Ǘ�)
	static CMeshdome * Create(
		D3DXVECTOR3 const &pos,										// �ʒu
		D3DXVECTOR3 const &size,									// �T�C�Y
		int const &nWidth = 10,										// ����
		int const &nDepth = 5,										// �c��
		TYPE const &type = TYPE_NORMAL,								// �^�C�v
		D3DXCOLOR	const &col = D3DXCOLOR_INI,						// �J���[
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,					// ��]
		bool const &bDrawback = false,								// ���ʕ`����
		CScene::LAYER const &layer = CScene::LAYER_DOME				// ���C���[
	);
	// �쐬����(�l�Ǘ�)
	static CMeshdome * Create_Self(
		D3DXVECTOR3 const &pos,										// �ʒu
		D3DXVECTOR3 const &size,									// �T�C�Y
		int const &nWidth = 10,										// ����
		int const &nDepth = 5,										// �c��
		TYPE const &type = TYPE_NORMAL,								// �^�C�v
		D3DXCOLOR	const &col = D3DXCOLOR_INI,						// �J���[
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO					// ��]
	);
	// �擾
	bool GetUse(void);						// �g�p���
	// �ݒ�
	void SetPos(D3DXVECTOR3 const &pos);	// �ʒu
	void SetRot(D3DXVECTOR3 const &rot);	// ��]
	void SetCol(D3DXCOLOR const &col);		// �F
	void SetUse(bool const bUse);					// �g�p���

	void SetDrawBack(bool const bUse) { m_bDrawBack = bUse; }
protected:

private:
	/* �֐� */
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);	// ���_���W�̐���
	/* �ϐ� */
	static int CMeshdome::m_nTexId[TYPE_MAX];	// �e�N�X�`���[ID
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIndex;			// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_OriginBlock;					// �����z�u
	D3DXMATRIX  m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_size;							// �T�C�Y
	D3DXVECTOR3 m_rot;							// �p�x
	D3DXCOLOR	m_col;							// �F
	int		m_nNumberVertex;					// �����_��
	int		m_nNumIndex;						// ���C���f�b�N�X
	int		m_nNumPolygon;						// ���|���S��
	int		m_nBlock_Depth;						// �c�u���b�N��
	int		m_nBlock_Width;						// ���u���b�N��
	bool	m_bUse;								// �g�p
	bool	m_bDrawBack;						// ���ʕ`��
	TYPE	m_type;								// �^�C�v
};

#endif