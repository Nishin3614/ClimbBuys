// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���ꏈ�� [stand.cpp]
// Author : FujiwaraMasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "stand.h"
#include "rectcollision.h"
#include "signboard.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define STAND_TARANSITION_SIZE	(200.0f)	// �J�ڔ͈͂̃T�C�Y

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CStand::m_bDetermination = false;
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand::CStand() : CScene_X::CScene_X()
{
	m_type = TYPE_BLOCK_NORMAL;	// �^�C�v
	m_bFall = false;		// ��������
	CScene::SetObj(CScene::OBJ::OBJ_STAND);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand::~CStand()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Init()
{
	CScene_X::Init();
	// �����蔻��̐ݒ�
	//CScene_X::SetCollision(CShape::SHAPETYPE_RECT, CCollision::OBJTYPE_BLOCK, false, true, NULL, D3DXVECTOR3(0.0, 50.0f, 0.0));
	//CRectCollision::Create(
	//	CScene_X::GetSize(),
	//	D3DXVECTOR3(0.0f,1000.0f,0.0f),
	//	CCollision::OBJTYPE_BLOCK,
	//	this,
	//	NULL,
	//	false,
	//	false
	//);
}	//

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Update(void)
{
	CScene_X::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����o�������蔻��
//	pos		: �ʒu
//	posOld	: �O��̈ʒu
//	move	: �ړ���
//	size	: �T�C�Y
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CStand::PushCollision(
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

	// �ϐ��錾
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	D3DXVECTOR3 BlockSize = CScene_X::GetModel()->size;
	// �f�ނ�Z�͈�
	if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + BlockSize.z * 0.5f)
	{
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + BlockSize.x * 0.5f)
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
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + BlockSize.y&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= BlockPos.y + BlockSize.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;

				// �f�ޏ�̍���
				pos->y = BlockPos.y + BlockSize.y + size->y * 0.5f - OffsetPos.y;

				// �ړ��ʂ̏�����
				move->y = 0.0f;
			}
			// �����蔻��(��)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
				pos->y + OffsetPos.y <= BlockPos.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + BlockSize.y&&
				pos->y + OffsetPos.y - size->y > BlockPos.y + BlockSize.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// �f�ނ�Y�͈�
	if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
		pos->y + OffsetPos.y - size->y * 0.5f <= BlockPos.y + BlockSize.y)
	{
		// �f�ނ�Z�͈�
		if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + BlockSize.z * 0.5f)
		{
			// �����蔻��(��)
			if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - BlockSize.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;

				// �f�ޏ�̍���
				pos->x = BlockPos.x - BlockSize.x * 0.5f - size->x * 0.5f - OffsetPos.x;

				// �ړ��ʂ̏�����
				move->x = 0.0f;

			}

			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + BlockSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + BlockSize.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;

				// �f�ޏ�̍���
				pos->x = BlockPos.x + BlockSize.x * 0.5f + size->x * 0.5f - OffsetPos.x;

				// �ړ��ʂ̏�����
				move->x = 0.0f;
			}
			// �����蔻��(��)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - BlockSize.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + BlockSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + BlockSize.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}

		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + BlockSize.x * 0.5f)
		{
			// �����蔻��(��O)
			if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - BlockSize.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;

				// �f�ޏ�̍���
				pos->z = BlockPos.z - BlockSize.z * 0.5f - size->z * 0.5f - OffsetPos.z;

				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}

			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + BlockSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + BlockSize.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;

				// �f�ޏ�̍���
				pos->z =
					BlockPos.z + BlockSize.z * 0.5f +
					size->z * 0.5f + 0.1f - OffsetPos.z;

				// �ړ��ʂ̏�����
				move->z = 0.0f;
			}
			// �����蔻��(��O)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - BlockSize.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;
			}
			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + BlockSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + BlockSize.z * 0.5f)
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
COLLISIONDIRECTION CStand::Collision(
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
	D3DXVECTOR3 BlockSize = CScene_X::GetModel()->size;

	// �f�ނ�Z�͈�
	if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + BlockSize.z * 0.5f)
	{
		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + BlockSize.x * 0.5f)
		{
			// �����蔻��(��)
			if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
				pos->y + OffsetPos.y >= BlockPos.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// �����蔻��(��)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + BlockSize.y&&
				pos->y + OffsetPos.y - size->y < BlockPos.y + BlockSize.y)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// �f�ނ�Y�͈�
	if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
		pos->y + OffsetPos.y - size->y * 0.5f <= BlockPos.y + BlockSize.y)
	{
		// �f�ނ�Z�͈�
		if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + BlockSize.z * 0.5f)
		{
			// �����蔻��(��)
			if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - BlockSize.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// �����蔻��(�E)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + BlockSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + BlockSize.x * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}

		// �f�ނ�X�͈�
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - BlockSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + BlockSize.x * 0.5f)
		{
			// �����蔻��(��O)
			if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - BlockSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - BlockSize.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::BACK;
			}
			// �����蔻��(��)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + BlockSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + BlockSize.z * 0.5f)
			{
				// �߂荞��ł���
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}




	/// ��邱��
	// �V�[�������ǂ�����Ď����Ă�����
	// �q�b�g��̏�Ԃ͂ǂ��Ȃ��Ă��邩
	// �������x�͂ǂ��Ȃ��Ă���̂�
	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �x�[�X�u���b�N�S�\�[�X�̓ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CStand::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�V�[���Ǘ�)
//	pos			: �ʒu
//	nModelId	: ���f���ԍ�
//	layer		: ���C���[
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand * CStand::Create(
	D3DXVECTOR3		const & pos,		// �ʒu
	int				const & nModelId,	// ���f���ԍ�
	CScene::LAYER	const & layer		// ���C���[
)
{
	// �ϐ��錾
	CStand * pStand;				// �V�[��2D�N���X
									// �������̐���(����->��{�N���X,��->�h���N���X)
	pStand = new CStand();
	// �ݒ�
	// �V�[���Ǘ��ݒ�
	pStand->ManageSetting(layer);
	pStand->SetPos(pos);			// �ʒu
	pStand->SetModelId(nModelId);	// �ʒu
	// ����������
	pStand->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pStand;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�l�Ǘ�)
//	pos			: �ʒu
//	nModelId	: ���f���ԍ�
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand * CStand::Create_Self(
	D3DXVECTOR3		const & pos,									// �ʒu
	int				const & nModelId								// ���f���ԍ�
)
{
	// �ϐ��錾
	CStand * pStand;		// �V�[��2D�N���X
									// �������̐���(����->��{�N���X,��->�h���N���X)
	pStand = new CStand;
	// �ݒ�
	pStand->SetPos(pos);			// �ʒu
	pStand->SetModelId(nModelId);	// �ʒu
										// ����������
	pStand->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pStand;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr�쐬(�l�Ǘ�unique)
// ���߂�l��std::move�Ŏ󂯎��
//	pos			: �ʒu
//	nModelId	: ���f���ԍ�
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CStand> CStand::Creat_Unique(
	D3DXVECTOR3		const & pos,									// �ʒu
	int				const & nModelId								// ���f���ԍ�
)
{
	// �ϐ��錾
	std::unique_ptr<CStand> pStand(new CStand);		// �V�[��2D�N���X
																// �ݒ�
	pStand->SetPos(pos);			// �ʒu
	pStand->SetModelId(nModelId);	// �ʒu
										// ����������
	pStand->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pStand;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`���[�g���A���p���ꐶ��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::CreateStand_Tutorial()
{
	static D3DXVECTOR3 StandPos[3] = {};

	// ����
	StandPos[0] = D3DXVECTOR3(-400, -50, 0);
	StandPos[1] = D3DXVECTOR3(0, 0, 0);
	StandPos[2] = D3DXVECTOR3(400, 50, 0);

	// �`���[�g���A���Ŏg�p���鑫��u���b�N�̐�
	for (int nBlockCnt = 0; nBlockCnt < 3; nBlockCnt++)
	{
		// �ϐ��錾
		CStand * pStand;		// �V�[��2D�N���X
		// �������̐���(����->��{�N���X,��->�h���N���X)
		pStand = new CStand();
		// �ݒ�
		// �V�[���Ǘ��ݒ�
		pStand->ManageSetting(LAYER_3DBLOCK);				// ���C���[�̔ԍ�
		pStand->SetModelId(3);								// ���f����id
		pStand->SetPos(StandPos[nBlockCnt]);				// �ʒu
		pStand->SetType(TYPE(TYPE_BLOCK_MAP1 + nBlockCnt));	// �^�C�v
		pStand->Init();										// ����������

		// �Ŕ�
		CSignboard::Create(D3DXVECTOR3(
			StandPos[nBlockCnt].x,
			StandPos[nBlockCnt].y + 150.0f,
			StandPos[nBlockCnt].z));
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����ɓ��Ă�ꂽ��̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
	// �V�[�����NULL�Ȃ�
	// ->�֐��𔲂���
	if (pScene == NULL) return;

	// �I�u�W�F�N�g�^�C�v���v���C���[�Ȃ�
	if (nObjType == CCollision::OBJTYPE_PLAYER)
	{
		// �ʏ�u���b�N�ȊO�Ȃ�
		if (m_type != TYPE_BLOCK_NORMAL)
		{
			CPlayer *pPlayer = (CPlayer *)pScene;
			// �v���C���[�^�O���v���C���[1�Ȃ�
			if (pPlayer->GetPlayerTag() == PLAYER_TAG::PLAYER_1)
			{
				m_bDetermination = true;
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����ɓ��Ă��Ȃ�������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene)
{
	// �I�u�W�F�N�g�^�C�v���v���C���[�Ȃ�
	if (nObjType == CCollision::OBJTYPE_PLAYER)
	{
		CPlayer *pPlayer = (CPlayer *)pScene;
		// �v���C���[�^�O���v���C���[1�Ȃ�
		if (pPlayer->GetPlayerTag() == PLAYER_TAG::PLAYER_1)
		{
			m_bDetermination = false;
		}
	}
}


#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Debug(void)
{

}
#endif // _DEBUG

