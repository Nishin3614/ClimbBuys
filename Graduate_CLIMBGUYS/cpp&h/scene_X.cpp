// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Xシーン処理の説明[scene_X.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_X.h"
#include "manager.h"
#include "debugproc.h"
#include "floor.h"
#include "shadow_mapping.h"
#include "scene_three.h"
#include "3Dparticle.h"
#include "camera.h"
#include "stencilshadow.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TEXTURE_FILE "data/LOAD/MODEL/model_manager.csv"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<std::unique_ptr<CScene_X::MODEL_LOAD>>  CScene_X::m_pModelLoad;	// プレイヤー(雷)情報の読み込み用

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// イニシャライザコンストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::CScene_X() : CScene::CScene()
{
	m_pos = D3DVECTOR3_ZERO;				// 位置情報
	m_rot = D3DVECTOR3_ZERO;				// 回転情報
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// サイズ情報
	m_nModelId = 0;							// プレイヤー(雷)番号
	m_fShadowAlpha = 1.0f;					// 影のα値
	m_fModelAlpha = 1.0f;					// プレイヤーのα値
	m_bShadowMap = false;					// シャドウマッピングにするかしないか
	m_pParentMtx = NULL;					// 親マトリックス
	m_pShadow = NULL;						// シャドウ
	m_pModelCol = NULL;						// モデルカラー情報
	m_pStencilshadow = NULL;				// ステンシルシャドウ
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::~CScene_X()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Init(void)
{
	/*
	if (!m_bShadowMap)
	{
		// 長さを計算
		float fLength = CCalculation::DiffPointSquare(
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(m_pModelLoad[m_nModelId]->size.x, m_pModelLoad[m_nModelId]->size.z));
		// 影を生成
		m_pShadow = std::move(CScene_THREE::Create_Uni(
			CScene_THREE::OFFSET_TYPE_VERTICAL_CENTER,
			m_pos,
			D3DXVECTOR3(fLength, 0.0f, fLength),
			1,
			D3DVECTOR3_ZERO,
			false,
			false,
			false,
			false,
			D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fShadowAlpha)
		));
	}
	*/
	// 行列の初期化処理
	D3DXMatrixIdentity(&m_mtxWorld);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Uninit(void)
{
	// まる影がNULLではないなら
	// ->開放
	if (m_pShadow != NULL)
	{
		m_pShadow->Release();
		m_pShadow = NULL;
	}
	// モデルカラー情報がNULLなら
	if (m_pModelCol != NULL)
	{
		// モデルカラー情報のメモリ確保
		delete m_pModelCol;
		m_pModelCol = NULL;
	}
	// ステンシルシャドウのヌルチェック
	// ステンシルシャドウの初期化
	if (m_pStencilshadow != NULL)
	{
		m_pStencilshadow = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Update(void)
{
	// まる影がNULLではないなら
	// ->更新
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPos(D3DXVECTOR3(m_pos.x,0.1f,m_pos.z));
		m_pShadow->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fShadowAlpha));
	}
	// ステンシルシャドウの位置設定
	if (m_pStencilshadow != NULL)
	{
		// 位置取得
		D3DXVECTOR3 pos = m_pos;
		// ステンシルシャドウの位置設定
		m_pStencilshadow->SetPos(pos);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans,mtxSize;		// 計算用マトリックス
	D3DXMATERIAL		*pMat;					// 現在のマテリアル保存
	D3DMATERIAL9		matDef;					// マテリアルデータのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	// スケールを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);

	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// モデルの拡大縮小
	D3DXMatrixScaling(&mtxSize,
		m_size.x,
		m_size.y,
		m_size.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxSize);					// 3

	// 親情報を持っているとき
	// ->自分のマトリックス情報 * 親のマトリックス情報
	if (m_pParentMtx != NULL)
	{
		// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			m_pParentMtx);
	}
	// シャドウマッピングがオンなら
	if (m_bShadowMap)
	{
		// シャドウマッピング
		CShadowmapping::Draw(
			pDevice,	// デバイス情報
			m_pModelLoad[m_nModelId].get(),	// プレイヤー(雷)情報
			m_mtxWorld	// マトリックス情報
		);
	}
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelLoad[m_nModelId]->pBuffMat->GetBufferPointer();
	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)m_pModelLoad[m_nModelId]->nNumMat; nCntMat++, pMat++)
	{
		// モデルカラー情報のNULLチェック
		if (m_pModelCol != NULL)
		{
			// モデルカラーの設定
			pMat->MatD3D.Diffuse = *m_pModelCol;
		}
		else
		{
			pMat->MatD3D.Diffuse = m_pModelLoad[m_nModelId]->vec_Diffuse[nCntMat];
		}
		// プレイヤー(雷)の透明度
		pMat->MatD3D.Diffuse.a = m_fModelAlpha;

		// マテリアルの設定
		pDevice->SetMaterial(&pMat->MatD3D);

		// テクスチャー設定
		pDevice->SetTexture(0, m_pModelLoad[m_nModelId]->vec_pTexture[nCntMat]);

		// 描画
		m_pModelLoad[m_nModelId]->pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Debug(void)
{
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// あたり判定を強制的に削除
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::CollisionDelete(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理(シーン管理)
//	pos			: 位置
//	rot			: 回転
//	nModelId	: モデル番号
//	bShadowMap	: シャドウマッピング状態
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X * CScene_X::Create(
	D3DXVECTOR3 const & pos,	// 位置
	D3DXVECTOR3 const & rot,	// 回転
	int const & nModelId,		// モデル番号
	bool const & bShadowMap		// シャドウマッピング状態
)
{
	// 変数宣言
	CScene_X * pScene_X;		// シーン3Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_X = new CScene_X();
	// シーン管理設定
	pScene_X->ManageSetting(CScene::LAYER_3DOBJECT);
	// 設定
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// 初期化処理
	pScene_X->Init();

	// 生成したオブジェクトを返す
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理(シーン管理)
//	pos			: 位置
//	rot			: 回転
//	nModelId	: モデル番号
//	bShadowMap	: シャドウマッピング状態
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X * CScene_X::Create(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & rot, D3DXVECTOR3 const & size, D3DXCOLOR color, int const & nModelId, bool const & bShadowMap)
{
	// 変数宣言
	CScene_X * pScene_X;		// シーン3Dクラス
								// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_X = new CScene_X();
	// シーン管理設定
	pScene_X->ManageSetting(CScene::LAYER_3DOBJECT);
	// 設定
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_size = size;
	pScene_X->SetModelColor(color);
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// 初期化処理
	pScene_X->Init();

	// 生成したオブジェクトを返す
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理(個人管理)
//	pos			: 位置
//	rot			: 回転
//	nModelId	: モデル番号
//	bShadowMap	: シャドウマッピング状態
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X * CScene_X::Create_Self(
	D3DXVECTOR3 const & pos,	// 位置
	D3DXVECTOR3 const & rot,	// 回転
	int const & nModelId,		// モデル番号
	bool const & bShadowMap		// シャドウマッピング状態
)
{
	// 変数宣言
	CScene_X * pScene_X;		// シーンXクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_X = new CScene_X();
	// 設定
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// 初期化処理
	pScene_X->Init();
	// 生成したオブジェクトを返す
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理(個人管理)
//	pos			: 位置
//	rot			: 回転
//	nModelId	: モデル番号
//	bShadowMap	: シャドウマッピング状態
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CScene_X> CScene_X::Create_Uni(
	D3DXVECTOR3 const & pos,	// 位置
	D3DXVECTOR3 const & rot,	// 回転
	int const & nModelId,		// モデル番号
	bool const & bShadowMap		// シャドウマッピング状態
)
{
	// 変数宣言
	std::unique_ptr<CScene_X> pScene_X(new CScene_X);		// シーンXクラス
	// 設定
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// 初期化処理
	pScene_X->Init();
	// 生成したオブジェクトを返す
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤー(雷)情報の読み込み処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CScene_X::LoadModel(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	std::vector<std::vector<std::string>> vsvec_Contens;		// テキストデータ格納用
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(TEXTURE_FILE, ',');
	// 行ごとに回す
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// 項目ごとに回す
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			// 変数宣言
			std::unique_ptr<MODEL_LOAD> pModelLoad(new MODEL_LOAD);	// プレイヤー(雷)情報の読み込み用
			switch (nCntItem)
			{
				// パス情報
			case 0:
				// Xファイルの読み込み
				if (!D3DXLoadMeshFromX(
					vsvec_Contens[nCntLine][nCntItem].c_str(),
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&pModelLoad->pBuffMat,
					NULL,
					&pModelLoad->nNumMat,
					&pModelLoad->pMesh) == S_OK
					)
				{
					std::string sNum = std::to_string(nCntLine) + vsvec_Contens[nCntLine][nCntItem].c_str();
					CCalculation::Messanger(sNum.c_str());
					break;
				}
				// プレイヤー(雷)情報の設定
				ModelSetting(pModelLoad.get());
				// プレイヤー(雷)情報の格納
				m_pModelLoad.push_back(std::move(pModelLoad));
				break;
			default:
				break;
			}
			// モデル読み込み変数の初期化
			pModelLoad.reset();
		}
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤー(雷)情報の開放処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CScene_X::UnLoadModel(void)
{
	// プレイヤー(雷)情報の開放
	for (int nCntModel = 0; nCntModel < (signed)m_pModelLoad.size(); nCntModel++)
	{
		// メッシュの開放
		if (m_pModelLoad[nCntModel]->pMesh != NULL)
		{
			m_pModelLoad[nCntModel]->pMesh->Release();
			m_pModelLoad[nCntModel]->pMesh = NULL;
		}

		// マテリアルの開放
		if (m_pModelLoad[nCntModel]->pBuffMat != NULL)
		{
			m_pModelLoad[nCntModel]->pBuffMat->Release();
			m_pModelLoad[nCntModel]->pBuffMat = NULL;
		}

		// テクスチャーと元のカラー情報の開放
		for (int nCntTex = 0; nCntTex < (signed)m_pModelLoad[nCntModel]->vec_pTexture.size(); nCntTex++)
		{
			if (m_pModelLoad[nCntModel]->vec_pTexture[nCntTex] != NULL)
			{
				m_pModelLoad[nCntModel]->vec_pTexture[nCntTex]->Release();
				m_pModelLoad[nCntModel]->vec_pTexture[nCntTex] = NULL;
			}
			m_pModelLoad[nCntModel]->vec_pTexture.clear();
			m_pModelLoad[nCntModel]->vec_pTexture.shrink_to_fit();
			m_pModelLoad[nCntModel]->vec_Diffuse.clear();
			m_pModelLoad[nCntModel]->vec_Diffuse.shrink_to_fit();
		}
		// モデル読み込み変数の初期化
		m_pModelLoad[nCntModel].reset();
	}
	m_pModelLoad.clear();
	m_pModelLoad.shrink_to_fit();
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤー(雷)設定処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::ModelSetting(MODEL_LOAD * pModel_load)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイス
	D3DXMATERIAL *pMat;				// 現在のマテリアル保存
	int nNumVertex;					// 頂点数
	DWORD sizeFVF;					// 頂点フォーマットのサイズ
	BYTE *pVertexBuffer;			// 頂点バッファへのポインタ
	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();
	// マテリアルの頂点位置の最大値・最小値
	pModel_load->vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	pModel_load->vtxMaxMaterials = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	// 頂点数を取得
	nNumVertex = pModel_load->pMesh->GetNumVertices();
	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(pModel_load->pMesh->GetFVF());
	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)pModel_load->pBuffMat->GetBufferPointer();
	// マテリアル情報を取得
	for (int nCntMat = 0; nCntMat < (int)pModel_load->nNumMat; nCntMat++, pMat++)
	{
		// テクスチャーの反映
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTexture);
		pModel_load->vec_pTexture.push_back(std::move(pTexture));
		// 元のカラー情報を保存
		D3DCOLORVALUE Diffuse = pMat->MatD3D.Diffuse;
		pModel_load->vec_Diffuse.push_back(Diffuse);
	}

	// 頂点バッファをロック
	pModel_load->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);
	// 頂点数分繰り返す
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		// 番地情報のD3DXVE
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
		// x //
		// 最小値
		if (pModel_load->vtxMinMaterials.x > vtx.x)
		{
			pModel_load->vtxMinMaterials.x = vtx.x;
		}
		// 最大値
		if (pModel_load->vtxMaxMaterials.x < vtx.x)
		{
			pModel_load->vtxMaxMaterials.x = vtx.x;
		}
		// y //
		// 最小値
		if (pModel_load->vtxMinMaterials.y > vtx.y)
		{
			pModel_load->vtxMinMaterials.y = vtx.y;
		}
		// 最大値
		if (pModel_load->vtxMaxMaterials.y < vtx.y)
		{
			pModel_load->vtxMaxMaterials.y = vtx.y;
		}
		// z //
		// 最小値
		if (pModel_load->vtxMinMaterials.z > vtx.z)
		{
			pModel_load->vtxMinMaterials.z = vtx.z;
		}
		// 最大値
		if (pModel_load->vtxMaxMaterials.z < vtx.z)
		{
			pModel_load->vtxMaxMaterials.z = vtx.z;
		}
		// サイズポインタを進める
		pVertexBuffer += sizeFVF;
	}
	// アンロック
	pModel_load->pMesh->UnlockVertexBuffer();
	// サイズ
	pModel_load->size = pModel_load->vtxMaxMaterials - pModel_load->vtxMinMaterials;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ステンシルシャドウの設定
//	Size	: サイズ
//	Type	: タイプ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::SetStencilshadow(
	D3DXVECTOR3 const & Size,			// サイズ
	int const & nType	// タイプ
)
{
	// ステンシルシャドウの生成
	m_pStencilshadow = CStencilshadow::Create(
		m_pos,
		Size,
		(CStencilshadow::TYPE)nType
	);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ステンシルシャドウの使用状態設定
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::SetUseStencillShadow(bool const & bUse)
{
	// ステンシルシャドウのNULLチェック
	if (m_pStencilshadow)
	{
		// 使用状態設定
		m_pStencilshadow->SetUse(bUse);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ステンシルシャドウの使用状態取得
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CScene_X::GetUseStencillShadow(void)
{
	// ステンシルシャドウのNULLチェック
	if (m_pStencilshadow)
	{
		// 使用状態を返す
		return m_pStencilshadow->GetUse();
	}
	// 使用状態を返す
	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルカラー情報の設定
//	col	: カラー
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::SetModelColor(
	D3DXCOLOR const &col
)
{
	// モデルカラー情報がNULLなら
	if (m_pModelCol == NULL)
	{
		// モデルカラー情報のメモリ確保
		m_pModelCol = new D3DXCOLOR;
	}
	// カラーの代入
	*m_pModelCol = col;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Xプレイヤー(雷)取得処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::MODEL_LOAD * CScene_X::GetModel(void)
{
	return m_pModelLoad[m_nModelId].get();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤー(雷)情報取得処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::MODEL_LOAD * CScene_X::GetModelLoad(int const & nModelId)
{
	// プレイヤー(雷)番号がサイズ以上なら
	// ->関数を抜ける
	if ((signed)m_pModelLoad.size() <= nModelId)
	{
		return m_pModelLoad[0].get();
	}
	return m_pModelLoad[nModelId].get();
}