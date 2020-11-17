// ----------------------------------------
//
// �O�Տ����̐���[meshobit.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "meshobit.h"
#include "game.h"
#include "debugproc.h"

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
// �e�N�X�`���[ID
int CMeshobit::m_nTexId[TEX_MAX] = 
{
	50,
	0
};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CMeshobit::CMeshobit() : CScene()
{
	m_pVtxBuff = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
	m_aOffset[0] = D3DVECTOR3_ZERO;		// �I�t�Z�b�g���W
	m_aOffset[1] = D3DVECTOR3_ZERO;		// �I�t�Z�b�g���W
	m_col = D3DXCOLOR_INI;				// �F
	m_tex = TEX_0;						// �e�N�X�`���[
	m_nLine = 0;						// ���̐�
	m_nNumberVertex = 0;				// �����_��
	m_nNumPolygon = 0;					// ���|���S��
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CMeshobit::~CMeshobit()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CMeshobit::Init(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	// �ݒ�
	m_nNumberVertex =			// ���_
		m_nLine * 2;
	m_nNumPolygon =				// �|���S��
		m_nLine * 2 - 2;
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
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
	//���_�ݒ� //
	//�s���[�v
	for (int nCntMeshObit = 0; nCntMeshObit < m_nLine; nCntMeshObit++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		// �e�N�X�`���[�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_nLine, 0.0f);
		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_���W�̐ݒ�
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �J���[�̐ݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		// �e�N�X�`���[�̐ݒ�
		pVtx[1].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_nLine, 1.0f);
		// �@���̐ݒ�
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// �|�C���g���킹
		pVtx += 2;
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CMeshobit::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CMeshobit::Update(void)
{
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CMeshobit::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	// �s��ϐ�
	D3DXMATRIX mtx;
	// ���ʂ�`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�o�b�t�@�@����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	// Z�����@����
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&mtx);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexId[m_tex]));
	// �O�Ղ̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,										// �`����n�߂钸�_
		m_nNumPolygon	// �|���S����
	);

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// ����(�����)�ŃJ�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Z�o�b�t�@�@�L��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// Z�����@�L��
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

// ----------------------------------------
// ���_���W�̊m�菈��
// ----------------------------------------
void CMeshobit::DrawSet(
	D3DXMATRIX const & mtx
)
{
	// �ϐ��錾
	VERTEX_3D * pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	for (int nCntObit = m_nLine - 1; nCntObit > 0; nCntObit--)
	{
		pVtx[nCntObit * 2].pos = pVtx[(nCntObit - 1) * 2].pos;
		pVtx[nCntObit * 2 + 1].pos = pVtx[(nCntObit - 1) * 2 + 1].pos;
	}
	// 1�Ԗڂ̒��_
	D3DXVec3TransformCoord(
		&pVtx[0].pos,
		&m_aOffset[0],
		&mtx
	);
	// 2�Ԗڂ̒��_
	D3DXVec3TransformCoord(
		&pVtx[1].pos,
		&m_aOffset[1],
		&mtx
	);
	// �A�����b�N
	m_pVtxBuff->Unlock();
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

//-------------------------------------------------------------------------------------------------------------
// �F�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::SetCol(
	D3DXCOLOR const & BiginCol,	// �n�_�̐F
	D3DXCOLOR const & EndCol	// �I�_�̐F
)
{
	// �ϐ��錾
	VERTEX_3D * pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	//���_�ݒ� //
	//�s���[�v
	for (int nCntMeshObit = 0; nCntMeshObit < m_nLine; nCntMeshObit++)
	{
		// �J���[�̐ݒ�
		pVtx[0].col = BiginCol;
		// �J���[�̐ݒ�
		pVtx[1].col = EndCol;
		// �|�C���g���킹
		pVtx += 2;
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// �ʒu�̏�����
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::InitPos(D3DXMATRIX const & mtx)
{
	// �ϐ��錾
	VERTEX_3D * pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 1�Ԗڂ̒��_
	D3DXVec3TransformCoord(
		&pVtx[0].pos,
		&m_aOffset[0],
		&mtx
	);
	// 2�Ԗڂ̒��_
	D3DXVec3TransformCoord(
		&pVtx[1].pos,
		&m_aOffset[1],
		&mtx
	);
	for (int nCntObit = 1; nCntObit < m_nLine; nCntObit++)
	{
		pVtx[nCntObit * 2].pos = pVtx[0].pos;
		pVtx[nCntObit * 2 + 1].pos = pVtx[1].pos;
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();

}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::Debug(void)
{
	// �ϐ��錾
	VERTEX_3D * pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	for (int nCntObit = 0; nCntObit < m_nLine; nCntObit++)
	{
		//CDebugproc::Print("[�O��%d]:�ʒu(%.2f,%.2f,%.2f)\n",nCntObit * 2, pVtx[nCntObit * 2].pos.x, pVtx[nCntObit * 2].pos.y,pVtx[nCntObit * 2].pos.z);
		//CDebugproc::Print("[�O��%d]:�ʒu(%.2f,%.2f,%.2f)\n", nCntObit * 2 + 1, pVtx[nCntObit * 2 + 1].pos.x, pVtx[nCntObit * 2 + 1].pos.y, pVtx[nCntObit * 2 + 1].pos.z);
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
}
#endif // _DEBUG

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CMeshobit::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CMeshobit::UnLoad(void)
{
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
std::unique_ptr<CMeshobit> CMeshobit::Create_Self(
	int const &nLine,				// ���̐�
	D3DXVECTOR3 const &BeginOffset,	// �n�_�I�t�Z�b�g
	D3DXVECTOR3 const &EndOffset,	// �I�_�I�t�Z�b�g
	TEX const &TexId				// �e�N�X�`���[�^�C�v
)
{
	// �ϐ��錾
	std::unique_ptr<CMeshobit> pMeshobit(new CMeshobit);		// �V�[��2D�N���X
	// ���̐�
	pMeshobit->m_nLine = nLine;
	// �n�_�I�t�Z�b�g
	pMeshobit->m_aOffset[0] = BeginOffset;
	// �I�_�I�t�Z�b�g
	pMeshobit->m_aOffset[1] = EndOffset;
	// �e�N�X�`���[ID
	pMeshobit->m_tex = TexId;
	// ����������
	pMeshobit->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pMeshobit;
}