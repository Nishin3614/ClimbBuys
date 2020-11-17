//=============================================================================
//
// ライト処理 [light.h]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CLight
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_MAIN = 0,
		TYPE_SUB1,
		TYPE_SUB2,
		TYPE_MAX
	} TYPE;

	/* 関数 */
	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static CLight * Create(void);	// 作成
	D3DLIGHT9 &GetLight(TYPE type);		// ライト情報
	D3DXCOLOR &GetCol(TYPE type);		// 色情報
	D3DVECTOR &GetVec(TYPE type);		// 方向情報
protected:

private:
	D3DLIGHT9 m_light[TYPE_MAX];	// ライト情報
	// 拡散光の色
	D3DXCOLOR m_col[TYPE_MAX] =		// カラー
	{
		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
		D3DXCOLOR(0.6f,0.6f,0.6f,1.0f),
		D3DXCOLOR(0.2f,0.2f,0.2f,1.0f),
	};
	// 光の方向
	D3DXVECTOR3 m_VecDir[TYPE_MAX] =// 方向
	{
		D3DXVECTOR3(0.5f,-1.0f,0.5f),	// メインライト
		D3DXVECTOR3(-0.5f, 1.0f, -0.5f),	// サブ１
		D3DXVECTOR3(1.0f, 0.0f, 0.0f)	// サブ２
	};
};

#endif
