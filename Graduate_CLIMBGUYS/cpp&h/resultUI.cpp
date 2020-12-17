// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���U���gUI[resultUI.cpp]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "resultUI.h"
#include "basemode.h"
#include "debugproc.h"
#include "scene_two.h"
#include "multinumber.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
/* D3DXVECTOR3((��l + �� * nCnt), ((�T�C�YY / 2 + �T�C�YY[���̉�ʊO�ɏo������]) + (�ړ��� * ���炷�t���[����) * nCnt), 0.0f) */
#define RESULT_UI_FRAME_POS								(D3DXVECTOR3((163.0f + 320.0f * nCnt), (1080.0f + 210.0f * nCnt), 0.0f))		// �g�̈ʒu
#define RESULT_UI_FRAME_SIZE							(D3DXVECTOR2(326.0f, SCREEN_HEIGHT))	// �g�̃T�C�Y
#define RESULT_UI_FRAME_MOVE_STOP_POS_Y					(360.0f)								// �g�̈ړ����~�܂�ʒuY

#define RESULT_UI_RANK_POS								(D3DXVECTOR3((265.0f + 320.0f * (nCnt - (int)RESULT_UI::RANK_01)), (740.0f + 210.0f * (nCnt - (int)RESULT_UI::RANK_01)), 0.0f))		// ���ʂ̈ʒu
#define RESULT_UI_RANK_SIZE								(D3DXVECTOR2(100.0f, 100.0f))			// ���ʂ̃T�C�Y
#define RESULT_UI_RANK_MOVE_STOP_POS_Y					(55.0f)									// ���ʂ̈ړ����~�܂�ʒuY

#define RESULT_UI_DEATH_POS								(D3DXVECTOR3((160.0f + 320.0f * (nCnt - (int)RESULT_UI::DEATH_01)), (1080.0f + 210.0f * (nCnt - (int)RESULT_UI::DEATH_01)), 0.0f))	// �����̈ʒu
#define RESULT_UI_DEATH_SIZE							(D3DXVECTOR2(200.0f, 80.0f))			// �����̃T�C�Y
#define RESULT_UI_DEATH_MOVE_STOP_POS_Y					(685.0f)								// �����̈ړ����~�܂�ʒuY

#define RESULT_SCORE_SURVIVAL_TIME_POS					(D3DXVECTOR3((230.0f + 320.0f * nCnt), (485.0f + SCREEN_HEIGHT), 0.0f))	// �������Ԃ̈ʒu
#define RESULT_SCORE_SURVIVAL_TIME_MOVE_STOP_POS_Y		(485.0f)																// �������Ԃ̈ړ����~�܂�ʒuY

#define RESULT_SCORE_PRESS_BLOCK_POS					(D3DXVECTOR3((230.0f + 320.0f * nCnt), (585.0f + SCREEN_HEIGHT), 0.0f))	// �u���b�N���������񐔂̈ʒu
#define RESULT_SCORE_PRESS_BLOCK_MOVE_STOP_POS_Y		(585.0f)																// �u���b�N���������񐔂̈ړ����~�܂�ʒuY

