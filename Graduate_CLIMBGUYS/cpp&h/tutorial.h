// ------------------------------------------
//
// チュートリアルヘッダー処理 [tutorial.h]
// Author : Nishiyama Koki
//
// ------------------------------------------
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "basemode.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------
class CPlayer;				// プレイヤークラス
class CTutorialUI;			// チュートリアル

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CTutorial : public CBaseMode
{
public:
	/* 列挙型 */

	/* 関数 */
	// コンストラクタ
	CTutorial();
	// デストラクタ
	~CTutorial();
	// 初期化処理
	void Init(void);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
	// 生成処理
	static CTutorial * Create(void);
//#ifdef _DEBUG
//	// デバッグ処理
//	void Debug(void);
//#endif // _DEBUG
protected:
private:
	int					m_nDeterminationCnt;
	CTutorialUI			*m_pTutorialUI;							// チュートリアルUI
};
#endif
