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
	CStand::SetDeterminationCnt(120);
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

	// ���̐���
	CFloor::Create(D3DVECTOR3_ZERO,D3DXVECTOR3(1000.0f,0.0f, 1000.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DVECTOR3_ZERO,2,2,0);

	// ����̐���
	CStand::CreateStand_Tutorial();

	// �v���C���[�̐���
	CPlayer *pPlayer[(int)PLAYER_TAG::PLAYER_MAX] = {};

	// �v���C���[�̐���	�����I
	pPlayer[(int)PLAYER_TAG::PLAYER_1] = CPlayer::Create(PLAYER_TAG::PLAYER_1, D3DXVECTOR3(0.0, 300.0f, 0.0f));
	pPlayer[(int)PLAYER_TAG::PLAYER_2] = CPlayer::Create(PLAYER_TAG::PLAYER_2, D3DXVECTOR3(100.0f, 300.0f, 0.0f));
}

// ------------------------------------------
// �I������
// ------------------------------------------
void CTutorial::Uninit(void)
{
	// ���[�h�I��
	CBaseMode::Uninit();
}

// ------------------------------------------
// �X�V����
// ------------------------------------------
void CTutorial::Update(void)
{
#ifdef _DEBUG
	// �Q�[���J��
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// �t�F�[�h��Ԃ������N�����Ă��Ȃ���ԂȂ�
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
	}


#endif // _DEBUG

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
