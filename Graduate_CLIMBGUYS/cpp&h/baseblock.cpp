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
#include "game.h"
#include "circleshadow.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define BLOCK_GRAVITY			(1.0f)									// ブロックにかかる重力
#define STENCIL_SIZE_Y			(200.0f)								// ステンシルバッファのyのサイズ
#define BLOCK_SIZE_RANGE		(25.0f)									// ブロックのサイズの範囲
#define BLOCK_SHADOWSIZE		(24.0f)									// ステンシルシャドウのサイズ
#define BLOCK_STATUS_TXT		("data/LOAD/STATUS/BlockStatus.txt")	// ブロックのステータスのテキスト

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 試験用
CBaseblock::BLOCK_STATUS CBaseblock::m_BlockStatus = {};
int CBaseblock::m_anHeight[20][20] = {};		// 1つ1つの行列の高さ

float	CBaseblock::m_fSizeRange = 0.0f;		// サイズ範囲
std::vector<int>	CBaseblock::m_nFeedValue;	// フェードの値

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::CBaseblock() : CScene_X::CScene_X()
{
	m_pShadowPolygon = NULL;
	m_type = TYPE_NORMAL;	// タイプ
	m_bFall = false;		// 落ちる状態
	CScene::SetObj(CScene::OBJ::OBJ_BLOCK);	// オブジェクトタイプの設定
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
	// フィールドブロック以外なら
	if (m_type != TYPE_FIELD)
	{
		// シャドウポリゴンの生成
		m_pShadowPolygon = CCircleshadow::Create(CScene_X::GetPos(),
			D3DXVECTOR3(m_BlockStatus.fBasicShadowSize, 0.0f, m_BlockStatus.fBasicShadowSize),
			-1
		);
	}
	/*
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
	*/
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
	// 前回の位置を代入
	m_posOld = CScene_X::GetPos();
	// シーンX更新処理
	CScene_X::Update();
	// 落ちる処理
	Update_Fall();
	// シャドウポリゴンの位置設定
	if (m_pShadowPolygon)
	{
		// 変数宣言
		D3DXVECTOR3 ShadowPos;	// シャドウ位置
		ShadowPos = m_pShadowPolygon->GetPos();
		ShadowPos.x = CScene_X::GetPos().x;
		ShadowPos.z = CScene_X::GetPos().z;
		m_pShadowPolygon->SetPos(ShadowPos);
	}
	// ブロックのループ
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK);nCntBlock++)
	{
		// 情報取得
		CBaseblock * pBlock = (CBaseblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);	// ブロック情報
		// 取得したブロックのNULLチェック ||
		// 取得したブロックと現在ブロックが同じ
		// ->ループスキップ
		if (pBlock == NULL ||
			pBlock == this) continue;
		else if (pBlock->m_grid.nColumn != this->m_grid.nColumn ||
			pBlock->m_grid.nLine != this->m_grid.nLine) continue;
		// 当たり判定処理
		Collision(pBlock);
		// ブロックのステンシルシャドウの更新
		Update_StencilShadow(pBlock);
	}
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
// 当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Collision(CBaseblock * pBlock)
{
	// 落ちている状態なら
	// ->関数を抜ける
	if (!this->GetFall()) return;
	// 変数宣言
	COLLISIONDIRECTION Direct;	// 当たり判定方向
	// 当たり判定
	Direct = Collision(
		CScene::OBJ_BLOCK,
		&pBlock->GetPos(),
		&pBlock->GetPosOld(),
		&pBlock->GetModel()->size,
		D3DXVECTOR3(0.0f, pBlock->GetModel()->size.y, 0.0f)
	);
	// ブロックが当たっているなら
	if (Direct != COLLISIONDIRECTION::NONE)
	{
		// 当たった処理
		HitCollision(Direct, CScene::OBJ_BLOCK, pBlock);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ステンシルシャドウの更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_StencilShadow(CBaseblock * pBlock)
{
	// シャドウのポリゴンのNULLチェック
	if (!m_pShadowPolygon) return;
	// フィールドブロックなら
	// ->関数を抜ける
	else if (m_type == TYPE_FIELD) return;
	// 落ちている状態なら
	// ->ステンシルシャドウの使用状態をfalseに
	else if (!this->GetFall())
	{
		m_pShadowPolygon->SetShadow(false);
		return;
	}
	// 下にブロックがあるなら
	// ->ステンシルシャドウの使用状態をfalseに
	else if (m_grid.nHeight - 1 == pBlock->GetGrid().nHeight)
	{
		m_pShadowPolygon->SetShadow(false);
		return;
	}
	
	// 高さが現在置かれているブロックの高さより高かったら

	else if (this->m_grid.nHeight > CBaseblock::GetHeight(pBlock->GetGrid().nColumn, pBlock->GetGrid().nLine) &&
		this->m_grid.nHeight > pBlock->GetGrid().nHeight)
	{
		m_pShadowPolygon->SetShadow(true);
		// 変数宣言
		float fDiffHeight = this->GetPos().y - pBlock->GetPos().y;	// 比較した高さの差
		// 高さが規定値以上なら
		if (fDiffHeight >= m_fSizeRange * m_BlockStatus.nAppearance)
		{
			// サイズ変更
			//CScene_X::GetStencillShadow()->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize, fDiffHeight, m_BlockStatus.fBasicShadowSize));
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize, 0.0f, m_BlockStatus.fBasicShadowSize));
			m_pShadowPolygon->Set_Vtx_Pos(CScene_THREE::OFFSET_TYPE_SIDE_CENTER);
		}
		// それ以外
		else
		{
			float fSize = m_fSizeRange * m_BlockStatus.nAppearance - fDiffHeight;
			// サイズ変更
			//CScene_X::GetStencillShadow()->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize * fSize, fDiffHeight , m_BlockStatus.fBasicShadowSize * fSize));
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetSize(D3DXVECTOR3(m_BlockStatus.fBasicShadowSize * fSize, 0.0f, m_BlockStatus.fBasicShadowSize * fSize));
			m_pShadowPolygon->Set_Vtx_Pos(CScene_THREE::OFFSET_TYPE_SIDE_CENTER);
		}
	}


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
// 押し出し当たり判定
//	pos		: 位置
//	posOld	: 前回の位置
//	move	: 移動量
//	size	: サイズ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushCollision(
	CScene::OBJ const & Obj,		// オブジェタイプ
	D3DXVECTOR3 * pos,				// 位置
	D3DXVECTOR3 * posOld,			// 前回の位置
	D3DXVECTOR3 * move,				// 移動量
	D3DXVECTOR3 * size,				// サイズ
	D3DXVECTOR3 const & OffsetPos	// オフセット位置
)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	bool bPush = false;
	// 変数宣言
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	CBaseblock::GRID MyGrid = this->GetGrid();					// 行列高
	CGame::STAGE Stage = CGame::GetStage();						// ステージ
	if (this->GetGrid().nHeight >= CBaseblock::GetHeight(this->GetGrid().nColumn + m_nFeedValue[Stage], this->GetGrid().nLine + m_nFeedValue[Stage]))
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f)
		{
			// 素材のX範囲
			if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f)
			{
				// 当たり判定(下)
				if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
					posOld->y + OffsetPos.y + size->y * 0.5f <= BlockPos.y)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::DOWN;

					// 素材状の左に
					pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;

					// 移動量の初期化
					move->y = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}

				// 当たり判定(上)
				else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
					posOld->y + OffsetPos.y - size->y * 0.5f >= BlockPos.y + m_fSizeRange)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::UP;
					// 素材状の左に
					pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
					// 移動量の初期化
					move->y = 0.0f;
					// 押し出し状態がtrue
					bPush = true;

				}
				// 当たり判定(下)
				else if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
					pos->y + OffsetPos.y <= BlockPos.y)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::DOWN;
				}

				// 当たり判定(上)
				else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y > BlockPos.y + m_fSizeRange)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::UP;
				}
			}
		}
	}
	// 当たった方向に情報が入っているなら
	//if (bPush) return Direct;
	// 素材のY範囲
	if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
		pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange)
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
				// 素材状の左に
				pos->x = BlockPos.x - m_fSizeRange * 0.5f - size->x * 0.5f - OffsetPos.x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
				// 素材状の左に
				pos->x = BlockPos.x + m_fSizeRange * 0.5f + size->x * 0.5f - OffsetPos.x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(左)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// 当たった方向に情報が入っているなら
		//if (bPush) return Direct;
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f)
		{
			// 当たり判定(手前)
			if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
				// 素材状の左に
				pos->z = BlockPos.z - m_fSizeRange * 0.5f - size->z * 0.5f - OffsetPos.z;
				// 移動量の初期化
				move->z = 0.0f;
			}

			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;

				// 素材状の左に
				pos->z =
					BlockPos.z + m_fSizeRange * 0.5f +
					size->z * 0.5f - OffsetPos.z;

				// 移動量の初期化
				move->z = 0.0f;
			}
			// 当たり判定(手前)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定
