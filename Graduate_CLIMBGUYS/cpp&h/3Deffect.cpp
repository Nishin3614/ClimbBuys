// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�G�t�F�N�g�̏���[3Deffect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Deffect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9	C3DEffect::m_pVtxBuff = NULL;			// �o�b�t�@���
CEffect::EFFECT			C3DEffect::m_aEffect[EFFECT_MAX] = {};	// �G�t�F�N�g���

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DEffect::C3DEffect() :CEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DEffect::~C3DEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Init(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;					// �f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �G�t�F�N�g�̏�����
	CEffect::Init();
	// �ϐ��̏�����
	CEffect::InitValues(&m_aEffect[0]);
	// ���_���쐬
	MakeVertex(pDevice);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Update(void)
{
	// �ϐ���`
	VERTEX_3D *pVtx;				// ���_���̃|�C���^
	C3DEffect::EFFECT *pEffect;		// �G�t�F�N�g�̃|�C���^
	// �|�C���^�̏�����
	pEffect = &m_aEffect[0];

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ő吔�����[�v
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++, pVtx+=4)
	{
		// �g�p�t���O���I�t�̎�
		// ->���[�v�X�L�b�v
		if (!pEffect->bUse)
		{
			continue;
		}
		// ���C�t�X�V
		// ->���[�v�X�L�b�v
		if (!CEffect::UpdateLife(pEffect))
		{
			continue;
		}
		// �ړ��̍X�V
		CEffect::UpdateMove(pEffect);
		// ���_���̍X�V
		CEffect::UpdateVetex(pEffect);
		// �ʒu�X�V
		pEffect->pos += pEffect->move;
		// ���_�T�C�Y�̐ݒ�
		SetVartexSize(pVtx, pEffect);
		// ���_�J���[�̐ݒ�
		SetVetexColor(pVtx, pEffect);
	}
	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;					// �f�o�C�X�̃|�C���^
	D3DXMATRIX
		mtxRot,										// ��]�s��
		mtxTrans,									// �ʒu�s��
		mtxView,									// �r���[�s��
		mtxWorld;									// ���[���h�s��

	C3DEffect::EFFECT *pEffect;						// �G�t�F�N�g�̃|�C���^

	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	pEffect = &m_aEffect[0];						// �|�C���^�̏�����

	// ���C�e�B���O���[�h����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Z�o�b�t�@�@�L���@����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// �������Z�ݒ�
			CManager::GetRenderer()->SetBlend(pEffect->BlendType);
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �}�g���b�N�X�r���[�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			// �r���{�[�h
			CCalculation::SetBillboard(
				&mtxWorld,
				&mtxView
			);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				pEffect->rot.y,
				pEffect->rot.x,
				pEffect->rot.z);
			D3DXMatrixMultiply(
				&mtxWorld,
				&mtxWorld,
				&mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(
				&mtxTrans,
				pEffect->pos.x,
				pEffect->pos.y,
				pEffect->pos.z);
			D3DXMatrixMultiply(
				&mtxWorld,
				&mtxWorld,
				&mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, CTexture_manager::GetTexture(pEffect->nTexType));
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
	//�A���t�@�e�X�g�߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Z�o�b�t�@�@�L��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�̍쐬
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;							// ���_���̃|�C���^

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �G�t�F�N�g���[�v
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pVtx += 4)
	{
		SetVartexSize(pVtx, NULL, &nCntEffect);

		// �@���x�N�g��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// texture���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�T�C�Y�̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVartexSize(VERTEX_3D * pVtx, EFFECT * pEffect, const int * nIndex)
{
	if (nIndex != NULL)
	{
		// �ϐ��錾
		C3DEffect::EFFECT *pLocalEffect;					// �G�t�F�N�g�̃|�C���^
		pLocalEffect = &C3DEffect::m_aEffect[*nIndex];		// �|�C���^�̏�����

		// ���_�̐ݒ�
		pVtx[0].pos.x = -sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[0].pos.y = -cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[0].pos.z = -cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[1].pos.x = -sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[1].pos.y = cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[1].pos.z = -cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[2].pos.x = sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[2].pos.y = -cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[2].pos.z = cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[3].pos.x = sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[3].pos.y = cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[3].pos.z = cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		return;
	}
	if (pEffect != NULL)
	{
		// ���_�̐ݒ�
		pVtx[0].pos.x = -sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[0].pos.y = -cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
		pVtx[0].pos.z = -cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[1].pos.x = -sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[1].pos.y = cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[1].pos.z = -cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[2].pos.x = sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[2].pos.y = -cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[2].pos.z = cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[3].pos.x = sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[3].pos.y = cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
		pVtx[3].pos.z = cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		return;
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�J���[�̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVetexColor(VERTEX_3D * pVtx, EFFECT * pEffect, const int * nIndex)
{
	if (nIndex != NULL)
	{
		// �ϐ��錾
		C3DEffect::EFFECT *pLocalEffect;					// �G�t�F�N�g�̃|�C���^
		pLocalEffect = &C3DEffect::m_aEffect[*nIndex];		// �|�C���^�̏�����

		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pLocalEffect->col;
	}
	if (pEffect != NULL)
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pEffect->col;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DEffect * C3DEffect::Create(void)
{
	// �ϐ��錾
	C3DEffect *p3DEffect = new C3DEffect;
	// �V�[���Ǘ��ݒ�
	p3DEffect->ManageSetting(CScene::LAYER_3DPARTICLE);
	// ������
	p3DEffect->Init();
	return p3DEffect;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �G�t�F�N�g�̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Set3DEffect(
	EFFECT_TYPE const &EffectType,
	int const &nTexType,
	D3DXVECTOR3 const &pos,
	D3DXVECTOR3 const &rot,
	D3DXVECTOR3 const &move,
	D3DXCOLOR const &col,
	D3DXVECTOR2 const &size,
	int const &nLife,
	CRenderer::BLEND const &Blend,
	D3DXVECTOR2 const &sizeValue,
	float const &fAlphaValue
)
{
	// �ϐ��錾
	C3DEffect::EFFECT *pEffect;			// �G�t�F�N�g�̃|�C���^
	pEffect = &C3DEffect::m_aEffect[0];	// �|�C���^�̏�����

	// �ő吔���[�v
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		// �g�p�t���O�I�t�̌̂̎�
		if (pEffect->bUse == false)
		{
			// �l�̏���������
			Init_OneValues(pEffect);
			// �ϐ���`
			VERTEX_3D *pVtx;		// ���_�|�C���^
			//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// ���_�|�C���^�̍X�V
			pVtx += nCntEffect * 4;
			// ���C�t�̐ݒ�
			pEffect->nLife = nLife;
			// �e�N�X�`����ނ̐ݒ�
			pEffect->nTexType = nTexType;
			// �G�t�F�N�g��ނ̐ݒ�
			pEffect->EffectType = EffectType;
			// �ړ��ʂ̐ݒ�
			pEffect->move = move;
			// ���a�̐ݒ�
			pEffect->size = size;
			// �ʒu�̐ݒ�
			pEffect->pos = pos;
			// ��]�̐ݒ�
			pEffect->rot = rot;
			// �u�����h�^�C�v�ݒ�
			pEffect->BlendType = Blend;
			// ���_�ʒu�̐ݒ�
			SetVartexSize(pVtx, pEffect);
			// �F�̐ݒ�
			pEffect->col = col;
			// ���_�J���[�̐ݒ�
			SetVetexColor(pVtx, pEffect);
			// �A���t�@�ω��l�̐ݒ�
			pEffect->fAlphaValue = fAlphaValue;
			// �T�C�Y�ω��l�̐ݒ�
			pEffect->sizeValue = sizeValue;
			// �g�p�t���O���I��
			pEffect->bUse = true;
			//���_�f�[�^���A�����b�N
			m_pVtxBuff->Unlock();
			break;
		}
	}

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::Load(void)
{
	// �e�N�X�`���t�@�C�����̎擾
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Unload(void)
{

}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Debug(void)
{

}
#endif // _DEBUG
