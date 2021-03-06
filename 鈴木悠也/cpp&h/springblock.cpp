// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// バネブロック処理 [springblock.cpp]
// Author : fujiwaramsato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "springblock.h"
#include "collision.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"

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
CSpringblock::CSpringblock() : CBaseblock::CBaseblock()
{
	CScene::SetObj(CScene::OBJ::OBJ_BLOCK);	// オブジェクトタイプの設定
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSpringblock::~CSpringblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::Init()
{
	// ブロックタイプの設定
	CBaseblock::SetType(TYPE_NORMAL);	// 普通のブロックタイプ
										// 落ちる設定
	CBaseblock::SetFall(true);
	// ベースブロック初期化処理
	CBaseblock::Init();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::Uninit(void)
{
	CBaseblock::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::Update(void)
{
	// ベースブロック更新処理
	CBaseblock::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::Draw(void)
{
	CBaseblock::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::Scene_MyCollision(
	int const & nObjType,
	CScene * pScene
)
{
	if (nObjType == CCollision::OBJTYPE_BLOCK)
	{
		// シーン情報がNULLなら
		// ->関数を抜ける
		if (pScene == NULL) return;
		if (!CBaseblock::GetFall()) return;
		// 変数宣言
		// シーン情報の代入
		CBaseblock * pBaseBlock = (CBaseblock *)pScene;
		// 相手の落ちる状態がtrueなら
		// ->関数を抜ける
		if (pBaseBlock->GetFall()) return;
		// 変数宣言
		CBaseblock::GRID MyGrid = CBaseblock::GetGrid();	// 自分の行列高
		CBaseblock::GRID OppGrid = pBaseBlock->GetGrid();	// 相手の行列高
															// 同じ行列ではないなら
															// ->関数を抜ける
		if (!(MyGrid.nColumn == OppGrid.nColumn &&
			MyGrid.nLine == OppGrid.nLine)) return;
		// 変数宣言
		int nFeedValue = m_nFeedValue[CGame::GetStage()];	// フェードの値
		int nHeight = CBaseblock::GetHeight(				// 高さ
			MyGrid.nColumn + nFeedValue,
			MyGrid.nLine + nFeedValue) + 1;
		// 高さを行列高に代入
		MyGrid.nHeight = nHeight;
		// 高さの設定
		CBaseblock::SetHeight(
			MyGrid.nColumn + nFeedValue,
			MyGrid.nLine + nFeedValue,
			MyGrid.nHeight
		);
		// 現在の行列高の設定
		CBaseblock::SetGrid(MyGrid);
		// 位置設定
		CBaseblock::SetPos(MyGrid.GetPos(m_fSizeRange));
		// 落ちている状態設定
		CBaseblock::SetFall(false);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
	if (nObjType == CCollision::OBJTYPE_BLOCK)
	{
		// シーン情報がNULLなら
		// ->関数を抜ける
		if (pScene == NULL) return;
		if (!CBaseblock::GetFall()) return;
		// シーン情報の代入
		CBaseblock * pBaseBlock = (CBaseblock *)pScene;
		// 相手の落ちる状態がtrueなら
		// ->関数を抜ける
		if (pBaseBlock->GetFall()) return;
		// 変数宣言
		CBaseblock::GRID MyGrid = CBaseblock::GetGrid();	// 自分の行列高
		CBaseblock::GRID OppGrid = pBaseBlock->GetGrid();	// 相手の行列高
															// 同じ行列ではないなら
															// ->関数を抜ける
		if (!(MyGrid.nColumn == OppGrid.nColumn &&
			MyGrid.nLine == OppGrid.nLine)) return;
		// 変数宣言
		int nFeedValue = m_nFeedValue[CGame::GetStage()];	// フェードの値
		int nHeight = CBaseblock::GetHeight(				// 高さ
			MyGrid.nColumn + nFeedValue,
			MyGrid.nLine + nFeedValue) + 1;
		// 高さを行列高に代入
		MyGrid.nHeight = nHeight;
		// 高さの設定
		CBaseblock::SetHeight(
			MyGrid.nColumn + nFeedValue,
			MyGrid.nLine + nFeedValue,
			MyGrid.nHeight
		);
		// 現在の行列高の設定
		CBaseblock::SetGrid(MyGrid);
		// 位置設定
		CBaseblock::SetPos(MyGrid.GetPos(m_fSizeRange));
		// 落ちている状態設定
		CBaseblock::SetFall(false);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出し当たり判定
//	pos		: 位置
//	posOld	: 前回の位置
//	move	: 移動量
//	size	: サイズ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CSpringblock::PushCollision(CPlayer * pplayer,D3DXVECTOR3 const & OffsetPos, D3DXVECTOR3 *size)
{
	// 変数宣言
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	CGame::STAGE Stage = CGame::GetStage();						// ステージ

	if (this->GetGrid().nHeight >= CBaseblock::GetHeight(this->GetGrid().nColumn + m_nFeedValue[Stage], this->GetGrid().nLine + m_nFeedValue[Stage]))
	{
		// 素材のZ範囲
		if (pplayer->GetPos().z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
			pplayer->GetPos().z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f)
		{
			// 素材のX範囲
			if (pplayer->GetPos().x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				pplayer->GetPos().x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f)
			{
				// 当たり判定(上)
				if (pplayer->GetPos().y + OffsetPos.y - size->y * 0.5f <= BlockPos.y + m_fSizeRange&&
					pplayer->GetPosOld().y + OffsetPos.y - size->y * 0.5f > BlockPos.y + m_fSizeRange)
				{
					// ばね用ジャンプ処理_バネはジャンプ判定
					pplayer->SpringJump();
					return true;
				}
			}
		}
	}

	return false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の判定
//	Obj		: オブジェタイプ
//	pScene	: シーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSpringblock::HitCollision(
	COLLISIONDIRECTION const &Direct,	// 前後左右上下
	CScene::OBJ const & Obj,			// オブジェタイプ
	CScene * pScene						// シーン情報
)
{
	// オブジェタイプがブロックなら
	if (CScene::OBJ_BLOCK == Obj)
	{
		if (Direct == COLLISIONDIRECTION::DOWN)
		{
			// シーン情報がNULLなら
			// ->関数を抜ける
			if (pScene == NULL) return;
			if (!CBaseblock::GetFall()) return;
			// シーン情報の代入
			CBaseblock * pBaseBlock = (CBaseblock *)pScene;
			// 相手の落ちる状態がtrueなら
			// ->関数を抜ける
			if (pBaseBlock->GetFall()) return;
			// 変数宣言
			CBaseblock::GRID MyGrid = CBaseblock::GetGrid();	// 自分の行列高
			CBaseblock::GRID OppGrid = pBaseBlock->GetGrid();	// 相手の行列高
																// 同じ行列ではないなら
																// ->関数を抜ける
			if (!(MyGrid.nColumn == OppGrid.nColumn &&
				MyGrid.nLine == OppGrid.nLine)) return;
			// 変数宣言
			int nFeedValue = m_nFeedValue[CGame::GetStage()];	// フェードの値
			int nHeight = CBaseblock::GetHeight(			// 高さ
				MyGrid.nColumn + nFeedValue,
				MyGrid.nLine + nFeedValue) + 1;
			// 高さを行列高に代入
			MyGrid.nHeight = nHeight;
			// 高さの設定
			CBaseblock::SetHeight(
				MyGrid.nColumn + nFeedValue,
				MyGrid.nLine + nFeedValue,
				MyGrid.nHeight
			);
			// 現在の行列高の設定
			CBaseblock::SetGrid(MyGrid);
			// 位置設定
			CBaseblock::SetPos(MyGrid.GetPos(m_fSizeRange));
			// 落ちている状態設定
			CBaseblock::SetFall(false);
		}
	}
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//CSpringblock * CSpringblock::Create(
//	D3DXVECTOR3		const & pos,		// 位置
//	CScene::LAYER	const & layer		// レイヤー
//)
//{
//	// 変数宣言
//	CSpringblock * pBaseblock;
//	pBaseblock = new CSpringblock();			// メモリ確保
//	pBaseblock->ManageSetting(layer);			// シーン管理設定
//	pBaseblock->SetPos(pos);					// 位置
//	pBaseblock->SetModelId(TYPE_BLOCK_SPRING);	// モデル番号
//	pBaseblock->Init();							// 初期化処理
//	return pBaseblock;
//}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	pCol		: 色情報
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSpringblock * CSpringblock::Create(
	int				const & nModelId,	// モデル番号
	GRID			const & Grid,		// 行列高さ番号
	D3DXCOLOR		* pCol,				// 色
	CScene::LAYER	const & layer		// レイヤー
)
{
	// 変数宣言
	CSpringblock * pSpringblock;		// シーン2Dクラス
										// メモリの生成(初め->基本クラス,後->派生クラス)
	pSpringblock = new CSpringblock();
	// 設定
	// シーン管理設定
	pSpringblock->ManageSetting(LAYER_3DBLOCK);
	pSpringblock->SetGrid(Grid);			// 行列高さ
	pSpringblock->SetPos(					// 位置
		D3DXVECTOR3(Grid.nColumn * m_fSizeRange, Grid.nHeight * m_fSizeRange + 30, Grid.nLine * m_fSizeRange));
	pSpringblock->SetModelId(TYPE_BLOCK_SPRING);	// モデル番号

	//// 色がNULLではないなら
	//if (pCol != NULL)
	//{
	//	// モデルの色を変える
	//	pSpringblock->SetModelColor(*pCol);
	//}
	// 初期化処理
	pSpringblock->Init();
	// 生成したオブジェクトを返す
	return pSpringblock;
}
