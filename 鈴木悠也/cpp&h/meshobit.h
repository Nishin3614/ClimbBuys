// ----------------------------------------
//
// 軌跡処理の説明[meshobit.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MESHOBIT_H_
#define _MESHOBIT_H_	 // ファイル名を基準を決める

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
class CMeshobit : public CScene
{
public:
	/* 列挙型 */
	// テクスチャータイプ
	typedef enum
	{
		TEX_0 = 0,
		TEX_1,
		TEX_MAX
	} TEX;

	/* 関数 */
	CMeshobit();
	~CMeshobit();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
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
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	void DrawSet(
		D3DXMATRIX const &mtx
	);
	// カラー設定
	void SetCol(
		D3DXCOLOR const & BiginCol,	// 始点の色
		D3DXCOLOR const & EndCol	// 終点の色
	);
	// 初期位置にする
	void InitPos(
		D3DXMATRIX const &mtx
	);
	// 読み込み
	static HRESULT Load(void);
	// 読み込んだものを破棄
	static void UnLoad(void);
	// 作成処理(個人管理)
	static std::unique_ptr<CMeshobit> Create_Self(
		int const &nLine,				// 線の数
		D3DXVECTOR3 const &BeginOffset,	// 始点オフセット
		D3DXVECTOR3 const &EndOffset,	// 終点オフセット
		TEX const &TexId = TEX_0		// テクスチャータイプ
	);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
protected:

private:
	/* 関数 */
	/* 変数 */
	static int				m_nTexId[TEX_MAX];			// テクスチャーID
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXVECTOR3				m_aOffset[2];				// オフセット座標
	D3DXCOLOR				m_col;						// 色
	int						m_nLine;					// 線の数
	int						m_nNumberVertex;			// 総頂点数
	int						m_nNumPolygon;				// 総ポリゴン
	TEX						m_tex;						// テクスチャータイプ
};

#endif