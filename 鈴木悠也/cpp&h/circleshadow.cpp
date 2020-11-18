// ----------------------------------------
//
// �~�`�L�����N�^�[�����̐���[circleshadow.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "circleshadow.h"
#include "character.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define CIRCLESHADOWCOLLISION_SIZE (30.0f)

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------
CCircleshadow::CCircleshadow() : CScene_THREE::CScene_THREE()
{
	m_bShadow = true;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CCircleshadow::~CCircleshadow()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CCircleshadow::Init(void)
{
	// �I�t�Z�b�g�^�C�v�ݒ�
	CScene_THREE::SetOffsetType(OFFSET_TYPE_SIDE_CENTER);
	// �F�̐ݒ�
	CScene_THREE::SetCol(D3DXCOLOR_CA(1.0f,1.0f));
	// �V�[��3D�̏�����
	CScene_THREE::Init();
	//CScene_THREE::SetLighting(true);		// ���C�e�B���O
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CCircleshadow::Uninit(void)
{
	CScene_THREE::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CCircleshadow::Update(void)
{
	CScene_THREE::Update();
#ifdef _DEBUG
	D3DXVECTOR3 pos = CScene_THREE::GetPos();
	/*
	CDebugproc::Print("�e�̏ꏊ:(%.1f,%.1f,%.1f)\n", pos.x, pos.y, pos.z);
	*/
#endif // _DEBUG

}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CCircleshadow::Draw(void)
{
	// �V���h�E��Ԃ�false�Ȃ�
	// ->�֐��𔲂���
	if (!m_bShadow) return;
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	/*
	// ���Z
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	*/
	// �`��
	CScene_THREE::Draw();
	/*
	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	*/
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CCircleshadow * CCircleshadow::Create(
	D3DXVECTOR3 const &	pos,						// �ʒu
	D3DXVECTOR3 const &	size,						// �T�C�Y
	int const &			nTexType					// �e�N�X�`���[�^�C�v
)
{
	// �ϐ��錾
	CCircleshadow * pCircleshadow;		// �V�[��3D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pCircleshadow = new CCircleshadow();
	// �V�[���}�l�[�W���ݒ�
	pCircleshadow->ManageSetting(CScene::LAYER_3DBLOCKSHADOW);
	// �ʒu�ݒ�
	pCircleshadow->SetPos(pos);
	// �T�C�Y�ݒ�
	pCircleshadow->SetSize(D3DXVECTOR3(size.x,0.0f,size.z));
	// �e�N�X�`���[�^�C�v�ݒ�
	pCircleshadow->SetTexType(nTexType);
	// ����������
	pCircleshadow->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pCircleshadow;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CCircleshadow::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// unload����
// ----------------------------------------
void CCircleshadow::UnLoad(void)
{
}