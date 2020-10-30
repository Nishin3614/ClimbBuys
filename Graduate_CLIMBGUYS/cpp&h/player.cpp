// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f������ [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "input.h"
#include "camera.h"
#include "ui.h"
#include "manager.h"
#include "fade.h"
#include "collision.h"
#include "game.h"
#include "XInputPad.h"
#include "keyboard.h"
#include "ui_group.h"
#include "meshdome.h"
#include "3Dparticle.h"
#include "debugproc.h"

#include "stand.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_GRAVITY				(0.1f)		// �v���C���[�̏d��
#define PLAYER_UPMOVELIMIT			(30.0f)		// �v���C���[�̏㏸�ړ��ʐ���
#define PLAYER_UNDERMOVELIMIT		(5.0f)		// �v���C���[�̉��~�ړ��ʐ���
#define DASH_TIME_MAX				(30)		// �_�b�V�����Ă��鎞��
#define DASH_ENABLE_STICK_RANGE		(0.8f)		// �_�b�V����L���ɂ���X�e�B�b�N�̌X��
#define PLAYER_STATUS_TXT			("data/LOAD/STATUS/PlayerStatus.txt")	// �v���C���[�̃X�e�[�^�X�̃e�L�X�g

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::PLAYER_STATUS CPlayer::m_PlayerStatus = {};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer(CHARACTER const &character) : CCharacter::CCharacter(character)
{
	m_nCntState			= 0;					// �X�e�[�g�J�E���g
	m_bDieFlag			= false;				// ���S�t���O
	m_bDashFlag			= false;				// �_�b�V���t���O
	m_nCntDashTime		= 0;					// �_�b�V�����̐؂�ւ��J�E���g
	m_PlayerStatusInit	= m_PlayerStatus;		// �v���C���[�̏����X�e�[�^�X
	CScene::SetObj(CScene::OBJ::OBJ_PLAYER);	// �I�u�W�F�N�g�^�C�v�̐ݒ�
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	// �o���[���L�����N�^�[������
	CCharacter::Init();

	// �p�b�h�̃|�C���^�擾
	m_pPad = CManager::CManager::GetPad(GetPlayerTag());
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	// �L�����N�^�[�I������
	CCharacter::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// ���L�����̍s������
	MyAction();

	// ���[�V�����ݒ菈��
	StatusMotion();
	// �L�����N�^�[�X�V
	CCharacter::Update();
	// ���E�n�_
	CCharacter::Limit();
	// �����蔻��̏���
	Collision();

	// ���S���肪�o���烊�U���g�ɑJ�ڂ���
	if (GetDie())
	{
		// ���S�֐�
		Die();
		if (CManager::GetFade()->GetFade() == CManager::GetFade()->FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���L�����s������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyAction(void)
{
	// ���L�����̈ړ�����
	MyMove();

	// �����I�_�b�V���̐؂�ւ�
	if (m_bDashFlag)
	{
		m_nCntDashTime++;

		if (m_nCntDashTime > DASH_TIME_MAX)
		{
			// ������
			m_nCntDashTime	= 0;
			m_bDashFlag		= false;
		}
	}
	CDebugproc::Print("\nPlayer�_�b�V�� %d\n", m_bDashFlag);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ړ�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyMove(void)
{
	// �ϐ��錾
	D3DXVECTOR3 move, rot;			// �ړ��ʁA��]
	D3DXVECTOR3 vec;				// �x�N�g��
	float fRot;						// ��]

	// ���擾
	rot = CCharacter::GetRotDest();								// �ړI��]��
	move = CCharacter::GetMove();								// �ړ���
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// �J������]
	vec = CCharacter::GetDirectionVec();						// �x�N�g��
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	// �ړ� //
	/* �L�[�{�[�h */
	// ��
	if (pKeyboard->GetKeyboardPress(DIK_A))
	{
		// ��
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = -D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.75f + fRot), 0.0f, cosf(D3DX_PI * 0.75f + fRot));
			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// ��O
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = -D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.25f + fRot), 0.0f, cosf(D3DX_PI * 0.25f + fRot));
			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// ��
		else
		{
			rot.y = -D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fRot), 0.0f, cosf(D3DX_PI * 0.5f + fRot));
			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
	}
	// �E
	else if (pKeyboard->GetKeyboardPress(DIK_D))
	{

		// ��
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f + fRot), 0.0f, cosf(-D3DX_PI * 0.75f + fRot));

			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// ��O
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f + fRot), 0.0f, cosf(-D3DX_PI * 0.25f + fRot));


			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// �E
		else
		{
			rot.y = D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f + fRot), 0.0f, cosf(-D3DX_PI * 0.5f + fRot));

			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
	}
	// ���ɍs��
	else if (pKeyboard->GetKeyboardPress(DIK_W))
	{
		rot.y = D3DX_PI * 0.0f + fRot;
		vec = D3DXVECTOR3(sinf(-D3DX_PI * 1.0f + fRot), 0.0f, cosf(-D3DX_PI * 1.0f + fRot));
		move.x += vec.x * m_PlayerStatus.fMove;
		move.z += vec.z * m_PlayerStatus.fMove;
	}
	// ��O�ɍs��
	else if (pKeyboard->GetKeyboardPress(DIK_S))
	{
		rot.y = D3DX_PI * 1.0f + fRot;
		vec = D3DXVECTOR3(sinf(D3DX_PI * 0.0f + fRot), 0.0f, cosf(D3DX_PI * 0.0f + fRot));
		move.x += vec.x * m_PlayerStatus.fMove;
		move.z += vec.z * m_PlayerStatus.fMove;
	}
	// ����ȊO
	else
	{

	}

	// �����I�L�[�{�[�h�W�����v
	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) && GetJumpAble())
	{
		move.y += m_PlayerStatus.fJump;
		SetJumpAble(false);
	}

	/* �Q�[���p�b�h */
	// �p�b�h�p //
	float fValueH, fValueV;		// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fAngle;				// �X�e�B�b�N�p�x�̌v�Z�p�ϐ�
	fAngle = 0.0f;				// �p�x

	if (m_pPad)
	{
		// �Q�[���p�b�h�̃X�e�B�b�N�����擾
		m_pPad->GetStickLeft(&fValueH, &fValueV);

		// �_�b�V�����Ă��Ȃ��Ƃ�
		if (!m_bDashFlag)
		{

			// �v���C���[�ړ�
			// �Q�[���p�b�h�ړ�
			if (fValueH != 0 || fValueV != 0)
			{
				// �p�x�̌v�Z
				fAngle = atan2f((float)fValueH, (float)fValueV);

				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				else if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				rot.y = fAngle + fRot;
				vec = D3DXVECTOR3(sinf(fAngle + fRot), 0.0f, cosf(fAngle + fRot));
				// �X�e�B�b�N�̊p�x�ɂ���ăv���C���[�ړ�
				move.x -= sinf(fAngle + fRot) * (m_PlayerStatus.fMove);
				move.z -= cosf(fAngle + fRot) * (m_PlayerStatus.fMove);
			}

			// �����I�W�����v ( �̂��ɒ��g�ς��\�� ���� )
			if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_A, 1) && GetJumpAble())
			{
				move.y += m_PlayerStatus.fJump;
				SetJumpAble(false);
			}

			// �X�e�B�b�N��������x�X�����Ƃ�
			if (fabs(fValueH / STICK_MAX_RANGE) > DASH_ENABLE_STICK_RANGE || fabs(fValueV / STICK_MAX_RANGE) > DASH_ENABLE_STICK_RANGE)
			{
				// �����I�^�b�N�� ( �̂��ɒ��g�ς��\�� ���� )
				if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_X, 1))
				{
					m_bDashFlag = true;

					switch (CCalculation::CheckPadStick())
					{
					case DIRECTION::LEFT:
					case DIRECTION::RIGHT:
					case DIRECTION::UP:
					case DIRECTION::DOWN:
						move.x -= vec.x * m_PlayerStatus.fDash;
						move.z -= vec.z * m_PlayerStatus.fDash;
						break;
					}
				}
			}
		}
	}

	// �W�����v���Ă���Ƃ��̊���
	if (!GetJumpAble())
	{
		move.x += ( -m_move.x) * m_PlayerStatus.fJumpInertia;
		move.z += ( -m_move.z) * m_PlayerStatus.fJumpInertia;
	}
	else
	{
		move.x += (-m_move.x) * m_PlayerStatus.fNormalInertia;
		move.z += (-m_move.z) * m_PlayerStatus.fNormalInertia;
	}

	if (vec.x < 0)
	{
		vec.x *= -1;
	}
	if (vec.z < 0)
	{
		vec.z *= -1;
	}
	CCharacter::SetDirectionVec(vec);
	// y�̏���ݒ�
	if (move.y > PLAYER_UPMOVELIMIT)
	{
		move.y = PLAYER_UPMOVELIMIT;
	}
	if (move.y < -PLAYER_UNDERMOVELIMIT)
	{
		move.y = -PLAYER_UNDERMOVELIMIT;
	}
	CCharacter::SetMove(move);

	// �L�����N�^�[�̏d�͉��Z����
	CCharacter::AddGravity(PLAYER_GRAVITY);
	CCharacter::SetRotDest(rot);

	//if (pKeyboard->GetKeyboardTrigger(DIK_C))
	//{
	//	CGame::GetPlayer(1)->SetRotDest(D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��Ԃɂ���Ẵ��[�V�����ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StatusMotion(void)
{
	/*
	// �W�����v��
	if (!CCharacter::GetbLanding())
	{
		SetMotion(MOTIONTYPE_JAMP);
	}
	// �ړ���
	else if (CCharacter::GetbMove())
	{
		// ���[�V�����ݒ�(�ړ�)
		SetMotion(MOTIONTYPE_MOVE);
	}
	// �ҋ@
	else
	{
		SetMotion(MOTIONTYPE_NEUTRAL);
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���ꔻ��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StandJudg(
	CScene_X * pScene_X,
	bool const & bJudg
)
{
	// ����I�u�W�F�N�g�Ȃ�
	if (pScene_X->GetObj() == CScene::OBJ_STAND)
	{
		// �v���C���[�^�O��1�v���C���[�Ȃ�
		if (this->GetPlayerTag() == PLAYER_TAG::PLAYER_1)
		{
			CStand * pStand = (CStand *)pScene_X;
			pStand->SetDetermination(bJudg);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��̏���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Collision(void)
{
	// �ϐ��錾
	CScene_X * pScene_X;		// �V�[��X���
	COLLISIONDIRECTION Direct;	// �����蔻��̕���
	bool bOn = false;			// ��̓����蔻��
	bool bUnder = false;		// ���̓����蔻��
	// �u���b�N���[�v
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK); nCntBlock++)
	{
		// NULL���
		pScene_X = NULL;
		// ���擾
		pScene_X = (CScene_X *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);
		// NULL�Ȃ�
		// ->�֐��𔲂���
		if (pScene_X == NULL)
		{
			continue;
		}
		// �����蔻��
		Direct = pScene_X->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&D3DXVECTOR3(25.0f, 40.0f, 25.0f),
			D3DXVECTOR3(0.0f, 20.0f, 0.0f)
		);
		// �u���b�N�̔���
		// �O
		if (Direct == COLLISIONDIRECTION::FRONT)
		{
			PushBlock(pScene_X, CBaseblock::GRID(0, 0, -1));
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::BACK)
		{
			PushBlock(pScene_X, CBaseblock::GRID(0, 0, 1));
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::LEFT)
		{
			PushBlock(pScene_X, CBaseblock::GRID(1, 0, 0));
		}
		// �E
		else if (Direct == COLLISIONDIRECTION::RIGHT)
		{
			PushBlock(pScene_X, CBaseblock::GRID(-1, 0, 0));
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::UP)
		{
			// �W�����v�\�ݒ�
			SetJumpAble(true);
			// ���ꔻ��
			StandJudg(pScene_X, true);
			// �v���C���[�����̃u���b�N�ɓ������Ă���
			bOn ^= true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::DOWN)
		{
			// �v���C���[�������u���b�N�ɓ������Ă���
  			bUnder ^= true;
		}
		else
		{
			// ���ꔻ��
			StandJudg(pScene_X, false);
		}
	}
	// ��������u���b�N�ɓ������Ă�����
	if (bOn && bUnder)
	{
		// �v���C���[�͎���
		Die();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����o������̃u���b�N�ɓ��������v���C���[�̉����o�����菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PushAfter_Collision(
	CBaseblock * pBaseBlock,	// �x�[�X�u���b�N���
	CBaseblock::GRID Grid		// �s�񍂏��
)
{
	// �ϐ��錾
	CPlayer * pPlayer = NULL;	// �v���C���[
	// �L�����N�^�[���[�v
	for (int nCntPlayer = 0; nCntPlayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntPlayer++)
	{
		// ���擾
		pPlayer = (CPlayer *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntPlayer);	// �v���C���[���
		// �擾�v���C���[�̏��NULL�Ȃ� ||
		// �擾�v���C���[�Ɠ������Ȃ�
		// ->���[�v�X�L�b�v
		if (pPlayer == NULL || pPlayer == this) continue;
		// ����
		COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;
		// ����������̕������擾
		Direct = pBaseBlock->Collision(
			pPlayer->GetObj(), &pPlayer->GetPos(), &pPlayer->GetPosOld(),
			&D3DXVECTOR3(25.0f, 40.0f, 25.0f),
			D3DXVECTOR3(0.0f, 20.0f, 0.0f)
		);
		// �擾�����v���C���[���u���b�N�ɓ������Ă����
		if (Direct != COLLISIONDIRECTION::NONE)
		{
			// �ϐ��錾
			CBaseblock::GRID PlayerGrid = Grid + pBaseBlock->GetGrid();	// �v���C���[�̍s��
			// �v���C���[�̈ʒu�ݒ�
			pPlayer->SetPos(PlayerGrid.GetPos(CBaseblock::GetSizeRange()));
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[�̃X�e�[�^�X�̃��[�h
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerStatusLoad(void)
{
	// �t�@�C���|�C���g
	FILE *pFile = nullptr;

	char cReadText[128];			// �����Ƃ��ēǂݎ��
	char cHeadText[128];			// ��r�p
	char cDie[128];					// �s�v�ȕ���

	// �t�@�C�����J��
	pFile = fopen(PLAYER_STATUS_TXT, "r");

	// �J���Ă���Ƃ�
	if (pFile)
	{
		// SCRIPT������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile); // �ꕶ�ǂݍ���
			sscanf(cReadText, "%s", &cHeadText);		// ��r�p�e�L�X�g�ɕ�������
		}

		// SCRIPT��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// END_SCRIPT������܂Ń��[�v
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				// STATUS_SET��������
				if (strcmp(cHeadText, "STATUS_SET") == 0)
				{
					// END_STATUS_SET������܂Ń��[�v
					while (strcmp(cHeadText, "END_STATUS_SET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						// Move��������
						if (strcmp(cHeadText, "Move") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fMove);
						}
						// Jump��������
						else if (strcmp(cHeadText, "Jump") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fJump);
						}
						// Dash��������
						else if (strcmp(cHeadText, "Dash") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fDash);
						}
						// NormalInertia��������
						else if (strcmp(cHeadText, "NormalInertia") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fNormalInertia);
						}
						// JumpInertia��������
						else if (strcmp(cHeadText, "JumpInertia") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fJumpInertia);
						}
						// PlayerSize��������
						else if (strcmp(cHeadText, "PlayerSize") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PlayerSize.x, &m_PlayerStatus.PlayerSize.y, &m_PlayerStatus.PlayerSize.z);
						}
						// PlayerOffSet��������
						else if (strcmp(cHeadText, "PlayerOffSet") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PlayerOffSet.x, &m_PlayerStatus.PlayerOffSet.y, &m_PlayerStatus.PlayerOffSet.z);
						}
						// PushSize��������
						else if (strcmp(cHeadText, "PushSize") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PushSize.x, &m_PlayerStatus.PushSize.y, &m_PlayerStatus.PushSize.z);
						}
						// PushOffSet��������
						else if (strcmp(cHeadText, "PushOffSet") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PushOffSet.x, &m_PlayerStatus.PushOffSet.y, &m_PlayerStatus.PushOffSet.z);
						}
						else if (strcmp(cHeadText, "END_BULLETSET") == 0)
						{
						}
					}
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		// �ǂݍ��ݎ��s���̌x���\��
		MessageBox(NULL, "�v���C���[�̃X�e�[�^�X�ǂݍ��ݎ��s", PLAYER_STATUS_TXT, MB_ICONWARNING);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[�̃X�e�[�^�X�̃Z�[�u
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerStatusSave(void)
{
	// �t�@�C���|�C���g
	FILE	*pFile = nullptr;

	// �t�@�C�����J��
	pFile = fopen(PLAYER_STATUS_TXT, "w");

	// �J���Ă���Ƃ�
	if (pFile)
	{
		fprintf(pFile, COMMENT02);
		fprintf(pFile, "// �v���C���[�̃X�e�[�^�X\n");
		fprintf(pFile, COMMENT02);

		fprintf(pFile, "SCRIPT\n");
		fprintf(pFile, NEWLINE);

		// �Z�[�u���郂�f���̏��
		fprintf(pFile, "STATUS_SET\n");
		fprintf(pFile, "	Move			= %.1f\n", m_PlayerStatus.fMove);
		fprintf(pFile, "	Jump			= %.1f\n", m_PlayerStatus.fJump);
		fprintf(pFile, "	Dash			= %.1f\n", m_PlayerStatus.fDash);
		fprintf(pFile, "	NormalInertia	= %.2f\n", m_PlayerStatus.fNormalInertia);
		fprintf(pFile, "	JumpInertia		= %.2f\n", m_PlayerStatus.fJumpInertia);
		fprintf(pFile, "	PlayerSize		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PlayerSize.x, m_PlayerStatus.PlayerSize.y, m_PlayerStatus.PlayerSize.z);
		fprintf(pFile, "	PlayerOffSet	= %.1f	%.1f	%.1f\n", m_PlayerStatus.PlayerOffSet.x, m_PlayerStatus.PlayerOffSet.y, m_PlayerStatus.PlayerOffSet.z);
		fprintf(pFile, "	PushSize		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PushSize.x, m_PlayerStatus.PushSize.y, m_PlayerStatus.PushSize.z);
		fprintf(pFile, "	PushOffSet		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PushOffSet.x, m_PlayerStatus.PushOffSet.y, m_PlayerStatus.PushOffSet.z);
		fprintf(pFile, "END_STATUS_SET\n\n");

		fprintf(pFile, "END_SCRIPT\n");

		// �t�@�C�������
		fclose(pFile);

		// �ǂݍ��ݐ������̌��ʕ\��
		MessageBox(NULL, "�Z�[�u���܂���", PLAYER_STATUS_TXT, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// �ǂݍ��ݎ��s���̌x���\��
		MessageBox(NULL, "�ǂݍ��ݎ��s", PLAYER_STATUS_TXT, MB_ICONWARNING);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[�̃X�e�[�^�X�̏����l�̃��[�h
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerStatusInitLoad(void)
{
	// �v���C���[�̏����X�e�[�^�X����
	m_PlayerStatus	= m_PlayerStatusInit;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̉����o������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PushBlock(
	CScene_X * pScene_X,			// �V�[��X���
	CBaseblock::GRID const & Grid	// ����
)
{
	// ����I�u�W�F�N�g�Ȃ�
	// ->�֐��𔲂���
	if (pScene_X->GetObj() != CScene::OBJ_BLOCK) return;
	// �v���C���[���_�b�V�����ȊO�Ȃ�
	// ->�֐��𔲂���
	if (!CPlayer::GetDashFlag()) return;

	// �ϐ��錾
	CBaseblock * pBlock = (CBaseblock *)pScene_X;					// �x�[�X�u���b�N�̏��

	// �����Ă����ԂȂ�
	if (pBlock->GetFall() ||
		pBlock->GetType() == CBaseblock::TYPE_FIELD) return;
	CBaseblock::GRID MyGrid = pBlock->GetGrid();					// �����o���u���b�N�̍s�񍂏��
	int nHeight;													// ����
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());	// �t�F�[�h�̒l
	// �s�񍂂��̕����������o���u���b�N�̍s�񍂂��ɉ��Z
	MyGrid += Grid;
	// ���̍s��̐ςݏd�Ȃ��Ă��鍂�����擾
	nHeight = CBaseblock::GetHeight(MyGrid.nColumn + nFeedValue, MyGrid.nLine + nFeedValue);
	if (MyGrid.nHeight <= CBaseblock::GetHeight(MyGrid.nColumn + nFeedValue, MyGrid.nLine + nFeedValue))
	{
		return;
	}

	// �ϐ��錾
	CBaseblock::GRID FallGrid = pBlock->GetGrid();	// �s��
	CBaseblock::GRID PlayerGrid = pBlock->GetGrid() - Grid;
	// �����O�̃u���b�N�̏�ɂ������u���b�N�𗎂Ƃ�����
	CBaseblock::FallBlock_Grid(FallGrid);
	// �������u���b�N�̌��݂܂ł����s��̍��������X�V
	CBaseblock::SetHeight(pBlock->GetGrid() + CBaseblock::GRID(nFeedValue,-1, nFeedValue));

	// ��������̍s�񍂂�ݒ�
	pBlock->SetGrid(MyGrid);
	// ��������̈ʒu�ݒ�
	pBlock->SetPos(MyGrid.GetPos(CBaseblock::GetSizeRange()));
	// �����o������̃u���b�N�ɓ��������v���C���[�̉����o�����菈��
	PushAfter_Collision(pBlock, Grid);
	// �����鏈��
	pBlock->SetFall(true);
	// �������u���b�N�̓�������̍s��̍��������X�V
	//CBaseblock::SetHeight(MyGrid + CBaseblock::GRID(nFeedValue, 0, nFeedValue));
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	// �L�����N�^�[�`��
	CCharacter::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���S����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Die(void)
{
	// ���S����
	CCharacter::Die();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �l�`�̎��S����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::OtherDie(void)
{
	// ���S����
	CCharacter::Die();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_MyCollision(int const & nObjType, CScene * pScene)
{
	// �o���[���L�����N�^�[�̓���������̏���
	CCharacter::Scene_MyCollision(nObjType, pScene);

	// ���������I�u�W�F�N�g���u���b�N��������W�����v���\�ɂ���
	if (nObjType == CCollision::OBJTYPE_BLOCK)
	{
		// �ϐ��錾
		CScene_X * pSceneX = (CScene_X *)pScene;
		pSceneX->GetCollision();
		this->GetCollision()->GetShape();
		SetJumpAble(true);
	}
	// ���������I�u�W�F�N�g���_���[�W���������玀�S�t���O��true�ɂ���
	else if (nObjType == CCollision::OBJTYPE_DAMAGEFLOOR)
	{
		SetDie(true);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����ɓ��Ă�ꂽ��̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
	// �L�����N�^�[�̑���ɓ��Ă�ꂽ��̏���
	CCharacter::Scene_OpponentCollision(nObjType, pScene);

	// �V�[�����NULL�Ȃ�
	// ->�֐��𔲂���
	if (pScene == NULL) return;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �������瓖����Ȃ�������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_NoMyCollision(int const & nObjType, CScene * pScene)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����ɓ��Ă��Ȃ�������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene)
{

}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Debug(void)
{
	// �v���C���[1�l�����ʂ�(�X�e�[�^�X�����ʂ̂���)
	if (GetPlayerTag() == PLAYER_TAG::PLAYER_1)
	{
		if (ImGui::Begin(u8"�v���C���[�̃X�e�[�^�X"))
		{
			//if (ImGui::CollapsingHeader(u8"�v���C���[�̃X�e�[�^�X"))
			{
				// �ړ���
				ImGui::DragFloat(u8"�ړ���", &m_PlayerStatus.fMove, 0.1f, 0.1f, 100.0f);						/* 3.0f */
				// �W�����v��
				ImGui::DragFloat(u8"�W�����v��", &m_PlayerStatus.fJump, 1.0f, 1.0f, 50.0f);						/* 10.0f */
				// �_�b�V���̈ړ���
				ImGui::DragFloat(u8"�_�b�V���̈ړ���", &m_PlayerStatus.fDash, 1.0f, 1.0f, 100.0f);				/* 30.0f */
				// �ʏ펞�̊���
				ImGui::DragFloat(u8"�ʏ펞�̊���", &m_PlayerStatus.fNormalInertia, 0.01f, 0.01f, 10.0f);		/* 0.7f */
				// �W�����v���̊���
				ImGui::DragFloat(u8"�W�����v���̊���", &m_PlayerStatus.fJumpInertia, 0.01f, 0.01f, 10.0f);		/* 1.6f */
				// �v���C���[�̃T�C�Y
				ImGui::DragFloat3(u8"�v���C���[�̃T�C�Y", m_PlayerStatus.PlayerSize, 1.0f, 0.0f, 500.0f);
				// �v���C���[�̃I�t�Z�b�g
				ImGui::DragFloat3(u8"�v���C���[�̃I�t�Z�b�g", m_PlayerStatus.PlayerOffSet, 1.0f, 0.0f, 500.0f);
				// �����o���p�̃T�C�Y
				ImGui::DragFloat3(u8"�����o���p�̃T�C�Y", m_PlayerStatus.PushSize, 1.0f, 0.0f, 500.0f);
				// �����o���p�̃I�t�Z�b�g
				ImGui::DragFloat3(u8"�����o���p�̃I�t�Z�b�g", m_PlayerStatus.PushOffSet, 1.0f, 0.0f, 500.0f);

				// �Z�[�u�{�^��
				if (ImGui::Button(u8"�ۑ�"))
				{
					// �v���C���[�̃X�e�[�^�X�̃Z�[�u
					PlayerStatusSave();
				}

				// ���s�L�����Z��
				ImGui::SameLine();

				// ������
				if (ImGui::Button(u8"������"))
				{
					// �v���C���[�̏����X�e�[�^�X���
					PlayerStatusInitLoad();
				}
				// ��؂��
				ImGui::Separator();
			}
		}
		// End
		ImGui::End();
	}
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
//	pos	: �ʒu
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(PLAYER_TAG tag,
	D3DXVECTOR3 const & pos	// �ʒu
)
{
	// �ϐ��錾
	CPlayer * pPlayer;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPlayer = new CPlayer(CHARACTER_PLAYER);
	// �V�[���Ǘ��ݒ�
	pPlayer->ManageSetting(CScene::LAYER_CHARACTER);
	// �ʒu���
	pPlayer->m_pos = pos;
	// �v���C���[�^�O�̐ݒ�
	pPlayer->SetPlayerTag(tag);
	// ����������
	pPlayer->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pPlayer;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���\�[�X���ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	// �v���C���[�̃X�e�[�^�X�̃��[�h
	PlayerStatusLoad();
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾃��\�[�X����j������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::UnLoad(void)
{
}
