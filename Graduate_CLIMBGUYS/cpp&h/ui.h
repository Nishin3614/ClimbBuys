// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ユーザーインターフェースヘッダー処理 [ui.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _UI_H_
#define _UI_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;
class C2DPresents;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CUi : public CScene
{
public:
	/* 列挙型 */
	// UIタイプ
	typedef enum
	{
		UITYPE_GAMEUI = 0,
		UITYPE_PAUSEUI_BG,
		UITYPE_PAUSEUI_SELECT,
		UITYPE_RANKINGUI,
		UITYPE_SETTINGUI_BG,
		UITYPE_SETTINGUI_SELECT,
		UITYPE_TITLEUI_NAME,
		UITYPE_TITLEUI_SELECT,
		UITYPE_TITLEUI_SELECT_BG,
		UITYPE_TUTORIALUI,
		UITYPE_SELECTCHARACTER,
		UITYPE_RESULT,
		UITYPE_DIE,
		UITYPE_GAMESTART,
		UITYPE_FINISH,
		UITYPE_WATCHING,
		UITYPE_MAX
	} UITYPE;
	// UIの情報格納用型名
	typedef std::vector<std::unique_ptr<CUi>> VEC_UNI_UI;
	// UIの情報格納用型名
	typedef std::vector<CUi*> VEC_P_UI;
	/* 構造体*/
	// 点滅ロード
	typedef struct COOPERATION_COL_LOAD
	{
		COOPERATION_COL_LOAD()
		{
			changecol = D3DXCOLOR_INI;
			nTimeSwitch = 0;
			nTimeChange = 0;
		}
		D3DXCOLOR changecol;	// 変わる色
		int nTimeSwitch;		// 切り替え時間
		int nTimeChange;		// 切り替わる時間
	} COOPERATION_COL_LOAD, *P_COOPERATION_COL_LOAD;
	// 拡大率ロード
	typedef struct COOPERATION_SCAL_LOAD
	{
		COOPERATION_SCAL_LOAD()
		{
			fChangeScal = 0.0f;
			nTimeChange = 0;
		}
		float fChangeScal;		// 拡大率
		int nTimeChange;		// 切り替わる時間
	} COOPERATION_SCAL_LOAD, *P_COOPERATION_SCAL_LOAD;

	// 強調ロード
	typedef struct COOPERATION_LOAD
	{
		COOPERATION_LOAD()
		{
			pCol = NULL;
			pScal = NULL;
		}
		P_COOPERATION_COL_LOAD pCol;		// 色
		P_COOPERATION_SCAL_LOAD pScal;		// 拡大率
	} COOPERATION_LOAD, *P_COOPERATION_LOAD;

	// フェードスタート時間
	typedef int NSTARTFADE, *P_NSTARTFADE;
	// フェード(色指定)ロード
	typedef struct FADE_COL_LOAD
	{
		FADE_COL_LOAD()
		{
			BeginCol = D3DXCOLOR_INI;
			EndCol = D3DXCOLOR_INI;
			nMaxfram = 0;
		}
		D3DXCOLOR BeginCol;	// 初期カラー
		D3DXCOLOR EndCol;	// 目的カラー
		int nMaxfram;		// 最大フレーム
	} FADE_COL_LOAD, *P_FADE_COL_LOAD;
	// フェード位置ロード
	typedef struct FADE_POS_LOAD
	{
		FADE_POS_LOAD()
		{
			BeginPos = D3DVECTOR3_ZERO;
			EndPos = D3DVECTOR3_ZERO;
			nMaxfram = 0;
		}
		D3DXVECTOR3 BeginPos;	// 初期位置
		D3DXVECTOR3 EndPos;		// 目的位置
		int nMaxfram;			// 最大フレーム
	} FADE_POS_LOAD, *P_FADE_POS_LOAD;

	// フェードインロード
	typedef struct FADEIN_LOAD
	{
		FADEIN_LOAD()
		{
			FadeStart = 0;
			pFadeCol = NULL;
			pFadePos = NULL;
			bSkip = false;
		}
		NSTARTFADE FadeStart;		// フェードスタート時間
		P_FADE_COL_LOAD pFadeCol;	// フェード(色指定)
		P_FADE_POS_LOAD pFadePos;	// フェード(位置)
		bool bSkip;					// フェードのスキップ
	} FADEIN_LOAD, *P_FADEIN_LOAD;
	// フェードアウトロード
	typedef struct FADEOUT_LOAD
	{
		FADEOUT_LOAD()
		{
			FadeStart = 0;
			pFadeCol = NULL;
			pFadePos = NULL;
			bSkip = false;
		}
		NSTARTFADE FadeStart;	// フェードスタート時間
		P_FADE_COL_LOAD pFadeCol;	// フェード(色指定)
		P_FADE_POS_LOAD pFadePos;	// フェード(位置)
		bool bSkip;					// フェードのスキップ
	} FADEOUT_LOAD, *P_FADEOUT_LOAD;
	// 2DPresentロード
	typedef struct PRESENTS_LOAD
	{
		PRESENTS_LOAD()
		{
			pFadeIn = NULL;
			pFadeOut = NULL;
			pCooperation = NULL;
		}
		P_FADEIN_LOAD pFadeIn;				// フェードインのロード
		P_FADEOUT_LOAD pFadeOut;			// フェードアウトのロード
		P_COOPERATION_LOAD pCooperation;	// 強調のロード
	} PRESENTS_LOAD, *P_PRESENTS_LOAD;


