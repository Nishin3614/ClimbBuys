// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 結合フィールドブロック処理 [connect_fieldblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CONNECT_FIELDBLOCK_H_
#define _CONNECT_FIELDBLOCK_H_

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
class CConnect_fieldblock : public CScene
{
public:
	/* 列挙型 */
	// マップによるs
	// フィールドタイプ情報
	typedef enum
	{
		TYPE_RECT = 0,
		TYPE_MAX
	} TYPE;
	/* 構造体 */
	// 読み込み情報
	typedef struct
	{
		int		nFailId;		// おちる番号
		bool	bUse;			// 使用状況
	} LOAD;
	/* 関数 */
	// コンストラクタ
	CConnect_fieldblock();
	// デストラクタ
	virtual ~CConnect_fieldblock();
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// 結合フィールドブロック全ソースの読み込み
	static HRESULT Load(void);
	// 結合フィールドブロック全ソースの開放
	static void UnLoad(void);

	/* おちてくるブロックの生成 */
	// 作成(シーン管理)
	//	type		: タイプ情報
	//	layer		: レイヤー
	static CConnect_fieldblock * Create(
		TYPE			const & type,									// タイプ情報
		CScene::LAYER	const & layer = CScene::LAYER_3DOBJECT			// レイヤー
	);
	// 作成(個人管理)
	//	type		: タイプ情報
	static CConnect_fieldblock * Create_Self(
		TYPE			const & type									// タイプ情報
		);
	// unique_ptr作成(個人管理unique)
	// ※戻り値はstd::moveで受け取る
	//	type		: タイプ情報
	static std::unique_ptr<CConnect_fieldblock> Creat_Unique(
		TYPE			const & type									// タイプ情報
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
	static std::vector<std::vector<LOAD>>	m_Dvec_pFileLoad;		// ファイルの読み込み情報
	std::vector<std::vector<CBaseblock *>>	m_Dvec_pFieldBlock;		// フィールドブロック情報
	int										m_nMaxField;			// フィールドの最大数
};

#endif