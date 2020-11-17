// ----------------------------------------
//
// 軌跡処理の説明[meshobit.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "meshobit.h"
#include "game.h"
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
// テクスチャーID
int CMeshobit::m_nTexId[TEX_MAX] = 
{
	50,
	0
};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CMeshobit::CMeshobit() : CScene()
{
	m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
	m_aOffset[0] = D3DVECTOR3_ZERO;		// オフセット座標
	m_aOffset[1] = D3DVECTOR3_ZERO;		// オフセット座標
	m_col = D3DXCOLOR_INI;				// 色
	m_tex = TEX_0;						// テクスチャー
	m_nLine = 0;						// 線の数
	m_nNumberVertex = 0;				// 総頂点数
	m_nNumPolygon = 0;					// 総ポリゴン
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CMeshobit::~CMeshobit()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CMeshobit::Init(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// 設定
	m_nNumberVertex =			// 頂点
		m_nLine * 2;
	m_nNumPolygon =				// ポリゴン
		m_nLine * 2 - 2;
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	//頂点設定 //
	//行ループ
	for (int nCntMeshObit = 0; nCntMeshObit < m_nLine; nCntMeshObit++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		// テクスチャーの設定
		pVtx[0].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_nLine, 0.0f);
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点座標の設定
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// カラーの設定
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		// テクスチャーの設定
		pVtx[1].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_nLine, 1.0f);
		// 法線の設定
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ポイント合わせ
		pVtx += 2;
	}
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CMeshobit::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CMeshobit::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CMeshobit::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// 行列変数
	D3DXMATRIX mtx;
	// 両面を描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファ　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	// Z方向　無効
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&mtx);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexId[m_tex]));
	// 軌跡の描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,										// 描画を始める頂点
		m_nNumPolygon	// ポリゴン数
	);

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 裏面(左回り)でカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Zバッファ　有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// Z方向　有効
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

// ----------------------------------------
// 頂点座標の確定処理
// ----------------------------------------
void CMeshobit::DrawSet(
	D3DXMATRIX const & mtx
)
{
	// 変数宣言
	VERTEX_3D * pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	for (int nCntObit = m_nLine - 1; nCntObit > 0; nCntObit--)
	{
		pVtx[nCntObit * 2].pos = pVtx[(nCntObit - 1) * 2].pos;
		pVtx[nCntObit * 2 + 1].pos = pVtx[(nCntObit - 1) * 2 + 1].pos;
	}
	// 1番目の頂点
	D3DXVec3TransformCoord(
		&pVtx[0].pos,
		&m_aOffset[0],
		&mtx
	);
	// 2番目の頂点
	D3DXVec3TransformCoord(
		&pVtx[1].pos,
		&m_aOffset[1],
		&mtx
	);
	// アンロック
	m_pVtxBuff->Unlock();
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

//-------------------------------------------------------------------------------------------------------------
// 色の設定
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::SetCol(
	D3DXCOLOR const & BiginCol,	// 始点の色
	D3DXCOLOR const & EndCol	// 終点の色
)
{
	// 変数宣言
	VERTEX_3D * pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	//頂点設定 //
	//行ループ
	for (int nCntMeshObit = 0; nCntMeshObit < m_nLine; nCntMeshObit++)
	{
		// カラーの設定
		pVtx[0].col = BiginCol;
		// カラーの設定
		pVtx[1].col = EndCol;
		// ポイント合わせ
		pVtx += 2;
	}
	// アンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 位置の初期化
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::InitPos(D3DXMATRIX const & mtx)
{
	// 変数宣言
	VERTEX_3D * pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 1番目の頂点
	D3DXVec3TransformCoord(
		&pVtx[0].pos,
		&m_aOffset[0],
		&mtx
	);
	// 2番目の頂点
	D3DXVec3TransformCoord(
		&pVtx[1].pos,
		&m_aOffset[1],
		&mtx
	);
	for (int nCntObit = 1; nCntObit < m_nLine; nCntObit++)
	{
		pVtx[nCntObit * 2].pos = pVtx[0].pos;
		pVtx[nCntObit * 2 + 1].pos = pVtx[1].pos;
	}
	// アンロック
	m_pVtxBuff->Unlock();

}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::Debug(void)
{
	// 変数宣言
	VERTEX_3D * pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	for (int nCntObit = 0; nCntObit < m_nLine; nCntObit++)
	{
		//CDebugproc::Print("[軌跡%d]:位置(%.2f,%.2f,%.2f)\n",nCntObit * 2, pVtx[nCntObit * 2].pos.x, pVtx[nCntObit * 2].pos.y,pVtx[nCntObit * 2].pos.z);
		//CDebugproc::Print("[軌跡%d]:位置(%.2f,%.2f,%.2f)\n", nCntObit * 2 + 1, pVtx[nCntObit * 2 + 1].pos.x, pVtx[nCntObit * 2 + 1].pos.y, pVtx[nCntObit * 2 + 1].pos.z);
	}
	// アンロック
	m_pVtxBuff->Unlock();
}
#endif // _DEBUG

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CMeshobit::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CMeshobit::UnLoad(void)
{
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
std::unique_ptr<CMeshobit> CMeshobit::Create_Self(
	int const &nLine,				// 線の数
	D3DXVECTOR3 const &BeginOffset,	// 始点オフセット
	D3DXVECTOR3 const &EndOffset,	// 終点オフセット
	TEX const &TexId				// テクスチャータイプ
)
{
	// 変数宣言
	std::unique_ptr<CMeshobit> pMeshobit(new CMeshobit);		// シーン2Dクラス
	// 線の数
	pMeshobit->m_nLine = nLine;
	// 始点オフセット
	pMeshobit->m_aOffset[0] = BeginOffset;
	// 終点オフセット
	pMeshobit->m_aOffset[1] = EndOffset;
	// テクスチャーID
	pMeshobit->m_tex = TexId;
	// 初期化処理
	pMeshobit->Init();
	// 生成したオブジェクトを返す
	return pMeshobit;
}