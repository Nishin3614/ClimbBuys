// ----------------------------------------
//
// 読み込み画面処理の説明[loadscreen.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "manager.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ----------------------------------------
//
// 前方宣言
//
// ----------------------------------------
class CScene_TWO;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CLoadScreen
{
public:
	/* 列挙型 */
	// UIタイプ
	typedef enum
	{
		UITYPE_BG = 0,	// UIタイプ_背景
		UITYPE_LOADING,	// UIタイプ_ローディング
		UITYPE_BALLOON,	// UIタイプ_風船
		UITYPE_MAX		// UIタイプ_最大数
	} UITYPE;
	/* 関数 */
	// コンストラクタ
	CLoadScreen();
	// デストラクタ
	~CLoadScreen();
	// 初期化処理
	void Init(void);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
#ifdef _DEBUG
	// デバッグ処理
	void Debug(void);
#endif // _DEBUG
	// 作成(シーン継承あり)
	static CLoadScreen * Create(void);
	// 変数の初期化処理
	void VariableInit(void);
protected:
	// 設定 //
private:
	/* 列挙型 */

	/* 構造体 */

	/* 関数 */

	/* 変数 */
	CScene_TWO * m_paScene_two[UITYPE_MAX];	// シーン2D情報
	int m_nCntLoad;							// ロードカウント
};

#endif