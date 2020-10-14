// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���[�U�[�C���^�[�t�F�[�X�w�b�_�[���� [ui.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _UI_H_
#define _UI_H_

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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;
class C2DPresents;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CUi : public CScene
{
public:
	/* �񋓌^ */
	// UI�^�C�v
	typedef enum
	{
		UITYPE_GAMEUI = 0,
		UITYPE_PAUSEUI_BG,
		UITYPE_PAUSEUI_SELECT,
		UITYPE_RANKINGUI,
		UITYPE_SETTINGUI_BG,
		UITYPE_SETTINGUI_SELECT,
		UITYPE_TITLEUI_NAME,
		UITYPE_TITLEUI_SELECT,
		UITYPE_TITLEUI_SELECT_BG,
		UITYPE_TUTORIALUI,
		UITYPE_SELECTCHARACTER,
		UITYPE_RESULT,
		UITYPE_DIE,
		UITYPE_GAMESTART,
		UITYPE_FINISH,
		UITYPE_WATCHING,
		UITYPE_MAX
	} UITYPE;
	// UI�̏��i�[�p�^��
	typedef std::vector<std::unique_ptr<CUi>> VEC_UNI_UI;
	// UI�̏��i�[�p�^��
	typedef std::vector<CUi*> VEC_P_UI;
	/* �\����*/
	// �_�Ń��[�h
	typedef struct COOPERATION_COL_LOAD
	{
		COOPERATION_COL_LOAD()
		{
			changecol = D3DXCOLOR_INI;
			nTimeSwitch = 0;
			nTimeChange = 0;
		}
		D3DXCOLOR changecol;	// �ς��F
		int nTimeSwitch;		// �؂�ւ�����
		int nTimeChange;		// �؂�ւ�鎞��
	} COOPERATION_COL_LOAD, *P_COOPERATION_COL_LOAD;
	// �g�嗦���[�h
	typedef struct COOPERATION_SCAL_LOAD
	{
		COOPERATION_SCAL_LOAD()
		{
			fChangeScal = 0.0f;
			nTimeChange = 0;
		}
		float fChangeScal;		// �g�嗦
		int nTimeChange;		// �؂�ւ�鎞��
	} COOPERATION_SCAL_LOAD, *P_COOPERATION_SCAL_LOAD;

	// �������[�h
	typedef struct COOPERATION_LOAD
	{
		COOPERATION_LOAD()
		{
			pCol = NULL;
			pScal = NULL;
		}
		P_COOPERATION_COL_LOAD pCol;		// �F
		P_COOPERATION_SCAL_LOAD pScal;		// �g�嗦
	} COOPERATION_LOAD, *P_COOPERATION_LOAD;

	// �t�F�[�h�X�^�[�g����
	typedef int NSTARTFADE, *P_NSTARTFADE;
	// �t�F�[�h(�F�w��)���[�h
	typedef struct FADE_COL_LOAD
	{
		FADE_COL_LOAD()
		{
			BeginCol = D3DXCOLOR_INI;
			EndCol = D3DXCOLOR_INI;
			nMaxfram = 0;
		}
		D3DXCOLOR BeginCol;	// �����J���[
		D3DXCOLOR EndCol;	// �ړI�J���[
		int nMaxfram;		// �ő�t���[��
	} FADE_COL_LOAD, *P_FADE_COL_LOAD;
	// �t�F�[�h�ʒu���[�h
	typedef struct FADE_POS_LOAD
	{
		FADE_POS_LOAD()
		{
			BeginPos = D3DVECTOR3_ZERO;
			EndPos = D3DVECTOR3_ZERO;
			nMaxfram = 0;
		}
		D3DXVECTOR3 BeginPos;	// �����ʒu
		D3DXVECTOR3 EndPos;		// �ړI�ʒu
		int nMaxfram;			// �ő�t���[��
	} FADE_POS_LOAD, *P_FADE_POS_LOAD;

	// �t�F�[�h�C�����[�h
	typedef struct FADEIN_LOAD
	{
		FADEIN_LOAD()
		{
			FadeStart = 0;
			pFadeCol = NULL;
			pFadePos = NULL;
			bSkip = false;
		}
		NSTARTFADE FadeStart;		// �t�F�[�h�X�^�[�g����
		P_FADE_COL_LOAD pFadeCol;	// �t�F�[�h(�F�w��)
		P_FADE_POS_LOAD pFadePos;	// �t�F�[�h(�ʒu)
		bool bSkip;					// �t�F�[�h�̃X�L�b�v
	} FADEIN_LOAD, *P_FADEIN_LOAD;
	// �t�F�[�h�A�E�g���[�h
	typedef struct FADEOUT_LOAD
	{
		FADEOUT_LOAD()
		{
			FadeStart = 0;
			pFadeCol = NULL;
			pFadePos = NULL;
			bSkip = false;
		}
		NSTARTFADE FadeStart;	// �t�F�[�h�X�^�[�g����
		P_FADE_COL_LOAD pFadeCol;	// �t�F�[�h(�F�w��)
		P_FADE_POS_LOAD pFadePos;	// �t�F�[�h(�ʒu)
		bool bSkip;					// �t�F�[�h�̃X�L�b�v
	} FADEOUT_LOAD, *P_FADEOUT_LOAD;
	// 2DPresent���[�h
	typedef struct PRESENTS_LOAD
	{
		PRESENTS_LOAD()
		{
			pFadeIn = NULL;
			pFadeOut = NULL;
			pCooperation = NULL;
		}
		P_FADEIN_LOAD pFadeIn;				// �t�F�[�h�C���̃��[�h
		P_FADEOUT_LOAD pFadeOut;			// �t�F�[�h�A�E�g�̃��[�h
		P_COOPERATION_LOAD pCooperation;	// �����̃��[�h
	} PRESENTS_LOAD, *P_PRESENTS_LOAD;


