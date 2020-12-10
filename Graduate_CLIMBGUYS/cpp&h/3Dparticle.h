// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�p�[�e�B�N���̏���[3Dparticle.h]
// Author : Nishiyama Koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Calculation.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X��`
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C3DParticle :public CScene
{
public:
	/* �񋓌^ */
	// �G�t�F�N�g�^�C�v
	typedef enum
	{
		TYPE_ANGLE_TO_VECTOR = 0,	// �p�x��VECTOR�ɓ�����
		TYPE_ANGLE_AND_VECTOR,		// �p�x��VECTOR�͕ʂł���
		TYPE_MAX,
	} TYPE;
	// �p�[�e�B�N���ԍ�
	typedef enum
	{
		PARTICLE_ID_NONE = -1,
		PARTICLE_ID_UNKNOWN,			// �������܂��Ă��Ȃ�
		PARTICLE_ID_ERUPTION_0,			// ���� 0 �A�j���[�V���� ��
		PARTICLE_ID_SMOKE,				// �� �A�j���[�V����
		PARTICLE_ID_SPARK,				// �Ή� �A�j���[�V����
		PARTICLE_ID_BLOCK,				// ���Ηp�u���b�N
		PARTICLE_ID_FIRE,				// ��
		PARTICLE_ID_EXPLOSION,			// ����
		PARTICLE_ID_AIRLINE,			// ��C�̐�
		PARTICLE_ID_MAX,
	} PARTICLE_ID;
	/* �\���� */
	// �p�[�e�B�N�����
	typedef struct PARTICLE_INFO
	{
		PARTICLE_INFO()
		{
			nFrame			= 0;				// �t���[������
			nNumber			= 0;				// �o����
			nEffectTexType	= 0;				// �G�t�F�N�g�̃e�N�X�`���̎��
			nEffectType		= 0;				// �G�t�F�N�g�̎��
			/* �ʒu��� */
			Pos				= D3DVECTOR3_ZERO;	// �ʒu
			PosXRand		= INTEGER2(0,0);	// �ʒuX���W�����_���p
			PosYRand		= INTEGER2(0, 0);	// �ʒuY���W�����_���p
			PosZRand		= INTEGER2(0, 0);	// �ʒuY���W�����_���p
			/* �J���[��� */
			Col = D3DXCOLOR_INI;				// �F
			bRedRand		= false;			// �ԃ����_��
			bGreenRand		= false;			// �΃����_��
			bBlueRand		= false;			// �����_��
			bAlphaRand		= false;			// �����x�����_��
			bAlphaDecrease	= false;					// �����x�������
			/* �T�C�Y��� */
			Size = D3DVECTOR2_ZERO;				// �T�C�Y
			SizeXRand		= INTEGER2(0, 0);	// �T�C�Yx�����_���p
			SizeYRand		= INTEGER2(0, 0);	// �T�C�Yy�����_���p
			fSizeChange		= 0.0f;						// �T�C�Y�ω��l
			bSizeDecrease	= false;					// �T�C�Y�̌������
			/* �p�x��� */
			Rot				= D3DVECTOR3_ZERO;	// ��]�[
			nAngleRand		= INTEGER2(0, 0);	// �p�x�̃����_���p
			/* �x�N�g����� */
			Vector = D3DVECTOR3_ZERO;				// �x�N�g��
			VectorXRand = INTEGER2(0, 0);			// �x�N�g��X�����_���p
			VectorYRand = INTEGER2(0, 0);			// �x�N�g��Y�����_���p
			VectorZRand = INTEGER2(0, 0);			// �x�N�g��Y�����_���p
			/* ���x��� */
			fSpeed			= 0.0f;				// ���x
			nSpeedRand		= INTEGER2(0, 0);	// ���x�̃����_���p
			/* ���C�t��� */
			nLife			= 0;				// ���C�t
			nLifeRand		= INTEGER2(0, 0);	// ���C�t�̃����_���p
			/* �A�j���[�V������� */
			Anim = ANIMATION_LOAD();							// �A�j���[�V�������
			/* �u�����h�^�C�v */
			Blend = CRenderer::BLEND_TRANSLUCENT;	// �u�����h�^�C�v
		}
		// �ϐ��錾
		int					nFrame;			// �t���[������
		int					nNumber;		// �o����
		int					nEffectTexType;	// �G�t�F�N�g�̃e�N�X�`���̎��
		int					nEffectType;	// �G�t�F�N�g�̎��
		/* �ʒu��� */
		D3DXVECTOR3			Pos;			// �ʒu
		INTEGER2			PosXRand;		// �ʒuX���W�����_���p
		INTEGER2			PosYRand;		// �ʒuY���W�����_���p
		INTEGER2			PosZRand;		// �ʒuY���W�����_���p
		/* �J���[��� */
		D3DXCOLOR			Col;			// �F
		bool				bRedRand;		// �ԃ����_��
		bool				bGreenRand;		// �΃����_��
		bool				bBlueRand;		// �����_��
		bool				bAlphaRand;		// �����x�����_��
		bool				bAlphaDecrease;	// �����x�������
		/* �T�C�Y��� */
		D3DXVECTOR2			Size;			// �T�C�Y
		INTEGER2			SizeXRand;		// �T�C�Yx�����_���p
		INTEGER2			SizeYRand;		// �T�C�Yy�����_���p
		float				fSizeChange;	// �T�C�Y�ω��l
		bool				bSizeDecrease;	// �T�C�Y�̌������
		/* �x�N�g����� */
		D3DXVECTOR3			Vector;			// �x�N�g��
		INTEGER2			VectorXRand;	// �x�N�g��X�����_���p
		INTEGER2			VectorYRand;	// �x�N�g��Y�����_���p
		INTEGER2			VectorZRand;	// �x�N�g��Y�����_���p
		/* �p�x��� */
		D3DXVECTOR3			Rot;			// ��]�[
		INTEGER2			nAngleRand;		// �p�x�̃����_���p
		/* ���x��� */
		float				fSpeed;			// ���x
		INTEGER2			nSpeedRand;		// ���x�̃����_���p
		/* �A�j���[�V������� */
		ANIMATION_LOAD		Anim;			// �A�j���[�V�������
		/* �^�C�v */
		int					nParticleType;	// �p�[�e�B�N���^�C�v
		/* ���C�t��� */
		int					nLife;			// ���C�t
		INTEGER2			nLifeRand;		// ���C�t�̃����_���p
		/* �u�����h�^�C�v */
		CRenderer::BLEND	Blend;			// �u�����h�^�C�v
	} PARTICLE_INFO;

	/* �֐� */
	// �R���X�g���N�^
	C3DParticle();
	// �f�X�g���N�^
	~C3DParticle();
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_Origin; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// ����
	//	ParticleId	: �p�[�e�B�N���ԍ�
	//	origin		: �����ʒu
	static C3DParticle * Create(
		PARTICLE_ID const	&ParticleId,	// �p�[�e�B�N���ԍ�
		D3DXVECTOR3 const	&origin,		// �����ʒu
		bool		const	&b2D = false	// 2D�`����
	);

	// �S���\�[�X���̓ǂݍ���
	static HRESULT Load(void);
	// �S���\�[�X���̊J��
	static void Unload(void);
	// �p�[�e�B�N���ԍ��ݒ�
	//	ParticleId	: �p�[�e�B�N���ԍ�
	void SetParticle(
		PARTICLE_ID const	&ParticleId	// �p�[�e�B�N���ԍ�
	);
	// �����ʒu�ݒ�
	//	origin	: �����ʒu
	void SetOrigin(
		D3DXVECTOR3 const	&origin		// �����ʒu
	);

private:
	/* �֐� */

	/* �ϐ� */
	static PARTICLE_INFO	*m_ParticleOffset[PARTICLE_ID_MAX];	// �p�[�e�B�N���̃I�t�Z�b�g
	PARTICLE_ID				m_offsetID;							// �I�t�Z�b�gID
	bool					m_b2D;								// 2D�`����
	int						m_nFrameCount;						// �t���[���̃J�E���g
	D3DXVECTOR3				m_Origin;							// ���_
};

#endif
