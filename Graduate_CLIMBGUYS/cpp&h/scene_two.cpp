// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// シーン処理の説明[scene_two.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_two.h"
#include "manager.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// イニシャライザコンストラクタ
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO::CScene_TWO() : CScene::CScene()
{
	// 変数の初期化
	m_pos = D3DVECTOR3_ZERO;
	m_size = D3DVECTOR2_ZERO;
	m_col = D3DXCOLOR_INI;
	m_rot = 0;
	m_fLengh = 0;
	m_fAngle = 0;
	m_offsetType = OFFSET_TYPE_CENTER;
	m_unipAnim = NULL;
	m_nTexId = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO::~CScene_TWO()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Init(void)
{
	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点座標を設定
	Set_Vtx_Pos(m_offsetType,pVtx);
	// 頂点の大きさ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー
	Set_Vtx_Col(pVtx);
	// 頂点テクスチャー
	Set_Vtx_Tex(D3DVECTOR2_ZERO, D3DVECTOR2_INI, pVtx);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Uninit(void)
{
	// アニメーション情報の開放
	if (m_unipAnim != NULL)
	{
		m_unipAnim.reset();
	}
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
 		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Update(void)
{
	// アニメーション更新処理
	Updata_Animation();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CTexture_manager::GetTexture(m_nTexId));

	// Zテストを無効にする
	CManager::GetRenderer()->SetType(CRenderer::TYPE_ZTEST_OFF);

	// ポリゴン描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	// Zテストを通常に戻す
	CManager::GetRenderer()->SetType(CRenderer::TYPE_ZTEST_DEFAULT);
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Debug(void)
{
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン継承あり)処理
//	offsettype	: タイプ
//	pos			: 位置
//	size		: サイズ
//	nTextype	: テクスチャータイプ
//	frot		: 角度
//	col			: 色
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO * CScene_TWO::Create(
	OFFSET_TYPE	const & type,		// タイプ
	D3DXVECTOR3 const & pos,		// 位置
	D3DXVECTOR2 const & size,		// サイズ
	int			const & nTextype,	// テクスチャータイプ
	float		const & frot,		// 角度
	D3DXCOLOR	const & col,		// 色
	CScene::LAYER	const & layer	// レイヤー
)
{
	// 変数宣言
	CScene_TWO * pScene_Two;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Two = new CScene_TWO();
	// 設定
	// シーン管理設定
	pScene_Two->ManageSetting(layer);
	pScene_Two->m_offsetType = type;	// タイプ
	pScene_Two->m_pos = pos;			// 位置
	pScene_Two->m_size = size;			// サイズ
	pScene_Two->m_nTexId = nTextype;	// テクスチャータイプ
	pScene_Two->m_rot = frot;			// 角度
	pScene_Two->m_col = col;			// カラー
	// 初期化処理
	pScene_Two->Init();
	// 生成したオブジェクトを返す
	return pScene_Two;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン継承なし)処理
//	offsettype	: タイプ
//	pos			: 位置
//	size		: サイズ
//	nTextype	: テクスチャータイプ
//	frot		: 角度
//	col			: 色
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO * CScene_TWO::Create_Self(
	OFFSET_TYPE	const & type,		// タイプ
	D3DXVECTOR3 const & pos,		// 位置
	D3DXVECTOR2 const & size,		// サイズ
	int			const & nTextype,	// テクスチャータイプ
	float		const & frot,		// 角度
	D3DXCOLOR	const & col			// 色
)
{
	// 変数宣言
	CScene_TWO * pScene_Two;		// シーン2Dクラス
									// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Two = new CScene_TWO;
	// 設定
	pScene_Two->m_offsetType = type;	// タイプ
	pScene_Two->m_pos = pos;			// 位置
	pScene_Two->m_size = size;			// サイズ
	pScene_Two->m_nTexId = nTextype;	// テクスチャータイプ
	pScene_Two->m_rot = frot;			// 角度
	pScene_Two->m_col = col;			// カラー
	// 初期化処理
	pScene_Two->Init();
	// 生成したオブジェクトを返す
	return pScene_Two;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン継承なし、std::unique_ptr)処理
// ※戻り値はstd::moveで受け取る
//	offsettype	: タイプ
//	pos			: 位置
//	size		: サイズ
//	nTextype	: テクスチャータイプ
//	frot		: 角度
//	col			: 色
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CScene_TWO> CScene_TWO::Creat_Unique(
	OFFSET_TYPE	const & type,		// タイプ
	D3DXVECTOR3 const & pos,		// 位置
	D3DXVECTOR2 const & size,		// サイズ
	int			const & nTextype,	// テクスチャータイプ
	float		const & frot,		// 角度
	D3DXCOLOR	const & col			// 色
)
{
	// 変数宣言
	std::unique_ptr<CScene_TWO> pScene_Two(new CScene_TWO);		// シーン2Dクラス
	// 設定
	pScene_Two->m_offsetType = type;	// タイプ
	pScene_Two->m_pos = pos;			// 位置
	pScene_Two->m_size = size;			// サイズ
	pScene_Two->m_nTexId = nTextype;	// テクスチャータイプ
	pScene_Two->m_rot = frot;			// 角度
	pScene_Two->m_col = col;			// カラー
										// 初期化処理
	pScene_Two->Init();
	// 生成したオブジェクトを返す
	return pScene_Two;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// アニメーション更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Updata_Animation(void)
{
	// アニメーション情報がNULLなら
	// ->関数を抜ける
	if (m_unipAnim == NULL) return;

	// カウントアニメアップ
	m_unipAnim->nCntAnim++;
	// カウントアニメが最大カウントアニメで割り切れたら
	if (m_unipAnim->nCntAnim % m_unipAnim->nMaxCntAnim == 0)
	{
		// 変数宣言
		D3DXVECTOR2 FirstPos;	// 初期位置
		D3DXVECTOR2 EndPos;		// 末期位置
		// 水平アニメーションカウントアップ
		m_unipAnim->nHorizonAnim++;
		if (m_unipAnim->nHorizonAnim >= m_unipAnim->nMaxHorizonAnim)
		{
			// 水平アニメーションカウント初期化
			m_unipAnim->nHorizonAnim = 0;
			// 垂直アニメーションカウントアップ
			m_unipAnim->nVirticalAnim++;
			if (m_unipAnim->nVirticalAnim >= m_unipAnim->nMaxVirticalAnim)
			{
				// 垂直アニメーションカウント初期化
				m_unipAnim->nVirticalAnim = 0;
			}
		}
		// 始点位置
		FirstPos = D3DXVECTOR2(
			m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize,
			m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize
		);
		// 終点位置
		EndPos = D3DXVECTOR2(
			m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize + m_unipAnim->fHorizonSize,
			m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize + m_unipAnim->fVirticalSize
		);
		// テクスチャー座標設定
		Set_Vtx_Tex(FirstPos, EndPos);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オフセットセンター処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Offset_Center(VERTEX_2D * pVtx)
{
	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((m_size.x * 0.5f) * (m_size.x * 0.5f) + (m_size.y * 0.5f) * (m_size.y * 0.5f));
	m_fAngle = atan2f((m_size.x), (m_size.y));

	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オフセット左処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Offset_Left(VERTEX_2D * pVtx)
{
	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((m_size.x) * (m_size.x) + (m_size.y * 0.5f) * (m_size.y * 0.5f));
	m_fAngle = atan2f((m_size.x), (m_size.y * 0.5f));

	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(
		sinf(m_rot - D3DX_PI) * m_size.y * 0.5f,
		cosf(m_rot - D3DX_PI) * m_size.y * 0.5f, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(
		sinf(D3DX_PI - m_fAngle + m_rot) * m_fLengh,
		cosf(D3DX_PI - m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(
		sinf(m_rot) * m_size.y * 0.5f,
		cosf(m_rot) * m_size.y * 0.5f, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(
		sinf(m_fAngle + m_rot) * m_fLengh,
		cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点座標設定
//	offsettype	: オフセットタイプ
//	pVtx		: 頂点情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Pos(
	OFFSET_TYPE const & offsettype,
	VERTEX_2D * pVtx
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
		// オフセットタイプ
		switch (offsettype)
		{
			// センター
		case OFFSET_TYPE_CENTER:
			Offset_Center(pVtx);
			break;
			// 左
		case OFFSET_TYPE_LEFT:
			Offset_Left(pVtx);
			break;
		default:
			break;
		}
		// アンロック
		m_pVtxBuff->Unlock();
	}
	else
	{
		// オフセットタイプ
		switch (offsettype)
		{
			// センター
		case OFFSET_TYPE_CENTER:
			Offset_Center(pVtx);
			break;
			// 左
		case OFFSET_TYPE_LEFT:
			Offset_Left(pVtx);
			break;
		default:
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点カラー設定
//	pVtx		: 頂点情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Col(VERTEX_2D * pVtx)
{
	// 頂点情報がNULLなら
	if (pVtx == NULL)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
		// 頂点カラー
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;
		// アンロック
		m_pVtxBuff->Unlock();
	}
	// それ以外なら
	else
	{
		// 頂点カラー
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点カラー設定(右の色と左の色)
//	col_r		: 右の色
//	col_l		: 左の色
//	pVtx		: 頂点情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Col2_RL(
	D3DXCOLOR const &col_r,		// 右の色
	D3DXCOLOR const &col_l,		// 左の色
	VERTEX_2D * pVtx			// 2D頂点情報
)
{
	// 頂点情報がNULLなら
	if (pVtx == NULL)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
		// 頂点カラー
		pVtx[0].col = col_l;
		pVtx[1].col = col_r;
		pVtx[2].col = col_l;
		pVtx[3].col = col_r;
		// アンロック
		m_pVtxBuff->Unlock();
	}
	// それ以外なら
	else
	{
		// 頂点カラー
		pVtx[0].col = col_l;
		pVtx[1].col = col_r;
		pVtx[2].col = col_l;
		pVtx[3].col = col_r;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点カラー設定(上の色と下の色)
//	col_o		: 上の色
//	col_u		: 下の色
//	pVtx		: 頂点情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Col2_UD(
	D3DXCOLOR const & col_o,
	D3DXCOLOR const & col_u,
	VERTEX_2D * pVtx
)
{
	// 頂点情報がNULLなら
	if (pVtx == NULL)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		m_pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);

		// 頂点カラー
		pVtx[0].col = col_o;
		pVtx[1].col = col_o;
		pVtx[2].col = col_u;
		pVtx[3].col = col_u;
		// アンロック
		m_pVtxBuff->Unlock();
	}
	// それ以外なら
	else
	{
		// 頂点カラー
		pVtx[0].col = col_o;
		pVtx[1].col = col_o;
		pVtx[2].col = col_u;
		pVtx[3].col = col_u;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// テクスチャー設定
//	first		: 初期の位置
//	last		: 末期の位置
//	pVtx		: 頂点情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::Set_Vtx_Tex(
	D3DXVECTOR2 const &first,	// 初期の配置
	D3DXVECTOR2 const &last,	// 最後の配置
	VERTEX_2D * pVtx			// 頂点情報
)
{
	// 頂点情報がNULLなら
	if (pVtx == NULL)
	{
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
		// 頂点情報がヌルだったら
			// アンロック
		m_pVtxBuff->Unlock();
	}
	// それ以外なら
	else
	{
		// 頂点テクスチャー
		pVtx[0].tex = D3DXVECTOR2(first.x, first.y);
		pVtx[1].tex = D3DXVECTOR2(last.x, first.y);
		pVtx[2].tex = D3DXVECTOR2(first.x, last.y);
		pVtx[3].tex = D3DXVECTOR2(last.x, last.y);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// テクスチャーアニメーションの設定
//	nMaxCntAnim			: 最大アニメーションカウント
//	nMaxHorizonAnim		: 最大水平アニメーションカウント
//	nMaxVirticalAnim	: 最大垂直アニメーションカウント
//	bLoop				: ループ状態
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_TWO::SetTexAnim(
	int	const &nMaxCntAnim,			// 最大アニメーションカウント
	int	const &nMaxHorizonAnim,		// 最大水平アニメーションカウント
	int	const &nMaxVirticalAnim,	// 最大垂直アニメーションカウント
	bool const &bLoop				// ループ状態
)
{
	// アニメーション情報がNULLなら
	if (m_unipAnim == NULL)
	{
		// アニメーション情報のメモリ確保
		m_unipAnim.reset(new ANIMATION);
	}
	// 変数宣言
	D3DXVECTOR2 FirstPos;
	D3DXVECTOR2 EndPos;
	// 各項目の設定
	m_unipAnim->nMaxCntAnim = nMaxCntAnim;					// 最大アニメーションカウント
	m_unipAnim->nMaxHorizonAnim = nMaxHorizonAnim;			// 最大水平アニメーションカウント
	m_unipAnim->nMaxVirticalAnim = nMaxVirticalAnim;		// 最大垂直アニメーションカウント
	m_unipAnim->fHorizonSize = 1.0f / nMaxHorizonAnim;		// 垂直アニメーションの1つのサイズ
	m_unipAnim->fVirticalSize = 1.0f / nMaxVirticalAnim;	// 垂直アニメーションの1つのサイズ
	m_unipAnim->bLoop = bLoop;								// ループ状態
	// 始点位置
	FirstPos = D3DXVECTOR2(
		m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize,
		m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize
	);
	// 終点位置
	EndPos = D3DXVECTOR2(
		m_unipAnim->nHorizonAnim * m_unipAnim->fHorizonSize + m_unipAnim->fHorizonSize,
		m_unipAnim->nVirticalAnim * m_unipAnim->fVirticalSize + m_unipAnim->fVirticalSize
	);
	// テクスチャー座標設定
	Set_Vtx_Tex(FirstPos, EndPos);
}