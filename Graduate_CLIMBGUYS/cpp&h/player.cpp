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
#include "game.h"
#include "XInputPad.h"
#include "keyboard.h"
#include "ui_group.h"
#include "meshdome.h"
#include "3Dparticle.h"
#include "debugproc.h"
#include "meshbox.h"
#include "stand.h"
#include "playerUI.h"
#include "3Dline.h"
#include "springblock.h"
#include "resultUI.h"

#include "sound.h"


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_GRAVITY				(0.1f)		// �v���C���[�̏d��
#define PLAYER_UPMOVELIMIT			(20.0f)		// �v���C���[�̏㏸�ړ��ʐ���
#define PLAYER_UNDERMOVELIMIT		(5.0f)		// �v���C���[�̉��~�ړ��ʐ���
#define DASH_TIME_MAX				(30)		// �_�b�V�����Ă��鎞��
#define DASH_ENABLE_STICK_RANGE		(0.8f)		// �_�b�V����L���ɂ���X�e�B�b�N�̌X��
#define PLAYER_STATUS_TXT			("data/LOAD/STATUS/PlayerStatus.txt")	// �v���C���[�̃X�e�[�^�X�̃e�L�X�g
#define RESPAWN_POS					(D3DXVECTOR3(0.0f, 300.0f, 0.0f))		// ���X�|�[���n�_
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::PLAYER_STATUS CPlayer::m_PlayerStatus		= {};		// �v���C���[�̃X�e�[�^�X
CPlayer::PLAYER_STATUS CPlayer::m_PlayerStatusInit	= {};		// �v���C���[�̏����X�e�[�^�X

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer(CHARACTER const &character) : CCharacter::CCharacter(character)
{
	m_nCntState			= 0;					// �X�e�[�g�J�E���g
	m_bDieFlag			= false;				// ���S�t���O
	m_bDashFlag			= false;				// �_�b�V���t���O
	m_bTackleFrag		= false;				// �^�b�N���t���O
	m_nCntDashTime		= 0;					// �_�b�V�����̐؂�ւ��J�E���g
	m_pPlayerUI			= nullptr;				// �v���C���[UI�̃|�C���^
	m_Record			= RECORD();				// �L�^
	CScene::SetObj(CScene::OBJ::OBJ_PLAYER);	// �I�u�W�F�N�g�^�C�v�̐ݒ�

#ifdef _DEBUG
	// �����蔻��{�b�N�X�̏�����
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX;nCntCollision++)
	{
		pCollisionBox[nCntCollision] = NULL;
	}
	// �����蔻����̏�����
	pCollisionLine = NULL;
#endif // _DEBUG
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

	// �v���C���[UI�̐���
	m_pPlayerUI = CPlayerUI::Create(GetPlayerTag());

#ifdef _DEBUG
	// �����蔻��{�b�N�X�̏�����
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		// �����蔻��̃^�C�v���L�����N�^�[�Ȃ�
		if (nCntCollision == CPlayer::COLLISIONTYPE_CHARACTER)
		{
			// ���b�V���{�b�N�X�̐���
			pCollisionBox[nCntCollision] = CMeshBox::Create(m_pos + m_PlayerStatus.PlayerOffSet, m_PlayerStatus.PlayerSize, CMeshBox::COLLISION_TYPE::TYPE_CENTER);
			// ���F��
			pCollisionBox[nCntCollision]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}

		// �����蔻��̃^�C�v�������o���Ȃ�
		else if (nCntCollision == CPlayer::COLLISIONTYPE_PUSH)
		{
			// 3D������
			pCollisionLine = C3DLine::Create(NULL, D3DVECTOR3_ZERO,
				m_pos + m_PlayerStatus.PushOffSet,
				m_pos + m_PlayerStatus.PushOffSet + D3DXVECTOR3(sinf(m_rot.y) * m_PlayerStatus.PushSize,0.0f,cosf(m_rot.y) * m_PlayerStatus.PushSize));
		}
	}

#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	// �L�^�X�V_�^�C��
	m_Record.nTime = CGame::GetSecond();

	// �L�^�������U���g�ɓn��
	CResultUI::SetRecord(GetPlayerTag(), m_Record);

	// �L�����N�^�[�I������
	CCharacter::Uninit();

	// �v���C���[UI�̏�����
	m_pPlayerUI = nullptr;

