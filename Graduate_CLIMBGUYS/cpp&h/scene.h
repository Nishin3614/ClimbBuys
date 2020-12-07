// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// シーン処理の説明[scene.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "manager.h"
#include "texture_manager.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene
{
public:
	/* 列挙型 */
	// 物の配置順番
	typedef enum
	{
		LAYER_WORLD = 0,
		LAYER_BG,
		LAYER_3DOBJECT,
		LAYER_3DSTAND,
		LAYER_3DBLOCK,
		LAYER_CHARACTER,
		LAYER_3DBLOCKSHADOW,
		LAYER_3DCHARACTERSHADOW,
		LAYER_FLOOR,
		LAYER_DEBUGCOLLISION,
		LAYER_3DPARTICLE,
		LAYER_3DOBJECT2,
		LAYER_CHARARANK,
		LAYER_DOME,
		LAYER_BGDOME,
		LAYER_PLAYER_UI,
		LAYER_UI,
		LAYER_COLLISION,
		LAYER_PARTICLE,
		LAYER_FADE,
		LAYER_RANK,
		LAYER_MAX
	} LAYER;
	// オブジェクトタイプ
	typedef enum
	{
		OBJ_NONE = 0,	// オブジェクトタイプが設定されていない
		OBJ_PLAYER,		// プレイヤー
		OBJ_BLOCK,		// ブロック
		OBJ_STAND,		// 足場
		OBJ_MAX			// 最大数
	} OBJ;

	/* 関数 */
	CScene();
	virtual ~CScene();
	virtual void Init(void) = 0;										// 初期化
	virtual void Uninit(void) = 0;										// 終了
	virtual void Update(void) = 0;										// 更新
	virtual void Draw(void) = 0;										// 描画
#ifdef _DEBUG
	virtual void Debug(void);											// デバッグ用
#endif // _DEBUG
	// 自分から当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
		) = 0;
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) = 0;
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) = 0;
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) = 0;

	// ポインター位置情報を取得
	virtual D3DXVECTOR3 * Scene_GetPPos(void) = 0;
	// ポインター過去の位置情報を取得
	virtual D3DXVECTOR3 * Scene_GetPPosold(void) = 0;
	// ポインター移動量情報の取得
	virtual D3DXVECTOR3 * Scene_GetPMove(void) = 0;
	static void ReleaseAll(void);										// シーン親子リリース
	static void UpdateAll(void);										// シーン親子更新
	static void DrawAll(void);											// シーン親子描画
	// 取得
	// シーン情報取得(レイヤー・クラス指定)
	static CScene * GetScene(
		LAYER const &layer,												// レイヤー
		int const &nCntScene,											// カウントシーン
		CScene &														// クラス型情報
	);
	static CScene * GetScene(
		LAYER const & layer,
		int const & nCntScene,
		char const * cName										// クラス型情報
	);
	// シーン情報取得(レイヤー指定)
	static CScene * GetScene(
		LAYER const &layer,												// レイヤー
		int const &nCntScene											// カウントシーン
	);
	// レイヤー内のシーン最大数取得
	static int GetMaxLayer(LAYER const &layer)
	{ return (signed)m_pScene[layer].size(); };
	static bool GetUpdateStop(void) { return m_sta_bStop; };			// 静止状態
	// 設定
	// シーンの静止
	static void UpdateStop(
		bool const &bStop,												// 静止するかしないか
		int const &nMaxStop = 0											// 最大静止時間
	);	// 更新をとめる
	// 1Fだけ更新する
	static void Set1FUpdate() { m_b1FUpdateFlag = true; };
	// シーン管理設定
	void ManageSetting(LAYER const &layer);
	// オブジェクトの更新ストップ状態設定
	void SetStop(bool const &bStopState)
	{
		m_bStop = bStopState;
	};
	// オブジェクトの更新ストップ状態取得
	bool GetStop(void)
	{
		return m_bStop;
	};
	// オブジェクトの描画状態設定
	void SetDrawState(bool const &bDraw)
	{
		m_bDraw = bDraw;
	};
	// オブジェクトの描画状態取得
	bool GetDrawState(void)
	{
		return m_bDraw;
	};
	// オブジェクトタイプの設定
	void SetObj(OBJ const & Obj) { m_Obj = Obj; };
	// オブジェクトタイプの取得
	OBJ & GetObj(void) { return m_Obj; };
	// オブジェクトの開放準備
	void Release(void);													// オブジェクトの破棄
protected:

private:
	/* 関数 */
	static void DeadFragAll(void);						// 死亡フラグが立ったものをリリース
	/* 変数 */
	static std::vector<CScene*> m_pScene[LAYER_MAX];	// シーン管理用変数
	static bool					m_sta_bStop;			// 更新を止める
	static bool					m_b1FUpdateFlag;		// 画面停止中に1Fだけ更新するフラグ
	static int					m_nMaxStop;				// 最大静止時間
	static int					m_nCntStop;				// 静止時間
	bool						m_bDeadFrag;			// フラグ
	bool						m_bStop;				// 更新を止めるか止めないか
	bool						m_bDraw;				// 描画状態
	OBJ							m_Obj;					// オブジェクトタイプ
};

#endif