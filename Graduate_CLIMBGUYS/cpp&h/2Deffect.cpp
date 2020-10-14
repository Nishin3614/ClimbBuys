// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2D�G�t�F�N�g�̏���[2Deffect.cpp]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "2Deffect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ORIGIN_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f))	// �����ʒu

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9	C2DEffect::m_pVtxBuff = NULL;			// �o�b�t�@���
CEffect::EFFECT			C2DEffect::m_aEffect[EFFECT_MAX] = {};	// �G�t�F�N�g���

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DEffect::C2DEffect() :CEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DEffect::~C2DEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Init(void)
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
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Uninit(void)
{
	// �o�b�t�@����NULL�`�F�b�N
	if (m_pVtxBuff != NULL)
	{
		// �o�b�t�@���̊J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Update(void)
{
	// �ϐ���`
	VERTEX_2D *pVtx;				// ���_���̃|�C���^
	C2DEffect::EFFECT *pEffect;		// �G�t�F�N�g�̃|�C���^
	// �|�C���^�̏�����
	pEffect = &m_aEffect[0];

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �G�t�F�N�g���[�v
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
		pEffect->pos -= pEffect->move;
		// ���_�T�C�Y�̐ݒ�
		SetVartexSize(pVtx, pEffect);
		// ���_�J���[�̐ݒ�
		SetVetexColor(pVtx, pEffect);
		// �e�N�X�`���[�����D�Ȃ�
		if (pEffect->nTexType == 55)
		{
			// �J�E���g�A�j���A�b�v
			pEffect->nCntAnim++;
			if (pEffect->nCntAnim % pEffect->nMaxCntAnim == 0)
			{
				// �����A�j���[�V�����J�E���g�A�b�v
				pEffect->nHorizonAnim++;
				if (pEffect->nHorizonAnim >= pEffect->nMaxHorizonAnim)
				{
					// �����A�j���[�V�����J�E���g������
					pEffect->nHorizonAnim = 0;
					// �����A�j���[�V�����J�E���g�A�b�v
					pEffect->nVirticalAnim++;
					if (pEffect->nVirticalAnim >= pEffect->nMaxVirticalAnim)
					{
						// �����A�j���[�V�����J�E���g������
						pEffect->nVirticalAnim = 0;
					}
				}
				// �e�N�X�`���[���W�ݒ�
				SetVartexTexSize(pVtx, pEffect);
			}
		}
	}
	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice;					// �f�o�C�X�̃|�C���^
	C2DEffect::EFFECT *pEffect;						// �G�t�F�N�g�̃|�C���^

	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	pEffect = &m_aEffect[0];						// �|�C���^�̏�����


	// �G�t�F�N�g���[�v
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// �������Z�ݒ�
			CManager::GetRenderer()->SetBlend(pEffect->BlendType);
			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(
				0,
				m_pVtxBuff,
				0,
				sizeof(VERTEX_2D)
			);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, CTexture_manager::GetTexture(pEffect->nTexType));
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
	// �����_�[�X�e�[�g(�ʏ�u�����h����)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�쐬
//	pDevice	: �f�o�C�X���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C2DEffect::MakeVertex(
	LPDIRECT3DDEVICE9 pDevice	// �f�o�C�X���
)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;							// ���_���

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pVtx += 4)
	{
		// ���_�T�C�Y�ݒ�
		SetVartexSize(pVtx, NULL, &nCntEffect);

		// ���_
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
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
	// �e�N�X�`���[���W�ݒ�
	SetVartexTexSize(pVtx);
	// ���_�f�[�^���A�����b�N����
	m_pVtxBuff->Unlock();

	// ������Ԃ�
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�T�C�Y�ݒ�
//	pVtx	: 2D���_���
//	pEffect	: �G�t�F�N�g���
//	nIndex	: �ԍ����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::SetVartexSize(
	VERTEX_2D *pVtx,	// 2D���_���
	EFFECT *pEffect,	// �G�t�F�N�g���
	const int *nIndex	// �ԍ����
)
{
	// �ԍ�����NULL�`�F�b�N
	if (nIndex != NULL)
	{
		// �ϐ��錾
		C2DEffect::EFFECT *pLocalEffect;					// �G�t�F�N�g�̃|�C���^
		pLocalEffect = &C2DEffect::m_aEffect[*nIndex];		// �|�C���^�̏�����

		// ���_�̐ݒ�
		pVtx[0].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[0].pos.x += -sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[0].pos.y += -cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[1].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[1].pos.x += sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[1].pos.y += -cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[2].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[2].pos.x += -sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[2].pos.y += cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[3].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[3].pos.x += sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[3].pos.y += cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
	}
	// �G�t�F�N�g����NULL�`�F�b�N
	else if (pEffect != NULL)
	{
		// ���_�̐ݒ�
		pVtx[0].pos = ORIGIN_POS + pEffect->pos;
		pVtx[0].pos.x += -sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[0].pos.y += -cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
		pVtx[1].pos = ORIGIN_POS + pEffect->pos;
		pVtx[1].pos.x += sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[1].pos.y += -cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[2].pos = ORIGIN_POS + pEffect->pos;
		pVtx[2].pos.x += -sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[2].pos.y += cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[3].pos = ORIGIN_POS + pEffect->pos;
		pVtx[3].pos.x += sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[3].pos.y += cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�J���[�ݒ�
//	pVtx	: 2D���_���
//	pEffect	: �G�t�F�N�g���
//	nIndex	: �ԍ����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::SetVetexColor(
	VERTEX_2D *pVtx,	// 2D���_���
	EFFECT *pEffect,	// �G�t�F�N�g���
	const int *nIndex	// �ԍ����
)
{
	// �ԍ�����NULL�`�F�b�N
	if (nIndex != NULL)
	{
		// �ϐ��錾
		C2DEffect::EFFECT *pLocalEffect;					// �G�t�F�N�g�̃|�C���^
		pLocalEffect = &C2DEffect::m_aEffect[*nIndex];		// �|�C���^�̏�����

		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pLocalEffect->col;
	}
	// �G�t�F�N�g����NULL�`�F�b�N
	else if (pEffect != NULL)
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pEffect->col;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�e�N�X�`���[���W�ݒ�
//	pVtx	: 2D���_���
//	pEffect	: �G�t�F�N�g���
//	nIndex	: �ԍ����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::SetVartexTexSize(
	VERTEX_2D *pVtx,	// 2D���_���
	EFFECT *pEffect,	// �G�t�F�N�g���
	const int *nIndex	// �ԍ����
)
{
	// �ϐ��錾
	D3DXVECTOR2 FirstSize;
	D3DXVECTOR2 EndSize;
	// �ԍ�����NULL�`�F�b�N
	if (nIndex != NULL)
	{
		// �ϐ��錾
		C2DEffect::EFFECT *pLocalEffect;					// �G�t�F�N�g�̃|�C���^
		pLocalEffect = &C2DEffect::m_aEffect[*nIndex];		// �|�C���^�̏�����
		FirstSize = D3DXVECTOR2(
			pLocalEffect->nHorizonAnim * (1.0f / pLocalEffect->nMaxHorizonAnim),
			pLocalEffect->nVirticalAnim * (1.0f / pLocalEffect->nMaxVirticalAnim)
		);
		EndSize = D3DXVECTOR2(
			pLocalEffect->nHorizonAnim * (1.0f / pLocalEffect->nMaxHorizonAnim) + (1.0f / pLocalEffect->nMaxHorizonAnim),
			pLocalEffect->nVirticalAnim * (1.0f / pLocalEffect->nMaxVirticalAnim) + (1.0f / pLocalEffect->nMaxVirticalAnim)
		);

		// ���_�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(FirstSize.x, FirstSize.y);
		pVtx[1].tex = D3DXVECTOR2(EndSize.x, FirstSize.y);
		pVtx[2].tex = D3DXVECTOR2(FirstSize.x, EndSize.y);
		pVtx[3].tex = D3DXVECTOR2(EndSize.x, EndSize.y);
	}
	// �G�t�F�N�g����NULL�`�F�b�N
	else if (pEffect != NULL)
	{
		FirstSize = D3DXVECTOR2(
			pEffect->nHorizonAnim * (1.0f / pEffect->nMaxHorizonAnim),
			pEffect->nVirticalAnim * (1.0f / pEffect->nMaxVirticalAnim)
		);
		EndSize = D3DXVECTOR2(
			pEffect->nHorizonAnim * (1.0f / pEffect->nMaxHorizonAnim) + (1.0f / pEffect->nMaxHorizonAnim),
			pEffect->nVirticalAnim * (1.0f / pEffect->nMaxVirticalAnim) + (1.0f / pEffect->nMaxVirticalAnim)
		);
		// ���_�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(FirstSize.x, FirstSize.y);
		pVtx[1].tex = D3DXVECTOR2(EndSize.x, FirstSize.y);
		pVtx[2].tex = D3DXVECTOR2(FirstSize.x, EndSize.y);
		pVtx[3].tex = D3DXVECTOR2(EndSize.x, EndSize.y);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu�������
//	pEffect	: �G�t�F�N�g���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Pos_Limit(
	EFFECT *pEffect		// �G�t�F�N�g���
)
{
	// �ʒux�̏���𒴂�����
	if (pEffect->pos.x < -pEffect->size.x || pEffect->pos.x >(SCREEN_WIDTH + pEffect->size.x))
	{
		pEffect->bUse = false;
	}
	// �ʒuy�̏���𒴂�����
	if (pEffect->pos.y < -pEffect->size.y || pEffect->pos.y >(SCREEN_HEIGHT + pEffect->size.y))
	{
		pEffect->bUse = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DEffect * C2DEffect::Create(void)
{
	// �ϐ��錾
	C2DEffect *p2DEffect = new C2DEffect;
	// �V�[���Ǘ��ݒ�
	p2DEffect->ManageSetting(CScene::LAYER_3DPARTICLE);
	// ������
	p2DEffect->Init();
	return p2DEffect;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S�̂̐ݒ�
//	EffectType	: �G�t�F�N�g�^�C�v
//	nTexType	: �e�N�X�`���[�^�C�v
//	pos			: �ʒu
//	rot			: ��]
//	move		: �ړ���
//	col			: �F
//	size		: �T�C�Y
//	nLife		: ���C�t
//	Blend		: �u�����h�^�C�v
//	sizeValue	: �T�C�Y�ω�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Set2DEffect(
	EFFECT_TYPE const &EffectType,					// �G�t�F�N�g�^�C�v
	int const &nTexType,							// �e�N�X�`���[�^�C�v
	D3DXVECTOR3 const &pos,							// �ʒu
	D3DXVECTOR3 const &rot,							// ��]
	D3DXVECTOR3 const &move,						// �ړ���
	D3DXCOLOR const &col,							// �F
	D3DXVECTOR2 const &size,						// �T�C�Y
	int const &nLife,								// ���C�t
	CRenderer::BLEND const &Blend,					// �u�����h�^�C�v
	D3DXVECTOR2 const &sizeValue,					// �T�C�Y�ω��l
	float const &fAlphaValue						// ���ω��l
)
{
	// �ϐ��錾
	C2DEffect::EFFECT *pEffect;			// �G�t�F�N�g�̃|�C���^
	pEffect = &C2DEffect::m_aEffect[0];	// �|�C���^�̏�����

	// �ő吔���[�v
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		// �g�p�t���O�I�t�̌̂̎�
		if (pEffect->bUse == false)
		{
			// �G�t�F�N�g���̒l������
			Init_OneValues(pEffect);
			// �ϐ���`
			VERTEX_2D *pVtx;		// ���_�|�C���^
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
			// �u�����h�ݒ�
			pEffect->BlendType = Blend;
			// ���_�ʒu�̐ݒ�
			SetVartexSize(pVtx, pEffect);
			// �F�̐ݒ�
			pEffect->col = col;
			// ���_�J���[�̐ݒ�
			SetVetexColor(pVtx, pEffect);
			// �g�p�t���O���I��
			pEffect->bUse = true;
			// �A���t�@�ω��l�̐ݒ�
			pEffect->fAlphaValue = fAlphaValue;
			// �T�C�Y�ω��l�̐ݒ�
			pEffect->sizeValue = sizeValue;
			// �e�N�X�`���[�����D�Ȃ�
			if (pEffect->nTexType == 55)
			{
				pEffect->nMaxCntAnim = 5;
				pEffect->nMaxHorizonAnim = 6;
			}
			// �e�N�X�`���[���W�ݒ�
			SetVartexTexSize(pVtx, pEffect);

			//���_�f�[�^���A�����b�N
			m_pVtxBuff->Unlock();
			break;
		}
	}

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S���\�[�X���̓ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C2DEffect::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S���\�[�X���̊J��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Unload(void)
{
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Debug(void)
{
}
#endif // _DEBUG
