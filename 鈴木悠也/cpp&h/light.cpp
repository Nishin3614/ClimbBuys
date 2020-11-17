//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#include "light.h"
#include "manager.h"

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CLight::CLight()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 Direct = D3DVECTOR3_ZERO;
	// �J�E���g���C�g
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_light[nCntLight].Diffuse = m_col[nCntLight];

		// ���C�g�̕����̐ݒ�
		D3DXVec3Normalize(&Direct, &m_VecDir[nCntLight]);		// ���K��
		m_light[nCntLight].Direction = Direct;					// ���C�g�ɑ��

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CLight::~CLight()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CLight::Init(void)
{
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CLight::Uninit(void)
{
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CLight::Update(void)
{
}
#ifdef _DEBUG
// ----------------------------------------
// �f�o�b�O����
// ----------------------------------------
void CLight::Debug(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 Direct = D3DVECTOR3_ZERO;
	// �J�����̏��
	if (ImGui::CollapsingHeader("Light"))
	{
		// ���C���J�������
		if (ImGui::TreeNode("MainLightInfo"))
		{
			// ������ݒ�
			if (ImGui::TreeNode("Direction"))
			{
				ImGui::DragFloat("X", &m_VecDir[0].x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Y", &m_VecDir[0].y, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Z", &m_VecDir[0].z, 0.1f, -1.0f, 1.0f);
				ImGui::TreePop();
			}
			// ������ݒ�
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
		// �T�u�J����1���
		if (ImGui::TreeNode("SubLightInfo_1"))
		{
			// ������ݒ�
			if (ImGui::TreeNode("Direction"))
			{
				ImGui::DragFloat("X", &m_VecDir[1].x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Y", &m_VecDir[1].y, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Z", &m_VecDir[1].z, 0.1f, -1.0f, 1.0f);
				ImGui::TreePop();
			}
			// ������ݒ�
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
		// �T�u�J����2���
		if (ImGui::TreeNode("SubLightInfo_2"))
		{
			// ������ݒ�
			if (ImGui::TreeNode("Direction"))
			{
				ImGui::DragFloat("X", &m_VecDir[2].x, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Y", &m_VecDir[2].y, 0.1f, -1.0f, 1.0f);
				ImGui::DragFloat("Z", &m_VecDir[2].z, 0.1f, -1.0f, 1.0f);
				ImGui::TreePop();
			}
			// ������ݒ�
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
		// ��؂��
		ImGui::Separator();
	}
	// �J�E���g���C�g
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ���C�g�̊g�U����ݒ�
		m_light[nCntLight].Diffuse = m_col[nCntLight];
		// ���C�g�̕����̐ݒ�
		D3DXVec3Normalize(&Direct, &m_VecDir[nCntLight]);		// ���K��
		m_light[nCntLight].Direction = Direct;					// ���C�g�ɑ��
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);
	}
}
#endif // _DEBUG

// ----------------------------------------
// ��������
// ----------------------------------------
CLight * CLight::Create(void)
{
	// �ϐ��錾
	CLight * pLight;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pLight = new CLight();
	// ����������
	pLight->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pLight;
}

// ----------------------------------------
// ���C�g���擾����
// ----------------------------------------
D3DLIGHT9 & CLight::GetLight(TYPE type)
{
	return m_light[type];
}

// ----------------------------------------
// �F���擾����
// ----------------------------------------
D3DXCOLOR & CLight::GetCol(TYPE type)
{
	return m_col[type];
}

// ----------------------------------------
// �������擾����
// ----------------------------------------
D3DVECTOR & CLight::GetVec(TYPE type)
{
	return 	m_light[type].Direction;
}
