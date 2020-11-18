// ----------------------------------------
//
// 3D線処理の説明[3Dline.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _3DLINE_H_
#define _3DLINE_H_	 // ファイル名を基準を決める

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
#define MAX_3DLINE (10)

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
class C3DLine : public CScene
{
public:
	/* 関数 */
	C3DLine();
	~C3DLine();
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return m_ppos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	void SetPos(			// 位置設定
		D3DXVECTOR3 &pos1,
		D3DXVECTOR3 &pos2
	);
	static C3DLine * Create(	// 作成
		D3DXVECTOR3 *pos,
		D3DXVECTOR3 &rot,
		D3DXVECTOR3 &pos1,
		D3DXVECTOR3 &pos2
	);
	static HRESULT Load(void);									// 読み込み
	static void UnLoad(void);									// 読み込んだ情報を破棄
protected:

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 *m_ppos;					// 位置
	D3DXVECTOR3 m_rot;					// 回転
};

#endif