	typedef struct UI_LOAD
	{
		UI_LOAD()
		{
			pos = D3DVECTOR3_ZERO;
			col = D3DXCOLOR_INI;
			size = D3DVECTOR2_ZERO;
			tex_first = D3DVECTOR2_ZERO;
			tex_last = { 1.0f,1.0f };
			fRot = 0.0f;
			nTexType = 0;
			nOffsetType = 0;
			pPresents = NULL;
		}
		D3DXVECTOR3 pos;			// �ʒu
		D3DXCOLOR col;				// �F
		D3DXVECTOR2 size;			// �T�C�Y
		D3DXVECTOR2 tex_first;		// �ŏ��̃e�N�X�`���[���W
		D3DXVECTOR2 tex_last;		// �Ō�̃e�N�X�`���[���W
		float fRot;					// �p�x
		int nTexType;				// �e�N�X�`���[�^�C�v
		int nOffsetType;			// �I�t�Z�b�g
		bool nUse;
		P_PRESENTS_LOAD pPresents;	// 2D�v���[���c�̃��[�h
	} UI_LOAD, *P_UI_LOAD;

	/* �֐� */
	// �R���X�g���N�^
	CUi();
	// �f�X�g���N�^
	~CUi();
	// ����������
	void Init(void);
	// �I������
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// ����������̏���
	// ����1:�I�u�W�F�N�g�^�C�v
	// ����2:����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	)
	{};
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
	// �V�[��2D�̎擾
	CScene_TWO * GetScene_Two(void);
	// �v���[���c2D�̎擾
	C2DPresents * GetPresents(void);

	static HRESULT Load(void);						// �e�L�X�g�̓ǂݍ���
	// UI�}�l�[�W���[�̓ǂݍ���
	static HRESULT UiManagerLoad(void);
	// UI�X�N���v�g�̓ǂݍ���
	static HRESULT UiScriptLoad(void);

	static void UnLoad(void);						// UnLoad����
	// �쐬����(�V�[���Ǘ�)
	static CUi * Create(
		UITYPE const &Uitype,
		int const &nUi
	);
	// �쐬����(�l�Ǘ�)
	static std::unique_ptr<CUi> Create_Self(
		UITYPE const &Uitype,
		int const &nUi
	);
	// ���[�h�������̂��쐬����(�V�[���Ǘ�)
	static CUi::VEC_P_UI LoadCreate(UITYPE const &Uitype);
	// ���[�h�������̂��쐬����(�l�Ǘ�)
	static VEC_UNI_UI LoadCreate_Self(UITYPE const &Uitype);
	// �g�p��Ԃ��擾
	static bool GetUse(
		UITYPE const &uitype
	) { return m_sta_UiUse[uitype]; };
	// �g�p��Ԃ�ݒ�
	static void SetUse(
		bool const &bUse,
		UITYPE const &uitype
		) { m_sta_UiUse[uitype] = bUse; };
	// �w�肵��UI���I��������
	static void End_Ui(
		UITYPE const &uitype
	);
	// �w�肵��UI�̏��擾
	static std::vector<UI_LOAD> GetUi(
		UITYPE const &uitype
	) { return m_vec_UiLoad[uitype]; };
protected:
private:
	/* �񋓌^ */

	/* �֐� */
	// ������ //

	// �X�V //

	/* �ϐ� */
	static std::vector<UI_LOAD> m_vec_UiLoad[UITYPE_MAX];	// Ui�ǂݍ��ݗp�ϐ�
	static bool	m_sta_UiUse[UITYPE_MAX];					// ����UI�̎g�p���
	static std::vector<std::string> m_vec_String;			// �t�@�C�����ǂݏ����p
	CScene_TWO * m_pScene_two;								// �V�[��2D
	C2DPresents * m_pPresents;								// 2D�v���[���c
	UITYPE m_Uitype;										// UI�^�C�v
	int m_nUi;												// UI

	// �����ɕK�v�ȕϐ�
	// int�^
	std::vector<int> m_vec_nNumber;							// int���
	// bool�^
	std::vector<bool> m_vec_Bool;							// bool���
	// �V�[���N���X
	std::unique_ptr<CScene> m_pScene;						// �V�[�����

};
#endif
