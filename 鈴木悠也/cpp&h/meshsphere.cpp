//-----------------------------------------------------------------------------
//
// メッシュスフィア処理 [meshsphere.cpp]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "meshsphere.h"
#include "game.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-----------------------------------------------------------------------------
// テクスチャーID
int CMeshsphere::m_nTexId[TEXTYPE_MAX] =
{
	-1,
	10,
	0
};

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CMeshsphere::CMeshsphere() : CScene()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CMeshsphere::~CMeshsphere()
{
}

//-----------------------------------------------------------------------------
// 作成処理(シーン管理)
//-----------------------------------------------------------------------------
CMeshsphere * CMeshsphere::Create(
	D3DXVECTOR3 const &pos,		// 位置
	float const &fRadius,		// 半径
	int const &nWidthBlock,		// 横数
	int const &nHeightBlock,	// 縦数
	D3DXCOLOR const &col,		// カラー
	TEXTYPE	const &textype,		// テクスチャー
	CScene::LAYER const &layer,
	int const &nMaxFrame		// フレーム
)
{
	// 変数宣言
	CMeshsphere *pMeshsphere = new CMeshsphere;
	// シーン管理設定
	pMeshsphere->ManageSetting(layer);
	// 設定
	pMeshsphere->m_MeshSphere.pos = pos;
	pMeshsphere->m_MeshSphere.fRadius = fRadius;
	pMeshsphere->m_MeshSphere.nWidthBlock = nWidthBlock;
	pMeshsphere->m_MeshSphere.nHeightBlock = nHeightBlock;
	pMeshsphere->m_MeshSphere.col = col;
	pMeshsphere->m_MeshSphere.Textype = textype;
	pMeshsphere->m_MeshSphere.nMaxFrame = nMaxFrame;
	// 初期化
	pMeshsphere->Init();
	return pMeshsphere;
}

//-----------------------------------------------------------------------------
// 作成処理(個人管理)
//-----------------------------------------------------------------------------
CMeshsphere * CMeshsphere::Create_Self(
	D3DXVECTOR3 const &pos,		// 位置
	float const &fRadius,		// 半径
	int const &nWidthBlock,		// 横数
	int const &nHeightBlock,	// 縦数
	D3DXCOLOR const &col,		// カラー
	TEXTYPE	const &textype,		// テクスチャー
	int const &nMaxFrame		// フレーム
)
{
	// 変数宣言
	CMeshsphere *pMeshsphere = new CMeshsphere;
	// 設定
	pMeshsphere->m_MeshSphere.pos = pos;
	pMeshsphere->m_MeshSphere.fRadius = fRadius;
	pMeshsphere->m_MeshSphere.nWidthBlock = nWidthBlock;
	pMeshsphere->m_MeshSphere.nHeightBlock = nHeightBlock;
	pMeshsphere->m_MeshSphere.col = col;
	pMeshsphere->m_MeshSphere.Textype = textype;
	pMeshsphere->m_MeshSphere.nMaxFrame = nMaxFrame;
	// 初期化
	pMeshsphere->Init();
	return pMeshsphere;
}