//	pos			: 位置
//	size		: サイズ
//	OffsetPos	: オフセット位置
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::Collision(
	CScene::OBJ const & Obj,		// オブジェタイプ
	D3DXVECTOR3 * pos,				// 位置
	D3DXVECTOR3 * posOld,			// 前回の位置
	D3DXVECTOR3 * size,				// サイズ
	D3DXVECTOR3 const & OffsetPos	// オフセット位置
)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	// 素材のZ範囲
	if (pos->z + OffsetPos.z + size->z * 0.5f >= BlockPos.z - m_fSizeRange * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + m_fSizeRange * 0.5f)
	{
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + m_fSizeRange * 0.5f)
		{
			// 当たり判定(下)
			if (pos->y + OffsetPos.y >= BlockPos.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y < BlockPos.y + m_fSizeRange)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// 素材のY範囲
	if (pos->y + OffsetPos.y + size->y * 0.5f >= BlockPos.y&&
		pos->y + OffsetPos.y - size->y * 0.5f <= BlockPos.y + m_fSizeRange)
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f >= BlockPos.z - m_fSizeRange * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= BlockPos.z + m_fSizeRange * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->z * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x + size->z * 0.5f <= BlockPos.x - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->z * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				posOld->x + OffsetPos.x - size->z * 0.5f >= BlockPos.x + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}

		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= BlockPos.x + m_fSizeRange * 0.5f)
		{
			// 当たり判定(手前)
			if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの押し出し処理
//	LineBegin	: 線の始点
//	LineEnd		: 線の終点
//	fDistance	: 距離
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushBlock(
	D3DXVECTOR3 const & LineBegin,	// 	LineBegin	: 線の始点
	D3DXVECTOR3 const & LineEnd,	// 	LineEnd		: 線の終点
	float &				fDistance	//	fDistance	: 距離
)
{
	// 仕様
	// プレイヤーと別のオブジェクトに当たり判定
	if (this->m_type == TYPE_FIELD) return COLLISIONDIRECTION::NONE;
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	// 左のポリゴンの判定
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::LEFT
	))
	{
		Direct = COLLISIONDIRECTION::LEFT;
	}
	// やること
	// 右、奥、手前の判定を取る
	// 結果どうなるか試す


	// 右のポリゴンの判定
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::RIGHT
	))
	{
		Direct = COLLISIONDIRECTION::RIGHT;
	}

	// 奥のポリゴンの判定
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z + m_fSizeRange * 0.5f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::FRONT
	))
	{
		Direct = COLLISIONDIRECTION::FRONT;
	}

	// 手前のポリゴンの判定
	if (CCalculation::PolygonToLineCollision(
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y + m_fSizeRange, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x - m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(BlockPos.x + m_fSizeRange * 0.5f, BlockPos.y, BlockPos.z - m_fSizeRange * 0.5f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		LineBegin,
		LineEnd,
		fDistance,
		COLLISIONDIRECTION::BACK
	))
	{
		Direct = COLLISIONDIRECTION::BACK;
	}

	return Direct;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ベースブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CBaseblock::Load(void)
{
	m_fSizeRange = BLOCK_SIZE_RANGE;
	BlockStatusLoad();
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::UnLoad(void)
{
	// フェードの値の開放
	m_nFeedValue.clear();
	m_nFeedValue.shrink_to_fit();
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
		
		CBaseblock * pBaseblock;								// シーン2Dクラス
		pBaseblock = Create(BlockPos[nBlockCnt], 2);			// 生成
		
		//pBaseblock->SetSize(D3DXVECTOR3(0.25f, 0.25f, 0.25f));	// サイズ設定
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 行列高さベースブロックの情報取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock * CBaseblock::GetBaseBlock(GRID & Grid)
{
	// 変数宣言
	CBaseblock * pBlock = NULL;	// ブロック情報
	// ブロック情報をループ
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK); nCntBlock++)
	{
		// 情報取得
		pBlock = (CBaseblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);	// ブロック
		// ブロック情報がNULLなら
		// ->ループスキップ
		if (pBlock == NULL) continue;
		// 行列高が同じなら
		// ->ブロック情報を返す
		if (pBlock->GetGrid() == Grid) return pBlock;
	}
	// NULLを返す
	return NULL;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 指定した行列の全ての落ちる状態をfalseに関数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::FallBlock_Grid(GRID & Grid)
{
	// 高さカウント
	for (int nCntHeight = 0; nCntHeight <= CBaseblock::GetHeight(Grid.nColumn + m_nFeedValue[CGame::GetStage()], Grid.nLine + m_nFeedValue[CGame::GetStage()]); nCntHeight++)
	{
		// 変数宣言
		GRID HeightGrid = Grid + GRID(0, nCntHeight, 0);	// 高さを足した行列
		HeightGrid.nHeight++;
		CBaseblock * pBlock = NULL;							// ブロック情報
		// 行列高さベースブロックの情報取得
		pBlock = CBaseblock::GetBaseBlock(HeightGrid);
		// ブロック情報がNULLなら
		// ->ループスキップ
		if (pBlock == NULL) continue;
		// 落ちる状態をfalseに
		pBlock->SetFall(true);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードの値取得
//	nStage	: ステージ番号
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CBaseblock::GetFeedValue(
	int const & nStage	// フェードの値
)
{
	// 指定したステージ数が読み込まれたステージ数以上の場合
	// ->-1を返す
	if (nStage >= (signed)m_nFeedValue.size())
	{
		CCalculation::Messanger("CBaseblock::GetFeedValue関数->ステージ数が読み込まれたステージ数を上回った。");
		return -1;
	}
	return m_nFeedValue[nStage];
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 現在積み重なっているブロックの高さを取得
//	nColumn	: 列
//	nLine	: 行
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CBaseblock::GetHeight(
	int const & nColumn,
	int const & nLine
)
{
	// 行列が0未満なら
	// ->関数を抜ける
	if (nColumn < 0 || nLine < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight関数->行列が上限下限が超えている");
		return 0;
	}
	return m_anHeight[nColumn][nLine];
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 現在積み重なっているブロックの高さを設定
//	nColumn	: 列
//	nLine	: 行
//	nHeight	: 高さ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetHeight(
	int const & nColumn,
	int const & nLine,
	int const & nHeight
)
{
	// 行列が0未満なら
	// ->関数を抜ける
	if (nColumn < 0 || nLine < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight関数->行列が上限下限が超えている");
		return;
	}
	m_anHeight[nColumn][nLine] = nHeight;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 現在積み重なっているブロックの高さを設定
//	Grid	: 列
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetHeight(
	GRID const & Grid
)
{
	// 行列が0未満なら
	// ->関数を抜ける
	if (Grid.nColumn < 0 || Grid.nLine < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight関数->行列が上限下限が超えている");
		return;
	}
	m_anHeight[Grid.nColumn][Grid.nLine] = Grid.nHeight;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックのステータスのロード
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusLoad(void)
{
	// ファイルポイント
	FILE *pFile = nullptr;

	char cReadText[128];			// 文字として読み取る
	char cHeadText[128];			// 比較用
	char cDie[128];					// 不要な文字

									// ファイルを開く
	pFile = fopen(BLOCK_STATUS_TXT, "r");

	// 開いているとき
	if (pFile)
	{
		// SCRIPTが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile); // 一文読み込み
			sscanf(cReadText, "%s", &cHeadText);		// 比較用テキストに文字を代入
		}

		// SCRIPTが来たら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// END_SCRIPTが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				// STATUS_SETが来たら
				if (strcmp(cHeadText, "STATUS_SET") == 0)
				{
					// END_STATUS_SETが来るまでループ
					while (strcmp(cHeadText, "END_STATUS_SET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						// Moveが来たら
						if (strcmp(cHeadText, "Move") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_BlockStatus.fMove);
						}
						// Jumpが来たら
						else if (strcmp(cHeadText, "App") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nAppearance);
						}
					}
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		// 読み込み失敗時の警告表示
		MessageBox(NULL, "ブロックのステータス読み込み失敗", BLOCK_STATUS_TXT, MB_ICONWARNING);
	}

	
	// 基本シャドウサイズの設定
	m_BlockStatus.fBasicShadowSize = BLOCK_SHADOWSIZE / (m_fSizeRange * m_BlockStatus.nAppearance);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックのステータスのセーブ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusSave(void)
{
	// ファイルポイント
	FILE	*pFile = nullptr;

	// ファイルを開く
	pFile = fopen(BLOCK_STATUS_TXT, "w");

	// 開いているとき
	if (pFile)
	{
		fprintf(pFile, COMMENT02);
		fprintf(pFile, "// ブロックのステータス\n");
		fprintf(pFile, COMMENT02);

		fprintf(pFile, "SCRIPT\n");
		fprintf(pFile, NEWLINE);

		// セーブするモデルの情報
		fprintf(pFile, "STATUS_SET\n");
		fprintf(pFile, "	Move			= %.1f\n", m_BlockStatus.fMove);
		fprintf(pFile, "	App				= %d\n", m_BlockStatus.nAppearance);
		fprintf(pFile, "END_STATUS_SET\n\n");

		fprintf(pFile, "END_SCRIPT\n");

		// ファイルを閉じる
		fclose(pFile);

		// 読み込み成功時の結果表示
		MessageBox(NULL, "セーブしました", BLOCK_STATUS_TXT, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// 読み込み失敗時の警告表示
		MessageBox(NULL, "読み込み失敗", BLOCK_STATUS_TXT, MB_ICONWARNING);
	}
}

#ifdef _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全てのデバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::AllDebug(void)
{
	if (ImGui::Begin(u8"ブロックのステータス"))
	{
		// 移動量
		ImGui::DragFloat(u8"移動量", &m_BlockStatus.fMove, 0.1f, 0.1f, 100.0f);						/* 3.0f */
		// 落ちる高さ
		ImGui::DragInt(u8"落ちる高さ", &m_BlockStatus.nAppearance, 1.0f);						/* 10.0f */

		// セーブボタン
		if (ImGui::Button(u8"保存"))
		{
			// ブロックのステータスのセーブ
			BlockStatusSave();
		}

		// 改行キャンセル
		ImGui::SameLine();

		// 初期化
		if (ImGui::Button(u8"初期化"))
		{
			// ブロックの初期ステータス代入
			//BlockStatusInitLoad();
		}
		// 区切り線
		ImGui::Separator();
	}
	// End
	ImGui::End();
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Debug(void)
{
	CDebugproc::Print("pos(%.2f,%.2f,%.2f)\n", CScene_X::GetMatrix()._41, CScene_X::GetMatrix()._42, CScene_X::GetMatrix()._43);
}
#endif // _DEBUG