	typedef struct UI_LOAD
	{
		UI_LOAD()
		{
			pos = D3DVECTOR3_ZERO;
			col = D3DXCOLOR_INI;
			size = D3DVECTOR2_ZERO;
			tex_first = D3DVECTOR2_ZERO;
			tex_last = { 1.0f,1.0f };
			fRot = 0.0f;
			nTexType = 0;
			nOffsetType = 0;
			pPresents = NULL;
		}
		D3DXVECTOR3 pos;			// 位置
		D3DXCOLOR col;				// 色
		D3DXVECTOR2 size;			// サイズ
		D3DXVECTOR2 tex_first;		// 最初のテクスチャー座標
		D3DXVECTOR2 tex_last;		// 最後のテクスチャー座標
		float fRot;					// 角度
		int nTexType;				// テクスチャータイプ
		int nOffsetType;			// オフセット
		bool nUse;
		P_PRESENTS_LOAD pPresents;	// 2Dプレゼンツのロード
	} UI_LOAD, *P_UI_LOAD;

	/* 関数 */
	// コンストラクタ
	CUi();
	// デストラクタ
	~CUi();
	// 初期化処理
	void Init(void);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// 当たった後の処理
	// 引数1:オブジェクトタイプ
	// 引数2:相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	)
	{};
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

	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// シーン2Dの取得
	CScene_TWO * GetScene_Two(void);
	// プレゼンツ2Dの取得
	C2DPresents * GetPresents(void);

	static HRESULT Load(void);						// テキストの読み込み
	// UIマネージャーの読み込み
	static HRESULT UiManagerLoad(void);
	// UIスクリプトの読み込み
	static HRESULT UiScriptLoad(void);

	static void UnLoad(void);						// UnLoadする
	// 作成処理(シーン管理)
	static CUi * Create(
		UITYPE const &Uitype,
		int const &nUi
	);
	// 作成処理(個人管理)
	static std::unique_ptr<CUi> Create_Self(
		UITYPE const &Uitype,
		int const &nUi
	);
	// ロードしたものを作成する(シーン管理)
	static CUi::VEC_P_UI LoadCreate(UITYPE const &Uitype);
	// ロードしたものを作成する(個人管理)
	static VEC_UNI_UI LoadCreate_Self(UITYPE const &Uitype);
	// 使用状態を取得
	static bool GetUse(
		UITYPE const &uitype
	) { return m_sta_UiUse[uitype]; };
	// 使用状態を設定
	static void SetUse(
		bool const &bUse,
		UITYPE const &uitype
		) { m_sta_UiUse[uitype] = bUse; };
	// 指定したUIを終了させる
	static void End_Ui(
		UITYPE const &uitype
	);
	// 指定したUIの情報取得
	static std::vector<UI_LOAD> GetUi(
		UITYPE const &uitype
	) { return m_vec_UiLoad[uitype]; };
protected:
private:
	/* 列挙型 */

	/* 関数 */
	// 初期化 //

	// 更新 //

	/* 変数 */
	static std::vector<UI_LOAD> m_vec_UiLoad[UITYPE_MAX];	// Ui読み込み用変数
	static bool	m_sta_UiUse[UITYPE_MAX];					// このUIの使用状態
	static std::vector<std::string> m_vec_String;			// ファイル情報読み書き用
	CScene_TWO * m_pScene_two;								// シーン2D
	C2DPresents * m_pPresents;								// 2Dプレゼンツ
	UITYPE m_Uitype;										// UIタイプ
	int m_nUi;												// UI

	// 処理に必要な変数
	// int型
	std::vector<int> m_vec_nNumber;							// int情報
	// bool型
	std::vector<bool> m_vec_Bool;							// bool情報
	// シーンクラス
	std::unique_ptr<CScene> m_pScene;						// シーン情報

};
#endif
