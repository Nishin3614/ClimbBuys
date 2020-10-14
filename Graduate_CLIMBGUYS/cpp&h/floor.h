// ----------------------------------------
//
// �������̐���[floor.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_	 // �t�@�C������������߂�

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
class CFloor : public CScene
{
public:
	/* �֐� */
	CFloor();
	~CFloor();
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
	void Scene_SetPos(D3DXVECTOR3 &pos) { m_pos = pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	// ���_�ړ�����
	void vertexMove(FILE *pFile);

	static HRESULT Load(void);
	static void UnLoad(void);
	// ����
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	col			: �F
	//	rot			: ��]
	//	nWidth		: ���u���b�N
	//	nDepth		: �c�u���b�N
	//	nTexType	: �e�N�X�`���[�^�C�v
	static CFloor * Create(
		D3DXVECTOR3 const &pos,		// �ʒu
		D3DXVECTOR3 const & size,	// �T�C�Y
		D3DXCOLOR const & col,		// �F
		D3DXVECTOR3 const & rot,	// ��]
		int const & nWidth,			// ���u���b�N
		int const & nDepth,			// �c�u���b�N
		int const & nTexType		// �e�N�X�`���[�^�C�v
	);
	float GetHeight(D3DXVECTOR3 &pos);								// �����擾
	void SetCalculation(bool bValue) { m_bCalculation = bValue; }	// �����v�Z�t���O�̐ݒ�
	bool GetCalculation(void) { return m_bCalculation; }			// �����v�Z�t���O�̎擾
	// �ʒu�ݒ�
	void SetPos(D3DXVECTOR3 const &pos) { m_pos = pos; };
	// �T�C�Y�ݒ�
	void SetSize(D3DXVECTOR3 const &size);
	// �F�ݒ�
	void SetCol(D3DXCOLOR const & col);
	// ��]�ݒ�
	void SetRot(D3DXVECTOR3 const &rot) { m_rot = rot; };
	// �ʒu�擾
	D3DXVECTOR3 &  GetPos(void) { return m_pos; };
	// �T�C�Y�擾
	D3DXVECTOR3 &  GetSize(void) { return m_size; };
	// �F�擾
	D3DXCOLOR &  GetCol(void) { return m_col; };
	// ��]�擾
	D3DXVECTOR3 & GetRot(void) { return m_rot; };

protected:

private:
	/* �֐� */
	void CalculationNormalize(void);

	/* �ϐ� */
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DTEXTURE9 m_pMaskTex;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIndex;					// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_OriginBlock;							// �����z�u
	D3DXMATRIX  m_mtxWorld;								// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_size;									// �T�C�Y
	D3DXVECTOR3 m_rot;									// �p�x
	D3DXCOLOR	m_col;									// �F
	int	m_nNumberVertex;								// �����_��
	int m_nNumIndex;									// ���C���f�b�N�X
	int m_nNumPolygon;									// ���|���S��
	int m_nBlock_Depth;									// �c�u���b�N��
	int m_nBlock_Width;									// ���u���b�N��
	int m_nTexType;										// �e�N�X�`���[�^�C�v
	bool m_bCalculation;									// ���̋N���v�Z�t���O
	static LPD3DXEFFECT		m_pEffect;
};
#endif