//=============================================================================
//
// ���U���g�̏��� [result.cpp]
// Author : Ryouma Inoue
//
//=============================================================================
#include "result.h"
#include "fade.h"

#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "player.h"
#include "manager.h"
#include "ui.h"
#include "keyboard.h"
#include "3Deffect.h"
#include "bg.h"
#include "connect_fieldblock.h"
#include "resultUI.h"
#include "3Dparticle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_VICTORY_POS	(D3DXVECTOR3(0.0f, 25.0f, -100.0f))		// ���҂̈ʒu
#define PLAYER_LOSER_POS	(D3DXVECTOR3(40.0f + (10.0f * m_nCntLoser), 25.0f, 50.0f + (-25.0f * m_nCntLoser)))		// �s�҂̈ʒu

//=============================================================================
//
// �R���X�g���N�^
//
//=============================================================================
CResult::CResult()
{
	// ������
	m_pResultUI				= nullptr;		// ���U���gUI
	m_nCntPressButton		= 0;			// �{�^������������
	m_nCntLoser				= 0;			// �s�҂̃J�E���g

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_pPlayer[nCnt] = nullptr;			// �v���C���[
	}
}

//=============================================================================
//
// �f�X�g���N�^
//
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//
// ����������
//
//=============================================================================
void CResult::Init()
{
	// ���[�h������
	CBaseMode::Init();

	// 3D�G�t�F�N�g�̐���
	C3DEffect::Create();

	// �����I�w�i�̐���
	CBg::Create(CTexture_manager::TYPE_BG);

	// �������ꂽ�t�B�[���h�u���b�N�̐���
	CConnect_fieldblock::Create(CGame::STAGE_1);

	// �v���C���[�̐���	�����I
	m_pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-50.0f, 25.0f, -50.0f));
	m_pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(50.0f, 25.0f, -50.0f));
	m_pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(-50.0f, 25.0f, 50.0f));
	m_pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(50.0f, 25.0f, 50.0f));

	// �v���C���[�̃��U���g���[�V����
	ResultMotion();
}

//=============================================================================
//
// �I������
//
//=============================================================================
void CResult::Uninit(void)
{
	// ���[�h�I��
	CBaseMode::Uninit();

	if (m_pResultUI)
	{
		// ���U���gUI�̏I��
		m_pResultUI->Uninit();
		m_pResultUI = nullptr;
	}

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		if (m_pPlayer[nCnt])
		{
			// �v���C���[�̏I��
			m_pPlayer[nCnt] = nullptr;
		}
	}
}

//=============================================================================
//
// �X�V����
//
//=============================================================================
void CResult::Update(void)
{
	// ���[�h�X�V
	CBaseMode::Update();

	// NULL�`�F�b�N
	if (m_pResultUI)
	{
		// ���U���gUI�̍X�V
		m_pResultUI->Update();
	}

	// �{�^�����������񐔂����Z
	if (CCalculation::PressAnyButton())
	{
		m_nCntPressButton++;
	}

	// �{�^�����������񐔂ɂ���ċN���鏈��
	switch (m_nCntPressButton)
	{
	case 1:
		// ���U���gUI����������Ă��Ȃ��Ƃ�
		if (!m_pResultUI)
		{
			// ���U���gUI�̐���
			m_pResultUI = CResultUI::Create();
		}
		break;

	case 2:
		// �t�F�[�h��Ԃ������N�����Ă��Ȃ���ԂȂ�
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
		break;
	}
}

//=============================================================================
//
// �`�揈��
//
//=============================================================================
void CResult::Draw(void)
{
	// ���[�h�`��
	CBaseMode::Draw();

	if (m_pResultUI)
	{
		// ���U���gUI�̕`��
		m_pResultUI->Draw();
	}
}

//=============================================================================
//
// ���U���g�̐���
//
//=============================================================================
CResult * CResult::Create(void)
{
	// �ϐ��錾
	CResult *pResult = new CResult;

	// ������
	pResult->Init();

	return pResult;
}

//=============================================================================
//
// ���U���g���[�V����
//
//=============================================================================
void CResult::ResultMotion(void)
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		// 1�ʂ̃v���C���[
		if (CResultUI::GetRecord(nCnt).nRanking == 1)
		{
			if (m_pPlayer[nCnt])
			{
				// �����_���ŏ������[�V�������s��
				m_pPlayer[nCnt]->SetMotion((CCharacter::MOTIONTYPE)CCalculation::GetRandomRange(CCharacter::MOTIONTYPE_VICTORY_01, CCharacter::MOTIONTYPE_VICTORY_04));
				// ���҂̈ʒu�̐ݒ�
				m_pPlayer[nCnt]->SetPos(PLAYER_VICTORY_POS);
			}
		}
		// 2�ʈȉ��̃v���C���[
		else
		{
			if (m_pPlayer[nCnt])
			{
				// �s�҂̃J�E���g�A�b�v
				m_nCntLoser++;

				// �o���U�C���[�V����
				m_pPlayer[nCnt]->SetMotion(CCharacter::MOTIONTYPE_BANZAI);
				// �s�҂̈ʒu�̐ݒ�
				m_pPlayer[nCnt]->SetPos(PLAYER_LOSER_POS);
			}
		}
	}
}