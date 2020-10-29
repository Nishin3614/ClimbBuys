// ------------------------------------------
//
// �����_�����O�����̐���[renderer.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------
#include "renderer.h"
#include "scene.h"
#include "scene_load.h"
#include "fade.h"
#include "game.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "keyboard.h"
#include "mouse.h"
#include "connectblock.h"

// �e�X�g
#include "3Dmap.h"

// ------------------------------------------
//
// �}�N���֐�
//
// ------------------------------------------

// ------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------

// ------------------------------------------
//
// �ÓI�ϐ��錾
//
// ------------------------------------------
#ifdef _DEBUG
CDebugproc * CRenderer::m_debugproc = NULL;
#endif
bool CRenderer::m_bDebug = false;				// �f�o�b�O���[�h

D3DXVECTOR3 g_f = D3DVECTOR3_ZERO;
// ------------------------------------------
// �R���X�g���N�^
// ------------------------------------------
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pDevice = NULL;
}

// ------------------------------------------
// �f�X�g���N�^
// ------------------------------------------
CRenderer::~CRenderer()
{
}

// ------------------------------------------
// ����������
// ------------------------------------------
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// �ϐ��錾
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));								// ���[�N���[���N���A
	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;								// �Q�[����ʃT�C�Y(��)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// �Q�[����ʃT�C�Y(����)
	m_d3dpp.BackBufferFormat = d3ddm.Format;							// �o�b�N�o�b�t�@�̌`��
	m_d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	m_d3dpp.EnableAutoDepthStencil = TRUE;								// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;						// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	m_d3dpp.Windowed = bWindow;											// �E�B���h�E���[�h
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// �C���^�[�o��(VSync��҂��ĕ`��)
	// m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �N���C�A���g�̈�𒼂��ɍX�V����

	// Direct3D�f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,									// �f�o�C�X�^�C�v
		hWnd,											// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,			// �f�o�C�X�쐬����̑g�ݍ��킹
		&m_d3dpp,											// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&m_pDevice)))									// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_���[�X�e�[�g�̐ݒ�
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���X�e�[�W�X�e�[�g�p�����[�^�̐ݒ�
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// ImGui�̃Z�b�g�A�b�v
	ImGui_SetUp(hWnd);
	// �f�o�b�O�v���V�[�W���[
	m_debugproc = new CDebugproc;
	// �f�o�b�O�\��������
	m_debugproc->Init(m_pDevice);
#endif
	// �V�[���̓ǂݍ���
	CScene_load::LoadAll();
	// �J�����̐���
	m_pCamera = CCamera::Create();
	// ���C�g�̐���
	m_pLight = CLight::Create();
	return S_OK;
}

// ------------------------------------------
// �I������
// ------------------------------------------
void CRenderer::Uninit(void)
{
	// �V�[���e�q�̃����[�X����
	CScene::ReleaseAll();
	// �ǂݍ��񂾂��̂�j�����鏈��
	CScene_load::UnLoadAll();
	// �J�����̐���
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	// ���C�g�̐���
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	// �f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// D3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
#ifdef _DEBUG
	// ImGui�̏I��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �f�o�b�O�\��
	if (m_debugproc != NULL)
	{
		m_debugproc->Uninit();
		delete m_debugproc;
		m_debugproc = NULL;
	}

#endif // _DEBUG
}

// ------------------------------------------
// �X�V����
// ------------------------------------------
void CRenderer::Update(void)
{
#ifdef _DEBUG
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("SideWindow");
#endif // _DEBUG
	// �J�����̍X�V
	m_pCamera->Update();
	// ���C�g�̍X�V
	m_pLight->Update();

#ifdef _DEBUG
	CDebugproc::Print("�f�o�b�O���[�h:F2\n");
	CDebugproc::Print("1�t���[���Đ�:L\n");
	// FPS�\��
	DrawFPS();
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_F1))
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	// �f�o�b�O���[�h
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_F2))
	{
		m_bDebug = !m_bDebug;
	}
	// �V�[��
	if (m_bDebug == false)
	{
		CScene::UpdateAll();
	}
	else
	{
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{
			CScene::UpdateAll();
		}
	}
	// ImGui�̍X�V
	UpdateImGui();
