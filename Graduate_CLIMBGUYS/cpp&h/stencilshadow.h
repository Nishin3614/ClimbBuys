// ----------------------------------------
//
// ステンシルシャドウ処理の説明[stencilshadow.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------


// ----------------------------------------
//
// 前方宣言
//
// ----------------------------------------
class CScene_TWO;	// シーン2Dクラス

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CStencilshadow : public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_CYLINDER = 0,	// 円柱
		TYPE_RECT,			// 矩形
		TYPE_MAX			// 最大数
	} TYPE;
	/* 関数 */
	// コンストラクタ
	CStencilshadow();
	// デストラクタ
	~CStencilshadow();
	// 初期化処理
	void Init(void);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
#ifdef _DEBUG
	// デバッグ処理
	void Debug(void);
	// ステンシル描画状態の設定
	static void SetStencil(bool const &bStencil) { m_bStencil = bStencil; };
	// ステンシル描画状態の取得
	static bool & GetStencil(void) { return m_bStencil; };
#endif // _DEBUG
	// 当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
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
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	static HRESULT Load(void);
	static void UnLoad(void);
	// 作成処理(シーン管理)
	static CStencilshadow * Create(
		D3DXVECTOR3 const &pos,							// 位置
		D3DXVECTOR3 const &size,						// サイズ
		TYPE const &type = TYPE_CYLINDER,				// タイプ
		int const &nWidth = 10,							// 横数
		int const &nDepth = 1,							// 縦数
		D3DXCOLOR	const &col = D3DXCOLOR_INI,			// カラー
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO		// 回転
	);
	// 作成処理(個人管理)
	static CStencilshadow * Create_Self(
		D3DXVECTOR3 const &pos,							// 位置
		D3DXVECTOR3 const &size,						// サイズ
		TYPE const &type = TYPE_CYLINDER,				// タイプ
		int const &nWidth = 10,							// 横数
		int const &nDepth = 5,							// 縦数
		D3DXCOLOR	const &col = D3DXCOLOR_INI,			// カラー
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO		// 回転
	);
	// 取得
	// 使用状態
	bool GetUse(void);
	// 設定
	// 位置設定
	void SetPos(D3DXVECTOR3 const &pos);
	// 回転設定
	void SetRot(D3DXVECTOR3 const &rot);
	// 色設定
	void SetCol(D3DXCOLOR const &col);
	// 仕様設定
	void SetUse(bool const bUse);
protected:

private:
	/* 関数 */
	// 円柱の設定
	void SetCylinder(LPDIRECT3DDEVICE9 pDevice);
	// 矩形の設定
	void SetRect(LPDIRECT3DDEVICE9 pDevice);
	/* 変数 */
	static int CStencilshadow::m_nTexId[TYPE_MAX];	// テクスチャーID
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndex;				// インデックスバッファのポインタ
	D3DXVECTOR3 m_OriginBlock;						// 初期配置
	D3DXMATRIX  m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_size;								// サイズ
	D3DXVECTOR3 m_rot;								// 角度
	D3DXCOLOR	m_col;								// 色
	int		m_nNumberVertex;						// 総頂点数
	int		m_nNumIndex;							// 総インデックス
	int		m_nNumPolygon;							// 総ポリゴン
	int		m_nBlock_Depth;							// 縦ブロック数
	int		m_nBlock_Width;							// 横ブロック数
	bool	m_bUse;									// 使用
	TYPE	m_type;									// タイプ
	std::unique_ptr<CScene_TWO> m_pSceneTwo;		// 2Dポリゴン
#ifdef _DEBUG
	static bool	m_bStencil;							// ステンシル描画するかしないか
#endif // _DEBUG
};

#endif