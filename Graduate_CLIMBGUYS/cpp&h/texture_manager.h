// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// テクスチャー管理処理 [texture_manager.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "manager.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CTexture_manager
{
public:
	/* 列挙型 */
	// テクスチャータイプ
	typedef enum
	{
		TYPE_TESTFLOOR = 0,		// テスト用の床
		TYPE_RESULT_LOG,		// リザルトのロゴ
		TYPE_ANIBUTTON_LOG,		// AniButtonロゴ
		TYPE_BG,				// ゲームの背景
		TYPE_NONE,				// 背景なし
		TYPE_TUTORIAL_FRAME,	// チュートリアルの枠
		TYPE_MAGMA,				// ダメージ床用マグマ
		TYPE_MAX				// 最大数
	} TYPE;
	/* 関数 */
	// コンストラクタ
	CTexture_manager();
	// デストラクタ
	~CTexture_manager();
	// 開放
	static void UnLoad(void);
	// 情報取得
	// テクスチャーの読み込み
	static HRESULT Load(void);
	// テクスチャー取得
	static LPDIRECT3DTEXTURE9 GetTexture(int const & nTextureId);
protected:
private:
	static std::vector<LPDIRECT3DTEXTURE9> m_st_vpTexture;
};

#endif