#else // _RELEASE
	// �V�[��
	if (m_bDebug == false)
	{
		CScene::UpdateAll();
	}
	else
	{
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{
			CScene::UpdateAll();
		}
	}
#endif // _DEBUG || _RELEASE
}

// -----------------------
// �`�揈��
// --------------------------
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	m_pDevice->Clear(0, NULL, (D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// Direct3D�ɂ��`��J�n
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// �J�����̐ݒ�
		m_pCamera->Set();
		// �V�[��
		CScene::DrawAll();
		// �x�[�X���[�h�`��
		CManager::GetBaseMode()->Draw();
		// �J�����̕`��
		m_pCamera->Draw();

		CManager::GetFade()->Draw();

#ifdef _DEBUG
		// �f�o�b�O�\���̕`��
		CDebugproc::Draw();

		// ImGui�̕`��
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif
		// Direct3D�ɂ��`��I��
		m_pDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt���[�g�o�b�t�@�̓���ւ�
	HRESULT result = m_pDevice->Present(NULL, NULL, NULL, NULL);
#ifdef _DEBUG
	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && m_pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
#endif

}

// -----------------------
// �`�揈��
// --------------------------
void CRenderer::LoadDraw(void)
{
	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	m_pDevice->Clear(0, NULL, (D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(1, 100, 155, 255), 1.0f, 0);

	// Direct3D�ɂ��`��J�n
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// Direct3D�ɂ��`��I��
		m_pDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt���[�g�o�b�t�@�̓���ւ�
	HRESULT result = m_pDevice->Present(NULL, NULL, NULL, NULL);
}

// ------------------------------------------
// �`���ԏ���
// ------------------------------------------
void CRenderer::SetType(
	TYPE type
)
{
	switch (type)
	{
	case TYPE_NORMAL:
		// �����_���[�X�e�[�g�̐ݒ�
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);	// �A���t�@�e�X�g�L��
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);				// �A���t�@�l�̊�ݒ�(����傫���ƕ`�悷��)
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// �󋵂ɉ����ăA���t�@�e�X�g�����s����(�V�s�N�Z�� > ���݂̃s�N�Z��)
		break;
		// �J�����O��ʏ�ɖ߂�
	case TYPE_CULLNORMAL:
		// �����_���[�X�e�[�g�̐ݒ�
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		break;
	case TYPE_CULLBACK:
		// �����_���[�X�e�[�g�̐ݒ�
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		break;
	case TYPE_CULLNULL:
		// �����_���[�X�e�[�g�̐ݒ�
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		break;
	case TYPE_NORMALMIX:
		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case TYPE_ADDMIX:
		// �����_�[�X�e�[�g(���Z��������)
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case TYPE_DECMIX:
		break;
	case TYPE_RIGHTINGON:
		// ���C�e�B���O���[�h�L��
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		break;
	case TYPE_RIGHTINGOFF:
		// ���C�e�B���O���[�h����
		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		break;
	case TYPE_ZBUFFON:
		//Z�o�b�t�@�@�L��
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		break;
	case TYPE_ZBUFFOFF:
		//Z�o�b�t�@�@�L��
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		break;
	case TYPE_ZTEST_DEFAULT:
		//Z�e�X�g �ʏ�
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		break;
	case TYPE_ZTEST_OFF:
		//Z�e�X�g ����
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		break;
	case TYPE_3DEFFECT_ON:
		// ���C�e�B���O���[�h����
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		//Z�o�b�t�@�@�L���@����
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		// �����_�[�X�e�[�g(���Z��������)
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case TYPE_3DEFFECT_OFF:
		//�A���t�@�e�X�g�߂�
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		//Z�o�b�t�@�@�L��
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		// ���C�e�B���O���[�h�L��
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
		//�t�H�OON
	case TYPE_FOG_ON:
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		break;
		//�t�H�OOFF
	case TYPE_FOG_OFF:
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		break;
	default:
		break;
	}
}


// ------------------------------------------
// ��������
//	blend	: �������Z
// ------------------------------------------
void CRenderer::SetBlend(
	BLEND const &blend	// �������Z
)
{
	// �������Z
	switch (blend)
	{
		// �ʏ퍇��
	case BLEND_NORMAL:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;
		// ����������
	case BLEND_TRANSLUCENT:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
		// ���Z����
	case BLEND_ADD:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
		// ���Z����������
	case BLEND_ADD_TRANSLUCENT:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
		// ���Z����
	case BLEND_SUBTRACTION:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		break;
		// ��Z����1
	case BLEND_MULTIPUL1:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		break;
		// ��Z����2
	case BLEND_MULTIPUL2:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		break;
		// �X�N���[������
	case BLEND_SCREEN:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
		// ���o�[�X����
	case BLEND_REVERSE:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		break;
	default:
		break;
	}
}

// ------------------------------------------
// �f�o�C�X�擾����
// ------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pDevice;
}

// ------------------------------------------
// �J�����擾����
// ------------------------------------------
CCamera * CRenderer::GetCamera(void)
{
	return m_pCamera;
}

#ifdef _DEBUG
// ------------------------------------------
// ImGui�̃Z�b�g�A�b�v����
// ------------------------------------------
void CRenderer::ImGui_SetUp(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// ImGui�̊�{�ݒ�
	io.MousePos;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos;     // Enable Keyboard Controls
	io.FontAllowUserScaling = true;
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("C:\\WINDOWS\\Fonts\\MEIRYO.TTC", 18.0f, &config, glyphRangesJapanese);

	// �w�i�F�̐ݒ�
	ImGui::StyleColorsDark();
	// �v���b�g�t�H�[���ɍ��킹���Z�b�g�A�b�v
	ImGui_ImplWin32_Init(hWnd);
	// �����_���[���o�C���h
	ImGui_ImplDX9_Init(m_pDevice);
	// �J���[�ݒ�
	m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}
// ------------------------------------------
// �`���ԏ���
// ------------------------------------------
void CRenderer::CleanupDeviceD3D()
{
	if (m_pDevice) { m_pDevice->Release(); m_pDevice = NULL; }
	if (m_pD3D) { m_pD3D->Release(); m_pD3D = NULL; }
}

// ------------------------------------------
// �`���ԏ���
// ------------------------------------------
void CRenderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = m_pDevice->Reset(&m_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// ------------------------------------------
// ImGui�̍X�V����
// ------------------------------------------
void CRenderer::UpdateImGui(void)
{
	// �f�o�b�O���
	CCalculation::ImG_DebugInfo();
	// �J�������̐ݒ�
	m_pCamera->Debug();
	// ���C�g�̏��̐ݒ�
	m_pLight->Debug();
	// �����u���b�N�̐ÓI�ȃf�o�b�O����
	CConnectblock::StaticDebug();
	// ���C�g���̐ݒ�
	CDebugproc::Print("�}�E�X�ʒu(%.1f,%.1f)\n", (float)CManager::GetMouse()->GetX(), (float)CManager::GetMouse()->GetY());
	CDebugproc::Print("ImGui�̃}�E�X�ʒu(%.1f,%.1f)\n",ImGui::GetMousePos().x,ImGui::GetMousePos().y);

	// �e�X�g 3D�}�b�v�̏��ݒ�
	C3DMap::Debug();


	// ImGui�̍X�V�I��
	ImGui::End();
	ImGui::EndFrame();
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(m_clear_color.x*255.0f), (int)(m_clear_color.y*255.0f), (int)(m_clear_color.z*255.0f), (int)(m_clear_color.w*255.0f));
}

// ------------------------------------------
// FPS�`�揈��
// ------------------------------------------
void CRenderer::DrawFPS(void)
{
	int nCntFPS = GetFPS();

	CDebugproc::Print("FPS:%d\n",nCntFPS);
}
#endif // _DEBUG