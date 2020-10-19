//------------------------------------------------------------------------------
//
// 背景の処理  [bg.h]
// Author:Yoshiki Sato
//
//------------------------------------------------------------------------------
#ifndef _BG_H_
#define _BG_H_
//------------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene_two.h"

//------------------------------------------------------------------------------
// マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// クラス定義
//------------------------------------------------------------------------------
class CBg : public CScene_TWO
{
public:


	CBg();
	~CBg();

	void Init();						//初期化
	void Uninit();						//終了
	void Update();						//更新
	void Draw();						//描画

	static CBg *Create();				//生成処理

private:
	/* メンバ変数 */
	CScene_TWO					*m_pScene2D;			// シーン2Dのポインタ

};

#endif