#ifdef _DEBUG
	// �����蔻��{�b�N�X�̏�����
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		pCollisionBox[nCntCollision] = NULL;
	}
	pCollisionLine = NULL;
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// �s���\��ԂȂ�s���\
		CGame *pGame = (CGame*)CManager::GetBaseMode();

		if (pGame && pGame->GetbOperatable())
		{
			// ���L�����̍s������
			MyAction();
		}
	}
	else
	{
		// ���L�����̍s������
		MyAction();
	}

	if (m_pPlayerUI)
	{	// �v���C���[UI�̈ʒu�̐ݒ�
		m_pPlayerUI->SetPos(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

	// ���[�V�����ݒ菈��
	StatusMotion();
	// �L�����N�^�[�X�V
	CCharacter::Update();
	// ���E�n�_
	CCharacter::Limit();
	// �����蔻��̏���
	Collision();
#ifdef _DEBUG
	// �����蔻��{�b�N�X�̈ʒu�̍X�V
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		// �����蔻��̃^�C�v���L�����N�^�[�Ȃ�
		if (nCntCollision == CPlayer::COLLISIONTYPE_CHARACTER)
		{
			// NULL�`�F�b�N
			if (pCollisionBox[nCntCollision] == NULL) continue;
			// �ʒu�ݒ�
			pCollisionBox[nCntCollision]->SetPos(m_pos + m_PlayerStatus.PlayerOffSet);
			// �T�C�Y�ݒ�
			pCollisionBox[nCntCollision]->SetSize(m_PlayerStatus.PlayerSize);
			// ���_���W�̐ݒ�
			pCollisionBox[nCntCollision]->SetVtxPos();
		}
		// �����蔻��̃^�C�v�������o���Ȃ�
		else if (nCntCollision == CPlayer::COLLISIONTYPE_PUSH)
		{
			D3DXVECTOR3 DirectVec = CCharacter::GetDirectionVec();
#ifdef _DEBUG
			CDebugproc::Print("����:(%.2f,%.2f,%.2f)\n", DirectVec.x, DirectVec.y, DirectVec.z);
			CDebugproc::Print("JumpAble:(%d)\n", GetJumpAble());

#endif // _DEBUG

			// �ʒu�ݒ�
			pCollisionLine->SetPos(
				m_pos + m_PlayerStatus.PushOffSet,
				m_pos + m_PlayerStatus.PushOffSet - D3DXVECTOR3(CCharacter::GetDirectionVec().x * m_PlayerStatus.PushSize, 0.0f, CCharacter::GetDirectionVec().z * m_PlayerStatus.PushSize));
		}
	}
#endif // _DEBUG

	// ���S���肪�o���烊�U���g�ɑJ�ڂ���
	if (GetDie())
	{
		// ���S�֐�
		Die();
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
			m_bTackleFrag	= false;
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
			vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));
			move.x -= vec.x * m_PlayerStatus.fMove;
			move.z -= vec.z * m_PlayerStatus.fMove;
		}
		// ��O
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = -D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));
			move.x -= vec.x * m_PlayerStatus.fMove;
			move.z -= vec.z * m_PlayerStatus.fMove;
		}
		// ��
		else
		{
			rot.y = -D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));
			move.x -= vec.x * m_PlayerStatus.fMove;
			move.z -= vec.z * m_PlayerStatus.fMove;
		}
	}
	// �E
	else if (pKeyboard->GetKeyboardPress(DIK_D))
	{

		// ��
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));

			move.x -= vec.x * m_PlayerStatus.fMove;
			move.z -= vec.z * m_PlayerStatus.fMove;
		}
		// ��O
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));


			move.x -= vec.x * m_PlayerStatus.fMove;
			move.z -= vec.z * m_PlayerStatus.fMove;
		}
		// �E
		else
		{
			rot.y = D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));

			move.x -= vec.x * m_PlayerStatus.fMove;
			move.z -= vec.z * m_PlayerStatus.fMove;
		}
	}
	// ���ɍs��
	else if (pKeyboard->GetKeyboardPress(DIK_W))
	{
		rot.y = D3DX_PI * 0.0f + fRot;
		vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));
		move.x -= vec.x * m_PlayerStatus.fMove;
		move.z -= vec.z * m_PlayerStatus.fMove;
	}
	// ��O�ɍs��
	else if (pKeyboard->GetKeyboardPress(DIK_S))
	{
		rot.y = D3DX_PI * 1.0f + fRot;
		vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));
		move.x -= vec.x * m_PlayerStatus.fMove;
		move.z -= vec.z * m_PlayerStatus.fMove;
	}
	// ����ȊO
	else
	{

	}

	// �����I�L�[�{�[�h�W�����v
	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) && GetJumpAble())
	{
		SetMotion(MOTIONTYPE_JUMP);
		move.y += m_PlayerStatus.fJump;
		SetJumpAble(false);
	}
	// �����I�^�b�N��
	if (pKeyboard->GetKeyboardTrigger(DIK_J))
	{
		SetMotion(MOTIONTYPE_DASH);
		m_bDashFlag = true;
		m_bTackleFrag = true;
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
				SetMotion(MOTIONTYPE_MOVE);

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
				vec = D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y));
				// �X�e�B�b�N�̊p�x�ɂ���ăv���C���[�ړ�
				move.x -= sinf(fAngle + fRot) * (m_PlayerStatus.fMove);
				move.z -= cosf(fAngle + fRot) * (m_PlayerStatus.fMove);
			}

			// �����I�W�����v ( �̂��ɒ��g�ς��\�� ���� )
			if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_A, 1) && GetJumpAble())
			{
				SetMotion(MOTIONTYPE_JUMP);

				move.y += m_PlayerStatus.fJump;
				SetJumpAble(false);
			}

			// �����I�^�b�N�� ( �̂��ɒ��g�ς��\�� ���� )
			if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_X, 1))
			{
				SetMotion(MOTIONTYPE_DASH);

				m_bDashFlag = true;
				m_bTackleFrag = true;

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

	/*
	if (vec.x < 0)
	{
		vec.x *= -1;
	}
	if (vec.z < 0)
	{
		vec.z *= -1;
	}
	*/
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
	// �L�����N�^�[�̓����蔻��
	CharacterCollision();
	// �u���b�N�̓����蔻��
	BlockCollision();
	// ����̓����蔻��
	StandCollision();
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����u���b�N�Ƃ̔���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StandCollision(void)
{
	// �ϐ��錾
	CStand * pStand;									// �V�[��X���
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;	// �����蔻��̕���
	bool bOn = false;										// ��̓����蔻��
	bool bUnder = false;									// ���̓����蔻��
															// �u���b�N���[�v
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DSTAND); nCntBlock++)
	{
		// �����蔻��̏�����
		Direct = COLLISIONDIRECTION::NONE;
		// NULL���
		pStand = NULL;
		// ���擾
		pStand = (CStand *)CScene::GetScene(CScene::LAYER_3DSTAND, nCntBlock);
		// NULL�Ȃ�
		// ->�֐��𔲂���
		if (pStand == NULL)
		{
			continue;
		}
		// �����蔻��
		Direct = pStand->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&m_PlayerStatus.PlayerSize,
			m_PlayerStatus.PlayerOffSet
		);
		// �u���b�N�̔���
		// ��
		if (Direct == COLLISIONDIRECTION::UP)
		{
			// �W�����v�\�ݒ�
			SetJumpAble(true);
			// ���ꔻ��
			StandJudg(pStand, true);
			// �v���C���[�����̃u���b�N�ɓ������Ă���
			bOn ^= true;
		}
		else
		{
			// ���ꔻ��
			StandJudg(pStand, false);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̓����蔻�菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::BlockCollision(void)
{
	// �ϐ��錾
	CBaseblock *			pBaseBlock;							// �V�[��X���
	COLLISIONDIRECTION		Direct = COLLISIONDIRECTION::NONE;	// �����蔻��̕���
	CBaseblock::PUSHBLOCK	Pushblock;							// �v�b�V���u���b�N���
	bool					bUp = false;						// ��̓����蔻��
	bool					bDown = false;						// ���̓����蔻��
	bool					bRight = false;						// �E�̓����蔻��
	bool					bLeft = false;						// ���̓����蔻��
	bool					bFront = false;						// �O�̓����蔻��
	bool					bBack = false;						// ��̓����蔻��
	float					fDistance = -1.0f;					// ����
	D3DXVECTOR3				PredictionPoint;					// �\���_
	PredictionPoint = m_pos + m_PlayerStatus.PushOffSet - D3DXVECTOR3(CCharacter::GetDirectionVec().x * m_PlayerStatus.PushSize, 0.0f, CCharacter::GetDirectionVec().z * m_PlayerStatus.PushSize);
	Pushblock = CBaseblock::PUSHBLOCK(NULL, -1.0f, Direct);
	// �u���b�N���[�v
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK); nCntBlock++)
	{
		// �����蔻��̏�����
		Direct = COLLISIONDIRECTION::NONE;
		// NULL���
		pBaseBlock = NULL;
		// ���擾
		pBaseBlock = (CBaseblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);
		// NULL�Ȃ�
		// ->�֐��𔲂���
		if (pBaseBlock == NULL) continue;
		// �_�b�V����ԂȂ�
		if (m_bTackleFrag &&
			!pBaseBlock->GetType() == CBaseblock::TYPE::TYPE_FIELD)
		{
			// �����ɒ������o��
			// ���ƃ|���S���Ŕ�������
			Direct = pBaseBlock->PushBlock(
				m_pos + m_PlayerStatus.PushOffSet,
				PredictionPoint,
				fDistance);

			// �u���b�N�ɓ������Ă�����
			if (Direct != COLLISIONDIRECTION::NONE)
			{
				// �v�b�V���u���b�N�̏�񂪑��݂���Ȃ�
				if (Pushblock.pBlock)
				{
					// �������r���Ăǂ��炪�߂���
					if (Pushblock.fDistance > fDistance ||
						Pushblock.fDistance < 0)
					{
						// ������
						Pushblock = CBaseblock::PUSHBLOCK(pBaseBlock, fDistance, Direct);
					}
				}
				else
				{
					Pushblock = CBaseblock::PUSHBLOCK(pBaseBlock, fDistance, Direct);
				}
			}
		}
		// �����蔻��
		Direct = pBaseBlock->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&m_PlayerStatus.PlayerSize,
			m_PlayerStatus.PlayerOffSet
		);

		// �u���b�N�̎�ނ��o�l�u���b�N��������
		CSpringblock *pSpringblock = (CSpringblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);
		if (pSpringblock && pSpringblock->GetModelId() == CScene_X::TYPE_BLOCK_SPRING)
		{
			if (// �o�l�u���b�N�̏ォ��̏���
				pSpringblock->PushCollision(
					this,
					m_PlayerStatus.PlayerOffSet,
					&m_PlayerStatus.PlayerSize
				))
			{
				SetJumpAble(false);
				Direct = COLLISIONDIRECTION::NONE;
			}
			else
			{
				m_bSpringFlag = false;
			}
		}

		// �u���b�N�̔���
		// �O
		if (Direct == COLLISIONDIRECTION::FRONT)
		{
			bFront = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::BACK)
		{
			bBack = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::LEFT)
		{
			bLeft = true;
		}
		// �E
		else if (Direct == COLLISIONDIRECTION::RIGHT)
		{
			bRight = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::UP)
		{
			// �W�����v�\�ݒ�
			SetJumpAble(true);
			bUp = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::DOWN)
		{
			bDown = true;
		}
	}
	// ���܂����玀�ʏ���
	if ((bUp && bDown) ||
		(bFront && bBack) ||
		(bRight && bLeft)
		)
	{
		// �L�^�X�V
		m_Record.DieCause = DIECAUSE::DIECAUSE_PRESS;
		// ���S����
		Die();
	}

	// �u���b�N�̉����o������
	if (m_bTackleFrag)
	{
		if (Pushblock.pBlock)
		{
			// ���艹
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_PUNCH);

			// �O
			if (Pushblock.Direction == COLLISIONDIRECTION::FRONT)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(0, 0, -1));
			}
			// ��
			else if (Pushblock.Direction == COLLISIONDIRECTION::BACK)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(0, 0, 1));
			}
			// ��
			else if (Pushblock.Direction == COLLISIONDIRECTION::LEFT)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(1, 0, 0));
			}
			// �E
			else if (Pushblock.Direction == COLLISIONDIRECTION::RIGHT)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(-1, 0, 0));
			}
			m_bTackleFrag = false;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �L�����N�^�[���m�̓����蔻�菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::CharacterCollision(void)
{
	// �ϐ��錾
	CCharacter * pCharacter;	// �L�����N�^�[���
	COLLISIONDIRECTION Direct;	// �����蔻��̕���
	// �u���b�N���[�v
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntBlock++)
	{
		// NULL���
		pCharacter = NULL;
		// ���擾
		pCharacter = (CCharacter *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntBlock);
		// NULL�Ȃ�
		// ->�֐��𔲂���
		if (pCharacter == NULL)
		{
			continue;
		}
		// �����蔻��
		Direct = pCharacter->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&m_PlayerStatus.PlayerSize,
			m_PlayerStatus.PlayerOffSet
		);
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
		// �����������Ă����
		if (Direct != COLLISIONDIRECTION::NONE)
		{
			// �v���C���[�̍s��
			CBaseblock::GRID PlayerGrid;
			// �v���C���[�̍ŐV�s��
			PlayerGrid = pBaseBlock->GetGrid() + Grid;
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
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.PushSize);
						}
						// PushOffSet��������
						else if (strcmp(cHeadText, "PushOffSet") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PushOffSet.x, &m_PlayerStatus.PushOffSet.y, &m_PlayerStatus.PushOffSet.z);
						}
						else if (strcmp(cHeadText, "END_STATUS_SET") == 0)
						{
							// �v���C���[�̏����X�e�[�^�X��ۑ�
							m_PlayerStatusInit = m_PlayerStatus;
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
		fprintf(pFile, "	PushSize		= %.1f\n", m_PlayerStatus.PushSize);
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
// �o�l�p�W�����v����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::SpringJump(void)
{
	if (!m_bSpringFlag)
	{
		// �o�l
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_SPRING);

		// �ϐ��錾
		D3DXVECTOR3 move;				// �ړ���
		move = CCharacter::GetMove();	// �ړ���

										// �o�l�p�W�����v
		move.y += m_PlayerStatus.fJump * 1.5f;
		SetJumpAble(false);

		// �W�����v���Ă���Ƃ��̊���
		if (!GetJumpAble())
		{
			move.x += (-m_move.x) * m_PlayerStatus.fJumpInertia;
			move.z += (-m_move.z) * m_PlayerStatus.fJumpInertia;
		}
		else
		{
			move.x += (-m_move.x) * m_PlayerStatus.fNormalInertia;
			move.z += (-m_move.z) * m_PlayerStatus.fNormalInertia;
		}

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

		m_bSpringFlag = true;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̉����o������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PushBlock(
	CBaseblock * pBlock,			// �u���b�N���
	CBaseblock::GRID const & Grid	// ����
)
{
	// �v���C���[���_�b�V�����ȊO�Ȃ�
	// ->�֐��𔲂���
	if (!CPlayer::GetDashFlag()) return;

	// �m�b�N�o�b�N����
	this->SetPos(this->GetPos() + D3DXVECTOR3(0.0f, 10.0f, 0.0f));


	// �����Ă����ԂȂ�
	if (pBlock->GetFall() ||
		pBlock->GetType() == CBaseblock::TYPE_FIELD) return;
	CBaseblock::GRID NextGrid = pBlock->GetGrid();					// �����o���u���b�N�̍s�񍂏��
	int nHeight;													// ����
	// �s�񍂂��̕����������o���u���b�N�̍s�񍂂��ɉ��Z
	NextGrid += Grid;
	// ���̍s��̐ςݏd�Ȃ��Ă��鍂�����擾
	nHeight = CBaseblock::GetHeight(NextGrid.nColumn, NextGrid.nLine);
	// �����o�������Ƀu���b�N���u����Ă��Ȃ������`�F�b�N
	if (NextGrid.nHeight <= CBaseblock::GetHeight(NextGrid.nColumn, NextGrid.nLine))
	{
		return;
	}
	// �L�^�X�V_�����o����
	m_Record.nPushCnt++;
	// �����o����̐ݒ�
	pBlock->SetPushAfter(CBaseblock::PUSHAFTER(true, Grid));
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����o�������蔻��
//	pos		: �ʒu
//	posOld	: �O��̈ʒu
//	move	: �ړ���
//	size	: �T�C�Y
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CPlayer::PushCollision(
	CScene::OBJ const & Obj,		// �I�u�W�F�^�C�v
	D3DXVECTOR3 * pos,				// �ʒu
	D3DXVECTOR3 * posOld,			// �O��̈ʒu
	D3DXVECTOR3 * move,				// �ړ���
	D3DXVECTOR3 * size,				// �T�C�Y
	D3DXVECTOR3 const & OffsetPos	// �I�t�Z�b�g�ʒu
)
{
	// �ϐ��錾
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// �ǂ��̓����蔻�肩
	bool bPush = false;
	// �ϐ��錾
	D3DXVECTOR3 MyPos = CCharacter::GetPos();
	CGame::STAGE Stage = CGame::GetStage();						// �X�e�[�W

	D3DXVECTOR3 Kari_Size = { 25.0f,50.0f,25.0f };	// ���T�C�Y
		// �f�ނ�Z�͈�
	if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
	{
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// �����蔻��(��)
			if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= MyPos.y - Kari_Size.y * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;

				// �f�ޏ�̍���
				pos->y = MyPos.y - Kari_Size.y * 0.5f - size->y * 0.5f - OffsetPos.y;

				// �ړ��ʂ̏�����
				move->y = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= MyPos.y + Kari_Size.y * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
				// �f�ޏ�̍���
				pos->y = MyPos.y + Kari_Size.y * 0.5f + size->y * 0.5f - OffsetPos.y;
				// �ړ��ʂ̏�����
				move->y = 0.0f;
				// �����o����Ԃ�true
				bPush = true;

			}
			// �����蔻��(��)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.z * 0.5f&&
				pos->y + OffsetPos.y <= MyPos.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				pos->y + OffsetPos.y - size->y > MyPos.y + Kari_Size.y * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// �������������ɏ�񂪓����Ă���Ȃ�
	//if (bPush) return Direct;
	// �f�ނ�Y�͈�
	if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
		pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f)
	{
		// �f�ނ�Z�͈�
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
		{
			// �����蔻��(��)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
				// �f�ޏ�̍���
				pos->x = MyPos.x - Kari_Size.x * 0.5f - size->x * 0.5f - OffsetPos.x;
				// �ړ��ʂ̏�����
				move->x = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}
			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;

				// �f�ޏ�̍���
				pos->x = MyPos.x + Kari_Size.x * 0.5f + size->x * 0.5f - OffsetPos.x;

				// �ړ��ʂ̏�����
				move->x = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}
			// �����蔻��(��)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// �������������ɏ�񂪓����Ă���Ȃ�
		//if (bPush) return Direct;
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// �����蔻��(��O)
			if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;

				// �f�ޏ�̍���
				pos->z = MyPos.z - Kari_Size.z * 0.5f - size->z * 0.5f - OffsetPos.z;

				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}

			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;

				// �f�ޏ�̍���
				pos->z =
					MyPos.z + Kari_Size.z * 0.5f +
					size->z * 0.5f - OffsetPos.z;

				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}
			// �����蔻��(��O)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;
			}
			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��
