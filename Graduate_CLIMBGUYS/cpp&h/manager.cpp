// ----------------------------------------------------------------------------------------------------
//
// �}�l�[�W���[�����̐���[manager.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------
/* My */
#include "manager.h"

/* ���̓f�o�C�X */
#include "keyboard.h"
#include "XInputPad.h"
#include "mouse.h"
#include "keyconfig.h"

/* �T�E���h */
#include "sound.h"

/* �f�o�b�O�\�� */
#include "debugproc.h"

/* �`�� */
#include "scene.h"
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "scene_two.h"
#include "loadscreen.h"

// ----------------------------------------------------------------------------------------------------
//
// �}�N���֐�
//
// ----------------------------------------------------------------------------------------------------
#define STARTMODE (MODE_TITLE)


// ----------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------
CRenderer		*CManager::m_renderer								= NULL;					// �����_���[
CKeyboard		* CManager::m_keyboard								= NULL;					// �L�[�{�[�h
CXInputPad		* CManager::m_pPad[(int)PLAYER_TAG::PLAYER_MAX]		= {};					// �Q�[���p�b�h
CMouse			* CManager::m_mouse									= NULL;					// �}�E�X
CKeyConfig		* CManager::m_keyconfig								= NULL;					// �L�[�ڍ�
CSound			* CManager::m_sound									= NULL;					// �T�E���h
CFade			* CManager::m_fade									= NULL;					// �t�F�[�h
CBaseMode		* CManager::m_BaseMode								= NULL;					// �x�[�X���[�h
CManager::MODE	CManager::m_mode									= CManager::MODE_TITLE;	// ���[�h
bool			CManager::m_bWire									= false;				// ���C���[
CLoadScreen		* CManager::m_pLoadScreen							= NULL;					// �ǂݍ��݉��

// ----------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// ----------------------------------------------------------------------------------------------------
CManager::CManager()
{
}

// ----------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// ----------------------------------------------------------------------------------------------------
CManager::~CManager()
{
}

// ----------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------
HRESULT CManager::Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance)
{
	/* �������̐��� */
	/* ���̓f�o�C�X */
	// �L�[�{�[�h�̐���
	m_keyboard = new CKeyboard;
	// �W���C�p�b�h�̐���
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_pPad[nCnt] = new CXInputPad;
	}
	// �}�E�X�̐���
	m_mouse = new CMouse;
	// �L�[�ڍׂ̐���
	m_keyconfig = new CKeyConfig;
	/* �Q�[���Ɍ������Ȃ����� */
	// �T�E���h�̐���
	m_sound = new CSound;
	// �����_�����O�̐���
	m_renderer = new CRenderer;
	// ���[�h�̐ݒ�
	m_mode = STARTMODE;
	/* ������ */
	// �L�[�{�[�h
	if (!m_keyboard->Init(hInstance, hWnd) == S_OK)
	{
		m_keyboard->Uninit();
		delete m_keyboard;
		m_keyboard = NULL;
	}
	// �W���C�p�b�h
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (!m_pPad[nCnt]->Init(hInstance, hWnd, (PLAYER_TAG)nCnt) == S_OK)
		{
			//m_pPad[nCnt]->Uninit();
			//delete m_pPad[nCnt];
			//m_pPad[nCnt] = nullptr;
		}
	}
	// �}�E�X
	if (!m_mouse->Init(hInstance, hWnd) == S_OK)
	{
		m_mouse->Uninit();
		delete m_mouse;
		m_mouse = NULL;
	}
	// �L�[�ڍ�
	m_keyconfig->Init();
	// �T�E���h
	if (!m_sound->Init(hWnd) == S_OK)
	{
		m_sound->Uninit();
		delete m_sound;
		m_sound = NULL;
		return E_FAIL;
	}
	// �����_�����O
	if (!m_renderer->Init(hWnd, bWindow) == S_OK)
	{
		m_renderer->Uninit();
		delete m_renderer;
		m_renderer = NULL;
		return E_FAIL;
	}

	// �J�����Ȃǂ̐ݒ�
	// �ǂ̃v���C���[�ԍ���ǔ����邩
	// �Q�[���̃J�����̃A�b�v�f�[�g�ł�����w�肷�邩��

	// �ǂݍ��݉�ʂ̐���
	m_pLoadScreen = CLoadScreen::Create();

	// ��ʑJ��
	m_fade = CFade::Create(STARTMODE);

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	/* �������̔j�� */
	// ���̓f�o�C�X
	CInput::ReleaseAll();
	// �L�[�{�[�h
	if (m_keyboard != NULL)
	{
		m_keyboard->Uninit();
		delete m_keyboard;
		m_keyboard = NULL;
	}
	// �W���C�p�b�h
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (m_pPad[nCnt])
		{
			m_pPad[nCnt]->Uninit();
			delete m_pPad[nCnt];
			m_pPad[nCnt] = NULL;
		}
	}

	// �}�E�X
	if (m_mouse != NULL)
	{
		m_mouse->Uninit();
		delete m_mouse;
		m_mouse = NULL;
	}
	// �L�[�ڍ�
	if (m_keyconfig != NULL)
	{
		m_keyconfig->Uninit();
		delete m_keyconfig;
		m_keyconfig = NULL;
	}
	// �T�E���h
	if (m_sound != NULL)
	{
		m_sound->Uninit();
		delete m_sound;
		m_sound = NULL;
	}
	// �����_�����O
	if (m_renderer != NULL)
	{
		m_renderer->Uninit();
		delete m_renderer;
		m_renderer = NULL;
	}
	// ��ʑJ��
	if (m_fade != NULL)
	{
		m_fade->Uninit();
		delete m_fade;
		m_fade = NULL;
	}
	// �^�C�g��
	if (m_BaseMode != NULL)
	{
		m_BaseMode->Uninit();
		delete m_BaseMode;
		m_BaseMode = NULL;
	}
	// �ǂݍ��݉��
	if (m_pLoadScreen != NULL)
	{
		m_pLoadScreen->Uninit();
		delete m_pLoadScreen;
		m_pLoadScreen = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------
void CManager::Update(void)
{
	// ���̓f�o�C�X�̃{�^����������Ă��邩���ۂ̏�����
	CInput::InitState();
	// �L�[�{�[�h
	if (m_keyboard != NULL)
	{
		m_keyboard->Update();
	}
	// �W���C�p�b�h
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (m_pPad[nCnt])
		{
			m_pPad[nCnt]->Update();
		}
	}
	// �}�E�X
	if (m_mouse != NULL)
	{
		m_mouse->Update();
	}
	// �����_�����O
	m_renderer->Update();
	// �x�[�X���[�h��NULL�`�F�b�N
	if (m_BaseMode != NULL)
	{
		// �x�[�X���[�h�̍X�V����
		m_BaseMode->Update();
#ifdef _DEBUG
		// �x�[�X���[�h�̃f�o�b�O����
		m_BaseMode->Debug();
#endif // _DEBUG
	}
	// �O��̃X�e�B�b�N���
	CCalculation::SaveLastStickInfo();

	// ��ʑJ��
	m_fade->Update();
}

