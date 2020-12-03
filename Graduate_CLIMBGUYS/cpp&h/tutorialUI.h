// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// チュートリアルのUI[tutorialUI.h]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _TUTORIALUI_H_
#define _TUTORIALUI_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Calculation.h"

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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CTutorialUI
{
public:
	/* 関数 */
	CTutorialUI();
	~CTutorialUI();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 作成(シーン継承あり)
	static CTutorialUI	*Create(void);					// 作成
	static HRESULT		Load(void);						// 読み込み
	static void			UnLoad(void);					// 破棄

	bool Ready(int nCntPlayer);		// 準備完了
	// 設定 //

	// 取得 //
protected:
private:
	/* 関数 */
	/* 変数 */
	CScene_TWO		*m_pScene2D[(int)PLAYER_TAG::PLAYER_MAX];	// シーン2D
	bool			m_bStart;								// スタートフラグ
};
#endif