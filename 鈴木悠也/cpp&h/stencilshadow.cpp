// ----------------------------------------
//
// �X�e���V���V���h�E�����̐���[stencilshadow.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "stencilshadow.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "scene_two.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define STENCILSHADOW_ROTMOVE (0.1f)				// �p�x�̈ړ���
#define STENCILSHADOW_SIZE (100.0f)					// �T�C�Y

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
// �e�N�X�`���[ID
int CStencilshadow::m_nTexId[TYPE_MAX] =
{
	0,
};
CScene_TWO * CStencilshadow::m_pShadow = NULL;	// �V���h�E2D

#ifdef _DEBUG
bool	CStencilshadow::m_bStencil = true;								// �X�e���V���`�悷�邩���Ȃ���
#endif // _DEBUG


// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CStencilshadow::CStencilshadow() : CScene()
{
	/* �ϐ��̏����� */
	// ��]��
	m_pVtxBuff = NULL;
	m_pIndex = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nNumberVertex = 4;
	m_nNumIndex = 4;
	m_nNumPolygon = 2;
	m_nBlock_Width = 1;
	m_nBlock_Depth = 1;
	m_bUse = true;
	m_type = TYPE_CYLINDER;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CStencilshadow::~CStencilshadow()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CStencilshadow::Init(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =				// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	D3DXVECTOR3 Cross[2000];				// �O��
	// ���_���W�̐���
	// MakeVertex(pDevice);
	//
	switch (m_type)
	{
		// �~��
	case TYPE_CYLINDER:
		SetCylinder(pDevice);
		break;
		// ��`
	case TYPE_RECT:
		SetRect(pDevice);
		break;
	default:
		break;
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CStencilshadow::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CStencilshadow::Update(void)
{

}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CStencilshadow::Draw(void)
{
	if (!m_bUse) return;
	// �`����
	CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLBACK);
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f //
	// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,	// �����̓��ꕨ
		m_rot.y, m_rot.x, m_rot.z);			// ���ꂼ��̉�]��
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxRot);					// 3
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,// �����̓��ꕨ
		m_pos.x, m_pos.y, m_pos.z);	// ���ꂼ��̈ړ���
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
	pDevice->SetIndices(
		m_pIndex);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

#ifdef _DEBUG
	// �X�e���V���`�悪false�Ȃ�
	// ->�ʏ�`��
	if (!m_bStencil)
	{
		CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLNORMAL);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,
			0,
			m_nNumberVertex,		//�g�p���钸�_�� �O�p�|���S���̒��_
			0,						//���_�̓ǂݎ����J�n����ʒu
			m_nNumPolygon);			//�|���S���̖���

					// �J�����O���Ȃ�
		CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLNORMAL);

		//Z�e�X�g�ʏ�
		//CManager::GetRenderer()->SetType(CRenderer::TYPE_ZBUFFOFF);

		//�ʏ퍇��
		CManager::GetRenderer()->SetType(CRenderer::TYPE_NORMALMIX);

		//���C�e�B���OON
		CManager::GetRenderer()->SetType(CRenderer::TYPE_RIGHTINGON);
	}
	// ����ȊO�Ȃ�
	// ->�X�e���V���`�悷��
	else
