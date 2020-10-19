// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "input.h"
#include "camera.h"
#include "ui.h"
#include "manager.h"
#include "fade.h"
#include "collision.h"
#include "game.h"
#include "XInputPad.h"
#include "keyboard.h"
#include "ui_group.h"
#include "meshdome.h"
#include "3Dparticle.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_GRAVITY (0.1f)
#define PLAYER_UPMOVELIMIT		(30.0f)	// プレイヤーの上昇移動量制限
#define PLAYER_UNDERMOVELIMIT	(5.0f)	// プレイヤーの下降移動量制限
#define PLAYER_JUMP_POWER		(18.0f)	// プレイヤーのジャンプ力
#define PLAYER_MOVE				(3.0f)	// プレイヤーの移動速度

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer(CHARACTER const &character) : CCharacter::CCharacter(character)
{
	m_nCntState = 0;				// ステートカウント
	m_bDieFlag = false;				// 死亡フラグ
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	// バルーンキャラクター初期化
	CCharacter::Init();

	// パッドのポインタ取得
	m_pPad = CManager::CManager::GetPad(GetPlayerTag());
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	// キャラクター終了処理
	CCharacter::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// 自キャラの行動処理
	MyAction();
	// モーション設定処理
	StatusMotion();

	// キャラクター更新
	CCharacter::Update();
	CCharacter::Limit();

	// 死亡判定が出たらリザルトに遷移する
	if (GetDie())
	{
		if (CManager::GetFade()->GetFade() == CManager::GetFade()->FADE_NONE)
		{
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自キャラ行動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyAction(void)
{
	// 自キャラの移動処理
	MyMove();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 移動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyMove(void)
{
	// 変数宣言
	D3DXVECTOR3 move, rot;			// 移動量、回転
	D3DXVECTOR3 vec;				// ベクトル
	float fRot;						// 回転

	// 情報取得
	rot = CCharacter::GetRotDest();								// 目的回転量
	move = CCharacter::GetMove();								// 移動量
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// カメラ回転
	vec = CCharacter::GetDirectionVec();						// ベクトル
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	// 移動 //
	/* キーボード */
	// 左
	if (pKeyboard->GetKeyboardPress(DIK_A))
	{
		// 奥
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = -D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.75f + fRot), 0.0f, cosf(D3DX_PI * 0.75f + fRot));
			move.x += vec.x * 10.0f;
			move.z += vec.z * 10.0f;
		}
		// 手前
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = -D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.25f + fRot), 0.0f, cosf(D3DX_PI * 0.25f + fRot));
			move.x += vec.x * 10.0f;
			move.z += vec.z * 10.0f;
		}
		// 左
		else
		{
			rot.y = -D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fRot), 0.0f, cosf(D3DX_PI * 0.5f + fRot));
			move.x += vec.x * 10.0f;
			move.z += vec.z * 10.0f;
		}
	}
	// 右
	else if (pKeyboard->GetKeyboardPress(DIK_D))
	{

		// 奥
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f + fRot), 0.0f, cosf(-D3DX_PI * 0.75f + fRot));

			move.x += vec.x * 10.0f;
			move.z += vec.z * 10.0f;
		}
		// 手前
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f + fRot), 0.0f, cosf(-D3DX_PI * 0.25f + fRot));


			move.x += vec.x * 10.0f;
			move.z += vec.z * 10.0f;
		}
		// 右
		else
		{
			rot.y = D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f + fRot), 0.0f, cosf(-D3DX_PI * 0.5f + fRot));

			move.x += vec.x * 10.0f;
			move.z += vec.z * 10.0f;
		}
	}
	// 奥に行く
	else if (pKeyboard->GetKeyboardPress(DIK_W))
	{
		rot.y = D3DX_PI * 0.0f + fRot;
		vec = D3DXVECTOR3(sinf(-D3DX_PI * 1.0f + fRot), 0.0f, cosf(-D3DX_PI * 1.0f + fRot));
		move.x += vec.x * 10.0f;
		move.z += vec.z * 10.0f;
	}
	// 手前に行く
	else if (pKeyboard->GetKeyboardPress(DIK_S))
	{
		rot.y = D3DX_PI * 1.0f + fRot;
		vec = D3DXVECTOR3(sinf(D3DX_PI * 0.0f + fRot), 0.0f, cosf(D3DX_PI * 0.0f + fRot));
		move.x += vec.x * 10.0f;
		move.z += vec.z * 10.0f;
	}
	// それ以外
	else
	{

	}

	// 試験的キーボードジャンプ
	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) && GetJumpAble())
	{
		move.y += PLAYER_JUMP_POWER;
		SetJumpAble(false);
	}

	/* ゲームパッド */
	// パッド用 //
	float fValueH, fValueV;		// ゲームパッドのスティック情報の取得用
	float fMove = PLAYER_MOVE;	// 移動速度
	float fAngle;				// スティック角度の計算用変数
	fAngle = 0.0f;				// 角度

	if (m_pPad)
	{
		// ゲームパッドのスティック情報を取得
		m_pPad->GetStickLeft(&fValueH, &fValueV);

		// プレイヤー移動
		// ゲームパッド移動
		if (fValueH != 0 || fValueV != 0)
		{
			// 角度の計算
			fAngle = atan2f((float)fValueH, (float)fValueV);

			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}
			else if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}
			rot.y = fAngle + fRot;
			vec = D3DXVECTOR3(sinf(fAngle + fRot), 0.0f, cosf(fAngle + fRot));
			// スティックの角度によってプレイヤー移動
			move.x -= sinf(fAngle + fRot) * (fMove);
			move.z -= cosf(fAngle + fRot) * (fMove);
		}

		// 試験的ジャンプ ( のちに中身変わる予定 多分 )
		if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_A, 1) && GetJumpAble())
		{
			move.y += PLAYER_JUMP_POWER;
			SetJumpAble(false);
		}

		// 試験的タックル ( のちに中身変わる予定 多分 )
		if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_X, 1))
		{
			switch (CCalculation::CheckPadStick())
			{
			case DIRECTION::LEFT:
				move.x -= 100.0f;
				break;

			case DIRECTION::RIGHT:
				move.x += 100.0f;
				break;

			case DIRECTION::UP:
				move.z += 100.0f;
				break;

			case DIRECTION::DOWN:
				move.z -= 100.0f;
				break;
			}
		}
	}

	// ジャンプしているときの慣性
	if (!GetJumpAble())
	{
		move.x += ( -m_move.x) * 1.6f;
		move.z += ( -m_move.z) * 1.6f;
	}
	else
	{
		move.x += (-m_move.x) * 0.7f;
		move.z += (-m_move.z) * 0.7f;
	}

	if (vec.x < 0)
	{
		vec.x *= -1;
	}
	if (vec.z < 0)
	{
		vec.z *= -1;
	}
	CCharacter::SetDirectionVec(vec);
	// yの上限設定
	if (move.y > PLAYER_UPMOVELIMIT)
	{
		move.y = PLAYER_UPMOVELIMIT;
	}
	if (move.y < -PLAYER_UNDERMOVELIMIT)
	{
		move.y = -PLAYER_UNDERMOVELIMIT;
	}
	CCharacter::SetMove(move);

	// キャラクターの重力加算処理
	CCharacter::AddGravity(PLAYER_GRAVITY);
	CCharacter::SetRotDest(rot);

	//if (pKeyboard->GetKeyboardTrigger(DIK_C))
	//{
	//	CGame::GetPlayer(1)->SetRotDest(D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 状態によってのモーション設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StatusMotion(void)
{
	/*
	// ジャンプ中
	if (!CCharacter::GetbLanding())
	{
		SetMotion(MOTIONTYPE_JAMP);
	}
	// 移動中
	else if (CCharacter::GetbMove())
	{
		// モーション設定(移動)
		SetMotion(MOTIONTYPE_MOVE);
	}
	// 待機
	else
	{
		SetMotion(MOTIONTYPE_NEUTRAL);
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	// キャラクター描画
	CCharacter::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 死亡処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Die(void)
{
	// 死亡処理
	CCharacter::Die();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 人形の死亡処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::OtherDie(void)
{
	// 死亡処理
	CCharacter::Die();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_MyCollision(int const & nObjType, CScene * pScene)
{
	// バルーンキャラクターの当たった後の処理
	CCharacter::Scene_MyCollision(nObjType, pScene);

	// 当たったオブジェクトがブロックだったらジャンプを可能にする
	if (nObjType == CCollision::OBJTYPE_BLOCK)
	{
		SetJumpAble(true);
	}
	// 当たったオブジェクトがダメージ床だったら死亡フラグをtrueにする
	else if (nObjType == CCollision::OBJTYPE_DAMAGEFLOOR)
	{
		SetDie(true);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
	// キャラクターの相手に当てられた後の処理
	CCharacter::Scene_OpponentCollision(nObjType, pScene);

	// シーン情報がNULLなら
	// ->関数を抜ける
	if (pScene == NULL) return;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自分から当たらなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_NoMyCollision(int const & nObjType, CScene * pScene)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene)
{

}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
//	pos	: 位置
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(PLAYER_TAG tag,
	D3DXVECTOR3 const & pos	// 位置
)
{
	// 変数宣言
	CPlayer * pPlayer;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPlayer = new CPlayer(CHARACTER_PLAYER);
	// シーン管理設定
	pPlayer->ManageSetting(CScene::LAYER_CHARACTER);
	// 位置情報
	pPlayer->m_pos = pos;
	// プレイヤータグの設定
	pPlayer->SetPlayerTag(tag);
	// 初期化処理
	pPlayer->Init();
	// 生成したオブジェクトを返す
	return pPlayer;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// リソース情報読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだリソース情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::UnLoad(void)
{
}
