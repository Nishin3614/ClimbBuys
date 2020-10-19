// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 結合ブロック処理 [connectblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CONNECTBLOCK_H_
#define _CONNECTBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseblock;	// ベースブロッククラス

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CConnectblock : public CScene
{
public:
	/* 列挙型 */
	// 形情報
	typedef enum
	{
		SHAPE_RECT = 0,
		SHAPE_MAX
	} SHAPE;
	// フィールドタイプ情報
	typedef enum
	{
		FIELDTYPE_RECT = 0,
		FIELDTYPE_MAX
	} FIELDTYPE;
	/* 構造体 */

	/* 関数 */
	// コンストラクタ
	CConnectblock();
	// デストラクタ
	virtual ~CConnectblock();
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
	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return &m_move; };
	// 結合ブロック全ソースの読み込み
	static HRESULT Load(void);
	// 結合ブロック全ソースの開放
	static void UnLoad(void);

	/* おちてくるブロックの生成 */
	// 作成(シーン管理)
	//	pos			: 位置
	//	shape		: 形情報
	//	layer		: レイヤー
	static CConnectblock * Create(
		D3DXVECTOR3		const & pos,									// 位置
		SHAPE			const & shape,									// 形情報
		CScene::LAYER	const & layer = CScene::LAYER_3DOBJECT			// レイヤー
	);
	// 作成(個人管理)
	//	pos			: 位置
	//	shape		: 形情報
	static CConnectblock * Create_Self(
		D3DXVECTOR3		const & pos,									// 位置
		SHAPE			const & shape									// 形情報
		);
	// unique_ptr作成(個人管理unique)
	// ※戻り値はstd::moveで受け取る
	//	pos			: 位置
	//	shape		: 形情報
	static std::unique_ptr<CConnectblock> Creat_Unique(
		D3DXVECTOR3		const & pos,									// 位置
		SHAPE			const & shape									// 形情報
	);

	/* フィールド用のブロック生成 */
	// 作成(シーン管理)
	//	layer		: レイヤー
	static CConnectblock * Create_Field(
		FIELDTYPE		const & fieldtype = FIELDTYPE_RECT,				// フィールドタイプ
		CScene::LAYER	const & layer = CScene::LAYER_3DOBJECT			// レイヤー
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
	std::vector<CBaseblock *>				m_vec_pBaseBlock;		// ベースブロック情報
	D3DXVECTOR3								m_pos;					// 位置
	D3DXVECTOR3								m_move;					// 移動
	D3DXCOLOR								m_col;				// まとまりの色情報
};

#endif