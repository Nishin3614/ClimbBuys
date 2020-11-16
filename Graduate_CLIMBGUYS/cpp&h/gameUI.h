// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ゲームのUI[gameUI.h]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _GAMEUI_H_
#define _GAMEUI_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;		// シーン2D
class CNumber;			// ナンバー

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CGameUI
{
public:
	/* ゲームUI */
	enum class GAME_UI
	{
		UI_NONE = -1,
		START,					// 始め
		FINISH,					// 終了
		UI_MAX
	};

	/* 関数 */
	CGameUI();
	~CGameUI();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 作成(シーン継承あり)
	static CGameUI	*Create(void);					// 作成
	static HRESULT	Load(void);						// 読み込み
	static void		UnLoad(void);					// 破棄
	// 設定 //

	// 取得 //
	bool			GetStartFlag() { return m_bStart; };	// スタートフラグの取得
protected:
private:
	/* 関数 */
	void			StartTimer();							// スタートタイマー
	void			FinishSignal();							// 終了の合図
	/* 変数 */
	CScene_TWO		*m_pScene2D[(int)GAME_UI::UI_MAX];		// シーン2D
	CNumber			*m_pStartCount;							// スタートカウント
	int				m_nCntTimer;							// タイマーカウント
	int				m_nTimerNum;							// タイマーのナンバー
	bool			m_bStart;								// スタートフラグ
};
#endif