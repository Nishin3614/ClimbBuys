// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ベースブロック処理 [baseblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _BASEBLOCK_H_
#define _BASEBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_x.h"
#include <random>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if ERROW_ACTION

#define BASEBLOCK_DEBUG				(0)					// デバッグ処理状態

#endif // ERROW_ACTION

#define BASEBLOCK_FIELDMAX			(14)				// フィールドのブロック数

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCircleshadow;	// 円シャドウクラス

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseblock : public CScene_X
{
public:
	/* 列挙型 */
	// ベースブロック
	typedef enum
	{
		BLOCKTYPE_NORMAL = 0,	// 通常
		BLOCKTYPE_FIELD,		// フィールドブロック
		BLOCKTYPE_BOMB,			// ボムブロック
		BLOCKTYPE_SPRING,		// スプリングブロック
		BLOCKTYPE_STEEL,		// 鋼鉄ブロック
		BLOCKTYPE_PANIC,		// 混乱ブロック
		BLOCKTYPE_ELECTRIC,		// 電気ブロック
		BLOCKTYPE_MAX			// タイプ全体数
	} BLOCKTYPE;

	// 上下左右前後
	typedef enum
	{
		DIRECTION_UP = 0b000001,	// 上
		DIRECTION_UNDER = 0b000010,	// 下
		DIRECTION_LEFT = 0b000100,	// 左
		DIRECTION_RIGHT = 0b001000,	// 右
		DIRECTION_FRONT = 0b010000,	// 前
		DIRECTION_BACK = 0b100000,	// 後ろ
		DIRECTION_MAX
	} DIRECTION;

	/* 構造体 */
	// 盤面情報
	typedef struct GRID
	{
		// コンストラクタ
		GRID() {}
		// コンストラクタ
		//	Column	: 列
		//	Height	: 高さ
		//	Line	: 行
		GRID(int const &Column, int const &Height, int const &Line)
		{
			nColumn = Column;
			nLine = Line;
			nHeight = Height;
		}
		// 行列高さの足し算のオペレーション
		GRID operator + (GRID const & Source) const
		{
			// 値を渡す変数
			GRID variable;
			variable.nColumn = nColumn + Source.nColumn;
			variable.nLine = nLine + Source.nLine;
			variable.nHeight = nHeight + Source.nHeight;
			return variable;
		}
		// 行列高さの引き算のオペレーション
		GRID operator - (GRID const & Source) const
		{
			// 値を渡す変数
			GRID variable;
			variable.nColumn = nColumn - Source.nColumn;
			variable.nLine = nLine - Source.nLine;
			variable.nHeight = nHeight - Source.nHeight;
			return variable;
		}
		// 行列高さの掛け算のオペレーション
		GRID operator * (int const & Source) const
		{
			// 値を渡す変数
			GRID variable;
			variable.nColumn = nColumn + Source;
			variable.nLine = nLine + Source;
			variable.nHeight = nHeight + Source;
			return variable;
		}
		// 行列高さの+=のオペレーション
		GRID & operator += (GRID const & Grid)
		{
			nColumn += Grid.nColumn;
			nLine += Grid.nLine;
			nHeight += Grid.nHeight;
			return *this;
		}
		// 行列高さの+=のオペレーション
		bool operator == (GRID & Grid)
		{
			return Grid.nColumn == nColumn && Grid.nLine == nLine && Grid.nHeight == nHeight;
		}
		// 行列高さの<=のオペレーション
		bool operator <= (GRID & Grid)
		{
			return nColumn <= Grid.nColumn && nLine <= Grid.nLine && nHeight <= Grid.nHeight;
		}
		// 行列高さの>=のオペレーション
		bool operator >= (GRID & Grid)
		{
			return nColumn >= Grid.nColumn && nLine >= Grid.nLine && nHeight >= Grid.nHeight;
		}
		D3DXVECTOR3 GetPos(float const & fRange)
		{
			D3DXVECTOR3 pos;
			pos.x = nColumn * fRange;
			pos.z = nLine * fRange;
			pos.y = nHeight * fRange;
			return pos;
		}
		int nColumn;	// 列
		int nLine;		// 行
		int nHeight;	// 高さ
	} GRID;
	// ブロック全体に対しての距離の比較とブロック情報とどっち方向なら押されたか情報
	typedef struct PUSHBLOCK
	{
		PUSHBLOCK() {};
		PUSHBLOCK(CBaseblock * pSouceBlock, float fSouceDistance, COLLISIONDIRECTION SouceDirection)
		{
			pBlock = pSouceBlock;
			fDistance = fSouceDistance;
			Direction = SouceDirection;
		}
		CBaseblock * pBlock;				// ブロック情報
		float fDistance;					// 距離
		COLLISIONDIRECTION Direction;		// 方向
	} PUSHBLOCK;
	// 押し出した後の変数
	typedef struct PUSHAFTER
	{
		PUSHAFTER() {};
		PUSHAFTER(bool const & SoucePushState,GRID const & SourceGrid) : bPushState(SoucePushState) , PushGrid(SourceGrid)
		{
		}
		void Init()
		{
			bPushState = false;
			PushGrid = GRID(0, 0, 0);
			GoalPos = D3DVECTOR3_ZERO;
		}
		bool		bPushState;	// 押し出し状態
		GRID		PushGrid;	// 行列高
		D3DXVECTOR3	GoalPos;	// 目的位置
	} PUSHAFTER;


	// ----- ブロックのステータス ----- //
	typedef struct _BLOCK_STATUS
	{
		_BLOCK_STATUS()
		{
			// ブロック用
			fMove = 0;				// 移動力
			nAppearance = 0;		// 出現する高さ
			fBasicShadowSize = 0;	// シャドウサイズ
			nMaxSprit = 0;			// 最大分割数
			nChangeTime = 0;		// 変化させる時間(変化するタイミング)
			nAppBlock = 0;			// ブロックが出現するタイミング

			// ダメージ床用
			fInitFloor = -100.0f;	// ダメージ床の初期位置
			fFloorMove = 0;			// ダメージ床の移動速度
			nFloorPhase = 0;		// ダメージ床の上がるフェーズタイミング
		}
		// ブロック用
		float					fMove;					// 移動力
		int						nAppearance;			// 出現する高さ
		float					fBasicShadowSize;		// シャドウサイズ
		int						nMaxSprit;				// 最大分割数
		int						nChangeTime;			// 変化させる時間(変化するタイミング)
		int						nAppBlock;				// ブロックが出現するタイミング
		std::vector<INTEGER2>	v_nDropBlock;			// 落とすブロックの数
		std::vector<FLOAT2>		v_fBlockGravity;		// 落ちる速度

		// ダメージ床用
		std::vector<int>		v_nDamageFloorHight;	// ダメージ床の高さ
		float					fInitFloor;				// ダメージ床の初期位置
		float					fFloorMove;				// ダメージ床の移動速度
		int						nFloorPhase;			// ダメージ床の上がるフェーズタイミング
	} BLOCK_STATUS;

	/* 関数 */
	// コンストラクタ
	CBaseblock();
	// デストラクタ
	virtual ~CBaseblock();
	// 初期化処理
	virtual void Init(void);
	// 更新処理
	virtual void Uninit(void);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);

	// 当たった後の判定
	//	Obj		: オブジェタイプ
	//	pScene	: シーン情報
	virtual void HitCollision(
		COLLISIONDIRECTION const &Direct,	// 前後左右上下
		CScene::OBJ const & Obj,			// オブジェタイプ
		CScene * pScene = NULL				// シーン情報
	) {};

	// 当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 押し出し当たり判定
	//	Obj		: オブジェタイプ
	//	pos		: 位置
	//	posOld	: 前回の位置
	//	move	: 移動量
	//	size	: サイズ
	COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * move,								// 移動量
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	);
	// 当たり判定
	//	Obj		: オブジェタイプ
	//	pos			: 位置
	//	size		: サイズ
	//	OffsetPos	: オフセット位置
	COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	);
	// ブロックの押し出し処理
	//	LineBegin	: 線の始点
	//	LineEnd		: 線の終点
	//	fDistance	: 距離
	COLLISIONDIRECTION PushBlock(
		D3DXVECTOR3 const & LineBegin,	// 	LineBegin	: 線の始点
		D3DXVECTOR3 const & LineEnd,	// 	LineEnd		: 線の終点
		float &				fDistance	//	fDistance	: 距離
	);

	// ベースブロック
	void SetType(BLOCKTYPE const Blocktype)			{ m_BlockType = Blocktype; };
	// ベースブロック
	BLOCKTYPE GetType(void) const					{ return m_BlockType; };
	// 落ちる状態設定
	void SetFall(bool const & bFall)				{ m_bFall = bFall; };
	// 落ちる状態取得
	bool & GetFall(void)							{ return m_bFall; };
	// 使用状態設定
	void SetUse(bool const & bUse)					{ m_bUse = bUse; };
	// 使用状態取得
	bool & GetUse(void)								{ return m_bUse; };
	// シャドウの使用状態状態設定
	void SetShadow(bool const & bShadow)			{ m_bShadow = bShadow; };
	// シャドウの使用状態状態取得
	bool & GetShadow(void)							{ return m_bShadow; };
	// 盤面情報取得
	GRID & GetGrid(void)							{ return m_grid; };
	// 盤面情報設定
	void SetGrid(GRID const &grid)					{ m_grid = grid; };
	// 押し出し後情報取得
	PUSHAFTER & GetPushAfter(void)					{ return m_PushAfeter; };
	// 押し出し後情報設定
	void SetPushAfter(PUSHAFTER const &PushAfter);
	// 前回の位置取得
	D3DXVECTOR3 & GetPosOld(void)					{ return m_posOld; };

	// ベースブロックの種類設定
	void SetGravity(float fGravity)					{ m_fGravity = fGravity; };
	// ベースブロックの種類取得
	float GetGravity()								{ return m_fGravity; };
	// 指定したベースブロックを削除する処理
	//	pBlock	: ブロック情報
	static bool DeleteBlock(
		CBaseblock * pBlock	// ブロック情報
	);
	// ベースブロック全ソースの読み込み
	static HRESULT Load(void);
	// ベースブロック全ソースの開放
	static void UnLoad(void);
	// 作成(シーン管理)
	//	pos			: 位置
	//	nModelId	: モデル番号
	//	layer		: レイヤー
	static CBaseblock * Create(
		D3DXVECTOR3		const & pos,									// 位置
		int				const & nModelId,								// モデル番号
		CScene::LAYER	const & layer = CScene::LAYER_3DBLOCK				// レイヤー
	);
	// 作成(個人管理)
	//	pos			: 位置
	//	nModelId	: モデル番号
	static CBaseblock * Create_Self(
		D3DXVECTOR3		const & pos,									// 位置
		int				const & nModelId								// モデル番号
	);
	// unique_ptr作成(個人管理unique)
	// ※戻り値はstd::moveで受け取る
	//	pos			: 位置
	//	nModelId	: モデル番号
	static std::unique_ptr<CBaseblock> Creat_Unique(
		D3DXVECTOR3		const & pos,									// 位置
		int				const & nModelId								// モデル番号
	);
	// タイトル用 ブロックを一斉に生成
	static void CreateInBulkBlock();
	// 行列高さが一致するブロックを取得する関数
	static CBaseblock * GetBaseBlock(GRID & Grid);
	// 指定した行列の全ての落ちる状態をfalseに関数
	static void FallBlock_Grid(GRID & Grid);
	// サイズ範囲設定
	static void SetSizeRange(float const &fSizeRange) { m_fSizeRange = fSizeRange; };
	// サイズ範囲取得
	static float & GetSizeRange(void) { return m_fSizeRange; };
	// フェードの値設定
	//	nFeedValue	: フェードの値
	static void SetFeedValue(
		int const & nFeedValud	// フェードの値
	) { m_nFeedValue.emplace_back(nFeedValud); };
	// フェードの値取得
	//	nStage	: ステージ番号
	static int GetFeedValue(
		int const & nStage	// フェードの値
	);
	// ブロックの位置計算
	//static D3DXVECTOR3 CaluBlockPos(GRID const & Grid)
	//{ return D3DXVECTOR3() }

	/* 試験用 */
	// 現在積み重なっているブロックの高さを取得
	//	nColumn	: 列
	//	nLine	: 行
	static int GetHeight(
		int const & nColumn,
		int const & nLine
	);
	// 現在積み重なっているブロックの高さを設定
	//	nColumn	: 列
	//	nLine	: 行
	//	nHeight	: 高さ
	static void SetHeight(
		int const & nColumn,
		int const & nLine,
		int const & nHeight
	);
	// 現在積み重なっているブロックの高さを設定
	//	Grid	: 列
	static void SetHeight(
		GRID const & Grid
	);
	// ブロックのステータス情報取得
	static BLOCK_STATUS const & GetBlockStatus(void) { return m_BlockStatus; };
	// ブロックのステータス読み込み処理
	static void BlockStatusLoad(void);
	// ブロックのステータス書き込み処理
	static void BlockStatusSave(void);
	// ブロックの静的変数を初期化する
	static void BlockStaticValue(void);
	// ブロックの最大高さを取得
	static int GetMaxHeight(void) { return m_nMaxHeight; };
	// ブロックの最大高さを再設定
	static void SetMaxHeight(void);
	// フェーズの取得
	static int GetPhase(void)					{ return m_nPhase; };
	// フェーズの設定
	static void SetPhase(int const & nPhase)	{ m_nPhase = nPhase; };
