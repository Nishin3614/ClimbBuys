// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// リザルトヘッダー処理 [result.h]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "basemode.h"

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
class CResultUI;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CResult : public CBaseMode
{
public:

	CResult();													// コンストラクタ
	~CResult();													// デストラクタ

	/* メンバ関数 */
	void	Init(void);											// 初期化
	void	Uninit(void);										// 終了
	void	Update(void);										// 更新
	void	Draw(void);											// 描画

	/* 静的メンバ関数 */
	static  CResult		*Create(void);							// リザルトの生成

private:
	CResultUI			*m_pResultUI;							// リザルトUI
	int					m_nCntPressButton;						// ボタンを押した回数
};
#endif
