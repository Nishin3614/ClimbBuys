// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2D�v���[���c�����̐���[2Dpresents.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "2Dpresents.h"
#include "manager.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C�j�V�����C�U�R���X�g���N�^����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents::C2DPresents() : CScene_TWO::CScene_TWO()
{
	// �ϐ��̏�����
	m_pCooperation = NULL;
	m_pFadeIn = NULL;
	m_pFadeOut = NULL;
	m_Fadetype = FADETYPE_COOPERATION;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents::~C2DPresents()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Init(void)
{
	CScene_TWO::Init();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Uninit(void)
{
	// �V�[��2D�I������
	CScene_TWO::Uninit();
	// �������̊J��
	if (m_pCooperation != NULL)
	{
		// �_�ŏ��̊J��
		if (m_pCooperation->pCol_flash != NULL)
		{
			delete m_pCooperation->pCol_flash;
			m_pCooperation->pCol_flash = NULL;
		}
		// �g�嗦���̊J��
		if (m_pCooperation->pScal != NULL)
		{
			delete m_pCooperation->pScal;
			m_pCooperation->pScal = NULL;
		}
		delete m_pCooperation;
		m_pCooperation = NULL;
	}
	// �t�F�[�h�C���̊J��
	if (m_pFadeIn != NULL)
	{
		// �t�F�[�h�C��(�w��F)�̊J��
		if(m_pFadeIn->pDesign_Col != NULL)
		{
			delete m_pFadeIn->pDesign_Col;
			m_pFadeIn->pDesign_Col = NULL;
		}
		// �t�F�[�h�C��(�ړ�)�̊J��
		if (m_pFadeIn->pDesign_Move != NULL)
		{
			delete m_pFadeIn->pDesign_Move;
			m_pFadeIn->pDesign_Move = NULL;
		}
		delete m_pFadeIn;
		m_pFadeIn = NULL;
	}
	// �t�F�[�h�A�E�g�̊J��
	if (m_pFadeOut != NULL)
	{
		// �t�F�[�h�A�E�g(�w��F)�̊J��
		if (m_pFadeOut->pDesign_Col != NULL)
		{
			delete m_pFadeOut->pDesign_Col;
			m_pFadeOut->pDesign_Col = NULL;
		}
		// �t�F�[�h�A�E�g(�ړ�)�̊J��
		if (m_pFadeOut->pDesign_Move != NULL)
		{
			delete m_pFadeOut->pDesign_Move;
			m_pFadeOut->pDesign_Move = NULL;
		}
		delete m_pFadeOut;
		m_pFadeOut = NULL;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update(void)
{
	switch (m_Fadetype)
	{
	case FADETYPE_IN:
		Update_FadeIn();
		break;
	case FADETYPE_COOPERATION:
		Update_Cooperation();
		break;
	case FADETYPE_OUT:
		Update_FadeOut();
		break;
	default:
		break;
	}
	// �V�[��2D�X�V
	CScene_TWO::Update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Draw(void)
{
	// �V�[��2D�X�V
	CScene_TWO::Draw();
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Debug(void)
{
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���ő�t���[�����擾
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInMaxFram(void)
{
	if (m_pFadeIn != NULL)
	{
		return m_pFadeIn->nMaxFade;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���t���[�����擾
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInFram(void)
{
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���ő�J�n�A�j���^�C�����擾
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInMaxAnimTime(void)
{
	if (m_pFadeIn != NULL)
	{
		return m_pFadeIn->nMaxAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���J�n�A�j���^�C�����擾
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInAnimTime(void)
{
	if (m_pFadeIn != NULL)
	{
		return m_pFadeIn->nCntAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g�ő�J�n�A�j���^�C�����擾
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeOutMaxAnimTime(void)
{
	if (m_pFadeOut != NULL)
	{
		return m_pFadeOut->nCntAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g�J�n�A�j���^�C�����擾
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeOutAnimTime(void)
{
	if (m_pFadeOut != NULL)
	{
		return m_pFadeOut->nCntAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�V�[���Ǘ�)����
//	offsettype	: �^�C�v
//	pos			: �ʒu
//	size		: �T�C�Y
//	nTextype	: �e�N�X�`���[�^�C�v
//	frot		: �p�x
//	col			: �F
//	layer		: ���C���[
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents * C2DPresents::Create(
	OFFSET_TYPE		const & type,		// �^�C�v
	D3DXVECTOR3		const & pos,		// �ʒu
	D3DXVECTOR2		const & size,		// �T�C�Y
	int				const & nTextype,	// �e�N�X�`���[�^�C�v
	float			const & frot,		// �p�x
	D3DXCOLOR		const & col,		// �F
	CScene::LAYER	const & layer		// ���C���[
)
{
	// �ϐ��錾
	C2DPresents * p2DPresents;		// 2D�v���[���c2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	p2DPresents = new C2DPresents;
	// �V�[���Ǘ��ݒ�
	p2DPresents->ManageSetting(layer);
	// �ݒ�
	p2DPresents->SetOffset(type);		// �^�C�v
	p2DPresents->SetPosition(pos);		// �ʒu
	p2DPresents->SetSize(size);			// �T�C�Y
	p2DPresents->BindTexture(nTextype);	// �e�N�X�`���[�^�C�v
	p2DPresents->SetRot(frot);			// �p�x
	p2DPresents->SetCol(col);			// �J���[
	// ����������
	p2DPresents->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return p2DPresents;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�l�Ǘ�)����
//	offsettype	: �^�C�v
//	pos			: �ʒu
//	size		: �T�C�Y
//	nTextype	: �e�N�X�`���[�^�C�v
//	frot		: �p�x
//	col			: �F
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents * C2DPresents::Create_Self(
	OFFSET_TYPE		const & type,		// �^�C�v
	D3DXVECTOR3		const & pos,		// �ʒu
	D3DXVECTOR2		const & size,		// �T�C�Y
	int				const & nTextype,	// �e�N�X�`���[�^�C�v
	float			const & frot,		// �p�x
	D3DXCOLOR		const & col			// �F
)
{
	// �ϐ��錾
	C2DPresents * p2DPresents;		// 2D�v���[���c2D�N���X
									// �������̐���(����->��{�N���X,��->�h���N���X)
	p2DPresents = new C2DPresents;
	// �ݒ�
	p2DPresents->SetOffset(type);		// �^�C�v
	p2DPresents->SetPosition(pos);		// �ʒu
	p2DPresents->SetSize(size);			// �T�C�Y
	p2DPresents->BindTexture(nTextype);	// �e�N�X�`���[�^�C�v
	p2DPresents->SetRot(frot);			// �p�x
	p2DPresents->SetCol(col);			// �J���[
	// ����������
	p2DPresents->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return p2DPresents;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬(�l�Ǘ�)����
//	offsettype	: �^�C�v
//	pos			: �ʒu
//	size		: �T�C�Y
//	nTextype	: �e�N�X�`���[�^�C�v
//	frot		: �p�x
//	col			: �F
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<C2DPresents> C2DPresents::Create_Unique(
	OFFSET_TYPE		const & type,		// �^�C�v
	D3DXVECTOR3		const & pos,		// �ʒu
	D3DXVECTOR2		const & size,		// �T�C�Y
	int				const & nTextype,	// �e�N�X�`���[�^�C�v
	float			const & frot,		// �p�x
	D3DXCOLOR		const & col			// �F
)
{
	// �ϐ��錾
	std::unique_ptr<C2DPresents> p2DPresents(new C2DPresents);		// 2D�v���[���c2D�N���X
	// �ݒ�
	p2DPresents->SetOffset(type);		// �^�C�v
	p2DPresents->SetPosition(pos);		// �ʒu
	p2DPresents->SetSize(size);			// �T�C�Y
	p2DPresents->BindTexture(nTextype);	// �e�N�X�`���[�^�C�v
	p2DPresents->SetRot(frot);			// �p�x
	p2DPresents->SetCol(col);			// �J���[
	// ����������
	p2DPresents->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return p2DPresents;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �_�ōX�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_ColorFlashing(void)
{
	// �k����������֐��𔲂���
	if (m_pCooperation->pCol_flash == NULL)
	{
		return;
	}
	// �ϐ��錾
	D3DXCOLOR col;	// �F
	// ���擾
	col = CScene_TWO::GetCol();

	// �؂�ւ�OFF��
	if (!m_pCooperation->pCol_flash->bSwitch)
	{
		// �؂�ւ�ON��
		if (m_pCooperation->pCol_flash->nCntTimeSwitch == m_pCooperation->pCol_flash->nTimeSwitch)
		{
			// �؂�ւ�ON
			m_pCooperation->pCol_flash->bSwitch = true;
			// �؂�ւ��OFF��
			if (!m_pCooperation->pCol_flash->bChange)
			{
				// �����v�Z
				m_pCooperation->pCol_flash->colDiff =
					(m_pCooperation->pCol_flash->changecol - m_pCooperation->pCol_flash->Origincol) / (float)m_pCooperation->pCol_flash->nTimeChange;
			}
			// �؂�ւ��ON��
			else
			{
				// �����v�Z
				m_pCooperation->pCol_flash->colDiff =
					(m_pCooperation->pCol_flash->Origincol - m_pCooperation->pCol_flash->changecol) / (float)m_pCooperation->pCol_flash->nTimeChange;
			}
		}
		// �؂�ւ����ԉ��Z
		m_pCooperation->pCol_flash->nCntTimeSwitch++;
		// �֐��𔲂���
		return;
	}
	// �؂�ւ�ON��
	else
	{
		// �؂�ւ�OFF|�؂�ւ��ON/OFF
		if (m_pCooperation->pCol_flash->nCntTimeChange == m_pCooperation->pCol_flash->nTimeChange)
		{
			// �؂�ւ�OFF
			m_pCooperation->pCol_flash->bSwitch = false;
			// �؂�ւ��OFF��
			if (!m_pCooperation->pCol_flash->bChange)
			{
				// �ڕW�̐F�ɋ����ω�
				col = m_pCooperation->pCol_flash->changecol;
			}
			// �؂�ւ��ON��
			else
			{
				// �ڕW�̐F�ɋ����ω�
				col = m_pCooperation->pCol_flash->Origincol;
			}
			// �؂�ւ��ON/OFF
			m_pCooperation->pCol_flash->bChange = !m_pCooperation->pCol_flash->bChange;
			// �J�E���g������
			m_pCooperation->pCol_flash->nCntTimeSwitch = 0;
			m_pCooperation->pCol_flash->nCntTimeChange = 0;
			// �֐��𔲂���
			return;
		}
		// �؂�ւ�鎞�ԉ��Z
		m_pCooperation->pCol_flash->nCntTimeChange++;
	}
	// �F�̕ω�
	col += m_pCooperation->pCol_flash->colDiff;
	// �F�̐ݒ�
	CScene_TWO::SetCol(col);
	// ���_�J���[�̐ݒ�
	CScene_TWO::Set_Vtx_Col();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �g��k�����X�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_Scaling(void)
{
	// �k����������֐��𔲂���
	if (m_pCooperation->pScal == NULL)
	{
		return;
	}

	// �؂�ւ�OFF|�؂�ւ��ON/OFF
	if (m_pCooperation->pScal->nCntTimeChange == m_pCooperation->pScal->nTimeChange)
	{
		// �؂�ւ��ON/OFF
		m_pCooperation->pScal->bChange = !m_pCooperation->pScal->bChange;
		// �؂�ւ��OFF��
		if (!m_pCooperation->pScal->bChange)
		{
			// �g�嗦�̍���
			m_pCooperation->pScal->fScalDiff = (m_pCooperation->pScal->fScalChange - m_pCooperation->pScal->fScal) / m_pCooperation->pScal->nTimeChange;
			// �ڕW�̊g�嗦�ɋ����ω�
			m_pCooperation->pScal->fScal = 1.0f;
		}
		// �؂�ւ��ON��
		else
		{
			// �ڕW�̊g�嗦�ɋ����ω�
			m_pCooperation->pScal->fScal = m_pCooperation->pScal->fScalChange;
			// �g�嗦�̍���
			m_pCooperation->pScal->fScalDiff = (1.0f - m_pCooperation->pScal->fScalChange) / m_pCooperation->pScal->nTimeChange;
		}
		// �J�E���g������
		m_pCooperation->pScal->nCntTimeChange = 0;
		// �֐��𔲂���
		return;
	}
	// �؂�ւ�鎞�ԉ��Z
	m_pCooperation->pScal->nCntTimeChange++;
	// �g�嗦�̕ω�
	m_pCooperation->pScal->fScal += m_pCooperation->pScal->fScalDiff;
	// �T�C�Y�̕ω�
	CScene_TWO::SetSize(m_pCooperation->pScal->OriginSize * m_pCooperation->pScal->fScal);
	// ���_�J���[�̐ݒ�
	CScene_TWO::Set_Vtx_Pos();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h(�w��F)����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_Fade_DesignCol(
	D3DXCOLOR const & DiffCol ,	// �����F
	D3DXCOLOR const & EndCol,	// �ڕW�F
	int & nFram,				// ���݂̃t���[��
	int const & nMaxFram		// �ő�̃t���[��
)
{
	// �ϐ��錾
	D3DXCOLOR col;	// �F
	// ���擾
	col = CScene_TWO::GetCol();
	// �ő�t���[���ɂȂ�����ڕW�F�ɋ����I�ɕς���
	if (nFram == nMaxFram)
	{
		col = EndCol;
		// �F�̐ݒ�
		CScene_TWO::SetCol(col);
		// ���_�J���[�̐ݒ�
		Set_Vtx_Col();
		// �t���[�����Z
		nFram++;
		return;
	}
	// �t���[�����Ԃ̐���
	else if (nFram > nMaxFram)
	{
		return;
	}
	// �������Z
	col += DiffCol;
	// �F�̐ݒ�
	CScene_TWO::SetCol(col);
	// ���_�J���[�̐ݒ�
	CScene_TWO::Set_Vtx_Col();
	// �t���[�����Z
	nFram++;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h(�ړ�)����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_Fade_DesignMove(
	D3DXVECTOR3 const & DiffPos,	// �����F
	D3DXVECTOR3 const & GoalPos,	// �ڕW�F
	int & nFram,					// ���݂̃t���[��
	int const & nMaxFram			// �ő�̃t���[��
)
{
	// �ϐ��錾
	D3DXVECTOR3 pos;					// �ʒu
	// ���擾
	pos = CScene_TWO::GetPosition();	// �ʒu
	// �ő�t���[���ɂȂ�����ڕW�F�ɋ����I�ɕς���
	if (nFram == nMaxFram)
	{
		pos = GoalPos;
		// �ʒu�̐ݒ�
		CScene_TWO::SetPosition(pos);
		// ���_���W�̐ݒ�
		Set_Vtx_Pos();
		// �t���[�����Z
		nFram++;
		return;
	}
	// �t���[�����Ԃ̐���
	else if (nFram > nMaxFram)
	{
		return;
	}
	// �������Z
	pos += DiffPos;
	// �ʒu�̐ݒ�
	CScene_TWO::SetPosition(pos);
	// ���_���W�̐ݒ�
	CScene_TWO::Set_Vtx_Pos();
	// �t���[�����Z
	nFram++;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���X�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::Update_FadeIn(void)
{
	// �t�F�[�h�C�����k����������֐��𔲂���
	if (m_pFadeIn == NULL)
	{
		m_Fadetype = FADETYPE_COOPERATION;
		return false;
	}
	// �����I������
	if (m_pFadeIn->bCompulsion)
	{
		if (CManager::GetKeyConfig()->GetKeyConfigTrigger(CKeyConfig::CONFIG_DECISION))
		{
			// �t�F�[�h�C���̋����I��
			Compulsion_FadeIn();
		}
	}
	// �t�F�[�h������������֐��𔲂���
	if (!m_pFadeIn->bFade)
	{
		return false;
	}
	// �t�F�[�h�C�����A�j���X�^�[�g����
	if (m_pFadeIn->nCntAnimStart < m_pFadeIn->nMaxAnimStart)
	{
		m_pFadeIn->nCntAnimStart++;
		return true;
	}
	// �w��F���k������Ȃ�������֐��ɓ���
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		// �t�F�[�h(�w��F)����
		Update_Fade_DesignCol(
			m_pFadeIn->pDesign_Col->DiffCol,
			m_pFadeIn->pDesign_Col->EndCol,
			m_pFadeIn->pDesign_Col->nfram,
			m_pFadeIn->pDesign_Col->nMaxfram
		);
	}
	// �ړ����k������Ȃ�������֐��ɓ���
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		// �t�F�[�h(�w��F)����
		Update_Fade_DesignMove(
			m_pFadeIn->pDesign_Move->DiffPos,
			m_pFadeIn->pDesign_Move->EndPos,
			m_pFadeIn->pDesign_Move->nfram,
			m_pFadeIn->pDesign_Move->nMaxfram
		);
	}
	// �t�F�[�h�J�E���g�̉��Z
	m_pFadeIn->nCntFade++;
	// �t�F�[�h���Ԃ��I�����Ă�����֐��𔲂���
	if (m_pFadeIn->nCntFade >= m_pFadeIn->nMaxFade)
	{
		m_pFadeIn->bFade = false;
		// �t�F�[�h�^�C�v
		m_Fadetype = FADETYPE_COOPERATION;
	}
	// �t�F�[�h��Ԃ�Ԃ�
	return m_pFadeIn->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::Update_FadeOut(void)
{
	// �k����������֐��𔲂���
	if (m_pFadeOut == NULL)
	{
		m_Fadetype = FADETYPE_END;
		return false;
	}
	// �����I������
	if (m_pFadeOut->bCompulsion)
	{
		if (CManager::GetKeyConfig()->GetKeyConfigTrigger(CKeyConfig::CONFIG_DECISION))
		{
			// �t�F�[�h�C���̋����I��
			Compulsion_FadeOut();
		}
	}
	// �t�F�[�h������������֐��𔲂���
	if (!m_pFadeOut->bFade)
	{
		return false;
	}
	// �t�F�[�h�C�����A�j���X�^�[�g����
	if (m_pFadeOut->nCntAnimStart < m_pFadeOut->nMaxAnimStart)
	{
		m_pFadeOut->nCntAnimStart++;
		return true;
	}
	// �w��F���k������Ȃ�������֐��ɓ���
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		// �t�F�[�h(�w��F)����
		Update_Fade_DesignCol(
			m_pFadeOut->pDesign_Col->DiffCol,
			m_pFadeOut->pDesign_Col->EndCol,
			m_pFadeOut->pDesign_Col->nfram,
			m_pFadeOut->pDesign_Col->nMaxfram
		);
	}
	// �ړ����k������Ȃ�������֐��ɓ���
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		// �t�F�[�h(�ړ�)����
		Update_Fade_DesignMove(
			m_pFadeOut->pDesign_Move->DiffPos,
			m_pFadeOut->pDesign_Move->EndPos,
			m_pFadeOut->pDesign_Move->nfram,
			m_pFadeOut->pDesign_Move->nMaxfram
		);
	}
	// �t�F�[�h�J�E���g�̉��Z
	m_pFadeOut->nCntFade++;
	// �t�F�[�h���Ԃ��I��������֐��𔲂���
	if (m_pFadeOut->nCntFade >= m_pFadeOut->nMaxFade)
	{
		m_pFadeOut->nCntAnimStart = 0;
		m_pFadeOut->bFade = false;
		// �t�F�[�h�^�C�v
		m_Fadetype = FADETYPE_END;
	}
	// �t�F�[�h��Ԃ�Ԃ�
	return m_pFadeOut->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����X�V����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::Update_Cooperation(void)
{
	// �k����������֐��𔲂���
	if (m_pCooperation == NULL)
	{
		return false;
	}
	// �_�ŏ�񂪃k������Ȃ�������֐��ɓ���
	if (m_pCooperation->pCol_flash != NULL)
	{
		// �F�̓_�ŏ���
		Update_ColorFlashing();
	}
	// �g�嗦��񂪃k������Ȃ�������֐��ɓ���
	if (m_pCooperation->pScal != NULL)
	{
		// �g�嗦�X�V
		Update_Scaling();
	}
	return false;
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C��(�ړ�)�ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn(
	D3DXVECTOR3 const & BeginPos,	// �����ʒu
	D3DXVECTOR3 const & EndPos,		// �ړI�ʒu
	int const & nMaxfram			// �ő�t���[��
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	// �t�F�[�h�A�E�g(�ʒu)�̃��������m�ۍς݂������甲����
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		return;
	}
	// �t�F�[�h�C��(�ړ�)���̃������m��
	m_pFadeIn->pDesign_Move = new DESIGN_MOVE;
	// �ړI�ʒu�ݒ�
	m_pFadeIn->pDesign_Move->EndPos = EndPos;
	// �����ʒu�ݒ�
	CScene_TWO::SetPosition(m_pFadeIn->pDesign_Move->BeginPos = BeginPos);
	// ���_���W�̐ݒ�
	CScene_TWO::Set_Vtx_Pos();
	// �ő�t���[��
	m_pFadeIn->pDesign_Move->nMaxfram = nMaxfram;
	// �����v�Z
	m_pFadeIn->pDesign_Move->DiffPos =
		(m_pFadeIn->pDesign_Move->EndPos -
		m_pFadeIn->pDesign_Move->BeginPos) /
		(float)m_pFadeIn->pDesign_Move->nMaxfram;
	// �t�F�[�h����
	if (m_pFadeIn->nMaxFade < m_pFadeIn->pDesign_Move->nMaxfram)
	{
		m_pFadeIn->nMaxFade = m_pFadeIn->pDesign_Move->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C��(�F�w��)�ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn(
	D3DXCOLOR const & BeginCol,		// �����J���[
	D3DXCOLOR const & EndCol,		// �ڕW�J���[
	int const & nMaxfram			// �ő�t���[��
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	// �t�F�[�h�C��(�ʒu)�̃��������m�ۍς݂������甲����
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		return;
	}
	// �t�F�[�h�C��(�ʒu)�̃������m��
	m_pFadeIn->pDesign_Col = new DESIGN_COL;
	// �ő�t���[����
	m_pFadeIn->pDesign_Col->nMaxfram = nMaxfram;
	// �F�̐ݒ�
	m_pFadeIn->pDesign_Col->BeginCol = BeginCol;
	// �����J���[
	m_pFadeIn->pDesign_Col->EndCol = EndCol;
	// �ő�t���[��
	m_pFadeIn->pDesign_Col->nMaxfram = nMaxfram;
	// �����v�Z
	m_pFadeIn->pDesign_Col->DiffCol =
		(m_pFadeIn->pDesign_Col->EndCol -
			m_pFadeIn->pDesign_Col->BeginCol) /
			(float)m_pFadeIn->pDesign_Col->nMaxfram;
	// �t�F�[�h����
	if (m_pFadeIn->nMaxFade < m_pFadeIn->pDesign_Col->nMaxfram)
	{
		m_pFadeIn->nMaxFade = m_pFadeIn->pDesign_Col->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���̃X�^�[�g���Ԑݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn_StarAnim(
	int const & nMaxStartAnim
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	// �A�j���X�^�[�g����
	m_pFadeIn->nMaxAnimStart = nMaxStartAnim;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C�������I���ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn_Compulsion(
	bool const & bCompulsion
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	m_pFadeIn->bCompulsion = bCompulsion;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���̎g�p��Ԏ擾����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::GetFadeIn_Bool(void)
{
	if (m_pFadeIn == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2D�v���[���c2D���̃t�F�[�h�C����񂪂���܂���B\n�g�p��Ԃ��擾�ł��܂���B");
#endif // _DEBUG
		return false;
	}
	// �����I���ݒ�
	return m_pFadeIn->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C�������I������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Compulsion_FadeIn(void)
{
	if (m_pFadeIn == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2D�v���[���c2D���̃t�F�[�h�C����񂪂���܂���B\n�g�p��Ԃ�OFF�ɂł��܂���ł����B");
#endif // _DEBUG
		return;
	}
	// �t�F�[�h�C���̎g�p��Ԃ�false��
	m_pFadeIn->bFade = false;
	// �A�j���X�^�[�g���Ԃ��ő��
	m_pFadeIn->nCntAnimStart = m_pFadeIn->nMaxAnimStart;
	// �t�F�[�h�C���̎w��F���NULL�ł͂Ȃ��Ƃ�
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		// �F�̐ݒ�
		CScene_TWO::SetCol(m_pFadeIn->pDesign_Col->EndCol);
		// ���_�J���[�̐ݒ�
		CScene_TWO::Set_Vtx_Col();
	}
	// �t�F�[�h�C���̎w��F���NULL�ł͂Ȃ��Ƃ�
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		CScene_TWO::SetPosition(m_pFadeIn->pDesign_Move->EndPos);
		// ���_���W�̐ݒ�
		CScene_TWO::Set_Vtx_Pos();
	}
	// �t�F�[�h�^�C�v
	m_Fadetype = FADETYPE_COOPERATION;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C��(�ʒu)�J�n����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Start_FadeIn(void)
{
	// �t�F�[�h�C����NULL�Ȃ�
	// ->�֐��𔲂���
	if (m_pFadeIn == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2D�v���[���c2D���̃t�F�[�h�C����񂪂���܂���B\n�t�F�[�h�C���J�n���ł��܂���ł����B");
#endif // _DEBUG
		return;
	}

	// �t�F�[�h�C��(�ʒu)������
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		// �����ʒu�ݒ�
		CScene_TWO::SetPosition(m_pFadeIn->pDesign_Move->BeginPos);
		// ���_���W�̐ݒ�
		CScene_TWO::Set_Vtx_Pos();
		// �t���[���̏�����
		m_pFadeIn->pDesign_Move->nfram = 0;
	}
	// �t�F�[�h�C��(�J���[)������
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		// �F�̐ݒ�
		CScene_TWO::SetCol(m_pFadeIn->pDesign_Col->BeginCol);
		// ���_�J���[�̐ݒ�
		CScene_TWO::Set_Vtx_Col();
		// �t���[���̏�����
		m_pFadeIn->pDesign_Col->nfram = 0;
	}
	// �t�F�[�h��
	m_pFadeIn->bFade = true;
	// �J�E���g���Z�b�g
	m_pFadeIn->nCntFade = 0;
	// �J�E���g���Z�b�g
	m_pFadeIn->nCntAnimStart = 0;
	// �t�F�[�h�^�C�v
	m_Fadetype = FADETYPE_IN;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g(�ړ�)�ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut(
	D3DXVECTOR3 const & BeginPos,	// �����ʒu
	D3DXVECTOR3 const & EndPos,		// �ړI�ʒu
	int const & nMaxfram			// �ő�t���[��
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// �t�F�[�h�A�E�g(�ʒu)�̃��������m�ۍς݂������甲����
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		return;
	}
	// �t�F�[�h�C��(�ړ�)���̃������m��
	m_pFadeOut->pDesign_Move = new DESIGN_MOVE;
	// �ړI�ʒu�ݒ�
	m_pFadeOut->pDesign_Move->EndPos = EndPos;
	// �����ʒu�ݒ�
	m_pFadeOut->pDesign_Move->BeginPos = BeginPos;
	// �ő�t���[��
	m_pFadeOut->pDesign_Move->nMaxfram = nMaxfram;
	// �����v�Z
	m_pFadeOut->pDesign_Move->DiffPos =
		(m_pFadeOut->pDesign_Move->EndPos -
		m_pFadeOut->pDesign_Move->BeginPos) /
		(float)m_pFadeOut->pDesign_Move->nMaxfram;
	// �t�F�[�h����
	if (m_pFadeOut->nMaxFade < m_pFadeOut->pDesign_Move->nMaxfram)
	{
		m_pFadeOut->nMaxFade = m_pFadeOut->pDesign_Move->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g(�F�w��)�ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut(
	D3DXCOLOR const & BeginCol,		// �����J���[
	D3DXCOLOR const & EndCol,		// �ڕW�J���[
	int const & nMaxfram			// �ő�t���[��
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// �t�F�[�h�A�E�g(�F�w��)�̃��������m�ۍς݂������甲����
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		return;
	}
	// �t�F�[�h�A�E�g(�F�w��)�̃������m��
	m_pFadeOut->pDesign_Col = new DESIGN_COL;
	// �����J���[
	m_pFadeOut->pDesign_Col->BeginCol = BeginCol;
	// �����J���[
	m_pFadeOut->pDesign_Col->EndCol = EndCol;
	// �ő�t���[��
	m_pFadeOut->pDesign_Col->nMaxfram = nMaxfram;
	// �����v�Z
	m_pFadeOut->pDesign_Col->DiffCol =
		(m_pFadeOut->pDesign_Col->EndCol -
			m_pFadeOut->pDesign_Col->BeginCol) /
		(float)m_pFadeOut->pDesign_Col->nMaxfram;
	// �t�F�[�h����
	if (m_pFadeOut->nMaxFade < m_pFadeOut->pDesign_Col->nMaxfram)
	{
		m_pFadeOut->nMaxFade = m_pFadeOut->pDesign_Col->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g�̃X�^�[�g���Ԑݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut_StarAnim(
	int const & nMaxStartAnim
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// �A�j���X�^�[�g����
	m_pFadeOut->nMaxAnimStart = nMaxStartAnim;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C�������I���ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut_Compulsion(
	bool const & bCompulsion
)
{
	// �t�F�[�h���̃������m��
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// �����I���ݒ�
	m_pFadeOut->bCompulsion = bCompulsion;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�C���̎g�p��Ԑݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::GetFadeOut_Bool(void)
{
	if (m_pFadeOut == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2D�v���[���c2D���̃t�F�[�h�C����񂪂���܂���B\n�g�p��Ԃ��擾�ł��܂���B");
#endif // _DEBUG
		return false;
	}
	return m_pFadeOut->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g�����I������
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Compulsion_FadeOut(void)
{
	if (m_pFadeOut == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2D�v���[���c2D���̃t�F�[�h�A�E�g��񂪂���܂���B\n�g�p��Ԃ�OFF�ɂł��܂���ł����B");
#endif // _DEBUG
		return;
	}
	// �t�F�[�h�A�E�g�̎g�p��Ԃ�false��������֐��𔲂���
	else if (!m_pFadeOut->bFade)
	{
		return;
	}
	// �t�F�[�h�C���̎g�p��Ԃ�false��
	m_pFadeOut->bFade = false;
	// �A�j���X�^�[�g���Ԃ��ő��
	m_pFadeOut->nCntAnimStart = 0;
	// �J�E���g���Z�b�g
	m_pFadeOut->nCntFade = 0;
	// �t�F�[�h�C���̎w��F���NULL�ł͂Ȃ��Ƃ�
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		// �F�̐ݒ�
		CScene_TWO::SetCol(m_pFadeOut->pDesign_Col->EndCol);
		// ���_�J���[�̐ݒ�
		CScene_TWO::Set_Vtx_Col();
		// �t���[���̏�����
		m_pFadeOut->pDesign_Col->nfram = 0;
	}
	// �t�F�[�h�C���̎w��F���NULL�ł͂Ȃ��Ƃ�
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		CScene_TWO::SetPosition(m_pFadeOut->pDesign_Move->EndPos);
		// ���_���W�̐ݒ�
		CScene_TWO::Set_Vtx_Pos();
		// �t���[���̏�����
		m_pFadeOut->pDesign_Move->nfram = 0;
	}
	// �t�F�[�h�^�C�v
	m_Fadetype = FADETYPE_END;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�A�E�g�J�n����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Start_FadeOut(void)
{
	// �t�F�[�h�A�E�g��NULL�Ȃ�
	// ->�֐��𔲂���
	if (m_pFadeOut == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2D�v���[���c2D���̃t�F�[�h�A�E�g��񂪂���܂���B\n�t�F�[�h�A�E�g�̊J�n���ł��܂���ł����B");
#endif // _DEBUG
		return;
	}
	// �t�F�[�h���s���Ă�����
	// ->�֐��𔲂���
	if (m_pFadeOut->bFade == true)
	{
		return;
	}
	// �t�F�[�h�A�E�g(�J���[)������
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		// �F�̐ݒ�
		CScene_TWO::SetCol(m_pFadeOut->pDesign_Col->BeginCol);
		// ���_�J���[�̐ݒ�
		CScene_TWO::Set_Vtx_Col();
		// �t���[���̏�����
		m_pFadeOut->pDesign_Col->nfram = 0;
	}
	// �t�F�[�h�A�E�g(�ʒu)������
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		// �����ʒu�ݒ�
		CScene_TWO::SetPosition(m_pFadeOut->pDesign_Move->BeginPos);
		// ���_���W�̐ݒ�
		CScene_TWO::Set_Vtx_Pos();
		// �t���[���̏�����
		m_pFadeOut->pDesign_Move->nfram = 0;
	}
	// �t�F�[�h��
	m_pFadeOut->bFade = true;
	// �J�E���g���Z�b�g
	m_pFadeOut->nCntFade = 0;
	// �J�E���g���Z�b�g
	m_pFadeOut->nCntAnimStart = 0;
	// �t�F�[�h�^�C�v
	m_Fadetype = FADETYPE_OUT;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �t�F�[�h�^�C�v�擾����
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents::FADETYPE C2DPresents::GetFadetype(void)
{
	return m_Fadetype;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����(�_��)�ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetCooperation(
	D3DXCOLOR const & changecol,
	int const & nTimeSwitch,
	int const & nTimeChange
)
{
	// �������̃�������NULL�Ȃ�
	if (m_pCooperation == NULL)
	{
		m_pCooperation = new COOPERATION;
	}
	// ����(�_��)�̃��������m�ۍς݂������甲����
	if (m_pCooperation->pCol_flash != NULL)
	{
		return;
	}
	// �_�ŏ��̃������m��
	m_pCooperation->pCol_flash = new COL_FLASHING;
	// �����J���[
	m_pCooperation->pCol_flash->Origincol = CScene_TWO::GetCol();
	// �ς��J���[
	m_pCooperation->pCol_flash->changecol = changecol;
	// �^�C���X�C�b�`
	m_pCooperation->pCol_flash->nTimeSwitch = nTimeSwitch;
	// �^�C���`�F���W
	m_pCooperation->pCol_flash->nTimeChange = nTimeChange;
	// �t�F�[�h�^�C�v
	m_Fadetype = FADETYPE_COOPERATION;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �g�嗦�ݒ菈��
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetCooperation(
	float const & fChangeScal,	// �g�嗦
	int const & nTimeChange		// �؂�ւ�鎞��
)
{
	// �������̃�������NULL�Ȃ�
	if (m_pCooperation == NULL)
	{
		m_pCooperation = new COOPERATION;
	}
	// ����(�g�嗦)�̃��������m�ۍς݂������甲����
	if (m_pCooperation->pScal != NULL)
	{
		return;
	}
	// �g�嗦�̃������m��
	m_pCooperation->pScal = new SCALING;
	// �ő�g�嗦�g�嗦
	m_pCooperation->pScal->fScalChange = fChangeScal;
	// �^�C���`�F���W
	m_pCooperation->pScal->nTimeChange = nTimeChange;
	// �g�嗦����
	m_pCooperation->pScal->fScalDiff = (m_pCooperation->pScal->fScalChange - m_pCooperation->pScal->fScal) / m_pCooperation->pScal->nTimeChange;
	// �����T�C�Y
	m_pCooperation->pScal->OriginSize = CScene_TWO::GetSize();
}
