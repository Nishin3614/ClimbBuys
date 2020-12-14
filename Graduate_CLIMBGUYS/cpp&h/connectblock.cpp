// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 結合ブロック処理 [connectblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "connectblock.h"
#include "baseblock.h"
#include "normalblock.h"
#include "bombblock..h"
#include "game.h"
#include "springblock.h"
#include "steelblock.h"
#include "electricblock.h"
#include "panicblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DROP_BLOCK	(1)		// 落とすブロックの数

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int	CConnectblock::m_nCntTime = 0;						// カウントタイム

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::CConnectblock()
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::~CConnectblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::SetBlockShape(void)
{
	// 変数宣言
	SHAPE Shape = (SHAPE)(CCalculation::GetRandomRange(SHAPE_CROSS_STEP_01, SHAPE_MAX - 1));	// 形
	D3DXCOLOR col;																	// 色
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());					// フェード値
	// 形
	CBaseblock::GRID BaseGrid = CBaseblock::SetFallPos();
	// 重力設定
	int nPhase = CBaseblock::GetPhase();											// フェーズ
	float fGravity;																	// 重力
	float fMinGravity = 0;															// 最小値
	float fMaxGravity = 0;															// 最大値
	if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMin;	// 最小値
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMax;	// 最大値
	}
	else
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMin;	// 最小値
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMax;	// 最大値
	}
	if (fMinGravity < fMaxGravity)
	{
		fGravity = CCalculation::RandamFloat(fMaxGravity, fMinGravity);
	}
	else
	{
		fGravity = (float)fMinGravity;
	}
	switch (Shape)
	{
		// クロスブロック_1段
	case SHAPE_CROSS_STEP_01:
		col = COLOR_RED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		break;
		// クロスブロック_2段
	case SHAPE_CROSS_STEP_02:
		col = COLOR_GREEN;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 2), &col,fGravity);
		break;
		// 中空きプラスブロック_1段
	case SHAPE_PLUS_CENTER_HOLE_STEP_01:
		col = COLOR_BLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		break;
		// 中空きプラスブロック_2段
	case SHAPE_PLUS_CENTER_HOLE_STEP_02:
		col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 2), &col,fGravity);

		break;
		// プラスブロック
	case SHAPE_PLUS:
		col = COLOR_VIOLET;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		break;
		// 中突起プラスブロック
	case SHAPE_PLUS_CENTER_PROTRUSION:
		col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &col,fGravity);
		break;
		// 対角線ブロック_01
	case SHAPE_DIAGONAL_01:
		col = COLOR_ORANGE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		break;
		// 対角線ブロック_02
	case SHAPE_DIAGONAL_02:
		col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		break;
		// 髭ブロック_01
	case SHAPE_BEARD_01:
		col = COLOR_PURPURE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		break;
		// 髭ブロック_02
	case SHAPE_BEARD_02:
		col = COLOR_PINK;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &col,fGravity);
		break;
		// 髭ブロック_03
	case SHAPE_BEARD_03:
		col = COLOR_EMERALD;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);

		break;
		// 髭ブロック_04
	case SHAPE_BEARD_04:
		col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &col,fGravity);
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 特殊ブロックの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::SetSpecialBlock(void)
{
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());					// フェード値
	// 落ちる位置設定
	CBaseblock::GRID BaseGrid = CBaseblock::SetSpecialFallPos();
	// 重力設定
	int nPhase = CBaseblock::GetPhase();											// フェーズ
	float fGravity;																	// 重力
	float fMinGravity = 0;															// 最小値
	float fMaxGravity = 0;															// 最大値
	if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMin;	// 最小値
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[CBaseblock::GetBlockStatus().nMaxSprit - 1].fMax;	// 最大値
	}
	else
	{
		fMinGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMin;	// 最小値
		fMaxGravity = CBaseblock::GetBlockStatus().v_fBlockGravity[nPhase].fMax;	// 最大値
	}
	if (fMinGravity < fMaxGravity)
	{
		fGravity = CCalculation::RandamFloat(fMaxGravity, fMinGravity);
	}
	else
	{
		fGravity = (float)fMinGravity;
	}
	CBaseblock::SPECIALBLOCK SpecialBlock;
	SpecialBlock = CBaseblock::GetSpecialSort(nPhase).GetRandam();
	switch (SpecialBlock)
	{
	case CBaseblock::SPECIALBLOCK_NONE:
		break;
	case CBaseblock::SPECIALBLOCK_BOMB:
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_SPRING:
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_STEEL:
		CSteelblock::Create(CScene_X::TYPE_BLOCKS_HARD, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_PANIC:
		CPanicblock::Create(CScene_X::TYPE_BLOCKS_HATENA, BaseGrid, NULL, fGravity);
		break;
	case CBaseblock::SPECIALBLOCK_ELECTRIC:
		CElectricblock::Create(CScene_X::TYPE_BLOCKS_INVERTER, BaseGrid, NULL, fGravity);
		break;
	default:
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 結合ブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CConnectblock::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// テスト作成(シーン管理)
//	pos			: 位置
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::TestCreate(void)
{
	// 変数宣言
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());
	D3DXCOLOR Col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	// 形
	CBaseblock::GRID BaseGrid = CBaseblock::GRID(-nFeedValue + rand() % (nFeedValue * 2), 5, -nFeedValue + rand() % (nFeedValue * 2));
	CNormalblock::Create(2, BaseGrid, &Col, 1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 1), &Col,1.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新_ブロック生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Update_CreateBlock(void)
{
	// 指定の時間に落ちてくる
	if (m_nCntTime % DERAY_TIME(CBaseblock::GetBlockStatus().nAppBlock) == 0)
	{
		// 重力設定
		int nPhase = CBaseblock::GetPhase();											// フェーズ
		int nDropBlock;																	// 重力
		int nMinDropBlock = 0;															// 最小値
		int nMaxDropBlock = 0;															// 最大値
		if (nPhase >= CBaseblock::GetBlockStatus().nMaxSprit)
		{
			nMinDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[CBaseblock::GetBlockStatus().nMaxSprit - 1].nMin;	// 最小値
			nMaxDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[CBaseblock::GetBlockStatus().nMaxSprit - 1].nMax;	// 最大値
		}
		else
		{
			nMinDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[nPhase].nMin;	// 最小値
			nMaxDropBlock = CBaseblock::GetBlockStatus().v_nDropBlock[nPhase].nMax;	// 最大値
		}
		int nRange = nMaxDropBlock - nMinDropBlock;											// 範囲
		nDropBlock = nMinDropBlock;
		if (nRange > 0)
		{
			nDropBlock += rand() % (nRange + 1);
		}
		// 最大の高さと重さを再設定
		CBaseblock::SetMaxPriority();

		// ブロックを落とす数
		for (int nCnt = 0; nCnt < nDropBlock; nCnt++)
		{
			// 結合されたブロックの生成
			SetBlockShape();
		}
		// 特殊ブロックの生成
		SetSpecialBlock();
	}
	// カウントタイムアップ
	m_nCntTime++;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// チュートリアルのブロックの初期配置
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Tutorial_InitArrangementBlock(void)
{
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());
	D3DXCOLOR Col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// 基準値
	CBaseblock::GRID BaseGrid = CBaseblock::GRID(-nFeedValue, 0, -nFeedValue);

	// ブロックの生成
	// 01
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(2, 0, 5), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(2, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(2, 1, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 1, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 2, 7), &Col,1.0f);

	// 02
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(7, 0, 5), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(6, 0, 6), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(5, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(7, 0, 7), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(6, 1, 6), &Col,1.0f);

	// 03
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(4, 0, 3), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(3, 0, 4), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(5, 0, 4), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(4, 0, 5), &Col,1.0f);

	// 04
	CNormalblock::Create(2, BaseGrid, &Col, 1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 2, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 2, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 2, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 2, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 3, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 3, 1), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 3, 0), &Col,1.0f);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 3, 1), &Col,1.0f);

	// バネ
	CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(2, 0, 1), &Col, 1.0f);
	CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(6, 0, 1), &Col, 1.0f);

	// ボム
	CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(4, 0, 1), NULL, 1.0f);
	CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(4, 0, 4), NULL, 1.0f);
}

