// ----------------------------------------------------------------------------------------------------
//
// Calculation�����̐���[calculation.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------
#include "Calculation.h"
#include "debugproc.h"
#include "renderer.h"
#include "XInputPad.h"
#include "keyboard.h"

// ----------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
// ----------------------------------------------------------------------------------------------------
CCalculation::PAD_STICK CCalculation::m_PadStick[(int)PLAYER_TAG::PLAYER_MAX]	= {};				// �p�b�h�̏��
DIRECTION CCalculation::m_direction												= DIRECTION::LEFT;	// ����

// ----------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------------------------------------------------------------------
bool CCalculation::m_bWire = false;

// ----------------------------------------------------------------------------------------------------
// �O�ς̌v�Z
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Cross_product(
	D3DXVECTOR3 &VecA,
	D3DXVECTOR3 &VecB)
{
	return D3DXVECTOR3(
		VecA.y * VecB.z - VecA.z * VecB.y,
		VecA.z * VecB.x - VecA.x * VecB.z,
		VecA.x * VecB.y - VecA.y * VecB.x);
}

// ----------------------------------------------------------------------------------------------------
// ���ς̌v�Z
// ----------------------------------------------------------------------------------------------------
float CCalculation::Dot_product(D3DXVECTOR3 &VecA, D3DXVECTOR3 &VecB)
{
	return (
		VecA.x * VecB.x +
		VecA.y * VecB.y +
		VecA.z * VecB.z);
}

// ----------------------------------------------------------------------------------------------------
// ��ʂ̎l�p�`�̒��ɓ����Ă��邩���Ȃ���
// ----------------------------------------------------------------------------------------------------
bool CCalculation::SquareBottom_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
)
{
	// �ϐ��錾
	D3DXVECTOR3 SquareDiff[4];
	D3DXVECTOR3 WorldDiff[4];
	//float		fCross_p[4];
	bool		bCross = false;

	// ���̋���
	SquareDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// ��̋���
	SquareDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// �E�̋���
	SquareDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// ���̋���
	SquareDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// ���̋���
	WorldDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// ��̋���
	WorldDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// �E�̋���
	WorldDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// ���̋���
	WorldDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// �O�ς̒����O��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		/*
		// �O��
		fCross_p[nCnt] = Cross_product(SquareDiff[nCnt], WorldDiff[nCnt]);

		// �O�ς̔��� //
		// ��
		if (fCross_p[nCnt] > 0)
		{
		bCross = true;
		}

		// �O
		else
		{
		return false;
		}
		*/
	}

	// �����O����Ԃ�
	return bCross;
}

// ----------------------------------------------------------------------------------------------------
// ���ʂ̎l�p�`�̒��ɓ����Ă��邩���Ȃ���
// ----------------------------------------------------------------------------------------------------
bool CCalculation::SquareSide_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int nBlock_W,
	int nBlock_H,
	D3DXVECTOR3 worldpos)
{
	// �����͈̔�
	if (worldpos.y < pos.y + size.y * nBlock_H &&
		worldpos.y >= pos.y)
	{
		// ���͈̔�
		if (worldpos.x > pos.x - size.x * 0.5f * nBlock_W * sinf(rot.y) &&
			worldpos.x < pos.x + size.x * 0.5f * nBlock_W * sinf(rot.y))
		{
			return true;
		}
		// ���͈̔�
		if (worldpos.z > pos.z - size.x * 0.5f * nBlock_W * cosf(rot.y) &&
			worldpos.z < pos.z + size.x * 0.5f * nBlock_W * cosf(rot.y))
		{
			return true;
		}
	}


	// �����O����Ԃ�
	return false;
}

// ----------------------------------------------------------------------------------------------------
// std::unique_ptr[int]�^�̏���Ԃ�
// ����:�Ԃ�����std::move���g�p���邱��
// ----------------------------------------------------------------------------------------------------
std::unique_ptr<int> CCalculation::int_ptr(int const nNum)
{
	// �ϐ��錾
	std::unique_ptr<int> uni_int(new int(nNum));	// std::unique_ptr[int]���
	return uni_int;
}

// ----------------------------------------------------------------------------------------------------
// std::unique_ptr[bool]�^�̏���Ԃ�
// ����:�Ԃ�����std::move���g�p���邱��
// ----------------------------------------------------------------------------------------------------
std::unique_ptr<bool> CCalculation::bool_ptr(void)
{
	// �ϐ��錾
	std::unique_ptr<bool> uni_bool(new bool);	// std::unique_ptr[bool]���
	return uni_bool;
}

// ----------------------------------------------------------------------------------------------------
// ���W���m�̍��v�Z
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Difference_Between(
	D3DXVECTOR3 &posA,
	D3DXVECTOR3 &posB)
{
	// ��
	D3DXVECTOR3 Diff;

	//���W���m�̍��v�Z
	Diff = posA - posB;

	return Diff;
}

// ----------------------------------------------------------------------------------------------------
// �Ԃ̍��̌v�Z(3D�p:�ʒu�A�ʒu)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPoint(
	D3DXVECTOR3 const & posA,
	D3DXVECTOR3 const & posB
)
{
	// �ϐ��錾
	D3DXVECTOR3 diffpos;	// �����̈ʒu
	float diff, diff2;		// ��
							// �����̈ʒu�̌v�Z
	diffpos = posA - posB;
	// x��y�̎΂߂̒���
	diff = sqrt(diffpos.x * diffpos.x + diffpos.y * diffpos.y);
	// ����o���΂߂�z�̒���
	diff2 = sqrt(diff * diff + diffpos.z * diffpos.z);
	return diff2;
}

