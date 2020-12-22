// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ベースブロック処理 [baseblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"
#include "debugproc.h"
#include "stencilshadow.h"
#include "game.h"
#include "circleshadow.h"
#include "bombblock..h"
#include "electricblock.h"
#include "3dparticle.h"
#include "player.h"
#include "damagefloor.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define BLOCK_GRAVITY			(1.0f)									// ブロックにかかる重力
#define STENCIL_SIZE_Y			(200.0f)								// ステンシルバッファのyのサイズ
#define BLOCK_SIZE_RANGE		(25.0f)									// ブロックのサイズの範囲
#define BLOCK_SHADOWSIZE		(25.0f)									// ステンシルシャドウのサイズ
#define BLOCK_STATUS_TXT		("data/LOAD/STATUS/BlockStatus.txt")	// ブロックのステータスのテキスト
#define BLOCK_LIMIT_Y			(-500.0f)								// ブロックの制限区域
#define BLOCK_LIMIT_WEIGHT		(1)										// 重さの限界
#define BLOCK_MAX_RATIO			(10)									// 倍率
#define BLOCK_SPECIALPOW		(2)										// 特殊ブロックの位置を微調整
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 試験用
CBaseblock::BLOCK_STATUS CBaseblock::m_BlockStatus = {};
int	CBaseblock::m_nPhase = 0;					// フェーズ
CBaseblock::HEIGHT_PRIORITY	CBaseblock::m_Priority[BASEBLOCK_FIELDMAX][BASEBLOCK_FIELDMAX] = {};			// 優先順位
int CBaseblock::m_nMaxHeight = 0;				// 最大高さ
int	CBaseblock::m_nMaxWeight = 0;				// 最大重さ
float	CBaseblock::m_fSizeRange = 0.0f;		// サイズ範囲
std::vector<int>	CBaseblock::m_nFeedValue;	// フェードの値

#if BASEBLOCK_DEBUG

int CBaseblock::m_nAll = 0;						// 個数の総数

