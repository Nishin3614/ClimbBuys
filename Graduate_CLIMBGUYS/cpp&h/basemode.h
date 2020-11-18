// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ベースモードヘッダー処理 [basemode.h]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _BASEMODE_H_
#define _BASEMODE_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TRANSTION_TIME (600) // 自動遷移するまでの時間

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseMode
{
public:
	/* 関数 */
	// コンストラクタ
	CBaseMode();
	// デストラクタ
	virtual ~CBaseMode();
	// 初期化処理
	virtual void Init(void);
	// 終了処理
	virtual void Uninit(void);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);

#ifdef _DEBUG
	// デバッグ処理
	virtual void Debug(void);
#endif // _DEBUG

	void SetTransitionCnt(int nCnt) { m_TransitionCnt = nCnt; };
	int  GetTransitionCnt() const{ return m_TransitionCnt; };

protected:
private:
	bool	m_bSceneStop;						// シーンを止めるフラグ
	int		m_TransitionCnt = TRANSTION_TIME;	// 自動で画面遷移までのカウント
};
#endif
