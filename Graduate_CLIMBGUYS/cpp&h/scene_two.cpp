// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �V�[�������̐���[scene_two.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_two.h"
#include "manager.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO::CScene_TWO() : CScene::CScene()
{
	// �ϐ��̏�����
	m_pos = D3DVECTOR3_ZERO;
	m_size = D3DVECTOR2_ZERO;
	m_col = D3DXCOLOR_INI;
	m_rot = 0;
	m_fLengh = 0;
	m_fAngle = 0;
	m_offsetType = OFFSET_TYPE_CENTER;
	m_unipAnim = NULL;
	m_nTexId = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO::~CScene_TWO()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Init(void)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;				// ���_���
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// ���_���W��ݒ�
	Set_Vtx_Pos(m_offsetType,pVtx);
	// ���_�̑傫��
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// ���_�J���[
	Set_Vtx_Col(pVtx);
	// ���_�e�N�X�`���[
	Set_Vtx_Tex(D3DVECTOR2_ZERO, D3DVECTOR2_INI, pVtx);
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Uninit(void)
{
	// �A�j���[�V�������̊J��
	if (m_unipAnim != NULL)
	{
		m_unipAnim.reset();
	}
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
 		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Update(void)
{
	// �A�j���[�V�����X�V����
	Updata_Animation();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture_manager::GetTexture(m_nTexId));

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �|���S���`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	// Z�e�X�g��ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Debug(void)
{
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�V�[���p������)����
//	offsettype	: �^�C�v
//	pos			: �ʒu
//	size		: �T�C�Y
//	nTextype	: �e�N�X�`���[�^�C�v
//	frot		: �p�x
//	col			: �F
//	layer		: ���C���[
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO * CScene_TWO::Create(
	OFFSET_TYPE	const & type,		// �^�C�v
	D3DXVECTOR3 const & pos,		// �ʒu
	D3DXVECTOR2 const & size,		// �T�C�Y
	int			const & nTextype,	// �e�N�X�`���[�^�C�v
	float		const & frot,		// �p�x
	D3DXCOLOR	const & col,		// �F
	CScene::LAYER	const & layer	// ���C���[
)
{
	// �ϐ��錾
	CScene_TWO * pScene_Two;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_Two = new CScene_TWO();
	// �ݒ�
	// �V�[���Ǘ��ݒ�
	pScene_Two->ManageSetting(layer);
	pScene_Two->m_offsetType = type;	// �^�C�v
	pScene_Two->m_pos = pos;			// �ʒu
	pScene_Two->m_size = size;			// �T�C�Y
	pScene_Two->m_nTexId = nTextype;	// �e�N�X�`���[�^�C�v
	pScene_Two->m_rot = frot;			// �p�x
	pScene_Two->m_col = col;			// �J���[
	// ����������
	pScene_Two->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Two;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�V�[���p���Ȃ�)����
//	offsettype	: �^�C�v
//	pos			: �ʒu
//	size		: �T�C�Y
//	nTextype	: �e�N�X�`���[�^�C�v
//	frot		: �p�x
//	col			: �F
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO * CScene_TWO::Create_Self(
	OFFSET_TYPE	const & type,		// �^�C�v
	D3DXVECTOR3 const & pos,		// �ʒu
	D3DXVECTOR2 const & size,		// �T�C�Y
	int			const & nTextype,	// �e�N�X�`���[�^�C�v
	float		const & frot,		// �p�x
	D3DXCOLOR	const & col			// �F
)
{
	// �ϐ��錾
	CScene_TWO * pScene_Two;		// �V�[��2D�N���X
									// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_Two = new CScene_TWO;
	// �ݒ�
	pScene_Two->m_offsetType = type;	// �^�C�v
	pScene_Two->m_pos = pos;			// �ʒu
	pScene_Two->m_size = size;			// �T�C�Y
	pScene_Two->m_nTexId = nTextype;	// �e�N�X�`���[�^�C�v
	pScene_Two->m_rot = frot;			// �p�x
	pScene_Two->m_col = col;			// �J���[
	// ����������
	pScene_Two->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Two;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�V�[���p���Ȃ��Astd::unique_ptr)����
// ���߂�l��std::move�Ŏ󂯎��
//	offsettype	: �^�C�v
//	pos			: �ʒu
//	size		: �T�C�Y
//	nTextype	: �e�N�X�`���[�^�C�v
//	frot		: �p�x
//	col			: �F
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CScene_TWO> CScene_TWO::Creat_Unique(
	OFFSET_TYPE	const & type,		// �^�C�v
	D3DXVECTOR3 const & pos,		// �ʒu
	D3DXVECTOR2 const & size,		// �T�C�Y
	int			const & nTextype,	// �e�N�X�`���[�^�C�v
	float		const & frot,		// �p�x
	D3DXCOLOR	const & col			// �F
)
{
	// �ϐ��錾
	std::unique_ptr<CScene_TWO> pScene_Two(new CScene_TWO);		// �V�[��2D�N���X
	// �ݒ�
	pScene_Two->m_offsetType = type;	// �^�C�v
	pScene_Two->m_pos = pos;			// �ʒu
	pScene_Two->m_size = size;			// �T�C�Y
	pScene_Two->m_nTexId = nTextype;	// �e�N�X�`���[�^�C�v
	pScene_Two->m_rot = frot;			// �p�x
	pScene_Two->m_col = col;			// �J���[
										// ����������
	pScene_Two->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_Two;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �A�j���[�V�����X�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Updata_Animation(void)
{
	// �A�j���[�V�������NULL�Ȃ�
	// ->�֐��𔲂���
	if (m_unipAnim == NULL) return;

	// �J�E���g�A�j���A�b�v
	m_unipAnim->nCntAnim++;
	// �J�E���g�A�j�����ő�J�E���g�A�j���Ŋ���؂ꂽ��
	if (m_unipAnim->nCntAnim % m_unipAnim->nMaxCntAnim == 0)
	{
		// �ϐ��錾
		D3DXVECTOR2 FirstPos;	// �����ʒu
		D3DXVECTOR2 EndPos;		// �����ʒu
		// �����A�j���[�V�����J�E���g�A�b�v
		m_unipAnim->nHorizonAnim++;
		if (m_unipAnim->nHorizonAnim >= m_unipAnim->nMaxHorizonAnim)
		{
			// �����A�j���[�V�����J�E���g������
			m_unipAnim->nHorizonAnim = 0;
			// �����A�j���[�V�����J�E���g�A�b�v
			m_unipAnim->nVirticalAnim++;
			if (m_unipAnim->nVirticalAnim >= m_unipAnim->nMaxVirticalAnim)
			{
				// �����A�j���[�V�����J�E���g������
				m_unipAnim->nVirticalAnim = 0;
			}
		}
		// �n�_�ʒu
		FirstPos = D3DXVECTOR2(
			m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize,
			m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize
		);
		// �I�_�ʒu
		EndPos = D3DXVECTOR2(
			m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize + m_unipAnim->fHorizonSize,
			m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize + m_unipAnim->fVirticalSize
		);
		// �e�N�X�`���[���W�ݒ�
		Set_Vtx_Tex(FirstPos, EndPos);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g�Z���^�[����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Offset_Center(VERTEX_2D * pVtx)
{
	// �O�p�֐����g���΂߂̒��������߂�
	m_fLengh = sqrtf((m_size.x * 0.5f) * (m_size.x * 0.5f) + (m_size.y * 0.5f) * (m_size.y * 0.5f));
	m_fAngle = atan2f((m_size.x), (m_size.y));

	// ���_���W
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I�t�Z�b�g������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Offset_Left(VERTEX_2D * pVtx)
{
	// �O�p�֐����g���΂߂̒��������߂�
	m_fLengh = sqrtf((m_size.x) * (m_size.x) + (m_size.y * 0.5f) * (m_size.y * 0.5f));
	m_fAngle = atan2f((m_size.x), (m_size.y * 0.5f));

	// ���_���W
	pVtx[0].pos = m_pos + D3DXVECTOR3(
		sinf(m_rot - D3DX_PI) * m_size.y * 0.5f,
		cosf(m_rot - D3DX_PI) * m_size.y * 0.5f, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(
		sinf(D3DX_PI - m_fAngle + m_rot) * m_fLengh,
		cosf(D3DX_PI - m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(
		sinf(m_rot) * m_size.y * 0.5f,
		cosf(m_rot) * m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(
		sinf(m_fAngle + m_rot) * m_fLengh,
		cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_���W�ݒ�
//	offsettype	: �I�t�Z�b�g�^�C�v
//	pVtx		: ���_���
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Pos(
	OFFSET_TYPE const & offsettype,
	VERTEX_2D * pVtx
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
	// �I�t�Z�b�g�^�C�v
	switch (offsettype)
	{
		// �Z���^�[
	case OFFSET_TYPE_CENTER:
		Offset_Center(pVtx);
		break;
		// ��
	case OFFSET_TYPE_LEFT:
		Offset_Left(pVtx);
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

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�J���[�ݒ�
//	pVtx		: ���_���
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Col(VERTEX_2D * pVtx)
{
	// ���_���NULL�Ȃ�
	if (pVtx == NULL)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
		// ���_�J���[
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;
		// �A�����b�N
		m_pVtxBuff->Unlock();
	}
	// ����ȊO�Ȃ�
	else
	{
		// ���_�J���[
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�J���[�ݒ�(�E�̐F�ƍ��̐F)
//	col_r		: �E�̐F
//	col_l		: ���̐F
//	pVtx		: ���_���
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Col2_RL(
	D3DXCOLOR const &col_r,		// �E�̐F
	D3DXCOLOR const &col_l,		// ���̐F
	VERTEX_2D * pVtx			// 2D���_���
)
{
	// ���_���NULL�Ȃ�
	if (pVtx == NULL)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
		// ���_�J���[
		pVtx[0].col = col_l;
		pVtx[1].col = col_r;
		pVtx[2].col = col_l;
		pVtx[3].col = col_r;
		// �A�����b�N
		m_pVtxBuff->Unlock();
	}
	// ����ȊO�Ȃ�
	else
	{
		// ���_�J���[
		pVtx[0].col = col_l;
		pVtx[1].col = col_r;
		pVtx[2].col = col_l;
		pVtx[3].col = col_r;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�J���[�ݒ�(��̐F�Ɖ��̐F)
//	col_o		: ��̐F
//	col_u		: ���̐F
//	pVtx		: ���_���
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Col2_UD(
	D3DXCOLOR const & col_o,
	D3DXCOLOR const & col_u,
	VERTEX_2D * pVtx
)
{
	// ���_���NULL�Ȃ�
	if (pVtx == NULL)
	{
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);

		// ���_�J���[
		pVtx[0].col = col_o;
		pVtx[1].col = col_o;
		pVtx[2].col = col_u;
		pVtx[3].col = col_u;
		// �A�����b�N
		m_pVtxBuff->Unlock();
	}
	// ����ȊO�Ȃ�
	else
	{
		// ���_�J���[
		pVtx[0].col = col_o;
		pVtx[1].col = col_o;
		pVtx[2].col = col_u;
		pVtx[3].col = col_u;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�N�X�`���[�ݒ�
//	first		: �����̈ʒu
//	last		: �����̈ʒu
//	pVtx		: ���_���
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Tex(
	D3DXVECTOR2 const &first,	// �����̔z�u
	D3DXVECTOR2 const &last,	// �Ō�̔z�u
	VERTEX_2D * pVtx			// ���_���
)
{
	// ���_���NULL�Ȃ�
	if (pVtx == NULL)
	{
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
		// ���_��񂪃k����������
			// �A�����b�N
		m_pVtxBuff->Unlock();
	}
	// ����ȊO�Ȃ�
	else
	{
		// ���_�e�N�X�`���[
		pVtx[0].tex = D3DXVECTOR2(first.x, first.y);
		pVtx[1].tex = D3DXVECTOR2(last.x, first.y);
		pVtx[2].tex = D3DXVECTOR2(first.x, last.y);
		pVtx[3].tex = D3DXVECTOR2(last.x, last.y);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�N�X�`���[�A�j���[�V�����̐ݒ�
//	nMaxCntAnim			: �ő�A�j���[�V�����J�E���g
//	nMaxHorizonAnim		: �ő吅���A�j���[�V�����J�E���g
//	nMaxVirticalAnim	: �ő吂���A�j���[�V�����J�E���g
//	bLoop				: ���[�v���
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::SetTexAnim(
	int	const &nMaxCntAnim,			// �ő�A�j���[�V�����J�E���g
	int	const &nMaxHorizonAnim,		// �ő吅���A�j���[�V�����J�E���g
	int	const &nMaxVirticalAnim,	// �ő吂���A�j���[�V�����J�E���g
	bool const &bLoop				// ���[�v���
)
{
	// �A�j���[�V�������NULL�Ȃ�
	if (m_unipAnim == NULL)
	{
		// �A�j���[�V�������̃������m��
		m_unipAnim.reset(new ANIMATION);
	}
	// �ϐ��錾
	D3DXVECTOR2 FirstPos;
	D3DXVECTOR2 EndPos;
	// �e���ڂ̐ݒ�
	m_unipAnim->nMaxCntAnim = nMaxCntAnim;					// �ő�A�j���[�V�����J�E���g
	m_unipAnim->nMaxHorizonAnim = nMaxHorizonAnim;			// �ő吅���A�j���[�V�����J�E���g
	m_unipAnim->nMaxVirticalAnim = nMaxVirticalAnim;		// �ő吂���A�j���[�V�����J�E���g
	m_unipAnim->fHorizonSize = 1.0f / nMaxHorizonAnim;		// �����A�j���[�V������1�̃T�C�Y
	m_unipAnim->fVirticalSize = 1.0f / nMaxVirticalAnim;	// �����A�j���[�V������1�̃T�C�Y
	m_unipAnim->bLoop = bLoop;								// ���[�v���
	// �n�_�ʒu
	FirstPos = D3DXVECTOR2(
		m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize,
		m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize
	);
	// �I�_�ʒu
	EndPos = D3DXVECTOR2(
		m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize + m_unipAnim->fHorizonSize,
		m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize + m_unipAnim->fVirticalSize
	);
	// �e�N�X�`���[���W�ݒ�
	Set_Vtx_Tex(FirstPos, EndPos);
}