// ----------------------------------------------------------------------------------------------------
// �Ԃ̍��̌v�Z(2D�p:�ʒu�A�ʒu)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPoint(
	D3DXVECTOR2 const & posA,
	D3DXVECTOR2 const & posB
)
{
	// �ϐ��錾
	D3DXVECTOR2 diffpos;	// �����̈ʒu
	float diff;		// ��
					// �����̈ʒu�̌v�Z
	diffpos = posA - posB;
	// x��y�̎΂߂̒���
	diff = sqrt(diffpos.x * diffpos.x + diffpos.y * diffpos.y);
	return diff;
}

// ----------------------------------------------------------------------------------------------------
// �Ԃ̍���2��v�Z(3D�p:�ʒu�A�ʒu)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPointSquare(D3DXVECTOR3 const & posA, D3DXVECTOR3 const & posB)
{
	// �ϐ��錾
	float PointSquare = DiffPoint(posA, posB);	// ����
	return PointSquare * PointSquare;
}

// ----------------------------------------------------------------------------------------------------
// �Ԃ̍���2��v�Z(2D�p:�ʒu�A�ʒu)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPointSquare(D3DXVECTOR2 const & posA, D3DXVECTOR2 const & posB)
{
	// �ϐ��錾
	float PointSquare = DiffPoint(posA, posB);	// ����
	return PointSquare * PointSquare;
}

// ----------------------------------------------------------------------------------------------------
// ���̑����Z�v�Z
// ----------------------------------------------------------------------------------------------------
float CCalculation::Addition_Sphere(
	float Length_x,
	float Length_y,
	float Length_z)
{
	// �ϐ��錾
	float fLengthX = 0;
	float fLengthY = 0;
	float fLengthZ = 0;
	float Lenght = 0;

	// �~�̌v�Z
	fLengthX = Length_x * Length_x;
	fLengthY = Length_y * Length_y;
	fLengthZ = Length_z * Length_z;

	// �~���m�̑����Z
	Lenght = fLengthX + fLengthY + fLengthZ;

	return Lenght;
}

// ----------------------------------------------------------------------------------------------------
// �~�̓����蔻��
// ----------------------------------------------------------------------------------------------------
bool CCalculation::Collision_Circle(
	D3DXVECTOR2 &Me,
	float		fMeSize,
	D3DXVECTOR2 &Opponent,
	float		fOppSize
)
{
	// �ϐ��錾
	float fRange;		// �͈�
	float fDistance;	// ����

	// �͈͂̌v�Z
	//fRange = fMeSize * fMeSize + fOppSize * fOppSize;
	fRange = (fMeSize + fOppSize) * (fMeSize + fOppSize);
	// ���ۂ̋����̌v�Z
	fDistance =
		(Me.x - Opponent.x) * (Me.x - Opponent.x) +
		(Me.y - Opponent.y) * (Me.y - Opponent.y);
	// �������Ă��邩���Ȃ�����Ԃ�
	return (fRange >= fDistance) ? true : false;
}

// ----------------------------------------------------------------------------------------------------
// ���̓����蔻��
// ----------------------------------------------------------------------------------------------------
bool CCalculation::Collision_Sphere(
	D3DXVECTOR3 const &Me,
	float const &fMeSize,
	D3DXVECTOR3 const &Opponent,
	float const &fOppSize
)
{
	// �ϐ��錾
	float fRange;		// �͈�
	float fDistance;	// ����

						// �͈͂̌v�Z
	fRange = fMeSize * fMeSize + fOppSize * fOppSize;

	// ���ۂ̋����̌v�Z
	fDistance =
		(Me.x - Opponent.x) * (Me.x - Opponent.x) +
		(Me.y - Opponent.y) * (Me.y - Opponent.y) +
		(Me.z - Opponent.z) * (Me.z - Opponent.z);

	// �������Ă��邩���Ȃ�����Ԃ�
	return (fRange >= fDistance) ? true : false;
}

// ----------------------------------------------------------------------------------------------------
// ��]�̌��E�v�Z
// ----------------------------------------------------------------------------------------------------
float CCalculation::Rot_One_Limit(float &fRot)
{
	// -3.14�`3.14�ɉ�]�������Œ肳����
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;		// -3.14����
	}

	else if (fRot > D3DX_PI)
	{
		fRot += -D3DX_PI * 2;	// 3.14����
	}

	return fRot;
}

// ----------------------------------------------------------------------------------------------------
// �X�N���[�����W�����[���h���W�ɕϊ�
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3* CCalculation::CalcScreenToWorld(
	D3DXVECTOR3* pout,	// �}�E�X�|�C���g
	float Sx,				// �X�N���[��X���W
	float Sy,				// �X�N���[��Y���W
	float fZ,			// �ˉe��Ԃł�Z�l�i0�`1�j
	float Screen_w,		// �X�N���[���̉���
	float Screen_h,		// �X�N���[���̍���
	D3DXMATRIX* mtxView,// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj)	// �v���W�F�N�V�����}�g���b�N�X
{
	// �e�s��̋t�s����Z�o //
	// �ϐ��錾
	D3DXMATRIX mtxInvViex;		// �v�Z�p�t�s��r���[�}�g���b�N�X
	D3DXMATRIX mtxInvPrj;		// �v�Z�p�t�s��v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxVP;			// �v�Z�p�r���[�|�b�g
	D3DXMATRIX mtxInvViexport;	// �v�Z�p�t�s��r���[�|�b�g
	D3DXMATRIX mtxChange;		// ���W�ϊ��p�}�g���b�N�X

								// �r���[�|�b�g�̍s��쐬
	D3DXMatrixIdentity(&mtxVP);	// ������
	mtxVP._11 = Screen_w / 2.0f;
	mtxVP._22 = -Screen_h / 2.0f;
	mtxVP._41 = Screen_w / 2.0f;
	mtxVP._42 = Screen_h / 2.0f;

	// �t�s��̌v�Z
	D3DXMatrixInverse(&mtxInvViex, NULL, mtxView);		// �r���[�}�g���b�N�X
	D3DXMatrixInverse(&mtxInvPrj, NULL, mtxPrj);		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixInverse(&mtxInvViexport, NULL, &mtxVP);	// �r���[�|�b�g�}�g���b�N�X

														// �X�N���[�����烏�[���h��
	mtxChange = mtxInvViexport * mtxInvPrj * mtxInvViex;		// ���W�ϊ��p
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &mtxChange);	// �X�N���[�����烏�[���h��

																		// ���[���h�̃|�C���g��Ԃ�
	return pout;
}

