// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �I�[�v�j���O�Ǘ����� [openingmanager.cpp]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "openingmanager.h"
#include "camera.h"

#include "manager.h"
#include "basemode.h"
#include "opening.h"
#include "stagingblock.h"

#include "3dparticle.h"
#include "3deffect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��
#define SPACE_Y (500)
// �^�C�g�����S�`���p �u���b�N���W
// CLIMBGUYS
#define STAGINGBLOCK_POS_C (D3DXVECTOR3(-600.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L (D3DXVECTOR3(-450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I (D3DXVECTOR3(-300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M (D3DXVECTOR3(-150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B (D3DXVECTOR3(0.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_G (D3DXVECTOR3(150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U (D3DXVECTOR3(300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y (D3DXVECTOR3(450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S (D3DXVECTOR3(600.0f, 0.0f + SPACE_Y, 0));

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3	COpeningManager::m_Targetpos[MAX_STAGINGBLOCK] = {};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COpeningManager::COpeningManager()
{
	// C
	m_Targetpos[0] = STAGINGBLOCK_POS_C;
	// L
	m_Targetpos[1] = STAGINGBLOCK_POS_L;
	// I
	m_Targetpos[2] = STAGINGBLOCK_POS_I;
	// M
	m_Targetpos[3] = STAGINGBLOCK_POS_M;
	// B
	m_Targetpos[4] = STAGINGBLOCK_POS_B;
	// G
	m_Targetpos[5] = STAGINGBLOCK_POS_G;
	// U
	m_Targetpos[6] = STAGINGBLOCK_POS_U;
	// Y
	m_Targetpos[7] = STAGINGBLOCK_POS_Y;
	// S
	m_Targetpos[8] = STAGINGBLOCK_POS_S;

	// 3D�G�t�F�N�g�̐���
	C3DEffect::Create();
	m_nCount = 0;
	m_NextStateCnt = 60;
	// �I�u�W�F�N�g�̐���
	CreateAll();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COpeningManager::~COpeningManager()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void COpeningManager::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void COpeningManager::Update(void)
{
	//--------------�e�X�g��-----------------------------
	//static int nCnt = 0;
	//nCnt++;

	//if (nCnt % 6 == 0)
	//{
	//	// �p�[�e�B�N������
	//	C3DParticle::Create(
	//		C3DParticle::PARTICLE_ID_SMOKE,
	//		D3DXVECTOR3(1020.0f, 500.0f, 900.0f)
	//	);
	//}

	//C3DParticle::Create(
	//	C3DParticle::PARTICLE_ID_BLOCK,
	//	D3DXVECTOR3(1030.0f, 500.0f, 950.0f)
	//);

	//----------------------------------------------


	if (CManager::GetMode() == CManager::MODE_OPENING)
	{
		COpening *pOpening = (COpening*)(CManager::GetBaseMode());

		if (pOpening && pOpening->GetState() == COpening::OpeningState::ERUPTION)
		{
			if (CManager::GetRenderer()->GetCamera()->GetShakeEndFlag())
			{
				// ���̃X�e�[�g�Ɉڍs
				pOpening->SetState(COpening::OpeningState::BUILDING);
			}
			else
			{
				// �J�����̐U��
				CManager::GetRenderer()->GetCamera()->CameraShake();
			}
		}

		if (pOpening && pOpening->GetState() == COpening::OpeningState::BUILDING)
		{
			//// �p�[�e�B�N������
			//C3DParticle::Create(
			//	C3DParticle::PARTICLE_ID_UNKNOWN,
			//	D3DXVECTOR3(1000.0f, 500.0f, 900.0f)
			//);

			/////////////----------------------------------------
			if (m_pStagingBlock[m_nCount]->GetPos().y > m_Targetpos[m_nCount].y)
			{ // �u���b�N������ړI�n�܂ňړ�
				m_pStagingBlock[m_nCount]->BlockFall();
			}
			else
			{
				if (m_nCount < MAX_STAGINGBLOCK - 1)
				{// ���̃u���b�N���܂�����Ȃ�
					m_pStagingBlock[m_nCount]->SetMoveStop(true);
					m_nCount++;
					return;
				}

				else
				{// ���̃X�e�[�g�Ɉڍs
					pOpening->SetState(COpening::OpeningState::STAGING);
				}
			}
			///////////-----------------------------------------------
		}

		// �I�[�v�j���O�̏�Ԃ� STAGING ��������
		if (pOpening && pOpening->GetState() == COpening::OpeningState::STAGING)
		{
			if (m_NextStateCnt <= 0)
			{
				for (int nNum = 0; nNum < MAX_STAGINGBLOCK; nNum++)
				{
					// �u���b�N�̔j��
					m_pStagingBlock[nNum]->BlockBurst();
				}
			}
			else
			{
				m_NextStateCnt--;
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �^�[�Q�b�g�̍��W�̎擾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 COpeningManager::GetTargetpos(int num)
{
	if (num < MAX_STAGINGBLOCK)
	{
		return m_Targetpos[num];
	}

	return m_Targetpos[0];
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S�ẴI�u�W�F�N�g�𐶐�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void COpeningManager::CreateAll()
{
	// �w�i�I�u�W�F�N�g�̐���
	CScene_X::Create(
		D3DVECTOR3_ZERO,					// �ʒu
		D3DVECTOR3_ZERO,					// ��]
		CScene_X::TYPE_OBJECT_MAP,			// ���f���ԍ�
		false);								// �V���h�E�}�b�s���O���

	// �u���b�N�̍ő吔���������|�C���^��ۑ�
	for (int nCnt = 0; nCnt < MAX_STAGINGBLOCK; nCnt++)
	{
		m_pStagingBlock[nCnt] = CStagingBlock::Create(
			D3DXVECTOR3(0.0f, 1500.0f, 0.0f),
			D3DVECTOR3_ZERO,
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CScene_X::TYPE_OBJECT_BLOCK_C + nCnt,
			false);

		m_pStagingBlock[nCnt]->SetBlockNum(nCnt);
	}
}
