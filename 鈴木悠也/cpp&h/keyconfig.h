// ------------------------------------------
//
// キー詳細処理 [keyconfig.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _KEYCONFIG_H_
#define _KEYCONFIG_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"

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

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CKeyConfig
{
public:
	/* 列挙型 */
	typedef enum
	{
		CONFIG_DECISION = 0,
		CONFIG_BACK,
		CONFIG_POUSE,
		CONFIG_UP,
		CONFIG_DOWN,
		CONFIG_LEFT,
		CONFIG_RIGHT,
		CONFIG_CAMERAUP,
		CONFIG_CAMERADOWN,
		CONFIG_CAMERALEFT,
		CONFIG_CAMERARIGHT,
		CONFIG_JUMP,
		CONFIG_BALLOONCREATE,
		CONFIG_TRICK1,
		CONFIG_TRICK2,
		CONFIG_MAX
	} CONFIG;
	/* 関数 */
	CKeyConfig();
	~CKeyConfig();
	void Init(void);
	void Uninit(void);
	void Update(void);
	bool const GetKeyConfigPress(CONFIG const &config);	// キー詳細(押している間)情報取得
	bool const GetKeyConfigTrigger(CONFIG const &config);	// キー詳細(押されたとき)情報取得
protected:
private:
	/* 関数 */
	/* 変数 */
	int m_nKeyboardId[CONFIG_MAX];			// キーボードID
	int m_nPadID[CONFIG_MAX];					// ジョイパッドID
};
#endif
