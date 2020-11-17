// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.cpp]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "XInputPad.h"
#include "keyboard.h"
#include "damagefloor.h"
#include "calculation.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CAMERA_FILE ("data/LOAD/camerainfo.txt")
#define CAMERA_INIT (1.0f)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的変数宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::LOAD	CCamera::m_load[TYPE_MAX] = {};	// 情報保存

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::CCamera()
{
	// カメラの初期化
	m_posV = D3DXVECTOR3(1.0f, 100.0f, -100.0f);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = m_posR;
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_offset = D3DVECTOR3_ZERO;
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_type = TYPE_TITLE;
	m_fLength = 750.0f;
	m_fHeight = 1000.0f;
	m_RotSpeed.x = CAMERA_INIT;				// 回転スピード
	m_RotSpeed.y = -CAMERA_INIT;			// 回転スピード
	m_fIntertia = 1.0f;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	// 360°回転
	m_Turn.nSpin = 2;
	m_Turn.nCntSpin = 0;
	m_Turn.nOneTime = 60;
	m_Turn.nCntTime = 0;
	m_Turn.fTrunRot = D3DX_PI * 2 / m_Turn.nOneTime;
	m_Turn.bSpin = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init(void)
{
	// 初期時のカメラ設定
	InitCamera();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update(void)
{
	// 変数宣言
	CManager::MODE Mode = CManager::GetMode();
	// タイトル
	if (Mode == CManager::MODE_TITLE)
	{
		Update_Title();
	}
	// チュートリアル
	else if (Mode == CManager::MODE_TUTORIAL)
	{
		Update_Tutorial();
	}
	// ゲーム
	else if (Mode == CManager::MODE_GAME)
	{
		Update_Game();
	}
	Update_Play();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイ時の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update_Play(void)
{
	// カメラ回転の限度設定
	CCalculation::CalcRotation_XYZ(m_rot);
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	/*
	// 目的の角度から現在の角度の差
	m_rotDiff =
		m_rotDest - m_rot;
	// 回転上限
	CCalculation::Rot_One_Limit(m_rotDiff.x);
	CCalculation::Rot_One_Limit(m_rotDiff.y);
	CCalculation::Rot_One_Limit(m_rotDiff.z);

	// 角度に慣性
	m_rot +=
		m_rotDiff * m_fIntertia;
	// 回転上限
	CCalculation::Rot_One_Limit(m_rot.x);
	CCalculation::Rot_One_Limit(m_rot.y);
	CCalculation::Rot_One_Limit(m_rot.z);
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オープニング初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init_Opening(void)
{
	// カメラの初期化
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	m_rot.x = -1.5f;
	// 注視点位置
	m_posR = D3DXVECTOR3(0.0f, 500.0f, -500.0f);
	m_fLength = 750.0f;
	m_fHeight = 1000.0f;

	// カメラの振動情報の初期化
	SetShakeInfo();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// チュートリアル初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init_Tutorial(void)
{
	// カメラの初期化
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	m_rot.x = 0.0f;
	// 注視点位置
	m_posR = D3DXVECTOR3(200.0f, 0.0f, -50.0f);
	m_fLength = 420.0f;
	m_fHeight = 700.0f;
	// 一度の回転にかかるタイム
	m_Turn.nOneTime = 10000;
	m_Turn.fTrunRot = D3DX_PI * 2 / m_Turn.nOneTime;
	// 一度の回転にかかるタイムカウント
	m_Turn.nCntTime = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// タイトル初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init_Title(void)
{
	// カメラの初期化
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	m_rot.x = 1.5f;
	// 注視点位置
	m_posR = D3DXVECTOR3(0.0f, 40.0f, -350.0f);
	m_fLength = 750.0f;
	m_fHeight = 1000.0f;
	// 回転回数
	m_Turn.nSpin = 1;
	// 一度の回転にかかるタイム
	m_Turn.nOneTime = 240;
	m_Turn.fTrunRot = D3DX_PI * 2 / m_Turn.nOneTime;
	// 一度の回転にかかるタイムカウント
	m_Turn.nCntTime = 0;
	// スピンカウント
	m_Turn.nCntSpin = 0;
	// 360°回転を再開させる
	m_Turn.bSpin = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ゲーム時の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init_Game(void)
{
	// カメラの初期化
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	m_rot.x = 0.0f;
	// 注視点位置
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 400.0f;
	m_fHeight = 400.0f;
	// 回転回数
	m_Turn.nSpin = 1;
	// 一度の回転にかかるタイム
	m_Turn.nOneTime = 10000;
	m_Turn.fTrunRot = D3DX_PI * 2 / m_Turn.nOneTime;
	// 一度の回転にかかるタイムカウント
	m_Turn.nCntTime = 0;
	// スピンカウント
	m_Turn.nCntSpin = 0;
	// 360°回転を再開させる
	m_Turn.bSpin = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// リザルト時の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init_Result(void)
{
	// カメラの初期化
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	m_rot.x = 0.0f;
	// 注視点位置
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 400.0f;
	m_fHeight = 400.0f;
	// 回転回数
	m_Turn.nSpin = 1;
	// 一度の回転にかかるタイム
	m_Turn.nOneTime = 10000;
	m_Turn.fTrunRot = D3DX_PI * 2 / m_Turn.nOneTime;
	// 一度の回転にかかるタイムカウント
	m_Turn.nCntTime = 0;
	// スピンカウント
	m_Turn.nCntSpin = 0;
	// 360°回転を再開させる
	m_Turn.bSpin = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// チュートリアル更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update_Tutorial(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// タイトル更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update_Title(void)
{
	// 回転状態がfalseなら
	// ->関数を抜ける
	if (!m_Turn.bSpin) return;
	// 回転回数カウントが回転数を超えていたら
	// ->関数を抜ける
	if (m_Turn.nCntSpin >= m_Turn.nSpin)
	{
		m_Turn.bSpin = false;
		InitCamera();
		return;
	}
	// タイムカウントが1週にかかる時間と同じになったら
	if (m_Turn.nCntTime == m_Turn.nOneTime)
	{
		// カウントタイムの初期化
		m_Turn.nCntTime = 0;
		// 回転回数カウントアップ
		m_Turn.nCntSpin++;
		return;
	}
	// カウントタイム
	m_Turn.nCntTime++;
	// 回転更新
	m_rot.y -= m_Turn.fTrunRot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイ時の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update_Game(void)
{
	// ダメージ床との距離が一定値以内かつ、カメラの高さが上限に達していない時、カメラをダメージ床と平行に上昇させる

	for (int nCnt = 0; nCnt < CScene::GetMaxLayer(CScene::LAYER_FLOOR); nCnt++)
	{
		CDamageFloor *pDamageFloor = (CDamageFloor*)CScene::GetScene(CScene::LAYER_FLOOR, nCnt);

		// nullじゃない時、かつカメラの限界の高さじゃなかったら
		if (pDamageFloor && m_posR.y <= 500.0f)
		{
			// ダメージ床との距離が一定値より狭くなったら床と平行に高さを上げる
			if (CCalculation::Range_Absolute(pDamageFloor->GetPos().y, m_posR.y) <= 100)
			{
				m_posR.y += pDamageFloor->GetDamageFloorMoveSpeed()/4;
			}
		}

		// ポインターエラー防止用にnull
		pDamageFloor = nullptr;
	}

	// タイムカウントが1週にかかる時間と同じになったら
	if (m_Turn.nCntTime == m_Turn.nOneTime)
	{
		// カウントタイムの初期化
		m_Turn.nCntTime = 0;
		return;
	}
	// カウントタイム
	m_Turn.nCntTime++;
	// 回転更新
	m_rot.y -= m_Turn.fTrunRot;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期時のカメラ設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::InitCamera(void)
{

	// カメラの初期化
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_bSet = false;
	// 目的の視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	// 360°回転
	m_Turn.nCntSpin = 0;
	m_Turn.nCntTime = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Draw(void)
{
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラのデバッグ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Debug(void)
{
	// カメラの情報
	if (ImGui::CollapsingHeader("Camera"))
	{
		// 注視点位置
		ImGui::DragFloat3("PosR", m_posR);
		// 回転
		ImGui::DragFloat3("ROT", m_rot, 0.01f);
		// 慣性
		ImGui::DragFloat("Intertia", &m_fIntertia, 0.01f,0.01f,1.0f);
		// 回転スピード
		ImGui::DragFloat2("RotSpeed", m_RotSpeed, 0.01f, 0.03f, 1.0f);
		// 長さ
		if (ImGui::TreeNode("LENGH&HEIGHT"))
		{
			ImGui::DragFloat("lengh", &m_fLength);
			ImGui::DragFloat("height", &m_fHeight);
			ImGui::Text("PosV(%.3f,%.3f,%.3f)\n", m_posV.x, m_posV.y, m_posV.z);
			ImGui::TreePop();
		}
		// 回転回数
		ImGui::DragInt("Spin", &m_Turn.nSpin);
		// 一度の回転にかかるタイム
		if (ImGui::DragInt("OneTime", &m_Turn.nOneTime))
		{
			m_Turn.fTrunRot = D3DX_PI * 2 / m_Turn.nOneTime;
		}
		// 一度の回転にかかるタイムカウント
		ImGui::Text("nCntOneTime(%d)", m_Turn.nCntTime);
		// スピンカウント
		ImGui::Text("nCntSpin(%d)", m_Turn.nCntSpin);

		// カメラの初期化処理
		if (ImGui::Button("CameraInit"))
		{
			InitCamera();
		}
		// 360°回転を再開させる
		if (ImGui::Button("SPIN"))
		{
			InitCamera();
			m_Turn.bSpin = !m_Turn.bSpin;
		}
		// 区切り線
		ImGui::Separator();
	}
	/*
	CDebugproc::Print("PosV(%.1f,%.1f,%.1f)\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugproc::Print("m_posVDest(%.1f,%.1f,%.1f)\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	CDebugproc::Print("m_posVDiff(%.1f,%.1f,%.1f)\n", m_posVDiff.x, m_posVDiff.y, m_posVDiff.z);
	*/
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera * CCamera::Create(void)
{
	// 変数宣言
	CCamera * pCamera;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pCamera = new CCamera();
	// 初期化処理
	pCamera->Init();
	// 生成したオブジェクトを返す
	return pCamera;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCamera::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 破棄
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Unload(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 各モードのカメラの初期設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::ModeCameraInit(void)
{
	// 変数宣言
	CManager::MODE Mode = CManager::GetMode();

	switch (Mode)
	{
		// オープニング
	case CManager::MODE_OPENING:
		Init_Opening();
		break;

		// タイトル
	case CManager::MODE_TITLE:
		Init_Title();
		break;

		// チュートリアル
	case CManager::MODE_TUTORIAL:
		Init_Tutorial();
		break;

		// ゲーム
	case CManager::MODE_GAME:
		Init_Game();
		break;
		// リザルト
	case CManager::MODE_RESULT:
		Init_Result();
		break;

	default:
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 情報初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::InfoInit(
	D3DXVECTOR3 const &pos,	// 位置情報
	D3DXVECTOR3 const &rot	// 回転情報
)
{
	// 回転
	m_rot = rot;
	// ゲームのカメラの回転の目的地
	m_posRDest.x = pos.x + sinf(m_rot.y + D3DX_PI) * m_offset.x;
	m_posRDest.y = pos.y + m_offset.y;
	m_posRDest.z = pos.z + cosf(m_rot.y + D3DX_PI) * m_offset.x;
	// 注視点
	m_posR = m_posRDest;
	// 視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	m_posVDest = m_posV;
	m_rotDest = m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// プロジェクションマトリックス //
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,							// (テレビ視点)
		D3DXToRadian(45.0f),						// 視野角(画角)
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,										// MearZ値(近いビュー平面のZ値)
		1500000.0f);								// FarZ値(遠いビュー平面のZ値)初期値は1000

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックス //
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(
		&m_mtxView,	// (カメラマン視点)
		&m_posV,	// カメラの視点
		&m_posR,	// カメラの注目点
		&m_posU);	// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの目的注視点設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetPosDestR(
	D3DXVECTOR3 const & pos,	// 位置情報
	D3DXVECTOR3 const & rot		// 回転情報
)
{
	// 回転設定
	m_rot = rot;
	// ゲームのカメラの回転の目的地
	m_posRDest.x = pos.x + sinf(m_rot.y + D3DX_PI) * m_offset.x;
	m_posRDest.y = pos.y + m_offset.y;
	m_posRDest.z = pos.z + cosf(m_rot.y + D3DX_PI) * m_offset.x;
	//-------------------- 視点移動 --------------------//
	// 目的地から現在地の距離(視点)
	m_posRDiff =
		m_posRDest - m_posR;

	// 注視点に慣性
	m_posR.x += m_posRDiff.x * m_fIntertia;
	m_posR.y += m_posRDiff.y * m_fIntertia;
	m_posR.z += m_posRDiff.z * m_fIntertia;

	// 目的の視点
	m_posVDest.x =
		m_posRDest.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posRDest.y + cosf(m_rot.x) * m_fHeight;
	m_posVDest.z =
		m_posRDest.z + cosf(m_rot.y) * m_fLength;
	// 目的地から現在地の距離(注視点)
	m_posVDiff =
		m_posVDest - m_posV;

	// 注視点に慣性
	m_posV.x += m_posVDiff.x * m_fIntertia;
	m_posV.y += m_posVDiff.y * m_fIntertia;
	m_posV.z += m_posVDiff.z * m_fIntertia;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの注視点設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetPosR(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & rot)
{
	// 回転設定
	m_rot = rot;
	// ゲームのカメラの回転の目的地
	m_posRDest.x = pos.x + sinf(m_rot.y + D3DX_PI) * m_offset.x;
	m_posRDest.y = pos.y + m_offset.y;
	m_posRDest.z = pos.z + cosf(m_rot.y + D3DX_PI) * m_offset.x;
	// ゲームのカメラの回転の目的地
	m_posR = m_posRDest;
	// 目的の視点
	m_posVDest.x =
		m_posRDest.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posRDest.y + cosf(m_rot.x) * m_fHeight;
	m_posVDest.z =
		m_posRDest.z + cosf(m_rot.y) * m_fLength;
	// 注視点に慣性
	m_posV = m_posVDest;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラのタイプ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetType(
	TYPE type
)
{
	if (type > TYPE_MAX ||
		type < 0)
	{
		CCalculation::Messanger("タイプがおかしいです。");
	}
	m_type = type;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの方向
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラスピードの取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR2 & CCamera::GetRotSpeed(void)
{
	return m_RotSpeed;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラ設定の初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetCameraInfo(void)
{
	m_RotSpeed.x = CAMERA_INIT;				// 回転スピード
	m_RotSpeed.y = -CAMERA_INIT;				// 回転スピード
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラモーションの設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetCamera_Motion(
	D3DXVECTOR3 const &posR,	// 注視点の位置
	D3DXVECTOR3 const &rot,		// 回転
	float const & fLength,		// 長さ
	float const & fHeight,		// 高さ
	float const & fIntertia		// 慣性の比率
)
{
	// 注視点
	m_posRDest = posR;
	// 回転
	m_rotDest = rot;
	// 長さ
	m_fLength = fLength;
	// 高さ
	m_fHeight = fHeight;
	// 慣性の比率
	m_fIntertia = fIntertia;
	// 回転上限
	CCalculation::Rot_One_Limit(m_rotDest.x);
	CCalculation::Rot_One_Limit(m_rotDest.y);
	CCalculation::Rot_One_Limit(m_rotDest.z);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの振動
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::CameraShake()
{
	// インターバルの回数分振動を繰り返す
	if (m_Shake.nInterbalCnt > m_Shake.nInterbalCntSave)
	{
		// 右に移動時
		if (m_Shake.nInterbalCntSave % 2 == 0)
		{
			// 最小地点と比較
			if (m_posR.x >= m_Shake.fRange[0])
			{
				m_posR.x -= m_Shake.fShakeSpeed;

				if (m_posR.x <= m_Shake.fRange[0])
				{// 目標地点に到達したらカウント加算
					m_Shake.nInterbalCntSave++;
				}
			}
		}
		// 左に移動時
		else if (m_Shake.nInterbalCntSave % 2 == 1)
		{
			// 最大地点と比較
			if (m_posR.x <= m_Shake.fRange[1])
			{
				m_posR.x += m_Shake.fShakeSpeed;

				if (m_posR.x >= m_Shake.fRange[1])
				{// 目標地点に到達したらカウント加算
					m_Shake.nInterbalCntSave++;
				}
			}
		}

		// カウントを満たしたら元の位置に戻し終了フラグをtrueにする
		if (m_Shake.nInterbalCnt <= m_Shake.nInterbalCntSave)
		{
			m_posR = m_Shake.CameraCurrentPos;
			m_Shake.bEnd = true;
		}
	}

	// 条件を既に満たしていたら即終了
	else
	{
		return;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの振動情報の設定 振動させたいmodeのカメラの初期化でする
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetShakeInfo()
{
	// 振動情報
	m_Shake.nInterbalCnt = 20;
	m_Shake.nInterbalCntSave = 0;
	m_Shake.fDistance = 120.0f;
	m_Shake.fShakeSpeed = 60.0f;

	m_Shake.fRange[0] = m_Shake.fDistance *-1.0f;
	m_Shake.fRange[1] = m_Shake.fDistance;

	m_Shake.CameraCurrentPos = m_posR;

	m_Shake.bEnd = false;
}

/*
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 回転
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Rot(void)
{
	// ジョイパッド用 //
	// パッド用 //
	float fValueH, fValueV;	// ゲームパッドのスティック情報の取得用
	float fRot;			// 移動速度

	// ジョイパッドの処理 //
	if (CManager::GetPad(PLAYER_TAG::PLAYER_1) != NULL)
	{
		// ゲームパッドのスティック情報を取得
		CManager::GetPad(PLAYER_TAG::PLAYER_1)->GetStickRight(&fValueH, &fValueV);
		if (fValueH != 0 || fValueV != 0)
		{
			// y回転の角度を計算
			if (fValueH < 0)
			{
				fRot = abs(fValueH) / 1024.0f * m_RotSpeed.y;
				m_rotDest.y += fRot;

			}
			else if (fValueH > 0)
			{
				fRot = abs(fValueH) / 1024.0f * m_RotSpeed.y;
				m_rotDest.y -= fRot;
			}
			// x回転の角度を計算
			if (fValueV < 0)
			{
				fRot = abs(fValueV) / 1024.0f * m_RotSpeed.x;
				m_rotDest.x += fRot;

			}
			else if (fValueV > 0)
			{
				fRot = abs(fValueV) / 1024.0f * m_RotSpeed.x;
				m_rotDest.x -= fRot;

			}
			// -3.14〜3.14に回転差分を固定させる
			CCalculation::Rot_One_Limit(m_rotDest.y);
			// -3.14〜3.14に回転差分を固定させる
			CCalculation::Rot_One_Limit(m_rotDest.x);
		}
	}
	// 注視点軸に回転 //
	// 半時計回り
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
	{
		// 加算
		m_rotDest.y -= m_RotSpeed.y;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.y);
	}
	// 時計回り
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_LEFT))
	{
		// 減算
		m_rotDest.y += m_RotSpeed.y;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.y);
	}
	// 上
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_UP))
	{
		// 加算
		m_rotDest.x -= m_RotSpeed.x;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.x);
	}
	// 下
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_DOWN))
	{
		// 加算
		m_rotDest.x += m_RotSpeed.x;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.x);
	}

	// x回転の限界地点
	if (m_rotDest.x > -0.1f)
	{
		m_rotDest.x = -0.1f;
	}
	if (m_rotDest.x < -2.5f)
	{
		m_rotDest.x = -2.5f;
	}
}
*/