#if IMGUI_DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的なデバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::StaticDebug(void)
{
	if (ImGui::Begin("Block"))
	{
		// 変数宣言
		static int nBlockGrid[3];	// ブロック行列高
		ImGui::InputInt3("BlockGrid", nBlockGrid);
		// 通常ブロック生成
		if (ImGui::Button("CreateNormal"))
		{
			CNormalblock::Create(CScene_X::TYPE_BLOCK, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
		}
		// ボムブロック生成
		if (ImGui::Button("CreateBomb"))
		{
			CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
		// ばねブロック生成
		if (ImGui::Button("CreateSpring"))
		{
			CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
		// 鋼鉄ブロック生成
		if (ImGui::Button("CreateSteel"))
		{
			CSteelblock::Create(CScene_X::TYPE_BLOCKS_HARD, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
		// パニックブロック生成
		if (ImGui::Button("CreatePanic"))
		{
			CPanicblock::Create(CScene_X::TYPE_BLOCKS_HATENA, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]),NULL, 1.0f);
		}
		// 電気ブロック生成
		if (ImGui::Button("CreateElectric"))
		{
			CElectricblock::Create(CScene_X::TYPE_BLOCKS_INVERTER, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), NULL, 1.0f);
		}
	}
	ImGui::End();
}

#endif // IMGUI_DEBUG

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Debug(void)
{

}
#endif // _DEBUG