#endif // BASEBLOCK_DEBUG


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::CBaseblock() : CScene_X::CScene_X()
{
	m_pShadowPolygon = NULL;							// シャドウポリゴン
	m_BlockType = BLOCKTYPE_NORMAL;								// タイプ
	m_bFall = false;									// 落ちる状態
	CScene::SetObj(CScene::OBJ::OBJ_BLOCK);				// オブジェクトタイプの設定
	m_PushAfeter.bPushState = false;					// 当たった方向
	m_PushAfeter.PushGrid = GRID(0,0,0);				// 押し出し力
	m_bShadow = true;									// シャドウの使用状態
	m_bUse = true;										// 使用状態
	m_fGravity = 0.0f;									// 重力
	m_pUpBlock = NULL;									// 上にあるブロック情報
	m_pDownBlock = NULL;									// 下にあるブロック情報
#if BASEBLOCK_DEBUG

	m_nAll++;

#endif
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::~CBaseblock()
{
#if BASEBLOCK_DEBUG

	m_nAll--;
#endif
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Init()
{
	// シーンXの初期化処理
	CScene_X::Init();
	// フィールドブロック以外なら
	if (m_BlockType != BLOCKTYPE_FIELD)
	{
		// シャドウポリゴンの生成
		m_pShadowPolygon = CCircleshadow::Create(CScene_X::GetPos(),
			D3DXVECTOR3(BLOCK_SHADOWSIZE, 0.0f, BLOCK_SHADOWSIZE),
			-1
		);
		m_pShadowPolygon->SetCol(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
		m_pShadowPolygon->Set_Vtx_Col();
	}
	else
	{
		m_fGravity = 1.0f;									// 重力
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Uninit(void)
{
	// ブロックの上下情報をNULLに
	BlockMoveOrDelete();
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update(void)
{
	if (!m_bUse) return;
	// 前回の位置を代入
	m_posOld = CScene_X::GetPos();
	// シーンX更新処理
	CScene_X::Update();
	// 落ちる処理
	Update_Fall();
	// 押し出し状態のブロック更新処理
	Update_PushState();
	// 自身のシャドウの出現条件処理
	Update_MyShadow();
	// 上限処理
	Update_Limit();
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
		if (pBlock->m_grid.nColumn != this->m_grid.nColumn ||
			pBlock->m_grid.nLine != this->m_grid.nLine) continue;
		// 当たり判定処理
		Collision(pBlock);
		// ブロックのステンシルシャドウの更新
		Update_OtherShadow(pBlock);
	}

	// プッシュされている状態ではないなら
	if (m_PushAfeter.bPushState ||
		m_bFall)
	{
		COLLISIONDIRECTION		Direct = COLLISIONDIRECTION::NONE;	// 当たり判定の方向
		// プレイヤーのループ
		for (int nCntPlayer = 0; nCntPlayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntPlayer++)
		{
			// 情報取得
			CPlayer * pPlayer = (CPlayer *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntPlayer);	// プレイヤー情報
			// 取得したブロックのNULLチェック ||
			// 取得したブロックと現在ブロックが同じ
			// ->ループスキップ
			if (pPlayer == NULL) continue;
			D3DXVECTOR3 PlayerSize = CPlayer::GetPlayerStatus().PlayerSize;
			CPlayer::DIESTATUS &PlayerDieStatus = pPlayer->GetPlayerDieStatus();
			Direct = CBaseblock::PushCollision_BlockMove(
				CScene::OBJ_PLAYER,
				&pPlayer->GetPos(),
				&pPlayer->GetPosOld(),
				&pPlayer->GetMove(),
				&PlayerSize,
				CPlayer::GetPlayerStatus().PlayerOffSet
			);

			// ブロックの判定
			// 前
			if (Direct == COLLISIONDIRECTION::FRONT)
			{
				PlayerDieStatus.bFront = true;
			}
			// 後
			else if (Direct == COLLISIONDIRECTION::BACK)
			{
				PlayerDieStatus.bBack = true;
			}
			// 左
			else if (Direct == COLLISIONDIRECTION::LEFT)
			{
				PlayerDieStatus.bLeft = true;
			}
			// 右
			else if (Direct == COLLISIONDIRECTION::RIGHT)
			{
				PlayerDieStatus.bRight = true;
			}
			// 上
			else if (Direct == COLLISIONDIRECTION::UP)
			{
				PlayerDieStatus.bUp = true;
			}
			// 下
			else if (Direct == COLLISIONDIRECTION::DOWN)
			{
				PlayerDieStatus.bDown = true;
			}
		}
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
	pos.y -= m_fGravity;
	// 位置設定
	CScene_X::SetPos(pos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出し状態のブロックの更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_PushState(void)
{
	// 押し出し状態がfalseなら
	// ->関数を抜ける
	if (!m_PushAfeter.bPushState) return;
	// 位置情報取得
	D3DXVECTOR3 pos = CScene_X::GetPos();
	// 位置更新
	// 列の値が設定されているなら
	if (m_PushAfeter.PushGrid.nLine)
	{
		// z位置の更新
		pos.z += m_BlockStatus.fMove * m_PushAfeter.PushGrid.nLine;
		if (m_PushAfeter.PushGrid.nLine < 0 &&	// 列が0未満
			pos.z < m_PushAfeter.GoalPos.z)			// z位置が規定値未満なら
		{
			// 位置設定
			pos.z = m_PushAfeter.GoalPos.z;
			// 押す前のブロックの上にあったブロックを落とさせる
			CBaseblock::FallBlock_Grid(m_grid);
			// 押したブロックの現在までいた行列の高さ情報を更新
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// 動いた後の行列高を設定
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// 落ちる処理
			this->SetFall(true);
			// 押し出し後の変数を初期化する
			m_PushAfeter.Init();

		}
		else if (m_PushAfeter.PushGrid.nLine > 0 &&	// 列が0超過
			pos.z > m_PushAfeter.GoalPos.z)				// z位置が規定値超過なら
		{
			// 位置設定
			pos.z = m_PushAfeter.GoalPos.z;
			// 押す前のブロックの上にあったブロックを落とさせる
			CBaseblock::FallBlock_Grid(m_grid);
			// 押したブロックの現在までいた行列の高さ情報を更新
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// 動いた後の行列高を設定
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// 落ちる処理
			this->SetFall(true);
			// 押し出し後の変数を初期化する
			m_PushAfeter.Init();
		}
	}
	// 行の値が設定されているなら
	else if (m_PushAfeter.PushGrid.nColumn)
	{
		// x位置の更新
		pos.x += m_BlockStatus.fMove * m_PushAfeter.PushGrid.nColumn;
		if (m_PushAfeter.PushGrid.nColumn < 0 &&	// 列が0未満
			pos.x < m_PushAfeter.GoalPos.x)			// z位置が規定値未満なら
		{
			// 位置設定
			pos.x = m_PushAfeter.GoalPos.x;
			// 押す前のブロックの上にあったブロックを落とさせる
			CBaseblock::FallBlock_Grid(m_grid);
			// 押したブロックの現在までいた行列の高さ情報を更新
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// 動いた後の行列高を設定
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// 落ちる処理
			this->SetFall(true);
			// 押し出し後の変数を初期化する
			m_PushAfeter.Init();
		}
		else if (m_PushAfeter.PushGrid.nColumn > 0 &&	// 列が0超過
			pos.x > m_PushAfeter.GoalPos.x)				// z位置が規定値超過なら
		{
			// 位置設定
			pos.x = m_PushAfeter.GoalPos.x;
			// 押す前のブロックの上にあったブロックを落とさせる
			CBaseblock::FallBlock_Grid(m_grid);
			// 押したブロックの現在までいた行列の高さ情報を更新
			CBaseblock::SetHeight(m_grid + CBaseblock::GRID(0, -1, 0));
			// 動いた後の行列高を設定
			this->SetGrid(this->GetGrid() + m_PushAfeter.PushGrid);
			// 落ちる処理
			this->SetFall(true);
			// 押し出し後の変数を初期化する
			m_PushAfeter.Init();
		}
	}
	else
	{
		// 押し出し後の変数を初期化する
		m_PushAfeter.Init();
	}
	//
	// 位置設定
	CScene_X::SetPos(pos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自身のシャドウの出現条件処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_MyShadow(void)
{
	// フィールドブロックなら
	if (m_BlockType == BLOCKTYPE_FIELD)
	{
		m_bShadow = false;
		return;
	}
	// シャドウのポリゴンのNULLチェック
	else if (!m_pShadowPolygon)
	{
		m_bShadow = false;
		return;
	}
	// 真下にブロックが存在していないなら
	else if (CBaseblock::GetHeight(this->GetGrid().nColumn, this->GetGrid().nLine) < 0)
	{
		ShadowRelease();
		m_bShadow = false;
		return;
	}
	// 落ちている状態なら
	// ->シャドウの使用状態をfalseに
	else if (!this->GetFall())
	{
		m_pShadowPolygon->SetShadow(false);
		m_bShadow = false;
		return;
	}
	else
	{
		m_bShadow = true;
	}
	// 変数宣言
	D3DXVECTOR3 ShadowPos;	// シャドウ位置
	// xyの位置設定
	ShadowPos = m_pShadowPolygon->GetPos();
	ShadowPos.x = CScene_X::GetPos().x;
	ShadowPos.z = CScene_X::GetPos().z;
	m_pShadowPolygon->SetPos(ShadowPos);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 上限処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_Limit(void)
{
	// 位置情報取得
	D3DXVECTOR3 pos = CScene_X::GetPos();
	// 下限より下に落ちていったら
	if (pos.y < BLOCK_LIMIT_Y)
	{
		// リリース処理
		Release();
		// シャドウのリリース処理
		ShadowRelease();
	}
	if (m_bFall) return;
	// ダメージ床のループ
	for (int nCntDamageFloor = 0; nCntDamageFloor < CScene::GetMaxLayer(CScene::LAYER_FLOOR); nCntDamageFloor++)
	{
		// 情報取得
		CDamageFloor * pDamageFloor = (CDamageFloor *)CScene::GetScene(CScene::LAYER_FLOOR, nCntDamageFloor);	// ダメージ床情報
		// 取得したダメージ床のNULLチェック ||
		// 取得したダメージ床と現在ダメージ床が同じ
		// ->ループスキップ
		if (pDamageFloor == NULL) continue;

		// ブロックがダメージ床に取り込まれたら
		if (pos.y + m_fSizeRange < pDamageFloor->GetPos().y)
		{
			// 使用状態をfalseに
			m_bUse = false;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Collision(CBaseblock * pBlock)
{
	// 落ちいない状態なら
	if (!this->GetFall() ||
		this->GetType() == CBaseblock::BLOCKTYPE_FIELD) return;
	// 変数宣言
	COLLISIONDIRECTION Direct;	// 当たり判定方向
	// 当たり判定
	Direct = Collision(
		CScene::OBJ_BLOCK,
		&pBlock->GetPos(),
		&pBlock->GetPosOld(),
		&pBlock->GetModel()->size,
		D3DXVECTOR3(0.0f, pBlock->GetModel()->size.y * 0.5f, 0.0f)
	);
	// どこにもあたっていなければ
	if (Direct == COLLISIONDIRECTION::NONE) return;
	// 下方向に当たっていたら
	else if (Direct == COLLISIONDIRECTION::DOWN)
	{
		// 相手の落ちる状態がtrueなら
		// ->関数を抜ける
		if (pBlock->GetFall()) return;
		// 変数宣言
		CBaseblock::GRID MyGrid = this->GetGrid();		// 自分の行列高
		CBaseblock::GRID OppGrid = pBlock->GetGrid();	// 相手の行列高
		// 同じ行列ではないなら
		// ->関数を抜ける
		if (!(MyGrid.nColumn == OppGrid.nColumn &&
			MyGrid.nLine == OppGrid.nLine)) return;
		// 変数宣言
		int nHeight = this->GetHeight(				// 高さ
			MyGrid.nColumn,
			MyGrid.nLine) + 1;
		// 高さを行列高に代入
		MyGrid.nHeight = nHeight;
		// 高さの設定
		this->SetHeight(
			MyGrid.nColumn,
			MyGrid.nLine,
			MyGrid.nHeight
		);
		// 現在の行列高の設定
		this->SetGrid(MyGrid);
		// 位置設定
		this->SetPos(MyGrid.GetPos(m_fSizeRange));
		// 落ちている状態設定
		this->SetFall(false);

		// 上下ブロック情報をつなげる
		CBaseblock * pThisBlock = pBlock;
		CBaseblock * pUpBlock = pBlock->m_pUpBlock;
		while (pUpBlock)
		{
			if (pUpBlock == this) break;
			pThisBlock = pUpBlock;
			pUpBlock = pThisBlock->m_pUpBlock;
		}
		this->m_pDownBlock = pThisBlock;
		pThisBlock->m_pUpBlock = this;
	}
	else if (Direct == COLLISIONDIRECTION::UP)
	{
		// 相手の落ちる状態がtrueなら
		// ->関数を抜ける
		if (pBlock->GetFall()) return;
		// 上下ブロック情報をつなげる
		this->m_pUpBlock = pBlock;
		pBlock->m_pDownBlock = this;
	}
	else
	{
		// プレイヤーから押されていない場合
		// ->関数を抜ける
		if (!pBlock->m_PushAfeter.bPushState) return;
		// 落ちているブロックが相手のブロックの位置の半分以上の高さなら
		else if (this->GetPos().y > pBlock->GetPos().y + m_fSizeRange * 0.5f)
		{
			// 変数宣言
			CBaseblock::GRID FallGrid = this->GetGrid();	// 行列高
			// 相手の行列高の設定
			pBlock->SetGrid(CBaseblock::GRID(FallGrid.nColumn,pBlock->GetGrid().nHeight,FallGrid.nLine));
			// 相手の動いた後の位置設定
			pBlock->SetPos(pBlock->GetGrid().GetPos(m_fSizeRange));
			// 自身の行列高の設定
			this->SetGrid(CBaseblock::GRID(FallGrid.nColumn, pBlock->GetGrid().nHeight + 1, FallGrid.nLine));
			// 自身の動いた後の位置設定
			this->SetPos(pBlock->GetGrid().GetPos(m_fSizeRange));
			// 落ちる処理
			this->SetFall(false);
			// 押したブロックの現在までいた行列の高さ情報を更新
			CBaseblock::SetHeight(this->GetGrid() + CBaseblock::GRID(0, -1, 0));
			// 押し出し力の初期化
			pBlock->m_PushAfeter = PUSHAFTER(false, CBaseblock::GRID(0, 0, 0));
		}
		else
		{
			// プレイヤーから押されていない場合
			// ->関数を抜ける
			if (!pBlock->m_PushAfeter.bPushState) return;
			// 押し出し力の初期化
			pBlock->m_PushAfeter = PUSHAFTER(false,CBaseblock::GRID(0,0,0));
			// 元の位置に戻す
			pBlock->SetPos(pBlock->GetGrid().GetPos(m_fSizeRange));
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// シャドウの更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Update_OtherShadow(CBaseblock * pBlock)
{
	// シャドウの使用状態がfalseなら
	if (!m_bShadow) return;
	// 一マス下にブロックがあるなら
	// ->シャドウの使用状態をfalseに
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
			// 透明度の更新
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_BlockStatus.fBasicShadowSize));
			m_pShadowPolygon->Set_Vtx_Col();
		}
		// それ以外
		else
		{
			// 透明度の更新
			float fSize = m_fSizeRange * m_BlockStatus.nAppearance - fDiffHeight;
			m_pShadowPolygon->SetPos(pBlock->GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f));
			m_pShadowPolygon->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_BlockStatus.fBasicShadowSize * fSize));
			m_pShadowPolygon->Set_Vtx_Col();
		}

	}
}


#if BASEBLOCK_DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全体個数を表示するデバッグ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::NumAllDebug(void)
{
	CDebugproc::Print("ブロックの総数(%d)\n", m_nAll);
}
#endif // BASEBLOCK_DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Draw(void)
{
	if (!m_bUse) return;
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

	// 素材のZ範囲
	if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f)
	{
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f)
		{

			// 当たり判定(下)
			if (!this->m_pDownBlock &&
				pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= BlockPos.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;

				// 素材状の左に
				pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;
				posOld->y = pos->y;
				// 移動量の初期化
				move->y = -1.0f;
				// 押し出し状態がtrue
				bPush = true;
			}

			// 当たり判定(上)
			else if (!this->m_pUpBlock &&
				pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= BlockPos.y + m_fSizeRange)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
				// 素材状の左に
				pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
				posOld->y = pos->y;
				// 移動量の初期化
 				move->y = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
				// タイプがボムなら
				if (m_BlockType == BLOCKTYPE_BOMB)
				{
					// ボムの状態設定
					CBombblock * pBombBlock = (CBombblock *)this;
					pBombBlock->SetbBomb(true);
				}
				// タイプが電気なら
				else if (m_BlockType == BLOCKTYPE_ELECTRIC)
				{
					// 電気の状態設定
					CElectricblock * pElectBlock = (CElectricblock *)this;
					pElectBlock->SetElectric(true);
				}
				// タイプがばねなら
				else if (m_BlockType == BLOCKTYPE_SPRING)
				{
					// 電気の状態設定
					move->y = 0;
				}


			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// 当たり判定(下)
				if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
					pos->y + OffsetPos.y + size->y * 0.5f <= m_posOld.y)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::DOWN;

					// 素材状の左に
					pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;
					posOld->y = pos->y;

					// 移動量の初期化
					move->y = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}

				// 当たり判定(上)
				else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y * 0.5f >= m_posOld.y + m_fSizeRange)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::UP;
					// 素材状の左に
					pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
					posOld->y = pos->y;
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
				else if (pos->y + OffsetPos.y > BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::UP;
				}
			}
		}
	}
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
				posOld->x = pos->x;
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
				posOld->x = pos->x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// 当たり判定(左)
				if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x + size->x * 0.5f <= m_posOld.x - m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::LEFT;
					// 素材状の左に
					pos->x = BlockPos.x - m_fSizeRange * 0.5f - size->x * 0.5f - OffsetPos.x;
					posOld->x = pos->x;
					// 移動量の初期化
					move->x = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}
				// 当たり判定(右)
				else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x - size->x * 0.5f >= m_posOld.x + m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::RIGHT;
					// 素材状の左に
					pos->x = BlockPos.x + m_fSizeRange * 0.5f + size->x * 0.5f - OffsetPos.x;
					posOld->x = pos->x;
					// 移動量の初期化
					move->x = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}
				// 当たり判定(左)
				else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::LEFT;
				}
				// 当たり判定(右)
				else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::RIGHT;
				}
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
				posOld->z = pos->z;
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
				posOld->z = pos->z;

				// 移動量の初期化
				move->z = 0.0f;
			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// 当たり判定(手前)
				if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z + size->z * 0.5f <= m_posOld.z - m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::BACK;
					// 素材状の左に
					pos->z = BlockPos.z - m_fSizeRange * 0.5f - size->z * 0.5f - OffsetPos.z;
					posOld->z = pos->z;
					// 移動量の初期化
					move->z = 0.0f;
				}

				// 当たり判定(奥)
				else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z - size->z * 0.5f >= m_posOld.z + m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::FRONT;

					// 素材状の左に
					pos->z =
						BlockPos.z + m_fSizeRange * 0.5f +
						size->z * 0.5f - OffsetPos.z;
					posOld->z = pos->z;

					// 移動量の初期化
					move->z = 0.0f;
				}
				// 当たり判定(手前)
				else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::BACK;
				}
				// 当たり判定(奥)
				else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::FRONT;
				}
			}
		}
	}

	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出し当たり判定
