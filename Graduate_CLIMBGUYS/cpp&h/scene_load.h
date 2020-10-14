// ----------------------------------------
//
// シーンの読み込み処理の説明[scene_load.h]
// Author : Koki Nishiyama
//
// ----------------------------------------

#ifndef _SCENE_LOAD_H_
#define _SCENE_LOAD_H_	 // ファイル名を基準を決める

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
#define SCENE_LOAD_MAX (128)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CScene_load
{
public:
	/* 関数 */
	CScene_load();
	~CScene_load();
	static void LoadAll(void);			// シーン親子の作成
	static void UnLoadAll(void);		// シーン親子の作成
protected:

private:
};

#endif