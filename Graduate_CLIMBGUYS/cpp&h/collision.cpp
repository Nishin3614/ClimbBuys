// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Collision�����̐���[collision.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Collision.h"
#include "debugproc.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define COLLISION_FILE "data/LOAD/collision.txt"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ�
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
int CCollision::nCollisionTime = 0;
#endif

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision::CCollision() : CScene::CScene()
{
	// ������
	m_pOwner = NULL;
	m_pParent = NULL;
	m_bCollision = false;
	m_bUse = true;
	m_nMyObjectId = 0;
	m_nOponentId = -1;
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Debug(void)
{
	// �ϐ��錾
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posold;

	// ����� //
	// �ʒu���擾
	if (GetShape()->Get_PPos() != NULL)
	{
		pos = *GetShape()->Get_PPos();
	}
	else
	{
		pos = GetShape()->Get_Pos();
	}
	// �ߋ��̈ʒu���擾
	if (GetShape()->Get_PPosold() != NULL)
	{
		posold = *GetShape()->Get_PPosold();
	}
	else
	{
		posold = GetShape()->Get_Posold();
	}
	/*
	ImGui::Text("Pos(%.1f,%.1f,%.1f)",
		pos.x,
		pos.y,
		pos.z
	);
	ImGui::Text("PosOld(%.1f,%.1f,%.1f)",
		posold.x,
		posold.y,
		posold.z
	);
	ImGui::Text("nCollisionTime:%d", nCollisionTime);
	*/
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �������瓖��������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_MyCollision(
	int const &nObjType,	// �I�u�W�F�N�g�^�C�v
	CScene * pScene			// ����̃V�[�����
)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���肩�瓖�Ă�ꂽ��̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_OpponentCollision(
	int const &nObjType,	// �I�u�W�F�N�g�^�C�v
	CScene * pScene			// ����̃V�[�����
)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �������瓖����Ȃ�������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_NoMyCollision(
	int const &nObjType,	// �I�u�W�F�N�g�^�C�v
	CScene * pScene			// ����̃V�[�����
)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����ɓ��Ă��Ȃ�������̏���
//	nObjType	: �I�u�W�F�N�g�^�C�v
//	pScene		: ����̃V�[�����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_NoOpponentCollision(
	int const &nObjType,	// �I�u�W�F�N�g�^�C�v
	CScene * pScene			// ����̃V�[�����
)
{
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�N���X�̑I��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::SelectShape(
	CShape * const shape
)
{
	// �N���X�^��r //
	// ��`�N���X
	if (shape->GetType() == CShape::SHAPETYPE_RECT)
	{
		return Judg((CRectShape*)shape);
	}
	// ���N���X
	else if (shape->GetType() == CShape::SHAPETYPE_SPHERE)
	{
		return Judg((CSphereShape*)shape);
	}
	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��(�w��)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::CollisionDetection(
	CCollision * pCollision
)
{
	// �����̓����蔻����NULL�̏ꍇ ||
	// �����̓����蔻�肪�����̂����蔻��Ȃ�
	// ->�֐��𔲂���
	if (pCollision == NULL ||
		this == pCollision)
	{
		return false;
	}
	// �����̓����蔻��̎g�p��Ԃ�false ||
	// ����̓����蔻��̎g�p��Ԃ�false ||
	// ->�֐��𔲂���
	if (!this->m_bUse ||
		!pCollision->m_bUse)
	{
		return false;
	}
	// �����̃I�u�W�F�N�g����NULL�`�F�b�N ||
	// ����̃I�u�W�F�N�g�̐e����NULL�`�F�b�N ||
	// �����̃I�u�W�F�N�g���Ƒ���̃I�u�W�F�N�g�̐e��񂪓����Ȃ�
	// ->�֐��𔲂���
	if (this->m_pOwner == NULL ||
		this->m_pOwner == pCollision->m_pParent)
	{
		return false;
	}

	// �ϐ��錾
	bool bJudg = false;	// �����蔻����
	// �N���X�^��r //
	// ��`�N���X
	if (pCollision->GetShape()->GetType() == CShape::SHAPETYPE_RECT)
	{
		bJudg = Judg((CRectShape*)pCollision->GetShape());
	}
	// ���N���X
	else if (pCollision->GetShape()->GetType() == CShape::SHAPETYPE_SPHERE)
	{
		bJudg = Judg((CSphereShape*)pCollision->GetShape());
	}
	// �~���N���X
	else if (pCollision->GetShape()->GetType() == CShape::SHAPETYPE_COLUMN)
	{
		bJudg = Judg((CColumnShape*)pCollision->GetShape());
	}
	// ���肪true�Ȃ�
	// ->����ۑ�
	if (bJudg ==  true)
	{

#ifdef _DEBUG
		// �e�X�g�ϐ�
		nCollisionTime++;
#endif // _DEBUG

		// ����̓����蔻���Ԃ�true��
		pCollision->m_bCollision = true;
		// ����̔ԍ�����
		pCollision->m_nOponentId = m_nMyObjectId;
		// �V�[�����NULL�ł͂Ȃ��Ȃ�
		// ->����������̏������s��
		if (m_pOwner != NULL)
		{
			m_pOwner->Scene_MyCollision(pCollision->m_nMyObjectId, pCollision->m_pOwner);
		}
		// ����̃V�[�����NULL�ł͂Ȃ��Ȃ�
		// ->����������̏������s��
		if (pCollision->m_pOwner != NULL)
		{
			pCollision->m_pOwner->Scene_OpponentCollision(m_nMyObjectId, m_pOwner);
		}
	}
	else
	{
		// �V�[�����NULL�ł͂Ȃ��Ȃ�
		// ->����������̏������s��
		if (m_pOwner != NULL)
		{
			m_pOwner->Scene_NoMyCollision(pCollision->m_nMyObjectId, pCollision->m_pOwner);
		}
		// ����̃V�[�����NULL�ł͂Ȃ��Ȃ�
		// ->����������̏������s��
		if (pCollision->m_pOwner != NULL)
		{
			pCollision->m_pOwner->Scene_NoOpponentCollision(m_nMyObjectId, m_pOwner);
		}
	}
	// �����蔻���Ԃ�Ԃ�
	return bJudg;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��(�w��I�u�W�F�N�g)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::CollisionDetection(OBJTYPE const & obj)
{
	// �ϐ��錾
	CCollision * pCollision;	// �����蔻����
	bool bCollision = false;	// �����蔻����
	// ����
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer++)
	{
		// �����蔻��擾
		pCollision = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer);
		// �����蔻���NULL�`�F�b�N ||
		// �����蔻�肪�����̓����蔻��ƃA�h���X�������Ȃ�
		// ->���[�v�X�L�b�v
		if (pCollision == NULL ||
			pCollision == this) continue;
		// �w�肵���I�u�W�F�N�g�ł͂Ȃ��Ȃ�
		// ->���[�v�X�L�b�v
		else if (pCollision->m_nMyObjectId != obj) continue;
		// �����蔻���true�ɂ���
		bCollision |= this->CollisionDetection(pCollision);
	}
	// �����蔻���Ԃ�Ԃ�
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��(�Ǘ����Ă���S��)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::CollisionDetection(void)
{
	// �ϐ��錾
	CCollision * pCollision1 = NULL;	// �����蔻����
	CCollision * pCollision2 = NULL;	// �����蔻����

	// ����
	for (int nCntLayer1 = 0; nCntLayer1 < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer1++)
	{
		// �����蔻��擾
		pCollision1 = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer1);
		// �����蔻����NULL�Ȃ�
		if (pCollision1 == NULL)
		{
			continue;
		}
		// !�I�u�W�F�N�g�^�C�v���v���C���[�Ȃ�
		else if (!(pCollision1->m_nMyObjectId == OBJTYPE_PLAYER)
			)
		{
			continue;
		}
		// ����
		for (int nCntLayer2 = 0; nCntLayer2 < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer2++)
		{
			// �����蔻��擾
			pCollision2 = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer2);
			// �����̓����蔻����NULL�̏ꍇ ||
			// �����̓����蔻�肪�����̂����蔻��Ȃ�
			// ->�֐��𔲂���
			if (pCollision2 == NULL ||
				pCollision1 == pCollision2)
			{
				continue;
			}
			// �����̓����蔻��̎g�p��Ԃ�false ||
			// ����̓����蔻��̎g�p��Ԃ�false ||
			// ->�֐��𔲂���
			else if (!pCollision1->m_bUse ||
				!pCollision2->m_bUse)
			{
				continue;
			}
			// �����̃I�u�W�F�N�g����NULL�`�F�b�N ||
			// ����̃I�u�W�F�N�g�̐e����NULL�`�F�b�N ||
			// �����̃I�u�W�F�N�g���Ƒ���̃I�u�W�F�N�g�̐e��񂪓����Ȃ�
			// ->�֐��𔲂���
			else if (pCollision1->m_pOwner == NULL ||
				pCollision1->m_pOwner == pCollision2->m_pParent)
			{
				continue;
			}
			// �ϐ��錾
			bool bJudg = false;	// �����蔻����
								// �N���X�^��r //
								// ��`�N���X
			if (pCollision2->GetShape()->GetType() == CShape::SHAPETYPE_RECT)
			{
				bJudg = pCollision1->Judg((CRectShape*)pCollision2->GetShape());
			}
			// ���N���X
			else if (pCollision2->GetShape()->GetType() == CShape::SHAPETYPE_SPHERE)
			{
				bJudg = pCollision1->Judg((CSphereShape*)pCollision2->GetShape());
			}
			// �~���N���X
			else if (pCollision2->GetShape()->GetType() == CShape::SHAPETYPE_COLUMN)
			{
				bJudg = pCollision1->Judg((CColumnShape*)pCollision2->GetShape());
			}
			// ���肪true�Ȃ�
			// ->����ۑ�
			if (bJudg == true)
			{

#ifdef _DEBUG
				// �e�X�g�ϐ�
				nCollisionTime++;
#endif // _DEBUG

				// ����̓����蔻���Ԃ�true��
				pCollision2->m_bCollision = true;
				// ����̔ԍ�����
				pCollision2->m_nOponentId = pCollision1->m_nMyObjectId;
				// �V�[�����NULL�ł͂Ȃ��Ȃ�
				// ->����������̏������s��
				if (pCollision1->m_pOwner != NULL)
				{
 					pCollision1->m_pOwner->Scene_MyCollision(pCollision2->m_nMyObjectId, pCollision2->m_pOwner);
				}
				// ����ȊO�Ȃ�
				// ->�����蔻�莩�̂̓���������̏������s��
				else
				{
					pCollision1->Scene_MyCollision(pCollision2->m_nMyObjectId, pCollision2->m_pOwner);
				}
				// ����̃V�[�����NULL�ł͂Ȃ��Ȃ�
				// ->���Ă�ꂽ��̏������s��
				if (pCollision2->m_pOwner != NULL)
				{
					pCollision2->m_pOwner->Scene_OpponentCollision(pCollision1->m_nMyObjectId, pCollision1->m_pOwner);
				}
				// ����ȊO�Ȃ�
				// ->�����蔻�莩�̂̓��Ă�ꂽ��̏������s��
				else
				{
					pCollision2->Scene_OpponentCollision(pCollision1->m_nMyObjectId, pCollision1->m_pOwner);
				}
			}
			else
			{
				// �V�[�����NULL�ł͂Ȃ��Ȃ�
				// ->����������̏������s��
				if (pCollision1->m_pOwner != NULL)
				{
					pCollision1->m_pOwner->Scene_NoMyCollision(pCollision2->m_nMyObjectId, pCollision2->m_pOwner);
				}
				// ����̃V�[�����NULL�ł͂Ȃ��Ȃ�
				// ->����������̏������s��
				if (pCollision2->m_pOwner != NULL)
				{
					pCollision2->m_pOwner->Scene_NoOpponentCollision(pCollision1->m_nMyObjectId, pCollision1->m_pOwner);
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻���Ԃ̋���false��
// ����������̍s�����N���������ɔ����false�ɂ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetCollision(void)
{
	m_bCollision = false;
	m_nOponentId = -1;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻������L���Ă���V�[����������NULL���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::CompulsionScene(void)
{
	m_pOwner = NULL;
	m_pParent = NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��`�Ƌ�`�̔���
//	pRectShapeA	: ��`A
//	pRectShapeB	: ��`B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::RectAndRect(
	CRectShape * const pRectShapeA,	// ��`A
	CRectShape * const pRectShapeB	// ��`B
)
{
	// �ϐ��錾
	D3DXVECTOR3 const &min_A = pRectShapeA->GetMin();
	D3DXVECTOR3 const &min_B = pRectShapeB->GetMin();
	D3DXVECTOR3 const &max_A = pRectShapeA->GetMax();
	D3DXVECTOR3 const &max_B = pRectShapeB->GetMax();
	D3DXVECTOR3 const &minOld_A = pRectShapeA->GetMinOld();
	D3DXVECTOR3 const &minOld_B = pRectShapeB->GetMinOld();
	D3DXVECTOR3 const &maxOld_A = pRectShapeA->GetMaxOld();
	D3DXVECTOR3 const &maxOld_B = pRectShapeB->GetMaxOld();
	D3DXVECTOR3 *pos_A = NULL;
	bool bCollision = false;
	// �|�C���^�[�ʒu���NULL�ł͂Ȃ��Ȃ�
	// ->�ʒu���ɑ��
	if (pRectShapeA->Get_PPos() != NULL)
	{
		pos_A = pRectShapeA->Get_PPos();
	}
	// �ڐG���Ă��Ȃ��Ƃ���false��Ԃ�
	if (max_A.y > min_B.y &&
		min_A.y < max_B.y)
	{
		// �f�ނ�Z�͈�
		if (max_A.z > min_B.z&&
			min_A.z < max_B.z)
		{
			// �����蔻��(��)
			if (max_A.x > min_B.x &&
				maxOld_A.x <= min_B.x)
			{
				// �ߋ��̈ʒu���A��NULL�ł͂Ȃ��Ȃ� ||
				// �����o������������Ȃ�
				// ->�����o������
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// �f�ޏ�̍���
					pos_A->x = min_B.x - pRectShapeA->GetSize().x * 0.6f;
				}
				// �ڐG���Ă���Ƃ���true��Ԃ�
				bCollision = true;
			}

			// �����蔻��(�E)
			else if (min_A.x < max_B.x &&
				minOld_A.x >= max_B.x)
			{
				// �ߋ��̈ʒu���A��NULL�ł͂Ȃ��Ȃ� ||
				// �����o������������Ȃ�
				// ->�����o������
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// �f�ޏ�̍���
					pos_A->x = max_B.x + pRectShapeA->GetSize().x * 0.6f;
				}
				// �ڐG���Ă���Ƃ���true��Ԃ�
				bCollision = true;
			}
		}
		// �f�ނ�X�͈�
		if (max_A.x > min_B.x&&
			min_A.x < max_B.x)
		{
			// �����蔻��(��O)
			if (max_A.z > min_B.z&&
				maxOld_A.z <= min_B.z)
			{
				// �ߋ��̈ʒu���A��NULL�ł͂Ȃ��Ȃ� ||
				// �����o������������Ȃ�
				// ->�����o������
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// �f�ޏ�̍���
					pos_A->z = min_B.z - pRectShapeA->GetSize().z * 0.6f;
				}
				// �ڐG���Ă���Ƃ���true��Ԃ�
				bCollision = true;
			}

			// �����蔻��(��)
			else if (min_A.z < max_B.z &&
				minOld_A.z >= max_B.z)
			{
				// �ߋ��̈ʒu���A��NULL�ł͂Ȃ��Ȃ� ||
				// �����o������������Ȃ�
				// ->�����o������
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// �f�ޏ�̍���
					pos_A->z = max_B.z +
						pRectShapeA->GetSize().z * 0.6f;
				}
				// �ڐG���Ă���Ƃ���true��Ԃ�
				bCollision = true;
			}

		}
	}
	// �f�ނ�Z�͈�
	if (max_A.z > min_B.z&&
		min_A.z < max_B.z)
	{
		// �f�ނ�X�͈�
		if (max_A.x > min_B.x&&
			min_A.x < max_B.x)
		{
			// �����蔻��(��)
			if (max_A.y > min_B.y &&
				maxOld_A.y <= min_B.y)
			{
				// �ߋ��̈ʒu���A��NULL�ł͂Ȃ��Ȃ� ||
				// �����o������������Ȃ�
				// ->�����o������
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// �f�ޏ�̍���
					pos_A->y = min_B.y - pRectShapeA->GetSize().y;
				}
				// �ڐG���Ă���Ƃ���true��Ԃ�
 				bCollision = true;
			}

			// �����蔻��(��)
			else if (min_A.y < max_B.y&&
				minOld_A.y >= max_B.y)
			{
				// �ߋ��̈ʒu���A��NULL�ł͂Ȃ��Ȃ� ||
				// �����o������������Ȃ�
				// ->�����o������
				if (pos_A != NULL ||
					pRectShapeA->m_bPush)
				{
					// �f�ޏ�̍���
					pos_A->y = max_B.y + 0.1f;
				}
				// �ڐG���Ă���Ƃ���true��Ԃ�
				bCollision = true;
			}

		}
	}
	// �������Ă�����X�V
	if (bCollision)
	{
		// �ʒu���X�V
		pRectShapeA->PassPos(D3DVECTOR3_ZERO);
	}
	// �ڐG���Ă���Ƃ���true��Ԃ�
	return bCollision;






	// ��邱��
	// ����������𐮗�����
	// ����Ȃ���������

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��`�Ƌ��̓����蔻��
//	pRectShapeA		: ��`A
//	pSphereShapeB	: ��B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::RectAndSphere(
	CRectShape * const pRectShapeA,
	CSphereShape * const pSphereShapeB
)
{
	// �ϐ��錾
	D3DXVECTOR3 ClosestPoint;	// ������W�̍ł��߂��A�{�b�N�X��̍��W
	D3DXVECTOR3 *pos_A;			//
	D3DXVECTOR3 *pos_B;			//
	bool bCollision = false;	// �������Ă��邩�ǂ���
	// ��`A�̃|�C���g�ʒu���NULL�ł͂Ȃ��ꍇ
	// �ʒu�����
	if (pRectShapeA->Get_PPos() != NULL)
	{
		pos_A = pRectShapeA->Get_PPos();
	}
	else
	{
		pos_A = &pRectShapeA->Get_Pos();
	}
	// �X�t�B�AB�̃|�C���g�ʒu���NULL�ł͂Ȃ��ꍇ
	// �ʒu�����
	if (pSphereShapeB->Get_PPos() != NULL)
	{
		pos_B = pSphereShapeB->Get_PPos();
	}
	else
	{
		pos_B = &pSphereShapeB->Get_Pos();
	}

	// ������W�̍ł��߂��A�{�b�N�X��̍��W
	ClosestPoint = pRectShapeA->GetClosestpoint(*pos_B + pSphereShapeB->GetOffset());
	float a = CCalculation::DiffPointSquare(*pos_B + pSphereShapeB->GetOffset(), ClosestPoint);
	// �����蔻�菈��
	bCollision = CCalculation::DiffPointSquare(*pos_B + pSphereShapeB->GetOffset(), ClosestPoint) <
		pSphereShapeB->GetRadius() * pSphereShapeB->GetRadius();
	// �������Ă����� &&
	// ��`A�̉����o��������true�Ȃ� &&
	// ��B�̑���ɑ΂��鉟���o��������true�Ȃ�
	// ->��`A�������o�����
	if (bCollision &&
		pRectShapeA->m_bPush &&
		pSphereShapeB->m_bOpponentPush
		)
	{

	}
	// �������Ă����� &&
	// ��`B�̉����o��������true�Ȃ� &&
	// ��`A�̑���ɑ΂��鉟���o��������true�Ȃ�
	// ->��B�������o�����
	else if (bCollision &&
		pRectShapeA->m_bOpponentPush &&
		pSphereShapeB->m_bPush
		)
	{
		// �ϐ��錾
		D3DXVECTOR3 A_To_BVec;	// A����B�̃x�N�g��
		A_To_BVec = *pos_B + pSphereShapeB->GetOffset() - ClosestPoint;
		D3DXVec3Normalize(&A_To_BVec, &A_To_BVec);
		*pos_B = ClosestPoint + A_To_BVec * pSphereShapeB->GetRadius() - pSphereShapeB->GetOffset();
	}
	// ���������a���Z���ꍇtrue,����ȊOfalse��Ԃ�
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��`�Ɖ~���̓����蔻��
//	pRectShapeA		: ��`A
//	pColumnShapeB	: �~��B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::RectAndColumn(
	CRectShape * const pRectShapeA,
	CColumnShape * const pColumnShapeB
)
{
	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���Ɖ~���̓����蔻��
//	pSphereShapeA	: ��A
//	pColumnShapeB	: �~��B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::SphereAndColumn(
	CSphereShape * const pSphereShapeA,
	CColumnShape * const pColumnShapeB
)
{
	// �ϐ��錾
	D3DXVECTOR3 *pos_A;			// �ʒuA
	D3DXVECTOR3 *pos_B;			// �ʒuB
	bool bCollision = false;	// �����蔻����
	// �X�t�B�AA�̃|�C���g�ʒu���NULL�ł͂Ȃ��ꍇ
	// �ʒu�����
	if (pSphereShapeA->Get_PPos() != NULL)
	{
		pos_A = pSphereShapeA->Get_PPos();
	}
	else
	{
		pos_A = &pSphereShapeA->Get_Pos();
	}
	// �X�t�B�AB�̃|�C���g�ʒu���NULL�ł͂Ȃ��ꍇ
	// �ʒu�����
	if (pColumnShapeB->Get_PPos() != NULL)
	{
		pos_B = pColumnShapeB->Get_PPos();
	}
	else
	{
		pos_B = &pColumnShapeB->Get_Pos();
	}

	// y����
	if ((pos_B->y + pColumnShapeB->GetOffset().y + pColumnShapeB->GetVertical()) <
		(pos_A->y + pSphereShapeA->GetOffset().y - pSphereShapeA->GetRadius()))
	{
		return false;
	}
	else if ((pos_B->y + pColumnShapeB->GetOffset().y - pColumnShapeB->GetVertical()) >
		(pos_A->y + pSphereShapeA->GetOffset().y + pSphereShapeA->GetRadius()))
	{
		return false;
	}
	// ���Ɖ~���̋����̔�r
	// �������Ă��邩�ǂ���
	bCollision = CCalculation::Collision_Sphere(
		*pos_A + pSphereShapeA->GetOffset(),
		pSphereShapeA->GetRadius(),
		*pos_B + pColumnShapeB->GetOffset(),
		pColumnShapeB->GetRadius()
	);
	if (bCollision)
	{
		// �����蔻��\��
		CDebugproc::Print("���Ɖ~���̓����蔻���� = 1\n");
	}
	else
	{
		// �����蔻��\��
		CDebugproc::Print("���Ɖ~���̓����蔻���� = 0\n");
	}
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���Ƌ��̓����蔻��
//	pSphereShapeA	: ��A
//	pSphereShapeB	: ��B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::SphereAndSphere(
	CSphereShape * const pSphereShapeA,
	CSphereShape * const pSphereShapeB
)
{
	// �ϐ��錾
	D3DXVECTOR3 *pos_A;
	D3DXVECTOR3 *pos_B;
	bool bCollision = false;
	// �X�t�B�AA�̃|�C���g�ʒu���NULL�ł͂Ȃ��ꍇ
	// �ʒu�����
	if (pSphereShapeA->Get_PPos() != NULL)
	{
		pos_A = pSphereShapeA->Get_PPos();
	}
	else
	{
		pos_A = &pSphereShapeA->Get_Pos();
	}
	// �X�t�B�AB�̃|�C���g�ʒu���NULL�ł͂Ȃ��ꍇ
	// �ʒu�����
	if (pSphereShapeB->Get_PPos() != NULL)
	{
		pos_B = pSphereShapeB->Get_PPos();
	}
	else
	{
		pos_B = &pSphereShapeB->Get_Pos();
	}

	// �ϐ��錾
	//D3DXVECTOR3 posA = pSphereShapeA->m_DestPos;	// �ʒuA
	//D3DXVECTOR3 posB = pSphereShapeB->m_DestPos;	// �ʒuB
	// �������Ă��邩�ǂ���
	bCollision = CCalculation::Collision_Sphere(
		*pos_A + pSphereShapeA->GetOffset(),
		pSphereShapeA->GetRadius(),
		*pos_B + pSphereShapeB->GetOffset(),
		pSphereShapeB->GetRadius()
	);
	// �������Ă����� &&
	// ��A�̉����o��������true�Ȃ� &&
	// ��B�̑���ɑ΂��鉟���o��������true�Ȃ�
	// ->��A�������o�����
	if (bCollision &&
		pSphereShapeA->m_bPush &&
		pSphereShapeB->m_bOpponentPush
		)
	{
		D3DXVECTOR3 diff = *pos_A - *pos_B;						// B����A�̍�
		D3DXVECTOR3 vec;									// B->A�̃x�N�g��
															// �x�N�g���̐��K��
		D3DXVec3Normalize(&vec, &diff);
		// �����o��
		*pos_A = *pos_B + vec * (pSphereShapeA->GetRadius() + pSphereShapeB->GetRadius());

		pSphereShapeA->PassPos(D3DVECTOR3_ZERO);
	}
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �~���Ɖ~���̓����蔻��
//	pColumnShapeA	: �~��A
//	pColumnShapeB	: �~��B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::ColumnAndColumn(
	CColumnShape * const pColumnShapeA,
	CColumnShape * const pColumnShapeB
)
{
	return false;
}
