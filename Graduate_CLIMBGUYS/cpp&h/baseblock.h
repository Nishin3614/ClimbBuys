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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#define BASEBLOCK_MINUSTOPLUS	(4)						// 行列をプラスに
// やること
// 行列のフィード値を格闘

//#define BASEBLOCK_RANGE			(50.0f)				// ブロックの範囲

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
		TYPE_NORMAL = 0,	// 通常
		TYPE_FIELD,			// フィールドブロック
		TYPE_MAX,			// タイプ全体数
	} TYPE;

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

	// ベースブロック
	void SetType(TYPE const type)		{ m_type = type; };
	// ベースブロック
	TYPE GetType(void) const			{ return m_type; };
	// 落ちる状態設定
	void SetFall(bool const & bFall)	{ m_bFall = bFall; };
	// 落ちる状態取得
	bool & GetFall(void)				{ return m_bFall; };
	// 盤面情報取得
	GRID & GetGrid(void)				{ return m_grid; };
	// 盤面情報設定
	void SetGrid(GRID const &grid)		{ m_grid = grid; };
	// 前回の位置取得
	D3DXVECTOR3 & GetPosOld(void)		{ return m_posOld; };

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
#ifdef _DEBUG
	// デバッグ処理
	virtual void  Debug(void);
#endif // _DEBUG
	/* 変数宣言 */

protected:
	/* 関数 */
	//D3DXVECTOR3 & GetGridToGrid(GRID const & Grid) {}
	// 設定 //
	/* 変数宣言 */
	static float			m_fSizeRange;	// サイズ範囲
	static std::vector<int>	m_nFeedValue;	// フェードの値
private:
	/* 関数 */
	// 落ちる更新処理
	void Update_Fall(void);
	/* 変数 */
	D3DXVECTOR3		m_posOld;		// 前回の位置
	TYPE			m_type;			// ベースブロック
	GRID			m_grid;			// 盤面情報
	bool			m_bFall;		// 落ちる状態
	// 試験用
	static int m_anHeight[20][20];
};

#endif