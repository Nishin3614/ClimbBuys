// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// プレイヤーのUI[pulayerUI.h]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYERUI_H_
#define _PLAYERUI_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_three.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayerUI : public CScene_THREE
{
public:
	/* 列挙型 */

	/* 関数 */
	CPlayerUI();
	~CPlayerUI();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 作成(シーン継承あり)
	static CPlayerUI * Create(PLAYER_TAG PlayerTag);	// 作成
	static HRESULT Load(void);							// 読み込み
	static void UnLoad(void);							// 破棄
	// 設定 //

	// 取得 //
protected:
private:
	/* 関数 */
	/* 変数 */
};
#endif