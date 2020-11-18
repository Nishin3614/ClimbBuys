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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
	/*
	for (size_t nCntBlock = 0; nCntBlock < m_vec_pBaseBlock.size(); nCntBlock++)
	{
		m_vec_pBaseBlock[nCntBlock]->SetModelColor(m_col);
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Uninit(void)
{
	/*
	// NULL代入
	for (size_t nCntBlock = 0; nCntBlock < m_vec_pBaseBlock.size(); nCntBlock++)
	{
		m_vec_pBaseBlock[nCntBlock] = NULL;
	}
	// ベースブロック情報の格納情報開放
	m_vec_pBaseBlock.clear();
	m_vec_pBaseBlock.shrink_to_fit();
	*/
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
	CBaseblock::GRID BaseGrid = CBaseblock::GRID(-nFeedValue + rand() % (nFeedValue * 2), CBaseblock::GetBlockStatus().nAppearance, -nFeedValue + rand() % (nFeedValue * 2));

	//BaseGrid = CBaseblock::GRID(10,10,0);
	switch (m_Shape)
	{
		// 矩形
	case SHAPE_RECT:
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid,&D3DXCOLOR(0.0f,0.0f,0.0f,1.0));
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 1), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 0, 1), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 1), &m_col);
		break;
		// Iブロック
	case SHAPE_I:
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid + CBaseblock::GRID(2, 0, 0), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0));
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(3, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(4, 0, 0), &m_col);
		break;
		// Jブロック
	case SHAPE_J:
		CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, BaseGrid, &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0));
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 2, 0), &m_col);
		break;
		// Lブロック
	case SHAPE_L:
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 2, 0), &m_col);
		break;
		// Sブロック
	case SHAPE_S:
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 1, 0), &m_col);
		CSpringblock::Create(53, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col);
		break;
		// Tブロック
	case SHAPE_T:
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(2, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, -1, 0), &m_col);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col);

		break;
		// Zブロック
	case SHAPE_Z:
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 1), &m_col);
		break;
		// バネブロック
	case SHAPE_SPRING:
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid, &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(1, 0, 0), &m_col);
		CNormalblock::Create(CScene_X::TYPE_BLOCK, BaseGrid + CBaseblock::GRID(0, 1, 0), &m_col);
		CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, BaseGrid + CBaseblock::GRID(1, 1, 0), &m_col);
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
	// シーン管理設定
	pConnectblock->ManageSetting(layer);
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
	CNormalblock::Create(2, BaseGrid, &Col);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 0), &Col);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 0, 1), &Col);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 0, 1), &Col);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 0), &Col);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(1, 1, 0), &Col);
	CNormalblock::Create(2, BaseGrid + CBaseblock::GRID(0, 1, 1), &Col);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新_ブロック生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Update_CreateBlock(void)
{
	// 5秒間に1回落ちてくる
	if (m_nCntTime % DERAY_TIME(5) == 0)
	{
		// 結合されたブロックの生成
		Create(D3DVECTOR3_ZERO, (SHAPE)(rand() % SHAPE_MAX));
	}
	// カウントタイムアップ
	m_nCntTime++;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnectblock::Debug(void)
{

}
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
			CNormalblock::Create(CScene_X::TYPE_BLOCK, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		// ボムブロック生成
		if (ImGui::Button("CreateBomb"))
		{
			CBombblock::Create(CScene_X::TYPE_BLOCK_BOMB, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
		// ばねブロック生成
		if (ImGui::Button("CreateSpring"))
		{
			CSpringblock::Create(CScene_X::TYPE_BLOCK_SPRING, CBaseblock::GRID(nBlockGrid[0], nBlockGrid[1], nBlockGrid[2]), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	ImGui::End();
}
#endif // _DEBUG

