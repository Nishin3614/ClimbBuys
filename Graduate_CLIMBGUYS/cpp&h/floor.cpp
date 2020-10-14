// ----------------------------------------
//
// 床処理の説明[floor.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "floor.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "character.h"
#include "light.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define FLOOR_ROTMOVE (0.01f)						// 角度の移動量
#define FLOOR_SIZE (100.0f)							// サイズ
#define FLOOR_ANGLE (1.0f)							// 湾曲の角度
#define TEXTURE_MASK ("data/TEXTURE/mask.jpg")		// マスクテクスチャアドレス

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
//LPD3DXEFFECT CFloor::m_pEffect = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CFloor::CFloor() : CScene()
{
	/* 変数の初期化 */
	// 回転量
	m_pTexture = NULL;
	//m_pMaskTex = NULL;
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
	m_bCalculation = false;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CFloor::~CFloor()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CFloor::Init(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;						// 頂点情報へのポイント
	WORD * pIdx;							// インデックスデータへのポインタ
	int nCountDirect;						// 縦のカウント
	int nCountWidth;						// 横のカウント
	int nCntBlock = 0;						// ブロックカウント
	float fYAngle;							// yの角度
	float fYRadian;							// yのラジアン値
	// 情報取得
	LPDIRECT3DDEVICE9 pDevice =				// デバイス
		CManager::GetRenderer()->GetDevice();

	/*
	if (m_pEffect == NULL)
	{
		//シェーダーを読み込む
		if (FAILED(D3DXCreateEffectFromFile(pDevice, "data/LOAD/MAPPING/MaskShader.fx", NULL, NULL,
			0, NULL, &m_pEffect, NULL)))
		{
			MessageBox(NULL, "シェーダーファイル読み込み失敗", "", MB_OK);
		}
	}

	// テクスチャー生成
	D3DXCreateTextureFromFile(
		CManager::GetRenderer()->GetDevice(),
		TEXTURE_MASK,
		&m_pMaskTex);
*/
	// ブロック描画の原点の初期設定
	m_OriginBlock = D3DXVECTOR3(
		m_size.x * -0.5f * m_nBlock_Width,
		0.0f,
		m_size.z * 0.5f * m_nBlock_Depth);
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
	fYAngle = FLOOR_ANGLE;
	m_size.y = 100;
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
			// ラジアン値(Y)
			fYRadian = fYAngle * nCountDirect;
			fYRadian = CCalculation::Rot_One_Limit(fYRadian);

			// 頂点座標の設定
			pVtx[0].pos =
				D3DXVECTOR3(
					m_OriginBlock.x +
					(m_size.x * nCountWidth),
					m_OriginBlock.y,
					m_OriginBlock.z -
					(m_size.z * nCountDirect));

			// カラーの設定
			pVtx[0].col = m_col;

			// テクスチャーの設定
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

			// ポイント合わせ
			pVtx++;
		}
	}
	// アンロック
	m_pVtxBuff->Unlock();

	CalculationNormalize();

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
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CFloor::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	/*
	// インデックス情報の開放
	if (m_pIndex != NULL)
	{
		m_pIndex->Release();
		m_pIndex = NULL;
	}
	// シェーダーの開放
	if (m_pEffect != NULL)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}
	*/
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CFloor::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CFloor::Draw(void)
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

	D3DXMATRIX mtxProj, mtxView;

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	/*
	if (m_bCalculation)
	{
		// シェーダー処理
		if (m_pEffect != NULL)
		{
			m_pEffect->SetTechnique("tecLambert");
			D3DXMATRIX mAll = m_mtxWorld * mtxView * mtxProj;
			m_pEffect->SetMatrix("WVP", &mAll);
			// 分割数の設定 (枚数ごとに決める)
			m_pEffect->SetFloat("fDivision", 1.0f / 50);
			m_pEffect->Begin(NULL, 0);
			m_pEffect->BeginPass(0);
		}
	}
	*/

	// インデックスバッファをデータストリームを設定
	pDevice->SetIndices(
		m_pIndex);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexType));

	/*
	if (m_pEffect != NULL)
	{
		//ワールド行列の逆行列の転置行列を渡す
		D3DXMATRIX mWIT;
		m_pEffect->SetMatrix("WIT", D3DXMatrixTranspose(&mWIT, D3DXMatrixInverse(&mWIT, NULL, &m_mtxWorld)));

		//ライトの方向ベクトルを渡す
		CLight *pLight = CManager::GetRenderer()->GetLight();
		D3DLIGHT9 Light = pLight->GetLight(CLight::TYPE_MAIN);
		D3DXVECTOR4 direction;
		direction = D3DXVECTOR4(Light.Direction.x, Light.Direction.y, Light.Direction.z, 0.0f);
		m_pEffect->SetVector("LightDir", &direction);

		//入射光（ライト）の強度を渡す　目一杯明るい白色光にしてみる
		m_pEffect->SetVector("LightIntensity", &D3DXVECTOR4(1, 1, 1, 1));

		//メッシュのディフューズ色を渡す
		m_pEffect->SetValue("Diffuse", (LPVOID)&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16);

		//メッシュのディフューズ色を渡す
		m_pEffect->SetValue("Ambient", (LPVOID)&D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), 16);

		// テクスチャ情報を渡す
		m_pEffect->SetTexture("texDecal", CTexture_manager::GetTexture(m_nTexType));
		m_pEffect->SetTexture("texMask", m_pMaskTex);

		m_pEffect->SetFloat("uv_left", 0.0f);
		m_pEffect->SetFloat("uv_top", 0.00f);
		m_pEffect->SetFloat("uv_width", 1.0f);
		m_pEffect->SetFloat("uv_height", 1.0f);
	}
	*/

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumberVertex,
		0,
		m_nNumPolygon);

	/*
	if (m_bCalculation)
	{
		// シェーダー終了
		if (m_pEffect != NULL)
		{
			m_pEffect->EndPass();
			m_pEffect->End();
		}
	}
	*/
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CFloor::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// 頂点移動
// ----------------------------------------
void CFloor::vertexMove(FILE *pFile)
{
	VERTEX_3D *pVtx;
	char cReadText[128];															// 文字
	char cHeadText[128];															// 比較

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < m_nBlock_Depth + 1; nDepth++)
	{
		for (int nWide = 0; nWide < m_nBlock_Width + 1; nWide++)
		{
			if (pFile != NULL)
			{

				fgets(cReadText, sizeof(cReadText), pFile);							// データから1行取得
				sscanf(cReadText, "%s", &cHeadText);								// 行を読み込む

				std::string Data = cReadText;
				std::vector<std::string> vsvec_Contens;		// テキストデータ格納用

				vsvec_Contens = CCalculation::split(Data, ',');

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3((float)atof(vsvec_Contens[0].c_str()), (float)atof(vsvec_Contens[1].c_str()), (float)atof(vsvec_Contens[2].c_str()));			// データを当てはめる

				D3DXCOLOR col = D3DXCOLOR((float)atof(vsvec_Contens[3].c_str()), (float)atof(vsvec_Contens[4].c_str()), (float)atof(vsvec_Contens[5].c_str()), (float)atof(vsvec_Contens[6].c_str()));

				//頂点カラー
				pVtx[0].col = col;	// データを当てはめる
			}

			pVtx++;
		}
	}

	//頂点データのアンロック
	m_pVtxBuff->Unlock();

	// 法線の再計算
	CalculationNormalize();
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CFloor::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CFloor::UnLoad(void)
{
}