//-----------------------------------------------------------------------------
// 作成処理(個人_unique管理)
//-----------------------------------------------------------------------------
std::unique_ptr<CMeshsphere> CMeshsphere::Create_Unique(
	D3DXVECTOR3 const &pos,		// 位置
	float const &fRadius,		// 半径
	int const &nWidthBlock,		// 横数
	int const &nHeightBlock,	// 縦数
	D3DXCOLOR const &col,		// カラー
	TEXTYPE	const &textype,		// テクスチャー
	int const &nMaxFrame		// フレーム
)
{
	// 変数宣言
	std::unique_ptr<CMeshsphere> pMeshsphere(new CMeshsphere);
	// 設定
	pMeshsphere->m_MeshSphere.pos = pos;
	pMeshsphere->m_MeshSphere.fRadius = fRadius;
	pMeshsphere->m_MeshSphere.nWidthBlock = nWidthBlock;
	pMeshsphere->m_MeshSphere.nHeightBlock = nHeightBlock;
	pMeshsphere->m_MeshSphere.col = col;
	pMeshsphere->m_MeshSphere.Textype = textype;
	pMeshsphere->m_MeshSphere.nMaxFrame = nMaxFrame;
	// 初期化
	pMeshsphere->Init();
	return pMeshsphere;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void CMeshsphere::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	m_MeshSphere.nMaxVtx = (m_MeshSphere.nWidthBlock + 1) * (m_MeshSphere.nHeightBlock + 1);
	m_MeshSphere.nMaxIndex = (m_MeshSphere.nWidthBlock * 2 + 2) * m_MeshSphere.nHeightBlock + ((m_MeshSphere.nHeightBlock - 1) * 4);
	m_MeshSphere.nMaxPolygon = m_MeshSphere.nWidthBlock * 2 * m_MeshSphere.nHeightBlock + (m_MeshSphere.nHeightBlock - 1) * 6;

	m_MeshSphere.fRot = (D3DX_PI * 2) / m_MeshSphere.nWidthBlock;
	m_MeshSphere.fRot2 = (D3DX_PI * 2) / m_MeshSphere.nHeightBlock;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshSphere.nMaxVtx,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_MeshSphere.pVtxBuffMeshSphere,
		NULL);

	// インデックス情報の作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshSphere.nMaxIndex,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_MeshSphere.pIdxBuffMeshSphere,
		NULL);

	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_MeshSphere.pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の情報
	//縦の個数分カウント
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock + 1; nCntV++)
	{
		//円の頂点分
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			//座標
			pVtx[0].pos.x = sinf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;		//カメラみたいな感じで
			pVtx[0].pos.y = cosf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;							//高さ
			pVtx[0].pos.z = cosf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;

			pVtx[0].nor = pVtx[0].pos;							//法線は必ず正規化
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);		//正規化

			pVtx[0].col = m_MeshSphere.col;		//色
			pVtx[0].tex = D3DXVECTOR2((float)nCntV, (float)nCntH);	//(テクスチャのUV座標)

			pVtx++;
		}
	}

	//頂点データをアンロック
	m_MeshSphere.pVtxBuffMeshSphere->Unlock();

	WORD *pIdx;
	//インデックスバッファの範囲をロックし、インデックスデータへのポインタを取得
	m_MeshSphere.pIdxBuffMeshSphere->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock; nCntV++)
	{
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			pIdx[0] = (m_MeshSphere.nWidthBlock + 1) + nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1);			//一列分下 + 何個進んだか + (縦*一列分)
			pIdx[1] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1);											//何個進んだか + (縦*一列分)

			pIdx += 2;
			if ((nCntH + 1) % (m_MeshSphere.nWidthBlock + 1) == 0 && nCntV < m_MeshSphere.nHeightBlock - 1)
			{
				//縮退ポリゴン分の計算
				pIdx[0] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1);
				pIdx[1] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1) + 1;
				pIdx[2] = nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1) + 1;
				pIdx[3] = (m_MeshSphere.nWidthBlock + 1) + nCntH + nCntV * (m_MeshSphere.nWidthBlock + 1) + 1;
				pIdx += 4;

			}
		}
	}

	//インデックスバッファのデータをアンロック
	m_MeshSphere.pIdxBuffMeshSphere->Unlock();
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CMeshsphere::Uninit(void)
{
	// 頂点バッファの開放
	if (m_MeshSphere.pVtxBuffMeshSphere != NULL)
	{
		m_MeshSphere.pVtxBuffMeshSphere->Release();
		m_MeshSphere.pVtxBuffMeshSphere = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CMeshsphere::Update(void)
{
	// フレームによって消える処理
	FrameDelete();
}

//-----------------------------------------------------------------------------
// 設定
//-----------------------------------------------------------------------------
void CMeshsphere::Set(
	D3DXVECTOR3 const & pos,
	int const & nMaxFrame,
	bool const & bUse
)
{
	m_MeshSphere.pos = pos;
	m_MeshSphere.bUse = bUse;
}

//-----------------------------------------------------------------------------
// 色設定
//	col	: 色
//-----------------------------------------------------------------------------
void CMeshsphere::SetCol(D3DXCOLOR const & col)
{
	// 色設定
	m_MeshSphere.col = col;
	// 頂点カラーの設定
	Set_Vtx_Col();
}

//-----------------------------------------------------------------------------
// 色設定
//	col	: 色
//-----------------------------------------------------------------------------
void CMeshsphere::Set_Vtx_Col(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 3D頂点情報

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_MeshSphere.pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の情報
	//縦の個数分カウント
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock + 1; nCntV++)
	{
		//円の頂点分
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			pVtx[0].col = m_MeshSphere.col;		//色
			pVtx++;
		}
	}

	//頂点データをアンロック
	m_MeshSphere.pVtxBuffMeshSphere->Unlock();
}

