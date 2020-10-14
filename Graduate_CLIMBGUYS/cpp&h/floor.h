// ----------------------------------------
//
// 床処理の説明[floor.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------


// ----------------------------------------
//
// 前方宣言
//
// ----------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CFloor : public CScene
{
public:
	/* 関数 */
	CFloor();
	~CFloor();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// 当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	void Scene_SetPos(D3DXVECTOR3 &pos) { m_pos = pos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	// 頂点移動処理
	void vertexMove(FILE *pFile);

	static HRESULT Load(void);
	static void UnLoad(void);
	// 生成
	//	pos			: 位置
	//	size		: サイズ
	//	col			: 色
	//	rot			: 回転
	//	nWidth		: 横ブロック
	//	nDepth		: 縦ブロック
	//	nTexType	: テクスチャータイプ
	static CFloor * Create(
		D3DXVECTOR3 const &pos,		// 位置
		D3DXVECTOR3 const & size,	// サイズ
		D3DXCOLOR const & col,		// 色
		D3DXVECTOR3 const & rot,	// 回転
		int const & nWidth,			// 横ブロック
		int const & nDepth,			// 縦ブロック
		int const & nTexType		// テクスチャータイプ
	);
	float GetHeight(D3DXVECTOR3 &pos);								// 高さ取得
	void SetCalculation(bool bValue) { m_bCalculation = bValue; }	// 高さ計算フラグの設定
	bool GetCalculation(void) { return m_bCalculation; }			// 高さ計算フラグの取得
	// 位置設定
	void SetPos(D3DXVECTOR3 const &pos) { m_pos = pos; };
	// サイズ設定
	void SetSize(D3DXVECTOR3 const &size);
	// 色設定
	void SetCol(D3DXCOLOR const & col);
	// 回転設定
	void SetRot(D3DXVECTOR3 const &rot) { m_rot = rot; };
	// 位置取得
	D3DXVECTOR3 &  GetPos(void) { return m_pos; };
	// サイズ取得
	D3DXVECTOR3 &  GetSize(void) { return m_size; };
	// 色取得
	D3DXCOLOR &  GetCol(void) { return m_col; };
	// 回転取得
	D3DXVECTOR3 & GetRot(void) { return m_rot; };

protected:

private:
	/* 関数 */
	void CalculationNormalize(void);

	/* 変数 */
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DTEXTURE9 m_pMaskTex;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndex;					// インデックスバッファのポインタ
	D3DXVECTOR3 m_OriginBlock;							// 初期配置
	D3DXMATRIX  m_mtxWorld;								// ワールドマトリックス
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	D3DXVECTOR3 m_rot;									// 角度
	D3DXCOLOR	m_col;									// 色
	int	m_nNumberVertex;								// 総頂点数
	int m_nNumIndex;									// 総インデックス
	int m_nNumPolygon;									// 総ポリゴン
	int m_nBlock_Depth;									// 縦ブロック数
	int m_nBlock_Width;									// 横ブロック数
	int m_nTexType;										// テクスチャータイプ
	bool m_bCalculation;									// 床の起伏計算フラグ
	static LPD3DXEFFECT		m_pEffect;
};
#endif