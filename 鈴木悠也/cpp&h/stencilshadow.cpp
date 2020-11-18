// ----------------------------------------
//
// ステンシルシャドウ処理の説明[stencilshadow.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "stencilshadow.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "scene_two.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define STENCILSHADOW_ROTMOVE (0.1f)				// 角度の移動量
#define STENCILSHADOW_SIZE (100.0f)					// サイズ

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
int CStencilshadow::m_nTexId[TYPE_MAX] =
{
	0,
};
CScene_TWO * CStencilshadow::m_pShadow = NULL;	// シャドウ2D

#ifdef _DEBUG
bool	CStencilshadow::m_bStencil = true;								// ステンシル描画するかしないか
#endif // _DEBUG


// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CStencilshadow::CStencilshadow() : CScene()
{
	/* 変数の初期化 */
	// 回転量
	m_pVtxBuff = NULL;
	m_pIndex = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_nNumberVertex = 4;
	m_nNumIndex = 4;
	m_nNumPolygon = 2;
	m_nBlock_Width = 1;
	m_nBlock_Depth = 1;
	m_bUse = true;
	m_type = TYPE_CYLINDER;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CStencilshadow::~CStencilshadow()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CStencilshadow::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =				// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross[2000];				// 外積
	// 頂点座標の生成
	// MakeVertex(pDevice);
	//
	switch (m_type)
	{
		// 円柱
	case TYPE_CYLINDER:
		SetCylinder(pDevice);
		break;
		// 矩形
	case TYPE_RECT:
		SetRect(pDevice);
		break;
	default:
		break;
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CStencilshadow::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CStencilshadow::Update(void)
{

}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CStencilshadow::Draw(void)
{
	if (!m_bUse) return;
	// 描画状態
	CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLBACK);
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映 //
	// ヨーピッチロールによる回転行列作成(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,	// 総合の入れ物
		m_rot.y, m_rot.x, m_rot.z);			// それぞれの回転量
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxRot);					// 3
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,// 総合の入れ物
		m_pos.x, m_pos.y, m_pos.z);	// それぞれの移動量
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームを設定
	pDevice->SetIndices(
		m_pIndex);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

#ifdef _DEBUG
	// ステンシル描画がfalseなら
	// ->通常描画
	if (!m_bStencil)
	{
		CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLNORMAL);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,
			0,
			m_nNumberVertex,		//使用する頂点数 三角ポリゴンの頂点
			0,						//頂点の読み取りを開始する位置
			m_nNumPolygon);			//ポリゴンの枚数

					// カリングしない
		CManager::GetRenderer()->SetType(CRenderer::TYPE_CULLNORMAL);

		//Zテスト通常
		//CManager::GetRenderer()->SetType(CRenderer::TYPE_ZBUFFOFF);

		//通常合成
		CManager::GetRenderer()->SetType(CRenderer::TYPE_NORMALMIX);

		//ライティングON
		CManager::GetRenderer()->SetType(CRenderer::TYPE_RIGHTINGON);
	}
	// それ以外なら
	// ->ステンシル描画する
	else
#endif // _DEBUG
	{
		// Zバッファの書き込みを無効に
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		// ステンシルバッファを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
		// カラー描画を無効にする
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);
		// ステンシル対象を設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		// ステンシルテスト、Zテスト両方とも合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
		// ステンシルテスト合格、Zテスト不合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
		// ステンシルテスト不合格、Zテスト不合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
		// 裏面カリング(元のカリング)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			m_nNumberVertex,
			0,
			m_nNumPolygon);
		// ステンシル参照値の設定
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);
		// ステンシル対象を設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		// ステンシルテスト、Zテスト両方とも合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
		// ステンシルテスト合格、Zテスト不合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
		// ステンシルテスト不合格、Zテスト不合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
		// 時計カリング(元のカリング)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			m_nNumberVertex,
			0,
			m_nNumPolygon);
		// カラー描画を有効にする
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
		// ステンシル参照値の設定
		pDevice->SetRenderState(D3DRS_STENCILREF, 2);
		// ステンシル対象を設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		// ステンシルテスト、Zテスト両方とも合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		// ステンシルテスト合格、Zテスト不合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		// ステンシルテスト不合格、Zテスト不合格の場合の判定
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		// 反時計カリング(元のカリング)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// シャドウ2Dが存在していたら
		if (m_pShadow)
		{
			// シャドウ2Dの開放
			m_pShadow->Draw();
		}
		// ステンシルバッファを無効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
		// 反時計カリング(元のカリング)
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// カラー描画を有効にする
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
		// Zを有効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	}
}

