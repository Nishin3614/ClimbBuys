//=============================================================================
//
// シャドウマッピング処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "shadow_mapping.h"
#include "light.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ----------------------------------------
// 影の描画(モデル)処理
// ----------------------------------------
void CShadowmapping::Draw(
	LPDIRECT3DDEVICE9	&pDevice,	// デバイス情報
	CScene_X::MODEL_LOAD *model,	// モデル情報
	D3DXMATRIX			&mtx		// マトリックス情報
)
{
	// 変数宣言
	D3DMATERIAL9		matDef;					// マテリアルデータのポインタ
	D3DXMATRIX			mtxShadow;				// シャドウマトリックス
	D3DXPLANE			planeField;				// 平面
	D3DXVECTOR4			vecLight;				// ライトの方向
	D3DXVECTOR3			pos, normal;			// 位置・法線
	CLight				*pLight;				// ライトの情報
	// 情報取得
	pLight = CManager::GetRenderer()->GetLight();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);
	// ライトの方向設定
	vecLight = D3DXVECTOR4(
		-pLight->GetVec(CLight::TYPE_MAIN).x,
		-pLight->GetVec(CLight::TYPE_MAIN).y,
		-pLight->GetVec(CLight::TYPE_MAIN).z,
		0.0f
	);
	// 位置
	pos = D3DXVECTOR3(0, 1.0f, 0);
	// 法線
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 描画する平面の位置を決める
	D3DXPlaneFromPointNormal(
		&planeField,			// 平面
		&pos,					// 位置
		&normal					// 法線
	);
	// 影を平面にする
	D3DXMatrixShadow(
		&mtxShadow,				// シャドウマトリックス
		&vecLight,				// ライト方向
		&planeField				// 平面
	);

	// 行列の積(1:影行列 = 2:シャドウマッピング行列 * 3:影行列)
	D3DXMatrixMultiply(&mtxShadow,	// 1
		&mtx,						// 2
		&mtxShadow					// 3
	);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);
	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)model->nNumMat; nCntMat++)
	{
		// 影の色
		matDef.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		// マテリアルの設定
		pDevice->SetMaterial(&matDef);
		// 描画
		model->pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

// ----------------------------------------
// 影の描画(メッシュ)処理
// ----------------------------------------
void CShadowmapping::Draw(
	LPDIRECT3DDEVICE9	&pDevice,		// デバイス情報
	D3DXMATRIX			&mtx,			// マトリックス情報
	LPDIRECT3DINDEXBUFFER9	&pIndex,	// インデックス情報
	int	&nNumberVertex,					// インデックス情報
	int	&nNumPolygon					// インデックス情報
)
{
	// 変数宣言
	D3DMATERIAL9		matDef;					// マテリアルデータのポインタ
	D3DXMATRIX			mtxShadow;				// シャドウマトリックス
	D3DXPLANE			planeField;				// 平面
	D3DXVECTOR4			vecLight;				// ライトの方向
	D3DXVECTOR3			pos, normal;			// 位置・法線
	CLight				*pLight;				// ライトの情報
												// 情報取得
	pLight = CManager::GetRenderer()->GetLight();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);
	// ライトの方向設定
	vecLight = D3DXVECTOR4(
		-pLight->GetVec(CLight::TYPE_MAIN).x,
		-pLight->GetVec(CLight::TYPE_MAIN).y,
		-pLight->GetVec(CLight::TYPE_MAIN).z,
		0.0f
	);
	// 位置
	pos = D3DXVECTOR3(0, 1.0f, 0);
	// 法線
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 描画する平面の位置を決める
	D3DXPlaneFromPointNormal(
		&planeField,			// 平面
		&pos,					// 位置
		&normal					// 法線
	);
	// 影を平面にする
	D3DXMatrixShadow(
		&mtxShadow,				// シャドウマトリックス
		&vecLight,				// ライト方向
		&planeField				// 平面
	);

	// 行列の積(1:影行列 = 2:シャドウマッピング行列 * 3:影行列)
	D3DXMatrixMultiply(&mtxShadow,	// 1
		&mtx,						// 2
		&mtxShadow					// 3
	);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);
	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	// インデックスバッファをデータストリームを設定
	pDevice->SetIndices(
		pIndex);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		nNumberVertex,
		0,
		nNumPolygon);
}
