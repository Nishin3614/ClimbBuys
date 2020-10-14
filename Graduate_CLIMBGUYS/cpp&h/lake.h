// ==========================================================
//
// 湖[lake.h]
// Author : masayasu wakita
//
// ==========================================================
#ifndef _LAKE_H_
#define _LAKE_H_

#include "main.h"
#include "scene_three.h"

#define	TEXTURE_BULLET	"data/TEXTURE/coin.png"			// 読み込むテクスチャファイル名

// ==========================================================
//
// クラスの定義
//
// ==========================================================
class CLake : public CScene
{
public:

	CLake();												// コンストラクタ
	~CLake();												// デストラクタ

	void Init(void);										// 初期化
	void Uninit(void);										// 終了
	void Update(void);										// 更新
	void Draw(void);										// 描画

	static CLake *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// 生成
	static HRESULT Load(void);													// ロード
	static void Unload(void);													// アンロード

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
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_NoOpponentCollision(
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

protected:

private:
	void MakeVertex(void);

	static LPDIRECT3DTEXTURE9 m_pTex;			// テクスチャ
	static LPDIRECT3DTEXTURE9 m_pBumpTex;
	LPD3DXEFFECT m_pEffect = NULL;
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	VERTEX_3D *m_pVtx;							// 頂点情報へのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス

	D3DXVECTOR3	m_vtxMin;						// 最小値
	D3DXVECTOR3	m_vtxMax;						// 最大値
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 回転
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_size;							// サイズ
	D3DXCOLOR m_col;							// 色
	float m_fAnim;								// アニメーション移動量
	float m_fHeight;
};
#endif