// ----------------------------------------------------------------------------------------------------
// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3* CCalculation::CalcScreenToXZ(
	D3DXVECTOR3* pout,		// ���[���h���W�̓��ꕨ
	float Sx,				// �X�N���[��X���W
	float Sy,				// �X�N���[��Y���W
	float Screen_w,			// �X�N���[���̉���
	float Screen_h,			// �X�N���[���̍���
	D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj,		// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 obj			// �I�u�W�F�N�g�̈ʒu
)
{
	// �ϐ��錾
	D3DXVECTOR3 Mearpos;	// �ŋߒl
	D3DXVECTOR3 Farpos;		// �ŉ��l
	D3DXVECTOR3 Ray;		// �}�E�X�|�C���g����

							// �X�N���[�����W�����[���h���W�ɕϊ�
	CalcScreenToWorld(		// �ŋߒl
		&Mearpos,
		Sx, Sy,
		0.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);

	CalcScreenToWorld(		// �ŉ��l
		&Farpos,
		Sx, Sy,
		1.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);

	// �}�E�X�|�C���g����
	Ray = Farpos - Mearpos;
	D3DXVec3Normalize(&Ray, &Ray);	// ���K��

	D3DXVECTOR3 diff;

	// ���P����K�v������
	diff = Mearpos - obj;
	diff = -(*D3DXVec3Normalize(&diff, &diff));

	// �����Ƃ̌�_ //
	if (Ray.y <= 0)
	{
		// ����_
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, 1, 0));		// �}�E�X�̕����̓���
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, 1, 0));	// ���_�̓���
		*pout = Mearpos + (LP0 / LRay)*Ray;
	}

	/* �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��	*/
	// �㏰�Ƃ̌�_ //
	else
	{
		// ����_
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, -1, 0));		// �}�E�X�̕����̓���
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, -1, 0));	// ���_�̓���
		*pout = Mearpos + (LP0 / LRay)*Ray;

		//*pout = Farpos;
	}

	return pout;
}

