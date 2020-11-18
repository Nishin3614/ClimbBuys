//------------------------------------------------------------------------------
//
//メッシュボックス処理  [meshbox.h]
//Author:fujiwaramasato
//
//------------------------------------------------------------------------------
#ifndef _MESHBOX_H_
#define _MESHBOX_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMeshBox : public CScene
{
public:

	//メッシュの種類
	enum COLLISION_TYPE
	{
		TYPE_CENTER, // 座標が中心
		TYPE_GROUND	 // 地面が中心
	};

	CMeshBox();
	~CMeshBox();

	void Init(void);															//初期化
	void Uninit();																//終了
	void Update();																//更新
	void Draw();																//描画
	void DebugInfo();															//デバッグ情報表記

	void MakeVertex();															//頂点生成

	void SetPos(D3DXVECTOR3 const& pos);										//座標設定
	void SetCol(D3DXCOLOR const& col);											//色設定
	void SetVtxPos();															// 頂点設定
	void SetSize(D3DXVECTOR3 size);												//サイズ設定
	static void SetDisp(bool disp) { m_bDisp = disp; };

	static bool	GetDisp() { return m_bDisp; };
	D3DXVECTOR3 &GetPos() { return m_pos; };									//座標取得処理
	D3DXVECTOR3 &GetRot() { return m_rot; };									//回転量取得
	D3DXCOLOR	&GetCol() { return m_col; };									//色取得
	D3DXMATRIX	*GetMtx() { return &m_mtxWorld; };								//ワールドマトリックス取得
	D3DXVECTOR3 GetSize() { return m_size; };									//サイズ取得

	static CMeshBox *Create(D3DXVECTOR3 const &pos, D3DXVECTOR3 const &onesize ,
		COLLISION_TYPE  const &type);	//生成処理

										// 当たった後の処理
										//	nObjType	: オブジェクトタイプ
										//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

protected:

private:
	static bool				m_bDisp;
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3				m_pos;				// 座標
	D3DXVECTOR3				m_size;				// サイズ
	D3DXVECTOR3				m_rot;				// 回転
	D3DXCOLOR				m_col;				// 色
	COLLISION_TYPE			m_Type;
};
#endif