#endif // _DEBUG
	{
		// Z�o�b�t�@�̏������݂𖳌���
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		// �X�e���V���o�b�t�@��L���ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
		// �J���[�`��𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);
		// �X�e���V���Ώۂ�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		// �X�e���V���e�X�g�AZ�e�X�g�����Ƃ����i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
		// �X�e���V���e�X�g���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
		// �X�e���V���e�X�g�s���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
		// ���ʃJ�����O(���̃J�����O)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			m_nNumberVertex,
			0,
			m_nNumPolygon);
		// �X�e���V���Q�ƒl�̐ݒ�
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);
		// �X�e���V���Ώۂ�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		// �X�e���V���e�X�g�AZ�e�X�g�����Ƃ����i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
		// �X�e���V���e�X�g���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
		// �X�e���V���e�X�g�s���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
		// ���v�J�����O(���̃J�����O)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			m_nNumberVertex,
			0,
			m_nNumPolygon);
		// �J���[�`���L���ɂ���
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
		// �X�e���V���Q�ƒl�̐ݒ�
		pDevice->SetRenderState(D3DRS_STENCILREF, 2);
		// �X�e���V���Ώۂ�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		// �X�e���V���e�X�g�AZ�e�X�g�����Ƃ����i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		// �X�e���V���e�X�g���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		// �X�e���V���e�X�g�s���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		// �����v�J�����O(���̃J�����O)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// �V���h�E2D�����݂��Ă�����
		if (m_pShadow)
		{
			// �V���h�E2D�̊J��
			m_pShadow->Draw();
		}
		// �X�e���V���o�b�t�@�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
		// �����v�J�����O(���̃J�����O)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// �J���[�`���L���ɂ���
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
		// Z��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	}
}

#ifdef _DEBUG
// ----------------------------------------
// �f�o�b�O�\������
// ----------------------------------------
void CStencilshadow::Debug(void)
{
}
#endif // _DEBUG
// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CStencilshadow::Load(void)
{
	// �V���h�E2D�����݂��Ă�����
	if (!m_pShadow)
	{
		m_pShadow = CScene_TWO::Create_Self(
			CScene_TWO::OFFSET_TYPE_CENTER,
			{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f },
			{ SCREEN_WIDTH,SCREEN_HEIGHT },
			-1,
			0.0f,
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f)
		);
	}
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CStencilshadow::UnLoad(void)
{
	// �V���h�E2D�����݂��Ă�����
	if (m_pShadow)
	{
		// �V���h�E2D�̊J��
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = NULL;
	}
}

// ----------------------------------------
// �쐬����(�V�[���Ǘ�)
// ----------------------------------------
CStencilshadow * CStencilshadow::Create(
	D3DXVECTOR3 const &		pos,	// �ʒu
	D3DXVECTOR3 const &		size,	// �T�C�Y
	TYPE const &			type,	// �^�C�v
	CScene::LAYER const &	layer,	// ���C���[
	int const &				nWidth,	// ����
	int const &				nDepth,	// �c��
	D3DXCOLOR	const &		col,	// �J���[
	D3DXVECTOR3 const &		rot		// ��]
)
{
	// �ϐ��錾
	CStencilshadow * pStencilshadow;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pStencilshadow = new CStencilshadow();
	// �V�[���Ǘ��ݒ�
	pStencilshadow->ManageSetting(layer);
	// �ʒu���
	pStencilshadow->m_pos = pos;
	// �T�C�Y���
	pStencilshadow->m_size = size;
	// ��]���
	pStencilshadow->m_rot = rot;
	// �F���
	pStencilshadow->m_col = D3DXCOLOR(0.0f,0.0f,0.0f,0.5f);
	// ���u���b�N
	pStencilshadow->m_nBlock_Width = nWidth;
	// �c�u���b�N
	pStencilshadow->m_nBlock_Depth = nDepth;
	// �^�C�v
	pStencilshadow->m_type = type;
	// ����������
	pStencilshadow->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pStencilshadow;
}

// ----------------------------------------
// �쐬����(�l�Ǘ�)
// ----------------------------------------
CStencilshadow * CStencilshadow::Create_Self(
	D3DXVECTOR3 const &pos,		// �ʒu
	D3DXVECTOR3 const &size,	// �T�C�Y
	TYPE const &type,			// �^�C�v
	int const &nWidth,			// ����
	int const &nDepth,			// �c��
	D3DXCOLOR	const &col,		// �J���[
	D3DXVECTOR3 const &rot		// ��]
)
{
	// �ϐ��錾
	CStencilshadow * pStencilshadow;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pStencilshadow = new CStencilshadow();
	// �ʒu���
	pStencilshadow->m_pos = pos;
	// �T�C�Y���
	pStencilshadow->m_size = size;
	// ��]���
	pStencilshadow->m_rot = rot;
	// �F���
	pStencilshadow->m_col = col;
	// ���u���b�N
	pStencilshadow->m_nBlock_Width = nWidth;
	// �c�u���b�N
	pStencilshadow->m_nBlock_Depth = nDepth;
	// �^�C�v
	pStencilshadow->m_type = type;
	// ����������
	pStencilshadow->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pStencilshadow;
}

