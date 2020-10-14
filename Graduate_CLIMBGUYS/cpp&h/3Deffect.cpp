// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3Dエフェクトの処理[3Deffect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Deffect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9	C3DEffect::m_pVtxBuff = NULL;			// バッファ情報
CEffect::EFFECT			C3DEffect::m_aEffect[EFFECT_MAX] = {};	// エフェクト情報

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DEffect::C3DEffect() :CEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DEffect::~C3DEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;					// デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// エフェクトの初期化
	CEffect::Init();
	// 変数の初期化
	CEffect::InitValues(&m_aEffect[0]);
	// 頂点情報作成
	MakeVertex(pDevice);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Update(void)
{
	// 変数定義
	VERTEX_3D *pVtx;				// 頂点情報のポインタ
	C3DEffect::EFFECT *pEffect;		// エフェクトのポインタ
	// ポインタの初期化
	pEffect = &m_aEffect[0];

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 最大数分ループ
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++, pVtx+=4)
	{
		// 使用フラグがオフの時
		// ->ループスキップ
		if (!pEffect->bUse)
		{
			continue;
		}
		// ライフ更新
		// ->ループスキップ
		if (!CEffect::UpdateLife(pEffect))
		{
			continue;
		}
		// 移動の更新
		CEffect::UpdateMove(pEffect);
		// 頂点情報の更新
		CEffect::UpdateVetex(pEffect);
		// 位置更新
		pEffect->pos += pEffect->move;
		// 頂点サイズの設定
		SetVartexSize(pVtx, pEffect);
		// 頂点カラーの設定
		SetVetexColor(pVtx, pEffect);
	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;					// デバイスのポインタ
	D3DXMATRIX
		mtxRot,										// 回転行列
		mtxTrans,									// 位置行列
		mtxView,									// ビュー行列
		mtxWorld;									// ワールド行列

	C3DEffect::EFFECT *pEffect;						// エフェクトのポインタ

	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	pEffect = &m_aEffect[0];						// ポインタの初期化

	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Zバッファ　有効　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// 合成演算設定
			CManager::GetRenderer()->SetBlend(pEffect->BlendType);
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// マトリックスビューの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			// ビルボード
			CCalculation::SetBillboard(
				&mtxWorld,
				&mtxView
			);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				pEffect->rot.y,
				pEffect->rot.x,
				pEffect->rot.z);
			D3DXMatrixMultiply(
				&mtxWorld,
				&mtxWorld,
				&mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(
				&mtxTrans,
				pEffect->pos.x,
				pEffect->pos.y,
				pEffect->pos.z);
			D3DXMatrixMultiply(
				&mtxWorld,
				&mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 頂点バッファをデータストリームにバインド
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, CTexture_manager::GetTexture(pEffect->nTexType));
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
	//アルファテスト戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Zバッファ　有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点の作成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_3D *pVtx;							// 頂点情報のポインタ

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// エフェクトループ
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pVtx += 4)
	{
		SetVartexSize(pVtx, NULL, &nCntEffect);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// texture座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点サイズの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVartexSize(VERTEX_3D * pVtx, EFFECT * pEffect, const int * nIndex)
{
	if (nIndex != NULL)
	{
		// 変数宣言
		C3DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C3DEffect::m_aEffect[*nIndex];		// ポインタの初期化

		// 頂点の設定
		pVtx[0].pos.x = -sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[0].pos.y = -cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[0].pos.z = -cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[1].pos.x = -sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[1].pos.y = cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[1].pos.z = -cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[2].pos.x = sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[2].pos.y = -cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[2].pos.z = cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[3].pos.x = sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[3].pos.y = cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[3].pos.z = cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		return;
	}
	if (pEffect != NULL)
	{
		// 頂点の設定
		pVtx[0].pos.x = -sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[0].pos.y = -cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
		pVtx[0].pos.z = -cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[1].pos.x = -sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[1].pos.y = cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[1].pos.z = -cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[2].pos.x = sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[2].pos.y = -cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[2].pos.z = cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[3].pos.x = sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[3].pos.y = cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
		pVtx[3].pos.z = cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		return;
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点カラーの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVetexColor(VERTEX_3D * pVtx, EFFECT * pEffect, const int * nIndex)
{
	if (nIndex != NULL)
	{
		// 変数宣言
		C3DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C3DEffect::m_aEffect[*nIndex];		// ポインタの初期化

		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pLocalEffect->col;
	}
	if (pEffect != NULL)
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pEffect->col;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DEffect * C3DEffect::Create(void)
{
	// 変数宣言
	C3DEffect *p3DEffect = new C3DEffect;
	// シーン管理設定
	p3DEffect->ManageSetting(CScene::LAYER_3DPARTICLE);
	// 初期化
	p3DEffect->Init();
	return p3DEffect;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// エフェクトの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Set3DEffect(
	EFFECT_TYPE const &EffectType,
	int const &nTexType,
	D3DXVECTOR3 const &pos,
	D3DXVECTOR3 const &rot,
	D3DXVECTOR3 const &move,
	D3DXCOLOR const &col,
	D3DXVECTOR2 const &size,
	int const &nLife,
	CRenderer::BLEND const &Blend,
	D3DXVECTOR2 const &sizeValue,
	float const &fAlphaValue
)
{
	// 変数宣言
	C3DEffect::EFFECT *pEffect;			// エフェクトのポインタ
	pEffect = &C3DEffect::m_aEffect[0];	// ポインタの初期化

	// 最大数ループ
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		// 使用フラグオフの個体の時
		if (pEffect->bUse == false)
		{
			// 値の初期化処理
			Init_OneValues(pEffect);
			// 変数定義
			VERTEX_3D *pVtx;		// 頂点ポインタ
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点ポインタの更新
			pVtx += nCntEffect * 4;
			// ライフの設定
			pEffect->nLife = nLife;
			// テクスチャ種類の設定
			pEffect->nTexType = nTexType;
			// エフェクト種類の設定
			pEffect->EffectType = EffectType;
			// 移動量の設定
			pEffect->move = move;
			// 半径の設定
			pEffect->size = size;
			// 位置の設定
			pEffect->pos = pos;
			// 回転の設定
			pEffect->rot = rot;
			// ブレンドタイプ設定
			pEffect->BlendType = Blend;
			// 頂点位置の設定
			SetVartexSize(pVtx, pEffect);
			// 色の設定
			pEffect->col = col;
			// 頂点カラーの設定
			SetVetexColor(pVtx, pEffect);
			// アルファ変化値の設定
			pEffect->fAlphaValue = fAlphaValue;
			// サイズ変化値の設定
			pEffect->sizeValue = sizeValue;
			// 使用フラグをオン
			pEffect->bUse = true;
			//頂点データをアンロック
			m_pVtxBuff->Unlock();
			break;
		}
	}

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::Load(void)
{
	// テクスチャファイル名の取得
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 開放
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Unload(void)
{

}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DEffect::Debug(void)
{

}
#endif // _DEBUG
