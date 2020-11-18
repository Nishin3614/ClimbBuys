// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// オープニング管理処理 [openingmanager.cpp]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "openingmanager.h"
#include "camera.h"

#include "manager.h"
#include "basemode.h"
#include "opening.h"
#include "stagingblock.h"

#include "3dparticle.h"
#include "3deffect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 仮
#define SPACE_Y (500)
// タイトルロゴ形成用 ブロック座標
// CLIMBGUYS
#define STAGINGBLOCK_POS_C (D3DXVECTOR3(-600.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L (D3DXVECTOR3(-450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I (D3DXVECTOR3(-300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M (D3DXVECTOR3(-150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B (D3DXVECTOR3(0.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_G (D3DXVECTOR3(150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U (D3DXVECTOR3(300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y (D3DXVECTOR3(450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S (D3DXVECTOR3(600.0f, 0.0f + SPACE_Y, 0));

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3	COpeningManager::m_Targetpos[MAX_STAGINGBLOCK] = {};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COpeningManager::COpeningManager()
{
	// C
	m_Targetpos[0] = STAGINGBLOCK_POS_C;
	// L
	m_Targetpos[1] = STAGINGBLOCK_POS_L;
	// I
	m_Targetpos[2] = STAGINGBLOCK_POS_I;
	// M
	m_Targetpos[3] = STAGINGBLOCK_POS_M;
	// B
	m_Targetpos[4] = STAGINGBLOCK_POS_B;
	// G
	m_Targetpos[5] = STAGINGBLOCK_POS_G;
	// U
	m_Targetpos[6] = STAGINGBLOCK_POS_U;
	// Y
	m_Targetpos[7] = STAGINGBLOCK_POS_Y;
	// S
	m_Targetpos[8] = STAGINGBLOCK_POS_S;

	// 3Dエフェクトの生成
	C3DEffect::Create();
	m_nCount = 0;
	m_NextStateCnt = 60;
	// オブジェクトの生成
	CreateAll();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COpeningManager::~COpeningManager()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void COpeningManager::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void COpeningManager::Update(void)
{
	//--------------テスト中-----------------------------
	//static int nCnt = 0;
	//nCnt++;

	//if (nCnt % 6 == 0)
	//{
	//	// パーティクル生成
	//	C3DParticle::Create(
	//		C3DParticle::PARTICLE_ID_SMOKE,
	//		D3DXVECTOR3(1020.0f, 500.0f, 900.0f)
	//	);
	//}

	//C3DParticle::Create(
	//	C3DParticle::PARTICLE_ID_BLOCK,
	//	D3DXVECTOR3(1030.0f, 500.0f, 950.0f)
	//);

	//----------------------------------------------


	if (CManager::GetMode() == CManager::MODE_OPENING)
	{
		COpening *pOpening = (COpening*)(CManager::GetBaseMode());

		if (pOpening && pOpening->GetState() == COpening::OpeningState::ERUPTION)
		{
			if (CManager::GetRenderer()->GetCamera()->GetShakeEndFlag())
			{
				// 次のステートに移行
				pOpening->SetState(COpening::OpeningState::BUILDING);
			}
			else
			{
				// カメラの振動
				CManager::GetRenderer()->GetCamera()->CameraShake();
			}
		}

		if (pOpening && pOpening->GetState() == COpening::OpeningState::BUILDING)
		{
			//// パーティクル生成
			//C3DParticle::Create(
			//	C3DParticle::PARTICLE_ID_UNKNOWN,
			//	D3DXVECTOR3(1000.0f, 500.0f, 900.0f)
			//);

			/////////////----------------------------------------
			if (m_pStagingBlock[m_nCount]->GetPos().y > m_Targetpos[m_nCount].y)
			{ // ブロックを一個ずつ目的地まで移動
				m_pStagingBlock[m_nCount]->BlockFall();
			}
			else
			{
				if (m_nCount < MAX_STAGINGBLOCK - 1)
				{// 次のブロックがまだあるなら
					m_pStagingBlock[m_nCount]->SetMoveStop(true);
					m_nCount++;
					return;
				}

				else
				{// 次のステートに移行
					pOpening->SetState(COpening::OpeningState::STAGING);
				}
			}
			///////////-----------------------------------------------
		}

		// オープニングの状態が STAGING だったら
		if (pOpening && pOpening->GetState() == COpening::OpeningState::STAGING)
		{
			if (m_NextStateCnt <= 0)
			{
				for (int nNum = 0; nNum < MAX_STAGINGBLOCK; nNum++)
				{
					// ブロックの破裂
					m_pStagingBlock[nNum]->BlockBurst();
				}
			}
			else
			{
				m_NextStateCnt--;
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ターゲットの座標の取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 COpeningManager::GetTargetpos(int num)
{
	if (num < MAX_STAGINGBLOCK)
	{
		return m_Targetpos[num];
	}

	return m_Targetpos[0];
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全てのオブジェクトを生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void COpeningManager::CreateAll()
{
	// 背景オブジェクトの生成
	CScene_X::Create(
		D3DVECTOR3_ZERO,					// 位置
		D3DVECTOR3_ZERO,					// 回転
		CScene_X::TYPE_OBJECT_MAP,			// モデル番号
		false);								// シャドウマッピング状態

	// ブロックの最大数分生成しポインタを保存
	for (int nCnt = 0; nCnt < MAX_STAGINGBLOCK; nCnt++)
	{
		m_pStagingBlock[nCnt] = CStagingBlock::Create(
			D3DXVECTOR3(0.0f, 1500.0f, 0.0f),
			D3DVECTOR3_ZERO,
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CScene_X::TYPE_OBJECT_BLOCK_C + nCnt,
			false);

		m_pStagingBlock[nCnt]->SetBlockNum(nCnt);
	}
}
