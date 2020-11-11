// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// オープニング管理処理 [openingmanager.cpp]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "openingmanager.h"

#include "manager.h"
#include "basemode.h"
#include "opening.h"
#include "stagingblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 仮
#define SPACE_Y (500)
// タイトルロゴ形成用 ブロック座標
// C_9
#define STAGINGBLOCK_POS_C_1 (D3DXVECTOR3(-525, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_2 (D3DXVECTOR3(-500, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_3 (D3DXVECTOR3(-475, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_4 (D3DXVECTOR3(-550, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_5 (D3DXVECTOR3(-550, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_6 (D3DXVECTOR3(-550, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_7 (D3DXVECTOR3(-525, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_8 (D3DXVECTOR3(-500, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_C_9 (D3DXVECTOR3(-475, 100 + SPACE_Y, 0));

// L_7
#define STAGINGBLOCK_POS_L_1 (D3DXVECTOR3(-425,0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_L_2 ( D3DXVECTOR3(-375, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L_3 ( D3DXVECTOR3(-400, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L_4 ( D3DXVECTOR3(-425, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L_5 ( D3DXVECTOR3(-425, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L_6 ( D3DXVECTOR3(-425, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L_7 ( D3DXVECTOR3(-425, 100 + SPACE_Y, 0));

// I_9
#define STAGINGBLOCK_POS_I_1 (D3DXVECTOR3(-325,0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_I_2 (D3DXVECTOR3(-300,0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_I_3 (D3DXVECTOR3(-275,0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_I_4 (D3DXVECTOR3(-300, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I_5 (D3DXVECTOR3(-300, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I_6 (D3DXVECTOR3(-300, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I_7 (D3DXVECTOR3(-325, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I_8 (D3DXVECTOR3(-300, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I_9 (D3DXVECTOR3(-275, 100 + SPACE_Y, 0));

// M_14
#define STAGINGBLOCK_POS_M_1 (D3DXVECTOR3(-225,0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_M_2 (D3DXVECTOR3(-100,0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_M_3 (D3DXVECTOR3(-225, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_4 (D3DXVECTOR3(-100, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_5 (D3DXVECTOR3(-225, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_6 (D3DXVECTOR3(-100, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_7 (D3DXVECTOR3(-225, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_8 (D3DXVECTOR3(-100, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_9 (D3DXVECTOR3(-175, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_10 (D3DXVECTOR3(-150, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_11 (D3DXVECTOR3(-225, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_12 (D3DXVECTOR3(-100, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_13 (D3DXVECTOR3(-200, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M_14 (D3DXVECTOR3(-125, 100 + SPACE_Y, 0));

// B_13
#define STAGINGBLOCK_POS_B_1 (D3DXVECTOR3(-50, 0 + SPACE_Y , 0));
#define STAGINGBLOCK_POS_B_2 (D3DXVECTOR3(-25, 0 + SPACE_Y , 0));
#define STAGINGBLOCK_POS_B_3 (D3DXVECTOR3(-0, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_4 (D3DXVECTOR3(-50, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_5 (D3DXVECTOR3(25, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_6 (D3DXVECTOR3(-50, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_7 (D3DXVECTOR3(-25, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_8 (D3DXVECTOR3(0, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_9 (D3DXVECTOR3(-50, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_10 (D3DXVECTOR3(25, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_11 (D3DXVECTOR3(-50, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_12 (D3DXVECTOR3(-25,100  + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B_13 (D3DXVECTOR3(0, 100 + SPACE_Y, 0));

// G_12
#define STAGINGBLOCK_POS_G_1 (D3DXVECTOR3(125, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_G_2 (D3DXVECTOR3(150, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_G_3 (D3DXVECTOR3(175, 0 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_4 (D3DXVECTOR3(100, 25 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_5 (D3DXVECTOR3(175, 25 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_6 (D3DXVECTOR3(100, 50 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_7 (D3DXVECTOR3(150, 50 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_8 (D3DXVECTOR3(175, 50 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_9 (D3DXVECTOR3(100, 75 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_10 (D3DXVECTOR3(125, 100 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_11 (D3DXVECTOR3(150, 100 + SPACE_Y,0));
#define STAGINGBLOCK_POS_G_12 (D3DXVECTOR3(175, 100 + SPACE_Y,0));

// U_10
#define STAGINGBLOCK_POS_U_1 (D3DXVECTOR3(250, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_2 (D3DXVECTOR3(275, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_3 (D3DXVECTOR3(225, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_4 (D3DXVECTOR3(300, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_5 (D3DXVECTOR3(225, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_6 (D3DXVECTOR3(300, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_7 (D3DXVECTOR3(225, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_8 (D3DXVECTOR3(300, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_9 (D3DXVECTOR3(225, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U_10 (D3DXVECTOR3(300, 100 + SPACE_Y, 0));

