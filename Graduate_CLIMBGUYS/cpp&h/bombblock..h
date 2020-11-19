// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 爆発ブロック処理 [bombblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _BOMBBLOCK_H_
#define _BOMBBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBombblock : public CBaseblock
{
public:
	/* 列挙型 */

	/* 構造体 */

	/* 関数 */
	// コンストラクタ
	CBombblock();
	// デストラクタ
	virtual ~CBombblock();
	// 初期化処理
	virtual void Init(void);
	// 更新処理
	virtual void Uninit(void);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);

	// 当たった後の処理
	//	nObjType	: 相手オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられた後の処理
	//	nObjType	: 相手オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 自分から当たらなかった後の処理
	//	nObjType	: 相手オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: 相手オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};

	// 当たった後の判定
	//	Obj		: オブジェタイプ
	//	pScene	: シーン情報
	void HitCollision(
		COLLISIONDIRECTION const &Direct,	// 前後左右上下
		CScene::OBJ const & Obj,			// オブジェタイプ
		CScene * pScene = NULL				// シーン情報
	);
	// 爆発するかしないか状態の設定
	void SetbBomb(bool const & bBomb)	{ m_bBomb = bBomb; };
	// 爆発するかしないか状態の取得
	bool GetbBomb(void)					{ return m_bBomb; };
	// 爆発ブロック全ソースの読み込み
	static HRESULT Load(void);
	// 爆発ブロック全ソースの開放
	static void UnLoad(void);
	// 作成(シーン管理)
	//	pos			: 位置
	//	nModelId	: モデル番号
	//	pCol		: 色情報
	//	layer		: レイヤー
	static CBombblock * Create(
		int				const & nModelId,							// モデル番号
		GRID			const & Grid,								// 行列高さ番号
		D3DXCOLOR		* pCol,										// 色情報
		CScene::LAYER	const & layer = CScene::LAYER_3DBLOCK		// レイヤー
	);
	// 作成(個人管理)
	//	pos			: 位置
	//	nModelId	: モデル番号
	//	pCol		: 色情報
	static CBombblock * Create_Self(
		int				const & nModelId,								// モデル番号
		GRID			const & Grid,									// 行列高さ番号
		D3DXCOLOR		* pCol											// 色情報
	);
	// unique_ptr作成(個人管理unique)
	// ※戻り値はstd::moveで受け取る
	//	pos			: 位置
	//	nModelId	: モデル番号
	//	pCol		: 色情報
	static std::unique_ptr<CBombblock> Creat_Unique(
		int				const & nModelId,								// モデル番号
		GRID			const & Grid,									// 行列高さ番号
		D3DXCOLOR		* pCol											// 色情報
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
	// 周りのブロックを削除する処理
	void AroundDelete(void);
	/* 変数 */
	bool	m_bBomb;		// 爆発するかしないか状態
	bool	m_bPlayerPush;	// プレイヤーに当たったら

};

#endif