//	pos		: 位置
//	posOld	: 前回の位置
//	move	: 移動量
//	size	: サイズ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushCollision_Side(CScene::OBJ const & Obj, D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, D3DXVECTOR3 const & OffsetPos)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	bool bPush = false;
	// 変数宣言
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	CBaseblock::GRID MyGrid = this->GetGrid();					// 行列高
	CGame::STAGE Stage = CGame::GetStage();						// ステージ

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
				posOld->x = pos->x;
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
				posOld->x = pos->x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// 当たり判定(左)
				if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x + size->x * 0.5f <= m_posOld.x - m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::LEFT;
					// 素材状の左に
					pos->x = BlockPos.x - m_fSizeRange * 0.5f - size->x * 0.5f - OffsetPos.x;
					posOld->x = pos->x;
					// 移動量の初期化
					move->x = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}
				// 当たり判定(右)
				else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x - size->x * 0.5f >= m_posOld.x + m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::RIGHT;
					// 素材状の左に
					pos->x = BlockPos.x + m_fSizeRange * 0.5f + size->x * 0.5f - OffsetPos.x;
					posOld->x = pos->x;
					// 移動量の初期化
					move->x = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}
				// 当たり判定(左)
				else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::LEFT;
				}
				// 当たり判定(右)
				else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
					pos->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::RIGHT;
				}
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
				posOld->z = pos->z;
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
				posOld->z = pos->z;

				// 移動量の初期化
				move->z = 0.0f;
			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// 当たり判定(手前)
				if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z + size->z * 0.5f <= m_posOld.z - m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::BACK;
					// 素材状の左に
					pos->z = BlockPos.z - m_fSizeRange * 0.5f - size->z * 0.5f - OffsetPos.z;
					posOld->z = pos->z;
					// 移動量の初期化
					move->z = 0.0f;
				}

				// 当たり判定(奥)
				else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z - size->z * 0.5f >= m_posOld.z + m_fSizeRange * 0.5f)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::FRONT;

					// 素材状の左に
					pos->z =
						BlockPos.z + m_fSizeRange * 0.5f +
						size->z * 0.5f - OffsetPos.z;
					posOld->z = pos->z;

					// 移動量の初期化
					move->z = 0.0f;
				}
				// 当たり判定(手前)
				else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::BACK;
				}
				// 当たり判定(奥)
				else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
					pos->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::FRONT;
				}
			}
		}
	}

	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出し当たり判定
