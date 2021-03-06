// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 看板[signboard.h]
// Author : Fujiwara Masato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _SIGNBOARD_H_
#define _SIGNBOARD_H_	 // ファイル名を基準を決める

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
class CSignboard : public CScene_THREE
{
public:
	/* 列挙型 */

	/* 関数 */
	CSignboard();
	~CSignboard();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 作成(シーン継承あり)
	static CSignboard * Create(D3DXVECTOR3 pos);		// 作成
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