#define RESULT_SCORE_SIZE								(D3DXVECTOR2(35.0f, 50.0f))			// �����̃T�C�Y
#define RESULT_UI_MOVE									(D3DXVECTOR3(0.0f, -70.0f, 0.0f))	// ���U���gUI�̈ړ���

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::RECORD		CResultUI::m_Record[(int)PLAYER_TAG::PLAYER_MAX] = {};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CResultUI::CResultUI()
{
	// ������
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		m_pScene2D[nCnt]	= nullptr;			// �V�[��2D
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_ResultScore[nCnt] = {};				// ���U���g�X�R�A
	}
	m_move					= D3DVECTOR3_ZERO;	// �ړ���
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CResultUI::~CResultUI()
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		m_Record[nCnt] = {};				// �L�^���
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Init(void)
{
	// ������
	m_move = RESULT_UI_MOVE;	// �ړ���

	// �g�̏����ݒ�
	InitSettingFrame();
	// �e�X�R�A�̏����ݒ�
	InitSettingScore();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			m_pScene2D[nCnt] = nullptr;			// �V�[��2D
		}
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- ���U���g�X�R�A --- */
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			m_ResultScore[nCnt].m_pSurvivalTime = nullptr;	// ��������
		}
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			m_ResultScore[nCnt].m_pPressBlock = nullptr;	// �u���b�N����������
		}
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Update(void)
{
	// �g�̍X�V�ݒ�
	UpdateSettingFrame();

	// �e�X�R�A�̍X�V�ݒ�
	UpdateSettingScore();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::Draw(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			// �`��
			m_pScene2D[nCnt]->Draw();
		}
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- ���U���g�X�R�A�̕`�� --- */
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			m_ResultScore[nCnt].m_pSurvivalTime->Draw();	// ��������
		}
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			m_ResultScore[nCnt].m_pPressBlock->Draw();		// �u���b�N����������
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CResultUI * CResultUI::Create(void)
{
	// �ϐ��錾
	CResultUI * pResultUI;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pResultUI = new CResultUI();

	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		// �V�[��2D�̐���
		pResultUI->m_pScene2D[nCnt] = CScene_TWO::Create(CScene_TWO::OFFSET_TYPE_CENTER, D3DVECTOR3_ZERO, D3DVECTOR2_ZERO, (CTexture_manager::TYPE_RESULT_UI_FRAME_1P + nCnt));
	}
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		// ���U���g�X�R�A�̐���
		pResultUI->m_ResultScore[nCnt].m_pSurvivalTime	= CMultiNumber::Create(D3DVECTOR3_ZERO, D3DVECTOR2_ZERO, 0, 3, 20.0f);
		pResultUI->m_ResultScore[nCnt].m_pPressBlock	= CMultiNumber::Create(D3DVECTOR3_ZERO, D3DVECTOR2_ZERO, 0, 2, 20.0f);
	}
	// ����������
	pResultUI->Init();

	// ���������I�u�W�F�N�g��Ԃ�
	return pResultUI;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CResultUI::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// unload����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �L�^���̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::SetRecord(PLAYER_TAG PlayerNum, CPlayer::RECORD record)
{
	m_Record[(int)PlayerNum] = record;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �g�̏����ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::InitSettingFrame(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			// �g�ɋ��ʂ���ݒ�
			if (nCnt <= (int)RESULT_UI::FRAME_4P)
			{
				// �ʒu�̐ݒ�
				m_pScene2D[nCnt]->SetPosition(RESULT_UI_FRAME_POS);
				// �T�C�Y�̐ݒ�
				m_pScene2D[nCnt]->SetSize(RESULT_UI_FRAME_SIZE);
			}
			// ���ʂɋ��ʂ���ݒ�
			else if (nCnt <= (int)RESULT_UI::RANK_04)
			{
				// �ʒu�̐ݒ�
				m_pScene2D[nCnt]->SetPosition(RESULT_UI_RANK_POS);
				// �T�C�Y�̐ݒ�
				m_pScene2D[nCnt]->SetSize(RESULT_UI_RANK_SIZE);
				// �e�N�X�`���̐ݒ�
				m_pScene2D[nCnt]->BindTexture(CTexture_manager::TYPE_RESULT_UI_RANK_01 + m_Record[nCnt - (int)RESULT_UI::RANK_01].nRanking - 1);
			}
			// �����ɋ��ʂ���ݒ�
			else if (nCnt <= (int)RESULT_UI::DEATH_04)
			{
				// �ʒu�̐ݒ�
				m_pScene2D[nCnt]->SetPosition(RESULT_UI_DEATH_POS);
				// �T�C�Y�̐ݒ�
				m_pScene2D[nCnt]->SetSize(RESULT_UI_DEATH_SIZE);
				// �e�N�X�`���̐ݒ�
				m_pScene2D[nCnt]->BindTexture(CTexture_manager::TYPE_RESULT_UI_DEATH_01 + m_Record[nCnt - (int)RESULT_UI::DEATH_01].DieCause);
			}

			// ������
			m_pScene2D[nCnt]->Init();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�X�R�A�̏����ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::InitSettingScore(void)
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- ���U���g�X�R�A�̏����ݒ� --- */
		// ��������
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			// �ʒu�̐ݒ�
			m_ResultScore[nCnt].m_pSurvivalTime->SetPos(RESULT_SCORE_SURVIVAL_TIME_POS);
			// �T�C�Y�̐ݒ�
			m_ResultScore[nCnt].m_pSurvivalTime->SetSize(RESULT_SCORE_SIZE);
			// �X�R�A�̐ݒ�
			m_ResultScore[nCnt].m_pSurvivalTime->SetScore(m_Record[nCnt].nTime);
		}
		// �u���b�N����������
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			// �ʒu�̐ݒ�
			m_ResultScore[nCnt].m_pPressBlock->SetPos(RESULT_SCORE_PRESS_BLOCK_POS);
			// �T�C�Y�̐ݒ�
			m_ResultScore[nCnt].m_pPressBlock->SetSize(RESULT_SCORE_SIZE);
			// �X�R�A�̐ݒ�
			m_ResultScore[nCnt].m_pPressBlock->SetScore(m_Record[nCnt].nPushCnt);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �g�̍X�V�ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::UpdateSettingFrame(void)
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::UI_MAX; nCnt++)
	{
		if (m_pScene2D[nCnt])
		{
			// �ʒu
			D3DXVECTOR3 pos = D3DVECTOR3_ZERO;

			// �ʒu�̎擾
			pos = m_pScene2D[nCnt]->GetPosition();

			// �ړ�
			pos.y += m_move.y;

			// �g�ɋ��ʂ���ݒ�
			if (nCnt <= (int)RESULT_UI::FRAME_4P)
			{
				// �ړ����~�߂�
				if (pos.y <= RESULT_UI_FRAME_MOVE_STOP_POS_Y)
				{
					pos.y = RESULT_UI_FRAME_MOVE_STOP_POS_Y;
				}
			}
			// ���ʂɋ��ʂ���ݒ�
			else if (nCnt <= (int)RESULT_UI::RANK_04)
			{
				// �ړ����~�߂�
				if (pos.y <= RESULT_UI_RANK_MOVE_STOP_POS_Y)
				{
					pos.y = RESULT_UI_RANK_MOVE_STOP_POS_Y;
				}
			}
			// �����ɋ��ʂ���ݒ�
			else if (nCnt <= (int)RESULT_UI::DEATH_04)
			{
				// �ړ����~�߂�
				if (pos.y <= RESULT_UI_DEATH_MOVE_STOP_POS_Y)
				{
					pos.y = RESULT_UI_DEATH_MOVE_STOP_POS_Y;
				}
			}

			// �ʒu�̐ݒ�
			m_pScene2D[nCnt]->SetPosition(pos);
			m_pScene2D[nCnt]->Set_Vtx_Pos();
			// �X�V
			m_pScene2D[nCnt]->Update();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�X�R�A�̍X�V�ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CResultUI::UpdateSettingScore(void)
{
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		/* --- ���U���g�X�R�A�̍X�V --- */
		// ��������
		if (m_ResultScore[nCnt].m_pSurvivalTime)
		{
			// �ʒu
			D3DXVECTOR3 SurvivalTimePos = m_ResultScore[nCnt].m_pSurvivalTime->GetPos();
			SurvivalTimePos.y += m_move.y;

			// �ړ����~�߂�
			if (SurvivalTimePos.y <= RESULT_SCORE_SURVIVAL_TIME_MOVE_STOP_POS_Y)
			{
				SurvivalTimePos.y = RESULT_SCORE_SURVIVAL_TIME_MOVE_STOP_POS_Y;
			}
			// �ʒu�̐ݒ�
			m_ResultScore[nCnt].m_pSurvivalTime->SetPos(SurvivalTimePos);

			// �X�V
			m_ResultScore[nCnt].m_pSurvivalTime->Update();
		}
		// �u���b�N����������
		if (m_ResultScore[nCnt].m_pPressBlock)
		{
			// �ʒu
			D3DXVECTOR3 PressBlockPos = m_ResultScore[nCnt].m_pPressBlock->GetPos();
			PressBlockPos.y += m_move.y;

			// �ړ����~�߂�
			if (PressBlockPos.y <= RESULT_SCORE_PRESS_BLOCK_MOVE_STOP_POS_Y)
			{
				PressBlockPos.y = RESULT_SCORE_PRESS_BLOCK_MOVE_STOP_POS_Y;
			}
			// �ʒu�̐ݒ�
			m_ResultScore[nCnt].m_pPressBlock->SetPos(PressBlockPos);

			// �X�V
			m_ResultScore[nCnt].m_pPressBlock->Update();
		}
	}
}