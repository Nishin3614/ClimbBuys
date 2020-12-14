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
	/* 列挙型 */
	// モデルタイプ
	typedef enum
	{
		TYPE_NONE = -1,					// NONE
		TYPE_CASTLE,					// 城？
		TYPE_PLAYER,					// プレイヤー？
		TYPE_BLOCK,						// ブロック
		TYPE_STAND,						// 足場
		TYPE_PLAYER00_ASTOLFO,			// プレイヤー00 アストルフォ
		TYPE_PLAYER01_TANJIRO,			// プレイヤー01 たんじろう
		TYPE_PLAYER00_WAIST,			// プレイヤー00 腰
		TYPE_PLAYER00_BODY,				// プレイヤー00 胴体
		TYPE_PLAYER00_HEAD,				// プレイヤー00 頭
		TYPE_PLAYER00_HAND_RIGHT,		// プレイヤー00 右手
		TYPE_PLAYER00_HAND_LEFT,		// プレイヤー00 左手
		TYPE_PLAYER00_THIGH_RIGHT,		// プレイヤー00 右腿
		TYPE_PLAYER00_LEG_RIGHT,		// プレイヤー00 右足
		TYPE_PLAYER00_THIGH_LEFT,		// プレイヤー00 左腿
		TYPE_PLAYER00_LEG_LEFT,			// プレイヤー00 左足
		TYPE_PLAYER01_WAIST,			// プレイヤー01 腰
		TYPE_PLAYER01_BODY,				// プレイヤー01 胴体
		TYPE_PLAYER01_HEAD,				// プレイヤー01 頭
		TYPE_PLAYER01_HAND_RIGHT,		// プレイヤー01 右手
		TYPE_PLAYER01_HAND_LEFT,		// プレイヤー01 左手
		TYPE_PLAYER01_THIGH_RIGHT,		// プレイヤー01 右腿
		TYPE_PLAYER01_LEG_RIGHT,		// プレイヤー01 右足
		TYPE_PLAYER01_THIGH_LEFT,		// プレイヤー01 左腿
		TYPE_PLAYER01_LEG_LEFT,			// プレイヤー01 左足
		TYPE_PLAYER02_WAIST,			// プレイヤー02 腰
		TYPE_PLAYER02_BODY,				// プレイヤー02 胴体
		TYPE_PLAYER02_HEAD,				// プレイヤー02 頭
		TYPE_PLAYER02_HAND_RIGHT,		// プレイヤー02 右手
		TYPE_PLAYER02_HAND_LEFT,		// プレイヤー02 左手
		TYPE_PLAYER02_THIGH_RIGHT,		// プレイヤー02 右腿
		TYPE_PLAYER02_LEG_RIGHT,		// プレイヤー02 右足
		TYPE_PLAYER02_THIGH_LEFT,		// プレイヤー02 左腿
		TYPE_PLAYER02_LEG_LEFT,			// プレイヤー02 左足
		TYPE_PLAYER03_WAIST,			// プレイヤー03 腰
		TYPE_PLAYER03_BODY,				// プレイヤー03 胴体
		TYPE_PLAYER03_HEAD,				// プレイヤー03 頭
		TYPE_PLAYER03_HAND_RIGHT,		// プレイヤー03 右手
		TYPE_PLAYER03_HAND_LEFT,		// プレイヤー03 左手
		TYPE_PLAYER03_THIGH_RIGHT,		// プレイヤー03 右腿
		TYPE_PLAYER03_LEG_RIGHT,		// プレイヤー03 右足
		TYPE_PLAYER03_THIGH_LEFT,		// プレイヤー03 左腿
		TYPE_PLAYER03_LEG_LEFT,			// プレイヤー03 左足
		TYPE_TUTORIALBLOCK,				// チュートリアルブロック
		TYPE_OBJECT_MAP,				// マップ
		TYPE_OBJECT_BLOCK_C,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_L,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_I,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_M,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_B,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_G,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_U,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_Y,			// 当たり判定無し演出用ブロック
		TYPE_OBJECT_BLOCK_S,			// 当たり判定無し演出用ブロック
		TYPE_BLOCK_SPRING,				// 特殊ブロック_バネ
		TYPE_BLOCK_BOMB,				// 特殊ブロック_ボム
		TYPE_OBJECT_VOLCANO,			// 火山
		TYPE_BLOCKS_INVERTER,			// 電気ブロック
		TYPE_BLOCKS_HARD,				// 硬いブロック
		TYPE_BLOCKS_HATENA,				// ハテナブロック
		TYPE_MAX						// 最大数
	} TYPE;

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
		std::vector<D3DCOLORVALUE>		vec_Diffuse;	// 元のカラー情報
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

	// 作成処理(シーン管理)
	//	pos			: 位置
	//	rot			: 回転
	//	size		: サイズ倍率 1.0が標準
	//	nModelId	: モデル番号
	//	bShadowMap	: シャドウマッピング状態
	static CScene_X * Create(
		D3DXVECTOR3 const &pos,						// 位置
		D3DXVECTOR3 const &rot,						// 回転
		D3DXVECTOR3 const &size,					// サイズ倍率
		D3DXCOLOR color,							// カラー
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

	// 押し出し当たり判定
	//	Obj		: オブジェタイプ
	//	pos		: 位置
	//	posOld	: 前回の位置
	//	move	: 移動量
	//	size	: サイズ
	virtual COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * move,								// 移動量
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	) {
		return COLLISIONDIRECTION::NONE;
	};
	// 当たり判定
	//	Obj		: オブジェタイプ
	//	pos			: 位置
	//	size		: サイズ
	//	OffsetPos	: オフセット位置
	virtual COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	) {
		return COLLISIONDIRECTION::NONE;
	};

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
	// ステンシルシャドウの使用状態設定
	void SetUseStencillShadow(bool const & bUse);
	// ステンシルシャドウの使用状態を取得
	bool GetUseStencillShadow(void);
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
	CScene_X::MODEL_LOAD * GetModel(void);
	// 位置取得
	D3DXVECTOR3 &GetPos(void) { return m_pos; };
	// 回転取得
	D3DXVECTOR3 &GetRot(void) { return m_rot; };
	// サイズ取得
	D3DXVECTOR3 &GetSize(void) { return m_size; };
	// 行列取得
	D3DXMATRIX &GetMatrix(void) { return m_mtxWorld; };
	// ステンシルシャドウ情報取得
	CStencilshadow * GetStencillShadow(void) { return m_pStencilshadow; };
	// 当たり判定状態取得
	bool const GetbCollision(void);
	// モデル情報の取得
	static MODEL_LOAD * GetModelLoad(int const &nModelId);
	// モデルidの取得
	int GetModelId() { return m_nModelId; };
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
	CStencilshadow									* m_pStencilshadow;	// ステンシルシャドウ
};

#endif