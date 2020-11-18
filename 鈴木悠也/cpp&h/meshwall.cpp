// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// メッシュウォール処理の説明[meshwall.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/* 描画 */
#include "meshwall.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "shadow_mapping.h"
#include "character.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MESHWALL_ROTMOVE (0.01f)		// 角度の移動量
#define MESHWALL_SIZE (100.0f)			// サイズ
#define MESHWALL_ANGLE (D3DX_PI / 10)	// 湾曲の角度

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// グローバル変数
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMeshwall::CMeshwall() : CScene()
{
	/* 変数の初期化 */
	// 回転量
	m_pVtxBuff = NULL;
	m_pIndex = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumberVertex = 4;
	m_nNumIndex = 4;
	m_nNumPolygon = 2;
	m_nBlock_Width = 1;
	m_nBlock_Depth = 1;
	m_nTexType = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMeshwall::~CMeshwall()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Init(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;						// 頂点情報へのポイント
	WORD * pIdx;							// インデックスデータへのポインタ
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 One_BlockSize;				// 一つのブロックサイズ
	int nCountDirect;						// 縦のカウント
	int nCountWidth;						// 横のカウント
	int nCntBlock = 0;						// ブロックカウント
	float fYAngle;							// yの角度
	D3DXVECTOR3 *pCross;					// ポリゴンの外積
	// 情報取得
	LPDIRECT3DDEVICE9 pDevice =				// デバイス
		CManager::GetRenderer()->GetDevice();
	pCross =								// メモリ確保
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	// ブロック描画の原点の初期設定
	m_OriginBlock = D3DXVECTOR3(
		m_size.x * -0.5f,
		m_size.y * 0.5f,
		0.0f);

	// 一つのブロックサイズ計算
	One_BlockSize.x = m_size.x / m_nBlock_Width;
	One_BlockSize.y = m_size.y / m_nBlock_Depth;
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
	fYAngle = MESHWALL_ANGLE;

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

			// 頂点座標の設定
			pVtx[0].pos =
				D3DXVECTOR3(
					m_OriginBlock.x +
					One_BlockSize.x * nCountWidth,
					m_OriginBlock.y - 
					One_BlockSize.y * nCountDirect,
					m_OriginBlock.z
				);

			// カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャーの設定
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

			// ポイント合わせ
			pVtx++;
		}
	}
	// 頂点情報のアドレスを初期に戻す
	pVtx -= m_nNumberVertex;
	// ポリゴンごとの法線の設定
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// ベクトル
			VecA = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 1].pos - pVtx[nCntBlock].pos;
			// 外積計算
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] = CCalculation::Cross_product(VecA, VecB);
			// 正規化
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2]);
			// ベクトル
			VecA = pVtx[nCntBlock + 1].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			// 外積計算
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1] = CCalculation::Cross_product(VecA, VecB);
			// 正規化
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1]);
			// 左上
			pVtx[nCntBlock].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// 右上
			pVtx[nCntBlock + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// 左下
			pVtx[nCntBlock + m_nBlock_Width + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2];
			// 右下
			pVtx[nCntBlock + m_nBlock_Width + 2].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
		}
	}
	nCntBlock = 0;
	// 頂点法線の設定
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++, nCntBlock++)
		{
			// 最初
			if (nCntDepth == 0 && nCntWidth == 0)
			{
				pVtx[0].nor /= 2;
			}
			// 最後
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 2;
			}
			// 1行の列の最後
			else if (nCntDepth == 0 && nCntWidth == m_nBlock_Width)
			{
			}
			// 最後行の列の最初
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == 0)
			{
			}
			// 最初の行または最後の行
			else if (nCntDepth == 0 || nCntDepth == m_nBlock_Depth)
			{
				pVtx[0].nor /= 3;
			}
			// 最初の列または最後の列
			else if (nCntWidth == 0 || nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 3;
			}
			// それ以外
			else
			{
				pVtx[0].nor /= 6;
			}
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

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Update(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Draw(void)
{
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
		&m_mtxWorld,					// 2
		&mtxRot);							// 3
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

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexType));
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumberVertex,
		0,
		m_nNumPolygon);
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::Debug(void)
{
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CMeshwall::Load(void)
{
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMeshwall::UnLoad(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMeshwall * CMeshwall::Create(
	D3DXVECTOR3 const &pos,		// 位置
	D3DXVECTOR3 const &size,	// サイズ
	D3DXVECTOR3 const &rot,		// 回転
	int const &nWidth,			// 横ブロック
	int const &nDepth,			// 縦ブロック
	int const &nType			// タイプ
)
{
	// 変数宣言
	CMeshwall * pMeshwall;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pMeshwall = new CMeshwall();
	// シーン管理設定
	pMeshwall->ManageSetting(CScene::LAYER_3DOBJECT);
	// 位置情報
	pMeshwall->m_pos = pos;
	// サイズ情報
	pMeshwall->m_size = size;
	// 回転情報
	pMeshwall->m_rot = rot;
	// 横ブロック
	pMeshwall->m_nBlock_Width = nWidth;
	// 縦ブロック
	pMeshwall->m_nBlock_Depth = nDepth;
	// テクスチャータイプ
	pMeshwall->m_nTexType = nType;
	// 初期化処理
	pMeshwall->Init();
	// 生成したオブジェクトを返す
	return pMeshwall;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 接している面の高さを取得
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CMeshwall::GetHeight(D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;					// 頂点情報へのポイント
	D3DXVECTOR3 VecA, VecB,VecObject;	// ベクトル
	D3DXVECTOR3 Normal;					// 法線
	float fLength = 0;					// 高さ
	int nCntBlock = 0;					// ブロック数
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// ブロックごとに
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// 右上のポリゴン
			if (CCalculation::CrossCollision(&pos, &pVtx[0].pos, &pVtx[1].pos, &pVtx[m_nBlock_Width + 2].pos))
			{
				// ベクトル
				VecA = pVtx[m_nBlock_Width + 2].pos - pVtx[1].pos;	// ベクトルB
				VecB = pVtx[0].pos - pVtx[1].pos;					// ベクトルA
				VecObject = pos - (m_pos + pVtx[1].pos);			// オブジェクトまでのベクトル
				// 高さの代入
				fLength = pVtx[1].pos.y;
				// 外積
				D3DXVec3Cross(&Normal, &VecA, &VecB);
				// 正規化
				D3DXVec3Normalize(&Normal, &Normal);
#ifdef _DEBUG
				//// ブロック番号
				//CDebugproc::Print("ブロック番号[%d]\n", nCntBlock);
				//// ポリゴン番号
				//CDebugproc::Print("ポリゴン番号[%d]\n", nCntBlock * 2 + 1);
				//// 法線
				//CDebugproc::Print("法線[%.3f,%.3f,%.3f]\n", Normal.x, Normal.y, Normal.z);
#endif // _DEBUG
				// 頂点データの範囲をアンロック
				m_pVtxBuff->Unlock();
				// 高さを返す
				return fLength + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// 左下のポリゴン
			else if (CCalculation::CrossCollision(&pos, &pVtx[m_nBlock_Width + 2].pos, &pVtx[m_nBlock_Width + 1].pos, &pVtx[0].pos))
			{
				// ベクトル
				VecA = pVtx[0].pos - pVtx[m_nBlock_Width + 1].pos;						// ベクトルA
				VecB = pVtx[m_nBlock_Width + 2].pos - pVtx[m_nBlock_Width + 1].pos;		// ベクトルB
				VecObject = pos - (m_pos + pVtx[m_nBlock_Width + 1].pos);				// オブジェクトまでのベクトル
				// 高さの代入
				fLength = pVtx[m_nBlock_Width + 1].pos.y;
				// 外積
				D3DXVec3Cross(&Normal, &VecA, &VecB);
				// 正規化
				D3DXVec3Normalize(&Normal, &Normal);
#ifdef _DEBUG
				//// ブロック番号
				//CDebugproc::Print("ブロック番号[%d]\n", nCntBlock);
				//// ポリゴン番号
				//CDebugproc::Print("ポリゴン番号[%d]\n", nCntBlock * 2);
				//// 法線
				//CDebugproc::Print("法線[%.3f,%.3f,%.3f]\n", Normal.x,Normal.y,Normal.z);
#endif // _DEBUG
				// 頂点データの範囲をアンロック
				m_pVtxBuff->Unlock();
				// 高さを返す
				return fLength + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// 頂点情報の更新
			pVtx++;
		}
		// 頂点情報の更新
		pVtx++;

	}

	// 頂点データの範囲をアンロック
	m_pVtxBuff->Unlock();
	// 高さを返す
	return 0.0f;
}
