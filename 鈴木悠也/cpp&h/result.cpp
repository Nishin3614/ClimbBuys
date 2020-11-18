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
	CBg::Create();

	// �������ꂽ�t�B�[���h�u���b�N�̐���
	CConnect_fieldblock::Create(CGame::STAGE_1);

	// �v���C���[
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// �v���C���[�̐���	�����I
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(50.0, 300.0f, -50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(-50.0, 300.0f, 50.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(50.0, 300.0f, 50.0f));
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