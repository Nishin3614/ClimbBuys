//-----------------------------------------------------------------------------
//
// ���b�V���X�t�B�A���� [meshsphere.cpp]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "meshsphere.h"
#include "game.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-----------------------------------------------------------------------------
// �e�N�X�`���[ID
int CMeshsphere::m_nTexId[TEXTYPE_MAX] =
{
	-1,
	10,
	0
};

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CMeshsphere::CMeshsphere() : CScene()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CMeshsphere::~CMeshsphere()
{
}

//-----------------------------------------------------------------------------
// �쐬����(�V�[���Ǘ�)
//-----------------------------------------------------------------------------
CMeshsphere * CMeshsphere::Create(
	D3DXVECTOR3 const &pos,		// �ʒu
	float const &fRadius,		// ���a
	int const &nWidthBlock,		// ����
	int const &nHeightBlock,	// �c��
	D3DXCOLOR const &col,		// �J���[
	TEXTYPE	const &textype,		// �e�N�X�`���[
	CScene::LAYER const &layer,
	int const &nMaxFrame		// �t���[��
)
{
	// �ϐ��錾
	CMeshsphere *pMeshsphere = new CMeshsphere;
	// �V�[���Ǘ��ݒ�
	pMeshsphere->ManageSetting(layer);
	// �ݒ�
	pMeshsphere->m_MeshSphere.pos = pos;
	pMeshsphere->m_MeshSphere.fRadius = fRadius;
	pMeshsphere->m_MeshSphere.nWidthBlock = nWidthBlock;
	pMeshsphere->m_MeshSphere.nHeightBlock = nHeightBlock;
	pMeshsphere->m_MeshSphere.col = col;
	pMeshsphere->m_MeshSphere.Textype = textype;
	pMeshsphere->m_MeshSphere.nMaxFrame = nMaxFrame;
	// ������
	pMeshsphere->Init();
	return pMeshsphere;
}

//-----------------------------------------------------------------------------
// �쐬����(�l�Ǘ�)
//-----------------------------------------------------------------------------
CMeshsphere * CMeshsphere::Create_Self(
	D3DXVECTOR3 const &pos,		// �ʒu
	float const &fRadius,		// ���a
	int const &nWidthBlock,		// ����
	int const &nHeightBlock,	// �c��
	D3DXCOLOR const &col,		// �J���[
	TEXTYPE	const &textype,		// �e�N�X�`���[
	int const &nMaxFrame		// �t���[��
)
{
	// �ϐ��錾
	CMeshsphere *pMeshsphere = new CMeshsphere;
	// �ݒ�
	pMeshsphere->m_MeshSphere.pos = pos;
	pMeshsphere->m_MeshSphere.fRadius = fRadius;
	pMeshsphere->m_MeshSphere.nWidthBlock = nWidthBlock;
	pMeshsphere->m_MeshSphere.nHeightBlock = nHeightBlock;
	pMeshsphere->m_MeshSphere.col = col;
	pMeshsphere->m_MeshSphere.Textype = textype;
	pMeshsphere->m_MeshSphere.nMaxFrame = nMaxFrame;
	// ������
	pMeshsphere->Init();
	return pMeshsphere;
}

//-----------------------------------------------------------------------------
// �쐬����(�l_unique�Ǘ�)
//-----------------------------------------------------------------------------
std::unique_ptr<CMeshsphere> CMeshsphere::Create_Unique(
	D3DXVECTOR3 const &pos,		// �ʒu
	float const &fRadius,		// ���a
	int const &nWidthBlock,		// ����
	int const &nHeightBlock,	// �c��
	D3DXCOLOR const &col,		// �J���[
	TEXTYPE	const &textype,		// �e�N�X�`���[
	int const &nMaxFrame		// �t���[��
)
{
	// �ϐ��錾
	std::unique_ptr<CMeshsphere> pMeshsphere(new CMeshsphere);
	// �ݒ�
	pMeshsphere->m_MeshSphere.pos = pos;
	pMeshsphere->m_MeshSphere.fRadius = fRadius;
	pMeshsphere->m_MeshSphere.nWidthBlock = nWidthBlock;
	pMeshsphere->m_MeshSphere.nHeightBlock = nHeightBlock;
	pMeshsphere->m_MeshSphere.col = col;
	pMeshsphere->m_MeshSphere.Textype = textype;
	pMeshsphere->m_MeshSphere.nMaxFrame = nMaxFrame;
	// ������
	pMeshsphere->Init();
	return pMeshsphere;
}