// ----------------------------------------
// �ʒu�ݒ�
// ----------------------------------------
void CStencilshadow::SetPos(D3DXVECTOR3 const & pos)
{
	m_pos = pos;
}

// ----------------------------------------
// �T�C�Y�ݒ�
// ----------------------------------------
void CStencilshadow::SetSize(D3DXVECTOR3 const & size)
{
	// �T�C�Y�ݒ�
	m_size = size;
	// ���_���W�̐ݒ�
	// �^�C�v���~���Ȃ�
	if (m_type == TYPE_CYLINDER)
	{
		SetCylinderVtxPos();
	}
	// �^�C�v����`�Ȃ�
	else if (m_type == TYPE_RECT)
	{
		SetRectVtxPos();
	}
}

// ----------------------------------------
// ��]�ݒ�
// ----------------------------------------
void CStencilshadow::SetRot(D3DXVECTOR3 const & rot)
{
	m_rot = rot;
}

// ----------------------------------------
// �F�ݒ�
// ----------------------------------------
void CStencilshadow::SetCol(D3DXCOLOR const & col)
{
	// �F�ݒ�
	m_col = col;
	// �V���h�E2D�����݂��Ă�����
	if (m_pShadow)
	{
		// �V���h�E2D�̊J��
		m_pShadow->SetCol(m_col);
		m_pShadow->Set_Vtx_Col();
	}
	// // �V�[��2D���NULL�`�F�b�N
	// if (m_pSceneTwo)
	// {
	// 	// �F�̐ݒ�
	// 	m_pSceneTwo->SetCol(m_col);
	// 	m_pSceneTwo->Set_Vtx_Col();
	// }
}

// ----------------------------------------
// �g�p���
// ----------------------------------------
void CStencilshadow::SetUse(bool const bUse)
{
	m_bUse = bUse;
}

// ----------------------------------------
// �~���̐ݒ�
// ----------------------------------------
void CStencilshadow::SetCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���g
	WORD * pIdx;				// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	D3DXVECTOR3 VecA, VecB;		// �x�N�g��
	float fOriginY;				// y�̈ʒu
	int nCountDirect;			// �c�̃J�E���g
	int nCountWidth;			// ���̃J�E���g
	float fAngle;				// y�̊p�x
	float fRadian;				// y�̃��W�A���l
	D3DXVECTOR3 *pCross;		// �|���S���̊O��
	pCross =					// �������m��
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	// �u���b�N�`��̌��_�̏����ݒ�
	fOriginY = -m_size.y * m_nBlock_Depth;
	// �����_���E�C���f�b�N�X�E�|���S���̏����ݒ�(�v�Z)
	m_nNumberVertex =
		(m_nBlock_Depth + 1) * (m_nBlock_Width + 1);
	m_nNumIndex =
		(m_nBlock_Width + 1) * 2 * m_nBlock_Depth +
		2 * (m_nBlock_Depth - 1);
	m_nNumPolygon =
		m_nBlock_Depth * m_nBlock_Width * 2 +
		4 * (m_nBlock_Depth - 1);
	// �p�x�̌v�Z
	fAngle = D3DX_PI * 2 / m_nBlock_Width;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//���_�ݒ� //
	//�s���[�v
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ���W�A���l
			fRadian = fAngle * nCountWidth;
			fRadian = CCalculation::Rot_One_Limit(fRadian);
			// ���_���W�̐ݒ�
			pVtx[0].pos =
				D3DXVECTOR3(
				(sinf(fRadian) * m_size.x),
					fOriginY + m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �J���[�̐ݒ�
			pVtx[0].col = m_col;

			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(
				1.0f,
				1.0f
			);

			// �|�C���g���킹
			pVtx++;
		}
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pIndex->Lock(0, 0, (void **)&pIdx, 0);

	// �c�u���b�N��
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth; nCountDirect++)
	{
		// �Q��ڂ̃��[�v�ȍ~
		if (nCountDirect >= 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width + 1;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}

		// ���u���b�N�̒��_��
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// �`�揇�Ԃ̃C���f�b�N�X
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth + m_nBlock_Width + 1;
			pIdx[1] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx += 2;
		}

		// �k�ރ|���S�������K�v������ꍇ
		if (nCountDirect < m_nBlock_Depth - 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width;
			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}
	}

	// �A�����b�N
	m_pIndex->Unlock();
	delete[] pCross;
	pCross = NULL;
}

