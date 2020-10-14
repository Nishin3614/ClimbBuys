//=============================================================================
//
// �X�R�A[score.cpp]
// Author:Ryouma Inoue
//
//=============================================================================
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//=============================================================================
// �ÓI�����o�[�ϐ��̏�����
//=============================================================================
CScore *CScore::m_pScore = NULL;
int CScore::m_nScore = 0;

//=============================================================================
//
// �R���X�g���N�^
//
//=============================================================================
CScore::CScore() : CScene()
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		m_apNumber[nCntScore] = NULL;
	}

}

//=============================================================================
//
// �f�X�g���N�^
//
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// �X�R�A�̏�����
//=============================================================================
void CScore::Init(void)
{
	// �ϐ��錾
	int nNumber;	// �ԍ�
	m_nScore = 0;
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create(0, D3DXVECTOR3(SCORE_POS_X + SCORE_INTERVAL_X * nCntScore, SCORE_POS_Y, 0.0f), CNumber::TEX_SCORE,
			D3DXVECTOR2(SCORE_SIZE_X, SCORE_SIZE_Y), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		nNumber = m_nScore % (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - nCntScore) / (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - 1.0f - nCntScore);
		m_apNumber[nCntScore]->SetNum(nNumber);
	}
}

//=============================================================================
// �X�R�A�̏I��
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}
}

//=============================================================================
// �X�R�A�̍X�V
//=============================================================================
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Update();
		}
	}
}

//=============================================================================
// �X�R�A�̕`��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// �X�R�A�̐���
//=============================================================================
CScore *CScore::Create(void)
{
	// �V�[�����I�Ɋm��
	m_pScore = new CScore();

	// �V�[��������
	m_pScore->Init();

	return m_pScore;
}

//=============================================================================
// �X�R�A�ݒ�
//=============================================================================
void CScore::AddScore(int nValue)
{
	// �ϐ��錾
	int nCount;		// �J�E���g
	int nNumber;	// �ԍ�
	// �X�R�A���Z����
	m_nScore += nValue;
	//m_nRanking = *CRecord::GetLoad(0);

	for (nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		nNumber = m_nScore % (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - nCount) / (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - 1.0f - nCount);

		m_apNumber[nCount]->SetNum(nNumber);
	}
}

//=============================================================================
// �X�R�A���擾
//=============================================================================
CScore *CScore::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// �X�R�A���擾
//=============================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �X�R�A�ݒ�
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
// �X�R�A���擾
//=============================================================================
int CScore::GetNowScore(void)
{
	return m_nScore;
}