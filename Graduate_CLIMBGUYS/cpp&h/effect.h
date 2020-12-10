// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �G�t�F�N�g�̏���[effect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Calculation.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EFFECT_MAX			(5000)				// �ő吔
#define EFFECT_NUM_VERTEX	(4)					// ���_��
#define BASEANGLE			(D3DX_PI * 0.25f)	// �x�[�X�p�x

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CRenderer;	// �����_���[�N���X

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X��`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CEffect :public CScene
{
public:
	/* �񋓌^ */
	// �G�t�F�N�g�^�C�v
	typedef enum
	{
		EFFECT_TYPE_NONE = -1,		// �Ȃ�
		EFFECT_TYPE_MAX				// �ő吔
	} EFFECT_TYPE;

	/* �\���� */
	// �G�t�F�N�g�A�j���[�V����
	typedef struct _EFFECTANIM : public ANIMATION
	{
		_EFFECTANIM() : ANIMATION()
		{
			FirstPos = D3DVECTOR2_ZERO;		// �����ʒu
			EndPos = D3DVECTOR2_ZERO;		// �����ʒu
			bUse = false;					// �A�j���[�V�����g�p���
			bTexUpdate = false;				// �e�N�X�`���[�X�V���
		}
		// �A�j���[�V�����ǂݍ��ݑ��
		_EFFECTANIM& operator = (ANIMATION_LOAD const & AnimLoad)
		{
			ANIMATION * pAnim = (ANIMATION *)this;
			*pAnim = AnimLoad;
			bUse = true;
			return *this;
		}
		// ����������
		void Init(void)
		{
			ANIMATION::Init();				// �A�j���[�V�����̏�����
			FirstPos = D3DVECTOR2_ZERO;		// �����ʒu
			EndPos = D3DVECTOR2_ZERO;		// �����ʒu
			bUse = false;					// �A�j���[�V�����g�p���
			bTexUpdate = false;				// �e�N�X�`���[�X�V���
		}

		D3DXVECTOR2			FirstPos;			// �����ʒu
		D3DXVECTOR2			EndPos;				// �����ʒu
		bool				bUse;				// �A�j���[�V�����g�p���
		bool				bTexUpdate;			// �e�N�X�`���[�X�V���
	} EFFECTANIM;
	// �G�t�F�N�g�v���~�e�B�u
	typedef struct
	{
		D3DXVECTOR3			pos;				// �ʒu
		D3DXVECTOR3			rot;				// ��]��
		D3DXVECTOR3			move;				// �ړ���
		D3DXCOLOR			col;				// �F
		D3DXVECTOR2			size;				// �T�C�Y
		D3DXVECTOR2			sizeValue;			// �T�C�Y�̕ω��l
		float				fAngle;				// �p�x
		float				fAlphaValue;		// �A���t�@�l�̕ω��l
		int					nLife;				// ��������
		int					nTexType;			// �e�N�X�`���̎��
		bool				bUse;				// �g�p���Ă��邩�ǂ���
		EFFECTANIM			Animation;			// �A�j���[�V�������
		EFFECT_TYPE			EffectType;			// �G�t�F�N�g�̎��
		CRenderer::BLEND	BlendType;			// �u�����h�^�C�v
	} EFFECT;

	/* �֐� */
	// �R���X�g���N�^
	CEffect();
	// �f�X�g���N�^
	virtual ~CEffect();
	// �S���\�[�X���̓ǂݍ��ݏ���
	static HRESULT Load(void);
	// �S���\�[�X���̊J��
	static void Unload(void);
	// ����������
	virtual void Init(void);
	// �I������
	virtual void Uninit(void) = 0;
	// �X�V����
	virtual void Update(void) = 0;
	// �`�揈��
	virtual void Draw(void) = 0;
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void Debug(void);
#endif // _DEBUG
	// �������瓖��������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_OpponentCollision(
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


	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// �e�N�X�`���[�A�j���[�V�����̐ݒ�
	//	nMaxCntAnim			: �ő�A�j���[�V�����J�E���g
	//	nMaxHorizonAnim		: �ő吅���A�j���[�V�����J�E���g
	//	nMaxVirticalAnim	: �ő吂���A�j���[�V�����J�E���g
	//	bLoop				: ���[�v���
	void SetTexAnim(
		EFFECT * pEffect,				// �G�t�F�N�g���
		int const & nMaxCntAnim,		// �J�E���g�A�j���ő吔
		int const & nMaxHorizonAnim,	// �����A�j���[�V�����ő吔
		int const & nMaxVirticalAnim	// �����A�j���[�V�����ő吔
	);

	// �l�̏���������
	//	pEffect		: �G�t�F�N�g���
	void InitValues(
		EFFECT *pEffect	// �G�t�F�N�g���
	);
	// �l�̏���������
	//	pEffect		: �G�t�F�N�g���
	//	nCntEffect	: �G�t�F�N�g�J�E���g
	static void Init_OneValues(
		EFFECT * pEffect		// �G�t�F�N�g���
	);

protected:
	/* �֐� */
	// ���C�t�X�V����
	//	pEffect	: �G�t�F�N�g���
	bool UpdateLife(
		EFFECT *pEffect		// �G�t�F�N�g���
	);
	// ���C�t�X�V����
	//	pVtx	: 2D���_���
	//	pEffect	: �G�t�F�N�g���
	bool UpdateVetex(
		EFFECT *pEffect		// �G�t�F�N�g���
	);
	// ���C�t�X�V����
	//	pVtx	: 2D���_���
	//	pEffect	: �G�t�F�N�g���
	void UpdateMove(
		EFFECT *pEffect		// �G�t�F�N�g���
	);
	// �A�j���[�V�����X�V����
	//	pEffect	: �G�t�F�N�g���
	void Updata_Animation(
		EFFECT *pEffect		// �G�t�F�N�g���
	);
private:
};

#endif