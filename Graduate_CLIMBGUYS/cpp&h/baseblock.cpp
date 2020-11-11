// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �x�[�X�u���b�N���� [baseblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"
#include "collision.h"
#include "debugproc.h"
#include "stencilshadow.h"
#include "game.h"
#include "circleshadow.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define BLOCK_GRAVITY			(1.0f)									// �u���b�N�ɂ�����d��
#define STENCIL_SIZE_Y			(200.0f)								// �X�e���V���o�b�t�@��y�̃T�C�Y
#define BLOCK_SIZE_RANGE		(25.0f)									// �u���b�N�̃T�C�Y�͈̔�
#define BLOCK_SHADOWSIZE		(24.0f)									// �X�e���V���V���h�E�̃T�C�Y
#define BLOCK_STATUS_TXT		("data/LOAD/STATUS/BlockStatus.txt")	// �u���b�N�̃X�e�[�^�X�̃e�L�X�g

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����p
CBaseblock::BLOCK_STATUS CBaseblock::m_BlockStatus = {};
int CBaseblock::m_anHeight[20][20] = {};		// 1��1�̍s��̍���

float	CBaseblock::m_fSizeRange = 0.0f;		// �T�C�Y�͈�
std::vector<int>	CBaseblock::m_nFeedValue;	// �t�F�[�h�̒l

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::CBaseblock() : CScene_X::CScene_X()
{
	m_pShadowPolygon = NULL;
	m_type = TYPE_NORMAL;	// �^�C�v
	m_bFall = false;		// ��������
	CScene::SetObj(CScene::OBJ::OBJ_BLOCK);	// �I�u�W�F�N�g�^�C�v�̐ݒ�
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
	if (m_type != TYPE_FIELD)
	{
		// �V���h�E�|���S���̐���
		m_pShadowPolygon = CCircleshadow::Create(CScene_X::GetPos(),
			D3DXVECTOR3(m_BlockStatus.fBasicShadowSize, 0.0f, m_BlockStatus.fBasicShadowSize),
			-1
		);
	}
	/*
	if (m_type == TYPE_FIELD)
	{
		// �����蔻��̐ݒ�
		CScene_X::SetCollision(CShape::SHAPETYPE_RECT, CCollision::OBJTYPE_BLOCK, false, true, NULL, D3DXVECTOR3(0.0, 50.0f, 0.0));
	}
	else
	{
		// �����蔻��̐ݒ�
		CScene_X::SetCollision(CShape::SHAPETYPE_RECT, CCollision::OBJTYPE_BLOCK, false, true, NULL, D3DXVECTOR3(0.0, 50.0f, 0.0));
	}
	*/
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
	// �V���h�E�|���S���̈ʒu�ݒ�
	if (m_pShadowPolygon)
	{
		// �ϐ��錾
		D3DXVECTOR3 ShadowPos;	// �V���h�E�ʒu
		ShadowPos = m_pShadowPolygon->GetPos();
		ShadowPos.x = CScene_X::GetPos().x;
		ShadowPos.z = CScene_X::GetPos().z;
		m_pShadowPolygon->SetPos(ShadowPos);
	}
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
		else if (pBlock->m_grid.nColumn != this->m_grid.nColumn ||
			pBlock->m_grid.nLine != this->m_grid.nLine) continue;
		// �����蔻�菈��
		Collision(pBlock);
		// �u���b�N�̃X�e���V���V���h�E�̍X�V
		Update_StencilShadow(pBlock);
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
	pos.y -= BLOCK_GRAVITY;
	// �ʒu�ݒ�
	CScene_X::SetPos(pos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻�菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Collision(CBaseblock * pBlock)
{
	// �����Ă����ԂȂ�
	// ->�֐��𔲂���
	if (!this->GetFall()) return;
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
	// �u���b�N���������Ă���Ȃ�
	if (Direct != COLLISIONDIRECTION::NONE)
	{
		// ������������
		HitCollision(Direct, CScene::OBJ_BLOCK, pBlock);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�e���V���V���h�E�̍X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_StencilShadow(CBaseblock * pBlock)
{
	// �V���h�E�̃|���S����NULL�`�F�b�N
	if (!m_pShadowPolygon) return;
	// �t�B�[���h�u���b�N�Ȃ�
	// ->�֐��𔲂���
	else if (m_type == TYPE_FIELD) return;
	// �����Ă����ԂȂ�
	// ->�X�e���V���V���h�E�̎g�p��Ԃ�false��
	else if (!this->GetFall())
	{
		m_pShadowPolygon->SetShadow(false);
		return;
	}
	// ���Ƀu���b�N������Ȃ�
	// ->�X�e���V���V���h�E�̎g�p��Ԃ�false��
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
			// �T�C�Y�ύX
			//CScene_X::GetStencillShadow()->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize, fDiffHeight, m_BlockStatus.fBasicShadowSize));
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize, 0.0f, m_BlockStatus.fBasicShadowSize));
			m_pShadowPolygon->Set_Vtx_Pos(CScene_THREE::OFFSET_TYPE_SIDE_CENTER);
		}
		// ����ȊO
		else
		{
			float fSize = m_fSizeRange * m_BlockStatus.nAppearance - fDiffHeight;
			// �T�C�Y�ύX
			//CScene_X::GetStencillShadow()->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize * fSize, fDiffHeight , m_BlockStatus.fBasicShadowSize * fSize));
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize * fSize, 0.0f, m_BlockStatus.fBasicShadowSize * fSize));
			m_pShadowPolygon->Set_Vtx_Pos(CScene_THREE::OFFSET_TYPE_SIDE_CENTER);
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
	if (this->GetGrid().nHeight >= CBaseblock::GetHeight(this->GetGrid().nColumn + m_nFeedValue[Stage], this->GetGrid().nLine + m_nFeedValue[Stage]))
	{
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
					pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;

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
					// �ړ��ʂ̏�����
					move->y = 0.0f;
					// �����o����Ԃ�true
					bPush = true;

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
				// �ړ��ʂ̏�����
				move->x = 0.0f;
				// �����o����Ԃ�true
				bPush = true;
			}
			// �����蔻��(��)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
			}
			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + m_fSizeRange * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
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

				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}
			// �����蔻��(��O)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
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
	// �d�l
	// �v���C���[�ƕʂ̃I�u�W�F�N�g�ɓ����蔻��
	if (this->m_type == TYPE_FIELD) return COLLISIONDIRECTION::NONE;
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
	// ��邱��
	// �E�A���A��O�̔�������
	// ���ʂǂ��Ȃ邩����


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
	for (int nCntHeight = 0; nCntHeight <= CBaseblock::GetHeight(Grid.nColumn + m_nFeedValue[CGame::GetStage()], Grid.nLine + m_nFeedValue[CGame::GetStage()]); nCntHeight++)
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
	if (nColumn < 0 || nLine < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight�֐�->�s�񂪏�������������Ă���");
		return 0;
	}
	return m_anHeight[nColumn][nLine];
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
	if (nColumn < 0 || nLine < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight�֐�->�s�񂪏�������������Ă���");
		return;
	}
	m_anHeight[nColumn][nLine] = nHeight;
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
	if (Grid.nColumn < 0 || Grid.nLine < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight�֐�->�s�񂪏�������������Ă���");
		return;
	}
	m_anHeight[Grid.nColumn][Grid.nLine] = Grid.nHeight;
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

						// Move��������
						if (strcmp(cHeadText, "Move") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_BlockStatus.fMove);
						}
						// Jump��������
						else if (strcmp(cHeadText, "App") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nAppearance);
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
	m_BlockStatus.fBasicShadowSize = BLOCK_SHADOWSIZE / (m_fSizeRange * m_BlockStatus.nAppearance);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �u���b�N�̃X�e�[�^�X�̃Z�[�u
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusSave(void)
{
	// �t�@�C���|�C���g
	FILE	*pFile = nullptr;

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
		fprintf(pFile, "	Move			= %.1f\n", m_BlockStatus.fMove);
		fprintf(pFile, "	App				= %d\n", m_BlockStatus.nAppearance);
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

#ifdef _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S�Ẵf�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::AllDebug(void)
{
	if (ImGui::Begin(u8"�u���b�N�̃X�e�[�^�X"))
	{
		// �ړ���
		ImGui::DragFloat(u8"�ړ���", &m_BlockStatus.fMove, 0.1f, 0.1f, 100.0f);						/* 3.0f */
		// �����鍂��
		ImGui::DragInt(u8"�����鍂��", &m_BlockStatus.nAppearance, 1.0f);						/* 10.0f */

		// �Z�[�u�{�^��
		if (ImGui::Button(u8"�ۑ�"))
		{
			// �u���b�N�̃X�e�[�^�X�̃Z�[�u
			BlockStatusSave();
		}

		// ���s�L�����Z��
		ImGui::SameLine();

		// ������
		if (ImGui::Button(u8"������"))
		{
			// �u���b�N�̏����X�e�[�^�X���
			//BlockStatusInitLoad();
		}
		// ��؂��
		ImGui::Separator();
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