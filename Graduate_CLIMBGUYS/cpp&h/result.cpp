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
#include "connectblock.h"

//=============================================================================
//
// �R���X�g���N�^
//
//=============================================================================
CResult::CResult()
{
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

	// �`���[�g���A��UI�̐���
	CUi::LoadCreate(CUi::UITYPE_TUTORIAL);

	// 3D�G�t�F�N�g�̐���
	C3DEffect::Create();

	// �����I�w�i�̐���
	CBg::Create();

	// �������ꂽ�t�B�[���h�u���b�N�̐���
	CConnect_fieldblock::Create(CGame::STAGE_1);
	// �������ꂽ�u���b�N�̍X�V�u���b�N����
	CConnectblock::TestCreate();

	//// ���̐���
	//CFloor::Create(D3DVECTOR3_ZERO,D3DXVECTOR3(1000.0f,0.0f, 1000.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DVECTOR3_ZERO,2,2,0);
	////// ����̐���
	//CStand::CreateStand_Tutorial();

	// �v���C���[
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// �v���C���[�̐���	�����I
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(-100.0, 300.0f, -100.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(100.0f, 300.0f, 0.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_3] = CPlayer::Create(PLAYER_TAG::PLAYER_3, D3DXVECTOR3(0.0, 300.0f, 100.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_4] = CPlayer::Create(PLAYER_TAG::PLAYER_4, D3DXVECTOR3(100.0f, 300.0f, 100.0f));
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

	// �^�C�g���J��
	if (CCalculation::PressAnyButton())
	{
		// �t�F�[�h��Ԃ������N�����Ă��Ȃ���ԂȂ�
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
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
}

//=============================================================================
//
// �^�C�g���̐���
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