// ----------------------------------------------------------------------------------------------------
// ���[���h���W���X�N���[�����W�ɕϊ�(����)
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3* CCalculation::CalcWorldToScreen(
	D3DXVECTOR3* Obj,	// �I�u�W�F�N�g�̈ʒu
	float Sx,			// �X�N���[��X���W
	float Sy,			// �X�N���[��Y���W
	float fZ,			// �ˉe��Ԃł�Z�l�i0�`1�j
	float Screen_w,		// �X�N���[���̉���
	float Screen_h,		// �X�N���[���̍���
	D3DXMATRIX* mtxView,// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxPrj)	// �v���W�F�N�V�����}�g���b�N�X
{
	// �e�s��̋t�s����Z�o //
	// �ϐ��錾
	D3DXMATRIX mtxVP;			// �v�Z�p�r���[�|�b�g
	D3DXMATRIX mtxChange;				//

										// �r���[�|�b�g�̍s��쐬
	D3DXMatrixIdentity(&mtxVP);			// ������
	mtxVP._11 = SCREEN_WIDTH / 2.0f;
	mtxVP._22 = -SCREEN_HEIGHT / 2.0f;
	mtxVP._41 = SCREEN_WIDTH / 2.0f;
	mtxVP._42 = SCREEN_HEIGHT / 2.0f;

	mtxChange = mtxVP * *mtxPrj * *mtxView;
	D3DXVec3TransformCoord(Obj, &D3DXVECTOR3(Sx, Sy, fZ), &mtxChange);

	return Obj;
}

// ----------------------------------------------------------------------------------------------------
// �ꎟ�֐�
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR2 CCalculation::Linear_function(D3DXVECTOR3 Apos, D3DXVECTOR3 Bpos)
{
	// �ϐ��錾
	float fCons;	// ���萔
	float fSection;	// �ؕ�

					// ���萔�v�Z
	fCons = (Apos.y - Bpos.y) / (Apos.x - Bpos.x);
	// �ؕЌv�Z
	fSection = Apos.y - Apos.x * fCons;
	// ���萔�ƐؕЂ̌v�Z���ʂ�Ԃ�
	return D3DXVECTOR2(fCons, fSection);
}

// ----------------------------------------------------------------------------------------------------
// 2�����̌�_(�A��������)
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::TwoLine_Inse(D3DXVECTOR2 ALinear, D3DXVECTOR2 BLinear)
{
	// �ϐ��錾
	D3DXVECTOR3 Insecsion;	// ��_

							/* ��_�v�Z */
							// x
	Insecsion.x = (BLinear.y - ALinear.y) / (ALinear.x - BLinear.x);
	// y
	Insecsion.y = ALinear.x * Insecsion.x + ALinear.y;
	// ��_��Ԃ�
	return Insecsion;
}

// ----------------------------------------------------------------------------------------------------
// �r���̌v�Z
// ----------------------------------------------------------------------------------------------------
bool CCalculation::TlyCollision(
	D3DXVECTOR3 ObjectPos,
	D3DXVECTOR3 pos,
	...
)
{
	// �ϐ��錾
	D3DXVECTOR3 posA, posB;
	va_list va;	// �ϕϐ�
	int nCntMax;
	// �ψ�����1�ɂ܂Ƃ߂�
	va_start(va, pos);
	nCntMax = (sizeof(va) / sizeof(D3DXVECTOR3));
	for (int nCnt = 0; nCnt < (sizeof(va) / sizeof(D3DXVECTOR3)); nCnt++)
	{
		posA = va_arg(va, D3DXVECTOR3);
		posB = va_arg(va, D3DXVECTOR3);

	}
	va_end(va);

	return false;
}

// ----------------------------------------------------------------------------------------------------
// �O�ς̓����蔻��
// ----------------------------------------------------------------------------------------------------
bool CCalculation::CrossCollision(
	D3DXVECTOR3 *ObjectPos,
	D3DXVECTOR3 *PosA,
	D3DXVECTOR3 *PosB,
	D3DXVECTOR3 *PosC
)
{
	// �ϐ��錾
	D3DXVECTOR3 VecCompOri, VecAhe;		// �x�N�g������
	D3DXVECTOR3 Cross1, Cross2, Cross3;	// �O�ς̌v�Z����
										// ���
	VecCompOri = *PosB - *PosA;
	VecAhe = *ObjectPos - *PosA;
	D3DXVec3Cross(&Cross1, &VecCompOri, &VecAhe);
	// ���
	VecCompOri = *PosC - *PosB;
	VecAhe = *ObjectPos - *PosB;
	D3DXVec3Cross(&Cross2, &VecCompOri, &VecAhe);
	// �O��
	VecCompOri = *PosA - *PosC;
	VecAhe = *ObjectPos - *PosC;
	D3DXVec3Cross(&Cross3, &VecCompOri, &VecAhe);
	// �͈͂ɓ����Ă���
	if (Cross1.y >= 0 && Cross2.y >= 0 && Cross3.y >= 0)
	{
		// �f�o�b�O�\��
		//CDebugproc::Print("�O��[%.3f][%.3f][%.3f]\n", Cross1.y, Cross2.y, Cross3.y);
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

// ----------------------------------------------------------------------------------------------------
// �s�񂩂�ʒu���擾
// 1:�ʒu���,2:�s����
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Vector3ToMatrix(
	D3DXVECTOR3 &vector3Out,
	D3DXMATRIX const & mtx
)
{
	return vector3Out = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

// ----------------------------------------------------------------------------------------------------
// �s�񂩂�ʒu���擾
// 1:�s����
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Vector3ToMatrix(
	D3DXMATRIX const & mtx
)
{
	return D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

// ----------------------------------------------------------------------------------------------------
// �����𑪂�(10�Ŋ�����@:�������Ȃ����ɗ��p)
// ----------------------------------------------------------------------------------------------------
unsigned CCalculation::GetDigit_Division(unsigned nNum)
{
	// �ϐ��錾
	unsigned digit = 0;	// ����
						// nNum��0�ɂȂ�܂Ń��[�v
	while (nNum != 0)
	{
		nNum /= 10;
		// �����A�b�v
		digit++;
	}
	// ������Ԃ�
	return digit;
}

// ----------------------------------------------------------------------------------------------------
// �����𑪂�(��p�ΐ����@:�����s����ő傫�����ɗ��p)
// ----------------------------------------------------------------------------------------------------
unsigned CCalculation::GetDigit_CommonLogarithm(unsigned nNum)
{
	return (int)log10(nNum) + 1;
}

// ----------------------------------------------------------------------------------------------------
// �����𑪂�(��������@:�R�[�f�B���O������)
// ----------------------------------------------------------------------------------------------------
unsigned CCalculation::GetDigit_Char(unsigned nNum)
{
	return std::to_string(nNum).length();
}

// ----------------------------------------------------------------------------------------------------
// �r���{�[�h�̐ݒ�
// ----------------------------------------------------------------------------------------------------
void CCalculation::SetBillboard(
	D3DXMATRIX *Originmtx,	// ���ɂȂ�s����
	D3DXMATRIX *Viewmtx		// �r���[�s����
)
{
	//�t�s��
	Originmtx->_11 = Viewmtx->_11;
	Originmtx->_12 = Viewmtx->_21;
	Originmtx->_13 = Viewmtx->_31;
	Originmtx->_21 = Viewmtx->_12;
	Originmtx->_22 = Viewmtx->_22;
	Originmtx->_23 = Viewmtx->_32;
	Originmtx->_31 = Viewmtx->_13;
	Originmtx->_32 = Viewmtx->_23;
	Originmtx->_33 = Viewmtx->_33;

}

// ----------------------------------------------------------------------------------------------------
// ��������(���ʂȂ� = ������1)
// ����1:A�̃X�s�[�h
// ����2:B�̃X�s�[�h
// ----------------------------------------------------------------------------------------------------
void CCalculation::CollisionAfter_VerticalComponent(
	float & fAfterSpeedA,	// �Փˌ�̃X�s�[�hA
	float & fAfterSpeedB,	// �Փˌ�̃X�s�[�hB
	float const & fSpeedA,	// �X�s�[�hA
	float const & fSpeedB,	// �X�s�[�hB
	float const & e			// �����W��
)
{
	// �Փˌ�̃X�s�[�hA�̌v�Z
	fAfterSpeedA = fSpeedA + fSpeedB - e * (fSpeedA - fSpeedB);
	// �Փˌ�̃X�s�[�hB�̌v�Z
	fAfterSpeedB = fSpeedA + fSpeedB + e * (fSpeedA - fSpeedB);
}


// ----------------------------------------------------------------------------------------------------
// �����m�̏Փˌ㑬�x�ʒu�Z�o
// ����1:�Փ˒��̋�A�̒��S�ʒu
// ����2:�Փ˂̏u�Ԃ̋�A�̑��x
// ����3:�Փ˒��̋�B�̒��S�ʒu
// ����4:�Փ˒��̋�B�̑��x
// ����5:��A�̎���
// ����6:��B�̎���
// ����7:��A�̔�����
// ����8:��B�̔�����
// ����9:��A�̔��ˌ�̑��x�x�N�g��
// ����10:��B�̔��ˌ�̑��x�x�N�g��
// ----------------------------------------------------------------------------------------------------
bool CCalculation::SquarColiAfterVec(
	D3DXVECTOR3 const & ColliPos_A,	// �Փ˒��̋�A�̒��S�ʒu
	D3DXVECTOR3 const & ColliVec_A,	// �Փ˂̏u�Ԃ̋�A�̑��x
	D3DXVECTOR3 const & ColliPos_B,	// �Փ˒��̋�B�̒��S�ʒu
	D3DXVECTOR3 const & ColliVec_B,	// �Փ˂̏u�Ԃ̋�B�̑��x
	float const & fWeight_A,		// ��A�̎���
	float const & fWeight_B,		// ��B�̎���
	float const & fRes_A,			// ��A�̔�����
	float const & fRes_B,			// ��B�̔�����
	D3DXVECTOR3 & pOut_Vec_A,		// ��A�̔��ˌ�̑��x�x�N�g��
	D3DXVECTOR3 & pOut_Vec_B		// ��B�̔��ˌ�̑��x�x�N�g��
)
{
	// �ϐ��錾
	D3DXVECTOR3 C;			// �Փˎ��x�N�g��
	D3DXVECTOR3 ConstVec;	// �萔�x�N�g��
	float TotalWeight;		// ���ʂ̍��v
	float RefRate;			// ������
	float Dot;				// ����

	// ���ʂ̍��v�v�Z
	TotalWeight = fWeight_A + fWeight_B;
	// ������
	RefRate = (1 + fRes_A * fRes_B);
	// �Փˎ��x�N�g��
	C = ColliPos_B - ColliPos_A;
	// �Փˎ��x�N�g���̐��K��
	D3DXVec3Normalize(&C, &C);
	// ����
	Dot = D3DXVec3Dot(&(ColliVec_A - ColliVec_B), &C);
	// �萔�x�N�g��
	ConstVec = RefRate * Dot / TotalWeight * C;
	// �Փˌ㑬�x�x�N�g��
	pOut_Vec_A = -fWeight_B * ConstVec + ColliVec_A;
	pOut_Vec_B = fWeight_A * ConstVec + ColliVec_B;
	if (pOut_Vec_A.x > 1000.0f ||
		pOut_Vec_A.y > 1000.0f ||
		pOut_Vec_A.z > 1000.0f)
	{
		return false;
	}
	return true;
}

// ----------------------------------------------------------------------------------------------------
// ���b�Z�[�W�\��
// ----------------------------------------------------------------------------------------------------
void CCalculation::Messanger(
	const char * cMessa,	// ���b�Z�[�W���e
	HWND hWnd				// �E�B���h�E���
)
{
	// �ϐ��錾
	MessageBox(hWnd, cMessa, "�x���I", MB_ICONWARNING);
	// �Q�[���I���̐ݒ�
	//SetDestWind(true);
}

// ----------------------------------------------------------------------------------------------------
// �L�[�����̔��f
// ----------------------------------------------------------------------------------------------------
void CCalculation::KeyString(
	int nKeyID,					// �L�[ID
	char *KeyName				// ����
)
{
	if (KeyName != NULL)
	{
		switch (nKeyID)
		{
		case DIK_0:
			strcpy(KeyName, "0");
			break;
		case DIK_1:
			strcpy(KeyName, "1");
			break;
		case DIK_2:
			strcpy(KeyName, "2");
			break;
		case DIK_3:
			strcpy(KeyName, "3");
			break;
		case DIK_4:
			strcpy(KeyName, "4");
			break;
		case DIK_5:
			strcpy(KeyName, "5");

			break;
		case DIK_6:
			strcpy(KeyName, "6");
			break;
		case DIK_7:
			strcpy(KeyName, "7");
			break;
		case DIK_8:
			strcpy(KeyName, "8");
			break;
		case DIK_9:
			strcpy(KeyName, "9");
			break;

		case DIK_Q:
			strcpy(KeyName, "Q");
			break;
		case DIK_W:
			strcpy(KeyName, "W");
			break;
		case DIK_E:
			strcpy(KeyName, "E");
			break;
		case DIK_R:
			strcpy(KeyName, "R");
			break;
		case DIK_T:
			strcpy(KeyName, "T");
			break;
		case DIK_Y:
			strcpy(KeyName, "Y");
			break;
		case DIK_U:
			strcpy(KeyName, "U");
			break;
		case DIK_I:
			strcpy(KeyName, "I");
			break;
		case DIK_O:
			strcpy(KeyName, "O");
			break;
		case DIK_P:
			strcpy(KeyName, "P");
			break;

		case DIK_A:
			strcpy(KeyName, "A");
			break;
		case DIK_S:
			strcpy(KeyName, "S");
			break;
		case DIK_D:
			strcpy(KeyName, "D");
			break;
		case DIK_F:
			strcpy(KeyName, "F");
			break;
		case DIK_G:
			strcpy(KeyName, "G");
			break;
		case DIK_H:
			strcpy(KeyName, "H");
			break;
		case DIK_J:
			strcpy(KeyName, "J");
			break;
		case DIK_K:
			strcpy(KeyName, "K");
			break;
		case DIK_L:
			strcpy(KeyName, "L");
			break;

		case DIK_Z:
			strcpy(KeyName, "Z");
			break;
		case DIK_X:
			strcpy(KeyName, "X");
			break;
		case DIK_C:
			strcpy(KeyName, "C");
			break;
		case DIK_V:
			strcpy(KeyName, "V");
			break;
		case DIK_B:
			strcpy(KeyName, "B");
			break;
		case DIK_N:
			strcpy(KeyName, "N");
			break;
		case DIK_M:
			strcpy(KeyName, "M");
			break;
		}
	}
}

// ------------------------------------------
// �X�N���v�g�����݂��邩����
// ------------------------------------------
bool CCalculation::Existenceofscript(FILE * pFile)
{
	// �ϐ��錾
	char cRead[128] = {};	// �ǂݍ��ݗp
	char cComp[128] = {};	// ��r�p
	int nCntError = 0;	// �G���[�p
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			return false;
		}
	}
	return true;
}

// ------------------------------------------
// �e�N�X�`���[�̃T�C�Y���擾(2D)
// ------------------------------------------
bool CCalculation::GetTexSize(
	LPDIRECT3DTEXTURE9 pTex,
	D3DXVECTOR2 * pSize
)
{
	// �e�N�X�`���[���ݒ肳��Ă��Ȃ���Ί֐��𔲂���
	if (pTex == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("���C�u����:�e�N�X�`���[�擾�ł��܂���ł����B");
#endif // _DEBUG
		return false;
	}
	// �e�N�X�`���T�C�Y�̎擾
	D3DSURFACE_DESC desc;
	if (pTex->GetLevelDesc(0, &desc) != S_OK)
	{
		return false;
	}
	// �e�N�X�`���[�̃T�C�Y���擾
	pSize->x = (float)desc.Width;
	pSize->y = (float)desc.Height;

	return true;
}

// ------------------------------------------
// �e�N�X�`���[�̃T�C�Y���擾(3D)
// ------------------------------------------
bool CCalculation::GetTexSize(
	LPDIRECT3DTEXTURE9 pTex,
	D3DXVECTOR3 * pSize
)
{
	// �e�N�X�`���T�C�Y�̎擾
	D3DSURFACE_DESC desc;

	if (pTex->GetLevelDesc(0, &desc) != S_OK)
	{
		return false;
	}
	// �e�N�X�`���[�̃T�C�Y���擾
	pSize->x = (float)desc.Width;
	pSize->y = (float)desc.Height;
	pSize->z = 0.0f;
	return true;
}

// ------------------------------------------
// ���������̋�؂育�ƂɎ擾����(1�s����)
// ------------------------------------------
std::vector<std::string> CCalculation::split(
	std::string& input,		// 1�s�̃X�g���[��
	char delimiter		// ��؂蕶��
)
{
	// �ϐ��錾
	std::istringstream iss_Line(input);	// ������X�g���[��
	std::string sRead;					// ������ǂݍ��ݗp
	std::vector<std::string> vec_Result;		// 1�s�ǂݍ��ݗp

	// �w�肵����؂肪������胋�[�v����
	while (std::getline(iss_Line, sRead, delimiter))
	{
		// �ǂݎ������������i�[����
		vec_Result.push_back(sRead);
	}
	// �m�ۂ��������񕪕Ԃ�
	return vec_Result;
}

// ------------------------------------------
// �t�@�C���̒��g���擾
// ------------------------------------------
std::vector<std::vector<std::string>> CCalculation::FileContens(
	char const * cns_cFile,
	char delimiter
)
{
	// �ϐ��錾
	std::ifstream				ifs_file;	// �t�@�C���p�X�g���[��
	std::string					s_Line;		// 1���I��1�s�ǂݍ���
	std::vector<std::vector<std::string>>	svec_Char;	// �t�@�C���̒��g�i�[�p

										// �t�@�C�����J��
	ifs_file.open(cns_cFile);
	// �ǂݍ��݂Ɏ��s������G���[�����o���֐��𔲂���
	if (ifs_file.fail())
	{
#ifdef _DEBUG
		Messanger("CCalculation::FileContens�̊֐��̃t�@�C���ǂݎ��G���[");
#endif // _DEBUG
		return svec_Char;
	}
	// �����񂪂Ȃ��Ȃ�܂Ń��[�v
	while (getline(ifs_file, s_Line))
	{
		// �P�s���ƕ�������擾����
		svec_Char.push_back(split(s_Line, delimiter));
	}
	// �t�@�C�������
	ifs_file.close();

	// �t�@�C���̒��g��Ԃ�
	return svec_Char;
}

//------------------------------------------------------------------------------
// 2D�̊O�όv�Z
//------------------------------------------------------------------------------
float CCalculation::Vec2Cross(D3DXVECTOR2 const & rVecA, D3DXVECTOR2 const & rVecB)
{
	return rVecA.x * rVecB.y - rVecB.x * rVecA.y;
}

//------------------------------------------------------------------------------
// �}�g���b�N�X�v�Z
//------------------------------------------------------------------------------
void CCalculation::CalcMatrix(D3DXMATRIX * pMtx, D3DXVECTOR3 const & rPos, D3DXVECTOR3 const & rRot)
{
	D3DXMATRIX	mtxRot, mtxTrans;			//�v�Z�p

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rRot.y, rRot.x, rRot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, rPos.x, rPos.y, rPos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);
}

//------------------------------------------------------------------------------
// �r���{�[�h�ݒ� X��Z�̂�
//------------------------------------------------------------------------------
void CCalculation::SetBillboard_XZ_Only(D3DXMATRIX * pMtx)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�ϐ�
	D3DXMATRIX mtxView;

	//�r���[�}�g���b�N�X������
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�r���{�[�h�ݒ�
	pMtx->_11 = mtxView._11;
	pMtx->_12 = mtxView._21;
	pMtx->_13 = mtxView._31;
	//pMtx->_21 = mtxView._12;
	//pMtx->_22 = mtxView._22;
	//pMtx->_23 = mtxView._32;
	pMtx->_31 = mtxView._13;
	pMtx->_32 = mtxView._23;
	pMtx->_33 = mtxView._33;
}

//------------------------------------------------------------------------------
// -3.14�`3.14��Ԃ�
//------------------------------------------------------------------------------
float CCalculation::Random_PI()
{
	return Random(3.14f);
}

//------------------------------------------------------------------------------
// ���͂��ꂽ�l��+-�����_���Ȓl�ŕԂ�
//------------------------------------------------------------------------------
float CCalculation::Random(float fInputValue)
{
	//0���Z�h�~
	if (fInputValue <= 0.0f)
	{
		return 0.0f;
	}

	int nValue = int(fInputValue * 100);

	float fValue;

	rand() % 2 ?
		fValue = float(rand() % nValue / 100.0f) :
		fValue = -float(rand() % nValue / 100.0f);

	return fValue;
}

//------------------------------------------------------------------------------
// �����_����vector3�^�Œl��Ԃ�
//------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::RandomVector3(float Max)
{
	D3DXVECTOR3 Value = D3DVECTOR3_ZERO;

	Value.x = Random(Max);
	Value.y = Random(Max);
	//Value.z = Random(Max);

	return Value;
}

//------------------------------------------------------------------------------
// ��]�̌v�Z�@360�x�ȓ��ɂ���
//------------------------------------------------------------------------------
void CCalculation::CalcRotation(float & fRot)
{
	//������3.14���ȏ�̎�
	if (fRot > D3DX_PI)
	{
		//6.28����
		fRot -= D3DX_PI * 2;
	}
	//������-3.14�ȉ��̎�
	if (fRot < -D3DX_PI)
	{
		//6.28���Z
		fRot += D3DX_PI * 2;
	}
}

//------------------------------------------------------------------------------
// ��]�̌v�Z�@360�x�ȓ��ɂ���
//------------------------------------------------------------------------------
void CCalculation::CalcRotation_XYZ(D3DXVECTOR3 & rot)
{
	CCalculation::CalcRotation(rot.x);
	CCalculation::CalcRotation(rot.y);
	CCalculation::CalcRotation(rot.z);
}
/*
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// XInput�̃p�b�h�p�֐�
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
//------------------------------------------------------------------------------
// �O��̃X�e�B�b�N����ۑ�
//------------------------------------------------------------------------------
void CCalculation::SaveLastStickInfo()
{
	CXInputPad *InpudPad[(int)PLAYER_TAG::PLAYER_MAX] = {};

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		InpudPad[nCnt] =  CManager::GetPad((PLAYER_TAG)nCnt);

		if (InpudPad[nCnt])
		{
			// �㉺�̓��͔���
			//if (fabsf(m_PadStick[nCnt].fLeftStickValue_Y / STICK_MAX_RANGE) > 0.8f)
			//{
			//	m_PadStick[nCnt].bLeftStickDown_Y = true;
			//}
			//else
			//{
			//	m_PadStick[nCnt].bLeftStickDown_Y = false;
			//}

			//if (fabsf(m_PadStick[nCnt].fLeftStickValue_X / STICK_MAX_RANGE) > 0.8f)
			//{
			//	m_PadStick[nCnt].bLeftStickDown_X = true;
			//}
			//else
			//{
			//	m_PadStick[nCnt].bLeftStickDown_X = false;
			//}

			// ���X�e�B�b�N�̓��͎擾
			InpudPad[nCnt]->GetStickLeft(&m_PadStick[nCnt].fLeftStickValue_X, &m_PadStick[nCnt].fLeftStickValue_Y);
		}
	}
}

//------------------------------------------------------------------------------
// �X�e�B�b�N��|���Ă���������`�F�b�N
//------------------------------------------------------------------------------
DIRECTION CCalculation::CheckPadStick()
{
	// �ϐ�
	CKeyboard	*Keyboard								= CManager::GetKeyboard();
	CXInputPad	*InpudPad[(int)PLAYER_TAG::PLAYER_MAX]	= {};
	DIRECTION	Direction								= (DIRECTION)-1;

	// �Q�[���p�b�h
	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		InpudPad[nCnt] = CManager::GetPad((PLAYER_TAG)nCnt);

		if (InpudPad[nCnt])
		{
			// ��
			if (InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_UP, 1) || ((m_PadStick[nCnt].fLeftStickValue_Y / STICK_MAX_RANGE) > 0.8f && m_PadStick[nCnt].bLeftStickDown_Y == false))
			{
				Direction = DIRECTION::UP;
				return Direction;
			}
			// ��
			if (InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_DOWN, 1) || ((m_PadStick[nCnt].fLeftStickValue_Y / STICK_MAX_RANGE) <= -0.8f && m_PadStick[nCnt].bLeftStickDown_Y == false))
			{
				Direction = DIRECTION::DOWN;
				return Direction;
			}
			// ��
			if (InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_LEFT, 1) || ((m_PadStick[nCnt].fLeftStickValue_X / STICK_MAX_RANGE) < -0.8f && m_PadStick[nCnt].bLeftStickDown_X == false))
			{
				Direction = DIRECTION::LEFT;
				return Direction;
			}
			// �E
			if (InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_RIGHT, 1) || ((m_PadStick[nCnt].fLeftStickValue_X / STICK_MAX_RANGE) >= +0.8f && m_PadStick[nCnt].bLeftStickDown_X == false))
			{
				Direction = DIRECTION::RIGHT;
				return Direction;
			}

		}
	}
	// �L�[�{�[�h
	// ��
	if (Keyboard->GetKeyboardTrigger(DIK_UP) || Keyboard->GetKeyboardTrigger(DIK_W))
	{
		Direction = DIRECTION::UP;
		return Direction;
	}
	// ��
	if (Keyboard->GetKeyboardTrigger(DIK_DOWN) || Keyboard->GetKeyboardTrigger(DIK_S))
	{
		Direction = DIRECTION::DOWN;
		return Direction;
	}
	return Direction;
}

//------------------------------------------------------------------------------
// ��������L�[���������Ƃ�
//------------------------------------------------------------------------------
bool CCalculation::PressAnyButton(void)
{
	// �ϐ�
	CKeyboard *Keyboard									= CManager::GetKeyboard();
	CXInputPad *InpudPad[(int)PLAYER_TAG::PLAYER_MAX]	= {};

	for (int nCnt = 0; nCnt < (int)PLAYER_TAG::PLAYER_MAX; nCnt++)
	{
		InpudPad[nCnt] = CManager::GetPad((PLAYER_TAG)nCnt);

		if (Keyboard->GetKeyboardTrigger(DIK_RETURN) ||
			Keyboard->GetKeyboardTrigger(DIK_SPACE) ||
			InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_START, 1) ||
			InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_A, 1) ||
			InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_B, 1) ||
			InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_X, 1) ||
			InpudPad[nCnt]->GetTrigger(CXInputPad::JOYPADKEY_Y, 1))
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// �p�b�h�̓��͏���
//------------------------------------------------------------------------------
bool CCalculation::PadMoveInput(D3DXVECTOR3 & rMove, DIRECTION & direction, bool bJump, PLAYER_TAG PlayerTag)
{
	// �ϐ�
	bool bInput = false;
	D3DXVECTOR3 InputValue = D3DVECTOR3_ZERO;
	InputValue.x /= STICK_MAX_RANGE;//�l�̐��K��
	InputValue.y /= STICK_MAX_RANGE;//�l�̐��K��
	CXInputPad *pad = CManager::GetPad(PlayerTag);	// �p�b�h�̃|�C���^
	pad->GetStickLeft(&InputValue.x, &InputValue.y);//�p�b�h�̓��͒l����

	//�E
	if (InputValue.x > 0.5f)
	{
		rMove = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		direction = DIRECTION::RIGHT;
		bInput = true;

	}
	//��
	else if (InputValue.x < -0.5f)
	{
		rMove = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
		direction = DIRECTION::LEFT;
		bInput = true;

	}
	//��
	if (InputValue.y > 0.6f)
	{
		direction = DIRECTION::UP;

	}
	//��
	else if (InputValue.y < -0.6f && bJump == false)
	{
		direction = DIRECTION::DOWN;
	}
	return bInput;
}

/*
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ImGui�p�֐�
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O���̏���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCalculation::ImG_DebugInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (ImGui::Checkbox("WireFrame", &m_bWire))
	{
		m_bWire ?
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME) :
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���񏈗�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCalculation::ImG_Parallel(void)
{
	// ���s
	ImGui::SameLine();
	// ���ɂ����
	ImGui::Dummy(ImVec2(10.0f, 0.0f));
	// ���s
	ImGui::SameLine();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ImGui�̃R���{�{�b�N�X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCalculation::ImGui_Combobox(std::vector<std::string> aItemNameList, std::string aTitle, int & nValue)
{
	bool bChange = false;

#ifdef _DEBUG
	//combo�J�n
	if (ImGui::BeginCombo(aTitle.data(), aItemNameList[nValue].data()))
	{
		//�v�f���J��Ԃ�
		for (size_t nCnt = 0; nCnt < aItemNameList.size(); nCnt++)
		{

			//�I��ԍ��������Ă邩�ǂ���
			bool is_selected = (aItemNameList[nValue] == aItemNameList[nCnt]);

			//�I�����ꂽ���̏���
			if (ImGui::Selectable(aItemNameList[nCnt].data(), is_selected))
			{
				//���݂̑I�����ڐݒ�
				nValue = nCnt;
				bChange = true;
			}
		}
		//combo�I��
		ImGui::EndCombo();

	}
#endif //DEBUG
	return bChange;
}




void MYLIBLARY::in(void)
{

}
