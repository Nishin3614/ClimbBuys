// ----------------------------------------
//
// �f�o�b�O�p�̓����蔻��\�������̐���[debugcollision.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "debugcollision.h"
#include "3Dline.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ----------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CDebugcollision::CDebugcollision()
{
	m_ppos = NULL;
	m_size = NULL;
	m_type = COLLISIONTYPE_RECT;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CDebugcollision::~CDebugcollision()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CDebugcollision::Init(void)
{
	switch (m_type)
	{
	case COLLISIONTYPE_RECT:
		break;
	default:
		break;
	}
}

// ----------------------------------------
// ��`�̓����蔻���������
// ----------------------------------------
void CDebugcollision::Create_Rect(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 const &size
)
{
	// ��邱��
	// ����̂�ƃR�l�N�g������





	// 1��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f)
	);
	// 2��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f)
	);
	// 3��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f)
	);
	// 4��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f)
	);
	// 5��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
	// 6��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
	// 7��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 8��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 9��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
	// 10��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 11��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 12��
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
}

// ----------------------------------------
// ���̓����蔻���������
// ----------------------------------------
void CDebugcollision::Create_Sphere(
	D3DXVECTOR3 * pos,
	float const fRadius
)
{


}