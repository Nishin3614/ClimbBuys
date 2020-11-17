// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// フィールドブロック処理 [fieldblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "fieldblock.h"
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
CFieldblock::CFieldblock() : CBaseblock::CBaseblock()
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CFieldblock::~CFieldblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::Init()
{
	// ブロックタイプの設定
	CBaseblock::SetType(CBaseblock::TYPE_FIELD);	// フィールドのブロックタイプ
	// 落ちる設定
	CBaseblock::SetFall(false);
	// ベースブロックの初期化処理
	CBaseblock::Init();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::Uninit(void)
{
	CBaseblock::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::Update(void)
{
	CBaseblock::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::Draw(void)
{
	CBaseblock::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::Scene_MyCollision(
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
void CFieldblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フィールドブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CFieldblock::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CFieldblock * CFieldblock::Create(
	int					const & nModelId,	// モデル番号
	CBaseblock::GRID	const & Grid,		// 行列高さの番号
	CScene::LAYER		const & layer		// レイヤー
)
{
	// 変数宣言
	CFieldblock * pFieldblock;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pFieldblock = new CFieldblock();
	// 設定
	// シーン管理設定
	pFieldblock->ManageSetting(layer);
	pFieldblock->SetPos(				// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange, Grid.nLine * m_fSizeRange));
	pFieldblock->SetModelId(nModelId);	// モデル番号
	pFieldblock->SetGrid(Grid);			// 行列高さの番号
	pFieldblock->SetModelColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// 現在のブロックの高さを設定
	CBaseblock::SetHeight(Grid.nColumn, Grid.nLine, Grid.nHeight);

	// 初期化処理
	pFieldblock->Init();
	// 生成したオブジェクトを返す
	return pFieldblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)
//	pos			: 位置
//	nModelId	: モデル番号
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CFieldblock * CFieldblock::Create_Self(
	int				const & nModelId,	// モデル番号
	CBaseblock::GRID	const & Grid	// 行列高さの番号
)
{
	// 変数宣言
	CFieldblock * pFieldblock;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pFieldblock = new CFieldblock;
	// 設定
	pFieldblock->SetPos(				// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange, Grid.nLine * m_fSizeRange));
	pFieldblock->SetModelId(nModelId);	// モデル番号
	pFieldblock->SetGrid(Grid);			// 行列高さの番号
	// 初期化処理
	pFieldblock->Init();
	// 生成したオブジェクトを返す
	return pFieldblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr作成(個人管理unique)
// ※戻り値はstd::moveで受け取る
//	pos			: 位置
//	nModelId	: モデル番号
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CFieldblock> CFieldblock::Creat_Unique(
	int				const & nModelId,	// モデル番号
	CBaseblock::GRID	const & Grid	// 行列高さの番号
)
{
	// 変数宣言
	std::unique_ptr<CFieldblock> pFieldblock(new CFieldblock);		// シーン2Dクラス
	// 設定
	pFieldblock->SetPos(					// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange, Grid.nLine * m_fSizeRange));
	pFieldblock->SetModelId(nModelId);	// モデル番号
	pFieldblock->SetGrid(Grid);			// 行列高さの番号
	// 初期化処理
	pFieldblock->Init();
	// 生成したオブジェクトを返す
	return pFieldblock;
}


#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFieldblock::Debug(void)
{

}
#endif // _DEBUG

