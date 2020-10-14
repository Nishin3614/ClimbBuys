// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2D�Q�[�W�����̐���[2Dgauge.h]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _2DGAUGE_H_
#define _2DGAUGE_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "character.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;	// 2D�V�[���N���X

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C2DGauge : public CScene
{
public:
	/* �֐� */
	// �R���X�g���N�^
	C2DGauge();
	// �f�X�g���N�^
	~C2DGauge();
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
	// �������瓖��������̏���
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
	virtual D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	virtual D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	virtual D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	// �S���\�[�X���̓ǂݍ���
	static HRESULT Load(void);
	// �S���\�[�X���̉��
	static void UnLoad(void);
	// ����(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	Maincol		: ���C���J���[
	//	Subcol		: �T�u�J���[
	//	BGcol		: �w�i�F
	static C2DGauge * Create(
		D3DXVECTOR3 const &pos,												// �ʒu
		D3DXVECTOR2 const &size,											// �T�C�Y
		D3DXCOLOR	const &Maincol,											// ���C���J���[
		CCharacter::CHARACTER	const &type,											// �L�����N�^�[�^�C�v
		D3DXCOLOR	const &Subcol = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),			// �T�u�J���[
		D3DXCOLOR	const &BGcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)		// �w�i�F
		);
	// ����(�l�Ǘ�)
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	Maincol		: ���C���J���[
	//	Subcol		: �T�u�J���[
	//	Blockcol	: �w�i�F
	static C2DGauge * Create_Self(
		D3DXVECTOR3 const &pos,												// �ʒu
		D3DXVECTOR2 const &size,											// �T�C�Y
		D3DXCOLOR	const &Maincol,											// ���C���J���[
		D3DXCOLOR	const &Subcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),		// �T�u�J���[
		D3DXCOLOR	const &BGcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)		// �w�i�F
	);
	// �ݒ� //
	// �Q�[�W�̒����̌Œ�l�ݒ�
	//	fMaxValue : �X�e�[�^�X�̍ő�l
	void SetConstance(
		float const &fMaxValue	// �X�e�[�^�X�̍ő�l
	);
	// ���C���J���[�ݒ�
	//	MainCol_R	: �E�̃��C���J���[
	//	MainCol_L	: ���̃��C���J���[
	void SetMainCol(
		D3DXCOLOR const &MainCol_R,	// �E�̐F
		D3DXCOLOR const &MainCol_L	// ���̐F
	);
	// ���C���A�T�u�A�w�i�̃J���[�ݒ�
	//	Maincol	: ���C���J���[
	//	Subcp;	: �T�u�J���[
	//	BGcol	: �w�i�F
	void SetCol(
		D3DXCOLOR	const &Maincol,										// ���C���J���[
		D3DXCOLOR	const &Subcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),	// �T�u�J���[
		D3DXCOLOR	const &BGcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// �w�i�F
	);
	// �ʒu�ݒ�
	//	pos	: �ʒu
	void SetPos(
		D3DXVECTOR3 const &pos	// �ʒu
	);
	// �擾 //
	// �ʒu�擾
	D3DXVECTOR3 & GetPos(void);
	// �Q�[�W�ω�
	//	fValue	: �X�e�[�^�X�̒l
	void ChangeGauge(
		float const &fValue	// �X�e�[�^�X�̒l
	);
	// �Q�[�W�̏����ݒ�
	//	fValue	: �X�e�[�^�X�̒l
	void BeginGauge(
		float const &fValue	// �X�e�[�^�X�̒l
	);
protected:

private:
	/* �񋓌^ */
	// �Q�[�W�̎��
	typedef enum
	{
		GAUGE_BLOCK = 0,	// �w�i�g
		GAUGE_UNDERLINE,	// �T�u�g
		GAUGE_MAINLINE,		// ���C���g
		GAUGE_DESIGN,		// �Q�[�W�f�U�C���g
		GAUGE_MAX
	} GAUGE;
	/* �֐� */

	/* �ϐ� */
	D3DXVECTOR2	m_size[GAUGE_MAX];							// �T�C�Y���
	float		m_fSizeLast;								// �T�C�Y�̖ڕW�l
	float		m_fSizeDiff;								// �T�C�Y�̍���
	float		m_fConstance;								// �T�C�Y���ς��萔
	D3DXCOLOR	m_col[GAUGE_MAX];							// �J���[���
	D3DXVECTOR3	m_pos;										// �ʒu���
	CScene_TWO *m_aScene_Two[GAUGE_MAX];					// 2D�Q�[�W2D
	CCharacter::CHARACTER m_type;							// �L�����N�^�[�^�C�v
	static int	m_TexType[CCharacter::CHARACTER_MAX];	// �e�N�X�`���[�^�C�v
};
#endif