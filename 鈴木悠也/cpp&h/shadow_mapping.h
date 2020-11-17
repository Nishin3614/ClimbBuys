// ------------------------------------------
//
// シャドウマッピング処理 [shadow_mapping.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _SHADOWMAPPING_H_
#define _SHADOWMAPPING_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#include "manager.h"
#include "scene_X.h"

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CShadowmapping
{
public:
	/* 関数 */
	// 影描画(モデル)
	static void Draw(
		LPDIRECT3DDEVICE9	&pDevice,	// デバイス情報
		CScene_X::MODEL_LOAD *model,	// モデル情報
		D3DXMATRIX			&mtx		// マトリックス情報
	);
	// 影描画(メッシュ)
	static void Draw(
		LPDIRECT3DDEVICE9	&pDevice,		// デバイス情報
		D3DXMATRIX			&mtx,			// マトリックス情報
		LPDIRECT3DINDEXBUFFER9	&pIndex,	// インデックス情報
		int	&nNumberVertex,					// インデックス情報
		int	&nNumPolygon					// インデックス情報
	);
protected:
private:
};
#endif