//	pos		: 位置
//	posOld	: 前回の位置
//	move	: 移動量
//	size	: サイズ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushCollision_UpDown(CScene::OBJ const & Obj, D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, D3DXVECTOR3 const & OffsetPos)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	bool bPush = false;
	// 変数宣言
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	CBaseblock::GRID MyGrid = this->GetGrid();					// 行列高
	CGame::STAGE Stage = CGame::GetStage();						// ステージ

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
				posOld->y = pos->y;
				// 移動量の初期化
				move->y = -1.0f;
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
				posOld->y = pos->y;
				// 移動量の初期化
				move->y = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
				// タイプがボムなら
				if (m_BlockType == BLOCKTYPE_BOMB)
				{
					// ボムの状態設定
					CBombblock * pBombBlock = (CBombblock *)this;
					pBombBlock->SetbBomb(true);
				}
				// タイプが電気なら
				else if (m_BlockType == BLOCKTYPE_ELECTRIC)
				{
					// 電気の状態設定
					CElectricblock * pElectBlock = (CElectricblock *)this;
					pElectBlock->SetElectric(true);
				}

			}
			if (Direct == COLLISIONDIRECTION::NONE)
			{
				// 当たり判定(下)
				if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
					pos->y + OffsetPos.y + size->y * 0.5f <= m_posOld.y)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::DOWN;

					// 素材状の左に
					pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;
					posOld->y = pos->y;

					// 移動量の初期化
					move->y = 0.0f;
					// 押し出し状態がtrue
					bPush = true;
				}

				// 当たり判定(上)
				else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y * 0.5f >= m_posOld.y + m_fSizeRange)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::UP;
					// 素材状の左に
					pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
					posOld->y = pos->y;
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
				else if (pos->y + OffsetPos.y > BlockPos.y + m_fSizeRange&&
					pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange)
				{
					// めり込んでいる
					Direct = COLLISIONDIRECTION::UP;
				}
			}
		}
	}
	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出し当たり判定(ブロックが動いている場合)
