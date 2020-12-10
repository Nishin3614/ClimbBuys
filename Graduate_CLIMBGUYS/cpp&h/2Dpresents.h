// ----------------------------------------
//
// 2DPRESENTS�����̐���[2Dpresents.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _2DPRESENTS_H_
#define _2DPRESENTS_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_two.h"
#include "Calculation.h"

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
class C2DPresents : public CScene_TWO
{
public:
	/* �񋓌^ */
	// �t�F�[�h�^�C�v
	typedef enum
	{
		FADETYPE_NONE = -1,
		FADETYPE_IN,
		FADETYPE_COOPERATION,
		FADETYPE_OUT,
		FADETYPE_END,
		FADETYPE_MAX
	} FADETYPE,*PFADETYPE;
	// �t�F�[�h�A�E�g
	typedef enum
	{
		FADE_OUT_NORMAL = 0,
		FADE_OUT_UNDER_FROM_ON,
		FADE_OUT_ON_FROM_UNDER,
		FADE_OUT_DISAPPEARANCE,
		FADE_OUT_MAX
	} FADE_OUT;
	// �t�F�[�h�C��
	typedef enum
	{
		FADE_IN_NORMAL = 0,
		FADE_IN_UNDER_FROM_ON,
		FADE_IN_ON_FROM_UNDER,
		FADE_IN_DISAPPEARANCE,
		FADE_IN_MAX
	} FADE_IN;
	// �����^�C�v
	typedef enum
	{
		COOPERATIONTYPE_NORMAL = 0,
		COOPERATIONTYPE_UNDER_FROM_ON,
		COOPERATIONTYPE_MAX
	} COOPERATIONTYPE;

	/* �֐� */
	C2DPresents();
	~C2DPresents();
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

	/* ���� */
	// �_�Őݒ�
	void SetCooperation(
		D3DXCOLOR const & changecol,	// �ς��F
		int const & nTimeSwitch,		// �؂�ւ�����
		int const & nTimeChange			// �؂�ւ�鎞��
	);
	// �g�嗦�ݒ�
	void SetCooperation(
		float const & fChangeScal,		// �g�嗦
		int const & nTimeChange			// �؂�ւ�鎞��
	);
	/* �t�F�[�h�C�� */
	// �t�F�[�h�C��(�ʒu)�ݒ�
	void SetFadeIn(
		D3DXVECTOR3 const & BeginPos,	// �����ʒu
		D3DXVECTOR3 const & EndPos,		// �ړI�ʒu
		int const & nMaxfram			// �ő�t���[��
	);
	// �t�F�[�h�C��(�F�w��)�ݒ�
	void SetFadeIn(
		D3DXCOLOR const & BeginCol,		// �����J���[
		D3DXCOLOR const & EndCol,		// �ڕW�J���[
		int const & nMaxfram			// �ő�t���[��
	);
	// �t�F�[�h�C��(�A�j���X�^�[�g)�ݒ�
	void SetFadeIn_StarAnim(
		int const & nMaxStartAnim		// �ő�A�j���X�^�[�g
	);
	// �t�F�[�h�C�������I���ݒ�
	void SetFadeIn_Compulsion(
		bool const & bCompulsion
	);
	// �t�F�[�h�C���̎g�p�ݒ�
	bool GetFadeIn_Bool(void);
	// �t�F�[�h�C���̋����I��
	void Compulsion_FadeIn(void);
	// �t�F�[�h�C��(�ʒu)�J�n
	void Start_FadeIn(void);
	/* �t�F�[�h�A�E�g */
	// �t�F�[�h�A�E�g(�ʒu)�ݒ�
	void SetFadeOut(
		D3DXVECTOR3 const & BeginPos,	// �����ʒu
		D3DXVECTOR3 const & EndPos,		// �ڕW�ʒu
		int const & nMaxfram			// �ő�t���[��
	);
	// �t�F�[�h�A�E�g(�F�w��)�ݒ�
	void SetFadeOut(
		D3DXCOLOR const & BeginCol,		// �����J���[
		D3DXCOLOR const & EndCol,		// �ڕW�J���[
		int const & nMaxfram			// �ő�t���[��
	);
	// �t�F�[�h�A�E�g(�A�j���X�^�[�g)�ݒ�
	void SetFadeOut_StarAnim(
		int const & nMaxStartAnim		// �ő�A�j���X�^�[�g
	);
	// �t�F�[�h�C�������I���ݒ菈��
	void SetFadeOut_Compulsion(
		bool const & bCompulsion
	);
	// �t�F�[�h�A�E�g�̎g�p�ݒ�
	bool GetFadeOut_Bool(void);
	// �t�F�[�h�A�E�g�̋����I��
	void Compulsion_FadeOut(void);
	// �t�F�[�h�A�E�g�̊J�n�ݒ�
	void Start_FadeOut(void);
	// �t�F�[�h�^�C�v�̏�Ԏ擾
	FADETYPE GetFadetype(void);
	// �t�F�[�h�^�C�v�̏�Ԑݒ�
	void SetFadetype(FADETYPE const &Fadetype) { m_Fadetype = Fadetype; };
	// �t�F�[�h�C���̍ő�t���[�����擾
	int const GetFadeInMaxFram(void);
	// �t�F�[�h�C���̃t���[�����擾
	int const GetFadeInFram(void);
	// �t�F�[�h�C���̍ő�J�n�A�j���^�C�����擾
	int const GetFadeInMaxAnimTime(void);
	// �t�F�[�h�C���̃A�j���^�C�����擾
	int const GetFadeInAnimTime(void);
	// �t�F�[�h�A�E�g�̍ő�J�n�A�j���^�C�����擾
	int const GetFadeOutMaxAnimTime(void);
	// �t�F�[�h�A�E�g�̃A�j���^�C�����擾
	int const GetFadeOutAnimTime(void);

