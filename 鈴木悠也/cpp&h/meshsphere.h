//-----------------------------------------------------------------------------
//
// メッシュスフィア処理 [meshsphere.h]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define RADIUS_SIZE		(450.0f)
#define SEPARATE		(15)

//-----------------------------------------------------------------------------
// クラス定義
//-----------------------------------------------------------------------------
class CMeshsphere :public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		TEXTYPE_NONE = 0,
		TEXTYPE_WORLD,
		TEXTYPE_STAR,
		TEXTYPE_MAX
	} TEXTYPE;
	/* 構造体 */
	typedef struct MESHSPHERE
	{
		MESHSPHERE()
		{
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
			col = D3DXCOLOR_INI;
			fRadius = 0;
			fRot = 0;
			fRot2 = 0;
			nHeightBlock = 0;
			nWidthBlock = 0;
			nMaxVtx = 0;
			nMaxIndex = 0;
			nMaxPolygon = 0;
			nMaxFrame = 0;
			nCntFrame = 0;
			bUse = true;
			Textype = TEXTYPE_WORLD;
			pVtxBuffMeshSphere = NULL;
			pIdxBuffMeshSphere = NULL;
		}
		D3DXVECTOR3		pos;								// 座標
		D3DXVECTOR3		rot;								// 回転
		D3DXCOLOR		col;								// 色
		D3DXMATRIX		mtxWorldMeshSphere;					// ワールドマトリックス
		float			fRadius;							// 半径
		float			fRot;								// 各頂点の角度
		float			fRot2;								// 各頂点の角度
		int				nHeightBlock;						// 高さの分割数
		int				nWidthBlock;						// 横の頂点の個数
		int				nMaxVtx;							// 頂点の個数
		int				nMaxIndex;							// インデックスの個数
		int				nMaxPolygon;						// ポリゴンの枚数
		int				nMaxFrame;							// 最大フレーム数
		int				nCntFrame;							// フレーム数
		bool			bUse;								// 使用状態
		TEXTYPE			Textype;							//  テクスチャータイプ
		LPDIRECT3DVERTEXBUFFER9	pVtxBuffMeshSphere;			// 頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshSphere;			// インデックスバッファへのポインタ
	}	MESHSPHERE;
	/* 関数 */
	CMeshsphere();
	~CMeshsphere();
	// 生成(シーン管理)
	static CMeshsphere *Create(
		D3DXVECTOR3 const &pos,
		float const &fRadius,
		int const &nWidthBlock = 10,
		int const &nHeightBlock = 10,
		D3DXCOLOR const &col = D3DXCOLOR_INI,
		TEXTYPE	const &textype = TEXTYPE_WORLD,
		CScene::LAYER const &layer = CScene::LAYER_WORLD,
		int const &nMaxFrame = 0
	);
	// 生成(個人管理)
	static CMeshsphere * Create_Self(
		D3DXVECTOR3 const &pos,
		float const &fRadius,
		int const &nWidthBlock = 10,
		int const &nHeightBlock = 10,
		D3DXCOLOR const &col = D3DXCOLOR_INI,
		TEXTYPE	const &textype = TEXTYPE_WORLD,
		int const &nMaxFrame = 0
	);
	// 作成処理(個人_unique管理)
	static std::unique_ptr<CMeshsphere> Create_Unique(
		D3DXVECTOR3 const &pos,
		float const &fRadius,
		int const &nWidthBlock = 10,
		int const &nHeightBlock = 10,
		D3DXCOLOR const &col = D3DXCOLOR_INI,
		TEXTYPE	const &textype = TEXTYPE_WORLD,
		int const &nMaxFrame = 0
	);
	void Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_MeshSphere.pos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	static HRESULT Load(void);	// 読み込み
	static void Unload(void);	// 破棄
	// 取得 //
	bool const &GetUse(void) { return m_MeshSphere.bUse; };
	// 設定 //
	// 位置設定
	void SetPosition(D3DXVECTOR3 const &pos);
	// 半径設定
	void SetRadius(float const &fRadius);
	// 使用状態設定
	void SetUse(bool const &bUse) { m_MeshSphere.bUse = bUse; };
	// 設定
	void Set(
		D3DXVECTOR3 const & pos,
		int const & nMaxFrame,
		bool const & bUse
	);
	// 色の設定
	void SetCol(D3DXCOLOR const &col);
	// 頂点カラーの設定
	void Set_Vtx_Col(void);

private:
	/* 関数 */
	void FrameDelete(void);	// 時間によって消える
	/* 変数 */
	static int m_nTexId[TEXTYPE_MAX];	// テクスチャーID
	MESHSPHERE	m_MeshSphere;			// 球構造体
};

#endif
