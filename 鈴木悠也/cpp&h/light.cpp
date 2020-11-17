//=============================================================================
//
// ライト処理 [light.cpp]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#include "light.h"
#include "manager.h"

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CLight::CLight()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXVECTOR3 Direct = D3DVECTOR3_ZERO;
	// カウントライト
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_light[nCntLight].Diffuse = m_col[nCntLight];

		// ライトの方向の設定
		D3DXVec3Normalize(&Direct, &m_VecDir[nCntLight]);		// 正規化
		m_light[nCntLight].Direction = Direct;					// ライトに代入

		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CLight::~CLight()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CLight::Init(void)
{
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CLight::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CLight::Update(void)
{
}
#ifdef _DEBUG
// ----------------------------------------
// デバッグ処理
// ----------------------------------------
void CLight::Debug(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXVECTOR3 Direct = D3DVECTOR3_ZERO;
	// カメラの情報
	if (ImGui::CollapsingHeader("Light"))
	{
		// メインカメラ情報
		if (ImGui::TreeNode("MainLightInfo"))
		{
			// 方向を設定
			if (ImGui::TreeNode("Direction"))
			{
				ImGui::DragFloat("X", &m_VecDir[0].x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Y", &m_VecDir[0].y, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Z", &m_VecDir[0].z, 0.1f, -1.0f, 1.0f);
				ImGui::TreePop();
			}
			// 方向を設定
			if (ImGui::TreeNode("Col"))
			{
				ImGui::DragFloat("R", &m_col[0].r, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("G", &m_col[0].g, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("B", &m_col[0].b, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("A", &m_col[0].a, 0.1f, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// サブカメラ1情報
		if (ImGui::TreeNode("SubLightInfo_1"))
		{
			// 方向を設定
			if (ImGui::TreeNode("Direction"))
			{
				ImGui::DragFloat("X", &m_VecDir[1].x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Y", &m_VecDir[1].y, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Z", &m_VecDir[1].z, 0.1f, -1.0f, 1.0f);
				ImGui::TreePop();
			}
			// 方向を設定
			if (ImGui::TreeNode("Col"))
			{
				ImGui::DragFloat("R", &m_col[1].r, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("G", &m_col[1].g, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("B", &m_col[1].b, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("A", &m_col[1].a, 0.1f, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// サブカメラ2情報
		if (ImGui::TreeNode("SubLightInfo_2"))
		{
			// 方向を設定
			if (ImGui::TreeNode("Direction"))
			{
				ImGui::DragFloat("X", &m_VecDir[2].x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Y", &m_VecDir[2].y, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Z", &m_VecDir[2].z, 0.1f, -1.0f, 1.0f);
				ImGui::TreePop();
			}
			// 方向を設定
			if (ImGui::TreeNode("Col"))
			{
				ImGui::DragFloat("R", &m_col[2].r, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("G", &m_col[2].g, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("B", &m_col[2].b, 0.1f, 0.0f, 1.0f);
				ImGui::DragFloat("A", &m_col[2].a, 0.1f, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// 区切り線
		ImGui::Separator();
	}
	// カウントライト
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ライトの拡散光を設定
		m_light[nCntLight].Diffuse = m_col[nCntLight];
		// ライトの方向の設定
		D3DXVec3Normalize(&Direct, &m_VecDir[nCntLight]);		// 正規化
		m_light[nCntLight].Direction = Direct;					// ライトに代入
		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);
	}
}
#endif // _DEBUG

// ----------------------------------------
// 生成処理
// ----------------------------------------
CLight * CLight::Create(void)
{
	// 変数宣言
	CLight * pLight;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pLight = new CLight();
	// 初期化処理
	pLight->Init();
	// 生成したオブジェクトを返す
	return pLight;
}

// ----------------------------------------
// ライト情報取得処理
// ----------------------------------------
D3DLIGHT9 & CLight::GetLight(TYPE type)
{
	return m_light[type];
}

// ----------------------------------------
// 色情報取得処理
// ----------------------------------------
D3DXCOLOR & CLight::GetCol(TYPE type)
{
	return m_col[type];
}

// ----------------------------------------
// 方向情報取得処理
// ----------------------------------------
D3DVECTOR & CLight::GetVec(TYPE type)
{
	return 	m_light[type].Direction;
}
