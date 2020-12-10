// ----------------------------------------
//
// 2DPRESENTS処理の説明[2Dpresents.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _2DPRESENTS_H_
#define _2DPRESENTS_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_two.h"
#include "Calculation.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C2DPresents : public CScene_TWO
{
public:
	/* 列挙型 */
	// フェードタイプ
	typedef enum
	{
		FADETYPE_NONE = -1,
		FADETYPE_IN,
		FADETYPE_COOPERATION,
		FADETYPE_OUT,
		FADETYPE_END,
		FADETYPE_MAX
	} FADETYPE,*PFADETYPE;
	// フェードアウト
	typedef enum
	{
		FADE_OUT_NORMAL = 0,
		FADE_OUT_UNDER_FROM_ON,
		FADE_OUT_ON_FROM_UNDER,
		FADE_OUT_DISAPPEARANCE,
		FADE_OUT_MAX
	} FADE_OUT;
	// フェードイン
	typedef enum
	{
		FADE_IN_NORMAL = 0,
		FADE_IN_UNDER_FROM_ON,
		FADE_IN_ON_FROM_UNDER,
		FADE_IN_DISAPPEARANCE,
		FADE_IN_MAX
	} FADE_IN;
	// 協調タイプ
	typedef enum
	{
		COOPERATIONTYPE_NORMAL = 0,
		COOPERATIONTYPE_UNDER_FROM_ON,
		COOPERATIONTYPE_MAX
	} COOPERATIONTYPE;

	/* 関数 */
	C2DPresents();
	~C2DPresents();
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
	void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_OpponentCollision(
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

	/* 強調 */
	// 点滅設定
	void SetCooperation(
		D3DXCOLOR const & changecol,	// 変わる色
		int const & nTimeSwitch,		// 切り替え時間
		int const & nTimeChange			// 切り替わる時間
	);
	// 拡大率設定
	void SetCooperation(
		float const & fChangeScal,		// 拡大率
		int const & nTimeChange			// 切り替わる時間
	);
	/* フェードイン */
	// フェードイン(位置)設定
	void SetFadeIn(
		D3DXVECTOR3 const & BeginPos,	// 初期位置
		D3DXVECTOR3 const & EndPos,		// 目的位置
		int const & nMaxfram			// 最大フレーム
	);
	// フェードイン(色指定)設定
	void SetFadeIn(
		D3DXCOLOR const & BeginCol,		// 初期カラー
		D3DXCOLOR const & EndCol,		// 目標カラー
		int const & nMaxfram			// 最大フレーム
	);
	// フェードイン(アニメスタート)設定
	void SetFadeIn_StarAnim(
		int const & nMaxStartAnim		// 最大アニメスタート
	);
	// フェードイン強制終了設定
	void SetFadeIn_Compulsion(
		bool const & bCompulsion
	);
	// フェードインの使用設定
	bool GetFadeIn_Bool(void);
	// フェードインの強制終了
	void Compulsion_FadeIn(void);
	// フェードイン(位置)開始
	void Start_FadeIn(void);
	/* フェードアウト */
	// フェードアウト(位置)設定
	void SetFadeOut(
		D3DXVECTOR3 const & BeginPos,	// 初期位置
		D3DXVECTOR3 const & EndPos,		// 目標位置
		int const & nMaxfram			// 最大フレーム
	);
	// フェードアウト(色指定)設定
	void SetFadeOut(
		D3DXCOLOR const & BeginCol,		// 初期カラー
		D3DXCOLOR const & EndCol,		// 目標カラー
		int const & nMaxfram			// 最大フレーム
	);
	// フェードアウト(アニメスタート)設定
	void SetFadeOut_StarAnim(
		int const & nMaxStartAnim		// 最大アニメスタート
	);
	// フェードイン強制終了設定処理
	void SetFadeOut_Compulsion(
		bool const & bCompulsion
	);
	// フェードアウトの使用設定
	bool GetFadeOut_Bool(void);
	// フェードアウトの強制終了
	void Compulsion_FadeOut(void);
	// フェードアウトの開始設定
	void Start_FadeOut(void);
	// フェードタイプの状態取得
	FADETYPE GetFadetype(void);
	// フェードタイプの状態設定
	void SetFadetype(FADETYPE const &Fadetype) { m_Fadetype = Fadetype; };
	// フェードインの最大フレームを取得
	int const GetFadeInMaxFram(void);
	// フェードインのフレームを取得
	int const GetFadeInFram(void);
	// フェードインの最大開始アニメタイムを取得
	int const GetFadeInMaxAnimTime(void);
	// フェードインのアニメタイムを取得
	int const GetFadeInAnimTime(void);
	// フェードアウトの最大開始アニメタイムを取得
	int const GetFadeOutMaxAnimTime(void);
	// フェードアウトのアニメタイムを取得
	int const GetFadeOutAnimTime(void);

	// 作成(2Dプレゼンツ継承あり)
	//	offsettype	: タイプ
	//	pos			: 位置
	//	size		: サイズ
	//	nTextype	: テクスチャータイプ
	//	frot		: 角度
	//	col			: 色
	//	layer		: レイヤー
	static C2DPresents * Create(
		OFFSET_TYPE		const & type,									// タイプ
		D3DXVECTOR3		const & pos,									// 位置
		D3DXVECTOR2		const & size,									// サイズ
		int				const & nTexType,								// テクスチャータイプ
		float			const & frot = 0.0f,							// 角度
		D3DXCOLOR		const & col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),	// 色
		CScene::LAYER	const & layer = CScene::LAYER_UI				// レイヤー
	);
	// 作成(2Dプレゼンツ継承なし)
	//	offsettype	: タイプ
	//	pos			: 位置
	//	size		: サイズ
	//	nTextype	: テクスチャータイプ
	//	frot		: 角度
	//	col			: 色
	static C2DPresents * Create_Self(
		OFFSET_TYPE	const & type,										// タイプ
		D3DXVECTOR3 const & pos,										// 位置
		D3DXVECTOR2 const & size,										// サイズ
		int			const & nTexType,									// テクスチャータイプ
		float		const & frot = 0.0f,								// 角度
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// 色
	);
	// 作成(個人管理スマートポインター)
	//	offsettype	: タイプ
	//	pos			: 位置
	//	size		: サイズ
	//	nTextype	: テクスチャータイプ
	//	frot		: 角度
	//	col			: 色
	static std::unique_ptr<C2DPresents> Create_Unique(
		OFFSET_TYPE	const & type,										// タイプ
		D3DXVECTOR3 const & pos,										// 位置
		D3DXVECTOR2 const & size,										// サイズ
		int			const & nTexType,									// テクスチャータイプ
		float		const & frot = 0.0f,								// 角度
		D3DXCOLOR	const & col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// 色
	);

protected:
	// 設定 //
private:
	/* 列挙型 */

	/* 構造体 */
	// 色の点滅
	typedef struct COL_FLASHING
	{
		COL_FLASHING()
		{
			Origincol = D3DXCOLOR_INI;
			changecol = D3DXCOLOR_INI;
			colDiff = D3DXCOLOR_CA(0.0f,0.0f);
			nTimeSwitch = 0;
			nTimeChange = 0;
			nCntTimeSwitch = 0;
			nCntTimeChange = 0;
			bSwitch = false;
			bChange = false;
		}
		D3DXCOLOR Origincol;			// 元色
		D3DXCOLOR changecol;			// 変わる色
		D3DXCOLOR colDiff;				// 色の差分
		int nTimeSwitch;				// 切り替え時間
		int nTimeChange;				// 切り替わる時間
		int nCntTimeSwitch;				// 切り替え時間カウント
		int nCntTimeChange;				// 切り替わる時間カウント
		bool bSwitch;					// 切り替え中
		bool bChange;					// 切り替わり中
	} COL_FLASHING,*P_COL_FLASHING;
	// 拡大率情報
	typedef struct SCALING
	{
		SCALING()
		{
			OriginSize = D3DVECTOR2_ZERO;
			fScal = 1.0f;
			fScalChange = 0.0f;
			fScalDiff = 0.0f;
			nTimeChange = 0;
			nCntTimeChange = 0;
			bChange = false;
		}
		D3DXVECTOR2 OriginSize;			// 元のサイズ
		float fScal;					// 拡大率
		float fScalChange;				// 目標拡大率
		float fScalDiff;				// 拡大率上昇率
		int nTimeChange;				// 切り替わる時間
		int nCntTimeChange;				// 切り替わる時間カウント
		bool bChange;					// 切り替わり中
	} SCALING, *P_SCALING;
	// 移動情報
	typedef struct DESIGN_MOVE
	{
		DESIGN_MOVE()
		{
			BeginPos = D3DVECTOR3_ZERO;
			EndPos = D3DVECTOR3_ZERO;
			DiffPos = D3DVECTOR3_ZERO;
			nfram = 0;
			nMaxfram = 0;
		}
		D3DXVECTOR3 BeginPos;			// 初期位置
		D3DXVECTOR3 EndPos;				// 目標位置
		D3DXVECTOR3 DiffPos;			// 差分位置
		int nfram;						// フレーム
		int nMaxfram;					// 最大フレーム
	} DESIGN_MOVE, *P_DESIGN_MOVE;
	// 色指定情報
	typedef struct DESIGN_COL
	{
		DESIGN_COL()
		{
			BeginCol = D3DXCOLOR_INI;
			EndCol = D3DXCOLOR_INI;
			DiffCol = D3DXCOLOR_INI;
			nfram = 0;
			nMaxfram = 0;
		}
		D3DXCOLOR BeginCol;				// 初期カラー
		D3DXCOLOR EndCol;				// 目標カラー
		D3DXCOLOR DiffCol;				// 差分カラー
		int nfram;						// フレーム
		int nMaxfram;					// 最大フレーム
	} DESIGN_COL, *P_DESIGN_COL;
	// フェード情報
	typedef struct FADE
	{
		FADE()
		{
			pDesign_Move = NULL;
			pDesign_Col = NULL;
			bFade = false;
			bCompulsion = false;
			nCntFade = 0;
			nMaxFade = 0;
			nCntAnimStart = 0;
			nMaxAnimStart = 0;
		}
		P_DESIGN_MOVE pDesign_Move;		// 移動情報
		P_DESIGN_COL pDesign_Col;		// 色指定情報
		bool bFade;						// フェード中
		bool bCompulsion;				// フェード強制終了させるかさせないか
		int nCntFade;					// カウントフェード
		int nMaxFade;					// 最大フェード数
		int nCntAnimStart;				// スタートカウント
		int nMaxAnimStart;				// 最大スタート
	} FADE, *P_FADE;
	// 強調情報
	typedef struct COOPERATION
	{
		COOPERATION()
		{
			pCol_flash = NULL;
			pScal = NULL;
		}
		P_COL_FLASHING pCol_flash;		// 色情報
		P_SCALING pScal;				// スケール情報
	} COOPERATION, *P_COOPERATION;

	/* 関数 */
	// 点滅更新
	void Update_ColorFlashing(void);
	// 協調更新
	void Update_Scaling(void);
	// フェード(指定色)
	void Update_Fade_DesignCol(
		D3DXCOLOR const & DiffCol,	// 差分色
		D3DXCOLOR const & EndCol,	// 目標色
		int & nFram,				// 現在のフレーム
		int const & nMaxFram		// 最大のフレーム
	);
	// フェード(移動)
	void Update_Fade_DesignMove(
		D3DXVECTOR3 const & DiffPos,	// 差分色
		D3DXVECTOR3 const & GoalPos,	// 目標色
		int & nFram,					// 現在のフレーム
		int const & nMaxFram			// 最大のフレーム
	);
	// フェードイン更新
	bool Update_FadeIn(void);
	// フェードアウト更新
	bool Update_FadeOut(void);
	// 強調更新
	bool Update_Cooperation(void);

	/* 変数 */
	// 強調情報
	P_COOPERATION m_pCooperation;		// 強調情報
	// フェードイン
	P_FADE m_pFadeIn;					// フェード情報
	// フェードアウト
	P_FADE m_pFadeOut;					// フェード情報
	// フェードタイプ
	FADETYPE m_Fadetype;				// フェードタイプ
};

#endif