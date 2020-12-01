// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 鋼鉄ブロック処理 [steelblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "steelblock.h"
#include "collision.h"
#include "debugproc.h"
#include "game.h"

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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSteelblock::CSteelblock() : CBaseblock::CBaseblock()
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSteelblock::~CSteelblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Init()
{
	// ブロックタイプの設定
	CBaseblock::SetType(TYPE_STEEL);	// 鋼鉄のブロックタイプ
	// 落ちる設定
	CBaseblock::SetFall(true);
	// ベースブロック初期化処理
	CBaseblock::Init();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Uninit(void)
{
	CBaseblock::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Update(void)
{
	CBaseblock::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Draw(void)
{
	CBaseblock::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: 相手オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Scene_MyCollision(
	int const & nObjType,
	CScene * pScene
)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: 相手オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の判定
//	Obj		: オブジェタイプ
//	pScene	: シーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::HitCollision(
	COLLISIONDIRECTION const &Direct,	// 前後左右上下
	CScene::OBJ const & Obj,			// オブジェタイプ
	CScene * pScene						// シーン情報
)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 鋼鉄ブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CSteelblock::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	pCol		: 色情報
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSteelblock * CSteelblock::Create(
	int				const & nModelId,	// モデル番号
	GRID			const & Grid,		// 行列高さ番号
	D3DXCOLOR		* pCol,				// 色
	CScene::LAYER	const & layer		// レイヤー
)
{
	// 変数宣言
	CSteelblock * pSteelblock;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pSteelblock = new CSteelblock();
	// 設定
	// シーン管理設定
	pSteelblock->ManageSetting(layer);
	pSteelblock->SetGrid(Grid);				// 行列高さ
	pSteelblock->SetPos(					// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange, Grid.nLine * m_fSizeRange));
	pSteelblock->SetModelId(nModelId);		// モデル番号
	// 色がNULLではないなら
	if (pCol != NULL)
	{
		// モデルの色を変える
		pSteelblock->SetModelColor(*pCol);
	}
	// 初期化処理
	pSteelblock->Init();
	// 生成したオブジェクトを返す
	return pSteelblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	pCol		: 色情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSteelblock * CSteelblock::Create_Self(
	int				const & nModelId,								// モデル番号
	GRID			const & Grid,									// 行列高さ番号
	D3DXCOLOR		* pCol											// 色
)
{
	// 変数宣言
	CSteelblock * pSteelblock;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pSteelblock = new CSteelblock;
	// 設定
	pSteelblock->SetGrid(Grid);			// 行列高さ
	pSteelblock->SetPos(					// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange, Grid.nLine * m_fSizeRange));
	pSteelblock->SetModelId(nModelId);		// モデル番号
	// 色がNULLではないなら
	if (pCol != NULL)
	{
		// モデルの色を変える
		pSteelblock->SetModelColor(*pCol);
	}

	// 初期化処理
	pSteelblock->Init();
	// 生成したオブジェクトを返す
	return pSteelblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr作成(個人管理unique)
// ※戻り値はstd::moveで受け取る
//	pos			: 位置
//	nModelId	: モデル番号
//	pCol		: 色情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CSteelblock> CSteelblock::Creat_Unique(
	int				const & nModelId,								// モデル番号
	GRID			const & Grid,									// 行列高さ番号
	D3DXCOLOR		* pCol											// 色
)
{
	// 変数宣言
	std::unique_ptr<CSteelblock> pSteelblock(new CSteelblock);		// シーン2Dクラス
	// 設定
	pSteelblock->SetGrid(Grid);			// 行列高さ
	pSteelblock->SetPos(					// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange, Grid.nLine * m_fSizeRange));
	pSteelblock->SetModelId(nModelId);		// モデル番号
	// 色がNULLではないなら
	if (pCol != NULL)
	{
		// モデルの色を変える
		pSteelblock->SetModelColor(*pCol);
	}
	// 初期化処理
	pSteelblock->Init();
	// 生成したオブジェクトを返す
	return pSteelblock;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSteelblock::Debug(void)
{
	/*
	// 落ちる状態でなければ
	// 関数を抜ける
	if (CBaseblock::GetFall())
	{
		CDebugproc::Print("NormalBlock::Fall = true\n");
	}
	else
	{
		CDebugproc::Print("NormalBlock::Fall = false\n");
	}
	CDebugproc::Print("NormalBlock::Pos(%.2f,%.2f,%.2f)\n",CScene_X::GetPos().x, CScene_X::GetPos().y, CScene_X::GetPos().z);
	*/
}
#endif // _DEBUG