#ifdef _DEBUG
// ----------------------------------------
// デバッグ表示処理
// ----------------------------------------
void CStencilshadow::Debug(void)
{
}
#endif // _DEBUG
// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CStencilshadow::Load(void)
{
	// シャドウ2Dが存在していたら
	if (!m_pShadow)
	{
		m_pShadow = CScene_TWO::Create_Self(
			CScene_TWO::OFFSET_TYPE_CENTER,
			{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f },
			{ SCREEN_WIDTH,SCREEN_HEIGHT },
			-1,
			0.0f,
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f)
		);
	}
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CStencilshadow::UnLoad(void)
{
	// シャドウ2Dが存在していたら
	if (m_pShadow)
	{
		// シャドウ2Dの開放
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = NULL;
	}
}

// ----------------------------------------
// 作成処理(シーン管理)
// ----------------------------------------
CStencilshadow * CStencilshadow::Create(
	D3DXVECTOR3 const &		pos,	// 位置
	D3DXVECTOR3 const &		size,	// サイズ
	TYPE const &			type,	// タイプ
	CScene::LAYER const &	layer,	// レイヤー
	int const &				nWidth,	// 横数
	int const &				nDepth,	// 縦数
	D3DXCOLOR	const &		col,	// カラー
	D3DXVECTOR3 const &		rot		// 回転
)
{
	// 変数宣言
	CStencilshadow * pStencilshadow;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pStencilshadow = new CStencilshadow();
	// シーン管理設定
	pStencilshadow->ManageSetting(layer);
	// 位置情報
	pStencilshadow->m_pos = pos;
	// サイズ情報
	pStencilshadow->m_size = size;
	// 回転情報
	pStencilshadow->m_rot = rot;
	// 色情報
	pStencilshadow->m_col = D3DXCOLOR(0.0f,0.0f,0.0f,0.5f);
	// 横ブロック
	pStencilshadow->m_nBlock_Width = nWidth;
	// 縦ブロック
	pStencilshadow->m_nBlock_Depth = nDepth;
	// タイプ
	pStencilshadow->m_type = type;
	// 初期化処理
	pStencilshadow->Init();
	// 生成したオブジェクトを返す
	return pStencilshadow;
}

// ----------------------------------------
// 作成処理(個人管理)
// ----------------------------------------
CStencilshadow * CStencilshadow::Create_Self(
	D3DXVECTOR3 const &pos,		// 位置
	D3DXVECTOR3 const &size,	// サイズ
	TYPE const &type,			// タイプ
	int const &nWidth,			// 横数
	int const &nDepth,			// 縦数
	D3DXCOLOR	const &col,		// カラー
	D3DXVECTOR3 const &rot		// 回転
)
{
	// 変数宣言
	CStencilshadow * pStencilshadow;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pStencilshadow = new CStencilshadow();
	// 位置情報
	pStencilshadow->m_pos = pos;
	// サイズ情報
	pStencilshadow->m_size = size;
	// 回転情報
	pStencilshadow->m_rot = rot;
	// 色情報
	pStencilshadow->m_col = col;
	// 横ブロック
	pStencilshadow->m_nBlock_Width = nWidth;
	// 縦ブロック
	pStencilshadow->m_nBlock_Depth = nDepth;
	// タイプ
	pStencilshadow->m_type = type;
	// 初期化処理
	pStencilshadow->Init();
	// 生成したオブジェクトを返す
	return pStencilshadow;
}