//	Obj		: オブジェタイプ
//	pos		: 位置
//	move	: 移動量
//	size	: サイズ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CBaseblock::PushCollision_BlockMove(
	CScene::OBJ const & Obj,						// オブジェタイプ
	D3DXVECTOR3 * pos,								// 位置
	D3DXVECTOR3 * posOld,
	D3DXVECTOR3 * move,								// 移動量
	D3DXVECTOR3 * size,								// サイズ
	D3DXVECTOR3 const & OffsetPos					// オフセット位置
)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	bool bPush = false;
	// 変数宣言
	D3DXVECTOR3 BlockPos = CScene_X::GetPos();
	D3DXVECTOR3 BlockPosOld = m_posOld;
	CBaseblock::GRID MyGrid = this->GetGrid();					// 行列高
	CGame::STAGE Stage = CGame::GetStage();						// ステージ

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
				pos->y + OffsetPos.y + size->y * 0.5f <= m_posOld.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;

				// 素材状の左に
				pos->y = BlockPos.y - size->y * 0.5f - OffsetPos.y;
				posOld->y = pos->y;

				// 移動量の初期化
				move->y = -1.0f;
				// 押し出し状態がtrue
				bPush = true;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < BlockPos.y + m_fSizeRange&&
				pos->y + OffsetPos.y - size->y * 0.5f >= m_posOld.y + m_fSizeRange)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
				// 素材状の左に
				pos->y = BlockPos.y + m_fSizeRange + size->y * 0.5f - OffsetPos.y;
				posOld->y = pos->y;
				// 移動量の初期化
				move->y = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
				// タイプがボムなら
				if (m_BlockType == BLOCKTYPE_BOMB)
				{
					// ボムの状態設定
					CBombblock * pBombBlock = (CBombblock *)this;
					pBombBlock->SetbBomb(true);
				}
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
				// タイプがボムなら
				if (m_BlockType == BLOCKTYPE_BOMB)
				{
					// ボムの状態設定
					CBombblock * pBombBlock = (CBombblock *)this;
					pBombBlock->SetbBomb(true);
				}
			}
		}
	}
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
				pos->x + OffsetPos.x + size->x * 0.5f <= m_posOld.x - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
				// 素材状の左に
				pos->x = BlockPos.x - m_fSizeRange * 0.5f - size->x * 0.5f - OffsetPos.x;
				posOld->x = pos->x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				pos->x + OffsetPos.x - size->x * 0.5f >= m_posOld.x + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
				// 素材状の左に
				pos->x = BlockPos.x + m_fSizeRange * 0.5f + size->x * 0.5f - OffsetPos.x;
				posOld->x = pos->x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(左)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > BlockPos.x - m_fSizeRange * 0.5f&&
				pos->x + OffsetPos.x + size->x * 0.5f <= BlockPos.x)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < BlockPos.x + m_fSizeRange * 0.5f&&
				pos->x + OffsetPos.x - size->x * 0.5f >= BlockPos.x)
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
				pos->z + OffsetPos.z + size->z * 0.5f <= m_posOld.z - m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
				// 素材状の左に
				pos->z = BlockPos.z - m_fSizeRange * 0.5f - size->z * 0.5f - OffsetPos.z;
				posOld->z = pos->z;
				// 移動量の初期化
				move->z = 0.0f;
			}

			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				pos->z + OffsetPos.z - size->z * 0.5f >= m_posOld.z + m_fSizeRange * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;

				// 素材状の左に
				pos->z =
					BlockPos.z + m_fSizeRange * 0.5f +
					size->z * 0.5f - OffsetPos.z;
				posOld->z = pos->z;

				// 移動量の初期化
				move->z = 0.0f;
			}
			// 当たり判定(手前)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > BlockPos.z - m_fSizeRange * 0.5f&&
				pos->z + OffsetPos.z + size->z * 0.5f <= BlockPos.z)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < BlockPos.z + m_fSizeRange * 0.5f&&
				pos->z + OffsetPos.z - size->z * 0.5f >= BlockPos.z)
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
			if (pos->y + OffsetPos.y + size->y * 0.5f > BlockPos.y&&
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
			/*
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
			*/
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
	// プレイヤーと別のオブジェクトに当たり判定
	if (this->m_BlockType == BLOCKTYPE_FIELD) return COLLISIONDIRECTION::NONE;
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
// 押し出し後の設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetPushAfter(PUSHAFTER const & PushAfter)
{
	m_PushAfeter = PushAfter;
	m_PushAfeter.GoalPos = (m_grid + m_PushAfeter.PushGrid).GetPos(m_fSizeRange);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自身のブロックが移動または削除処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockMoveOrDelete(void)
{
	// 変数宣言
	CBaseblock * pUpBlock = this->m_pUpBlock;		// 上のブロック情報
	CBaseblock * pDownBlock = this->m_pDownBlock;	// 下のブロック情報
	if (pUpBlock)
	{
		pUpBlock->m_pDownBlock = NULL;
	}
	// 下のブロックの情報NULLチェック
	if (pDownBlock)
	{
		pDownBlock->m_pUpBlock = NULL;
	}
	// 自身の下と上のブロック情報をNULLに
	this->m_pDownBlock = NULL;
	this->m_pUpBlock = NULL;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 影のリリース処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::ShadowRelease(void)
{
	// シャドウが存在しているなら
	if (m_pShadowPolygon)
	{
		// シャドウをリリース
		m_pShadowPolygon->Release();
		m_pShadowPolygon = NULL;
		m_bShadow = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 指定したベースブロックを削除する処理
//	pBlock	: ブロック情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CBaseblock::DeleteBlock(
	CBaseblock * pBlock	// ブロック情報
)
{
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::PARTICLE_ID_EXPLOSION,
		pBlock->GetPos()
	);

	// 押す前のブロックの上にあったブロックを落とさせる
	CBaseblock::FallBlock_Grid(pBlock->GetGrid());
	// 押したブロックの現在までいた行列の高さ情報を更新
	CBaseblock::SetHeight(pBlock->GetGrid() + CBaseblock::GRID(0, -1, 0));
	// シャドウをリリース処理
	pBlock->ShadowRelease();
	// リリース処理
	pBlock->Release();
	return true;
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
	// 落ちるブロック数の開放
	m_BlockStatus.v_nDropBlock.clear();
	m_BlockStatus.v_nDropBlock.shrink_to_fit();
	// 落ちる速度の開放
	m_BlockStatus.v_fBlockGravity.clear();
	m_BlockStatus.v_fBlockGravity.shrink_to_fit();
	// 落ちる速度の開放
	m_BlockStatus.v_Special.clear();
	m_BlockStatus.v_Special.shrink_to_fit();
	// ダメージ床の開放
	m_BlockStatus.v_nDamageFloorHight.clear();
	m_BlockStatus.v_nDamageFloorHight.shrink_to_fit();
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
	for (int nCntHeight = 0; nCntHeight <= CBaseblock::GetHeight(Grid.nColumn, Grid.nLine); nCntHeight++)
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
	if (nColumn + m_nFeedValue[CGame::GetStage()] < 0 || nLine + m_nFeedValue[CGame::GetStage()] < 0)
	{
		//CCalculation::Messanger("CBaseblock::GetHeight関数->行列が上限下限が超えている");
		return -1;
	}
	else if (nColumn + m_nFeedValue[CGame::GetStage()] >= BASEBLOCK_FIELDMAX ||
		nLine + m_nFeedValue[CGame::GetStage()] >= BASEBLOCK_FIELDMAX)
	{
		//CCalculation::Messanger("CBaseblock::GetHeight関数->行列が上限下限が超えている");
		return -1;
	}
	return m_Priority[nColumn + m_nFeedValue[CGame::GetStage()]][nLine + m_nFeedValue[CGame::GetStage()]].nHeight;
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
	if (nColumn + m_nFeedValue[CGame::GetStage()] < 0 || nLine + m_nFeedValue[CGame::GetStage()] < 0)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight関数->行列が上限下限が超えている");
		return;
	}
	else if (nColumn + m_nFeedValue[CGame::GetStage()] >= BASEBLOCK_FIELDMAX ||
		nLine + m_nFeedValue[CGame::GetStage()] >= BASEBLOCK_FIELDMAX)
	{
		//CCalculation::Messanger("CBaseblock::GetHeight関数->行列が上限下限が超えている");
		return;
	}

	// 高さを設定
	m_Priority[nColumn + m_nFeedValue[CGame::GetStage()]][nLine + m_nFeedValue[CGame::GetStage()]].nHeight = nHeight;
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
	if (Grid.nColumn + m_nFeedValue[CGame::GetStage()] < 0 ||
		Grid.nLine + m_nFeedValue[CGame::GetStage()] < 0
		)
	{
		//CCalculation::Messanger("CBaseblock::SetHeight関数->行列が上限下限が超えている");
		return;
	}
	else if (Grid.nColumn + m_nFeedValue[CGame::GetStage()] >= BASEBLOCK_FIELDMAX ||
		Grid.nLine + m_nFeedValue[CGame::GetStage()] >= BASEBLOCK_FIELDMAX)
	{
		//CCalculation::Messanger("CBaseblock::GetHeight関数->行列が上限下限が超えている");
		return;
	}
	else if (m_Priority[Grid.nColumn + m_nFeedValue[CGame::GetStage()]][Grid.nLine + m_nFeedValue[CGame::GetStage()]].nHeight <= Grid.nHeight) return;
	m_Priority[Grid.nColumn + m_nFeedValue[CGame::GetStage()]][Grid.nLine + m_nFeedValue[CGame::GetStage()]].nHeight = Grid.nHeight;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 現在の優先順位を再設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SetMaxPriority(void)
{
	// 変数宣言
	HEIGHT_PRIORITY * pPriority = &m_Priority[0][0];	// 高さポインタ
	int nHeight = 0;									// 代入用の高さ
	m_nMaxWeight = 0;									// 重さの初期化
	// 最大高さを代入
	for (int nCntPriority = 0; nCntPriority < BASEBLOCK_FIELDMAX * BASEBLOCK_FIELDMAX; nCntPriority++, pPriority++)
	{
		// 高さの設定
		if (nHeight >= pPriority->nHeight) continue;
		nHeight = pPriority->nHeight;
	}
	m_nMaxHeight = nHeight;
	pPriority = &m_Priority[0][0];	// 高さポインタ
	// 最大重さを代入
	for (int nCntPriority = 0; nCntPriority < BASEBLOCK_FIELDMAX * BASEBLOCK_FIELDMAX; nCntPriority++, pPriority++)
	{
		// 重さの設定
		if (pPriority->nHeight < 0) continue;
		pPriority->nWeight = (m_nMaxHeight - pPriority->nHeight) * BLOCK_MAX_RATIO + BLOCK_LIMIT_WEIGHT;
		// 最大重さの加算
		m_nMaxWeight += pPriority->nWeight;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの落ちる行列を設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::GRID CBaseblock::SetFallPos(void)
{
	// 変数宣言
	HEIGHT_PRIORITY * pPriority = &m_Priority[0][0];	// 高さポインタ
	int nFeedValue = m_nFeedValue[CGame::GetStage()];	// フィード値
	int nRand = (int)CCalculation::GetRandomRange((uint64_t)1, (uint64_t)m_nMaxWeight);
	//int nRand = rand() % m_nMaxWeight;					// 最大重さからのランダム値
	int nWeight = 0;									// 重さ計算用
	for (int nCntColumn = 0; nCntColumn < BASEBLOCK_FIELDMAX; nCntColumn++)
	{
		for (int nCntLine = 0; nCntLine < BASEBLOCK_FIELDMAX; nCntLine++)
		{
			// ブロックの設定がされていないとき
			if (pPriority->nHeight < 0)
			{
				// 優先順位ポインター更新
				pPriority++;
				continue;
			}
			// 重さを加算
			nWeight += pPriority->nWeight;
			// ランダム値が重さ計算用の値を下回ったら
			if (nRand <= nWeight)
			{
				if (nCntColumn > BASEBLOCK_FIELDMAX - 2)
				{
					nCntColumn -= 2;
				}
				if (nCntLine > BASEBLOCK_FIELDMAX - 2)
				{
					nCntLine -= 2;
				}
				// 落ちる行列高さ情報を返す
				return CBaseblock::GRID(nCntColumn - nFeedValue, CBaseblock::GetBlockStatus().nAppearance + CBaseblock::GetMaxHeight(), nCntLine - nFeedValue);
			}
			// 優先順位ポインター更新
			pPriority++;
		}
	}
	// 中心
	return GRID(0, 0, 0);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの落ちる行列を設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::GRID CBaseblock::SetSpecialFallPos(void)
{
	// 変数宣言
	HEIGHT_PRIORITY * pPriority = &m_Priority[0][0];	// 高さポインタ
	int nFeedValue = m_nFeedValue[CGame::GetStage()];	// フィード値
	int nRand = (int)CCalculation::GetRandomRange((uint64_t)1, (uint64_t)m_nMaxWeight);
	//int nRand = rand() % m_nMaxWeight;					// 最大重さからのランダム値
	int nWeight = 0;									// 重さ計算用
	for (int nCntColumn = 0; nCntColumn < BASEBLOCK_FIELDMAX; nCntColumn++)
	{
		for (int nCntLine = 0; nCntLine < BASEBLOCK_FIELDMAX; nCntLine++)
		{
			// ブロックの設定がされていないとき
			if (pPriority->nHeight < 0)
			{
				// 優先順位ポインター更新
				pPriority++;
				continue;
			}
			// 重さを加算
			nWeight += pPriority->nWeight;
			// ランダム値が重さ計算用の値を下回ったら
			if (nRand <= nWeight)
			{
				// 落ちる行列高さ情報を返す
				return CBaseblock::GRID(nCntColumn - nFeedValue, CBaseblock::GetBlockStatus().nAppearance + CBaseblock::GetMaxHeight() + BLOCK_SPECIALPOW, nCntLine - nFeedValue);
			}
			// 優先順位ポインター更新
			pPriority++;
		}
	}
	// 中心
	return GRID(0, 0, 0);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 特殊ブロックの確立取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CBaseblock::SPECIALSORT CBaseblock::GetSpecialSort(int const & nPhase)
{
	return m_BlockStatus.v_Special[nPhase];
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
	int nCntDropSprit = 0;			// 落とすブロック数カウント
	int nCntBlockGravity = 0;		// 重力カウント
	int nCntFloor = 0;				// 床カウント
	int nCntSpecial = 0;			// 特殊ブロックカウント
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

						// AppBlockが来たら
						if (strcmp(cHeadText, "AppBlock") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nAppBlock);
						}
						// Moveが来たら
						else if (strcmp(cHeadText, "Move") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_BlockStatus.fMove);
						}
						// Jumpが来たら
						else if (strcmp(cHeadText, "App") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nAppearance);
						}
						// MaxSpritが来たら
						else if (strcmp(cHeadText, "MaxSprit") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nMaxSprit);

							// 分割数分配列確保する
							std::vector<INTEGER2>	v_nSprit(m_BlockStatus.nMaxSprit);
							std::vector<FLOAT2>		v_fSprit(m_BlockStatus.nMaxSprit);
							std::vector<int>		v_nFloorSprit(m_BlockStatus.nMaxSprit);
							std::vector<SPECIALSORT>	v_Special(m_BlockStatus.nMaxSprit);
							for (size_t CntSprit = 0; CntSprit < v_nSprit.size(); CntSprit++)
							{
								v_nSprit[CntSprit] = INTEGER2(0, 0);
								v_fSprit[CntSprit] = FLOAT2(0, 0);
								v_nFloorSprit[CntSprit] = 0;
								v_Special[CntSprit].Init();
							}
							m_BlockStatus.v_nDropBlock = v_nSprit;
							m_BlockStatus.v_fBlockGravity = v_fSprit;
							m_BlockStatus.v_nDamageFloorHight = v_nFloorSprit;
							m_BlockStatus.v_Special = v_Special;
						}
						// ChangeTimeが来たら
						else if (strcmp(cHeadText, "ChangeTime") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nChangeTime);
						}
						// DropBlockが来たら
						else if (strcmp(cHeadText, "DropBlock") == 0)
						{
							if (nCntDropSprit >= (int)m_BlockStatus.v_nDropBlock.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d %d",
								&cDie, &cDie,
								&m_BlockStatus.v_nDropBlock[nCntDropSprit].nMax,
								&m_BlockStatus.v_nDropBlock[nCntDropSprit].nMin);
							nCntDropSprit++;
						}
						// BlockGravityが来たら
						else if (strcmp(cHeadText, "BlockGravity") == 0)
						{
							if (nCntBlockGravity >= (int)m_BlockStatus.v_fBlockGravity.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %f %f",
								&cDie, &cDie,
								&m_BlockStatus.v_fBlockGravity[nCntBlockGravity].fMax,
								&m_BlockStatus.v_fBlockGravity[nCntBlockGravity].fMin);
							nCntBlockGravity++;
						}
						// 特殊ブロックがきたら //
						else if (strcmp(cHeadText, "NONE") == 0)
						{
							if (nCntSpecial >= (int)m_BlockStatus.v_Special.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_Special[nCntSpecial].anSpecial[SPECIALBLOCK_NONE]);
						}
						else if (strcmp(cHeadText, "BOMB") == 0)
						{
							if (nCntSpecial >= (int)m_BlockStatus.v_Special.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_Special[nCntSpecial].anSpecial[SPECIALBLOCK_BOMB]);
						}
						else if (strcmp(cHeadText, "SPRING") == 0)
						{
							if (nCntSpecial >= (int)m_BlockStatus.v_Special.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_Special[nCntSpecial].anSpecial[SPECIALBLOCK_SPRING]);
						}
						else if (strcmp(cHeadText, "STEEL") == 0)
						{
							if (nCntSpecial >= (int)m_BlockStatus.v_Special.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_Special[nCntSpecial].anSpecial[SPECIALBLOCK_STEEL]);
						}
						else if (strcmp(cHeadText, "PANIC") == 0)
						{
							if (nCntSpecial >= (int)m_BlockStatus.v_Special.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_Special[nCntSpecial].anSpecial[SPECIALBLOCK_PANIC]);
						}
						else if (strcmp(cHeadText, "ELECTRIC") == 0)
						{
							if (nCntSpecial >= (int)m_BlockStatus.v_Special.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_Special[nCntSpecial].anSpecial[SPECIALBLOCK_ELECTRIC]);
						}
						else if (strcmp(cHeadText, "SPECIALEND") == 0)
						{
							nCntSpecial++;
						}


						// ダメージ床の初期位置が来たら
						else if (strcmp(cHeadText, "InitFloor") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_BlockStatus.fInitFloor);
						}
						// FloorMoveが来たら
						else if (strcmp(cHeadText, "FloorMove") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_BlockStatus.fFloorMove);
						}
						// FloorPhaseが来たら
						else if (strcmp(cHeadText, "FloorPhase") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_BlockStatus.nFloorPhase);
						}
						// DamegeFloorが来たら
						else if (strcmp(cHeadText, "DamegeFloor") == 0)
						{
							if (nCntFloor >= (int)m_BlockStatus.v_nDamageFloorHight.size())
							{
								continue;
							}
							sscanf(cReadText, "%s %s %d",
								&cDie, &cDie,
								&m_BlockStatus.v_nDamageFloorHight[nCntFloor]);
							nCntFloor++;
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
	m_BlockStatus.fBasicShadowSize = 1.0f / (m_fSizeRange * m_BlockStatus.nAppearance);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックのステータスのセーブ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusSave(void)
{
	// ファイルポイント
	FILE	*pFile = nullptr;
	int nCntDropSprit = 0;			// 落とすブロック数カウント
	int nCntBlockGravity = 0;		// 重力カウント

	// ファイルを開く
	pFile = fopen(BLOCK_STATUS_TXT, "w");

	// 開いているとき
	if (pFile)
	{
		fprintf(pFile, COMMENT02);
		fprintf(pFile, "// ブロックとダメージ床のステータス\n");
		fprintf(pFile, COMMENT02);

		fprintf(pFile, "SCRIPT\n");
		fprintf(pFile, NEWLINE);

		// セーブするモデルの情報
		fprintf(pFile, "STATUS_SET\n");
		fprintf(pFile, "// ブロックのステータス\n");
		fprintf(pFile, "	AppBlock		= %d\n", m_BlockStatus.nAppBlock);
		fprintf(pFile, "	Move			= %.1f\n", m_BlockStatus.fMove);
		fprintf(pFile, "	App				= %d\n", m_BlockStatus.nAppearance);
		fprintf(pFile, "	MaxSprit		= %d\n", m_BlockStatus.nMaxSprit);
		fprintf(pFile, "	ChangeTime		= %d\n", m_BlockStatus.nChangeTime);
		for (size_t nCntDropSprit = 0; nCntDropSprit < m_BlockStatus.v_nDropBlock.size(); nCntDropSprit++)
		{
			fprintf(pFile, "	Phase %zd\n",nCntDropSprit);

			fprintf(pFile, "		DropBlock		= %d %d\n",
				m_BlockStatus.v_nDropBlock[nCntDropSprit].nMax,
				m_BlockStatus.v_nDropBlock[nCntDropSprit].nMin
			);
		}
		for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_fBlockGravity.size(); nCntBlockGravity++)
		{
			fprintf(pFile, "	Phase %zd\n", nCntBlockGravity);

			fprintf(pFile, "		BlockGravity	= %f %f\n",
				m_BlockStatus.v_fBlockGravity[nCntBlockGravity].fMax,
				m_BlockStatus.v_fBlockGravity[nCntBlockGravity].fMin
			);
		}
		for (size_t nCntSprit = 0; nCntSprit < m_BlockStatus.v_Special.size(); nCntSprit++)
		{
			fprintf(pFile, "	Phase %zd\n", nCntBlockGravity);
			fprintf(pFile, "	SPECIAL\n");
			int * npSpecial = (int *)m_BlockStatus.v_Special[nCntSprit];
			for (int nCntSpecial = 0; nCntSpecial < SPECIALBLOCK_MAX; nCntSpecial++, npSpecial++)
			{
				if (nCntSpecial == SPECIALBLOCK_NONE)
				{
					fprintf(pFile, "		NONE	= %d\n",
						*npSpecial
					);
				}
				else if (nCntSpecial == SPECIALBLOCK_BOMB)
				{
					fprintf(pFile, "		BOMB	= %d\n",
						*npSpecial
					);
				}
				else if (nCntSpecial == SPECIALBLOCK_SPRING)
				{
					fprintf(pFile, "		SPRING	= %d\n",
						*npSpecial
					);
				}
				else if (nCntSpecial == SPECIALBLOCK_STEEL)
				{
					fprintf(pFile, "		STEEL	= %d\n",
						*npSpecial
					);
				}
				else if (nCntSpecial == SPECIALBLOCK_PANIC)
				{
					fprintf(pFile, "		PANIC	= %d\n",
						*npSpecial
					);
				}
				else if (nCntSpecial == SPECIALBLOCK_ELECTRIC)
				{
					fprintf(pFile, "		ELECTRIC	= %d\n",
						*npSpecial
					);
				}
			}
			fprintf(pFile, "	SPECIALEND\n");
		}

		fprintf(pFile, "\n// ダメージ床のステータス\n");
		fprintf(pFile, "	InitFloor		= %.2f\n", m_BlockStatus.fInitFloor);
		fprintf(pFile, "	FloorMove		= %.2f\n", m_BlockStatus.fFloorMove);
		fprintf(pFile, "	FloorPhase		= %d\n", m_BlockStatus.nFloorPhase);
		for (size_t nCntFloor = 0; nCntFloor < m_BlockStatus.v_nDamageFloorHight.size(); nCntFloor++)
		{
			fprintf(pFile, "	Phase %zd\n", nCntFloor);

			fprintf(pFile, "		DamegeFloor		= %d\n",
				m_BlockStatus.v_nDamageFloorHight[nCntFloor]
			);
		}

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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの静的変数を初期化する
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStaticValue(void)
{
	// 変数宣言
	HEIGHT_PRIORITY * pPriority = &m_Priority[0][0];								// 冒頭のアドレスを取得
	int nCntMax = sizeof(m_Priority) / sizeof(m_Priority[0][0]);	// 配列の個数
	for (int nCntHeight = 0; nCntHeight < nCntMax; nCntHeight++,pPriority++)
	{
		pPriority->nHeight = -1;
		pPriority->nWeight = BLOCK_LIMIT_WEIGHT;
	}

	m_nPhase = 0;
	m_nMaxWeight = 0;
	m_nMaxHeight = 0;
}


#if IMGUI_DEBUG


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全てのデバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::AllDebug(void)
{
	// ブロックを出現させる処理と合わせる
	if (ImGui::Begin(u8"ブロックとダメージ床のステータス"))
	{
		// 必要情報表示
		ImGui::Text(u8"経過時間(%d)\n", CGame::GetTime());
		ImGui::Text(u8"フェーズ状態(%d)\n", m_nPhase);
		// ブロックステータス用のImGui関数
		BlockStatusImG();
		// ダメージ床用のImGui関数
		DamageStatusImG();

		// セーブボタン
		if (ImGui::Button(u8"保存"))
		{
			// ブロックのステータスのセーブ
			BlockStatusSave();
		}
	}
	// End
	ImGui::End();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックステータス用のImGui関数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::BlockStatusImG(void)
{
	// 落ちるブロック数
	if (ImGui::TreeNode(u8"ブロックのステータス"))
	{
		// 出現する時間
		ImGui::DragInt(u8"出現する時間", &m_BlockStatus.nAppBlock, 1.0f, 1, 1000);					/* 10.0f */
																								// 移動量
		ImGui::DragFloat(u8"移動量", &m_BlockStatus.fMove, 0.1f, 0.1f, 100.0f);
		// 落ちる高さ
		ImGui::DragInt(u8"落ちる高さ", &m_BlockStatus.nAppearance, 1.0f);						/* 10.0f */
																							// 最大分割数
		if (ImGui::DragInt(u8"変化する回数", &m_BlockStatus.nMaxSprit, 1.0f, 1, 100))						/* 10.0f */
		{
			// 変数宣言
			std::vector<int>		v_nFloorSprit(m_BlockStatus.nMaxSprit);
			// 分割数に合わせ、数値を更新
			std::vector<INTEGER2>	v_Sprit(m_BlockStatus.nMaxSprit);
			std::vector<FLOAT2>		v_fSprit(m_BlockStatus.nMaxSprit);
			// 初期化
			for (int nCntSprit = 0; nCntSprit < m_BlockStatus.nMaxSprit; nCntSprit++)
			{
				v_Sprit[nCntSprit] = INTEGER2(0, 0);
				v_fSprit[nCntSprit] = FLOAT2(0, 0);
			}

			// 落ちるブロック数
			for (size_t nCntDropSprit = 0; nCntDropSprit < m_BlockStatus.v_nDropBlock.size(); nCntDropSprit++)
			{
				if (nCntDropSprit >= (size_t)m_BlockStatus.nMaxSprit) break;
				v_Sprit[nCntDropSprit] = m_BlockStatus.v_nDropBlock[nCntDropSprit];
			}
			m_BlockStatus.v_nDropBlock.clear();
			m_BlockStatus.v_nDropBlock.shrink_to_fit();
			m_BlockStatus.v_nDropBlock = v_Sprit;
			// 落ちる速度
			for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_fBlockGravity.size(); nCntBlockGravity++)
			{
				if (nCntBlockGravity >= (size_t)m_BlockStatus.nMaxSprit) break;
				v_fSprit[nCntBlockGravity] = m_BlockStatus.v_fBlockGravity[nCntBlockGravity];
			}
			m_BlockStatus.v_fBlockGravity.clear();
			m_BlockStatus.v_fBlockGravity.shrink_to_fit();
			m_BlockStatus.v_fBlockGravity = v_fSprit;
			// ダメージ床
			for (size_t nCntFloor = 0; nCntFloor < m_BlockStatus.v_nDamageFloorHight.size(); nCntFloor++)
			{
				if (nCntFloor >= (size_t)m_BlockStatus.nMaxSprit) break;
				v_nFloorSprit[nCntFloor] = m_BlockStatus.v_nDamageFloorHight[nCntFloor];
			}
			m_BlockStatus.v_nDamageFloorHight.clear();
			m_BlockStatus.v_nDamageFloorHight.shrink_to_fit();
			m_BlockStatus.v_nDamageFloorHight = v_nFloorSprit;
		}
		// 変化させる時間
		ImGui::DragInt(u8"変化させる時間", &m_BlockStatus.nChangeTime, 1.0f);					/* 10.0f */
																						// 落ちるブロック数
		if (ImGui::TreeNode(u8"落ちるブロック数(最大値、最小値)"))
		{
			for (size_t nCntDropSprit = 0; nCntDropSprit < m_BlockStatus.v_nDropBlock.size(); nCntDropSprit++)
			{
				ImGui::Text("Phase %d", nCntDropSprit);
				std::string sName = " (";
				sName += std::to_string(nCntDropSprit * m_BlockStatus.nChangeTime) + u8"秒)";
				// 落とすブロックの数
				ImGui::DragInt2(sName.c_str(), (int *)m_BlockStatus.v_nDropBlock[nCntDropSprit], 1.0f);					/* 10.0f */
			}
			ImGui::TreePop();
		}
		// 落ちる速度
		if (ImGui::TreeNode(u8"落ちる速度(最大値、最小値)"))
		{
			for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_fBlockGravity.size(); nCntBlockGravity++)
			{
				ImGui::Text("Phase %d", nCntBlockGravity);
				std::string sName = " (";
				sName += std::to_string(nCntBlockGravity * m_BlockStatus.nChangeTime) + u8"秒)";
				// 落ちる速度
				ImGui::DragFloat2(sName.c_str(), (float *)m_BlockStatus.v_fBlockGravity[nCntBlockGravity], 0.1f);					/* 10.0f */
			}
			ImGui::TreePop();
		}
		// 特殊ブロックの名前別設定
		SpecialSetImG();

		ImGui::TreePop();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 特殊ブロックの名前別設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::SpecialSetImG(void)
{
	// 特殊ブロックの落ちる確率
	if (ImGui::TreeNode(u8"特殊ブロックの落ちる確率"))
	{
		for (size_t nCntBlockGravity = 0; nCntBlockGravity < m_BlockStatus.v_Special.size(); nCntBlockGravity++)
		{
			ImGui::Text(u8"Phase %zd", nCntBlockGravity);
			ImGui::Text(u8"現在の振り分け数(%d / %d)",NowSortPoint(nCntBlockGravity), SPECIALBLOCK_MAXSORTPOINT);
			// 割り当てポイントが最大以上なら
			if (NowSortPoint(nCntBlockGravity) >= SPECIALBLOCK_MAXSORTPOINT)
			{
				ImGui::Text(u8"※※※※※ 割り当てが最大です。※※※※※");
			}
			int * npSpecial = (int *)m_BlockStatus.v_Special[nCntBlockGravity];
			for (int nCntSpecial = 0; nCntSpecial < SPECIALBLOCK_MAX; nCntSpecial++, npSpecial++)
			{
				std::string sName = "[Phase ";
				sName += std::to_string(nCntBlockGravity) + "] ";
				int nOldSpecial = *npSpecial;
				if (nCntSpecial == SPECIALBLOCK_NONE)
				{
					sName += u8"何もなし(%)";
				}
				else if (nCntSpecial == SPECIALBLOCK_BOMB)
				{
					sName += u8"ボムブロック(%)";
				}
				else if (nCntSpecial == SPECIALBLOCK_SPRING)
				{
					sName += u8"ばねブロック(%)";
				}
				else if (nCntSpecial == SPECIALBLOCK_STEEL)
				{
					sName += u8"鉄ブロック(%)";
				}
				else if (nCntSpecial == SPECIALBLOCK_PANIC)
				{
					sName += u8"パニックブロック(%)";
				}
				else if (nCntSpecial == SPECIALBLOCK_ELECTRIC)
				{
					sName += u8"電気ブロック(%)";
				}
				ImGui::InputInt(sName.c_str(), npSpecial);

				// 下限設定
				if (*npSpecial < 0)
				{
					*npSpecial = 0;
				}
				// 振り分けポイントが最大なら
				if (NowSortPoint(nCntBlockGravity) > SPECIALBLOCK_MAXSORTPOINT)
				{
					*npSpecial = nOldSpecial;
				}
			}
		}
		ImGui::TreePop();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェーズごとの各現在の確立振り分け数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CBaseblock::NowSortPoint(int const & nBlockGravity)
{
	return m_BlockStatus.v_Special[nBlockGravity].AddSpecial();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ダメージ床用のImGui関数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::DamageStatusImG(void)
{
	if (ImGui::TreeNode(u8"ダメージのステータス"))
	{
		// ダメージ床の初期位置
		ImGui::DragFloat(u8"ダメージ床の初期位置", &m_BlockStatus.fInitFloor, 0.1f);
		// ダメージ床の移動速度
		ImGui::DragFloat(u8"ダメージ床の移動速度", &m_BlockStatus.fFloorMove, 0.1f, 0.1f, 100.0f);
		// ダメージ床の上がるフェーズタイミング
		ImGui::DragInt(u8"ダメージ床の上がるPhase", &m_BlockStatus.nFloorPhase, 1.0f, 0, m_BlockStatus.nMaxSprit);						/* 10.0f */
																																// ダメージ床の高さ
		if (ImGui::TreeNode(u8"ダメージ床の高さ"))
		{
			for (size_t nCntFloor = 0; nCntFloor < m_BlockStatus.v_nDamageFloorHight.size(); nCntFloor++)
			{
				ImGui::Text("Phase %d", nCntFloor);
				std::string sName = " (";
				sName += std::to_string(nCntFloor * m_BlockStatus.nChangeTime) + u8"秒)";
				// ダメージ床の高さ
				ImGui::DragInt(sName.c_str(), &m_BlockStatus.v_nDamageFloorHight[nCntFloor], 1.0f);					/* 10.0f */
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

#endif // IMGUI_DEBUG

#ifdef _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CBaseblock::Debug(void)
{

}
#endif // _DEBUG