#if IMGUI_DEBUG

	// 全体のデバッグ処理
	static void AllDebug(void);

#endif // ERROW_ACTION

#ifdef _DEBUG
	// デバッグ処理
	virtual void  Debug(void);
#endif // _DEBUG
#if BASEBLOCK_DEBUG
	static void NumAllDebug(void);				// 全体個数を表示するデバッグ処理
#endif // BASEBLOCK_DEBUG
	/* 変数宣言 */


protected:
	/* 関数 */

	// 設定 //
	/* 変数宣言 */
	static float			m_fSizeRange;	// サイズ範囲
	static std::vector<int>	m_nFeedValue;	// フェードの値
private:
	/* 関数 */
	// 落ちる更新処理
	void Update_Fall(void);
	// 押し出し状態のブロックの更新処理
	void Update_PushState(void);
	// 自身のシャドウの出現条件処理
	void Update_MyShadow(void);
	// 上限処理
	void Update_Limit(void);
	// 当たり判定処理
	void Collision(CBaseblock * pBlock);
	// 自信と他のブロックの比較し、シャドウを更新させる処理
	void Update_OtherShadow(CBaseblock * pBlock);
	/* 変数 */
	static BLOCK_STATUS		m_BlockStatus;		// ブロックのステータス
	CCircleshadow *			m_pShadowPolygon;	// シャドウポリゴン
	D3DXVECTOR3				m_posOld;			// 前回の位置
	BLOCKTYPE				m_BlockType;		// ベースブロック
	GRID					m_grid;				// 盤面情報
	PUSHAFTER				m_PushAfeter;		// 押し出した後要の変数
	float					m_fGravity;			// 重力
	bool					m_bFall;			// 落ちる状態
	bool					m_bShadow;			// シャドウの使用状態
	bool					m_bUse;				// 表示状態
	static int				m_nPhase;			// フェーズ
	// 試験用
	static int				m_anHeight[BASEBLOCK_FIELDMAX][BASEBLOCK_FIELDMAX];	// それぞれの行列の高さ
	static int				m_nMaxHeight;		// 最大高さ
#if BASEBLOCK_DEBUG
	static int				m_nAll;				// 全体個数
#endif // BASEBLOCK_DEBUG

};

#endif