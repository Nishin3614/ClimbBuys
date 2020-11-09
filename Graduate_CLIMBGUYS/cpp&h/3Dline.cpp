// ----------------------------------------
//
// 3D線処理の説明[3Dline.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "3Dline.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG

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

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
C3DLine::C3DLine() : CScene::CScene()
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
C3DLine::~C3DLine()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void C3DLine::Init(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;							// 頂点情報へのポイント
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();

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

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void C3DLine::Uninit(void)
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
void C3DLine::Update(void)
{
#ifdef _DEBUG
#endif // _DEBUG
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void C3DLine::Draw(void)
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
		m_rot.y, m_rot.x, m_rot.z);	// それぞれの回転量

									// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxRot);					// 3


	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	if (m_ppos)
	{
		D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
			m_ppos->x, m_ppos->y, m_ppos->z);	// それぞれの移動量
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
		D3DXMatrixMultiply(&m_mtxWorld,	// 1
			&m_mtxWorld,					// 2
			&mtxTrans);							// 3
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_LINELIST,
		0,
		2
	);
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void C3DLine::SetPos(
	D3DXVECTOR3 &pos1,
	D3DXVECTOR3 &pos2
)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	pVtx[0].pos = pos1;
	pVtx[1].pos = pos2;
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
C3DLine * C3DLine::Create(
	D3DXVECTOR3 *pos,
	D3DXVECTOR3 &rot,
	D3DXVECTOR3 &pos1,
	D3DXVECTOR3 &pos2
)
{
	// 変数宣言
	C3DLine * p3DLine;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	p3DLine = new C3DLine();
	// シーン管理設定
	p3DLine->ManageSetting(CScene::LAYER_3DOBJECT);
	// 設定
	p3DLine->m_ppos = pos;
	p3DLine->m_rot = rot;
	// 初期化処理
	p3DLine->Init();
	// 位置設定
	p3DLine->SetPos(pos1, pos2);
	// 生成したオブジェクトを返す
	return p3DLine;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT C3DLine::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void C3DLine::UnLoad(void)
{
}


#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void C3DLine::Debug(void)
{
}
#endif // _DEBUG
