// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 足場処理 [stand.h]
// Author : FujiwaraMasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _STAND_H_
#define _STAND_H_

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
class CStand : public CScene_X
{
public:
	/* 列挙型 */

	// ブロックの種類
	typedef enum
	{
		TYPE_BLOCK_NORMAL = 0,	// ただの足場ブロック
		TYPE_BLOCK_MAP1,		// マップ1に移動するための足場ブロック
		TYPE_BLOCK_MAP2,		// マップ2に移動するための足場ブロック
		TYPE_BLOCK_MAP3,		// マップ3に移動するための足場ブロック
		TYPE_MAX,				// タイプ全体数
	} TYPE;

	/* 構造体 */

	/* 関数 */
	// コンストラクタ
	CStand();
	// デストラクタ
	~CStand();
	// 初期化処理
	void Init(void);
	// 更新処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
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


	// ベースブロック
	void SetType(TYPE const type) { m_type = type; };
	// ベースブロック
	TYPE	GetType(void) const { return m_type; };
	// ベースブロック全ソースの読み込み
	static HRESULT Load(void);
	// ベースブロック全ソースの開放
	static void UnLoad(void);
	// 作成(シーン管理)
	//	pos			: 位置
	//	nModelId	: モデル番号
	//	layer		: レイヤー
	static CStand * Create(
		D3DXVECTOR3		const & pos,									// 位置
		int				const & nModelId,								// モデル番号
		CScene::LAYER	const & layer = CScene::LAYER_3DSTAND			// レイヤー
	);
	// 作成(個人管理)
	//	pos			: 位置
	//	nModelId	: モデル番号
	static CStand * Create_Self(
		D3DXVECTOR3		const & pos,									// 位置
		int				const & nModelId								// モデル番号
	);
	// unique_ptr作成(個人管理unique)
	// ※戻り値はstd::moveで受け取る
	//	pos			: 位置
	//	nModelId	: モデル番号
	static std::unique_ptr<CStand> Creat_Unique(
		D3DXVECTOR3		const & pos,									// 位置
		int				const & nModelId								// モデル番号
	);

	static void CreateStand_Tutorial();									// チュートリアル用足場生成

	// 落ちる状態設定
	void SetFall(bool const & bFall) { m_bFall = bFall; };
	// 落ちる状態取得
	bool & GetFall(void) { return m_bFall; };

	// ステージ決定設定
	static void SetDetermination(bool flag) { m_bDetermination = flag; };
	// ステージ決定取得
	static bool &GetDetermination(void) { return m_bDetermination; };

	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);

#ifdef _DEBUG
	// デバッグ処理
	virtual void  Debug(void);
#endif // _DEBUG
protected:
	/* 関数 */
	// 設定 //
private:
	/* 関数 */

	/* 変数 */
	static bool		m_bDetermination;	// ステージ決定までのカウント

	TYPE	m_type;		// ベースブロック
	bool	m_bFall;	// 落ちる状態
};

#endif