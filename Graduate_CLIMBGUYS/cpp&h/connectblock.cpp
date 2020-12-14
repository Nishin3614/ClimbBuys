// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �����u���b�N���� [connectblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "connectblock.h"
#include "baseblock.h"
#include "normalblock.h"
#include "bombblock..h"
#include "game.h"
#include "springblock.h"
#include "steelblock.h"
#include "electricblock.h"
#include "panicblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DROP_BLOCK	(1)		// ���Ƃ��u���b�N�̐�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int	CConnectblock::m_nCntTime = 0;						// �J�E���g�^�C��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::CConnectblock()
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::~CConnectblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::SetBlockShape(void)
{
	// �ϐ��錾
	SHAPE Shape = (SHAPE)(CCalculation::GetRandomRange(SHAPE_CROSS_STEP_01, SHAPE_MAX - 1));	// �`
	D3DXCOLOR col;																	// �F
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());					// �t�F�[�h�l
	// �`
	CBaseblock::GRID BaseGrid = CBaseblock::SetFallPos();
	// �d�͐ݒ�
	int nPhase = CBaseblock::GetPhase();											// �t�F�[�Y
	float fGravity;																	// �d��
	float fMinGravity = 0;															// �ŏ��l
	float fMaxGravity = 0;															// �ő�l
	if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMin;	// �ŏ��l
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMax;	// �ő�l
	}
	else
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMin;	// �ŏ��l
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMax;	// �ő�l
	}
	if (fMinGravity < fMaxGravity)
	{
		fGravity = CCalculation::RandamFloat(fMaxGravity, fMinGravity);
	}
	else
	{
		fGravity = (float)fMinGravity;
	}
	switch (Shape)
	{
		// �N���X�u���b�N_1�i
	case SHAPE_CROSS_STEP_01:
		col = COLOR_RED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		break;
		// �N���X�u���b�N_2�i
	case SHAPE_CROSS_STEP_02:
		col = COLOR_GREEN;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 2), &col,fGravity);
		break;
		// ���󂫃v���X�u���b�N_1�i
	case SHAPE_PLUS_CENTER_HOLE_STEP_01:
		col = COLOR_BLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		break;
		// ���󂫃v���X�u���b�N_2�i
	case SHAPE_PLUS_CENTER_HOLE_STEP_02:
		col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 2), &col,fGravity);

		break;
		// �v���X�u���b�N
	case SHAPE_PLUS:
		col = COLOR_VIOLET;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		break;
		// ���ˋN�v���X�u���b�N
	case SHAPE_PLUS_CENTER_PROTRUSION:
		col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &col,fGravity);
		break;
		// �Ίp���u���b�N_01
	case SHAPE_DIAGONAL_01:
		col = COLOR_ORANGE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		break;
		// �Ίp���u���b�N_02
	case SHAPE_DIAGONAL_02:
		col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		break;
		// �E�u���b�N_01
	case SHAPE_BEARD_01:
		col = COLOR_PURPURE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		break;
		// �E�u���b�N_02
	case SHAPE_BEARD_02:
		col = COLOR_PINK;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		break;
		// �E�u���b�N_03
	case SHAPE_BEARD_03:
		col = COLOR_EMERALD;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);

		break;
		// �E�u���b�N_04
	case SHAPE_BEARD_04:
		col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����u���b�N�̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::SetSpecialBlock(void)
{
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());					// �t�F�[�h�l
	// ������ʒu�ݒ�
	CBaseblock::GRID BaseGrid = CBaseblock::SetSpecialFallPos();
	// �d�͐ݒ�
	int nPhase = CBaseblock::GetPhase();											// �t�F�[�Y
	float fGravity;																	// �d��
	float fMinGravity = 0;															// �ŏ��l
	float fMaxGravity = 0;															// �ő�l
	if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMin;	// �ŏ��l
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMax;	// �ő�l
	}
	else
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMin;	// �ŏ��l
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMax;	// �ő�l
	}
	if (fMinGravity < fMaxGravity)
	{
		fGravity = CCalculation::RandamFloat(fMaxGravity, fMinGravity);
	}
	else
	{
		fGravity = (float)fMinGravity;
	}
	CBaseblock::SPECIALBLOCK SpecialBlock;
	SpecialBlock = CBaseblock::GetSpecialSort(nPhase).GetRandam();
	switch (SpecialBlock)
	{
	case CBaseblock::SPECIALBLOCK_NONE:
		break;
	case CBaseblock::SPECIALBLOCK_BOMB:
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_SPRING:
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_STEEL:
		CSteelblock::Create(CScene_X::TYPE_BLOCKS_HARD, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_PANIC:
		CPanicblock::Create(CScene_X::TYPE_BLOCKS_HATENA, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_ELECTRIC:
		CElectricblock::Create(CScene_X::TYPE_BLOCKS_INVERTER, BaseGrid, NULL, fGravity);
		break;
	default:
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����u���b�N�S�\�[�X�̓ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CConnectblock::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�X�g�쐬(�V�[���Ǘ�)
//	pos			: �ʒu
//	layer		: ���C���[
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::TestCreate(void)
{
	// �ϐ��錾
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());
	D3DXCOLOR Col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	// �`
	CBaseblock::GRID BaseGrid = CBaseblock::GRID(-nFeedValue + rand() % (nFeedValue * 2), 5, -nFeedValue + rand() % (nFeedValue * 2));
	CNormalblock::Create(2, BaseGrid, &Col, 1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 1), &Col,1.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V_�u���b�N����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Update_CreateBlock(void)
{
	// �w��̎��Ԃɗ����Ă���
	if (m_nCntTime % DERAY_TIME(CBaseblock::GetBlockStatus().nAppBlock) == 0)
	{
		// �d�͐ݒ�
		int nPhase = CBaseblock::GetPhase();											// �t�F�[�Y
		int nDropBlock;																	// �d��
		int nMinDropBlock = 0;															// �ŏ��l
		int nMaxDropBlock = 0;															// �ő�l
		if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
		{
			nMinDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[CBaseblock::GetBlockStatus().nMaxSprit - 1].nMin;	// �ŏ��l
			nMaxDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[CBaseblock::GetBlockStatus().nMaxSprit - 1].nMax;	// �ő�l
		}
		else
		{
			nMinDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[nPhase].nMin;	// �ŏ��l
			nMaxDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[nPhase].nMax;	// �ő�l
		}
		int nRange = nMaxDropBlock - nMinDropBlock;											// �͈�
		nDropBlock = nMinDropBlock;
		if (nRange > 0)
		{
			nDropBlock += rand() % (nRange + 1);
		}
		// �ő�̍����Əd�����Đݒ�
		CBaseblock::SetMaxPriority();

		// �u���b�N�𗎂Ƃ���
		for (int nCnt = 0; nCnt < nDropBlock; nCnt++)
		{
			// �������ꂽ�u���b�N�̐���
			SetBlockShape();
		}
		// ����u���b�N�̐���
		SetSpecialBlock();
	}
	// �J�E���g�^�C���A�b�v
	m_nCntTime++;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`���[�g���A���̃u���b�N�̏����z�u
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Tutorial_InitArrangementBlock(void)
{
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());
	D3DXCOLOR Col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// ��l
	CBaseblock::GRID BaseGrid = CBaseblock::GRID(-nFeedValue, 0, -nFeedValue);

	// �u���b�N�̐���
	// 01
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(2, 0, 5), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(2, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(2, 1, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 1, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 2, 7), &Col,1.0f);

	// 02
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(7, 0, 5), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(6, 0, 6), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(5, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(7, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(6, 1, 6), &Col,1.0f);

	// 03
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(4, 0, 3), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 0, 4), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(5, 0, 4), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(4, 0, 5), &Col,1.0f);

	// 04
	CNormalblock::Create(2, BaseGrid, &Col, 1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 2, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 2, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 2, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 2, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 3, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 3, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 3, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 3, 1), &Col,1.0f);

	// �o�l
	CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(2, 0, 1), &Col, 1.0f);
	CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(6, 0, 1), &Col, 1.0f);

	// �{��
	CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(4, 0, 1), NULL, 1.0f);
	CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(4, 0, 4), NULL, 1.0f);
}

#if IMGUI_DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�ȃf�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::StaticDebug(void)
{
	if (ImGui::Begin("Block"))
	{
		// �ϐ��錾
		static int nBlockGrid[3];	// �u���b�N�s��
		ImGui::InputInt3("BlockGrid", nBlockGrid);
		// �ʏ�u���b�N����
		if (ImGui::Button("CreateNormal"))
		{
			CNormalblock::Create(CScene_X::TYPE_BLOCK, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
		}
		// �{���u���b�N����
		if (ImGui::Button("CreateBomb"))
		{
			CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
		// �΂˃u���b�N����
		if (ImGui::Button("CreateSpring"))
		{
			CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
		// �|�S�u���b�N����
		if (ImGui::Button("CreateSteel"))
		{
			CSteelblock::Create(CScene_X::TYPE_BLOCKS_HARD, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
		// �p�j�b�N�u���b�N����
		if (ImGui::Button("CreatePanic"))
		{
			CPanicblock::Create(CScene_X::TYPE_BLOCKS_HATENA, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]),NULL, 1.0f);
		}
		// �d�C�u���b�N����
		if (ImGui::Button("CreateElectric"))
		{
			CElectricblock::Create(CScene_X::TYPE_BLOCKS_INVERTER, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
	}
	ImGui::End();
}

#endif // IMGUI_DEBUG

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Debug(void)
{

}
#endif // _DEBUG

