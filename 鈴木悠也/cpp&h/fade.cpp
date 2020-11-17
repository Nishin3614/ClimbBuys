// ----------------------------------------
//
// ��ʑJ�ڏ����̐���[fade.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "fade.h"
#include "manager.h"

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

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CFade::CFade() : C2DPresents::C2DPresents()
{
	m_fade = FADE_NONE;					// �t�F�[�h���
	m_modeNext = CManager::MODE_GAME;	// ���̉�ʁi���[�h�j
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CFade::~CFade()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CFade::Init(void)
{	
	// �ϐ��錾
	m_fade = FADE_NONE;
	// �ʒu�ݒ�
	C2DPresents::SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	// �T�C�Y�ݒ�
	C2DPresents::SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	// �T�C�Y�ݒ�
	C2DPresents::SetCol(D3DXCOLOR_CA(0.0f, 0.0f));
	// �v���[��������
	C2DPresents::Init();
	// �e�N�X�`���[
	C2DPresents::BindTexture(0);
	// �t�F�[�h�C��
	C2DPresents::SetFadeIn(
		D3DXCOLOR_CA(0.0f, 0.0f),
		D3DXCOLOR_CA(0.0f, 1.0f),
		30
	);
	// �t�F�[�h�A�E�g
	C2DPresents::SetFadeOut(
		D3DXCOLOR_CA(0.0f, 1.0f),
		D3DXCOLOR_CA(0.0f, 0.0f),
		30
	);
	// ���[�h�ύX
	CManager::SetMode(m_modeNext);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CFade::Uninit(void)
{
	// �v���[���I��
	C2DPresents::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_OUT)
		{
			// �t�F�[�h�C�����s�g�p��������
			if (C2DPresents::GetFadeOut_Bool() == false)
			{
				// �t�F�[�h�I��
				m_fade = FADE_NONE;
				// �t�F�[�h�A�E�g�����I��
				C2DPresents::Compulsion_FadeOut();
			}
		}
		else if (m_fade == FADE_IN)
		{
			// �t�F�[�h�A�E�g���s�g�p��������
			if (C2DPresents::GetFadeIn_Bool() == false)
			{
				// �t�F�[�h�I��
				m_fade = FADE_OUT;
				// ���[�h�ύX
				CManager::SetMode(m_modeNext);
				// �t�F�[�h�A�E�g�J�n
				C2DPresents::Start_FadeOut();
			}
		}
	}
	// �v���[���X�V
	C2DPresents::Update();
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CFade::Draw(void)
{
	// �v���[���`��
	C2DPresents::Draw();
}

// ----------------------------------------
// ��ʑJ�ڐݒ菈��
// ----------------------------------------
void CFade::SetFade(CManager::MODE const & Mode)
{
	// �t�F�[�h�C��
	m_fade = FADE_IN;
	// ���[�h���
	m_modeNext = Mode;
	// �t�F�[�h�C���J�n
	C2DPresents::Start_FadeIn();
}

// ----------------------------------------
// ��ʑJ�ڎ擾����
// ----------------------------------------
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CFade * CFade::Create(CManager::MODE const & mode)
{
	// �ϐ��錾
	CFade * pFade;
	// �������m��
	pFade = new CFade;
	// �ݒ�
	pFade->m_modeNext = mode;
	// ������
	pFade->Init();
	// �Ԃ�
	return pFade;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CFade::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CFade::UnLoad(void)
{
}