//-----------------------------------------------------------------------------
// フレームによって消える処理
//-----------------------------------------------------------------------------
void CMeshsphere::FrameDelete(void)
{
	// 不使用状態なら抜ける
	if (m_MeshSphere.bUse == false)
	{
		return;
	}
	// 最大フレーム数が初期値または初期値以下なら関数を抜ける
	else if (m_MeshSphere.nMaxFrame <= 0)
	{
		return;
	}
	// フレームカウントが最大フレーム数以上になったら不使用状態に
	else if (m_MeshSphere.nCntFrame >= m_MeshSphere.nMaxFrame)
	{
		m_MeshSphere.nCntFrame = 0;
		m_MeshSphere.bUse = false;
		return;
	}
	// フレームカウントアップ
	m_MeshSphere.nCntFrame++;
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CMeshsphere::Draw(void)
{
	// 不使用状態だったら抜ける
	if (m_MeshSphere.bUse == false)
	{
		return;
	}
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX			mtxRot, mtxTrans;								// 計算用

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// カリングoff
	pDevice->SetRenderState(D3DRS_LIGHTING, false);				// ライティングoff

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexId[m_MeshSphere.Textype]));
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MeshSphere.mtxWorldMeshSphere);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_MeshSphere.rot.y, m_MeshSphere.rot.x, m_MeshSphere.rot.z);
	D3DXMatrixMultiply(&m_MeshSphere.mtxWorldMeshSphere, &m_MeshSphere.mtxWorldMeshSphere, &mtxRot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_MeshSphere.pos.x, m_MeshSphere.pos.y, m_MeshSphere.pos.z);
	D3DXMatrixMultiply(&m_MeshSphere.mtxWorldMeshSphere, &m_MeshSphere.mtxWorldMeshSphere, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MeshSphere.mtxWorldMeshSphere);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_MeshSphere.pVtxBuffMeshSphere, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(m_MeshSphere.pIdxBuffMeshSphere);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,
		0,
		m_MeshSphere.nMaxIndex,		//頂点数
		0,
		m_MeshSphere.nMaxPolygon);	//ポリゴンの枚数

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// 描画
	pDevice->SetRenderState(D3DRS_LIGHTING, true);				// ライティングon

	// レンダーステート(Zテスト)
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);					// Zテスト有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);			// Zテスト無効
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// 条件
}
#ifdef _DEBUG
//-----------------------------------------------------------------------------
// デバッグ表示
//-----------------------------------------------------------------------------
void CMeshsphere::Debug(void)
{
}
#endif // _DEBUG
//-----------------------------------------------------------------------------
// 読み込み
//-----------------------------------------------------------------------------
HRESULT CMeshsphere::Load(void)
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// 読み込んだものを破棄
//-----------------------------------------------------------------------------
void CMeshsphere::Unload(void)
{
}

//-----------------------------------------------------------------------------
// 位置の設定
//-----------------------------------------------------------------------------
void CMeshsphere::SetPosition(D3DXVECTOR3 const &pos)
{
	m_MeshSphere.pos = pos;
}

//-----------------------------------------------------------------------------
// スフィアの半径の変更
//-----------------------------------------------------------------------------
void CMeshsphere::SetRadius(float const &fRadius)
{
	m_MeshSphere.fRadius = fRadius;
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_MeshSphere.pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の情報
	//縦の個数分カウント
	for (int nCntV = 0; nCntV < m_MeshSphere.nHeightBlock + 1; nCntV++)
	{
		//円の頂点分
		for (int nCntH = 0; nCntH < m_MeshSphere.nWidthBlock + 1; nCntH++)
		{
			//座標
			pVtx[0].pos.x = sinf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;		//カメラみたいな感じで
			pVtx[0].pos.y = cosf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;							//高さ
			pVtx[0].pos.z = cosf(m_MeshSphere.fRot * nCntH) * sinf(m_MeshSphere.fRot2 * nCntV * 0.5f) * m_MeshSphere.fRadius;

			pVtx++;

		}
	}
	//インデックスバッファのデータをアンロック
	m_MeshSphere.pIdxBuffMeshSphere->Unlock();
}
