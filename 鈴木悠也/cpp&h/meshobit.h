// ----------------------------------------
//
// �O�Տ����̐���[meshobit.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHOBIT_H_
#define _MESHOBIT_H_	 // �t�@�C������������߂�

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
class CMeshobit : public CScene
{
public:
	/* �񋓌^ */
	// �e�N�X�`���[�^�C�v
	typedef enum
	{
		TEX_0 = 0,
		TEX_1,
		TEX_MAX
	} TEX;

	/* �֐� */
	CMeshobit();
	~CMeshobit();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	void DrawSet(
		D3DXMATRIX const &mtx
	);
	// �J���[�ݒ�
	void SetCol(
		D3DXCOLOR const & BiginCol,	// �n�_�̐F
		D3DXCOLOR const & EndCol	// �I�_�̐F
	);
	// �����ʒu�ɂ���
	void InitPos(
		D3DXMATRIX const &mtx
	);
	// �ǂݍ���
	static HRESULT Load(void);
	// �ǂݍ��񂾂��̂�j��
	static void UnLoad(void);
	// �쐬����(�l�Ǘ�)
	static std::unique_ptr<CMeshobit> Create_Self(
		int const &nLine,				// ���̐�
		D3DXVECTOR3 const &BeginOffset,	// �n�_�I�t�Z�b�g
		D3DXVECTOR3 const &EndOffset,	// �I�_�I�t�Z�b�g
		TEX const &TexId = TEX_0		// �e�N�X�`���[�^�C�v
	);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
protected:

private:
	/* �֐� */
	/* �ϐ� */
	static int				m_nTexId[TEX_MAX];			// �e�N�X�`���[ID
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_aOffset[2];				// �I�t�Z�b�g���W
	D3DXCOLOR				m_col;						// �F
	int						m_nLine;					// ���̐�
	int						m_nNumberVertex;			// �����_��
	int						m_nNumPolygon;				// ���|���S��
	TEX						m_tex;						// �e�N�X�`���[�^�C�v
};

#endif