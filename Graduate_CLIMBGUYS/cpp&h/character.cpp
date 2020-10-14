// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// キャラクター処理 [character.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"
#include "floor.h"
#include "input.h"
#include "collision.h"
#include "3Dparticle.h"
#include "camera.h"
#include "meshobit.h"
#include "ui.h"

#include "rectcollision.h"
#include "spherecollision.h"
#include "columncollision.h"
#include "circleshadow.h"
#include "stencilshadow.h"

#include "fade.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CHARACTER_KEYMOVE (1)													// キー移動
#define CHARACTER_G (0.5f)														// 重力
#define CHARACTER_RESISTANCE (0.5f)												// 抵抗力
#define CHARACTER_STATUS_FILE ("data/LOAD/STATUS/status_manager_Character.csv")	// ステータスファイル名
#define CHARACTER_INFO_FILE ("data/LOAD/CHARACTER/CHARACTER_MANAGER.txt")		// キャラクターファイル名
#define CIRCLESHADOW (true)														// 円形のシャドウにするかしないか
#define REFLECTION_COEFFICIENT (1)												// 反発係数

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MODEL_ALL	*CCharacter::m_modelAll[CHARACTER_MAX] = {};		// キャラクター全体の情報
CModel_info	*CCharacter::m_model_info[CHARACTER_MAX] = {};		// キャラクター情報
std::vector<int>	CCharacter::m_modelId[CHARACTER_MAX];				// キャラクター番号
D3DXVECTOR3	CCharacter::m_CharacterSize[CHARACTER_MAX] = {};	// キャラクターのサイズ
CCharacter::STATUS CCharacter::m_sStatus[CHARACTER_MAX] = {};	// キャラクターすべてのスタータス情報
int			CCharacter::m_NumParts[CHARACTER_MAX] = {};			// 動かすキャラクター数
int			CCharacter::m_NumModel[CHARACTER_MAX] = {};			// 最大キャラクター数
int			CCharacter::m_nCameraCharacter = 0;					// キャラクターに追尾するID
int			CCharacter::m_nAllCharacter = 0;					// 出現しているキャラクター人数
float		CCharacter::m_fAlpha = 0.0f;						// 透明度

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCharacter::CCharacter(CHARACTER const &character) : CScene::CScene()
{
	m_pMeshobit = NULL;								// 軌跡
	m_pModel = NULL;								// モデル
	m_character = CHARACTER_NPC;				// キャラクター
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);			// 位置
	m_posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在回転量
	m_rotLast = m_rot;								// 向きたい方向
	m_rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転の差分
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// キャラクターのサイズ
	m_nMotiontypeOld = 0;							// 前回のモーションタイプ
	m_nMotiontype = 0;								// モーションタイプ
	m_keyinfoCnt = 0;								// キー情報のカウント
	m_nFrame = 0;									// フレームカウント
	m_nMotionFrame = 0;								// モーション一つののフレームカウント
	m_nMaxMotion = 0;								// 最大モーション数
	m_nCntDamage = 0;								// ダメージカウント
	m_State = STATE_NORMAL;							// 現状のステータス
	m_nCntState = 0;								// カウントステータス
	m_fLength = 0;									// 攻撃の当たり範囲
	m_fAlpha = 1.0f;								// アルファ値
	m_bMotionCamera = false;						// モーションカメラの切り替え
	m_bLanding = false;								// 着地状態
	m_bMove = false;								// 移動状態
	m_Directvector = D3DVECTOR3_ONE;				// 方向ベクトル
	m_pStencilshadow = NULL;						// ステンシルシャドウ
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	m_character = character;						// キャラクター設定
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCharacter::~CCharacter()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Init()
{
	// 過去の位置情報保存
	m_posold = m_pos;
	// 総キャラクターカウントアップ
	m_nAllCharacter++;
	// キャラクターのメモリ確保
	m_pModel = new CModel[m_NumParts[m_character]];

	// キャラクターの設定
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// キャラクター情報渡し
		m_pModel[nCntModel].Init();
		// キャラクターの情報を渡す
		m_pModel[nCntModel].BindModel(
			m_modelAll[m_character]->pModel_offset[nCntModel]
		);
		// キャラクターの番号設定
		m_pModel[nCntModel].SetModelId(m_modelId[m_character][nCntModel]);
		// シャドウon
		if (CIRCLESHADOW == false)
		{
			m_pModel[nCntModel].SetShadowMap(true);
		}
		// モーション設定
		m_pModel[nCntModel].BeginMotion(
			m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].Key[nCntModel],
			m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].nFrame);
		// すべての親以外
		if (nCntModel != 0)
		{
			// 親情報設定
			m_pModel[nCntModel].SetParentMtx(
				&m_pModel[m_modelAll[m_character]->pModel_offset[nCntModel].nParent].GetMatrix()
			);
		}
		// すべての親
		else
		{
			// 親情報設定
			m_pModel[nCntModel].SetParentMtx(
				&m_mtxWorld
			);
		}
	}

	// 軌跡の設定
	for (int nCntObit_Basic = 0; nCntObit_Basic < (signed)m_modelAll[m_character]->v_MeshObitLoad.size(); nCntObit_Basic++)
	{
		m_vec_pMeshObit.push_back(std::move(CMeshobit::Create_Self(
			m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).nLine,
			m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset,
			m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).EndOffset,
			(CMeshobit::TEX)m_modelAll[m_character]->v_MeshObitLoad.at(nCntObit_Basic).nTexType
		)));
	}

	// 攻撃当たり判定設定
	for (int nCntAttackCollision = 0; nCntAttackCollision < (signed)m_modelAll[m_character]->v_AttackCollision.size(); nCntAttackCollision++)
	{
		// 変数宣言
		D3DXVECTOR3 pos;
		// 当たり判定の位置の設定
		D3DXVec3TransformCoord(
			&pos,
			&m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
			&m_pModel[m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).nParts].GetMatrix()
		);
		// 矩形の当たり判定
		if (m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_RectInfo)
		{
			// 矩形の当たり判定
			m_vec_AttackCollision.push_back(std::move(CRectCollision::Create_Self(
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_RectInfo->size,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
				CCollision::OBJTYPE_ATTACK
			)));
		}
		// 球の当たり判定
		else if (m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_SphereInfo)
		{
			// 球の当たり判定
			m_vec_AttackCollision.push_back(std::move(CSphereCollision::Create_Self(
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_SphereInfo->fRadius,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
				CCollision::OBJTYPE_ATTACK
			)));
		}
		// 円柱の当たり判定
		else if (m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_ColumnInfo)
		{
			// 円柱の当たり判定
			m_vec_AttackCollision.push_back(std::move(CColumnCollision::Create_Self(
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_ColumnInfo->fRadius,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).p_uni_ColumnInfo->fVertical,
				m_modelAll[m_character]->v_AttackCollision.at(nCntAttackCollision).Offset,
				CCollision::OBJTYPE_ATTACK
			)));
		}
	}

	// キャラクター当たり判定設定
	if (m_modelAll[m_character]->pCharacterCollision != NULL)
	{
		// 変数宣言
		CCollision::OBJTYPE objtype = CCollision::OBJTYPE_PLAYER;	// あたり判定のオブジェクトタイプ
		// オブジェクトタイプがNPCの時
		if (m_character == CHARACTER_NPC)
		{
			objtype = CCollision::OBJTYPE_ENEMY;
		}
		// それ以外
		else
		{
			objtype = CCollision::OBJTYPE_PLAYER;
		}
		// 矩形の当たり判定
		if (m_modelAll[m_character]->pCharacterCollision->RectInfo)
		{
			m_pCharacterCollision = CRectCollision::Create(
				m_modelAll[m_character]->pCharacterCollision->RectInfo->size,
				m_modelAll[m_character]->pCharacterCollision->Offset,
				objtype,
				this,
				NULL,
				true,
				true,
				&m_pos,
				&m_posold
			);
		}
		// 球の当たり判定
		else if (m_modelAll[m_character]->pCharacterCollision->p_uni_SphereInfo)
		{
			m_pCharacterCollision = CSphereCollision::Create(
				m_modelAll[m_character]->pCharacterCollision->p_uni_SphereInfo->fRadius,
				m_modelAll[m_character]->pCharacterCollision->Offset,
				objtype,
				this,
				NULL,
				true,
				true,
				&m_pos,
				&m_posold
			);

		}
	}
	// シャドウon
	if (CIRCLESHADOW == true)
	{
		D3DXVECTOR3 pos = m_pos;
		pos.y = 0;
		// ステンシルシャドウの生成
		m_pStencilshadow = CStencilshadow::Create(m_pos, D3DXVECTOR3(10.0f, 10000.0f, 10.0f));
	}
	// 行列の初期化処理
	D3DXMatrixIdentity(&m_mtxWorld);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Uninit(void)
{
	// モデルヌルチェック
	// ->モデルの開放
	if (m_pModel != NULL)
	{
		delete[] m_pModel;
		m_pModel = NULL;
	}
	// キャラクター当たり判定のヌルチェック
	// ->開放
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->CompulsionScene();
		m_pCharacterCollision = NULL;
	}
	// 軌跡の情報を開放する
	for (int nCntMotionObit = 0; nCntMotionObit < (signed)m_vec_pMeshObit.size(); nCntMotionObit++)
	{
		// 軌跡の終了処理
		m_vec_pMeshObit.at(nCntMotionObit)->Uninit();
	}
	// ステンシルシャドウのヌルチェック
	// ステンシルシャドウの初期化
	if (m_pStencilshadow != NULL)
	{
		m_pStencilshadow = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Update(void)
{
	// 過去の位置情報保存
	m_posold = m_pos;
	// 通常時の更新処理
	Update_Normal();
	// 当たり判定処理
	Collision();
	// ステンシルシャドウの位置設定
	if (m_pStencilshadow != NULL)
	{
		// 位置取得
		D3DXVECTOR3 pos = m_pos;
		pos.y = -1000.0f;
		// ステンシルシャドウの位置設定
		m_pStencilshadow->SetPos(pos);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// それぞれの当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Collision(void)
{
	/*
	// 変数宣言
	CCollision * pCollision;
	int nCnt = CScene::GetMaxLayer(LAYER_COLLISION);
	// あたり判定判定
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer++)
	{
		// あたり判定の情報取得
		pCollision = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer);
		// 自分の当たり判定の情報がNULLなら
		// ->ループスキップ
		if (m_pCharacterCollision == NULL) continue;
		// あたり判定情報入っていない場合
		// ->ループスキップ
		else if (pCollision == NULL) continue;
		// キャラクターの当たり判定と取得した当たり判定が同じなら
		// ->ループスキップ
		else if (m_pCharacterCollision == pCollision)
		{
			continue;
		}
		// キャラクターとあたり判定同士の当たり判定処理
		else if (m_pCharacterCollision->CollisionDetection(pCollision))
		{
		}
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 通常時の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Update_Normal(void)
{
	// 移動
	Move();
	// 高さ
	GetFloorHeight();
	// モーション
	Motion();
	// エフェクト情報更新
	Motion_Effect();
	// キャラクター更新
	ModelUpdate();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 次のモーション設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::NextKeyMotion(void)
{
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// ヌルチェック
		if (&m_pModel[nCntModel] != NULL)
		{
			// モーション設定
			m_pModel[nCntModel].SetMotion(
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].Key[nCntModel],
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].nFrame);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 移動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Move(void)
{
	// 現在回転差分
	m_rotbetween = m_rotLast - m_rot;

	// 限度調整
	m_rotbetween.x = CCalculation::Rot_One_Limit(m_rotbetween.x);
	m_rotbetween.y = CCalculation::Rot_One_Limit(m_rotbetween.y);
	m_rotbetween.z = CCalculation::Rot_One_Limit(m_rotbetween.z);

	// 回転移動
	m_rot += m_rotbetween * 0.1f;

	// 限度調整
	m_rot.x = CCalculation::Rot_One_Limit(m_rot.x);
	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);
	m_rot.z = CCalculation::Rot_One_Limit(m_rot.z);

	// 位置情報更新
	m_pos += m_move;
	// 抵抗力
	m_move.x *= m_sStatus[m_character].fMaxInertia;
	m_move.z *= m_sStatus[m_character].fMaxInertia;
	// 上限処理
	Limit();

	// キャラクターの当たり判定がNULLではないなら
	// ->当たり判定の更新
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->GetShape()->PassPos(m_pos);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーション処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Motion(void)
{
	// フレームカウントアップ
	m_nFrame++;
	m_nMotionFrame++;
	// モーションタイプが変化
	// 初期化
	if (m_nMotiontype != m_nMotiontypeOld)
	{
		m_nFrame = 0;		// フレームキー情報のカウント
		m_nMotionFrame = 0;	// モーション一つのフレームカウント
		m_keyinfoCnt = 0;	// キー情報のカウント
		// 次のモーション設定
		NextKeyMotion();
	}
	// モーションの保存
	m_nMotiontypeOld = m_nMotiontype;
	// フレーム数が同じになったら
	if (m_nFrame >= m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].nFrame)
	{
		// 初期化
		m_nFrame = 0;	// フレーム
		// カウントアップ
		m_keyinfoCnt++;
		// モーションカメラの切り替えの初期化
		m_bMotionCamera = false;
		// キー情報が超えたら
		if (m_keyinfoCnt >= m_modelAll[m_character]->pMotion[m_nMotiontype]->nNumKey)
		{
			// ループしないとき
			if (m_modelAll[m_character]->pMotion[m_nMotiontype]->nLoop == 0)
			{
				m_nMotiontype = 0;	// モーションタイプ
			}
			// 初期化
			m_keyinfoCnt = 0;				// キー情報
			m_nMotionFrame = 0;				// モーション一つのフレームカウント
		}
		// 次のモーション設定
		NextKeyMotion();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// キャラクターの更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::ModelUpdate(void)
{
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// ヌルチェック
		if (&m_pModel[nCntModel] != NULL)
		{
			// 更新
			m_pModel[nCntModel].Update();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラ追尾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::TrackCamera(void)
{
	//// モードが選択画面とチュートリアルの場合
	//if (CManager::GetMode() == CManager::MODE_SELECT)
	//{
	//	// カメラの注視点設定
	//	CManager::GetRenderer()->GetCamera()->SetPosDestRPlayer(
	//		CCharacter::GetPos(),
	//		CCharacter::GetRot()
	//	);
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 制限区域
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Limit(void)
{
	//// xの制限
	//if (m_pos.x < -950)
	//{
	//	m_pos.x = -950;
	//	m_move.x = 0.0f;
	//}
	//if (m_pos.x > 950)
	//{
	//	m_pos.x = 950;
	//	m_move.x = 0.0f;
	//}
	//// zの制限
	//if (m_pos.z < -950)
	//{
	//	m_pos.z = -950;
	//	m_move.z = 0.0f;
	//}
	//if (m_pos.z > 950)
	//{
	//	m_pos.z = 950;
	//	m_move.z = 0.0f;
	//}
	if (m_pos.y > 1000.0f)
	{
		m_pos.y = 1000.0f;
		m_move.y = 0.0f;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーションエフェクト更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Motion_Effect(void)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	for (int nCntMotionEffect = 0; nCntMotionEffect < (signed)m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.size(); nCntMotionEffect++)
	{
		// 開始時間外ならスキップ
		if (m_nFrame < m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nStart)
		{
			continue;
		}
		// 終了時間外ならスキップ
		else if (m_nFrame > m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nEnd)
		{
			continue;
		}
		// 時間内なら
		// 位置設定
		D3DXVec3TransformCoord(
			&pos,
			&m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).offset,
			CCharacter::GetMatrix(m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nKeyID)
		);
		// パーティクルの生成
		C3DParticle::Create(
			(C3DParticle::PARTICLE_ID)m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].v_MotionEffect.at(nCntMotionEffect).nParticleType,
			pos);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーション軌跡更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Motion_Obit()
{
	for (int nCntMotionObit = 0; nCntMotionObit < (signed)m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.size(); nCntMotionObit++)
	{
		// 開始時間外ならスキップ
		if (m_nMotionFrame < m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nBeginFram)
		{
			continue;
		}
		// 終了時間外ならスキップ
		else if (m_nMotionFrame > m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nEndFram)
		{
			continue;
		}
		// 開始時に位置を初期に
		else if (m_nMotionFrame == m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nBeginFram)
		{
			m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->InitPos(*CCharacter::GetMatrix(m_modelAll[m_character]->v_MeshObitLoad.at(
				m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID).nPart));
		}
		// 軌跡の色の設定
		m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->SetCol(
			m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).BeginCol,
			m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).EndCol
		);
		// 軌跡の描画設定
		m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->DrawSet(*CCharacter::GetMatrix(m_modelAll[m_character]->v_MeshObitLoad.at(
			m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID).nPart));
		// 軌跡の描画
		m_vec_pMeshObit.at(m_modelAll[m_character]->pMotion[m_nMotiontype]->v_MeshObit_detail.at(nCntMotionObit).nObitID)->Draw();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーションカメラの更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::MotionCamera(void)
{
	// 変数宣言
	// 情報取得
	static CCamera * pCamera = CManager::GetRenderer()->GetCamera();	// カメラ
	// モーションカメラの切り替えがOFFの場合
	if (!m_bMotionCamera)
	{
		// 要素があったら処理を行う
		if (m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera != NULL)
		{
			// カメラモーション設定
			pCamera->SetCamera_Motion(
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->offsetR + m_pos,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->rot + m_rot,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->fLength,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->fHeight,
				m_modelAll[m_character]->pMotion[m_nMotiontype]->KeyInfo[m_keyinfoCnt].pMotionCamera->fIntertia
			);
			// モーションカメラの切り替え
			m_bMotionCamera = true;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Draw(void)
{
	// 変数宣言
	D3DXMATRIX	mtxRot, mtxTrans;	// 計算用マトリックス

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
	D3DXMatrixTranslation(&mtxTrans,	// 総合の入れ物
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3


	// キャラクター
	for (int nCntModel = 0; nCntModel < m_NumParts[m_character]; nCntModel++)
	{
		// ヌルチェック
		if (&m_pModel[nCntModel] != NULL)
		{
			// モデルの描画
			m_pModel[nCntModel].Draw();

		}
	}

	// モーション軌跡更新
	Motion_Obit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 死んだときの処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Die(void)
{
	// 開放
	Release();
	// ステンシルシャドウがNULLなら
	// ->開放する
	if (m_pStencilshadow != NULL)
	{
		m_pStencilshadow->Release();
		m_pStencilshadow = NULL;
	}
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::PARTICLE_ID_UNKNOWN,
		m_pos
	);

	// 総キャラクターカウントダウン
	m_nAllCharacter--;

	// キャラクター当たり判定のヌルチェック
	// ->開放
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->CompulsionScene();
		m_pCharacterCollision->Release();
		m_pCharacterCollision = NULL;
	}
	//// 総キャラクターが一人だけなら
	//// ->タイトルへフェード
	//if (m_nAllCharacter <= 1)
	//{
	//	if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
	//	{
	//		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	//	}
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Scene_MyCollision(
	int const & nObjType,
	CScene * pScene
)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フィールドの高さを算出
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCharacter::GetFloorHeight(void)
{
	// 変数宣言
	CFloor * pFloor = NULL;	// 床
							// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		pFloor = (CFloor*)CScene::GetScene( CScene::LAYER_3DOBJECT, nCntLayer,CFloor());	// 床
		if (pFloor != NULL)
		{
			if (pFloor->GetCalculation())
			{
				// 床の高さを代入
				if (m_pos.y < pFloor->GetHeight(m_pos))
				{
					m_pos.y = pFloor->GetHeight(m_pos);
					m_move.y = 0;
					// 着地状態をtrueに
					m_bLanding = true;
					return true;
				}
				// それ以外
				else
				{
					// 着地状態をfalseに
					m_bLanding = false;
				}
			}
		}
	}
	return false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーションのフレーム情報取得処理
// 1)キャラクターID、2)モーションID、3)現在のキーカウント
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCharacter::GetMaxFrame(
	CHARACTER character,	// キャラクター
	int nMotionID,			// モーションID
	int nNowKeyCnt			// 現在のキーカウント
)
{
	// モーション全体のフレーム数
	if (nNowKeyCnt == -1)
	{
		return m_modelAll[character]->pMotion[nMotionID]->nAllFrame;
	}
	// 一つのキー間のフレーム数
	return m_modelAll[character]->pMotion[nMotionID]->KeyInfo[nNowKeyCnt].nFrame;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラ追尾処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCharacter::GetCameraCharacter(void)
{
	return m_nCameraCharacter;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 重力処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::FagGravity(void)
{
	// 重力処理
	//m_move.y -= 1.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーション設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::SetMotion(int const nMotiontype)
{
	// ループ状態の時
	if (m_modelAll[m_character]->pMotion[m_nMotiontype]->nLoop == 1)
	{
		m_nMotiontype = nMotiontype;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 強制モーション設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::ComplusionSetMotion(int const nMotiontype)
{
	m_nMotiontype = nMotiontype;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 回転取得処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 *CCharacter::GetPartsRot(int const nModelID)
{
	// キャラクターの行列情報取得
	if (nModelID >= 0 ||
		nModelID < m_NumParts[m_character])
	{
		return &m_pModel[nModelID].GetRot();
	}
	// キャラクターの行列情報取得
	else if (nModelID == -1)
	{
		return &m_rot;
	}
#ifdef _DEBUG
	CCalculation::Messanger("キャラクターのキャラクターの回転情報がありません");
#endif // _DEBUG
	// 指定されたIDがない場合
	return &m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 位置取得処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 *CCharacter::GetPartsPos(int const nModelID)
{
	// キャラクターの行列情報取得
	if (nModelID >= 0 ||
		nModelID < m_NumParts[m_character])
	{
		return &m_pModel[nModelID].GetPos();
	}
	// キャラクターの行列情報取得
	else if (nModelID == -1)
	{
		return &m_rot;
	}
#ifdef _DEBUG
	CCalculation::Messanger("キャラクターのキャラクターの回転情報がありません");
#endif // _DEBUG
	// 指定されたIDがない場合
	return &m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 行列取得処理
// -1はキャラクターの行列情報、0～はキャラクターの行列情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXMATRIX * CCharacter::GetMatrix(int const nModelID)
{
	// キャラクターの行列情報取得
	if(nModelID >= 0 ||
		nModelID < m_NumParts[m_character])
	{
		return &m_pModel[nModelID].GetMatrix();
	}
	// キャラクターの行列情報取得
	else if (nModelID == -1)
	{
		return &m_mtxWorld;
	}
#ifdef _DEBUG
	CCalculation::Messanger("キャラクターのキャラクターの行列情報がありません");
#endif // _DEBUG
	// 指定されたIDがない場合
	return &m_mtxWorld;
}

/*
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Load(
	CHARACTER const character,
	int const nMaxMotion,
	const char * file_name
)
{
	// 変数宣言
	// キャラクターとモーション情報の生成
	m_modelAll[character] = new MODEL_ALL;
	// キャラクターのテキストデータの取得
	CModel_info::TextLoad(
		m_modelAll[character],					// キャラクター情報
		m_modelId[character],					// キャラクターファイル
		nMaxMotion,								// モーション数
		m_NumModel[character],					// 最大キャラクター数
		m_NumParts[character],					// 動かすキャラクター数
		file_name								// ファイル名
	);
}
*/

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// キャラクター全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCharacter::Load(void)
{
	// 変数宣言
	HRESULT hr;
	// キャラクターの情報読み込み
	hr = Load_Character();
	// ステータスの情報読み込み
	hr = LoadStatus();
	return hr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// キャラクターの情報読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCharacter::Load_Character(void)
{
	// ファイルの中身格納用
	std::vector<std::vector<std::string>> vsvec_Contens;
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(CHARACTER_INFO_FILE, ',');
	// 行ごとに回す
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// 項目ごとに回す
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			switch (nCntItem)
			{
				// パス情報
			case 0:
				// キャラクターの最大数を超えたら
				// ケースを抜ける
				if (nCntLine >= CHARACTER_MAX)
				{
#ifdef _DEBUG
					CCalculation::Messanger("CharacterのLoad_Character関数->キャラクターの最大数を超えました。");
#endif // _DEBUG
					break;
				}
				// キャラクターとモーション情報の生成
				m_modelAll[nCntLine] = new MODEL_ALL;
				// キャラクターのテキストデータの取得
				CModel_info::TextLoad(
					m_modelAll[nCntLine],							// キャラクター情報
					m_modelId[nCntLine],							// キャラクターファイル
					m_NumModel[nCntLine],							// 最大キャラクター数
					m_NumParts[nCntLine],							// 動かすキャラクター数
					vsvec_Contens.at(nCntLine).at(nCntItem).c_str()	// ファイル名
				);
				break;
			default:
				break;
			}
		}
	}
	// std::vectorの多重配列開放
	std::vector<std::vector<std::string>>().swap(vsvec_Contens);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ステータス情報読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCharacter::LoadStatus(void)
{
	// 変数宣言
	std::vector<std::vector<std::string>> vsvec_Contens;	// ファイルの中身格納用
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(CHARACTER_STATUS_FILE, ',');
	// 行ごとに回す
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// キャラクターが上限を超えていたら抜ける
		if (nCntLine >= CHARACTER_MAX)
		{
			break;
		}
		// 項目ごとに回す
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			switch (nCntItem)
			{
				// 風船の個数
			case 0:
				m_sStatus[nCntLine].nMaxBalloon = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// 風船の個数
			case 1:
				m_sStatus[nCntLine].nMaxPopBalloon = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// 慣性力
			case 2:
				m_sStatus[nCntLine].fMaxInertia = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// ジャンプ力
			case 3:
				m_sStatus[nCntLine].fMaxJump = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// 移動力
			case 4:
				m_sStatus[nCntLine].fMaxMove = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// 最大MP数
			case 5:
				m_sStatus[nCntLine].nMaxMp = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// マイフレームで増えるMP数
			case 6:
				m_sStatus[nCntLine].nMaxMpUp_Every = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// 倒したときのMP数
			case 7:
				m_sStatus[nCntLine].nMaxMpUp_KnockDown = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// スキル使用時の減る量
			case 8:
				m_sStatus[nCntLine].nMaxMpDown = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
				// 能力の倍率
			case 9:
				m_sStatus[nCntLine].fMaxSkill = stof(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;

			default:
				break;
			}
		}
	}
	// std::vectorの多重配列開放
	std::vector<std::vector<std::string>>().swap(vsvec_Contens);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::UnLoad(void)
{
	// キャラクター・モーションの破棄
	for (int nCntCharacter = 0; nCntCharacter < CHARACTER_MAX; nCntCharacter++)
	{
		CModel_info::TextUnload(m_modelAll[nCntCharacter]);
		m_modelId[nCntCharacter].clear();
		m_modelId[nCntCharacter].shrink_to_fit();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的変数の初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::InitStatic(void)
{
	m_nAllCharacter = 0;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::Debug(void)
{
	// キャラクターの当たり判定がNULLではないなら
	// デバッグ処理
	if (m_pCharacterCollision != NULL)
	{
		m_pCharacterCollision->Debug();
	}
	// 移動量表示
	CDebugproc::Print("キャラクター[%d]:移動量(%.2f,%.2f,%.2f)\n", m_character, m_move.x, m_move.y, m_move.z);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示(キャラクター全体)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCharacter::AllDebug(void)
{
	/*
	// キャラクターステータスの表示 //
	CDebugproc::Print("//----------キャラクターステータス情報----------//\n");
	for (int nCntCharacter = 0; nCntCharacter < CHARACTER_MAX; nCntCharacter++)
	{
		CDebugproc::Print("// [%d]\n",nCntCharacter);
		CDebugproc::Print("最大持っていける風船数:%d\n", m_sStatus[nCntCharacter].nMaxBalloon);
		CDebugproc::Print("最大出現風船数:%d\n", m_sStatus[nCntCharacter].nMaxPopBalloon);
		CDebugproc::Print("慣性力:%.2f\n", m_sStatus[nCntCharacter].fMaxInertia);
		CDebugproc::Print("ジャンプ力:%.2f\n", m_sStatus[nCntCharacter].fMaxJump);
		CDebugproc::Print("移動力:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);
		CDebugproc::Print("移動力:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);
		CDebugproc::Print("移動力:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);
		CDebugproc::Print("移動力:%.2f\n", m_sStatus[nCntCharacter].fMaxMove);

	}
	*/
}
#endif // _DEBUG

