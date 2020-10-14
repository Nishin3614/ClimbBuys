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
	static CFloor * Create(		// 作成
		D3DXVECTOR3 const &pos,
		D3DXVECTOR3 const & size,
		D3DXVECTOR3 const & rot,
		int const & nWidth,
		int const & nDepth,
		int const & nTexType
	);
	float GetHeight(D3DXVECTOR3 &pos);								// 高さ取得
	void SetCalculation(bool bValue) { m_bCalculation = bValue; }	// 高さ計算フラグの設定
	bool GetCalculation(void) { return m_bCalculation; }			// 高さ計算フラグの取得

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