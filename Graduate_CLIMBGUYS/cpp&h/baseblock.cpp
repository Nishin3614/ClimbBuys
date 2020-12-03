// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �x�[�X�u���b�N���� [baseblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"
#include "debugproc.h"
#include "stencilshadow.h"
#include "game.h"
#include "circleshadow.h"
#include "bombblock..h"
#include "electricblock.h"
#include "3dparticle.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define BLOCK_GRAVITY			(1.0f)									// �u���b�N�ɂ�����d��
#define STENCIL_SIZE_Y			(200.0f)								// �X�e���V���o�b�t�@��y�̃T�C�Y
#define BLOCK_SIZE_RANGE		(25.0f)									// �u���b�N�̃T�C�Y�͈̔�
#define BLOCK_SHADOWSIZE		(25.0f)									// �X�e���V���V���h�E�̃T�C�Y
#define BLOCK_STATUS_TXT		("data/LOAD/STATUS/BlockStatus.txt")	// �u���b�N�̃X�e�[�^�X�̃e�L�X�g

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����p
CBaseblock::BLOCK_STATUS CBaseblock::m_BlockStatus = {};
int	CBaseblock::m_nPhase = 0;					// �t�F�[�Y
int CBaseblock::m_anHeight[20][20] = {};		// 1��1�̍s��̍���

