// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�V�[�������̐���[scene_three.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_three.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE::CScene_THREE() : CScene::CScene()
{
	// �ϐ��̏�����
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR_INI;
	m_fLengh = 0;
	m_fAngle = 0;
	m_bBillboard = false;
	m_bZEneble = false;
	m_bLighting = false;
	m_bAlpha = false;
	m_offsetType = OFFSET_TYPE_VERTICAL_CENTER;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE::~CScene_THREE()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Init(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	D3DXVECTOR3 Cross0, Cross1_2, Cross3;	// �O��
	int nCntBlock = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// ���_���W�̐ݒ�
	Set_Vtx_Pos(m_offsetType,pVtx);

	// �x�N�g��
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pVtx[2].pos - pVtx[0].pos;
	// �O�όv�Z
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// �x�N�g��
	VecA = pVtx[2].pos - pVtx[3].pos;
	VecB = pVtx[1].pos - pVtx[3].pos;
	// �O�όv�Z
	Cross3 = CCalculation::Cross_product(VecA, VecB);
	// ���K��
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross3, &Cross3);
	// �O�όv�Z
	Cross1_2 = (Cross0 + Cross3) / 2;

	pVtx[0].nor = Cross0;
	pVtx[1].nor = Cross1_2;
	pVtx[2].nor = Cross1_2;
	pVtx[3].nor = Cross3;

	// ���_�J���[�̐ݒ�
	Set_Vtx_Col(pVtx);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
 		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Update(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ��]�E�ʒu�E�r���[�s��̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans, mtxView;

	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���{�[�h�ݒ�
	if (m_bBillboard)
	{
		// �}�g���b�N�X�r���[�̎擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		// �r���{�[�h
		CCalculation::SetBillboard(
			&m_mtxWorld,
			&mtxView
		);
	}
	// Z���̐ݒ�
	if (m_bZEneble)
	{
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);		// �A���t�@�e�X�g�L��
	}
	// ���C�e�B���O�ݒ�
	if (m_bLighting)
	{
		// ���C�e�B���O���[�h����
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
	// ���u�����h�ݒ�
	if (m_bAlpha)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L��
		pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)
	}

	// ��]�𔽉f //
	// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
		m_rot.y, m_rot.x, m_rot.z);	// ���ꂼ��̉�]��

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxRot);					// 3


	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
		m_pos.x, m_pos.y, m_pos.z);	// ���ꂼ��̈ړ���

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexType)
	);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
		);
	CManager::GetRenderer()->SetType(CRenderer::TYPE_NORMAL);
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CScene_THREE::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �V�[���Ǘ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE * CScene_THREE::Create(
	OFFSET_TYPE	const & type,			// �^�C�v
	D3DXVECTOR3 const & pos,			// �ʒu
	D3DXVECTOR3 const & size,			// �T�C�Y
	int			const & nTexType,		// �e�N�X�`���[�^�C�v
	D3DXVECTOR3	const & rot,			// �p�x
	bool		const & bBillboard,		// �r���{�[�h
	bool		const & bZEneble,		// Z�o�b�t�@
	bool		const & bLighting,		// ���C�e�B���O
	bool		const & bAlpha,			// �A���t�@�u�����h
	D3DXCOLOR	const & col				// �F
)
{
	// �ϐ��錾
	CScene_THREE * pScene_Three;		// �V�[��3D�N���X

	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_Three = new CScene_THREE();
	// �V�[���Ǘ��ݒ�
	pScene_Three->ManageSetting(CScene::LAYER_3DOBJECT);
	// �ݒ�
	pScene_Three->m_offsetType = type;			// �^�C�v
	pScene_Three->m_pos = pos;					// �ʒu
	pScene_Three->m_size = size;				// �T�C�Y
	pScene_Three->m_rot = rot;					// �p�x
	pScene_Three->m_col = col;					// �J���[
	pScene_Three->m_nTexType = nTexType;		// �e�N�X�`���[�^�C�v
	pScene_Three->m_bBillboard = bBillboard;	// �r���{�[�h
	pScene_Three->m_bZEneble = bZEneble;		// Z�o�b�t�@
	pScene_Three->m_bLighting = bLighting;		// ���C�e�B���O
	pScene_Three->m_bAlpha = bAlpha;			// �A���t�@�u�����h
	// ����������
	pScene_Three->Init();

	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Three;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �l�Ǘ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE * CScene_THREE::Create_Self(
	OFFSET_TYPE	const & type,			// �^�C�v
	D3DXVECTOR3 const & pos,			// �ʒu
	D3DXVECTOR3 const & size,			// �T�C�Y
	int			const & nTexType,		// �e�N�X�`���[�^�C�v
	D3DXVECTOR3	const & rot,			// �p�x
	bool		const & bBillboard,		// �r���{�[�h
	bool		const & bZEneble,		// Z�o�b�t�@
	bool		const & bLighting,		// ���C�e�B���O
	bool		const & bAlpha,			// �A���t�@�u�����h
	D3DXCOLOR	const & col				// �F
)
{
	// �ϐ��錾
	CScene_THREE * pScene_Three;		// �V�[��3D�N���X

	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_Three = new CScene_THREE();
	// �ݒ�
	pScene_Three->m_offsetType = type;			// �^�C�v
	pScene_Three->m_pos = pos;					// �ʒu
	pScene_Three->m_size = size;				// �T�C�Y
	pScene_Three->m_rot = rot;					// �p�x
	pScene_Three->m_col = col;					// �J���[
	pScene_Three->m_nTexType = nTexType;		// �e�N�X�`���[�^�C�v
	pScene_Three->m_bBillboard = bBillboard;	// �r���{�[�h
	pScene_Three->m_bZEneble = bZEneble;		// Z�o�b�t�@
	pScene_Three->m_bLighting = bLighting;		// ���C�e�B���O
	pScene_Three->m_bAlpha = bAlpha;			// �A���t�@�u�����h
												// ����������
	pScene_Three->Init();

	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Three;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �l�Ǘ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CScene_THREE> CScene_THREE::Create_Uni(
	OFFSET_TYPE	const & type,			// �^�C�v
	D3DXVECTOR3 const & pos,			// �ʒu
	D3DXVECTOR3 const & size,			// �T�C�Y
	int			const & nTexType,		// �e�N�X�`���[�^�C�v
	D3DXVECTOR3	const & rot,			// �p�x
	bool		const & bBillboard,		// �r���{�[�h
	bool		const & bZEneble,		// Z�o�b�t�@
	bool		const & bLighting,		// ���C�e�B���O
	bool		const & bAlpha,			// �A���t�@�u�����h
	D3DXCOLOR	const & col				// �F
)
{
	// �ϐ��錾
	std::unique_ptr<CScene_THREE> pScene_Three(new CScene_THREE);		// �V�[��3D�N���X
	// �ݒ�
	pScene_Three->m_offsetType = type;			// �^�C�v
	pScene_Three->m_pos = pos;					// �ʒu
	pScene_Three->m_size = size;				// �T�C�Y
	pScene_Three->m_rot = rot;					// �p�x
	pScene_Three->m_col = col;					// �J���[
	pScene_Three->m_nTexType = nTexType;		// �e�N�X�`���[�^�C�v
	pScene_Three->m_bBillboard = bBillboard;	// �r���{�[�h
	pScene_Three->m_bZEneble = bZEneble;		// Z�o�b�t�@
	pScene_Three->m_bLighting = bLighting;		// ���C�e�B���O
	pScene_Three->m_bAlpha = bAlpha;			// �A���t�@�u�����h

		// ����������
	pScene_Three->Init();

	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Three;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ڂ��Ă���ʂ̍������擾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CScene_THREE::GetHeight(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;						// ���_���ւ̃|�C���g
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	D3DXVECTOR3 Cross0, Cross1, Cross2;	// �O��
	D3DXVECTOR3 CrossPolygon;			// �|���S���̖@��
	D3DXVECTOR3 CrossAns;				// �ʒu�̊O��

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// �x�N�g��
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pos - pVtx[0].pos;
	// �O�όv�Z
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// �x�N�g��
	VecA = pVtx[2].pos - pVtx[1].pos;
	VecB = pos - pVtx[1].pos;
	// �O�όv�Z
	Cross1 = CCalculation::Cross_product(VecA, VecB);
	// �x�N�g��
	VecA = pVtx[0].pos - pVtx[2].pos;

	VecB = pos - pVtx[2].pos;
	// �O�όv�Z
	Cross2 = CCalculation::Cross_product(VecA, VecB);
	// �|���S���̖@��
	CrossPolygon = pVtx[0].nor;
	VecA = pos - pVtx[0].pos;

	CrossAns = CCalculation::Cross_product(VecA, CrossPolygon);
	// ���K��
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross1, &Cross1);
	D3DXVec3Normalize(&Cross2, &Cross2);
	D3DXVec3Normalize(&CrossAns, &CrossAns);
	if (Cross0.y > 0 && Cross1.y > 0 && Cross2.y > 0)
	{
		// �|���S���̖@��
		D3DXVECTOR3 VecAB, VecBC;
		VecA = pos - (m_pos + pVtx[0].pos);
		//D3DXVec3Normalize(&VecA, &VecA);

		float Dot = 0;				// ����
		float fY = 0;				// ����
		// ����
		Dot = CCalculation::Dot_product(VecA, CrossPolygon);
		// �|���S���̎w��ʒu�̍����v�Z
		CrossPolygon = pVtx[0].nor;
		fY = pVtx[0].pos.y + ((-CrossPolygon.x * VecA.x) + (-CrossPolygon.z * VecA.z)) / CrossPolygon.y;
		// ���_�f�[�^�͈̔͂��A�����b�N
		m_pVtxBuff->Unlock();
		return fY;
	}
	// ���_�f�[�^�͈̔͂��A�����b�N
	m_pVtxBuff->Unlock();
	return 0.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_���W�̐ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Set_Vtx_Pos(
	OFFSET_TYPE const & offsettype,	// �I�t�Z�b�g�^�C�v
	VERTEX_3D * pVtx				// ���_���
)
{
	// ���_��񂪃k����������
	if (pVtx == NULL)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
	}
	// ���_���W�̃I�t�Z�b�g�I��
	switch (offsettype)
	{
		// �c�̃Z���^�[
	case OFFSET_TYPE_VERTICAL_CENTER:
		Offset_Virtical_Center(pVtx);
		break;
		// �c�̍�
	case OFFSET_TYPE_VERTICAL_LEFT:
		Offset_Virtical_Left(pVtx);
		break;
		// �c�̉�
	case OFFSET_TYPE_VERTICAL_UNDER:
		Offset_Virtical_Under(pVtx);
		break;
		// ���̒��S
	case OFFSET_TYPE_SIDE_CENTER:
		Offset_Side_Center(pVtx);
		break;
	default:
		break;
	}
	// ���_��񂪃k����������
	if (pVtx == NULL)
	{
		// �A�����b�N
		m_pVtxBuff->Unlock();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�J���[�ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Set_Vtx_Col(VERTEX_3D * pVtx)
{
	// ���_��񂪃k����������
	if (pVtx == NULL)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
	}
	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// ���_��񂪃k����������
	if (pVtx == NULL)
	{
		// �A�����b�N
		m_pVtxBuff->Unlock();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\���e�N�X�`���[�ݒ菈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::SetTex(
	D3DXVECTOR2 const &first,	// �ŒZ�l
	D3DXVECTOR2 const &last		// �Œ��l
)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// ���_�e�N�X�`���[
	pVtx[0].tex = D3DXVECTOR2(first.x, first.y);
	pVtx[1].tex = D3DXVECTOR2(last.x, first.y);
	pVtx[2].tex = D3DXVECTOR2(first.x, last.y);
	pVtx[3].tex = D3DXVECTOR2(last.x, last.y);
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g�c�̃Z���^�[����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Virtical_Center(VERTEX_3D * pVtx)
{
	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,m_size.y * 0.5f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g�c�̍�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Virtical_Left(VERTEX_3D * pVtx)
{
	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_size.y * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, -m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y * 0.5f, 0.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g�c�̉�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Virtical_Under(VERTEX_3D * pVtx)
{
	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f,0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, 0.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g���̒��S����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Side_Center(VERTEX_3D * pVtx)
{
	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f,0.0f , -m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f,0.0f, -m_size.z * 0.5f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �A�j���[�V��������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::SetAnimation(float fTexX, float fTexY, float fTexY2, int nPatternAnim)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY2);
	pVtx[1].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY2);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY);
	pVtx[3].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY);

	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

