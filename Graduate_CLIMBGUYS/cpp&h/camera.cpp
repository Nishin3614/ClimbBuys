// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �J�������� [camera.cpp]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CAMERA_FILE ("data/LOAD/camerainfo.txt")
#define CAMERA_INIT (0.03f)

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�ϐ��錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::LOAD	CCamera::m_load[TYPE_MAX] = {};	// ���ۑ�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::CCamera()
{
	// �J�����̏�����
	m_posV = D3DXVECTOR3(1.0f, 100.0f, -100.0f);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = m_posR;
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_type = TYPE_FOLLOW;
	m_fLength = m_load[m_type].fLengh;
	m_fHeight = m_load[m_type].fHeight;
	m_RotSpeed.x = CAMERA_INIT;				// ��]�X�s�[�h
	m_RotSpeed.y = -CAMERA_INIT;			// ��]�X�s�[�h
	m_fIntertia = 0.1f;
	m_bSet = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update(void)
{
	Update_Play();

#ifdef _DEBUG
	/*
	CDebugproc::Print("��]��X%.3f\n", m_rot.x);
	CDebugproc::Print("��]��X%.3f\n", m_posVDest.y);
	// CDebugproc::Print("�J�����̎��_x:%f,y:%f,z:%f\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	// CDebugproc::Print("�J�����̒����_x:%f,y:%f,z:%f\n", m_posR.x, m_posR.y, m_posR.z);
	// CDebugproc::Print("�J�����̊p�xx:%f,y:%f,z:%f\n", m_rot.x, m_rot.y, m_rot.z);
	*/
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���̍X�V����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update_Play(void)
{
	Perfom_Normal();
	// �ړI�̊p�x���猻�݂̊p�x�̍�
	m_rotDiff =
		m_rotDest - m_rot;
	// ��]���
	CCalculation::Rot_One_Limit(m_rotDiff.x);
	CCalculation::Rot_One_Limit(m_rotDiff.y);
	CCalculation::Rot_One_Limit(m_rotDiff.z);

	// �p�x�Ɋ���
	m_rot +=
		m_rotDiff * m_fIntertia;
	// ��]���
	CCalculation::Rot_One_Limit(m_rot.x);
	CCalculation::Rot_One_Limit(m_rot.y);
	CCalculation::Rot_One_Limit(m_rot.z);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʏ��Ԃ̃J�������o
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Perfom_Normal(void)
{
	// ��]
	Rot();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Draw(void)
{
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̃f�o�b�O����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Debug(void)
{
	// �J�����̏��
	if (ImGui::CollapsingHeader("Camera"))
	{
		// ����
		if (ImGui::TreeNode("Intertia"))
		{
			ImGui::DragFloat("Intertia", &m_fIntertia, 0.01f,0.1f,0.01f);
			ImGui::TreePop();
		}
		// ��]�X�s�[�h
		if (ImGui::TreeNode("RotSpeed"))
		{
			ImGui::DragFloat("x", &m_RotSpeed.x, 0.01f, 0.03f, 0.1f);
			ImGui::DragFloat("y", &m_RotSpeed.y, 0.01f, 0.03f, 0.1f);
			ImGui::TreePop();
		}
		// ����
		if (ImGui::TreeNode("LENGH&HEIGHT"))
		{
			ImGui::DragFloat("lengh", &m_fLength, 0, 5000, 1.0f);
			ImGui::DragFloat("height", &m_fHeight, 0, 5000, 1.0f);
			ImGui::TreePop();
		}
	}
	/*
	CDebugproc::Print("PosV(%.1f,%.1f,%.1f)\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugproc::Print("m_posVDest(%.1f,%.1f,%.1f)\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	CDebugproc::Print("m_posVDiff(%.1f,%.1f,%.1f)\n", m_posVDiff.x, m_posVDiff.y, m_posVDiff.z);
	*/
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera * CCamera::Create(void)
{
	// �ϐ��錾
	CCamera * pCamera;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pCamera = new CCamera();
	// ����������
	pCamera->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pCamera;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCamera::Load(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntObj = 0;		// �V���h�E�}�b�s���O�J�E���g
	char cRaedText[128] = {};	// �����Ƃ��ēǂݎ��p
	char cHeadText[128] = {};	// ��r����悤
	char cDie[128] = {};			// �s�K�v�ȕ���

	// �t�@�C���J
	pFile = fopen(CAMERA_FILE, "r");

	// �J����
	if (pFile != NULL)
	{
		// �G���h�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "END") != 0)
		{
			// ������
			cHeadText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// �}�e���A���Z�b�g������
			if (strcmp(cHeadText, "TYPE") == 0)
			{
				sscanf(cRaedText, "%s %d",
					&cDie,
					&m_load[nCntObj].nType
				);
				// �G���h�}�e���A���Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_TYPE") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// ��]���ǂݍ���
					if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&m_load[nCntObj].rot.x,
							&m_load[nCntObj].rot.y,
							&m_load[nCntObj].rot.z);
					}
					// �I�t�Z�b�g���ǂݍ���
					else if (strcmp(cHeadText, "OFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&m_load[nCntObj].offset.x,
							&m_load[nCntObj].offset.y,
							&m_load[nCntObj].offset.z
						);
					}
					// �������ǂݍ���
					else if (strcmp(cHeadText, "LENGH") == 0)
					{
						sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
							&m_load[nCntObj].fLengh);
					}
					// �������ǂݍ���
					else if (strcmp(cHeadText, "HEIGHT") == 0)
					{
						sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
							&m_load[nCntObj].fHeight);
					}
				}
				// �����艺�Ȃ�
				if (nCntObj < TYPE_MAX)
				{
					// �I�u�W�F�N�g�̍X�V
					nCntObj++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�J�����̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �j��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Unload(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��񏉊���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::InfoInit(
	D3DXVECTOR3 const &pos,	// �ʒu���
	D3DXVECTOR3 const &rot	// ��]���
)
{
	// ��]
	m_rot = rot;
	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posRDest.x = pos.x + sinf(m_rot.y + D3DX_PI) * m_load[m_type].offset.x;
	m_posRDest.y = pos.y + m_load[m_type].offset.y;
	m_posRDest.z = pos.z + cosf(m_rot.y + D3DX_PI) * m_load[m_type].offset.x;
	// �����_
	m_posR = m_posRDest;
	// ���_
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	m_posVDest = m_posV;
	m_rotDest = m_rot;
	// ������񏉊���

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X //
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,							// (�e���r���_)
		D3DXToRadian(45.0f),						// ����p(��p)
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,										// MearZ�l(�߂��r���[���ʂ�Z�l)
		1500000.0f);								// FarZ�l(�����r���[���ʂ�Z�l)�����l��1000

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X //
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(
		&m_mtxView,	// (�J�����}�����_)
		&m_posV,	// �J�����̎��_
		&m_posR,	// �J�����̒��ړ_
		&m_posU);	// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̖ړI�����_�ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetPosDestR(
	D3DXVECTOR3 const & pos,	// �ʒu���
	D3DXVECTOR3 const & rot		// ��]���
)
{
	m_fLength = m_load[m_type].fLengh;
	m_fHeight = m_load[m_type].fHeight;
	m_rotDest = rot + m_load[m_type].rot;
	// �����_���ɉ�] //

	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posRDest.x = pos.x + m_load[m_type].offset.x;
	m_posRDest.y = pos.y + m_load[m_type].offset.y;
	m_posRDest.z = pos.z + m_load[m_type].offset.z;
	//-------------------- ���_�ړ� --------------------//
	// �ړI�n���猻�ݒn�̋���(���_)
	m_posRDiff =
		m_posRDest - m_posR;

	// �����_�Ɋ���
	m_posR.x += m_posRDiff.x * m_fIntertia;
	m_posR.y += m_posRDiff.y * m_fIntertia;
	m_posR.z += m_posRDiff.z * m_fIntertia;

	// �ړI�̎��_
	m_posVDest.x =
		m_posRDest.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posRDest.y + cosf(m_rot.x) * m_fHeight;
	m_posVDest.z =
		m_posRDest.z + cosf(m_rot.y) * m_fLength;
	// �ړI�n���猻�ݒn�̋���(�����_)
	m_posVDiff =
		m_posVDest - m_posV;

	// �����_�Ɋ���
	m_posV.x += m_posVDiff.x * m_fIntertia;
	m_posV.y += m_posVDiff.y * m_fIntertia;
	m_posV.z += m_posVDiff.z * m_fIntertia;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̖ړI�����_�ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetPosDestRPlayer(
	D3DXVECTOR3 const & pos,	// �ʒu���
	D3DXVECTOR3 const & rot		// ��]���
)
{
	m_fLength = m_load[m_type].fLengh;
	m_fHeight = m_load[m_type].fHeight;
	// �����_���ɉ�] //

	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posRDest.x = pos.x + m_load[m_type].offset.x;
	m_posRDest.y = pos.y + m_load[m_type].offset.y;
	m_posRDest.z = pos.z + m_load[m_type].offset.z;
	//-------------------- ���_�ړ� --------------------//
	// �ړI�n���猻�ݒn�̋���(���_)
	m_posRDiff =
		m_posRDest - m_posR;

	// �����_�Ɋ���
	m_posR.x += m_posRDiff.x * m_fIntertia;
	m_posR.y += m_posRDiff.y * m_fIntertia;
	m_posR.z += m_posRDiff.z * m_fIntertia;

	// �ړI�̎��_
	m_posVDest.x =
		m_posRDest.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posRDest.y + cosf(m_rot.x) * m_fHeight;
	m_posVDest.z =
		m_posRDest.z + cosf(m_rot.y) * m_fLength;
	// �ړI�n���猻�ݒn�̋���(�����_)
	m_posVDiff =
		m_posVDest - m_posV;

	// �����_�Ɋ���
	m_posV.x += m_posVDiff.x * m_fIntertia;
	m_posV.y += m_posVDiff.y * m_fIntertia;
	m_posV.z += m_posVDiff.z * m_fIntertia;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̒����_�ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetPosR(D3DXVECTOR3 const & pos, D3DXVECTOR3 const & rot)
{
	m_fLength = m_load[m_type].fLengh;
	m_fHeight = m_load[m_type].fHeight;
	m_rot = rot + m_load[m_type].rot;
	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posRDest.x = pos.x + m_load[m_type].offset.x;
	m_posRDest.y = pos.y + m_load[m_type].offset.y;
	m_posRDest.z = pos.z + m_load[m_type].offset.z;
	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posR = m_posRDest;
	// �ړI�̎��_
	m_posVDest.x =
		m_posRDest.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posRDest.y + cosf(m_rot.x) * m_fHeight;
	m_posVDest.z =
		m_posRDest.z + cosf(m_rot.y) * m_fLength;
	// �����_�Ɋ���
	m_posV = m_posVDest;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̃^�C�v
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetType(
	TYPE type
)
{
	if (type == TYPE_FOLLOW)
	{
		m_rotOrigin = m_rotDest;
	}
	if (type == TYPE_FOLLOW)
	{
		m_rotDest = m_rotOrigin;
	}
	if (type > TYPE_MAX ||
		type < 0)
	{
		CCalculation::Messanger("�^�C�v�����������ł��B");
	}
	m_type = type;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̕���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����X�s�[�h�̎擾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR2 & CCamera::GetRotSpeed(void)
{
	return m_RotSpeed;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����ݒ�̏�����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetCameraInfo(void)
{
	m_RotSpeed.x = CAMERA_INIT;				// ��]�X�s�[�h
	m_RotSpeed.y = -CAMERA_INIT;				// ��]�X�s�[�h
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ��]
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Rot(void)
{
	// �W���C�p�b�h�p //
	// �p�b�h�p //
	int nValueH, nValueV;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fRot;			// �ړ����x

	// �W���C�p�b�h�̏��� //
	if (CManager::GetJoy() != NULL)
	{
		// �Q�[���p�b�h�̃X�e�B�b�N�����擾
		CManager::GetJoy()->GetStickRight(0, nValueH, nValueV);
		if (nValueH != 0 || nValueV != 0)
		{
			// y��]�̊p�x���v�Z
			if (nValueH < 0)
			{
				fRot = abs(nValueH) / 1024.0f * m_RotSpeed.y;
				m_rotDest.y += fRot;

			}
			else if (nValueH > 0)
			{
				fRot = abs(nValueH) / 1024.0f * m_RotSpeed.y;
				m_rotDest.y -= fRot;
			}
			// x��]�̊p�x���v�Z
			if (nValueV < 0)
			{
				fRot = abs(nValueV) / 1024.0f * m_RotSpeed.x;
				m_rotDest.x += fRot;

			}
			else if (nValueV > 0)
			{
				fRot = abs(nValueV) / 1024.0f * m_RotSpeed.x;
				m_rotDest.x -= fRot;

			}
			// -3.14�`3.14�ɉ�]�������Œ肳����
			CCalculation::Rot_One_Limit(m_rotDest.y);
			// -3.14�`3.14�ɉ�]�������Œ肳����
			CCalculation::Rot_One_Limit(m_rotDest.x);
		}
	}
	// �����_���ɉ�] //
	// �����v���
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
	{
		// ���Z
		m_rotDest.y -= m_RotSpeed.y;
		// -3.14�`3.14�ɉ�]�������Œ肳����
		CCalculation::Rot_One_Limit(m_rotDest.y);
	}
	// ���v���
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_LEFT))
	{
		// ���Z
		m_rotDest.y += m_RotSpeed.y;
		// -3.14�`3.14�ɉ�]�������Œ肳����
		CCalculation::Rot_One_Limit(m_rotDest.y);
	}
	// ��
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_UP))
	{
		// ���Z
		m_rotDest.x -= m_RotSpeed.x;
		// -3.14�`3.14�ɉ�]�������Œ肳����
		CCalculation::Rot_One_Limit(m_rotDest.x);
	}
	// ��
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_DOWN))
	{
		// ���Z
		m_rotDest.x += m_RotSpeed.x;
		// -3.14�`3.14�ɉ�]�������Œ肳����
		CCalculation::Rot_One_Limit(m_rotDest.x);
	}

	// x��]�̌��E�n�_
	if (m_rotDest.x > -0.1f)
	{
		m_rotDest.x = -0.1f;
	}
	if (m_rotDest.x < -2.5f)
	{
		m_rotDest.x = -2.5f;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�������[�V�����̐ݒ�
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetCamera_Motion(
	D3DXVECTOR3 const &posR,	// �����_�̈ʒu
	D3DXVECTOR3 const &rot,		// ��]
	float const & fLength,		// ����
	float const & fHeight,		// ����
	float const & fIntertia		// �����̔䗦
)
{
	// �����_
	m_posRDest = posR;
	// ��]
	m_rotDest = rot;
	// ����
	m_fLength = fLength;
	// ����
	m_fHeight = fHeight;
	// �����̔䗦
	m_fIntertia = fIntertia;
	// ��]���
	CCalculation::Rot_One_Limit(m_rotDest.x);
	CCalculation::Rot_One_Limit(m_rotDest.y);
	CCalculation::Rot_One_Limit(m_rotDest.z);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J�����̉��o
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetCamera_Perfom(
	D3DXVECTOR3 const & posR,
	D3DXVECTOR3 const & rot,
	float const & fLength,
	float const & fHeight
)
{
	// ��]
	m_rot = rot;
	// �Q�[���̃J�����̉�]�̖ړI�n
	m_posRDest.x = posR.x;
	m_posRDest.y = posR.y;
	m_posRDest.z = posR.z;
	// �����_
	m_posR = m_posRDest;
	// ����
	m_fLength = fLength;
	// ����
	m_fHeight = fHeight;
	// ���_
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
// �J�������ύX
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::ChangeInfo(void)
{

}