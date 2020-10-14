//=============================================================================
//
// スコア[score.cpp]
// Author:Ryouma Inoue
//
//=============================================================================
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//=============================================================================
// 静的メンバー変数の初期化
//=============================================================================
CScore *CScore::m_pScore = NULL;
int CScore::m_nScore = 0;

//=============================================================================
//
// コンストラクタ
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
// デストラクタ
//
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// スコアの初期化
//=============================================================================
void CScore::Init(void)
{
	// 変数宣言
	int nNumber;	// 番号
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
// スコアの終了
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
// スコアの更新
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
// スコアの描画
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
// スコアの生成
//=============================================================================
CScore *CScore::Create(void)
{
	// シーン動的に確保
	m_pScore = new CScore();

	// シーン初期化
	m_pScore->Init();

	return m_pScore;
}

//=============================================================================
// スコア設定
//=============================================================================
void CScore::AddScore(int nValue)
{
	// 変数宣言
	int nCount;		// カウント
	int nNumber;	// 番号
	// スコア加算処理
	m_nScore += nValue;
	//m_nRanking = *CRecord::GetLoad(0);

	for (nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		nNumber = m_nScore % (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - nCount) / (int)powf(SCORE_INTERVAL, (float)MAX_SCORE - 1.0f - nCount);

		m_apNumber[nCount]->SetNum(nNumber);
	}
}

//=============================================================================
// スコア情報取得
//=============================================================================
CScore *CScore::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// スコア情報取得
//=============================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// スコア設定
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
// スコア情報取得
//=============================================================================
int CScore::GetNowScore(void)
{
	return m_nScore;
}