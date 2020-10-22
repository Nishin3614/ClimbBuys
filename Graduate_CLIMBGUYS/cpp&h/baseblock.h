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
#define BASEBLOCK_RANGE			(50.0f)				// ブロックの範囲
#define BASEBLOCK_XYZTOPOS(X)	(BASEBLOCK_RANGE * X)	// 行列高さからの位置
#define BASEBLOCK_MINUSTOPLUS	(3)						// 行列をプラスに

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
		GRID operator + (CONST GRID& Source) const
		{
			// 値を渡す変数
			GRID variable;
			variable.nColumn = nColumn + Source.nColumn;
			variable.nLine = nLine + Source.nLine;
			variable.nHeight = nHeight + Source.nHeight;
			return variable;
		}
		// 行列高さの掛け算のオペレーション
		GRID operator * (CONST int& Source) const
		{
			// 値を渡す変数
			GRID variable;
			variable.nColumn = nColumn + Source;
			variable.nLine = nLine + Source;
			variable.nHeight = nHeight + Source;
			return variable;
		}
		// 行列高さから位置を定めたオペレーション
		operator D3DXVECTOR3 ()
		{
			D3DXVECTOR3 pos;
			pos.x = nColumn * BASEBLOCK_RANGE;
			pos.z = nLine * BASEBLOCK_RANGE;
			pos.y = nHeight * BASEBLOCK_RANGE;
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
		CScene::LAYER	const & layer = CScene::LAYER_UI				// レイヤー
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
	) { return m_anHeight[nColumn][nLine]; };
	// 現在積み重なっているブロックの高さを設定
	//	nColumn	: 列
	//	nLine	: 行
	//	nHeight	: 高さ
	static void SetHeight(
		int const & nColumn,
		int const & nLine,
		int const & nHeight
	) { m_anHeight[nColumn][nLine] = nHeight; };
	// 現在積み重なっているブロックの高さを設定
	//	Grid	: 列
	static void SetHeight(
		GRID const & Grid
	) {
		m_anHeight[Grid.nColumn][Grid.nLine] = Grid.nHeight;
	};
#ifdef _DEBUG
	// デバッグ処理
	virtual void  Debug(void);
#endif // _DEBUG
protected:
	/* 関数 */
	//D3DXVECTOR3 & GetGridToGrid(GRID const & Grid) {}
	// 設定 //
private:
	/* 関数 */
	// 落ちる更新処理
	void Update_Fall(void);
	/* 変数 */
	TYPE	m_type;		// ベースブロック
	GRID	m_grid;		// 盤面情報
	bool	m_bFall;	// 落ちる状態

	// 試験用
	static int m_anHeight[10][10];
};

#endif