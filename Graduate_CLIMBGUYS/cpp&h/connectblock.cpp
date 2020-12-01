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
int	CConnectblock::m_nCntTime = 0;				// �J�E���g�^�C��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::CConnectblock() : CScene::CScene()
{
	m_pos = D3DVECTOR3_ZERO;
	m_move = D3DVECTOR3_ZERO;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Shape = SHAPE_RECT;
	// �����_���ɐF����ݒ�
	CCalculation::GetRandomColor(m_col);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::~CConnectblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Init()
{
	// �u���b�N�ݒ�
	SetBlockShape();
	/*
	for (size_t nCntBlock = 0; nCntBlock < m_vec_pBaseBlock.size(); nCntBlock++)
	{
		m_vec_pBaseBlock[nCntBlock]->SetModelColor(m_col);
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Uninit(void)
{
	/*
	// NULL���
	for (size_t nCntBlock = 0; nCntBlock < m_vec_pBaseBlock.size(); nCntBlock++)
	{
		m_vec_pBaseBlock[nCntBlock] = NULL;
	}
	// �x�[�X�u���b�N���̊i�[���J��
	m_vec_pBaseBlock.clear();
	m_vec_pBaseBlock.shrink_to_fit();
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Update(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::SetBlockShape(void)
{
	// �ϐ��錾
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());
	// �`
	CBaseblock::GRID BaseGrid = CBaseblock::GRID(-nFeedValue + rand() % (nFeedValue * 2), CBaseblock::GetBlockStatus().nAppearance, -nFeedValue + rand() % (nFeedValue * 2));
	// �d�͐ݒ�
	int nPhase = CBaseblock::GetPhase();											// �t�F�[�Y
	int nGravity;																	// �d��
	int nMinGravity = 0;															// �ŏ��l
	int nMaxGravity = 0;															// �ő�l
	if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
	{
		nMinGravity = CBaseblock::GetBlockStatus().v_nBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].nMin;	// �ŏ��l
		nMaxGravity = CBaseblock::GetBlockStatus().v_nBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].nMax;	// �ő�l
	}
	else
	{
		nMinGravity = CBaseblock::GetBlockStatus().v_nBlockGravity[nPhase].nMin;	// �ŏ��l
		nMaxGravity = CBaseblock::GetBlockStatus().v_nBlockGravity[nPhase].nMax;	// �ő�l
	}
	int nRange = nMaxGravity - nMinGravity;											// �͈�
	nGravity = nMinGravity;
	if (nRange > 0)
	{
		nGravity += rand() % (nRange + 1);
	}

	switch (m_Shape)
	{
		// ��`
	case SHAPE_RECT:
		m_col = COLOR_ORANGE;
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid,NULL, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &m_col,(float)nGravity);
		break;
		// I�u���b�N
	case SHAPE_I:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, (float)nGravity);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(2, 0, 0), NULL, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(3, 0, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(4, 0, 0), &m_col, (float)nGravity);
		break;
		// J�u���b�N
	case SHAPE_J:
		m_col = COLOR_EMERALD;
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid, NULL, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 2, 0), &m_col, (float)nGravity);
		break;
		// L�u���b�N
	case SHAPE_L:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 2, 0), &m_col, (float)nGravity);
		break;
		// S�u���b�N
	case SHAPE_S:
		m_col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &m_col,(float)nGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col, (float)nGravity);
		break;
		// T�u���b�N
	case SHAPE_T:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, -1, 0), &m_col, (float)nGravity);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col, (float)nGravity);

		break;
		// Z�u���b�N
	case SHAPE_Z:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,(float)nGravity);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col, (float)nGravity);
		break;
		// �o�l�u���b�N
	case SHAPE_SPRING:
		m_col = COLOR_ORANGE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col, (float)nGravity);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col, (float)nGravity);
		break;
		// �N���X�u���b�N_1�i
	case SHAPE_CROSS_STEP_01:
		m_col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,(float)nGravity);
		break;
		// �N���X�u���b�N_2�i
	case SHAPE_CROSS_STEP_02:
		m_col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 2), &m_col,(float)nGravity);
		break;
		// ���󂫃v���X�u���b�N_1�i
	case SHAPE_PLUS_CENTER_HOLE_STEP_01:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,(float)nGravity);
		break;
		// ���󂫃v���X�u���b�N_2�i
	case SHAPE_PLUS_CENTER_HOLE_STEP_02:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 2), &m_col,(float)nGravity);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(0, 0, 0), NULL, (float)nGravity);

		break;
		// �v���X�u���b�N
	case SHAPE_PLUS:
		m_col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,(float)nGravity);
		break;
		// ���ˋN�v���X�u���b�N
	case SHAPE_PLUS_CENTER_PROTRUSION:
		m_col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col,(float)nGravity);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(0, 0, 0), NULL, (float)nGravity);
		break;
		// �Ίp���u���b�N_01
	case SHAPE_DIAGONAL_01:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,(float)nGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col, (float)nGravity);
		break;
		// �Ίp���u���b�N_02
	case SHAPE_DIAGONAL_02:
		m_col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,(float)nGravity);
		break;
		// �E�u���b�N_01
	case SHAPE_BEARD_01:
		m_col = COLOR_EMERALD;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,(float)nGravity);
		break;
		// �E�u���b�N_02
	case SHAPE_BEARD_02:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,(float)nGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col, (float)nGravity);
		break;
		// �E�u���b�N_03
	case SHAPE_BEARD_03:
		m_col = COLOR_ORANGE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,(float)nGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col, (float)nGravity);

		break;
		// �E�u���b�N_04
	case SHAPE_BEARD_04:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, (float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,(float)nGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,(float)nGravity);
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Draw(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Scene_MyCollision(
	int const & nObjType,
	CScene * pScene
)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����ɓ��Ă�ꂽ��̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{

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
// �쐬(�V�[���Ǘ�)
//	pos			: �ʒu
//	layer		: ���C���[
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock * CConnectblock::Create(
	D3DXVECTOR3		const & pos,									// �ʒu
	SHAPE			const & shape,									// �`���
	CScene::LAYER	const & layer									// ���C���[
)
{
	// �ϐ��錾
	CConnectblock * pConnectblock;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pConnectblock = new CConnectblock();
	// �ݒ�
	// �V�[���Ǘ��ݒ�
	pConnectblock->ManageSetting(layer);
	pConnectblock->m_pos = pos;				// �ʒu
	pConnectblock->m_Shape = shape;
	// ����������
	pConnectblock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pConnectblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�l�Ǘ�)
//	pos			: �ʒu
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock * CConnectblock::Create_Self(
	D3DXVECTOR3		const & pos,									// �ʒu
	SHAPE			const & shape									// �`���
)
{
	// �ϐ��錾
	CConnectblock * pConnectblock;		// �V�[��2D�N���X
									// �������̐���(����->��{�N���X,��->�h���N���X)
	pConnectblock = new CConnectblock;
	// �ݒ�
	pConnectblock->m_pos = pos;				// �ʒu
	// ����������
	pConnectblock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pConnectblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr�쐬(�l�Ǘ�unique)
// ���߂�l��std::move�Ŏ󂯎��
//	pos			: �ʒu
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CConnectblock> CConnectblock::Creat_Unique(
	D3DXVECTOR3		const & pos,									// �ʒu
	SHAPE			const & shape									// �`���
)
{
	// �ϐ��錾
	std::unique_ptr<CConnectblock> pConnectblock(new CConnectblock);		// �V�[��2D�N���X
	// �ݒ�
	pConnectblock->m_pos = pos;				// �ʒu
	// ����������
	pConnectblock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pConnectblock;
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
	// 5�b�Ԃ�1�񗎂��Ă���
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

		// �u���b�N�𗎂Ƃ���
		for (int nCnt = 0; nCnt < nDropBlock; nCnt++)
		{
			// �������ꂽ�u���b�N�̐���SHAPE_MAX SHAPE_BEARD_04
			Create(D3DVECTOR3_ZERO, (SHAPE)(CCalculation::GetRandomRange(SHAPE_RECT, SHAPE_MAX - 1)));
		}
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

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Debug(void)
{

}
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
			CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]),NULL, 1.0f);
		}
		// �΂˃u���b�N����
		if (ImGui::Button("CreateSpring"))
		{
			CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
		}
		// �|�S�u���b�N����
		if (ImGui::Button("CreateSteel"))
		{
			CSteelblock::Create(CScene_X::TYPE_BLOCK, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);
		}
	}
	ImGui::End();
}
#endif // _DEBUG