	// �쐬(2D�v���[���c�p������)
	//	offsettype	: �^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	nTextype	: �e�N�X�`���[�^�C�v
	//	frot		: �p�x
	//	col			: �F
	//	layer		: ���C���[
	static C2DPresents * Create(
		OFFSET_TYPE		const & type,									// �^�C�v
		D3DXVECTOR3		const & pos,									// �ʒu
		D3DXVECTOR2		const & size,									// �T�C�Y
		int				const & nTexType,								// �e�N�X�`���[�^�C�v
		float			const & frot = 0.0f,							// �p�x
		D3DXCOLOR		const & col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),	// �F
		CScene::LAYER	const & layer = CScene::LAYER_UI				// ���C���[
	);
	// �쐬(2D�v���[���c�p���Ȃ�)
	//	offsettype	: �^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	nTextype	: �e�N�X�`���[�^�C�v
	//	frot		: �p�x
	//	col			: �F
	static C2DPresents * Create_Self(
		OFFSET_TYPE	const & type,										// �^�C�v
		D3DXVECTOR3 const & pos,										// �ʒu
		D3DXVECTOR2 const & size,										// �T�C�Y
		int			const & nTexType,									// �e�N�X�`���[�^�C�v
		float		const & frot = 0.0f,								// �p�x
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �F
	);
	// �쐬(�l�Ǘ��X�}�[�g�|�C���^�[)
	//	offsettype	: �^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	nTextype	: �e�N�X�`���[�^�C�v
	//	frot		: �p�x
	//	col			: �F
	static std::unique_ptr<C2DPresents> Create_Unique(
		OFFSET_TYPE	const & type,										// �^�C�v
		D3DXVECTOR3 const & pos,										// �ʒu
		D3DXVECTOR2 const & size,										// �T�C�Y
		int			const & nTexType,									// �e�N�X�`���[�^�C�v
		float		const & frot = 0.0f,								// �p�x
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �F
	);

protected:
	// �ݒ� //