// Y_8
#define STAGINGBLOCK_POS_Y_1 (D3DXVECTOR3(400, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y_2 (D3DXVECTOR3(400, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y_3 (D3DXVECTOR3(400, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y_4 (D3DXVECTOR3(375, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y_5 (D3DXVECTOR3(425, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y_6 (D3DXVECTOR3(350, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y_7 (D3DXVECTOR3(450, 100 + SPACE_Y, 0));

// S_11
#define STAGINGBLOCK_POS_S_1  (D3DXVECTOR3(500, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_2  (D3DXVECTOR3(525, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_3  (D3DXVECTOR3(550, 0 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_4  (D3DXVECTOR3(550, 25 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_5  (D3DXVECTOR3(500, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_6  (D3DXVECTOR3(525, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_7  (D3DXVECTOR3(550, 50 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_8  (D3DXVECTOR3(500, 75 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_9  (D3DXVECTOR3(500, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_10 (D3DXVECTOR3(525, 100 + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S_11 (D3DXVECTOR3(550, 100 + SPACE_Y, 0));

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
	m_Targetpos[0] = STAGINGBLOCK_POS_C_1;
	m_Targetpos[1] = STAGINGBLOCK_POS_C_2;
	m_Targetpos[2] = STAGINGBLOCK_POS_C_3;
	m_Targetpos[3] = STAGINGBLOCK_POS_C_4;
	m_Targetpos[4] = STAGINGBLOCK_POS_C_5;
	m_Targetpos[5] = STAGINGBLOCK_POS_C_6;
	m_Targetpos[6] = STAGINGBLOCK_POS_C_7;
	m_Targetpos[7] = STAGINGBLOCK_POS_C_8;
	m_Targetpos[8] = STAGINGBLOCK_POS_C_9;

	// L
	m_Targetpos[9] = STAGINGBLOCK_POS_L_1;
	m_Targetpos[10] = STAGINGBLOCK_POS_L_2;
	m_Targetpos[11] = STAGINGBLOCK_POS_L_3;
	m_Targetpos[12] = STAGINGBLOCK_POS_L_4;
	m_Targetpos[13] = STAGINGBLOCK_POS_L_5;
	m_Targetpos[14] = STAGINGBLOCK_POS_L_6;
	m_Targetpos[15] = STAGINGBLOCK_POS_L_7;

	// I
	m_Targetpos[16] = STAGINGBLOCK_POS_I_1;
	m_Targetpos[17] = STAGINGBLOCK_POS_I_2;
	m_Targetpos[18] = STAGINGBLOCK_POS_I_3;
	m_Targetpos[19] = STAGINGBLOCK_POS_I_4;
	m_Targetpos[20] = STAGINGBLOCK_POS_I_5;
	m_Targetpos[21] = STAGINGBLOCK_POS_I_6;
	m_Targetpos[22] = STAGINGBLOCK_POS_I_7;
	m_Targetpos[23] = STAGINGBLOCK_POS_I_8;
	m_Targetpos[24] = STAGINGBLOCK_POS_I_9;

	// M
	m_Targetpos[25] = STAGINGBLOCK_POS_M_1;
	m_Targetpos[26] = STAGINGBLOCK_POS_M_2;
	m_Targetpos[27] = STAGINGBLOCK_POS_M_3;
	m_Targetpos[28] = STAGINGBLOCK_POS_M_4;
	m_Targetpos[29] = STAGINGBLOCK_POS_M_5;
	m_Targetpos[30] = STAGINGBLOCK_POS_M_6;
	m_Targetpos[31] = STAGINGBLOCK_POS_M_7;
	m_Targetpos[32] = STAGINGBLOCK_POS_M_8;
	m_Targetpos[35] = STAGINGBLOCK_POS_M_9;
	m_Targetpos[36] = STAGINGBLOCK_POS_M_10;
	m_Targetpos[33] = STAGINGBLOCK_POS_M_11;
	m_Targetpos[34] = STAGINGBLOCK_POS_M_12;
	m_Targetpos[37] = STAGINGBLOCK_POS_M_13;
	m_Targetpos[38] = STAGINGBLOCK_POS_M_14;

	//B
	m_Targetpos[39] = STAGINGBLOCK_POS_B_1;
	m_Targetpos[40] = STAGINGBLOCK_POS_B_2;
	m_Targetpos[41] = STAGINGBLOCK_POS_B_3;
	m_Targetpos[42] = STAGINGBLOCK_POS_B_4;
	m_Targetpos[43] = STAGINGBLOCK_POS_B_5;
	m_Targetpos[44] = STAGINGBLOCK_POS_B_6;
	m_Targetpos[45] = STAGINGBLOCK_POS_B_7;
	m_Targetpos[46] = STAGINGBLOCK_POS_B_8;
	m_Targetpos[47] = STAGINGBLOCK_POS_B_9;
	m_Targetpos[48] = STAGINGBLOCK_POS_B_10;
	m_Targetpos[49] = STAGINGBLOCK_POS_B_11;
	m_Targetpos[50] = STAGINGBLOCK_POS_B_12;
	m_Targetpos[51] = STAGINGBLOCK_POS_B_13;

	// G
	m_Targetpos[52] = STAGINGBLOCK_POS_G_1;
	m_Targetpos[53] = STAGINGBLOCK_POS_G_2;
	m_Targetpos[54] = STAGINGBLOCK_POS_G_3;
	m_Targetpos[55] = STAGINGBLOCK_POS_G_4;
	m_Targetpos[56] = STAGINGBLOCK_POS_G_5;
	m_Targetpos[59] = STAGINGBLOCK_POS_G_6;
	m_Targetpos[60] = STAGINGBLOCK_POS_G_7;
	m_Targetpos[61] = STAGINGBLOCK_POS_G_8;
	m_Targetpos[61] = STAGINGBLOCK_POS_G_9;
	m_Targetpos[63] = STAGINGBLOCK_POS_G_10;
	m_Targetpos[64] = STAGINGBLOCK_POS_G_11;
	m_Targetpos[65] = STAGINGBLOCK_POS_G_12;

	//U
	m_Targetpos[66] = STAGINGBLOCK_POS_U_1;
	m_Targetpos[67] = STAGINGBLOCK_POS_U_2;
	m_Targetpos[68] = STAGINGBLOCK_POS_U_3;
	m_Targetpos[69] = STAGINGBLOCK_POS_U_4;
	m_Targetpos[70] = STAGINGBLOCK_POS_U_5;
	m_Targetpos[71] = STAGINGBLOCK_POS_U_6;
	m_Targetpos[72] = STAGINGBLOCK_POS_U_7;
	m_Targetpos[73] = STAGINGBLOCK_POS_U_8;
	m_Targetpos[74] = STAGINGBLOCK_POS_U_9;
	m_Targetpos[75] = STAGINGBLOCK_POS_U_10;

	// Y
	m_Targetpos[76] = STAGINGBLOCK_POS_Y_1;
	m_Targetpos[77] = STAGINGBLOCK_POS_Y_2;
	m_Targetpos[78] = STAGINGBLOCK_POS_Y_3;
	m_Targetpos[79] = STAGINGBLOCK_POS_Y_4;
	m_Targetpos[80] = STAGINGBLOCK_POS_Y_5;
	m_Targetpos[81] = STAGINGBLOCK_POS_Y_6;
	m_Targetpos[82] = STAGINGBLOCK_POS_Y_7;

	// S
	m_Targetpos[83] = STAGINGBLOCK_POS_S_1;
	m_Targetpos[84] = STAGINGBLOCK_POS_S_2;
	m_Targetpos[85] = STAGINGBLOCK_POS_S_3;
	m_Targetpos[86] = STAGINGBLOCK_POS_S_4;
	m_Targetpos[87] = STAGINGBLOCK_POS_S_5;
	m_Targetpos[88] = STAGINGBLOCK_POS_S_6;
	m_Targetpos[89] = STAGINGBLOCK_POS_S_7;
	m_Targetpos[90] = STAGINGBLOCK_POS_S_8;
	m_Targetpos[91] = STAGINGBLOCK_POS_S_9;
	m_Targetpos[92] = STAGINGBLOCK_POS_S_10;
	m_Targetpos[93] = STAGINGBLOCK_POS_S_11;

	m_nCount = 0;
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
	// オープニングの状態が BUILDING だったら
	if (CManager::GetMode() == CManager::MODE_OPENING)
	{
		COpening *pOpening = (COpening*)(CManager::GetBaseMode());

		if (pOpening && pOpening->GetState() == COpening::OpeningState::BUILDING)
		{
			/////////////----------------------------------------
			if (m_pStagingBlock[m_nCount]->GetPos().y >= m_Targetpos[m_nCount].y)
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
			for (int nNum = 0; nNum < MAX_STAGINGBLOCK; nNum++)
			{
				// ブロックの破裂
				m_pStagingBlock[nNum]->BlockBurst();
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
	// ブロックの最大数分生成しポインタを保存
	for (int nCnt = 0; nCnt < MAX_STAGINGBLOCK; nCnt++)
	{
		m_pStagingBlock[nCnt] = CStagingBlock::Create(
			D3DXVECTOR3(0.0f, 1500.0f, 0.0f),
			D3DVECTOR3_ZERO,
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CScene_X::TYPE_BLOCK,
			false);
	}
}
