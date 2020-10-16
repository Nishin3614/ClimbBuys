// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ベースブロック処理 [baseblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"
#include "collision.h"
#include "debugproc.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BLOCK_GRAVITY	(1.0f)	// ブロックにかかる重力

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::CBaseblock() : CScene_X::CScene_X()
{
	m_type = TYPE_NORMAL;	// タイプ
	m_bFall = false;		// 落ちる状態
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::~CBaseblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Init()
{
	// シーンXの初期化処理
	CScene_X::Init();
	if (m_type == TYPE_FIELD)
	{
		// 当たり判定の設定
		CScene_X::SetCollision(CShape::SHAPETYPE_RECT, CCollision::OBJTYPE_BLOCK, false, true, NULL, D3DXVECTOR3(0.0, 50.0f, 0.0));
	}
	else
	{
		// 当たり判定の設定
		CScene_X::SetCollision(CShape::SHAPETYPE_RECT, CCollision::OBJTYPE_BLOCK, false, true, NULL, D3DXVECTOR3(0.0, 50.0f, 0.0));
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update(void)
{
	// シーンX更新処理
	CScene_X::Update();
	// 落ちる処理
	Update_Fall();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 落ちる処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_Fall(void)
{
	// 落ちる状態でなければ
	// 関数を抜ける
	if (!m_bFall) return;
	// 位置情報取得
	D3DXVECTOR3 pos = CScene_X::GetPos();
	// y位置更新
	pos.y -= BLOCK_GRAVITY;
	// 位置設定
	CScene_X::SetPos(pos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Scene_MyCollision(
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
void CBaseblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ベースブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CBaseblock::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock * CBaseblock::Create(
	D3DXVECTOR3		const & pos,		// 位置
	int				const & nModelId,	// モデル番号
	CScene::LAYER	const & layer		// レイヤー
)
{
	// 変数宣言
	CBaseblock * pBaseblock;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pBaseblock = new CBaseblock();
	// 設定
	// シーン管理設定
	pBaseblock->ManageSetting(layer);
	pBaseblock->SetPos(pos);			// 位置
	pBaseblock->SetModelId(nModelId);	// モデル番号
	// 初期化処理
	pBaseblock->Init();
	// 生成したオブジェクトを返す
	return pBaseblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)
//	pos			: 位置
//	nModelId	: モデル番号
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock * CBaseblock::Create_Self(
	D3DXVECTOR3		const & pos,									// 位置
	int				const & nModelId								// モデル番号
)
{
	// 変数宣言
	CBaseblock * pBaseblock;		// シーン2Dクラス
									// メモリの生成(初め->基本クラス,後->派生クラス)
	pBaseblock = new CBaseblock;
	// 設定
	pBaseblock->SetPos(pos);			// 位置
	pBaseblock->SetModelId(nModelId);	// 位置
	// 初期化処理
	pBaseblock->Init();
	// 生成したオブジェクトを返す
	return pBaseblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr作成(個人管理unique)
// ※戻り値はstd::moveで受け取る
//	pos			: 位置
//	nModelId	: モデル番号
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CBaseblock> CBaseblock::Creat_Unique(
	D3DXVECTOR3		const & pos,									// 位置
	int				const & nModelId								// モデル番号
)
{
	// 変数宣言
	std::unique_ptr<CBaseblock> pBaseblock(new CBaseblock);		// シーン2Dクラス
																// 設定
	pBaseblock->SetPos(pos);			// 位置
	pBaseblock->SetModelId(nModelId);	// 位置
	// 初期化処理
	pBaseblock->Init();
	// 生成したオブジェクトを返す
	return pBaseblock;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// タイトル用 ブロックを一斉に生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::CreateInBulkBlock()
{
	static D3DXVECTOR3 BlockPos[46] = {};

	//BlockPos[0];
	// タイトルに表示するブロックの数分
	for (int nBlockCnt = 0; nBlockCnt < 46; nBlockCnt++)
	{
		// 変数宣言
		CBaseblock * pBaseblock;		// シーン2Dクラス
										// メモリの生成(初め->基本クラス,後->派生クラス)
		pBaseblock = new CBaseblock();
		// 設定
		// シーン管理設定
		pBaseblock->ManageSetting(LAYER_3DOBJECT);				// レイヤーの番号

		BlockPos[nBlockCnt] = D3DXVECTOR3(0.0f + nBlockCnt * 25.0f, 0.0f, 0.0f);

		pBaseblock->SetModelId(2);								// モデルのid

		pBaseblock->SetPos(D3DXVECTOR3(0.0f + nBlockCnt * 25.0f, 0.0f, 0.0f));				// 位置
		pBaseblock->SetSize(D3DXVECTOR3(0.25f, 0.25f, 0.25f));	// サイズ設定
		pBaseblock->Init();										// 初期化処理
	}
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Debug(void)
{
	CDebugproc::Print("pos(%.2f,%.2f,%.2f)\n", CScene_X::GetMatrix()._41, CScene_X::GetMatrix()._42, CScene_X::GetMatrix()._43);
}
#endif // _DEBUG