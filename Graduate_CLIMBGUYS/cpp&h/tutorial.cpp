// ------------------------------------------
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Nishiyama Koki
//
// ------------------------------------------
#include "tutorial.h"
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
#include "camera.h"
#include "sound.h"
#include "scene.h"
#include "manager.h"
#include "keyboard.h"

/* �`�� */
#include "fade.h"



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
			CManager::SetMode(CManager::MODE_GAME);
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
