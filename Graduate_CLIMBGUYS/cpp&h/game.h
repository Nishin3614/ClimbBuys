// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ゲームヘッダー処理 [game.h]
// Author : Nishiyama Koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "basemode.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPause;			// ポーズクラス
class CTime;			// タイムクラス
class CScore;			// スコアクラス
class CScore_knock;		// 倒した数クラス
class CEvent_ui;		// イベントクラス
class CUi;				// UIクラス
class CGameUI;			// ゲームUIクラス

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CGame : public CBaseMode
{
public:
	/* 列挙型 */
	// 状態
	typedef enum
	{
		STATE_NORMAL = 0,	// 通常状態
		STATE_START,		// 開始状態
		STATE_CLEAR,		// クリア状態
		STATE_GAMEOVER,		// ゲームオーバー状態
		STATE_GAMEDIED,		// 死亡状態
		STATE_PAUSE,		// ポーズ状態
		STATE_MAX			// 最大数
	} STATE;
	// ステージ
	typedef enum
	{
		STAGE_1 = 0,	// ステージ1
		STAGE_2,		// ステージ2
		STAGE_MAX		// 最大ステージ数
	} STAGE;
	// float3型

	/*
	//struct D3DXVECTOR3 Float3;
	typedef struct _FLOAT3 : public D3DXVECTOR3
	{
		union
		{
			D3DXVECTOR3 Float3;
			float fFloat[3];
		};
	} FLOAT3;
	*/
	/* 関数 */
	// コンストラクタ
	CGame();
	// デストラクタ
	~CGame();
	// 初期化処理
	void Init(void);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 生成処理
	static CGame * Create(void);
	// ステージの設定

	// ステージの取得
	static STAGE GetStage(void)					{ return m_Stage; };
	// カウントタイムの取得
	static int GetCntTime(void)					{ return m_nCntTime; };
	// タイムの取得
	static int GetTime(void)					{ return m_nTime; };
	// カウントタイムを秒刻みで取得
	static int GetSecond(void)					{ return m_nCntTime / 60; };
	// 終了フラグの設定
	void SetFinishFlag(bool bFlag)				{ m_bFinishFlag = bFlag; };

	void SetbOperatable(bool bFlag) { m_bOperatable = bFlag; };
	bool GetbOperatable() { return m_bOperatable; };

#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

protected:
private:
	/* 関数 */
	// 静的変数の初期化処理
	void StaticInit(void);
	// ポーズの状態
	void PauseState(void);
	/* 変数 */
	CPause *		m_pause;		// ポーズ
	static STAGE	m_Stage;		// ステージ
	static int		m_nCntTime;		// タイム
	static int		m_nTime;		// タイム
	CGameUI			*m_pGameUI;		// ゲームUI
	int				m_nCntFinish;	// 終了カウント
	bool			m_bFinishFlag;	// 終了フラグ
	bool			m_bOperatable;	// 操作可能フラグ
	bool			m_bBgm;			// 一度だけ処理を通す
};
#endif