// ----------------------------------------
// 生成
//	pos			: 位置
//	size		: サイズ
//	col			: 色
//	rot			: 回転
//	nWidth		: 横ブロック
//	nDepth		: 縦ブロック
//	nTexType	: テクスチャータイプ
// ----------------------------------------
CFloor * CFloor::Create(
	D3DXVECTOR3 const &pos,		// 位置
	D3DXVECTOR3 const & size,	// サイズ
	D3DXCOLOR const & col,		// 色
	D3DXVECTOR3 const & rot,	// 回転
	int const & nWidth,			// 横ブロック
	int const & nDepth,			// 縦ブロック
	int const & nTexType		// テクスチャータイプ
)
{
	// 変数宣言
	CFloor * pFloor;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pFloor = new CFloor();
	// シーン管理設定
	pFloor->ManageSetting(CScene::LAYER_3DOBJECT);
	// 位置情報
	pFloor->m_pos = pos;
	// サイズ情報
	pFloor->m_size = size;
	// 色情報
	pFloor->m_col = col;
	// 回転情報
	pFloor->m_rot = rot;
	// 横ブロック
	pFloor->m_nBlock_Width = nWidth;
	// 縦ブロック
	pFloor->m_nBlock_Depth = nDepth;
	// テクスチャータイプ
	pFloor->m_nTexType = nTexType;
	// 初期化処理
	pFloor->Init();
	// 生成したオブジェクトを返す
	return pFloor;
}

