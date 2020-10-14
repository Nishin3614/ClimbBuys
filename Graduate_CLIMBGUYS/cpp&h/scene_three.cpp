// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3Dシーン処理の説明[scene_three.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_three.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE::CScene_THREE() : CScene::CScene()
{
	// 変数の初期化
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR_INI;
	m_fLengh = 0;
	m_fAngle = 0;
	m_bBillboard = false;
	m_bZEneble = false;
	m_bLighting = false;
	m_bAlpha = false;
	m_offsetType = OFFSET_TYPE_VERTICAL_CENTER;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE::~CScene_THREE()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Init(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross0, Cross1_2, Cross3;	// 外積
	int nCntBlock = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
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

	// 頂点座標の設定
	Set_Vtx_Pos(m_offsetType,pVtx);

	// ベクトル
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pVtx[2].pos - pVtx[0].pos;
	// 外積計算
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[2].pos - pVtx[3].pos;
	VecB = pVtx[1].pos - pVtx[3].pos;
	// 外積計算
	Cross3 = CCalculation::Cross_product(VecA, VecB);
	// 正規化
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross3, &Cross3);
	// 外積計算
	Cross1_2 = (Cross0 + Cross3) / 2;

	pVtx[0].nor = Cross0;
	pVtx[1].nor = Cross1_2;
	pVtx[2].nor = Cross1_2;
	pVtx[3].nor = Cross3;

	// 頂点カラーの設定
	Set_Vtx_Col(pVtx);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
 		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Update(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 回転・位置・ビュー行列の計算用変数
	D3DXMATRIX mtxRot, mtxTrans, mtxView;

	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビルボード設定
	if (m_bBillboard)
	{
		// マトリックスビューの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		// ビルボード
		CCalculation::SetBillboard(
			&m_mtxWorld,
			&mtxView
		);
	}
	// Z軸の設定
	if (m_bZEneble)
	{
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);		// アルファテスト有効
	}
	// ライティング設定
	if (m_bLighting)
	{
		// ライティングモード無効
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
	// αブレンド設定
	if (m_bAlpha)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
		pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// アルファ値の基準設定(基準より大きいと描画する)
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)
	}

	// 回転を反映 //
	// ヨーピッチロールによる回転行列作成(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
		m_rot.y, m_rot.x, m_rot.z);	// それぞれの回転量

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxRot);					// 3


	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
		m_pos.x, m_pos.y, m_pos.z);	// それぞれの移動量

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexType)
	);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
		);
	CManager::GetRenderer()->SetType(CRenderer::TYPE_NORMAL);
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CScene_THREE::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// シーン管理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE * CScene_THREE::Create(
	OFFSET_TYPE	const & type,			// タイプ
	D3DXVECTOR3 const & pos,			// 位置
	D3DXVECTOR3 const & size,			// サイズ
	int			const & nTexType,		// テクスチャータイプ
	D3DXVECTOR3	const & rot,			// 角度
	bool		const & bBillboard,		// ビルボード
	bool		const & bZEneble,		// Zバッファ
	bool		const & bLighting,		// ライティング
	bool		const & bAlpha,			// アルファブレンド
	D3DXCOLOR	const & col				// 色
)
{
	// 変数宣言
	CScene_THREE * pScene_Three;		// シーン3Dクラス

	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Three = new CScene_THREE();
	// シーン管理設定
	pScene_Three->ManageSetting(CScene::LAYER_3DOBJECT);
	// 設定
	pScene_Three->m_offsetType = type;			// タイプ
	pScene_Three->m_pos = pos;					// 位置
	pScene_Three->m_size = size;				// サイズ
	pScene_Three->m_rot = rot;					// 角度
	pScene_Three->m_col = col;					// カラー
	pScene_Three->m_nTexType = nTexType;		// テクスチャータイプ
	pScene_Three->m_bBillboard = bBillboard;	// ビルボード
	pScene_Three->m_bZEneble = bZEneble;		// Zバッファ
	pScene_Three->m_bLighting = bLighting;		// ライティング
	pScene_Three->m_bAlpha = bAlpha;			// アルファブレンド
	// 初期化処理
	pScene_Three->Init();

	// 生成したオブジェクトを返す
	return pScene_Three;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 個人管理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_THREE * CScene_THREE::Create_Self(
	OFFSET_TYPE	const & type,			// タイプ
	D3DXVECTOR3 const & pos,			// 位置
	D3DXVECTOR3 const & size,			// サイズ
	int			const & nTexType,		// テクスチャータイプ
	D3DXVECTOR3	const & rot,			// 角度
	bool		const & bBillboard,		// ビルボード
	bool		const & bZEneble,		// Zバッファ
	bool		const & bLighting,		// ライティング
	bool		const & bAlpha,			// アルファブレンド
	D3DXCOLOR	const & col				// 色
)
{
	// 変数宣言
	CScene_THREE * pScene_Three;		// シーン3Dクラス

	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Three = new CScene_THREE();
	// 設定
	pScene_Three->m_offsetType = type;			// タイプ
	pScene_Three->m_pos = pos;					// 位置
	pScene_Three->m_size = size;				// サイズ
	pScene_Three->m_rot = rot;					// 角度
	pScene_Three->m_col = col;					// カラー
	pScene_Three->m_nTexType = nTexType;		// テクスチャータイプ
	pScene_Three->m_bBillboard = bBillboard;	// ビルボード
	pScene_Three->m_bZEneble = bZEneble;		// Zバッファ
	pScene_Three->m_bLighting = bLighting;		// ライティング
	pScene_Three->m_bAlpha = bAlpha;			// アルファブレンド
												// 初期化処理
	pScene_Three->Init();

	// 生成したオブジェクトを返す
	return pScene_Three;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 個人管理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CScene_THREE> CScene_THREE::Create_Uni(
	OFFSET_TYPE	const & type,			// タイプ
	D3DXVECTOR3 const & pos,			// 位置
	D3DXVECTOR3 const & size,			// サイズ
	int			const & nTexType,		// テクスチャータイプ
	D3DXVECTOR3	const & rot,			// 角度
	bool		const & bBillboard,		// ビルボード
	bool		const & bZEneble,		// Zバッファ
	bool		const & bLighting,		// ライティング
	bool		const & bAlpha,			// アルファブレンド
	D3DXCOLOR	const & col				// 色
)
{
	// 変数宣言
	std::unique_ptr<CScene_THREE> pScene_Three(new CScene_THREE);		// シーン3Dクラス
	// 設定
	pScene_Three->m_offsetType = type;			// タイプ
	pScene_Three->m_pos = pos;					// 位置
	pScene_Three->m_size = size;				// サイズ
	pScene_Three->m_rot = rot;					// 角度
	pScene_Three->m_col = col;					// カラー
	pScene_Three->m_nTexType = nTexType;		// テクスチャータイプ
	pScene_Three->m_bBillboard = bBillboard;	// ビルボード
	pScene_Three->m_bZEneble = bZEneble;		// Zバッファ
	pScene_Three->m_bLighting = bLighting;		// ライティング
	pScene_Three->m_bAlpha = bAlpha;			// アルファブレンド

		// 初期化処理
	pScene_Three->Init();

	// 生成したオブジェクトを返す
	return pScene_Three;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 接している面の高さを取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CScene_THREE::GetHeight(D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;						// 頂点情報へのポイント
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross0, Cross1, Cross2;	// 外積
	D3DXVECTOR3 CrossPolygon;			// ポリゴンの法線
	D3DXVECTOR3 CrossAns;				// 位置の外積

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// ベクトル
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pos - pVtx[0].pos;
	// 外積計算
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[2].pos - pVtx[1].pos;
	VecB = pos - pVtx[1].pos;
	// 外積計算
	Cross1 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[0].pos - pVtx[2].pos;

	VecB = pos - pVtx[2].pos;
	// 外積計算
	Cross2 = CCalculation::Cross_product(VecA, VecB);
	// ポリゴンの法線
	CrossPolygon = pVtx[0].nor;
	VecA = pos - pVtx[0].pos;

	CrossAns = CCalculation::Cross_product(VecA, CrossPolygon);
	// 正規化
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross1, &Cross1);
	D3DXVec3Normalize(&Cross2, &Cross2);
	D3DXVec3Normalize(&CrossAns, &CrossAns);
	if (Cross0.y > 0 && Cross1.y > 0 && Cross2.y > 0)
	{
		// ポリゴンの法線
		D3DXVECTOR3 VecAB, VecBC;
		VecA = pos - (m_pos + pVtx[0].pos);
		//D3DXVec3Normalize(&VecA, &VecA);

		float Dot = 0;				// 内積
		float fY = 0;				// 高さ
		// 内積
		Dot = CCalculation::Dot_product(VecA, CrossPolygon);
		// ポリゴンの指定位置の高さ計算
		CrossPolygon = pVtx[0].nor;
		fY = pVtx[0].pos.y + ((-CrossPolygon.x * VecA.x) + (-CrossPolygon.z * VecA.z)) / CrossPolygon.y;
		// 頂点データの範囲をアンロック
		m_pVtxBuff->Unlock();
		return fY;
	}
	// 頂点データの範囲をアンロック
	m_pVtxBuff->Unlock();
	return 0.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点座標の設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Set_Vtx_Pos(
	OFFSET_TYPE const & offsettype,	// オフセットタイプ
	VERTEX_3D * pVtx				// 頂点情報
)
{
	// 頂点情報がヌルだったら
	if (pVtx == NULL)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
	}
	// 頂点座標のオフセット選択
	switch (offsettype)
	{
		// 縦のセンター
	case OFFSET_TYPE_VERTICAL_CENTER:
		Offset_Virtical_Center(pVtx);
		break;
		// 縦の左
	case OFFSET_TYPE_VERTICAL_LEFT:
		Offset_Virtical_Left(pVtx);
		break;
		// 縦の下
	case OFFSET_TYPE_VERTICAL_UNDER:
		Offset_Virtical_Under(pVtx);
		break;
		// 横の中心
	case OFFSET_TYPE_SIDE_CENTER:
		Offset_Side_Center(pVtx);
		break;
	default:
		break;
	}
	// 頂点情報がヌルだったら
	if (pVtx == NULL)
	{
		// アンロック
		m_pVtxBuff->Unlock();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点カラー設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Set_Vtx_Col(VERTEX_3D * pVtx)
{
	// 頂点情報がヌルだったら
	if (pVtx == NULL)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
	}
	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// 頂点情報がヌルだったら
	if (pVtx == NULL)
	{
		// アンロック
		m_pVtxBuff->Unlock();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 表示テクスチャー設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::SetTex(
	D3DXVECTOR2 const &first,	// 最短値
	D3DXVECTOR2 const &last		// 最長値
)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点テクスチャー
	pVtx[0].tex = D3DXVECTOR2(first.x, first.y);
	pVtx[1].tex = D3DXVECTOR2(last.x, first.y);
	pVtx[2].tex = D3DXVECTOR2(first.x, last.y);
	pVtx[3].tex = D3DXVECTOR2(last.x, last.y);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オフセット縦のセンター処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Virtical_Center(VERTEX_3D * pVtx)
{
	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f,m_size.y * 0.5f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オフセット縦の左処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Virtical_Left(VERTEX_3D * pVtx)
{
	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_size.y * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, -m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y * 0.5f, 0.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オフセット縦の下処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Virtical_Under(VERTEX_3D * pVtx)
{
	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f,0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, 0.0f);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オフセット横の中心処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::Offset_Side_Center(VERTEX_3D * pVtx)
{
	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x * 0.5f, 0.0f, m_size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x * 0.5f,0.0f , -m_size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x * 0.5f,0.0f, -m_size.z * 0.5f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// アニメーション生成
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_THREE::SetAnimation(float fTexX, float fTexY, float fTexY2, int nPatternAnim)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY2);
	pVtx[1].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY2);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * fTexX, fTexY);
	pVtx[3].tex = D3DXVECTOR2(fTexX + nPatternAnim * fTexX, fTexY);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

