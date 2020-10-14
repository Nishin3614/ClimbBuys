// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �L�����N�^�[���� [character.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"
#include "floor.h"
#include "input.h"
#include "collision.h"
#include "3Dparticle.h"
#include "camera.h"
#include "meshobit.h"
#include "ui.h"

#include "rectcollision.h"
#include "spherecollision.h"
#include "columncollision.h"
#include "circleshadow.h"
#include "stencilshadow.h"

#include "fade.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHARACTER_KEYMOVE (1)													// �L�[�ړ�
#define CHARACTER_G (0.5f)														// �d��
#define CHARACTER_RESISTANCE (0.5f)												// ��R��
#define CHARACTER_STATUS_FILE ("data/LOAD/STATUS/status_manager_Character.csv")	// �X�e�[�^�X�t�@�C����
#define CHARACTER_INFO_FILE ("data/LOAD/CHARACTER/CHARACTER_MANAGER.txt")		// �L�����N�^�[�t�@�C����
#define CIRCLESHADOW (true)														// �~�`�̃V���h�E�ɂ��邩���Ȃ���
#define REFLECTION_COEFFICIENT (1)												// �����W��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MODEL_ALL	*CCharacter::m_modelAll[CHARACTER_MAX] = {};		// �L�����N�^�[�S�̂̏��
CModel_info	*CCharacter::m_model_info[CHARACTER_MAX] = {};		// �L�����N�^�[���
std::vector<int>	CCharacter::m_modelId[CHARACTER_MAX];				// �L�����N�^�[�ԍ�
D3DXVECTOR3	CCharacter::m_CharacterSize[CHARACTER_MAX] = {};	// �L�����N�^�[�̃T�C�Y
CCharacter::STATUS CCharacter::m_sStatus[CHARACTER_MAX] = {};	// �L�����N�^�[���ׂẴX�^�[�^�X���
int			CCharacter::m_NumParts[CHARACTER_MAX] = {};			// �������L�����N�^�[��
int			CCharacter::m_NumModel[CHARACTER_MAX] = {};			// �ő�L�����N�^�[��
int			CCharacter::m_nCameraCharacter = 0;					// �L�����N�^�[�ɒǔ�����ID
int			CCharacter::m_nAllCharacter = 0;					// �o�����Ă���L�����N�^�[�l��
float		CCharacter::m_fAlpha = 0.0f;						// �����x

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�[�o�[���[�o�[�R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCharacter::CCharacter(CHARACTER const &character) : CScene::CScene()
{
	m_pMeshobit = NULL;								// �O��
	m_pModel = NULL;								// ���f��
	m_character = CHARACTER_NPC;				// �L�����N�^�[
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);			// �ʒu
	m_posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݉�]��
	m_rotLast = m_rot;								// ������������
	m_rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�̍���
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �L�����N�^�[�̃T�C�Y
	m_nMotiontypeOld = 0;							// �O��̃��[�V�����^�C�v
	m_nMotiontype = 0;								// ���[�V�����^�C�v
	m_keyinfoCnt = 0;								// �L�[���̃J�E���g
	m_nFrame = 0;									// �t���[���J�E���g
	m_nMotionFrame = 0;								// ���[�V������̂̃t���[���J�E���g
	m_nMaxMotion = 0;								// �ő僂�[�V������
	m_nCntDamage = 0;								// �_���[�W�J�E���g
	m_State = STATE_NORMAL;							// ����̃X�e�[�^�X
	m_nCntState = 0;								// �J�E���g�X�e�[�^�X
	m_fLength = 0;									// �U���̓�����͈�
	m_fAlpha = 1.0f;								// �A���t�@�l
	m_bMotionCamera = false;						// ���[�V�����J�����̐؂�ւ�
	m_bLanding = false;								// ���n���
	m_bMove = false;								// �ړ����
	m_Directvector = D3DVECTOR3_ONE;				// �����x�N�g��
	m_pStencilshadow = NULL;						// �X�e���V���V���h�E
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	m_character = character;						// �L�����N�^�[�ݒ�
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCharacter::~CCharacter()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Init()
{
	// �ߋ��̈ʒu���ۑ�
	m_posold = m_pos;
	// ���L�����N�^�[�J�E���g�A�b�v
	m_nAllCharacter++;
	// �L�����N�^�[�̃������m��
	m_pModel = new CModel[m_NumParts[m_character]];

	// �L�����N�^�[�̐ݒ�
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// �L�����N�^�[���n��
		m_pModel[nCntModel].Init();
		// �L�����N�^�[�̏���n��
		m_pModel[nCntModel].BindModel(
			m_modelAll[m_character]->pModel_offset[nCntModel]
		);
		// �L�����N�^�[�̔ԍ��ݒ�
		m_pModel[nCntModel].SetModelId(m_modelId[m_character][nCntModel]);
		// �V���h�Eon
		if (CIRCLESHADOW == false)
		{
			m_pModel[nCntModel].SetShadowMap(true);
		}
		// ���[�V�����ݒ�
		m_pModel[nCntModel].BeginMotion(
			m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].Key[nCntModel],
			m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].nFrame);
		// ���ׂĂ̐e�ȊO
		if (nCntModel != 0)
		{
			// �e���ݒ�
			m_pModel[nCntModel].SetParentMtx(
				&m_pModel[m_modelAll[m_character]->pModel_offset[nCntModel].nParent].GetMatrix()
			);
		}
		// ���ׂĂ̐e
		else
		{
			// �e���ݒ�
			m_pModel[nCntModel].SetParentMtx(
				&m_mtxWorld
			);
		}
	}

	// �O�Ղ̐ݒ�
	for (int nCntObit_Basic = 0; nCntObit_Basic < (signed)m_modelAll[m_character]->v_MeshObitLoad.size(); nCntObit_Basic++)
	{
		m_vec_pMeshObit.push_back(std::move(CMeshobit::Create_Self(
			m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).nLine,
			m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset,
			m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).EndOffset,
			(CMeshobit::TEX)m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).nTexType
		)));
	}

	// �U�������蔻��ݒ�
	for (int nCntAttackCollision = 0; nCntAttackCollision < (signed)m_modelAll[m_character]->v_AttackCollision.size(); nCntAttackCollision++)
	{
		// �ϐ��錾
		D3DXVECTOR3 pos;
		// �����蔻��̈ʒu�̐ݒ�
		D3DXVec3TransformCoord(
			&pos,
			&m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
			&m_pModel[m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).nParts].GetMatrix()
		);
		// ��`�̓����蔻��
		if (m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_RectInfo)
		{
			// ��`�̓����蔻��
			m_vec_AttackCollision.push_back(std::move(CRectCollision::Create_Self(
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_RectInfo->size,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
				CCollision::OBJTYPE_ATTACK
			)));
		}
		// ���̓����蔻��
		else if (m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_SphereInfo)
		{
			// ���̓����蔻��
			m_vec_AttackCollision.push_back(std::move(CSphereCollision::Create_Self(
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_SphereInfo->fRadius,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
				CCollision::OBJTYPE_ATTACK
			)));
		}
		// �~���̓����蔻��
		else if (m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_ColumnInfo)
		{
			// �~���̓����蔻��
			m_vec_AttackCollision.push_back(std::move(CColumnCollision::Create_Self(
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_ColumnInfo->fRadius,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_ColumnInfo->fVertical,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
				CCollision::OBJTYPE_ATTACK
			)));
		}
	}

	// �L�����N�^�[�����蔻��ݒ�
	if (m_modelAll[m_character]->pCharacterCollision != NULL)
	{
		// �ϐ��錾
		CCollision::OBJTYPE objtype = CCollision::OBJTYPE_PLAYER;	// �����蔻��̃I�u�W�F�N�g�^�C�v
		// �I�u�W�F�N�g�^�C�v��NPC�̎�
		if (m_character == CHARACTER_NPC)
		{
			objtype = CCollision::OBJTYPE_ENEMY;
		}
		// ����ȊO
		else
		{
			objtype = CCollision::OBJTYPE_PLAYER;
		}
		// ��`�̓����蔻��
		if (m_modelAll[m_character]->pCharacterCollision->RectInfo)
		{
			m_pCharacterCollision = CRectCollision::Create(
				m_modelAll[m_character]->pCharacterCollision->RectInfo->size,
				m_modelAll[m_character]->pCharacterCollision->Offset,
				objtype,
				this,
				NULL,
				true,
				true,
				&m_pos,
				&m_posold
			);
		}
		// ���̓����蔻��
		else if (m_modelAll[m_character]->pCharacterCollision->p_uni_SphereInfo)
		{
			m_pCharacterCollision = CSphereCollision::Create(
				m_modelAll[m_character]->pCharacterCollision->p_uni_SphereInfo->fRadius,
				m_modelAll[m_character]->pCharacterCollision->Offset,
				objtype,
				this,
				NULL,
				true,
				true,
				&m_pos,
				&m_posold
			);

		}
	}
	// �V���h�Eon
	if (CIRCLESHADOW == true)
	{
		D3DXVECTOR3 pos = m_pos;
		pos.y = 0;
		// �X�e���V���V���h�E�̐���
		m_pStencilshadow = CStencilshadow::Create(m_pos, D3DXVECTOR3(10.0f, 10000.0f, 10.0f));
	}
	// �s��̏���������
	D3DXMatrixIdentity(&m_mtxWorld);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Uninit(void)
{
	// ���f���k���`�F�b�N
	// ->���f���̊J��
	if (m_pModel != NULL)
	{
		delete[] m_pModel;
		m_pModel = NULL;
	}
	// �L�����N�^�[�����蔻��̃k���`�F�b�N
	// ->�J��
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->CompulsionScene();
		m_pCharacterCollision = NULL;
	}
	// �O�Ղ̏����J������
	for (int nCntMotionObit = 0; nCntMotionObit < (signed)m_vec_pMeshObit.size(); nCntMotionObit++)
	{
		// �O�Ղ̏I������
		m_vec_pMeshObit.at(nCntMotionObit)->Uninit();
	}
	// �X�e���V���V���h�E�̃k���`�F�b�N
	// �X�e���V���V���h�E�̏�����
	if (m_pStencilshadow != NULL)
	{
		m_pStencilshadow = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Update(void)
{
	// �ߋ��̈ʒu���ۑ�
	m_posold = m_pos;
	// �ʏ펞�̍X�V����
	Update_Normal();
	// �����蔻�菈��
	Collision();
	// �X�e���V���V���h�E�̈ʒu�ݒ�
	if (m_pStencilshadow != NULL)
	{
		// �ʒu�擾
		D3DXVECTOR3 pos = m_pos;
		pos.y = -1000.0f;
		// �X�e���V���V���h�E�̈ʒu�ݒ�
		m_pStencilshadow->SetPos(pos);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���ꂼ��̓����蔻�菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Collision(void)
{
	/*
	// �ϐ��錾
	CCollision * pCollision;
	int nCnt = CScene::GetMaxLayer(LAYER_COLLISION);
	// �����蔻�蔻��
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer++)
	{
		// �����蔻��̏��擾
		pCollision = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer);
		// �����̓����蔻��̏��NULL�Ȃ�
		// ->���[�v�X�L�b�v
		if (m_pCharacterCollision == NULL) continue;
		// �����蔻��������Ă��Ȃ��ꍇ
		// ->���[�v�X�L�b�v
		else if (pCollision == NULL) continue;
		// �L�����N�^�[�̓����蔻��Ǝ擾���������蔻�肪�����Ȃ�
		// ->���[�v�X�L�b�v
		else if (m_pCharacterCollision == pCollision)
		{
			continue;
		}
		// �L�����N�^�[�Ƃ����蔻�蓯�m�̓����蔻�菈��
		else if (m_pCharacterCollision->CollisionDetection(pCollision))
		{
		}
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʏ펞�̍X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Update_Normal(void)
{
	// �ړ�
	Move();
	// ����
	GetFloorHeight();
	// ���[�V����
	Motion();
	// �G�t�F�N�g���X�V
	Motion_Effect();
	// �L�����N�^�[�X�V
	ModelUpdate();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���̃��[�V�����ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::NextKeyMotion(void)
{
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// �k���`�F�b�N
		if (&m_pModel[nCntModel] != NULL)
		{
			// ���[�V�����ݒ�
			m_pModel[nCntModel].SetMotion(
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].Key[nCntModel],
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].nFrame);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ړ�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Move(void)
{
	// ���݉�]����
	m_rotbetween = m_rotLast - m_rot;

	// ���x����
	m_rotbetween.x = CCalculation::Rot_One_Limit(m_rotbetween.x);
	m_rotbetween.y = CCalculation::Rot_One_Limit(m_rotbetween.y);
	m_rotbetween.z = CCalculation::Rot_One_Limit(m_rotbetween.z);

	// ��]�ړ�
	m_rot += m_rotbetween * 0.1f;

	// ���x����
	m_rot.x = CCalculation::Rot_One_Limit(m_rot.x);
	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);
	m_rot.z = CCalculation::Rot_One_Limit(m_rot.z);

	// �ʒu���X�V
	m_pos += m_move;
	// ��R��
	m_move.x *= m_sStatus[m_character].fMaxInertia;
	m_move.z *= m_sStatus[m_character].fMaxInertia;
	// �������
	Limit();

	// �L�����N�^�[�̓����蔻�肪NULL�ł͂Ȃ��Ȃ�
	// ->�����蔻��̍X�V
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->GetShape()->PassPos(m_pos);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V��������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Motion(void)
{
	// �t���[���J�E���g�A�b�v
	m_nFrame++;
	m_nMotionFrame++;
	// ���[�V�����^�C�v���ω�
	// ������
	if (m_nMotiontype != m_nMotiontypeOld)
	{
		m_nFrame = 0;		// �t���[���L�[���̃J�E���g
		m_nMotionFrame = 0;	// ���[�V������̃t���[���J�E���g
		m_keyinfoCnt = 0;	// �L�[���̃J�E���g
		// ���̃��[�V�����ݒ�
		NextKeyMotion();
	}
	// ���[�V�����̕ۑ�
	m_nMotiontypeOld = m_nMotiontype;
	// �t���[�����������ɂȂ�����
	if (m_nFrame >= m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].nFrame)
	{
		// ������
		m_nFrame = 0;	// �t���[��
		// �J�E���g�A�b�v
		m_keyinfoCnt++;
		// ���[�V�����J�����̐؂�ւ��̏�����
		m_bMotionCamera = false;
		// �L�[��񂪒�������
		if (m_keyinfoCnt >= m_modelAll[m_character]->pMotion[m_nMotiontype]->nNumKey)
		{
			// ���[�v���Ȃ��Ƃ�
			if (m_modelAll[m_character]->pMotion[m_nMotiontype]->nLoop == 0)
			{
				m_nMotiontype = 0;	// ���[�V�����^�C�v
			}
			// ������
			m_keyinfoCnt = 0;				// �L�[���
			m_nMotionFrame = 0;				// ���[�V������̃t���[���J�E���g
		}
		// ���̃��[�V�����ݒ�
		NextKeyMotion();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �L�����N�^�[�̍X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::ModelUpdate(void)
{
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// �k���`�F�b�N
		if (&m_pModel[nCntModel] != NULL)
		{
			// �X�V
			m_pModel[nCntModel].Update();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����ǔ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::TrackCamera(void)
{
	//// ���[�h���I����ʂƃ`���[�g���A���̏ꍇ
	//if (CManager::GetMode() == CManager::MODE_SELECT)
	//{
	//	// �J�����̒����_�ݒ�
	//	CManager::GetRenderer()->GetCamera()->SetPosDestRPlayer(
	//		CCharacter::GetPos(),
	//		CCharacter::GetRot()
	//	);
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Limit(void)
{
	//// x�̐���
	//if (m_pos.x < -950)
	//{
	//	m_pos.x = -950;
	//	m_move.x = 0.0f;
	//}
	//if (m_pos.x > 950)
	//{
	//	m_pos.x = 950;
	//	m_move.x = 0.0f;
	//}
	//// z�̐���
	//if (m_pos.z < -950)
	//{
	//	m_pos.z = -950;
	//	m_move.z = 0.0f;
	//}
	//if (m_pos.z > 950)
	//{
	//	m_pos.z = 950;
	//	m_move.z = 0.0f;
	//}
	if (m_pos.y > 1000.0f)
	{
		m_pos.y = 1000.0f;
		m_move.y = 0.0f;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�����G�t�F�N�g�X�V
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Motion_Effect(void)
{
	// �ϐ��錾
	D3DXVECTOR3 pos;
	for (int nCntMotionEffect = 0; nCntMotionEffect < (signed)m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.size(); nCntMotionEffect++)
	{
		// �J�n���ԊO�Ȃ�X�L�b�v
		if (m_nFrame < m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nStart)
		{
			continue;
		}
		// �I�����ԊO�Ȃ�X�L�b�v
		else if (m_nFrame > m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nEnd)
		{
			continue;
		}
		// ���ԓ��Ȃ�
		// �ʒu�ݒ�
		D3DXVec3TransformCoord(
			&pos,
			&m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).offset,
			CCharacter::GetMatrix(m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nKeyID)
		);
		// �p�[�e�B�N���̐���
		C3DParticle::Create(
			(C3DParticle::PARTICLE_ID)m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nParticleType,
			pos);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�����O�ՍX�V
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Motion_Obit()
{
	for (int nCntMotionObit = 0; nCntMotionObit < (signed)m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.size(); nCntMotionObit++)
	{
		// �J�n���ԊO�Ȃ�X�L�b�v
		if (m_nMotionFrame < m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nBeginFram)
		{
			continue;
		}
		// �I�����ԊO�Ȃ�X�L�b�v
		else if (m_nMotionFrame > m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nEndFram)
		{
			continue;
		}
		// �J�n���Ɉʒu��������
		else if (m_nMotionFrame == m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nBeginFram)
		{
			m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->InitPos(*CCharacter::GetMatrix(m_modelAll[m_character]->v_MeshObitLoad.at(
				m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID).nPart));
		}
		// �O�Ղ̐F�̐ݒ�
		m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->SetCol(
			m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).BeginCol,
			m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).EndCol
		);
		// �O�Ղ̕`��ݒ�
		m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->DrawSet(*CCharacter::GetMatrix(m_modelAll[m_character]->v_MeshObitLoad.at(
			m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID).nPart));
		// �O�Ղ̕`��
		m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->Draw();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�����J�����̍X�V
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::MotionCamera(void)
{
	// �ϐ��錾
	// ���擾
	static CCamera * pCamera = CManager::GetRenderer()->GetCamera();	// �J����
	// ���[�V�����J�����̐؂�ւ���OFF�̏ꍇ
	if (!m_bMotionCamera)
	{
		// �v�f���������珈�����s��
		if (m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera != NULL)
		{
			// �J�������[�V�����ݒ�
			pCamera->SetCamera_Motion(
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->offsetR + m_pos,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->rot + m_rot,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->fLength,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->fHeight,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->fIntertia
			);
			// ���[�V�����J�����̐؂�ւ�
			m_bMotionCamera = true;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Draw(void)
{
	// �ϐ��錾
	D3DXMATRIX	mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	// �X�P�[���𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);


	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,	// �����̓��ꕨ
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3


	// �L�����N�^�[
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// �k���`�F�b�N
		if (&m_pModel[nCntModel] != NULL)
		{
			// ���f���̕`��
			m_pModel[nCntModel].Draw();

		}
	}

	// ���[�V�����O�ՍX�V
	Motion_Obit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���񂾂Ƃ��̏���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Die(void)
{
	// �J��
	Release();
	// �X�e���V���V���h�E��NULL�Ȃ�
	// ->�J������
	if (m_pStencilshadow != NULL)
	{
		m_pStencilshadow->Release();
		m_pStencilshadow = NULL;
	}
	// �p�[�e�B�N������
	C3DParticle::Create(
		C3DParticle::PARTICLE_ID_UNKNOWN,
		m_pos
	);

	// ���L�����N�^�[�J�E���g�_�E��
	m_nAllCharacter--;

	// �L�����N�^�[�����蔻��̃k���`�F�b�N
	// ->�J��
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->CompulsionScene();
		m_pCharacterCollision->Release();
		m_pCharacterCollision = NULL;
	}
	//// ���L�����N�^�[����l�����Ȃ�
	//// ->�^�C�g���փt�F�[�h
	//if (m_nAllCharacter <= 1)
	//{
	//	if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
	//	{
	//		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	//	}
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Scene_MyCollision(
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
void CCharacter::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�B�[���h�̍������Z�o
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCharacter::GetFloorHeight(void)
{
	// �ϐ��錾
	CFloor * pFloor = NULL;	// ��
							// ���擾
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		pFloor = (CFloor*)CScene::GetScene( CScene::LAYER_3DOBJECT, nCntLayer,CFloor());	// ��
		if (pFloor != NULL)
		{
			if (pFloor->GetCalculation())
			{
				// ���̍�������
				if (m_pos.y < pFloor->GetHeight(m_pos))
				{
					m_pos.y = pFloor->GetHeight(m_pos);
					m_move.y = 0;
					// ���n��Ԃ�true��
					m_bLanding = true;
					return true;
				}
				// ����ȊO
				else
				{
					// ���n��Ԃ�false��
					m_bLanding = false;
				}
			}
		}
	}
	return false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�����̃t���[�����擾����
// 1)�L�����N�^�[ID�A2)���[�V����ID�A3)���݂̃L�[�J�E���g
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCharacter::GetMaxFrame(
	CHARACTER character,	// �L�����N�^�[
	int nMotionID,			// ���[�V����ID
	int nNowKeyCnt			// ���݂̃L�[�J�E���g
)
{
	// ���[�V�����S�̂̃t���[����
	if (nNowKeyCnt == -1)
	{
		return m_modelAll[character]->pMotion[nMotionID]->nAllFrame;
	}
	// ��̃L�[�Ԃ̃t���[����
	return m_modelAll[character]->pMotion[nMotionID]->KeyInfo[nNowKeyCnt].nFrame;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����ǔ�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCharacter::GetCameraCharacter(void)
{
	return m_nCameraCharacter;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �d�͏���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::FagGravity(void)
{
	// �d�͏���
	//m_move.y -= 1.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�����ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::SetMotion(int const nMotiontype)
{
	// ���[�v��Ԃ̎�
	if (m_modelAll[m_character]->pMotion[m_nMotiontype]->nLoop == 1)
	{
		m_nMotiontype = nMotiontype;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �������[�V�����ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::ComplusionSetMotion(int const nMotiontype)
{
	m_nMotiontype = nMotiontype;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��]�擾����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 *CCharacter::GetPartsRot(int const nModelID)
{
	// �L�����N�^�[�̍s����擾
	if (nModelID >= 0 ||
		nModelID < m_NumParts[m_character])
	{
		return &m_pModel[nModelID].GetRot();
	}
	// �L�����N�^�[�̍s����擾
	else if (nModelID == -1)
	{
		return &m_rot;
	}
#ifdef _DEBUG
	CCalculation::Messanger("�L�����N�^�[�̃L�����N�^�[�̉�]��񂪂���܂���");
#endif // _DEBUG
	// �w�肳�ꂽID���Ȃ��ꍇ
	return &m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu�擾����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 *CCharacter::GetPartsPos(int const nModelID)
{
	// �L�����N�^�[�̍s����擾
	if (nModelID >= 0 ||
		nModelID < m_NumParts[m_character])
	{
		return &m_pModel[nModelID].GetPos();
	}
	// �L�����N�^�[�̍s����擾
	else if (nModelID == -1)
	{
		return &m_rot;
	}
#ifdef _DEBUG
	CCalculation::Messanger("�L�����N�^�[�̃L�����N�^�[�̉�]��񂪂���܂���");
#endif // _DEBUG
	// �w�肳�ꂽID���Ȃ��ꍇ
	return &m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �s��擾����
// -1�̓L�����N�^�[�̍s����A0�`�̓L�����N�^�[�̍s����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXMATRIX * CCharacter::GetMatrix(int const nModelID)
{
	// �L�����N�^�[�̍s����擾
	if(nModelID >= 0 ||
		nModelID < m_NumParts[m_character])
	{
		return &m_pModel[nModelID].GetMatrix();
	}
	// �L�����N�^�[�̍s����擾
	else if (nModelID == -1)
	{
		return &m_mtxWorld;
	}
#ifdef _DEBUG
	CCalculation::Messanger("�L�����N�^�[�̃L�����N�^�[�̍s���񂪂���܂���");
#endif // _DEBUG
	// �w�肳�ꂽID���Ȃ��ꍇ
	return &m_mtxWorld;
}

/*
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Load(
	CHARACTER const character,
	int const nMaxMotion,
	const char * file_name
)
{
	// �ϐ��錾
	// �L�����N�^�[�ƃ��[�V�������̐���
	m_modelAll[character] = new MODEL_ALL;
	// �L�����N�^�[�̃e�L�X�g�f�[�^�̎擾
	CModel_info::TextLoad(
		m_modelAll[character],					// �L�����N�^�[���
		m_modelId[character],					// �L�����N�^�[�t�@�C��
		nMaxMotion,								// ���[�V������
		m_NumModel[character],					// �ő�L�����N�^�[��
		m_NumParts[character],					// �������L�����N�^�[��
		file_name								// �t�@�C����
	);
}
*/

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �L�����N�^�[�S�\�[�X�̓ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCharacter::Load(void)
{
	// �ϐ��錾
	HRESULT hr;
	// �L�����N�^�[�̏��ǂݍ���
	hr = Load_Character();
	// �X�e�[�^�X�̏��ǂݍ���
	hr = LoadStatus();
	return hr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �L�����N�^�[�̏��ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCharacter::Load_Character(void)
{
	// �t�@�C���̒��g�i�[�p
	std::vector<std::vector<std::string>> vsvec_Contens;
	// �t�@�C���̒��g���擾����
	vsvec_Contens = CCalculation::FileContens(CHARACTER_INFO_FILE, ',');
	// �s���Ƃɉ�
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// ���ڂ��Ƃɉ�
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			switch (nCntItem)
			{
				// �p�X���
			case 0:
				// �L�����N�^�[�̍ő吔�𒴂�����
				// �P�[�X�𔲂���
				if (nCntLine >= CHARACTER_MAX)
				{
#ifdef _DEBUG
					CCalculation::Messanger("Character��Load_Character�֐�->�L�����N�^�[�̍ő吔�𒴂��܂����B");
#endif // _DEBUG
					break;
				}
				// �L�����N�^�[�ƃ��[�V�������̐���
				m_modelAll[nCntLine] = new MODEL_ALL;
				// �L�����N�^�[�̃e�L�X�g�f�[�^�̎擾
				CModel_info::TextLoad(
					m_modelAll[nCntLine],							// �L�����N�^�[���
					m_modelId[nCntLine],							// �L�����N�^�[�t�@�C��
					m_NumModel[nCntLine],							// �ő�L�����N�^�[��
					m_NumParts[nCntLine],							// �������L�����N�^�[��
					vsvec_Contens.at(nCntLine).at(nCntItem).c_str()	// �t�@�C����
				);
				break;
			default:
				break;
			}
		}
	}
	// std::vector�̑��d�z��J��
	std::vector<std::vector<std::string>>().swap(vsvec_Contens);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�e�[�^�X���ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCharacter::LoadStatus(void)
{
	// �ϐ��錾
	std::vector<std::vector<std::string>> vsvec_Contens;	// �t�@�C���̒��g�i�[�p
	// �t�@�C���̒��g���擾����
	vsvec_Contens = CCalculation::FileContens(CHARACTER_STATUS_FILE, ',');
	// �s���Ƃɉ�
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// �L�����N�^�[������𒴂��Ă����甲����
		if (nCntLine >= CHARACTER_MAX)
		{
			break;
		}
		// ���ڂ��Ƃɉ�
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			switch (nCntItem)
			{
				// ���D�̌�
			case 0:
				m_sStatus[nCntLine].nMaxBalloon = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// ���D�̌�
			case 1:
				m_sStatus[nCntLine].nMaxPopBalloon = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// ������
			case 2:
				m_sStatus[nCntLine].fMaxInertia = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �W�����v��
			case 3:
				m_sStatus[nCntLine].fMaxJump = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �ړ���
			case 4:
				m_sStatus[nCntLine].fMaxMove = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �ő�MP��
			case 5:
				m_sStatus[nCntLine].nMaxMp = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �}�C�t���[���ő�����MP��
			case 6:
				m_sStatus[nCntLine].nMaxMpUp_Every = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �|�����Ƃ���MP��
			case 7:
				m_sStatus[nCntLine].nMaxMpUp_KnockDown = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �X�L���g�p���̌����
			case 8:
				m_sStatus[nCntLine].nMaxMpDown = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// �\�͂̔{��
			case 9:
				m_sStatus[nCntLine].fMaxSkill = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;

			default:
				break;
			}
		}
	}
	// std::vector�̑��d�z��J��
	std::vector<std::vector<std::string>>().swap(vsvec_Contens);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::UnLoad(void)
{
	// �L�����N�^�[�E���[�V�����̔j��
	for (int nCntCharacter = 0; nCntCharacter < CHARACTER_MAX; nCntCharacter++)
	{
		CModel_info::TextUnload(m_modelAll[nCntCharacter]);
		m_modelId[nCntCharacter].clear();
		m_modelId[nCntCharacter].shrink_to_fit();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�ϐ��̏�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::InitStatic(void)
{
	m_nAllCharacter = 0;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Debug(void)
{
	// �L�����N�^�[�̓����蔻�肪NULL�ł͂Ȃ��Ȃ�
	// �f�o�b�O����
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->Debug();
	}
	// �ړ��ʕ\��
	CDebugproc::Print("�L�����N�^�[[%d]:�ړ���(%.2f,%.2f,%.2f)\n", m_character, m_move.x, m_move.y, m_move.z);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��(�L�����N�^�[�S��)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::AllDebug(void)
{
	/*
	// �L�����N�^�[�X�e�[�^�X�̕\�� //
	CDebugproc::Print("//----------�L�����N�^�[�X�e�[�^�X���----------//\n");
	for (int nCntCharacter = 0; nCntCharacter < CHARACTER_MAX; nCntCharacter++)
	{
		CDebugproc::Print("// [%d]\n",nCntCharacter);
		CDebugproc::Print("�ő厝���Ă����镗�D��:%d\n", m_sStatus[nCntCharacter].nMaxBalloon);
		CDebugproc::Print("�ő�o�����D��:%d\n", m_sStatus[nCntCharacter].nMaxPopBalloon);
		CDebugproc::Print("������:%.2f\n", m_sStatus[nCntCharacter].fMaxInertia);
		CDebugproc::Print("�W�����v��:%.2f\n", m_sStatus[nCntCharacter].fMaxJump);
		CDebugproc::Print("�ړ���:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);
		CDebugproc::Print("�ړ���:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);
		CDebugproc::Print("�ړ���:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);
		CDebugproc::Print("�ړ���:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);

	}
	*/
}
#endif // _DEBUG