// ----------------------------------------
// ��`�̒��_���W�̐ݒ�
// ----------------------------------------
void CStencilshadow::SetCylinderVtxPos(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���g
	float fOriginY;				// y�̈ʒu
	float fAngle;				// �p�x
	float fRadian;				// y�̃��W�A���l
	// �u���b�N�`��̌��_�̏����ݒ�
	fOriginY = -m_size.y * m_nBlock_Depth;
	// �p�x�̌v�Z
	fAngle = D3DX_PI * 2 / m_nBlock_Width;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//���_�ݒ� //
	//�s���[�v
	for (int nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (int nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ���W�A���l
			fRadian = fAngle * nCountWidth;
			fRadian = CCalculation::Rot_One_Limit(fRadian);
			// ���_���W�̐ݒ�
			pVtx[0].pos =
				D3DXVECTOR3(
				(sinf(fRadian) * m_size.x),
					fOriginY + m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// �|�C���g���킹
			pVtx++;
		}
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// ��`�̐ݒ�
// ----------------------------------------
void CStencilshadow::SetRect(LPDIRECT3DDEVICE9 pDevice)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;
	//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	WORD * pIdx;

	// �����_���E�C���f�b�N�X�E�|���S���̏����ݒ�(�v�Z)
	m_nNumberVertex = 10;
	m_nNumIndex = 10;
	m_nNumPolygon = 8;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��v = 8
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,	-m_size.y,	m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y,	m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y,	-m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y,	-m_size.z * 0.5f);
	pVtx[4].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y,	m_size.z * 0.5f);

	pVtx[5].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f,	m_size.z * 0.5f);
	pVtx[6].pos = D3DXVECTOR3(m_size.x * 0.5f,	0.0f,	m_size.z * 0.5f);
	pVtx[7].pos = D3DXVECTOR3(m_size.x * 0.5f,	0.0f,	-m_size.z * 0.5f);
	pVtx[8].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f,	-m_size.z * 0.5f);
	pVtx[9].pos = D3DXVECTOR3(-m_size.x * 0.5f,	0.0f,	m_size.z * 0.5f);



	//���_�̍��v = 8
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�̍��v = 8
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	pVtx[4].col = m_col;
	pVtx[5].col = m_col;
	pVtx[6].col = m_col;
	pVtx[7].col = m_col;
	pVtx[8].col = m_col;
	pVtx[9].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃��b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIndex->Lock(0, 0, (void**)&pIdx, 0);

	// ���u���b�N�̒��_��
	for (int nCntIndex = 0; nCntIndex < 4 + 1; nCntIndex++)
	{
		// �`�揇�Ԃ̃C���f�b�N�X
		pIdx[0] = nCntIndex + 4 + 1;
		pIdx[1] = nCntIndex;

		// �C���f�b�N�X�̃|�C���g���킹
		pIdx += 2;
	}
	//�C���f�b�N�X�̃o�b�t�@�̃A�����b�N
	m_pIndex->Unlock();
}

// ----------------------------------------
// ��`�̒��_���W�̐ݒ�
// ----------------------------------------
void CStencilshadow::SetRectVtxPos(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��v = 8
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y, m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y, -m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, -m_size.z * 0.5f);
	pVtx[4].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, m_size.z * 0.5f);

	pVtx[5].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[6].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[7].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, -m_size.z * 0.5f);
	pVtx[8].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, -m_size.z * 0.5f);
	pVtx[9].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}