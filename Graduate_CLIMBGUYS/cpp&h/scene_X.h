// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Xシーン処理の説明[scene_X.h]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _SCENE_X_H_
#define _SCENE_X_H_	 // ファイル名を基準を決める

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model_info.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCollision;		// 当たり判定クラス
class CScene_THREE;		// シーン3Dクラス
class CStencilshadow;	// ステンシルシャドウクラス

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_X : public CScene
{
public:
	/* 構造体 */
	// モデル情報
	typedef struct MODEL_LOAD
	{
		// コンストラクタ
		MODEL_LOAD()
		{
			pMesh = NULL;
			pBuffMat = NULL;
			nNumMat = 0;
			vtxMinMaterials = D3DVECTOR3_ZERO;
			vtxMaxMaterials = D3DVECTOR3_ZERO;
			size = D3DVECTOR3_ZERO;
		}
		LPD3DXMESH			pMesh;						// メッシュ情報へのポインタ
		LPD3DXBUFFER		pBuffMat;					// マテリアル情報へのポインタ
		std::vector<LPDIRECT3DTEXTURE9> vec_pTexture;	// テクスチャー情報
		DWORD				nNumMat;					// マテリアルの数
		D3DXVECTOR3			vtxMinMaterials;			// モデル情報の位置の最小値
		D3DXVECTOR3			vtxMaxMaterials;			// モデル情報の位置の最大値
		D3DXVECTOR3			size;						// サイズ
	} MODEL_LOAD;

	/* 関数 */
	// コンストラクタ
	CScene_X();
	// デストラクタ
	~CScene_X();
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
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};

	// あたり判定を強制的に削除
	void CollisionDelete(void);
	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	// 作成処理(シーン管理)
	//	pos			: 位置
	//	rot			: 回転
	//	nModelId	: モデル番号
	//	bShadowMap	: シャドウマッピング状態
	static CScene_X * Create(
		D3DXVECTOR3 const &pos,						// 位置
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,	// 回転
		int const &nModelId = 0,					// モデル番号
		bool const &bShadowMap = false				// シャドウマッピング状態
	);
	// 作成処理(個人管理)
	//	pos			: 位置
	//	rot			: 回転
	//	nModelId	: モデル番号
	//	bShadowMap	: シャドウマッピング状態
	static CScene_X * Create_Self(
		D3DXVECTOR3 const &pos,						// 位置
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,	// 回転
		int const &nModelId = 0,					// モデル番号
		bool const &bShadowMap = false				// シャドウマッピング状態
	);
	// 作成処理(個人管理)
	//	pos			: 位置
	//	rot			: 回転
	//	nModelId	: モデル番号
	//	bShadowMap	: シャドウマッピング状態
	static std::unique_ptr<CScene_X> Create_Uni(
		D3DXVECTOR3 const &pos,						// 位置
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,	// 回転
		int const &nModelId = 0,					// モデル番号
		bool const &bShadowMap = false				// シャドウマッピング状態
	);

	// モデル情報の読み込み
	static HRESULT LoadModel(void);
	// モデル情報の開放
	static HRESULT UnLoadModel(void);
	// 設定
	// 位置設定
	void SetPos(D3DXVECTOR3 const &pos)			{ m_pos = pos; };
	// 回転設定
	void SetRot(D3DXVECTOR3 const &rot)			{ m_rot = rot; };
	// サイズ設定
	void SetSize(D3DXVECTOR3 const &size)		{ m_size = size; };
	// モデル番号設定
	void SetModelId(int const &nModelId)		{ m_nModelId = nModelId; };
	// シャドウマッピング設定
	void SetShadowMap(bool const &bShadow)		{ m_bShadowMap = bShadow; };
	// 親の行列設定
	void SetParentMtx(D3DXMATRIX * ParentMax)	{ m_pParentMtx = ParentMax; };
	// モデルのα値設定
	void SetModelAlpha(float fModelAlpha)		{ m_fModelAlpha = fModelAlpha; };
	// ステンシルシャドウ設定
	//	Size	: サイズ
	//	Type	: タイプ
	void SetStencilshadow(
		D3DXVECTOR3 const & Size,			// サイズ
		int const & nType	// タイプ
	);
	// 当たり判定設定
	//	nShapeType	: 0:矩形、1:球、2:円柱
	//	Obj			: オブジェクトタイプ
	//	bPush		: 押し出し処理
	//	pParent		: 親情報
	//	Offset_pos	: オフセット位置
	void SetCollision(
		int const &nShapeType,
		int const &Obj,
		bool const &bPush = false,
		bool const &bOpponent = false,
		CScene * pParent = NULL,
		D3DXVECTOR3 const &Offset_pos = D3DVECTOR3_ZERO
	);									// 当たり判定設定
	// モデルカラー情報の設定
	//	col	: カラー
	void SetModelColor(
		D3DXCOLOR const &Col	// カラー
	);
	// 取得
	// モデル情報取得
	CScene_X::MODEL_LOAD * GetModel(int const & nModelId);
	// 位置取得
	D3DXVECTOR3 &GetPos(void) { return m_pos; };
	// 回転取得
	D3DXVECTOR3 &GetRot(void) { return m_rot; };
	// サイズ取得
	D3DXVECTOR3 &GetSize(void) { return m_size; };
	// 行列取得
	D3DXMATRIX &GetMatrix(void) { return m_mtxWorld; };
	// 当たり判定状態取得
	bool const GetbCollision(void);
	// 当たり判定の情報を取得
	CCollision * GetCollision(void);
	// モデル情報の取得
	static MODEL_LOAD * GetModelLoad(int const &nModelId);
protected:
private:
	/* 関数 */
	// モデル設定
	static void ModelSetting(MODEL_LOAD * pModel_load);
	/* 変数 */
	static std::vector<std::unique_ptr<MODEL_LOAD>> m_pModelLoad;		// モデル情報の読み込み用
	D3DXVECTOR3										m_pos;				// 位置情報
	D3DXVECTOR3										m_rot;				// 回転情報
	D3DXVECTOR3										m_size;				// サイズ情報
	D3DXMATRIX										*m_pParentMtx;		// 親マトリックス
	D3DXMATRIX										m_mtxWorld;			// ワールドマトリックス
	int												m_nModelId;			// モデル番号
	bool											m_bShadowMap;		// シャドウマッピングにするかしないか
	float											m_fShadowAlpha;		// 影のα値
	float											m_fModelAlpha;		// モデルのアルファ値
	D3DXCOLOR										* m_pModelCol;		// モデルカラー情報
	CScene_THREE									* m_pShadow;		// まる影
	CCollision										* m_Collision;		// 当たり判定
	CStencilshadow									* m_pStencilshadow;	// ステンシルシャドウ
};

#endif