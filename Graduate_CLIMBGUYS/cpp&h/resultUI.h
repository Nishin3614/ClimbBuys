// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// リザルトのUI[resultUI.h]
// Author : Fujiwara Masato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _RESULTUI_H_
#define _RESULTUI_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_two.h"

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
class CResultUI : public CScene_TWO
{
public:
	/* 列挙型 */

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
	/* 変数 */
};
#endif