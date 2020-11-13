// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// リザルトのUI[resultUI.h]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _RESULTUI_H_
#define _RESULTUI_H_	 // ファイル名を基準を決める

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
class CResultUI
{
public:
	/* リザルトUI */
	enum class RESULT_UI
	{
		UI_NONE = -1,
		FRAME_1P,			// 1Pの枠
		FRAME_2P,			// 2Pの枠
		FRAME_3P,			// 3Pの枠
		FRAME_4P,			// 4Pの枠
		RANK_01,			// 1位
		RANK_02,			// 2位
		RANK_03,			// 3位
		RANK_04,			// 4位
		UI_MAX
	};

	/* 関数 */
	CResultUI();
	~CResultUI();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 作成(シーン継承あり)
	static CResultUI * Create(void);					// 作成
	static HRESULT Load(void);							// 読み込み
	static void UnLoad(void);							// 破棄
	// 設定 //

	// 取得 //
protected:
private:
	/* 関数 */
	void			InitSettingFrame(void);					// 枠の初期設定
	void			InitSettingScore(void);					// 各スコアの初期設定
	void			UpdateSettingFrame(void);				// 枠の更新設定
	void			UpdateSettingScore(void);				// 各スコアの更新設定
	/* 変数 */
	CScene_TWO		*m_pScene2D[(int)RESULT_UI::UI_MAX];	// シーン2D
	CNumber			*m_pNumber;								// ナンバー
	D3DXVECTOR3		m_move;									// 移動量
	int				m_nMoveCnt;								// 移動を開始するカウント
	bool			m_bMoveStart;							
};
#endif