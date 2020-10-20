// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ベースブロック処理 [baseblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"
#include "collision.h"
#include "debugproc.h"
#include "stencilshadow.h"

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
	// ステンシルシャドウの設定
	CScene_X::SetStencilshadow(
		D3DXVECTOR3(BASEBLOCK_RANGE,BASEBLOCK_XYZTOPOS(100),BASEBLOCK_RANGE),
		CStencilshadow::TYPE_RECT
	);
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
	static D3DXVECTOR3 BlockPos[94] = {};

	// C
	BlockPos[0] = D3DXVECTOR3(-525,0,0);
	BlockPos[1] = D3DXVECTOR3(-500, 0, 0);
	BlockPos[2] = D3DXVECTOR3(-475, 0, 0);
	BlockPos[3] = D3DXVECTOR3(-550,25,0);
	BlockPos[4] = D3DXVECTOR3(-550, 50, 0);
	BlockPos[5] = D3DXVECTOR3(-550, 75, 0);
	BlockPos[6] = D3DXVECTOR3(-525, 100, 0);
	BlockPos[7] = D3DXVECTOR3(-500, 100, 0);
	BlockPos[8] = D3DXVECTOR3(-475, 100, 0);

	// L
	BlockPos[9] = D3DXVECTOR3(-425,0,0);
	BlockPos[10] = D3DXVECTOR3(-375, 0, 0);
	BlockPos[11] = D3DXVECTOR3(-400, 0, 0);
	BlockPos[12] = D3DXVECTOR3(-425, 25, 0);
	BlockPos[13] = D3DXVECTOR3(-425, 50, 0);
	BlockPos[14] = D3DXVECTOR3(-425, 75, 0);
	BlockPos[15] = D3DXVECTOR3(-425, 100, 0);

	// I
	BlockPos[16] = D3DXVECTOR3(-325,0,0);
	BlockPos[17] = D3DXVECTOR3(-300,0,0);
	BlockPos[18] = D3DXVECTOR3(-275,0,0);
	BlockPos[19] = D3DXVECTOR3(-300, 25, 0);
	BlockPos[20] = D3DXVECTOR3(-300, 50, 0);
	BlockPos[21] = D3DXVECTOR3(-300, 75, 0);
	BlockPos[22] = D3DXVECTOR3(-325, 100, 0);
	BlockPos[23] = D3DXVECTOR3(-300, 100, 0);
	BlockPos[24] = D3DXVECTOR3(-275, 100, 0);

	// M
	BlockPos[25] = D3DXVECTOR3(-225,0,0);
	BlockPos[26] = D3DXVECTOR3(-100,0,0);
	BlockPos[27] = D3DXVECTOR3(-225, 25, 0);
	BlockPos[28] = D3DXVECTOR3(-100, 25, 0);
	BlockPos[29] = D3DXVECTOR3(-225, 50, 0);
	BlockPos[30] = D3DXVECTOR3(-100, 50, 0);
	BlockPos[31] = D3DXVECTOR3(-225, 75, 0);
	BlockPos[32] = D3DXVECTOR3(-100, 75, 0);
	BlockPos[35] = D3DXVECTOR3(-175, 75, 0);
	BlockPos[36] = D3DXVECTOR3(-150, 75, 0);
	BlockPos[33] = D3DXVECTOR3(-225, 100, 0);
	BlockPos[34] = D3DXVECTOR3(-100, 100, 0);
	BlockPos[37] = D3DXVECTOR3(-200, 100, 0);
	BlockPos[38] = D3DXVECTOR3(-125, 100, 0);

	//B
	BlockPos[39] = D3DXVECTOR3(-50, 0 , 0);
	BlockPos[40] = D3DXVECTOR3(-25, 0 , 0);
	BlockPos[41] = D3DXVECTOR3(-0, 0, 0);
	BlockPos[42] = D3DXVECTOR3(-50, 25, 0);
	BlockPos[43] = D3DXVECTOR3(25, 25, 0);
	BlockPos[44] = D3DXVECTOR3(-50, 50, 0);
	BlockPos[45] = D3DXVECTOR3(-25, 50, 0);
	BlockPos[46] = D3DXVECTOR3(0, 50, 0);
	BlockPos[47] = D3DXVECTOR3(-50, 75, 0);
	BlockPos[48] = D3DXVECTOR3(25, 75, 0);
	BlockPos[49] = D3DXVECTOR3(-50, 100, 0);
	BlockPos[50] = D3DXVECTOR3(-25,100 , 0);
	BlockPos[51] = D3DXVECTOR3(0, 100, 0);

	// G
	BlockPos[52] = D3DXVECTOR3(125, 0, 0);
	BlockPos[53] = D3DXVECTOR3(150, 0, 0);
	BlockPos[54] = D3DXVECTOR3(175, 0,0);
	BlockPos[55] = D3DXVECTOR3(100, 25,0);
	BlockPos[56] = D3DXVECTOR3(175, 25,0);
	BlockPos[59] = D3DXVECTOR3(100, 50,0);
	BlockPos[60] = D3DXVECTOR3(150, 50,0);
	BlockPos[61] = D3DXVECTOR3(175, 50,0);
	BlockPos[61] = D3DXVECTOR3(100, 75,0);
	BlockPos[63] = D3DXVECTOR3(125, 100,0);
	BlockPos[64] = D3DXVECTOR3(150, 100,0);
	BlockPos[65] = D3DXVECTOR3(175, 100,0);

	//U
	BlockPos[66] = D3DXVECTOR3(250, 0, 0);
	BlockPos[67] = D3DXVECTOR3(275, 0, 0);
	BlockPos[68] = D3DXVECTOR3(225, 25, 0);
	BlockPos[69] = D3DXVECTOR3(300, 25, 0);
	BlockPos[70] = D3DXVECTOR3(225, 50, 0);
	BlockPos[71] = D3DXVECTOR3(300, 50, 0);
	BlockPos[72] = D3DXVECTOR3(225, 75, 0);
	BlockPos[73] = D3DXVECTOR3(300, 75, 0);
	BlockPos[74] = D3DXVECTOR3(225, 100, 0);
	BlockPos[75] = D3DXVECTOR3(300, 100, 0);

	// Y
	BlockPos[76] = D3DXVECTOR3(400, 0, 0);
	BlockPos[77] = D3DXVECTOR3(400, 25, 0);
	BlockPos[78] = D3DXVECTOR3(400, 50, 0);
	BlockPos[79] = D3DXVECTOR3(375, 75, 0);
	BlockPos[80] = D3DXVECTOR3(425, 75, 0);
	BlockPos[81] = D3DXVECTOR3(350, 100, 0);
	BlockPos[82] = D3DXVECTOR3(450, 100, 0);

	// S
	BlockPos[83] = D3DXVECTOR3(500, 0, 0);
	BlockPos[84] = D3DXVECTOR3(525, 0, 0);
	BlockPos[85] = D3DXVECTOR3(550, 0, 0);
	BlockPos[86] = D3DXVECTOR3(550, 25, 0);
	BlockPos[87] = D3DXVECTOR3(500, 50, 0);
	BlockPos[88] = D3DXVECTOR3(525, 50, 0);
	BlockPos[89] = D3DXVECTOR3(550, 50, 0);
	BlockPos[90] = D3DXVECTOR3(500, 75, 0);
	BlockPos[91] = D3DXVECTOR3(500, 100, 0);
	BlockPos[92] = D3DXVECTOR3(525, 100, 0);
	BlockPos[93] = D3DXVECTOR3(550, 100, 0);

	// タイトルに表示するブロックの数分
	for (int nBlockCnt = 0; nBlockCnt < 94; nBlockCnt++)
	{
		// 変数宣言
		CBaseblock * pBaseblock;		// シーン2Dクラス
		pBaseblock = Create(BlockPos[nBlockCnt], 2);
		pBaseblock->SetSize(D3DXVECTOR3(0.25f, 0.25f, 0.25f));	// サイズ設定
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