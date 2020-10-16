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
		GRID() {}
		GRID(int const &Line, int const &Column, int const &Height)
		{
			nLine = Line;
			nColumn = Column;
			nHeight = Height;
		}
		int nLine;		// 行
		int nColumn;	// 列
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
	// 落ちる状態設定
	void SetFall(bool const & bFall) { m_bFall = bFall; };
	// 落ちる状態取得
	bool & GetFall(void) { return m_bFall; };
#ifdef _DEBUG
	// デバッグ処理
	virtual void  Debug(void);
#endif // _DEBUG
protected:
	/* 関数 */
	// 設定 //
private:
	/* 関数 */
	// 落ちる更新処理
	void Update_Fall(void);
	/* 変数 */
	TYPE	m_type;		// ベースブロック
	GRID	m_grid;		// 盤面情報
	bool	m_bFall;	// 落ちる状態
};

#endif