// ----------------------------------------
//
// �ԍ������̐���[number.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "number.h"

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
int CNumber::m_TexId[CNumber::TEX_MAX] = {
	4,
	5,
	6
};

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CNumber::CNumber() : CScene_TWO::CScene_TWO()
{
	m_texID = TEX_SCORE;
	m_nNum = 0;
	m_nRandamTime = 0;
	m_nFram = 0;
	m_bRand = false;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CNumber::~CNumber()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CNumber::Init(void)
{	
	CScene_TWO::Init();
	CScene_TWO::BindTexture(m_TexId[m_texID]);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CNumber::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CNumber::Update(void)
{
	// �����_���^�C����0���߂�������
	// ->�A�j���[�V�����J�n
	if (m_bRand == true)
	{
		// �t���[���������_���^�C���𓯂��ɂȂ�����
		// ->�ۑ������ԍ��ɂ���
		if (m_nFram == m_nRandamTime)
		{
			SetNum(m_nNum);
			m_bRand = false;
			return;	
		}
		// ����{�^����������
		// ->�ۑ������ԍ��ɂ���
		if (CManager::GetKeyConfig()->GetKeyConfigTrigger(CKeyConfig::CONFIG_DECISION))
		{
			SetNum(m_nNum);
			m_bRand = false;
			return;
		}
		// �����������_���Őݒ�
		SetNum(rand() % 10);
		// �t���[���^�C���A�b�v
		m_nFram++;
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CNumber::Draw(void)
{
	// ������
	CScene_TWO::Draw();
}

// ----------------------------------------
// �ԍ��ݒ菈��
// ----------------------------------------
void CNumber::SetNum(int const &nNum)
{
	// �ϐ��錾
	float fTex;			// �e�N�X�`���[�͈�
	// �e�N�X�`���[�͈͂̌v�Z
	fTex = (float)nNum / 10.0f;
	// �e�N�X�`���[�ݒ�
	CScene_TWO::Set_Vtx_Tex(
		D3DXVECTOR2(fTex, 0.0f),
		D3DXVECTOR2(fTex + 0.1f, 1.0f)
	);
}

// ----------------------------------------
// �e�N�X�`���[����
// ----------------------------------------
void CNumber::SetTex(TEX const &tex)
{
	m_texID = tex;
}

// ----------------------------------------
// �����A�j���I��
// ----------------------------------------
void CNumber::Complusion(void)
{
	SetNum(m_nNum);
	m_bRand = false;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CNumber::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CNumber::UnLoad(void)
{
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CNumber * CNumber::Create(
	int			const & nScore,
	D3DXVECTOR3 const & pos,
	TEX			const & tex,			
	D3DXVECTOR2 const & size,
	D3DXCOLOR	const & col
)
{
	// �ϐ��錾
	CNumber * pNumber;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pNumber = new CNumber();
	// �ʒu�ݒ�
	pNumber->SetPosition(pos);
	//�T�C�Y�ݒ�
	pNumber->SetSize(size);
	// �e�N�X�`���[�^�C�v�ݒ�
	pNumber->SetTex(tex);
	// ����������
	pNumber->Init();
	// �X�R�A�ݒ�
	pNumber->SetNum(nScore);
	// �����ۑ�
	pNumber->m_nNum = nScore;
	// ���������I�u�W�F�N�g��Ԃ�
	return pNumber;
}