float	CBaseblock::m_fSizeRange = 0.0f;		// �T�C�Y�͈�
std::vector<int>	CBaseblock::m_nFeedValue;	// �t�F�[�h�̒l

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::CBaseblock() : CScene_X::CScene_X()
{
	m_pShadowPolygon = NULL;							// �V���h�E�|���S��
	m_BlockType = BLOCKTYPE_NORMAL;								// �^�C�v
	m_bFall = false;									// ��������
	CScene::SetObj(CScene::OBJ::OBJ_BLOCK);				// �I�u�W�F�N�g�^�C�v�̐ݒ�
	m_PushAfeter.bPushState = false;					// ������������
	m_PushAfeter.PushGrid = GRID(0,0,0);				// �����o����
	m_bShadow = true;									// �V���h�E�̎g�p���
	m_fGravity = 0.0f;									// �d��
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::~CBaseblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Init()
{
	// �V�[��X�̏���������
	CScene_X::Init();
	// �t�B�[���h�u���b�N�ȊO�Ȃ�
	if (m_BlockType != BLOCKTYPE_FIELD)
	{
		// �V���h�E�|���S���̐���
		m_pShadowPolygon = CCircleshadow::Create(CScene_X::GetPos(),
			D3DXVECTOR3(BLOCK_SHADOWSIZE, 0.0f, BLOCK_SHADOWSIZE),
			-1
		);
		m_pShadowPolygon->SetCol(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
		m_pShadowPolygon->Set_Vtx_Col();
	}
	else
	{
		m_fGravity = 1.0f;									// �d��
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update(void)
{
	// �O��̈ʒu����
	m_posOld = CScene_X::GetPos();
	// �V�[��X�X�V����
	CScene_X::Update();
	// �����鏈��
	Update_Fall();
	// �����o����Ԃ̃u���b�N�X�V����
	Update_PushState();
	// ���g�̃V���h�E�̏o����������
	Update_MyShadow();
	// �u���b�N�̃��[�v
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK);nCntBlock++)
	{
		// ���擾
		CBaseblock * pBlock = (CBaseblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);	// �u���b�N���
		// �擾�����u���b�N��NULL�`�F�b�N ||
		// �擾�����u���b�N�ƌ��݃u���b�N������
		// ->���[�v�X�L�b�v
		if (pBlock == NULL ||
			pBlock == this) continue;
		if (pBlock->m_grid.nColumn != this->m_grid.nColumn ||
			pBlock->m_grid.nLine != this->m_grid.nLine) continue;
		// �����蔻�菈��
		Collision(pBlock);
		// �u���b�N�̃X�e���V���V���h�E�̍X�V
		Update_OtherShadow(pBlock);
	}

	// �v�b�V������Ă����Ԃł͂Ȃ��Ȃ�
	if (!m_PushAfeter.bPushState) return;
	COLLISIONDIRECTION		Direct = COLLISIONDIRECTION::NONE;	// �����蔻��̕���
	// �v���C���[�̃��[�v
	for (int nCntPlayer = 0; nCntPlayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntPlayer++)
	{
		// ���擾
		CPlayer * pPlayer = (CPlayer *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntPlayer);	// �v���C���[���
		// �擾�����u���b�N��NULL�`�F�b�N ||
		// �擾�����u���b�N�ƌ��݃u���b�N������
		// ->���[�v�X�L�b�v
		if (pPlayer == NULL) continue;
		D3DXVECTOR3 PlayerSize = CPlayer::GetPlayerStatus().PlayerSize;
		CPlayer::DIESTATUS &PlayerDieStatus = pPlayer->GetPlayerDieStatus();
		Direct = CBaseblock::PushCollision(
			CScene::OBJ_PLAYER,
			&pPlayer->GetPos(),
			&pPlayer->GetPosOld(),
			&pPlayer->GetMove(),
			&PlayerSize,
			CPlayer::GetPlayerStatus().PlayerOffSet
		);

		// �u���b�N�̔���
		// �O
		if (Direct == COLLISIONDIRECTION::FRONT)
		{
			PlayerDieStatus.bFront = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::BACK)
		{
			PlayerDieStatus.bBack = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::LEFT)
		{
			PlayerDieStatus.bLeft = true;
		}
		// �E
		else if (Direct == COLLISIONDIRECTION::RIGHT)
		{
			PlayerDieStatus.bRight = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::UP)
		{
			PlayerDieStatus.bUp = true;
		}
		// ��
		else if (Direct == COLLISIONDIRECTION::DOWN)
		{
			PlayerDieStatus.bDown = true;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����鏈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_Fall(void)
{
	// �������ԂłȂ����
	// �֐��𔲂���
	if (!m_bFall) return;
	// �ʒu���擾
	D3DXVECTOR3 pos = CScene_X::GetPos();
	// y�ʒu�X�V
	pos.y -= m_fGravity;
	// �ʒu�ݒ�
	CScene_X::SetPos(pos);
	// �u���b�N�̏��
	if (pos.y < -500.0f)
	{
		// �����[�X����
		Release();
		// �V���h�E�����݂��Ă���Ȃ�
		if (m_pShadowPolygon)
		{
			// �V���h�E�������[�X
			m_pShadowPolygon->Release();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����o����Ԃ̃u���b�N�̍X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_PushState(void)
{
	// �����o����Ԃ�false�Ȃ�
	// ->�֐��𔲂���
	if (!m_PushAfeter.bPushState) return;
	// �ʒu���擾
	D3DXVECTOR3 pos = CScene_X::GetPos();
	// �ʒu�X�V
	// ��̒l���ݒ肳��Ă���Ȃ�
	if (m_PushAfeter.PushGrid.nLine)
	{
		// z�ʒu�̍X�V
		pos.z += m_BlockStatus.fMove * m_PushAfeter.PushGrid.nLine;
		if (m_PushAfeter.PushGrid.nLine < 0 &&	// ��0����
			pos.z < m_PushAfeter.GoalPos.z)			// z�ʒu���K��l�����Ȃ�
		{
			// �ʒu�ݒ�
			pos.z = m_PushAfeter.GoalPos.z;
			// �����O�̃u���b�N�̏�ɂ������u���b�N�𗎂Ƃ�����
			CBaseblock::FallBlock_Grid(m_grid);
			// �������u���b�N�̌��݂܂ł����s��̍��������X�V
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// ��������̍s�񍂂�ݒ�
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// �����鏈��
			this->SetFall(true);
			// �����o����̕ϐ�������������
			m_PushAfeter.Init();

		}
		else if (m_PushAfeter.PushGrid.nLine > 0 &&	// ��0����
			pos.z > m_PushAfeter.GoalPos.z)				// z�ʒu���K��l���߂Ȃ�
		{
			// �ʒu�ݒ�
			pos.z = m_PushAfeter.GoalPos.z;
			// �����O�̃u���b�N�̏�ɂ������u���b�N�𗎂Ƃ�����
			CBaseblock::FallBlock_Grid(m_grid);
			// �������u���b�N�̌��݂܂ł����s��̍��������X�V
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// ��������̍s�񍂂�ݒ�
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// �����鏈��
			this->SetFall(true);
			// �����o����̕ϐ�������������
			m_PushAfeter.Init();
		}
	}
	// �s�̒l���ݒ肳��Ă���Ȃ�
	else if (m_PushAfeter.PushGrid.nColumn)
	{
		// x�ʒu�̍X�V
		pos.x += m_BlockStatus.fMove * m_PushAfeter.PushGrid.nColumn;
		if (m_PushAfeter.PushGrid.nColumn < 0 &&	// ��0����
			pos.x < m_PushAfeter.GoalPos.x)			// z�ʒu���K��l�����Ȃ�
		{
			// �ʒu�ݒ�
			pos.x = m_PushAfeter.GoalPos.x;
			// �����O�̃u���b�N�̏�ɂ������u���b�N�𗎂Ƃ�����
			CBaseblock::FallBlock_Grid(m_grid);
			// �������u���b�N�̌��݂܂ł����s��̍��������X�V
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// ��������̍s�񍂂�ݒ�
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// �����鏈��
			this->SetFall(true);
			// �����o����̕ϐ�������������
			m_PushAfeter.Init();
		}
		else if (m_PushAfeter.PushGrid.nColumn > 0 &&	// ��0����
			pos.x > m_PushAfeter.GoalPos.x)				// z�ʒu���K��l���߂Ȃ�
		{
			// �ʒu�ݒ�
			pos.x = m_PushAfeter.GoalPos.x;
			// �����O�̃u���b�N�̏�ɂ������u���b�N�𗎂Ƃ�����
			CBaseblock::FallBlock_Grid(m_grid);
			// �������u���b�N�̌��݂܂ł����s��̍��������X�V
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// ��������̍s�񍂂�ݒ�
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// �����鏈��
			this->SetFall(true);
			// �����o����̕ϐ�������������
			m_PushAfeter.Init();
		}
	}
	else
	{
		// �����o����̕ϐ�������������
		m_PushAfeter.Init();
	}
	//
	// �ʒu�ݒ�
	CScene_X::SetPos(pos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���g�̃V���h�E�̏o����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_MyShadow(void)
{
	// �t�B�[���h�u���b�N�Ȃ�
	if (m_BlockType == BLOCKTYPE_FIELD)
	{
		m_bShadow = false;
		return;
	}
	// �V���h�E�̃|���S����NULL�`�F�b�N
	else if (!m_pShadowPolygon)
	{
		m_bShadow = false;
		return;
	}
	// �^���Ƀu���b�N�����݂��Ă��Ȃ��Ȃ�
	else if (CBaseblock::GetHeight(this->GetGrid().nColumn, this->GetGrid().nLine) < 0)
	{
		m_pShadowPolygon->SetShadow(false);
		m_pShadowPolygon->Release();
		m_pShadowPolygon = NULL;
		m_bShadow = false;
		return;
	}
	// �����Ă����ԂȂ�
	// ->�V���h�E�̎g�p��Ԃ�false��
	else if (!this->GetFall())
	{
		m_pShadowPolygon->SetShadow(false);
		m_bShadow = false;
		return;
	}
	else
	{
		m_bShadow = true;
	}
	// �ϐ��錾
	D3DXVECTOR3 ShadowPos;	// �V���h�E�ʒu
	// xy�̈ʒu�ݒ�
	ShadowPos = m_pShadowPolygon->GetPos();
	ShadowPos.x = CScene_X::GetPos().x;
	ShadowPos.z = CScene_X::GetPos().z;
	m_pShadowPolygon->SetPos(ShadowPos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻�菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Collision(CBaseblock * pBlock)
{
	// �������Ȃ���ԂȂ�
	if (!this->GetFall() ||
		this->GetType() == CBaseblock::BLOCKTYPE_FIELD) return;
	// �ϐ��錾
	COLLISIONDIRECTION Direct;	// �����蔻�����
	// �����蔻��
	Direct = Collision(
		CScene::OBJ_BLOCK,
		&pBlock->GetPos(),
		&pBlock->GetPosOld(),
		&pBlock->GetModel()->size,
		D3DXVECTOR3(0.0f, pBlock->GetModel()->size.y, 0.0f)
	);
	// �ǂ��ɂ��������Ă��Ȃ����
	if (Direct == COLLISIONDIRECTION::NONE) return;
	// �������ɓ������Ă�����
	else if (Direct == COLLISIONDIRECTION::DOWN)
	{
		// ����̗������Ԃ�true�Ȃ�
		// ->�֐��𔲂���
		if (pBlock->GetFall()) return;
		// �ϐ��錾
		CBaseblock::GRID MyGrid = this->GetGrid();		// �����̍s��
		CBaseblock::GRID OppGrid = pBlock->GetGrid();	// ����̍s��
		// �����s��ł͂Ȃ��Ȃ�
		// ->�֐��𔲂���
		if (!(MyGrid.nColumn == OppGrid.nColumn &&
			MyGrid.nLine == OppGrid.nLine)) return;
		// �ϐ��錾
		int nHeight = this->GetHeight(				// ����
			MyGrid.nColumn,
			MyGrid.nLine) + 1;
		// �������s�񍂂ɑ��
		MyGrid.nHeight = nHeight;
		// �����̐ݒ�
		this->SetHeight(
			MyGrid.nColumn,
			MyGrid.nLine,
			MyGrid.nHeight
		);
		// ���݂̍s�񍂂̐ݒ�
		this->SetGrid(MyGrid);
		// �ʒu�ݒ�
		this->SetPos(MyGrid.GetPos(m_fSizeRange));
		// �����Ă����Ԑݒ�
		this->SetFall(false);
	}
	else if (Direct == COLLISIONDIRECTION::UP) {}
	else
	{
		// �v���C���[���牟����Ă��Ȃ��ꍇ
		// ->�֐��𔲂���
		if (!pBlock->m_PushAfeter.bPushState) return;
		// �����Ă���u���b�N������̃u���b�N�̈ʒu�̔����ȏ�̍����Ȃ�
		else if (this->GetPos().y > pBlock->GetPos().y + m_fSizeRange * 0.5f)
		{
			// �ϐ��錾
			CBaseblock::GRID FallGrid = this->GetGrid();	// �s��
			// ����̍s�񍂂̐ݒ�
			pBlock->SetGrid(CBaseblock::GRID(FallGrid.nColumn,pBlock->GetGrid().nHeight,FallGrid.nLine));
			// ����̓�������̈ʒu�ݒ�
			pBlock->SetPos(pBlock->GetGrid().GetPos(m_fSizeRange));
			// ���g�̍s�񍂂̐ݒ�
			this->SetGrid(CBaseblock::GRID(FallGrid.nColumn, pBlock->GetGrid().nHeight + 1, FallGrid.nLine));
			// ���g�̓�������̈ʒu�ݒ�
			this->SetPos(pBlock->GetGrid().GetPos(m_fSizeRange));
			// �����鏈��
			this->SetFall(false);
			// �������u���b�N�̌��݂܂ł����s��̍��������X�V
			CBaseblock::SetHeight(this->GetGrid() + CBaseblock::GRID(0, -1, 0));
			// �����o���͂̏�����
			pBlock->m_PushAfeter = PUSHAFTER(false, CBaseblock::GRID(0, 0, 0));
		}
		else
		{
			// �v���C���[���牟����Ă��Ȃ��ꍇ
			// ->�֐��𔲂���
			if (!pBlock->m_PushAfeter.bPushState) return;
			// �����o���͂̏�����
			pBlock->m_PushAfeter = PUSHAFTER(false,CBaseblock::GRID(0,0,0));
			// ���̈ʒu�ɖ߂�
			pBlock->SetPos(pBlock->GetGrid().GetPos(m_fSizeRange));
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �V���h�E�̍X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_OtherShadow(CBaseblock * pBlock)
{
	// �V���h�E�̎g�p��Ԃ�false�Ȃ�
	if (!m_bShadow) return;
	// ��}�X���Ƀu���b�N������Ȃ�
	// ->�V���h�E�̎g�p��Ԃ�false��
	else if (m_grid.nHeight - 1 == pBlock->GetGrid().nHeight)
	{
		m_pShadowPolygon->SetShadow(false);
		return;
	}

	// ���������ݒu����Ă���u���b�N�̍�����荂��������
	else if (this->m_grid.nHeight > CBaseblock::GetHeight(pBlock->GetGrid().nColumn, pBlock->GetGrid().nLine) &&
		this->m_grid.nHeight > pBlock->GetGrid().nHeight)
	{
		m_pShadowPolygon->SetShadow(true);
		// �ϐ��錾
		float fDiffHeight = this->GetPos().y - pBlock->GetPos().y;	// ��r���������̍�
		// �������K��l�ȏ�Ȃ�
		if (fDiffHeight >= m_fSizeRange * m_BlockStatus.nAppearance)
		{
			// �����x�̍X�V
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_BlockStatus.fBasicShadowSize));
			m_pShadowPolygon->Set_Vtx_Col();
		}
		// ����ȊO
		else
		{
			// �����x�̍X�V
			float fSize = m_fSizeRange * m_BlockStatus.nAppearance - fDiffHeight;
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_BlockStatus.fBasicShadowSize * fSize));
			m_pShadowPolygon->Set_Vtx_Col();
		}

	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Scene_MyCollision(
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
void CBaseblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����o�������蔻��
//	pos		: �ʒu
//	posOld	: �O��̈ʒu
//	move	: �ړ���
//	size	: �T�C�Y
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushCollision(
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
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	CBaseblock::GRID MyGrid = this->GetGrid();					// �s��
	CGame::STAGE Stage = CGame::GetStage();						// �X�e�[�W
		// �f�ނ�Z�͈�
	if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f)
	{
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f)
		{
			// �����蔻��(��)
			if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= BlockPos.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;

				// �f�ޏ�̍���
				pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y - 3.0f;
				//posOld->y = pos->y;
				// �ړ��ʂ̏�����
				move->y = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= BlockPos.y + m_fSizeRange)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
				// �f�ޏ�̍���
				pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
				//posOld->y = pos->y;
				// �ړ��ʂ̏�����
				move->y = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
				// �^�C�v���{���Ȃ�
				if (m_BlockType == BLOCKTYPE_BOMB)
				{
					// �{���̏�Ԑݒ�
					CBombblock * pBombBlock = (CBombblock *)this;
					pBombBlock->SetbBomb(true);
				}
				// �^�C�v���d�C�Ȃ�
				else if (m_BlockType == BLOCKTYPE_ELECTRIC)
				{
					// �d�C�̏�Ԑݒ�
					CElectricblock * pElectBlock = (CElectricblock *)this;
					pElectBlock->SetElectric(true);
				}

			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// �����蔻��(��)
				if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
					pos->y + OffsetPos.y + size->y * 0.5f <= m_posOld.y)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::DOWN;

					// �f�ޏ�̍���
					pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;
					//posOld->y = pos->y;

					// �ړ��ʂ̏�����
					move->y = 0.0f;
					// �����o����Ԃ�true
					bPush = true;
				}

				// �����蔻��(��)
				else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y * 0.5f >= m_posOld.y + m_fSizeRange)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::UP;
					// �f�ޏ�̍���
					pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
					//posOld->y = pos->y;
					// �ړ��ʂ̏�����
					move->y = 0.0f;
					// �����o����Ԃ�true
					bPush = true;
					// �^�C�v���{���Ȃ�
					if (m_BlockType == BLOCKTYPE_BOMB)
					{
						// �{���̏�Ԑݒ�
						CBombblock * pBombBlock = (CBombblock *)this;
						pBombBlock->SetbBomb(true);
					}
				}
				// �����蔻��(��)
				else if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
					pos->y + OffsetPos.y <= BlockPos.y)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::DOWN;
				}

				// �����蔻��(��)
				else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y > BlockPos.y + m_fSizeRange)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::UP;
					// �^�C�v���{���Ȃ�
					if (m_BlockType == BLOCKTYPE_BOMB)
					{
						// �{���̏�Ԑݒ�
						CBombblock * pBombBlock = (CBombblock *)this;
						pBombBlock->SetbBomb(true);
					}
				}
			}
		}
	}
	// �������������ɏ�񂪓����Ă���Ȃ�
	//if (bPush) return Direct;
	// �f�ނ�Y�͈�
	if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
		pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange)
	{
		// �f�ނ�Z�͈�
		if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f)
		{
			// �����蔻��(��)
			if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
				// �f�ޏ�̍���
				pos->x = BlockPos.x - m_fSizeRange * 0.5f - size->x * 0.5f - OffsetPos.x;
				//posOld->x = pos->x;
				// �ړ��ʂ̏�����
				move->x = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}
			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
				// �f�ޏ�̍���
				pos->x = BlockPos.x + m_fSizeRange * 0.5f + size->x * 0.5f - OffsetPos.x;
				//posOld->x = pos->x;
				// �ړ��ʂ̏�����
				move->x = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// �����蔻��(��)
				if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x + size->x * 0.5f <= m_posOld.x - m_fSizeRange * 0.5f)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::LEFT;
					// �f�ޏ�̍���
					pos->x = BlockPos.x - m_fSizeRange * 0.5f - size->x * 0.5f - OffsetPos.x;
					//posOld->x = pos->x;
					// �ړ��ʂ̏�����
					move->x = 0.0f;
					// �����o����Ԃ�true
					bPush = true;
				}
				// �����蔻��(�E)
				else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x - size->x * 0.5f >= m_posOld.x + m_fSizeRange * 0.5f)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::RIGHT;
					// �f�ޏ�̍���
					pos->x = BlockPos.x + m_fSizeRange * 0.5f + size->x * 0.5f - OffsetPos.x;
					//posOld->x = pos->x;
					// �ړ��ʂ̏�����
					move->x = 0.0f;
					// �����o����Ԃ�true
					bPush = true;
				}
				// �����蔻��(��)
				else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::LEFT;
				}
				// �����蔻��(�E)
				else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::RIGHT;
				}
			}
		}
		// �������������ɏ�񂪓����Ă���Ȃ�
		//if (bPush) return Direct;
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f)
		{
			// �����蔻��(��O)
			if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;
				// �f�ޏ�̍���
				pos->z = BlockPos.z - m_fSizeRange * 0.5f - size->z * 0.5f - OffsetPos.z;
				//posOld->z = pos->z;
				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}

			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;

				// �f�ޏ�̍���
				pos->z =
					BlockPos.z + m_fSizeRange * 0.5f +
					size->z * 0.5f - OffsetPos.z;
				//posOld->z = pos->z;

				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// �����蔻��(��O)
				if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z + size->z * 0.5f <= m_posOld.z - m_fSizeRange * 0.5f)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::BACK;
					// �f�ޏ�̍���
					pos->z = BlockPos.z - m_fSizeRange * 0.5f - size->z * 0.5f - OffsetPos.z;
					//posOld->z = pos->z;
					// �ړ��ʂ̏�����
					move->z = 0.0f;
				}

				// �����蔻��(��)
				else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z - size->z * 0.5f >= m_posOld.z + m_fSizeRange * 0.5f)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::FRONT;

					// �f�ޏ�̍���
					pos->z =
						BlockPos.z + m_fSizeRange * 0.5f +
						size->z * 0.5f - OffsetPos.z;
					//posOld->z = pos->z;

					// �ړ��ʂ̏�����
					move->z = 0.0f;
				}
				// �����蔻��(��O)
				else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::BACK;
				}
				// �����蔻��(��)
				else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z)
				{
					// �߂荞��ł���
					Direct = COLLISIONDIRECTION::FRONT;
				}
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
COLLISIONDIRECTION CBaseblock::Collision(
	CScene::OBJ const & Obj,		// �I�u�W�F�^�C�v
	D3DXVECTOR3 * pos,				// �ʒu
	D3DXVECTOR3 * posOld,			// �O��̈ʒu
	D3DXVECTOR3 * size,				// �T�C�Y
	D3DXVECTOR3 const & OffsetPos	// �I�t�Z�b�g�ʒu
)
{
	// �ϐ��錾
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// �ǂ��̓����蔻�肩
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	// �f�ނ�Z�͈�
	if (pos->z + OffsetPos.z + size->z * 0.5f >= BlockPos.z - m_fSizeRange * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + m_fSizeRange * 0.5f)
	{
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f >= BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + m_fSizeRange * 0.5f)
		{
			// �����蔻��(��)
			if (pos->y + OffsetPos.y >= BlockPos.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y < BlockPos.y + m_fSizeRange)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// �f�ނ�Y�͈�
	if (pos->y + OffsetPos.y + size->y * 0.5f >= BlockPos.y&&
		pos->y + OffsetPos.y - size->y * 0.5f <= BlockPos.y + m_fSizeRange)
	{
		// �f�ނ�Z�͈�
		if (pos->z + OffsetPos.z + size->z * 0.5f >= BlockPos.z - m_fSizeRange * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + m_fSizeRange * 0.5f)
		{
			// �����蔻��(��)
			if (pos->x + OffsetPos.x + size->z * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x + size->z * 0.5f <= BlockPos.x - m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->z * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x - size->z * 0.5f >= BlockPos.x + m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}

		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f >= BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + m_fSizeRange * 0.5f)
		{
			// �����蔻��(��O)
			if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;
			}
			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̉����o������
//	LineBegin	: ���̎n�_
//	LineEnd		: ���̏I�_
//	fDistance	: ����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushBlock(
	D3DXVECTOR3 const & LineBegin,	// 	LineBegin	: ���̎n�_
	D3DXVECTOR3 const & LineEnd,	// 	LineEnd		: ���̏I�_
	float &				fDistance	//	fDistance	: ����
)
{
	// �v���C���[�ƕʂ̃I�u�W�F�N�g�ɓ����蔻��
	if (this->m_BlockType == BLOCKTYPE_FIELD) return COLLISIONDIRECTION::NONE;
	// �ϐ��錾
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// �ǂ��̓����蔻�肩
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	// ���̃|���S���̔���
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::LEFT
	))
	{
		Direct = COLLISIONDIRECTION::LEFT;
	}
	// �E�̃|���S���̔���
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::RIGHT
	))
	{
		Direct = COLLISIONDIRECTION::RIGHT;
	}

	// ���̃|���S���̔���
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::FRONT
	))
	{
		Direct = COLLISIONDIRECTION::FRONT;
	}

	// ��O�̃|���S���̔���
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::BACK
	))
	{
		Direct = COLLISIONDIRECTION::BACK;
	}

	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����o����̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetPushAfter(PUSHAFTER const & PushAfter)
{
	m_PushAfeter = PushAfter;
	m_PushAfeter.GoalPos = (m_grid + m_PushAfeter.PushGrid).GetPos(m_fSizeRange);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �w�肵���x�[�X�u���b�N���폜���鏈��
//	pBlock	: �u���b�N���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CBaseblock::DeleteBlock(
	CBaseblock * pBlock	// �u���b�N���
)
{
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::PARTICLE_ID_EXPLOSION,
		pBlock->GetPos()
	);

	// �����O�̃u���b�N�̏�ɂ������u���b�N�𗎂Ƃ�����
	CBaseblock::FallBlock_Grid(pBlock->GetGrid());
	// �������u���b�N�̌��݂܂ł����s��̍��������X�V
	CBaseblock::SetHeight(pBlock->GetGrid() + CBaseblock::GRID(0, -1, 0));
	// �V���h�E�̃����[�X����
	if (pBlock->m_pShadowPolygon)
	{
		// �V���h�E�������[�X
		pBlock->m_pShadowPolygon->Release();
	}
	// �����[�X����
	pBlock->Release();
	return true;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �x�[�X�u���b�N�S�\�[�X�̓ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CBaseblock::Load(void)
{
	m_fSizeRange = BLOCK_SIZE_RANGE;
	BlockStatusLoad();
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::UnLoad(void)
{
	// �t�F�[�h�̒l�̊J��
	m_nFeedValue.clear();
	m_nFeedValue.shrink_to_fit();
	// ������u���b�N���̊J��
	m_BlockStatus.v_nDropBlock.clear();
	m_BlockStatus.v_nDropBlock.shrink_to_fit();
	// �����鑬�x�̊J��
	m_BlockStatus.v_nBlockGravity.clear();
	m_BlockStatus.v_nBlockGravity.shrink_to_fit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�V�[���Ǘ�)
//	pos			: �ʒu
//	nModelId	: ���f���ԍ�
//	layer		: ���C���[
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock * CBaseblock::Create(
	D3DXVECTOR3		const & pos,		// �ʒu
	int				const & nModelId,	// ���f���ԍ�
	CScene::LAYER	const & layer		// ���C���[
)
{
	// �ϐ��錾
	CBaseblock * pBaseblock;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pBaseblock = new CBaseblock();
	// �ݒ�
	// �V�[���Ǘ��ݒ�
	pBaseblock->ManageSetting(layer);
	pBaseblock->SetPos(pos);			// �ʒu
	pBaseblock->SetModelId(nModelId);	// ���f���ԍ�
	// ����������
	pBaseblock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pBaseblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�l�Ǘ�)
//	pos			: �ʒu
//	nModelId	: ���f���ԍ�
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock * CBaseblock::Create_Self(
	D3DXVECTOR3		const & pos,									// �ʒu
	int				const & nModelId								// ���f���ԍ�
)
{
	// �ϐ��錾
	CBaseblock * pBaseblock;		// �V�[��2D�N���X
									// �������̐���(����->��{�N���X,��->�h���N���X)
	pBaseblock = new CBaseblock;
	// �ݒ�
	pBaseblock->SetPos(pos);			// �ʒu
	pBaseblock->SetModelId(nModelId);	// �ʒu
	// ����������
	pBaseblock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pBaseblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr�쐬(�l�Ǘ�unique)
// ���߂�l��std::move�Ŏ󂯎��
//	pos			: �ʒu
//	nModelId	: ���f���ԍ�
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CBaseblock> CBaseblock::Creat_Unique(
	D3DXVECTOR3		const & pos,									// �ʒu
	int				const & nModelId								// ���f���ԍ�
)
{
	// �ϐ��錾
	std::unique_ptr<CBaseblock> pBaseblock(new CBaseblock);		// �V�[��2D�N���X
																// �ݒ�
	pBaseblock->SetPos(pos);			// �ʒu
	pBaseblock->SetModelId(nModelId);	// �ʒu
	// ����������
	pBaseblock->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pBaseblock;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �^�C�g���p �u���b�N����Ăɐ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::CreateInBulkBlock()
{
	static D3DXVECTOR3 BlockPos[94] = {};

	// C
	BlockPos[0] = D3DXVECTOR3(-525,0,0);
	BlockPos[1] = D3DXVECTOR3(-500, 0, 0);
	BlockPos[2] = D3DXVECTOR3(-475, 0, 0);
	BlockPos[3] = D3DXVECTOR3(-550,25,0);
	BlockPos[4] = D3DXVECTOR3(-550, 50, 0);
	BlockPos[5] = D3DXVECTOR3(-550, 75, 0);
	BlockPos[6] = D3DXVECTOR3(-525, 100, 0);
	BlockPos[7] = D3DXVECTOR3(-500, 100, 0);
	BlockPos[8] = D3DXVECTOR3(-475, 100, 0);

	// L
	BlockPos[9] = D3DXVECTOR3(-425,0,0);
	BlockPos[10] = D3DXVECTOR3(-375, 0, 0);
	BlockPos[11] = D3DXVECTOR3(-400, 0, 0);
	BlockPos[12] = D3DXVECTOR3(-425, 25, 0);
	BlockPos[13] = D3DXVECTOR3(-425, 50, 0);
	BlockPos[14] = D3DXVECTOR3(-425, 75, 0);
	BlockPos[15] = D3DXVECTOR3(-425, 100, 0);

	// I
	BlockPos[16] = D3DXVECTOR3(-325,0,0);
	BlockPos[17] = D3DXVECTOR3(-300,0,0);
	BlockPos[18] = D3DXVECTOR3(-275,0,0);
	BlockPos[19] = D3DXVECTOR3(-300, 25, 0);
	BlockPos[20] = D3DXVECTOR3(-300, 50, 0);
	BlockPos[21] = D3DXVECTOR3(-300, 75, 0);
	BlockPos[22] = D3DXVECTOR3(-325, 100, 0);
	BlockPos[23] = D3DXVECTOR3(-300, 100, 0);
	BlockPos[24] = D3DXVECTOR3(-275, 100, 0);

	// M
	BlockPos[25] = D3DXVECTOR3(-225,0,0);
	BlockPos[26] = D3DXVECTOR3(-100,0,0);
	BlockPos[27] = D3DXVECTOR3(-225, 25, 0);
	BlockPos[28] = D3DXVECTOR3(-100, 25, 0);
	BlockPos[29] = D3DXVECTOR3(-225, 50, 0);
	BlockPos[30] = D3DXVECTOR3(-100, 50, 0);
	BlockPos[31] = D3DXVECTOR3(-225, 75, 0);
	BlockPos[32] = D3DXVECTOR3(-100, 75, 0);
	BlockPos[35] = D3DXVECTOR3(-175, 75, 0);
	BlockPos[36] = D3DXVECTOR3(-150, 75, 0);
	BlockPos[33] = D3DXVECTOR3(-225, 100, 0);
	BlockPos[34] = D3DXVECTOR3(-100, 100, 0);
	BlockPos[37] = D3DXVECTOR3(-200, 100, 0);
	BlockPos[38] = D3DXVECTOR3(-125, 100, 0);

	//B
	BlockPos[39] = D3DXVECTOR3(-50, 0 , 0);
	BlockPos[40] = D3DXVECTOR3(-25, 0 , 0);
	BlockPos[41] = D3DXVECTOR3(-0, 0, 0);
	BlockPos[42] = D3DXVECTOR3(-50, 25, 0);
	BlockPos[43] = D3DXVECTOR3(25, 25, 0);
	BlockPos[44] = D3DXVECTOR3(-50, 50, 0);
	BlockPos[45] = D3DXVECTOR3(-25, 50, 0);
	BlockPos[46] = D3DXVECTOR3(0, 50, 0);
	BlockPos[47] = D3DXVECTOR3(-50, 75, 0);
	BlockPos[48] = D3DXVECTOR3(25, 75, 0);
	BlockPos[49] = D3DXVECTOR3(-50, 100, 0);
	BlockPos[50] = D3DXVECTOR3(-25,100 , 0);
	BlockPos[51] = D3DXVECTOR3(0, 100, 0);

	// G
	BlockPos[52] = D3DXVECTOR3(125, 0, 0);
	BlockPos[53] = D3DXVECTOR3(150, 0, 0);
	BlockPos[54] = D3DXVECTOR3(175, 0,0);
	BlockPos[55] = D3DXVECTOR3(100, 25,0);
	BlockPos[56] = D3DXVECTOR3(175, 25,0);
	BlockPos[59] = D3DXVECTOR3(100, 50,0);
	BlockPos[60] = D3DXVECTOR3(150, 50,0);
	BlockPos[61] = D3DXVECTOR3(175, 50,0);
	BlockPos[61] = D3DXVECTOR3(100, 75,0);
	BlockPos[63] = D3DXVECTOR3(125, 100,0);
	BlockPos[64] = D3DXVECTOR3(150, 100,0);
	BlockPos[65] = D3DXVECTOR3(175, 100,0);

	//U
	BlockPos[66] = D3DXVECTOR3(250, 0, 0);
	BlockPos[67] = D3DXVECTOR3(275, 0, 0);
	BlockPos[68] = D3DXVECTOR3(225, 25, 0);
	BlockPos[69] = D3DXVECTOR3(300, 25, 0);
	BlockPos[70] = D3DXVECTOR3(225, 50, 0);
	BlockPos[71] = D3DXVECTOR3(300, 50, 0);
	BlockPos[72] = D3DXVECTOR3(225, 75, 0);
	BlockPos[73] = D3DXVECTOR3(300, 75, 0);
	BlockPos[74] = D3DXVECTOR3(225, 100, 0);
	BlockPos[75] = D3DXVECTOR3(300, 100, 0);

	// Y
	BlockPos[76] = D3DXVECTOR3(400, 0, 0);
	BlockPos[77] = D3DXVECTOR3(400, 25, 0);
	BlockPos[78] = D3DXVECTOR3(400, 50, 0);
	BlockPos[79] = D3DXVECTOR3(375, 75, 0);
	BlockPos[80] = D3DXVECTOR3(425, 75, 0);
	BlockPos[81] = D3DXVECTOR3(350, 100, 0);
	BlockPos[82] = D3DXVECTOR3(450, 100, 0);

	// S
	BlockPos[83] = D3DXVECTOR3(500, 0, 0);
	BlockPos[84] = D3DXVECTOR3(525, 0, 0);
	BlockPos[85] = D3DXVECTOR3(550, 0, 0);
	BlockPos[86] = D3DXVECTOR3(550, 25, 0);
	BlockPos[87] = D3DXVECTOR3(500, 50, 0);
	BlockPos[88] = D3DXVECTOR3(525, 50, 0);
	BlockPos[89] = D3DXVECTOR3(550, 50, 0);
	BlockPos[90] = D3DXVECTOR3(500, 75, 0);
	BlockPos[91] = D3DXVECTOR3(500, 100, 0);
	BlockPos[92] = D3DXVECTOR3(525, 100, 0);
	BlockPos[93] = D3DXVECTOR3(550, 100, 0);

	// �^�C�g���ɕ\������u���b�N�̐���
	for (int nBlockCnt = 0; nBlockCnt < 94; nBlockCnt++)
	{
		// �ϐ��錾

		CBaseblock * pBaseblock;								// �V�[��2D�N���X
		pBaseblock = Create(BlockPos[nBlockCnt], 2);			// ����

		//pBaseblock->SetSize(D3DXVECTOR3(0.25f, 0.25f, 0.25f));	// �T�C�Y�ݒ�
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �s�񍂂��x�[�X�u���b�N�̏��擾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock * CBaseblock::GetBaseBlock(GRID & Grid)
{
	// �ϐ��錾
	CBaseblock * pBlock = NULL;	// �u���b�N���
	// �u���b�N�������[�v
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK); nCntBlock++)
	{
		// ���擾
		pBlock = (CBaseblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);	// �u���b�N
		// �u���b�N���NULL�Ȃ�
		// ->���[�v�X�L�b�v
		if (pBlock == NULL) continue;
		// �s�񍂂������Ȃ�
		// ->�u���b�N����Ԃ�
		if (pBlock->GetGrid() == Grid) return pBlock;
	}
	// NULL��Ԃ�
	return NULL;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �w�肵���s��̑S�Ă̗������Ԃ�false�Ɋ֐�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::FallBlock_Grid(GRID & Grid)
{
	// �����J�E���g
	for (int nCntHeight = 0; nCntHeight <= CBaseblock::GetHeight(Grid.nColumn, Grid.nLine); nCntHeight++)
	{
		// �ϐ��錾
		GRID HeightGrid = Grid + GRID(0, nCntHeight, 0);	// �����𑫂����s��
		HeightGrid.nHeight++;
		CBaseblock * pBlock = NULL;							// �u���b�N���
		// �s�񍂂��x�[�X�u���b�N�̏��擾
		pBlock = CBaseblock::GetBaseBlock(HeightGrid);
		// �u���b�N���NULL�Ȃ�
		// ->���[�v�X�L�b�v
		if (pBlock == NULL) continue;
		// �������Ԃ�false��
		pBlock->SetFall(true);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�̒l�擾
//	nStage	: �X�e�[�W�ԍ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CBaseblock::GetFeedValue(
	int const & nStage	// �t�F�[�h�̒l
)
{
	// �w�肵���X�e�[�W�����ǂݍ��܂ꂽ�X�e�[�W���ȏ�̏ꍇ
	// ->-1��Ԃ�
	if (nStage >= (signed)m_nFeedValue.size())
	{
		CCalculation::Messanger("CBaseblock::GetFeedValue�֐�->�X�e�[�W�����ǂݍ��܂ꂽ�X�e�[�W�����������B");
		return -1;
	}
	return m_nFeedValue[nStage];
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���ݐςݏd�Ȃ��Ă���u���b�N�̍������擾
//	nColumn	: ��
//	nLine	: �s
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CBaseblock::GetHeight(
	int const & nColumn,
	int const & nLine
)
{
	// �s��0�����Ȃ�
	// ->�֐��𔲂���
	if (nColumn + m_nFeedValue[CGame::GetStage()] < 0 || nLine + m_nFeedValue[CGame::GetStage()] < 0)
	{
		//CCalculation::Messanger("CBaseblock::GetHeight�֐�->�s�񂪏�������������Ă���");
		return -1;
	}
	return m_anHeight[nColumn + m_nFeedValue[CGame::GetStage()]][nLine + m_nFeedValue[CGame::GetStage()]];
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���ݐςݏd�Ȃ��Ă���u���b�N�̍�����ݒ�
//	nColumn	: ��
//	nLine	: �s
//	nHeight	: ����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetHeight(
	int const & nColumn,
	int const & nLine,
	int const & nHeight
)
{
	// �s��0�����Ȃ�
	// ->�֐��𔲂���
	if (nColumn + m_nFeedValue[CGame::GetStage()] < 0 || nLine + m_nFeedValue[CGame::GetStage()] < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight�֐�->�s�񂪏�������������Ă���");
		return;
	}
	m_anHeight[nColumn + m_nFeedValue[CGame::GetStage()]][nLine + m_nFeedValue[CGame::GetStage()]] = nHeight;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���ݐςݏd�Ȃ��Ă���u���b�N�̍�����ݒ�
//	Grid	: ��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetHeight(
	GRID const & Grid
)
{
	// �s��0�����Ȃ�
	// ->�֐��𔲂���
	if (Grid.nColumn + m_nFeedValue[CGame::GetStage()] < 0 ||
		Grid.nLine + m_nFeedValue[CGame::GetStage()] < 0
		)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight�֐�->�s�񂪏�������������Ă���");
		return;
	}
	else if (m_anHeight[Grid.nColumn + m_nFeedValue[CGame::GetStage()]][Grid.nLine + m_nFeedValue[CGame::GetStage()]] <= Grid.nHeight) return;
	m_anHeight[Grid.nColumn + m_nFeedValue[CGame::GetStage()]][Grid.nLine + m_nFeedValue[CGame::GetStage()]] = Grid.nHeight;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̃X�e�[�^�X�̃��[�h
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusLoad(void)
{
	// �t�@�C���|�C���g
	FILE *pFile = nullptr;

	char cReadText[128];			// �����Ƃ��ēǂݎ��
	char cHeadText[128];			// ��r�p
	char cDie[128];					// �s�v�ȕ���
	int nCntDropSprit = 0;			// ���Ƃ��u���b�N���J�E���g
	int nCntBlockGravity = 0;		// �d�̓J�E���g

									// �t�@�C�����J��
	pFile = fopen(BLOCK_STATUS_TXT, "r");

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

						// AppBlock��������
						if (strcmp(cHeadText, "AppBlock") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nAppBlock);
						}
						// Move��������
						else if (strcmp(cHeadText, "Move") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_BlockStatus.fMove);
						}
						// Jump��������
						else if (strcmp(cHeadText, "App") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nAppearance);
						}
						// MaxSprit��������
						else if (strcmp(cHeadText, "MaxSprit") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nMaxSprit);

							// ���������z��m�ۂ���
							std::vector<INTEGER2> v_nSprit(m_BlockStatus.nMaxSprit);
							for (size_t CntSprit = 0; CntSprit < v_nSprit.size(); CntSprit++)
							{
								v_nSprit[CntSprit] = INTEGER2(0, 0);
							}
							m_BlockStatus.v_nDropBlock = v_nSprit;
							m_BlockStatus.v_nBlockGravity = v_nSprit;

						}
						// ChangeTime��������
						else if (strcmp(cHeadText, "ChangeTime") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nChangeTime);
						}
						// DropBlock��������
						else if (strcmp(cHeadText, "DropBlock") == 0)
						{
							if (nCntDropSprit >= (int)m_BlockStatus.v_nDropBlock.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d %d",
								&cDie, &cDie,
								&m_BlockStatus.v_nDropBlock[nCntDropSprit].nMax,
								&m_BlockStatus.v_nDropBlock[nCntDropSprit].nMin);
							nCntDropSprit++;
						}
						// BlockGravity��������
						else if (strcmp(cHeadText, "BlockGravity") == 0)
						{
							if (nCntBlockGravity >= (int)m_BlockStatus.v_nBlockGravity.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d %d",
								&cDie, &cDie,
								&m_BlockStatus.v_nBlockGravity[nCntBlockGravity].nMax,
								&m_BlockStatus.v_nBlockGravity[nCntBlockGravity].nMin);
							nCntBlockGravity++;
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
		MessageBox(NULL, "�u���b�N�̃X�e�[�^�X�ǂݍ��ݎ��s", BLOCK_STATUS_TXT, MB_ICONWARNING);
	}


	// ��{�V���h�E�T�C�Y�̐ݒ�
	m_BlockStatus.fBasicShadowSize = 1.0f / (m_fSizeRange * m_BlockStatus.nAppearance);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̃X�e�[�^�X�̃Z�[�u
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusSave(void)
{
	// �t�@�C���|�C���g
	FILE	*pFile = nullptr;
	int nCntDropSprit = 0;			// ���Ƃ��u���b�N���J�E���g
	int nCntBlockGravity = 0;		// �d�̓J�E���g

	// �t�@�C�����J��
	pFile = fopen(BLOCK_STATUS_TXT, "w");

	// �J���Ă���Ƃ�
	if (pFile)
	{
		fprintf(pFile, COMMENT02);
		fprintf(pFile, "// �u���b�N�̃X�e�[�^�X\n");
		fprintf(pFile, COMMENT02);

		fprintf(pFile, "SCRIPT\n");
		fprintf(pFile, NEWLINE);

		// �Z�[�u���郂�f���̏��
		fprintf(pFile, "STATUS_SET\n");
		fprintf(pFile, "	AppBlock		= %d\n", m_BlockStatus.nAppBlock);
		fprintf(pFile, "	Move			= %.1f\n", m_BlockStatus.fMove);
		fprintf(pFile, "	App				= %d\n", m_BlockStatus.nAppearance);
		fprintf(pFile, "	MaxSprit		= %d\n", m_BlockStatus.nMaxSprit);
		fprintf(pFile, "	ChangeTime		= %d\n", m_BlockStatus.nChangeTime);
		for (size_t nCntDropSprit = 0; nCntDropSprit < m_BlockStatus.v_nDropBlock.size(); nCntDropSprit++)
		{
			fprintf(pFile, "	Phase %zd\n",nCntDropSprit);

			fprintf(pFile, "		DropBlock		= %d %d\n",
				m_BlockStatus.v_nDropBlock[nCntDropSprit].nMax,
				m_BlockStatus.v_nDropBlock[nCntDropSprit].nMin
			);
		}
		for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_nBlockGravity.size(); nCntBlockGravity++)
		{
			fprintf(pFile, "	Phase %zd\n", nCntBlockGravity);

			fprintf(pFile, "		BlockGravity	= %d %d\n",
				m_BlockStatus.v_nBlockGravity[nCntBlockGravity].nMax,
				m_BlockStatus.v_nBlockGravity[nCntBlockGravity].nMin
			);
		}
		fprintf(pFile, "END_STATUS_SET\n\n");

		fprintf(pFile, "END_SCRIPT\n");

		// �t�@�C�������
		fclose(pFile);

		// �ǂݍ��ݐ������̌��ʕ\��
		MessageBox(NULL, "�Z�[�u���܂���", BLOCK_STATUS_TXT, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// �ǂݍ��ݎ��s���̌x���\��
		MessageBox(NULL, "�ǂݍ��ݎ��s", BLOCK_STATUS_TXT, MB_ICONWARNING);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̐ÓI�ϐ�������������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStaticValue(void)
{
	// �ϐ��錾
	int * nHeight = &m_anHeight[0][0];								// �`���̃A�h���X���擾
	int nCntMax = sizeof(m_anHeight) / sizeof(m_anHeight[0][0]);	// �z��̌�
	for (int nCntHeight = 0; nCntHeight < nCntMax; nCntHeight++,nHeight++)
	{
		*nHeight = -1;
	}

	m_nPhase = 0;
}

#ifdef _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S�Ẵf�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::AllDebug(void)
{
	if (ImGui::Begin(u8"�u���b�N�̃X�e�[�^�X"))
	{
		// �o�����鎞��
		ImGui::DragInt(u8"�o�����鎞��", &m_BlockStatus.nAppBlock, 1.0f,1,1000);					/* 10.0f */
		// �ړ���
		ImGui::DragFloat(u8"�ړ���", &m_BlockStatus.fMove, 0.1f, 0.1f, 100.0f);
		// �����鍂��
		ImGui::DragInt(u8"�����鍂��", &m_BlockStatus.nAppearance, 1.0f);						/* 10.0f */
		// �ő啪����
		if (ImGui::DragInt(u8"�ω������", &m_BlockStatus.nMaxSprit, 1.0f,1,100))						/* 10.0f */
		{
			// �������ɍ��킹�A���l���X�V
			std::vector<INTEGER2> v_Sprit(m_BlockStatus.nMaxSprit);
			// ������
			for (int nCntSprit = 0; nCntSprit < m_BlockStatus.nMaxSprit; nCntSprit++)
			{
				v_Sprit[nCntSprit] = INTEGER2(0, 0);
			}

			// ������u���b�N��
			for (size_t nCntDropSprit = 0; nCntDropSprit < m_BlockStatus.v_nDropBlock.size(); nCntDropSprit++)
			{
				if (nCntDropSprit >= (size_t)m_BlockStatus.nMaxSprit) break;
				v_Sprit[nCntDropSprit] = m_BlockStatus.v_nDropBlock[nCntDropSprit];
			}
			m_BlockStatus.v_nDropBlock.clear();
			m_BlockStatus.v_nDropBlock.shrink_to_fit();
			m_BlockStatus.v_nDropBlock = v_Sprit;
			// �����鑬�x
			for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_nBlockGravity.size(); nCntBlockGravity++)
			{
				if (nCntBlockGravity >= (size_t)m_BlockStatus.nMaxSprit) break;
				v_Sprit[nCntBlockGravity] = m_BlockStatus.v_nBlockGravity[nCntBlockGravity];
			}
			m_BlockStatus.v_nBlockGravity.clear();
			m_BlockStatus.v_nBlockGravity.shrink_to_fit();
			m_BlockStatus.v_nBlockGravity = v_Sprit;
		}
		// �ω������鎞��
		ImGui::DragInt(u8"�ω������鎞��", &m_BlockStatus.nChangeTime, 1.0f);					/* 10.0f */
		// ������u���b�N��
		if (ImGui::TreeNode(u8"������u���b�N��(�ő�l�A�ŏ��l)"))
		{
			for (size_t nCntDropSprit = 0; nCntDropSprit < m_BlockStatus.v_nDropBlock.size(); nCntDropSprit++)
			{
				ImGui::Text("Phase %d", nCntDropSprit);
				std::string sName = " (";
				sName += std::to_string(nCntDropSprit * m_BlockStatus.nChangeTime) + u8"�b)";
				// ���Ƃ��u���b�N�̐�
				ImGui::DragInt2(sName.c_str(), (int *)m_BlockStatus.v_nDropBlock[nCntDropSprit], 1.0f);					/* 10.0f */
			}
			ImGui::TreePop();
		}
		// �����鑬�x
		if (ImGui::TreeNode(u8"�����鑬�x(�ő�l�A�ŏ��l)"))
		{
			for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_nBlockGravity.size(); nCntBlockGravity++)
			{
				ImGui::Text("Phase %d", nCntBlockGravity);
				std::string sName = " (";
				sName += std::to_string(nCntBlockGravity * m_BlockStatus.nChangeTime) + u8"�b)";
				// �����鑬�x
				ImGui::DragInt2(sName.c_str(), (int *)m_BlockStatus.v_nBlockGravity[nCntBlockGravity], 1.0f);					/* 10.0f */
			}
			ImGui::TreePop();
		}

		// �Z�[�u�{�^��
		if (ImGui::Button(u8"�ۑ�"))
		{
			// �u���b�N�̃X�e�[�^�X�̃Z�[�u
			BlockStatusSave();
		}
	}
	// End
	ImGui::End();
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Debug(void)
{
	CDebugproc::Print("pos(%.2f,%.2f,%.2f)\n", CScene_X::GetMatrix()._41, CScene_X::GetMatrix()._42, CScene_X::GetMatrix()._43);
}
#endif // _DEBUG