//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void CMeshsphere::Init(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	m_MeshSphere.nMaxVtx = (m_MeshSphere.nWidthBlock + 1) * (m_MeshSphere.nHeightBlock + 1);
	m_MeshSphere.nMaxIndex = (m_MeshSphere.nWidthBlock * 2 + 2) * m_MeshSphere.nHeightBlock + ((m_MeshSphere.nHeightBlock - 1) * 4);
	m_MeshSphere.nMaxPolygon = m_MeshSphere.nWidthBlock * 2 * m_MeshSphere.nHeightBlock + (m_MeshSphere.nHeightBlock - 1) * 6;

	m_MeshSphere.fRot = (D3DX_PI * 2) / m_MeshSphere.nWidthBlock;
	m_MeshSphere.fRot2 = (D3DX_PI * 2) / m_MeshSphere.nHeightBlock;

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshSphere.nMaxVtx,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_MeshSphere.pVtxBuffMeshSphere,
		NULL);

	// �C���f�b�N�X���̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshSphere.nMaxIndex,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_MeshSphere.pIdxBuffMeshSphere,
		NULL);

	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_MeshSphere.pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̏��
	//�c�̌����J�E���g
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock + 1; nCntV++)
	{
		//�~�̒��_��
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			//���W
			pVtx[0].pos.x = sinf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;		//�J�����݂����Ȋ�����
			pVtx[0].pos.y = cosf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;							//����
			pVtx[0].pos.z = cosf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;

			pVtx[0].nor = pVtx[0].pos;							//�@���͕K�����K��
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);		//���K��

			pVtx[0].col = m_MeshSphere.col;		//�F
			pVtx[0].tex = D3DXVECTOR2((float)nCntV, (float)nCntH);	//(�e�N�X�`����UV���W)

			pVtx++;
		}
	}

	//���_�f�[�^���A�����b�N
	m_MeshSphere.pVtxBuffMeshSphere->Unlock();

	WORD *pIdx;
	//�C���f�b�N�X�o�b�t�@�͈̔͂����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_MeshSphere.pIdxBuffMeshSphere->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock; nCntV++)
	{
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			pIdx[0] = (m_MeshSphere.nWidthBlock + 1) + nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1);			//��񕪉� + ���i�񂾂� + (�c*���)
			pIdx[1] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1);											//���i�񂾂� + (�c*���)

			pIdx += 2;
			if ((nCntH + 1) % (m_MeshSphere.nWidthBlock + 1) == 0 && nCntV < m_MeshSphere.nHeightBlock - 1)
			{
				//�k�ރ|���S�����̌v�Z
				pIdx[0] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1);
				pIdx[1] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1) + 1;
				pIdx[2] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1) + 1;
				pIdx[3] = (m_MeshSphere.nWidthBlock + 1) + nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1) + 1;
				pIdx += 4;

			}
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃f�[�^���A�����b�N
	m_MeshSphere.pIdxBuffMeshSphere->Unlock();
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void CMeshsphere::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_MeshSphere.pVtxBuffMeshSphere != NULL)
	{
		m_MeshSphere.pVtxBuffMeshSphere->Release();
		m_MeshSphere.pVtxBuffMeshSphere = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void CMeshsphere::Update(void)
{
	// �t���[���ɂ���ď����鏈��
	FrameDelete();
}

//-----------------------------------------------------------------------------
// �ݒ�
//-----------------------------------------------------------------------------
void CMeshsphere::Set(
	D3DXVECTOR3 const & pos,
	int const & nMaxFrame,
	bool const & bUse
)
{
	m_MeshSphere.pos = pos;
	m_MeshSphere.bUse = bUse;
}

//-----------------------------------------------------------------------------
// �F�ݒ�
//	col	: �F
//-----------------------------------------------------------------------------
void CMeshsphere::SetCol(D3DXCOLOR const & col)
{
	// �F�ݒ�
	m_MeshSphere.col = col;
	// ���_�J���[�̐ݒ�
	Set_Vtx_Col();
}

//-----------------------------------------------------------------------------
// �F�ݒ�
//	col	: �F
//-----------------------------------------------------------------------------
void CMeshsphere::Set_Vtx_Col(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;	// 3D���_���

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_MeshSphere.pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̏��
	//�c�̌����J�E���g
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock + 1; nCntV++)
	{
		//�~�̒��_��
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			pVtx[0].col = m_MeshSphere.col;		//�F
			pVtx++;
		}
	}

	//���_�f�[�^���A�����b�N
	m_MeshSphere.pVtxBuffMeshSphere->Unlock();
}