//	pos			: �ʒu
//	size		: �T�C�Y
//	OffsetPos	: �I�t�Z�b�g�ʒu
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CPlayer::Collision(
	CScene::OBJ const & Obj,		// �I�u�W�F�^�C�v
	D3DXVECTOR3 * pos,				// �ʒu
	D3DXVECTOR3 * posOld,			// �O��̈ʒu
	D3DXVECTOR3 * size,				// �T�C�Y
	D3DXVECTOR3 const & OffsetPos	// �I�t�Z�b�g�ʒu
)
{
	// �ϐ��錾
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// �ǂ��̓����蔻�肩
	bool bPush = false;
	// �ϐ��錾
	D3DXVECTOR3 MyPos = CCharacter::GetPos();
	CGame::STAGE Stage = CGame::GetStage();						// �X�e�[�W

	D3DXVECTOR3 Kari_Size = { 25.0f,50.0f,25.0f };	// ���T�C�Y
													// �f�ނ�Z�͈�
	if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
	{
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// �����蔻��(��)
			if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= MyPos.y - Kari_Size.y * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;
				// �����o����Ԃ�true
				bPush = true;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= MyPos.y + Kari_Size.y * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
				// �����o����Ԃ�true
				bPush = true;

			}
			// �����蔻��(��)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.z * 0.5f&&
				pos->y + OffsetPos.y <= MyPos.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				pos->y + OffsetPos.y - size->y > MyPos.y + Kari_Size.y * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// �������������ɏ�񂪓����Ă���Ȃ�
	//if (bPush) return Direct;
	// �f�ނ�Y�͈�
	if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
		pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f)
	{
		// �f�ނ�Z�͈�
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
		{
			// �����蔻��(��)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
				// �����o����Ԃ�true
				bPush = true;
			}
			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
				// �����o����Ԃ�true
				bPush = true;
			}
			// �����蔻��(��)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// �������������ɏ�񂪓����Ă���Ȃ�
		//if (bPush) return Direct;
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// �����蔻��(��O)
			if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;

			}

			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;

			}
			// �����蔻��(��O)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;
			}
			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
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
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::PARTICLE_ID_UNKNOWN,
		m_pos
	);

	// �`���[�g���A���ȊO�̂Ƃ�����
	if (CManager::GetMode() != CManager::MODE_TUTORIAL)
	{
		if (m_pPlayerUI)
		{
// �����[�X���ɂ̂ݒʂ�
#ifndef _DEBUG
			// �o�C�u���[�V�����̐ݒ�
			m_pPad->StartVibration(60);
#endif // _DEBUG

			// �v���C���[UI���J��
			m_pPlayerUI->Release();
			m_pPlayerUI = nullptr;
		}
		// �L�^�X�V_�����L���O
		m_Record.nRanking = CCharacter::GetAllCharacter();
		// �L�^�X�V_�^�C��
		m_Record.nTime = CGame::GetSecond();
		// ���S����
		CCharacter::Die();

#ifdef _DEBUG
		// �����蔻��{�b�N�X�̊J��
		for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
		{
			// NULL�`�F�b�N
			if (pCollisionBox[nCntCollision] == NULL) continue;
			pCollisionBox[nCntCollision]->Release();
			pCollisionBox[nCntCollision] = NULL;
		}
#endif // _DEBUG
	}
	// �`���[�g���A���̂Ƃ����X�|�[������
	else
	{
		// ���S������I�t�ɂ��� ( ���� )
		SetDie(false);
		// ���X�|�[���ʒu�̐ݒ�
		SetPos(RESPAWN_POS);
	}
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
				ImGui::DragFloat(u8"�����o���p�̃T�C�Y", &m_PlayerStatus.PushSize, 1.0f, 0.0f, 500.0f);
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
	CPlayer * pPlayer = nullptr;		// �V�[��2D�N���X

	switch (tag)
	{
	case PLAYER_TAG::PLAYER_1:
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pPlayer = new CPlayer(CHARACTER_PLAYER_0);
		break;
	case PLAYER_TAG::PLAYER_2:
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pPlayer = new CPlayer(CHARACTER_PLAYER_2);
		break;
	case PLAYER_TAG::PLAYER_3:
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pPlayer = new CPlayer(CHARACTER_PLAYER_1);
		break;
	case PLAYER_TAG::PLAYER_4:
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pPlayer = new CPlayer(CHARACTER_PLAYER_3);
		break;
	}

	if (pPlayer)
	{
		// �V�[���Ǘ��ݒ�
		pPlayer->ManageSetting(CScene::LAYER_CHARACTER);
		// �ʒu���
		pPlayer->m_pos = pos;
		// �v���C���[�^�O�̐ݒ�
		pPlayer->SetPlayerTag(tag);
		// ����������
		pPlayer->Init();
	}

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