// ----------------------------------------
// 位置設定
// ----------------------------------------
void CStencilshadow::SetPos(D3DXVECTOR3 const & pos)
{
	m_pos = pos;
}

// ----------------------------------------
// サイズ設定
// ----------------------------------------
void CStencilshadow::SetSize(D3DXVECTOR3 const & size)
{
	// サイズ設定
	m_size = size;
	// 頂点座標の設定
	// タイプが円柱なら
	if (m_type == TYPE_CYLINDER)
	{
		SetCylinderVtxPos();
	}
	// タイプが矩形なら
	else if (m_type == TYPE_RECT)
	{
		SetRectVtxPos();
	}
}

// ----------------------------------------
// 回転設定
// ----------------------------------------
void CStencilshadow::SetRot(D3DXVECTOR3 const & rot)
{
	m_rot = rot;
}

// ----------------------------------------
// 色設定
// ----------------------------------------
void CStencilshadow::SetCol(D3DXCOLOR const & col)
{
	// 色設定
	m_col = col;
	// シャドウ2Dが存在していたら
	if (m_pShadow)
	{
		// シャドウ2Dの開放
		m_pShadow->SetCol(m_col);
		m_pShadow->Set_Vtx_Col();
	}
	// // シーン2D情報NULLチェック
	// if (m_pSceneTwo)
	// {
	// 	// 色の設定
	// 	m_pSceneTwo->SetCol(m_col);
	// 	m_pSceneTwo->Set_Vtx_Col();
	// }
}

// ----------------------------------------
// 使用状態
// ----------------------------------------
void CStencilshadow::SetUse(bool const bUse)
{
	m_bUse = bUse;
}

// ----------------------------------------
// 円柱の設定
// ----------------------------------------
void CStencilshadow::SetCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_3D *pVtx;			// 頂点情報へのポイント
	WORD * pIdx;				// インデックスデータへのポインタを取得
	D3DXVECTOR3 VecA, VecB;		// ベクトル
	float fOriginY;				// yの位置
	int nCountDirect;			// 縦のカウント
	int nCountWidth;			// 横のカウント
	float fAngle;				// yの角度
	float fRadian;				// yのラジアン値
	D3DXVECTOR3 *pCross;		// ポリゴンの外積
	pCross =					// メモリ確保
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	// ブロック描画の原点の初期設定
	fOriginY = -m_size.y * m_nBlock_Depth;
	// 総頂点数・インデックス・ポリゴンの初期設定(計算)
	m_nNumberVertex =
		(m_nBlock_Depth + 1) * (m_nBlock_Width + 1);
	m_nNumIndex =
		(m_nBlock_Width + 1) * 2 * m_nBlock_Depth +
		2 * (m_nBlock_Depth - 1);
	m_nNumPolygon =
		m_nBlock_Depth * m_nBlock_Width * 2 +
		4 * (m_nBlock_Depth - 1);
	// 角度の計算
	fAngle = D3DX_PI * 2 / m_nBlock_Width;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//頂点設定 //
	//行ループ
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// 列ループ
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ラジアン値
			fRadian = fAngle * nCountWidth;
			fRadian = CCalculation::Rot_One_Limit(fRadian);
			// 頂点座標の設定
			pVtx[0].pos =
				D3DXVECTOR3(
				(sinf(fRadian) * m_size.x),
					fOriginY + m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// カラーの設定
			pVtx[0].col = m_col;

			// テクスチャーの設定
			pVtx[0].tex = D3DXVECTOR2(
				1.0f,
				1.0f
			);

			// ポイント合わせ
			pVtx++;
		}
	}
	// アンロック
	m_pVtxBuff->Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pIndex->Lock(0, 0, (void **)&pIdx, 0);

	// 縦ブロック個数
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth; nCountDirect++)
	{
		// ２回目のループ以降
		if (nCountDirect >= 1)
		{
			// 縮退ポリゴン分の頂点追加
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width + 1;

			// インデックスのポイント合わせ
			pIdx++;
		}

		// 横ブロックの頂点数
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// 描画順番のインデックス
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth + m_nBlock_Width + 1;
			pIdx[1] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth;

			// インデックスのポイント合わせ
			pIdx += 2;
		}

		// 縮退ポリゴンを作る必要がある場合
		if (nCountDirect < m_nBlock_Depth - 1)
		{
			// 縮退ポリゴン分の頂点追加
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width;
			// インデックスのポイント合わせ
			pIdx++;
		}
	}

	// アンロック
	m_pIndex->Unlock();
	delete[] pCross;
	pCross = NULL;
}

