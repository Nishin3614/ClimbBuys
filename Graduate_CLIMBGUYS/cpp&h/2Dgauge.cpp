// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2D�Q�[�W�����̐���[2Dgauge.cpp]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "2Dgauge.h"
#include "scene_two.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int	C2DGauge::m_TexType[CCharacter::CHARACTER_MAX]
{
	56
};	// �e�N�X�`���[�^�C�v

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DGauge::C2DGauge() : CScene::CScene()
{
	// ������
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;			// �V�[��2D���
		m_size[nCnt] = D3DVECTOR2_ZERO;		// �T�C�Y���
		m_col[nCnt] = D3DXCOLOR_INI;		// �J���[���
	}
	m_fSizeLast = 0;						// �T�C�Y�̖ڕW�l
	m_fSizeDiff = 0;						// �T�C�Y�̍���
	m_fConstance = 0;						// �T�C�Y���ς��萔
	m_pos = D3DVECTOR3_ZERO;				// �ʒu���
	m_type = CCharacter::CHARACTER_NPC;			// �L�����N�^�[�^�C�v
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DGauge::~C2DGauge()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::Init(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ���C���E�T�u�E�w�i�F�̃Q�[�W����
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		// 2D�V�[���̐���
		m_aScene_Two[nCnt] = CScene_TWO::Create_Self(
			CScene_TWO::OFFSET_TYPE_LEFT,
			m_pos,
			m_size[nCnt],
			-1,
			0.0f,
			m_col[nCnt]
		);
	}

	CScene_TWO *pFream = NULL;

	switch (m_type)
	{
	case CCharacter::CHARACTER_NPC:
		pFream = CScene_TWO::Create(
			CScene_TWO::OFFSET_TYPE_LEFT,
			D3DXVECTOR3(m_pos.x - 103, m_pos.y - 5.5f, m_pos.z),
			D3DXVECTOR2(680.0f, 250.0f),
			m_TexType[m_type],
			0.0f,
			D3DXCOLOR(1.0f,0.64f,0.0f,1.0f));
		break;
	}

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::Uninit(void)
{
	// ���C���E�T�u�E�w�i�F�̃Q�[�W�I��
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		// 2D�V�[����NULL�`�F�b�N
		// ->�J��
		if (m_aScene_Two[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Release();
			m_aScene_Two[nCnt] = NULL;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::Update(void)
{
	// �Q�[�W�̒�����0�����̏ꍇ������
	if (m_size[GAUGE_UNDERLINE].x < 0)
	{
		return;
	}
	// �v���C���[��HP���ς���Ă��Ȃ��ꍇ�֐��𔲂���
	else if (m_size[GAUGE_UNDERLINE].x == m_fSizeLast)
	{
		return;
	}
	// �����������Z
	m_size[GAUGE_UNDERLINE].x += m_fSizeDiff;
	// �ړI�̃T�C�Y�ʂɍ��킹��
	if (m_size[GAUGE_UNDERLINE].x < m_fSizeLast)
	{
		m_size[GAUGE_UNDERLINE].x = m_fSizeLast;
	}
	// �ʒu�̐ݒ�
	m_aScene_Two[GAUGE_UNDERLINE]->SetSize(m_size[GAUGE_UNDERLINE]);
	// ���_���W�̐ݒ�
	m_aScene_Two[GAUGE_UNDERLINE]->Set_Vtx_Pos(CScene_TWO::OFFSET_TYPE_LEFT);
	// ���C���E�T�u�E�w�i�F�̃Q�[�W�X�V
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		// 2D�V�[����NULL�`�F�b�N
		// ->�X�V
		if (m_aScene_Two[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Update();
		}
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::Draw(void)
{
	// ���C���E�T�u�E�w�i�F�̃Q�[�W�`��
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		// 2D�V�[����NULL�`�F�b�N
		// ->�`��
		if (m_aScene_Two[nCnt] != NULL)
		{
			if (nCnt >= GAUGE_MAINLINE)
			{
				m_aScene_Two[nCnt]->Draw();
			}
		}
	}
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C2DGauge::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����(�V�[���Ǘ�)
//	pos		 : �ʒu
//	size	 : �T�C�Y
//	Maincol	 : ���C���J���[
//	Undercol : �T�u�J���[
//	Blockcol : �w�i�F
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DGauge * C2DGauge::Create(
	D3DXVECTOR3 const &pos,			// �ʒu
	D3DXVECTOR2 const &size,		// �T�C�Y
	D3DXCOLOR	const &Maincol,		// ���C���J���[
	CCharacter::CHARACTER	const &type,		// �L�����N�^�[�^�C�v
	D3DXCOLOR	const &Undercol,	// �T�u�J���[
	D3DXCOLOR	const &Blockcol		// �w�i�F
)
{
	// �ϐ��錾
	C2DGauge * p2DGauge;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	p2DGauge = new C2DGauge();
	// �V�[���Ǘ��ݒ�
	p2DGauge->ManageSetting(CScene::LAYER_3DOBJECT);
	// �ݒ�h
	p2DGauge->SetCol(Maincol, Undercol, Blockcol);	// �J���[
	p2DGauge->m_pos = pos;							// �ʒu
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		p2DGauge->m_size[nCntGauge] = size;			// �T�C�Y
	}
	p2DGauge->m_fSizeLast = size.x;					// �T�C�Y�̖ڕW�l
	p2DGauge->m_type = type;						// �^�C�v�ݒ�
	// ����������
	p2DGauge->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return p2DGauge;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����(�l�Ǘ�)
//	pos		 : �ʒu
//	size	 : �T�C�Y
//	Maincol	 : ���C���J���[
//	Undercol : �T�u�J���[
//	Blockcol : �w�i�F
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DGauge * C2DGauge::Create_Self(
	D3DXVECTOR3 const &pos,			// �ʒu
	D3DXVECTOR2 const &size,		// �T�C�Y
	D3DXCOLOR	const &Maincol,		// ���C���J���[
	D3DXCOLOR	const &Undercol,	// �T�u�J���[
	D3DXCOLOR	const &Blockcol		// �w�i�F
)
{
	// �ϐ��錾
	C2DGauge * p2DGauge;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	p2DGauge = new C2DGauge();
	// �ݒ�h
	p2DGauge->SetCol(Maincol, Undercol, Blockcol);	// �J���[
	p2DGauge->m_pos = pos;							// �ʒu
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		p2DGauge->m_size[nCntGauge] = size;			// �T�C�Y
	}
	p2DGauge->m_fSizeLast = size.x;					// �T�C�Y�̖ڕW�l
	// ����������
	p2DGauge->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return p2DGauge;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �Q�[�W�ω�
//	fValue	: �X�e�[�^�X�̒l
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::ChangeGauge(
	float const &fValue	// �X�e�[�^�X�̒l
)
{
	// �T�C�Y�̍X�V
	m_fSizeLast = m_fConstance * fValue;
	// �ڕW�̃Q�[�W�̃T�C�Y��0�����̏ꍇ������
	if (m_fSizeLast < 0)
	{
		return;
	}
	// ���݂̃T�C�Y�ɑ��(��)
	m_size[GAUGE_MAINLINE].x = m_fSizeLast;
	// ��������
	m_fSizeDiff =
		(m_fSizeLast - m_size[GAUGE_UNDERLINE].x) /
		DERAY_TIME(1);
	// �ʒu�̐ݒ�
	m_aScene_Two[GAUGE_MAINLINE]->SetSize(m_size[GAUGE_MAINLINE]);
	// ���_���W�̐ݒ�
	m_aScene_Two[GAUGE_MAINLINE]->Set_Vtx_Pos(CScene_TWO::OFFSET_TYPE_LEFT);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �Q�[�W�̏����ݒ�
//	fValue	: �X�e�[�^�X�̒l
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::BeginGauge(
	float const &fValue	// �X�e�[�^�X�̒l
)
{
	// �T�C�Y�̍X�V
	m_fSizeLast = m_fConstance * fValue;
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		// �Q�[�W�u���b�N�̏ꍇ�X�L�b�v
		if (nCntGauge == GAUGE_BLOCK)
		{
			continue;
		}
		// ���݂̃T�C�Y�ɑ��(��)
		m_size[nCntGauge].x = m_fSizeLast;
		// �ʒu�̐ݒ�
		m_aScene_Two[nCntGauge]->SetSize(m_size[nCntGauge]);
		// ���_���W�̐ݒ�
		m_aScene_Two[nCntGauge]->Set_Vtx_Pos(CScene_TWO::OFFSET_TYPE_LEFT);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �Q�[�W�̒����̌Œ�l�ݒ�
//	fMaxValue : �X�e�[�^�X�̍ő�l
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::SetConstance(
	float const &fMaxValue	// �X�e�[�^�X�̍ő�l
)
{
	// �萔�ݒ�
	m_fConstance = m_size[GAUGE_BLOCK].x / fMaxValue;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���C���J���[�ݒ�
//	MainCol_R	: �E�̃��C���J���[
//	MainCol_L	: ���̃��C���J���[
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::SetMainCol(
	D3DXCOLOR const & MainCol_R,	// ���C���J���[�E
	D3DXCOLOR const & MainCol_L		// ���C���J���[��
)
{
	// ���_�J���[�ݒ�(�E�̐F�ƍ��̐F)
	m_aScene_Two[2]->Set_Vtx_Col2_RL(
		MainCol_R,
		MainCol_L
	);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���C���A�T�u�A�w�i�̃J���[�ݒ�
//	Maincol	: ���C���J���[
//	Subcp;	: �T�u�J���[
//	BGcol	: �w�i�F
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::SetCol(
	D3DXCOLOR	const &Maincol,		// ���C���̐F
	D3DXCOLOR	const &Subcol,		// �T�u�J���[
	D3DXCOLOR	const &BGcol		// �w�i�F
)
{
	m_col[0] = BGcol;
	m_col[1] = Subcol;
	m_col[2] = Maincol;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu�ݒ�
//	pos	: �ʒu
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DGauge::SetPos(
	D3DXVECTOR3 const & pos	// �ʒu
)
{
	// �ʒu�ݒ�
	m_pos = pos;
	// �e�Q�[�W�̈ʒu�ݒ�
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		// �ʒu�ݒ�
		m_aScene_Two[nCnt]->SetPosition(m_pos);
		// ���_���W�̐ݒ�
		m_aScene_Two[nCnt]->Set_Vtx_Pos(CScene_TWO::OFFSET_TYPE_LEFT);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu�擾����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 & C2DGauge::GetPos(void)
{
	return m_pos;
}