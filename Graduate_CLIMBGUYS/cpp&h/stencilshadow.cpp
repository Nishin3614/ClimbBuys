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
	m_bUse = false;
	m_type = TYPE_CYLINDER;
	m_pSceneTwo = NULL;
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
	m_pSceneTwo = std::move(CScene_TWO::Creat_Unique(
		CScene_TWO::OFFSET_TYPE_CENTER,
		{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f },
		{ SCREEN_WIDTH,SCREEN_HEIGHT },
		-1,
		0.0f,
		{0.0f,0.0f,0.0f,1.0f}
	));
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
	// 2D�|���S���̊J��
	if (m_pSceneTwo != NULL)
	{
		m_pSceneTwo->Uninit();
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CStencilshadow::Update(void)
{
	/*
	// �ϐ��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���g
	int nCountDirect;
	int nCountWidth;

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
			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = pVtx[0].tex +
				D3DXVECTOR2(
					1.0f / m_nBlock_Width * 0.1f,
					0.0f);
			// �F�ݒ�
			pVtx[0].col = m_col;

			// �|�C���g���킹
			pVtx++;
		}
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
	// 2D�|���S���̊J��
	if (m_pSceneTwo != NULL)
	{
		m_pSceneTwo->Update();
	}
	*/
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CStencilshadow::Draw(void)
{
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




	/*
	CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLBACK);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,
		0,
		8,		//�g�p���钸�_�� �O�p�|���S���̒��_
		0,		//���_�̓ǂݎ����J�n����ʒu
		16);	//�|���S���̖���

				// �J�����O���Ȃ�
	CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLNULL);

	//Z�e�X�g�ʏ�
	CManager::GetRenderer()->SetType(CRenderer::TYPE_ZBUFFOFF);

	//�ʏ퍇��
	CManager::GetRenderer()->SetType(CRenderer::TYPE_NORMALMIX);

	//���C�e�B���OON
	CManager::GetRenderer()->SetType(CRenderer::TYPE_RIGHTINGON);
	*/


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
	// 2D�|���S���̕`��
	if (m_pSceneTwo != NULL)
	{
		m_pSceneTwo->Draw();
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
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CStencilshadow::UnLoad(void)
{
}

// ----------------------------------------
// �쐬����(�V�[���Ǘ�)
// ----------------------------------------
CStencilshadow * CStencilshadow::Create(
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
	// �V�[���Ǘ��ݒ�
	pStencilshadow->ManageSetting(CScene::LAYER_3DSHADOW);
	// �ʒu���
	pStencilshadow->m_pos = pos;
	// �T�C�Y���
	pStencilshadow->m_size = size;
	// ��]���
	pStencilshadow->m_rot = rot;
	// �F���
	pStencilshadow->m_col = col;
	// �F���
	pStencilshadow->m_col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
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
// �g�p��Ԏ擾
// ----------------------------------------
bool CStencilshadow::GetUse(void)
{
	return m_bUse;
}

// ----------------------------------------
// �ʒu�ݒ�
// ----------------------------------------
void CStencilshadow::SetPos(D3DXVECTOR3 const & pos)
{
	m_pos = pos;
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
	m_col = col;
}

// ----------------------------------------
// �g�p���
// ----------------------------------------
void CStencilshadow::SetUse(bool const bUse)
{
	m_bUse = bUse;
}

// ----------------------------------------
// ���_���W�̐���
// ----------------------------------------
void CStencilshadow::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���g
	WORD * pIdx;				// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	D3DXVECTOR3 VecA, VecB;		// �x�N�g��
	int nCountDirect;			// �c�̃J�E���g
	int nCountWidth;			// ���̃J�E���g
	int nCntBlock = 0;
	float fAngle;				// y�̊p�x
	float fRadian;				// y�̃��W�A���l
	D3DXVECTOR3 *pCross;		// �|���S���̊O��
	pCross =					// �������m��
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

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
					m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// �@���x�N�g���̐ݒ�
			//pVtx[0].nor = pVtx[0].pos;
			//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			// �J���[�̐ݒ�
			pVtx[0].col = m_col;

			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(
				1.0f / m_nBlock_Width * nCountWidth,
				1.0f / m_nBlock_Depth * (m_nBlock_Depth - nCountDirect)
			);

			// �|�C���g���킹
			pVtx++;
		}
	}
	pVtx -= m_nNumberVertex;
	// �|���S�����Ƃ̖@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// �x�N�g��
			VecA = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 1].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2]);
			// �x�N�g��
			VecA = pVtx[nCntBlock + 1].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1]);
			// ����
			pVtx[nCntBlock].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// �E��
			pVtx[nCntBlock + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// ����
			pVtx[nCntBlock + m_nBlock_Width + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2];
			// �E��
			pVtx[nCntBlock + m_nBlock_Width + 2].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
		}
	}
	nCntBlock = 0;
	// ���_�@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++, nCntBlock++)
		{
			// �ŏ�
			if (nCntDepth == 0 && nCntWidth == 0)
			{
				pVtx[0].nor /= 2;
			}
			// �Ō�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 2;
			}
			// 1�s�̗�̍Ō�
			else if (nCntDepth == 0 && nCntWidth == m_nBlock_Width)
			{
			}
			// �Ō�s�̗�̍ŏ�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == 0)
			{
			}
			// �ŏ��̍s�܂��͍Ō�̍s
			else if (nCntDepth == 0 || nCntDepth == m_nBlock_Depth)
			{
				pVtx[0].nor /= 3;
			}
			// �ŏ��̗�܂��͍Ō�̗�
			else if (nCntWidth == 0 || nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 3;
			}
			// ����ȊO
			else
			{
				pVtx[0].nor /= 6;
			}
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
// �~���̐ݒ�
// ----------------------------------------
void CStencilshadow::SetCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���g
	WORD * pIdx;				// �C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	D3DXVECTOR3 VecA, VecB;		// �x�N�g��
	int nCountDirect;			// �c�̃J�E���g
	int nCountWidth;			// ���̃J�E���g
	int nCntBlock = 0;
	float fAngle;				// y�̊p�x
	float fRadian;				// y�̃��W�A���l
	D3DXVECTOR3 *pCross;		// �|���S���̊O��
	pCross =					// �������m��
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	// �u���b�N�`��̌��_�̏����ݒ�
	m_OriginBlock = D3DXVECTOR3(
		m_size.x * -0.5f * m_nBlock_Width,
		0.0f,
		m_size.z * 0.5f * m_nBlock_Depth);
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
					m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// �@���x�N�g���̐ݒ�
			//pVtx[0].nor = pVtx[0].pos;
			//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			// �J���[�̐ݒ�
			pVtx[0].col = m_col;

			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(
				1.0f / m_nBlock_Width * nCountWidth,
				1.0f / m_nBlock_Depth * (m_nBlock_Depth - nCountDirect)
			);

			// �|�C���g���킹
			pVtx++;
		}
	}
	pVtx -= m_nNumberVertex;
	// �|���S�����Ƃ̖@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// �x�N�g��
			VecA = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 1].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2]);
			// �x�N�g��
			VecA = pVtx[nCntBlock + 1].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1]);
			// ����
			pVtx[nCntBlock].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// �E��
			pVtx[nCntBlock + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// ����
			pVtx[nCntBlock + m_nBlock_Width + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2];
			// �E��
			pVtx[nCntBlock + m_nBlock_Width + 2].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
		}
	}
	nCntBlock = 0;
	// ���_�@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++, nCntBlock++)
		{
			// �ŏ�
			if (nCntDepth == 0 && nCntWidth == 0)
			{
				pVtx[0].nor /= 2;
			}
			// �Ō�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 2;
			}
			// 1�s�̗�̍Ō�
			else if (nCntDepth == 0 && nCntWidth == m_nBlock_Width)
			{
			}
			// �Ō�s�̗�̍ŏ�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == 0)
			{
			}
			// �ŏ��̍s�܂��͍Ō�̍s
			else if (nCntDepth == 0 || nCntDepth == m_nBlock_Depth)
			{
				pVtx[0].nor /= 3;
			}
			// �ŏ��̗�܂��͍Ō�̗�
			else if (nCntWidth == 0 || nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 3;
			}
			// ����ȊO
			else
			{
				pVtx[0].nor /= 6;
			}
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