//-----------------------------------------------------------------------------
// �t���[���ɂ���ď����鏈��
//-----------------------------------------------------------------------------
void CMeshsphere::FrameDelete(void)
{
	// �s�g�p��ԂȂ甲����
	if (m_MeshSphere.bUse == false)
	{
		return;
	}
	// �ő�t���[�����������l�܂��͏����l�ȉ��Ȃ�֐��𔲂���
	else if (m_MeshSphere.nMaxFrame <= 0)
	{
		return;
	}
	// �t���[���J�E���g���ő�t���[�����ȏ�ɂȂ�����s�g�p��Ԃ�
	else if (m_MeshSphere.nCntFrame >= m_MeshSphere.nMaxFrame)
	{
		m_MeshSphere.nCntFrame = 0;
		m_MeshSphere.bUse = false;
		return;
	}
	// �t���[���J�E���g�A�b�v
	m_MeshSphere.nCntFrame++;
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void CMeshsphere::Draw(void)
{
	// �s�g�p��Ԃ������甲����
	if (m_MeshSphere.bUse == false)
	{
		return;
	}
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX			mtxRot, mtxTrans;								// �v�Z�p

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// �J�����Ooff
	pDevice->SetRenderState(D3DRS_LIGHTING, false);				// ���C�e�B���Ooff

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexId[m_MeshSphere.Textype]));
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MeshSphere.mtxWorldMeshSphere);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_MeshSphere.rot.y, m_MeshSphere.rot.x, m_MeshSphere.rot.z);
	D3DXMatrixMultiply(&m_MeshSphere.mtxWorldMeshSphere, &m_MeshSphere.mtxWorldMeshSphere, &mtxRot);


	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_MeshSphere.pos.x, m_MeshSphere.pos.y, m_MeshSphere.pos.z);
	D3DXMatrixMultiply(&m_MeshSphere.mtxWorldMeshSphere, &m_MeshSphere.mtxWorldMeshSphere, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MeshSphere.mtxWorldMeshSphere);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_MeshSphere.pVtxBuffMeshSphere, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetIndices(m_MeshSphere.pIdxBuffMeshSphere);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,
		0,
		m_MeshSphere.nMaxIndex,		//���_��
		0,
		m_MeshSphere.nMaxPolygon);	//�|���S���̖���

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// �`��
	pDevice->SetRenderState(D3DRS_LIGHTING, true);				// ���C�e�B���Oon

	// �����_�[�X�e�[�g(Z�e�X�g)
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);					// Z�e�X�g�L��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);			// Z�e�X�g����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// ����
}
#ifdef _DEBUG
//-----------------------------------------------------------------------------
// �f�o�b�O�\��
//-----------------------------------------------------------------------------
void CMeshsphere::Debug(void)
{
}
#endif // _DEBUG
//-----------------------------------------------------------------------------
// �ǂݍ���
//-----------------------------------------------------------------------------
HRESULT CMeshsphere::Load(void)
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// �ǂݍ��񂾂��̂�j��
//-----------------------------------------------------------------------------
void CMeshsphere::Unload(void)
{
}

//-----------------------------------------------------------------------------
// �ʒu�̐ݒ�
//-----------------------------------------------------------------------------
void CMeshsphere::SetPosition(D3DXVECTOR3 const &pos)
{
	m_MeshSphere.pos = pos;
}

//-----------------------------------------------------------------------------
// �X�t�B�A�̔��a�̕ύX
//-----------------------------------------------------------------------------
void CMeshsphere::SetRadius(float const &fRadius)
{
	m_MeshSphere.fRadius = fRadius;
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_MeshSphere.pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̏��
	//�c�̌����J�E���g
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock + 1; nCntV++)
	{
		//�~�̒��_��
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			//���W
			pVtx[0].pos.x = sinf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;		//�J�����݂����Ȋ�����
			pVtx[0].pos.y = cosf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;							//����
			pVtx[0].pos.z = cosf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;

			pVtx++;

		}
	}
	//�C���f�b�N�X�o�b�t�@�̃f�[�^���A�����b�N
	m_MeshSphere.pIdxBuffMeshSphere->Unlock();
}