private:
	/* �񋓌^ */

	/* �\���� */
	// �F�̓_��
	typedef struct COL_FLASHING
	{
		COL_FLASHING()
		{
			Origincol = D3DXCOLOR_INI;
			changecol = D3DXCOLOR_INI;
			colDiff = D3DXCOLOR_CA(0.0f,0.0f);
			nTimeSwitch = 0;
			nTimeChange = 0;
			nCntTimeSwitch = 0;
			nCntTimeChange = 0;
			bSwitch = false;
			bChange = false;
		}
		D3DXCOLOR Origincol;			// ���F
		D3DXCOLOR changecol;			// �ς��F
		D3DXCOLOR colDiff;				// �F�̍���
		int nTimeSwitch;				// �؂�ւ�����
		int nTimeChange;				// �؂�ւ�鎞��
		int nCntTimeSwitch;				// �؂�ւ����ԃJ�E���g
		int nCntTimeChange;				// �؂�ւ�鎞�ԃJ�E���g
		bool bSwitch;					// �؂�ւ���
		bool bChange;					// �؂�ւ�蒆
	} COL_FLASHING,*P_COL_FLASHING;
	// �g�嗦���
	typedef struct SCALING
	{
		SCALING()
		{
			OriginSize = D3DVECTOR2_ZERO;
			fScal = 1.0f;
			fScalChange = 0.0f;
			fScalDiff = 0.0f;
			nTimeChange = 0;
			nCntTimeChange = 0;
			bChange = false;
		}
		D3DXVECTOR2 OriginSize;			// ���̃T�C�Y
		float fScal;					// �g�嗦
		float fScalChange;				// �ڕW�g�嗦
		float fScalDiff;				// �g�嗦�㏸��
		int nTimeChange;				// �؂�ւ�鎞��
		int nCntTimeChange;				// �؂�ւ�鎞�ԃJ�E���g
		bool bChange;					// �؂�ւ�蒆
	} SCALING, *P_SCALING;
	// �ړ����
	typedef struct DESIGN_MOVE
	{
		DESIGN_MOVE()
		{
			BeginPos = D3DVECTOR3_ZERO;
			EndPos = D3DVECTOR3_ZERO;
			DiffPos = D3DVECTOR3_ZERO;
			nfram = 0;
			nMaxfram = 0;
		}
		D3DXVECTOR3 BeginPos;			// �����ʒu
		D3DXVECTOR3 EndPos;				// �ڕW�ʒu
		D3DXVECTOR3 DiffPos;			// �����ʒu
		int nfram;						// �t���[��
		int nMaxfram;					// �ő�t���[��
	} DESIGN_MOVE, *P_DESIGN_MOVE;
	// �F�w����
	typedef struct DESIGN_COL
	{
		DESIGN_COL()
		{
			BeginCol = D3DXCOLOR_INI;
			EndCol = D3DXCOLOR_INI;
			DiffCol = D3DXCOLOR_INI;
			nfram = 0;
			nMaxfram = 0;
		}
		D3DXCOLOR BeginCol;				// �����J���[
		D3DXCOLOR EndCol;				// �ڕW�J���[
		D3DXCOLOR DiffCol;				// �����J���[
		int nfram;						// �t���[��
		int nMaxfram;					// �ő�t���[��
	} DESIGN_COL, *P_DESIGN_COL;
	// �t�F�[�h���
	typedef struct FADE
	{
		FADE()
		{
			pDesign_Move = NULL;
			pDesign_Col = NULL;
			bFade = false;
			bCompulsion = false;
			nCntFade = 0;
			nMaxFade = 0;
			nCntAnimStart = 0;
			nMaxAnimStart = 0;
		}
		P_DESIGN_MOVE pDesign_Move;		// �ړ����
		P_DESIGN_COL pDesign_Col;		// �F�w����
		bool bFade;						// �t�F�[�h��
		bool bCompulsion;				// �t�F�[�h�����I�������邩�����Ȃ���
		int nCntFade;					// �J�E���g�t�F�[�h
		int nMaxFade;					// �ő�t�F�[�h��
		int nCntAnimStart;				// �X�^�[�g�J�E���g
		int nMaxAnimStart;				// �ő�X�^�[�g
	} FADE, *P_FADE;
	// �������
	typedef struct COOPERATION
	{
		COOPERATION()
		{
			pCol_flash = NULL;
			pScal = NULL;
		}
		P_COL_FLASHING pCol_flash;		// �F���
		P_SCALING pScal;				// �X�P�[�����
	} COOPERATION, *P_COOPERATION;

	/* �֐� */
	// �_�ōX�V
	void Update_ColorFlashing(void);
	// �����X�V
	void Update_Scaling(void);
	// �t�F�[�h(�w��F)
	void Update_Fade_DesignCol(
		D3DXCOLOR const & DiffCol,	// �����F
		D3DXCOLOR const & EndCol,	// �ڕW�F
		int & nFram,				// ���݂̃t���[��
		int const & nMaxFram		// �ő�̃t���[��
	);
	// �t�F�[�h(�ړ�)
	void Update_Fade_DesignMove(
		D3DXVECTOR3 const & DiffPos,	// �����F
		D3DXVECTOR3 const & GoalPos,	// �ڕW�F
		int & nFram,					// ���݂̃t���[��
		int const & nMaxFram			// �ő�̃t���[��
	);
	// �t�F�[�h�C���X�V
	bool Update_FadeIn(void);
	// �t�F�[�h�A�E�g�X�V
	bool Update_FadeOut(void);
	// �����X�V
	bool Update_Cooperation(void);

	/* �ϐ� */
	// �������
	P_COOPERATION m_pCooperation;		// �������
	// �t�F�[�h�C��
	P_FADE m_pFadeIn;					// �t�F�[�h���
	// �t�F�[�h�A�E�g
	P_FADE m_pFadeOut;					// �t�F�[�h���
	// �t�F�[�h�^�C�v
	FADETYPE m_Fadetype;				// �t�F�[�h�^�C�v
};

#endif