// ----------------------------------------
//
// デバッグ表示処理の説明[debugproc.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_	 // ファイル名を基準を決める

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
#define MAX_DEBUGPROC (10)
#define STRING_MAX (5000)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CDebugproc
{
public:
	/* 関数 */
	CDebugproc();
	~CDebugproc();

	void Init(LPDIRECT3DDEVICE9 m_pDevice);
	void Uninit(void);
	static void Print(char* fmt,...);
	static void Draw(void);
protected:

private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[STRING_MAX];
};

#endif