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
class CCollision;
class CExtrusion;
class CScene_THREE;

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
		LPD3DXMESH			pMesh;					// メッシュ情報へのポインタ
		LPD3DXBUFFER		pBuffMat;				// マテリアル情報へのポインタ
		std::vector<LPDIRECT3DTEXTURE9> vec_pTexture;	// テクスチャー情報
		DWORD				nNumMat;				// マテリアルの数
		D3DXVECTOR3			vtxMinMaterials;		// モデル情報の位置の最小値
		D3DXVECTOR3			vtxMaxMaterials;		// モデル情報の位置の最大値
		D3DXVECTOR3			size;					// サイズ
	} MODEL_LOAD;
	// 列挙体定義
	typedef enum
	{
		LOADTYPE_ADD = 0,
		LOADTYPE_POS_X,
		LOADTYPE_POS_Y,
		LOADTYPE_POS_Z,
		LOADTYPE_ROT_X,
		LOADTYPE_ROT_Y,
		LOADTYPE_ROT_Z,
		LOADTYPE_MAX
	} LOADTYPE;

	/* 関数 */
	CScene_X();
	~CScene_X();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
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
	static CScene_X * Create(
		D3DXVECTOR3 const &pos,
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,
		int const &nModelId = 0,
		bool const &bShadowMap = false
	);
	// 作成処理(個人管理)
	static CScene_X * Create_Self(
		D3DXVECTOR3 const &pos,
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,
		int const &nModelId = 0,
		bool const &bShadowMap = false
	);
	// 作成処理(個人管理)
	static std::unique_ptr<CScene_X> Create_Uni(
		D3DXVECTOR3 const &pos,
		D3DXVECTOR3 const &rot = D3DVECTOR3_ZERO,
		int const &nModelId = 0,
		bool const &bShadowMap = false
	);

	// モデル情報の読み込み
	static HRESULT LoadModel(void);
	// モデル情報の開放
	static HRESULT UnLoadModel(void);
	// 設定
	void SetPos(D3DXVECTOR3 const &pos) { m_pos = pos; };							// 位置
	void SetRot(D3DXVECTOR3 const &rot) { m_rot = rot; };							// 回転
	void SetModelId(int const &nModelId) { m_nModelId = nModelId; };				// モデル番号
	void SetShadowMap(bool const &bShadow) { m_bShadowMap = bShadow; };				// モデル番号
	void SetCollision(void);														// 当たり判定
	void SetParentMtx(D3DXMATRIX * ParentMax) { m_pParentMtx = ParentMax; };		// 親マトリックスの設定
	void SetModelAlpha(float fModelAlpha) { m_fModelAlpha = fModelAlpha; }			// マトリックスの透明度
	// 当たり判定設定
	// nShapeType:0:矩形、1:球、2:円柱
	// obj:オブジェクトタイプ
	// bPush:押し出し処理
	// pParent:親情報
	void SetCollision(
		int const &nShapeType,
		int const &obj,
		bool const &bPush = false,
		bool const &bOpponent = false,
		CScene * pParent = NULL,
		D3DXVECTOR3 const &offset_pos = D3DVECTOR3_ZERO
	);									// 当たり判定設定
	// モデルカラー情報の設定
	//	col	: カラー
	void SetModelColor(
		D3DXCOLOR const &col	// カラー
	);
	// 取得
	CScene_X::MODEL_LOAD * GetModel(int const & nModelId);						// モデル情報取得
	D3DXVECTOR3 &GetPos(void) { return m_pos; };								// 位置
	D3DXVECTOR3 &GetRot(void) { return m_rot; };								// 回転
	D3DXMATRIX &GetMatrix(void) { return m_mtxWorld; };							// マトリックス情報
	// 当たり判定状態取得
	bool const GetbCollision(void);												// 当たり判定状態
	// 当たり判定の情報を取得
	CCollision * GetCollision(void);
	static MODEL_LOAD * GetModelLoad(int const &nModelId);						// モデル情報取得
	static void LoadScrept(char* add);
protected:
private:
	/* 関数 */
	static void ModelSetting(MODEL_LOAD * pModel_load);
	/* 変数 */
	static std::vector<std::unique_ptr<MODEL_LOAD>> m_pModelLoad;	// モデル情報の読み込み用

	D3DXVECTOR3		m_pos;								// 位置情報
	D3DXVECTOR3		m_rot;								// 回転情報
	D3DXMATRIX		*m_pParentMtx;						// 親マトリックス
	D3DXMATRIX		m_mtxWorld;							// ワールドマトリックス
	int				m_nModelId;							// モデル番号
	bool			m_bShadowMap;						// シャドウマッピングにするかしないか
	float			m_fShadowAlpha;						// 影のα値
	float			m_fModelAlpha;						// モデルのアルファ値
	D3DXCOLOR *		m_pModelCol;						// モデルカラー情報
	CScene_THREE *	m_pShadow;							// まる影
	CExtrusion		*m_pExtrusion;						// 押し出し
	CCollision		*m_Collision;						// 当たり判定
};

#endif