// ----------------------------------------
// 接している面の高さを取得
// ----------------------------------------
float CFloor::GetHeight(D3DXVECTOR3 &pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;					// 頂点情報へのポイント
	D3DXVECTOR3 VecA, VecB,VecObject;	// ベクトル
	D3DXVECTOR3 Normal;					// 法線
	float fPos_Y = 0;					// 高さ
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
				fPos_Y = pVtx[1].pos.y;
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
				return fPos_Y + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// 左下のポリゴン
			else if (CCalculation::CrossCollision(&pos, &pVtx[m_nBlock_Width + 2].pos, &pVtx[m_nBlock_Width + 1].pos, &pVtx[0].pos))
			{
				// ベクトル
				VecA = pVtx[0].pos - pVtx[m_nBlock_Width + 1].pos;						// ベクトルA
				VecB = pVtx[m_nBlock_Width + 2].pos - pVtx[m_nBlock_Width + 1].pos;		// ベクトルB
				VecObject = pos - (m_pos + pVtx[m_nBlock_Width + 1].pos);				// オブジェクトまでのベクトル
				// 高さの代入
				fPos_Y = pVtx[m_nBlock_Width + 1].pos.y;
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
				return fPos_Y + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
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

// ----------------------------------------
// サイズ設定
// ----------------------------------------
void CFloor::SetSize(D3DXVECTOR3 const & size)
{
	// 変数宣言
	VERTEX_3D * pVtx;						// 3D頂点情報
	// 設定
	m_size = size;							// サイズ
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
			// 頂点座標の設定
			pVtx[0].pos =
				D3DXVECTOR3(
					m_OriginBlock.x +
					(m_size.x * nCountWidth),
					m_OriginBlock.y,
					m_OriginBlock.z -
					(m_size.z * nCountDirect));

			// ポイント合わせ
			pVtx++;
		}
	}
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 色設定
// ----------------------------------------
void CFloor::SetCol(D3DXCOLOR const & col)
{
	// 変数宣言
	VERTEX_3D * pVtx;						// 3D頂点情報
											// 設定
	m_col = col;							// 色
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
			// カラーの設定
			pVtx[0].col = m_col;

			// ポイント合わせ
			pVtx++;
		}
	}
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 面の法線を求める
// ----------------------------------------
void CFloor::CalculationNormalize(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;						// 頂点情報へのポイント
	int nCntBlock = 0;						// ブロックカウント
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 *pCross;					// ポリゴンの外積

	pCross =								// メモリ確保
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	// 頂点法線の設定
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++)
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

	//頂点データのアンロック
	m_pVtxBuff->Unlock();
	delete[] pCross;
	pCross = NULL;
}