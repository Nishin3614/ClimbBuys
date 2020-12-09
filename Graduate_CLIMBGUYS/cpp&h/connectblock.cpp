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
int	CConnectblock::m_nCntTime = 0;				// カウントタイム

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::CConnectblock() : CScene::CScene()
{
	m_pos = D3DVECTOR3_ZERO;
	m_move = D3DVECTOR3_ZERO;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Shape = SHAPE_RECT;
	// ランダムに色情報を設定
	CCalculation::GetRandomColor(m_col);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock::~CConnectblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Init()
{
	// ブロック設定
	SetBlockShape();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Update(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::SetBlockShape(void)
{
	// 変数宣言
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());
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
	switch (m_Shape)
	{
		// 矩形
	case SHAPE_RECT:
		m_col = COLOR_ORANGE;
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid,NULL, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &m_col,fGravity);
		break;
		// Iブロック
	case SHAPE_I:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, fGravity);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(2, 0, 0), NULL, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(3, 0, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(4, 0, 0), &m_col, fGravity);
		break;
		// Jブロック
	case SHAPE_J:
		m_col = COLOR_EMERALD;
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid, NULL, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 2, 0), &m_col, fGravity);
		break;
		// Lブロック
	case SHAPE_L:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 2, 0), &m_col, fGravity);
		break;
		// Sブロック
	case SHAPE_S:
		m_col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &m_col,fGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), NULL, fGravity);
		break;
		// Tブロック
	case SHAPE_T:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, -1, 0), &m_col, fGravity);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 1), NULL, fGravity);

		break;
		// Zブロック
	case SHAPE_Z:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,fGravity);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 1), NULL, fGravity);
		break;
		// バネブロック
	case SHAPE_SPRING:
		m_col = COLOR_ORANGE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col, fGravity);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 0), NULL, fGravity);
		break;
		// クロスブロック_1段
	case SHAPE_CROSS_STEP_01:
		m_col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,fGravity);
		break;
		// クロスブロック_2段
	case SHAPE_CROSS_STEP_02:
		m_col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 2), &m_col,fGravity);
		break;
		// 中空きプラスブロック_1段
	case SHAPE_PLUS_CENTER_HOLE_STEP_01:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,fGravity);
		break;
		// 中空きプラスブロック_2段
	case SHAPE_PLUS_CENTER_HOLE_STEP_02:
		m_col = COLOR_LIGHTBLUE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 2), &m_col,fGravity);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(0, 0, 0), NULL, fGravity);

		break;
		// プラスブロック
	case SHAPE_PLUS:
		m_col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,fGravity);
		break;
		// 中突起プラスブロック
	case SHAPE_PLUS_CENTER_PROTRUSION:
		m_col = COLOR_YELLOW;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col,fGravity);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(0, 0, 0), NULL, fGravity);
		break;
		// 対角線ブロック_01
	case SHAPE_DIAGONAL_01:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,fGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), NULL, fGravity);
		break;
		// 対角線ブロック_02
	case SHAPE_DIAGONAL_02:
		m_col = COLOR_LIME;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,fGravity);
		break;
		// 髭ブロック_01
	case SHAPE_BEARD_01:
		m_col = COLOR_EMERALD;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,fGravity);
		break;
		// 髭ブロック_02
	case SHAPE_BEARD_02:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 2), &m_col,fGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), NULL, fGravity);
		break;
		// 髭ブロック_03
	case SHAPE_BEARD_03:
		m_col = COLOR_ORANGE;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,fGravity);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), NULL, fGravity);

		break;
		// 髭ブロック_04
	case SHAPE_BEARD_04:
		m_col = COLOR_LIGHTRED;
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col, fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 2), &m_col,fGravity);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 2), &m_col,fGravity);
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Draw(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Scene_MyCollision(
	int const & nObjType,
	CScene * pScene
)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{

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
// 作成(シーン管理)
//	pos			: 位置
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock * CConnectblock::Create(
	D3DXVECTOR3		const & pos,									// 位置
	SHAPE			const & shape,									// 形情報
	CScene::LAYER	const & layer									// レイヤー
)
{
	// 変数宣言
	CConnectblock * pConnectblock;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pConnectblock = new CConnectblock();
	// 設定
	pConnectblock->m_pos = pos;				// 位置
	pConnectblock->m_Shape = shape;
	// 初期化処理
	pConnectblock->Init();
	// 生成したオブジェクトを返す
	return pConnectblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)
//	pos			: 位置
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnectblock * CConnectblock::Create_Self(
	D3DXVECTOR3		const & pos,									// 位置
	SHAPE			const & shape									// 形情報
)
{
	// 変数宣言
	CConnectblock * pConnectblock;		// シーン2Dクラス
									// メモリの生成(初め->基本クラス,後->派生クラス)
	pConnectblock = new CConnectblock;
	// 設定
	pConnectblock->m_pos = pos;				// 位置
	// 初期化処理
	pConnectblock->Init();
	// 生成したオブジェクトを返す
	return pConnectblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr作成(個人管理unique)
// ※戻り値はstd::moveで受け取る
//	pos			: 位置
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CConnectblock> CConnectblock::Creat_Unique(
	D3DXVECTOR3		const & pos,									// 位置
	SHAPE			const & shape									// 形情報
)
{
	// 変数宣言
	std::unique_ptr<CConnectblock> pConnectblock(new CConnectblock);		// シーン2Dクラス
	// 設定
	pConnectblock->m_pos = pos;				// 位置
	// 初期化処理
	pConnectblock->Init();
	// 生成したオブジェクトを返す
	return pConnectblock;
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
			// 結合されたブロックの生成SHAPE_MAX SHAPE_BEARD_04
			Create(D3DVECTOR3_ZERO, (SHAPE)(CCalculation::GetRandomRange(SHAPE_RECT, SHAPE_MAX - 1)));
		}
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
			CPanicblock::Create(CScene_X::TYPE_BLOCK, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 1.0f);
		}
		// 電気ブロック生成
		if (ImGui::Button("CreateElectric"))
		{
			CElectricblock::Create(CScene_X::TYPE_BLOCK, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);
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