// ----------------------------------------------------------------------------------------------------
//
// �`��֐�
//
// ----------------------------------------------------------------------------------------------------
void CManager::Draw(void)
{
	// �����_�����O
	m_renderer->Draw();
}

// ----------------------------------------------------------------------------------------------------
// ���[�h�ݒ�֐�
// ----------------------------------------------------------------------------------------------------
void CManager::SetMode(MODE const &mode)
{
	// �x�[�X���[�h��NULL�`�F�b�N
	if (m_BaseMode != NULL)
	{
		// �x�[�X���[�h�̏I������
		m_BaseMode->Uninit();
	}
	// �O�̃��[�h�I��
	switch (m_mode)
	{
		// �^�C�g��
	case MODE_TITLE:
		break;
		// �`���[�g���A��
	case MODE_TUTORIAL:
		break;
		// �Q�[��
	case MODE_GAME:
		break;
		// ���U���g
	case MODE_RESULT:
		break;
		// �����L���O
	case MODE_RANKING:
		break;
	default:
		break;
	}

	m_mode = mode;	// ���݂̃��[�h����

	// ���݂̃��[�h������
	switch (mode)
	{
		// �^�C�g��
	case MODE_TITLE:
		// �^�C�g������
		m_BaseMode = CTitle::Create();
		break;
		// �`���[�g���A��
	case MODE_TUTORIAL:
		// �`���[�g���A������
		m_BaseMode = CTutorial::Create();
		// �`���[�g���A��BGM
		break;
		// �Q�[��
	case MODE_GAME:
		// �Q�[������
		m_BaseMode = CGame::Create();
		// �Q�[��BGM
		break;
		// ���U���g
	case MODE_RESULT:
		// ���U���g����
		m_BaseMode = CResult::Create();
		// ���U���gBGM
		break;
		// �����L���O
	case MODE_RANKING:
		// �Q�[������
		//m_BaseMode = CRanking::Create();
		// �Q�[��BGM
		break;
	default:
		break;
	}
}