// ----------------------------------------
// 矩形の頂点座標の設定
// ----------------------------------------
void CStencilshadow::SetCylinderVtxPos(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;			// 頂点情報へのポイント
	float fOriginY;				// yの位置
	float fAngle;				// 角度
	float fRadian;				// yのラジアン値
	// ブロック描画の原点の初期設定
	fOriginY = -m_size.y * m_nBlock_Depth;
	// 角度の計算
	fAngle = D3DX_PI * 2 / m_nBlock_Width;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//頂点設定 //
	//行ループ
	for (int nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// 列ループ
		for (int nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ラジアン値
			fRadian = fAngle * nCountWidth;
			fRadian = CCalculation::Rot_One_Limit(fRadian);
			// 頂点座標の設定
			pVtx[0].pos =
				D3DXVECTOR3(
				(sinf(fRadian) * m_size.x),
					fOriginY + m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// ポイント合わせ
			pVtx++;
		}
	}
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 矩形の設定
// ----------------------------------------
void CStencilshadow::SetRect(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;
	//インデックスデータへのポインタ
	WORD * pIdx;

	// 総頂点数・インデックス・ポリゴンの初期設定(計算)
	m_nNumberVertex = 10;
	m_nNumIndex = 10;
	m_nNumPolygon = 8;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の合計 = 8
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,	-m_size.y,	m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y,	m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y,	-m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y,	-m_size.z * 0.5f);
	pVtx[4].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y,	m_size.z * 0.5f);

	pVtx[5].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f,	m_size.z * 0.5f);
	pVtx[6].pos = D3DXVECTOR3(m_size.x * 0.5f,	0.0f,	m_size.z * 0.5f);
	pVtx[7].pos = D3DXVECTOR3(m_size.x * 0.5f,	0.0f,	-m_size.z * 0.5f);
	pVtx[8].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f,	-m_size.z * 0.5f);
	pVtx[9].pos = D3DXVECTOR3(-m_size.x * 0.5f,	0.0f,	m_size.z * 0.5f);



	//頂点の合計 = 8
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点の合計 = 8
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	pVtx[4].col = m_col;
	pVtx[5].col = m_col;
	pVtx[6].col = m_col;
	pVtx[7].col = m_col;
	pVtx[8].col = m_col;
	pVtx[9].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファのロックし、インデックスデータへのポインタを取得
	m_pIndex->Lock(0, 0, (void**)&pIdx, 0);

	// 横ブロックの頂点数
	for (int nCntIndex = 0; nCntIndex < 4 + 1; nCntIndex++)
	{
		// 描画順番のインデックス
		pIdx[0] = nCntIndex + 4 + 1;
		pIdx[1] = nCntIndex;

		// インデックスのポイント合わせ
		pIdx += 2;
	}
	//インデックスのバッファのアンロック
	m_pIndex->Unlock();
}

// ----------------------------------------
// 矩形の頂点座標の設定
// ----------------------------------------
void CStencilshadow::SetRectVtxPos(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の合計 = 8
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y, m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y, -m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, -m_size.z * 0.5f);
	pVtx[4].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, m_size.z * 0.5f);

	pVtx[5].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[6].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[7].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, -m_size.z * 0.5f);
	pVtx[8].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, -m_size.z * 0.5f);
	pVtx[9].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}