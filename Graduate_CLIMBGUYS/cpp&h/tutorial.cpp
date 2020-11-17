// ------------------------------------------
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Nishiyama Koki
//
// ------------------------------------------
#include "tutorial.h"
#include "manager.h"
#include "keyboard.h"
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "collision.h"
#include "XInputPad.h"

/* �`�� */
#include "fade.h"
#include "character.h"
#include "3Deffect.h"
#include "floor.h"
#include "meshobit.h"
#include "meshdome.h"
#include "meshsphere.h"
#include "meshwall.h"
#include "player.h"
#include "ui_group.h"
#include "3Dmap.h"
#include "stand.h"
#include "debugproc.h"
#include "meshbox.h"
#include "connect_fieldblock.h"
#include "connectblock.h"
#include "damagefloor.h"
#include "bg.h"
#include "tutorialUI.h"

// ------------------------------------------
//
// �}�N����`
//
// ------------------------------------------

// ------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------

// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CTutorial::CTutorial()
{
	// �X�e�[�W����J�E���g��ݒ�
	m_nDeterminationCnt = 120;
	m_pTutorialUI = nullptr;
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CTutorial::~CTutorial()
{
}

// ------------------------------------------
// ����������
// ------------------------------------------
void CTutorial::Init(void)
{
	// ���[�h������
	CBaseMode::Init();
	// �u���b�N�̐ÓI�ϐ��̏���������
	CBaseblock::BlockStaticValue();

	// �`���[�g���A��UI�̐���
	CUi::LoadCreate(CUi::UITYPE_TUTORIAL);
	m_pTutorialUI = CTutorialUI::Create();

	// 3D�G�t�F�N�g�̐���
	C3DEffect::Create();

	// �����I�w�i�̐���
	CBg::Create();

	// �������ꂽ�t�B�[���h�u���b�N�̐���
	CConnect_fieldblock::Create(CGame::STAGE_1);
	// �������ꂽ�u���b�N�̍X�V�u���b�N����
	//CConnectblock::TestCreate();

	// �v���C���[
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// �v���C���[�̐���	�����I
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(-50.0, 300.0f, 50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(50.0, 300.0f, 50.0f));

	// �_���[�W���̐���
	CDamageFloor::Create();
}

// ------------------------------------------
// �I������
// ------------------------------------------
void CTutorial::Uninit(void)
{
	// ���[�h�I��
	CBaseMode::Uninit();

	if (m_pTutorialUI)
	{
		// �`���[�g���A��UI�̏I��
		m_pTutorialUI->Uninit();
		m_pTutorialUI = nullptr;
	}
}

// ------------------------------------------
// �X�V����
// ------------------------------------------
void CTutorial::Update(void)
{
	// NULL�`�F�b�N
	if (m_pTutorialUI)
	{
		// �`���[�g���A��UI�̍X�V
		m_pTutorialUI->Update();
	}

//#ifdef _DEBUG
	CXInputPad *InpudPad[(int)PLAYER_TAG::PLAYER_MAX] = {};

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		InpudPad[nCnt] = CManager::GetPad((PLAYER_TAG)nCnt);

		// �Q�[���J��
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN) || InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_START, 1))
		{
			// �S�����������������Ƃ�
			if (m_pTutorialUI->Ready(nCnt))
			{
				// �t�F�[�h��Ԃ������N�����Ă��Ȃ���ԂȂ�
				if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
				{
					CManager::GetFade()->SetFade(CManager::MODE_GAME);
				}
			}
		}
	}

	CDebugproc::Print("�`���[�g���A���J�E���g < %d\n" ,m_nDeterminationCnt);
//#endif // _DEBUG

	// �J�E���g��0�ȉ��ɂȂ�����Q�[���Ɉړ�
	if (m_nDeterminationCnt <= 0)
	{
		m_nDeterminationCnt = 0;
		// �t�F�[�h��Ԃ������N�����Ă��Ȃ���ԂȂ�
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
	}

	if (CStand::GetDetermination() && m_nDeterminationCnt >= 0)
	{
		m_nDeterminationCnt--;
	}
	else
	{
		m_nDeterminationCnt = 120;
	}


	// ���[�h�X�V
	CBaseMode::Update();
}

// ------------------------------------------
// �`�揈��
// ------------------------------------------
void CTutorial::Draw(void)
{
	// ���[�h�`��
	CBaseMode::Draw();

	if (m_pTutorialUI)
	{
		// �`���[�g���A��UI�̕`��
		m_pTutorialUI->Draw();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTutorial * CTutorial::Create(void)
{
	// �ϐ��錾
	CTutorial * pTutorial = new CTutorial;
	// ����������
	pTutorial->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pTutorial;
}