void CStencilshadow::SetRect(LPDIRECT3DDEVICE9 pDevice)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;
	//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	WORD * pIdx;

	// �����_���E�C���f�b�N�X�E�|���S���̏����ݒ�(�v�Z)
	m_nNumberVertex = 8;
	m_nNumIndex = 18;
	m_nNumPolygon = 16;

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
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y * 0.5f,	-m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f,	m_size.y * 0.5f,	-m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y * 0.5f,	-m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f,	-m_size.z * 0.5f);
	pVtx[4].pos = D3DXVECTOR3(m_size.x * 0.5f,	m_size.y * 0.5f,	m_size.z * 0.5f);
	pVtx[5].pos = D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y * 0.5f,	m_size.z * 0.5f);
	pVtx[6].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y * 0.5f,	m_size.z * 0.5f);
	pVtx[7].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f,	m_size.z * 0.5f);

	//���_�̍��v = 8
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�̍��v = 8
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	pVtx[4].col = m_col;
	pVtx[5].col = m_col;
	pVtx[6].col = m_col;
	pVtx[7].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃��b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIndex->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�ݒ�
	pIdx[0] = 7;
	pIdx[1] = 6;
	pIdx[2] = 5;
	pIdx[3] = 4;
	pIdx[4] = 2;
	pIdx[5] = 1;
	pIdx[6] = 3;
	pIdx[7] = 0;
	pIdx[8] = 0;
	pIdx[9] = 1;
	pIdx[10] = 1;
	pIdx[11] = 4;
	pIdx[12] = 0;
	pIdx[13] = 6;
	pIdx[14] = 3;
	pIdx[15] = 7;
	pIdx[16] = 2;
	pIdx[17] = 5;

	//�C���f�b�N�X�̃o�b�t�@�̃A�����b�N
	m_pIndex->Unlock();
}
