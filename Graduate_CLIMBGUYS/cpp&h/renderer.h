// ------------------------------------------
//
// �����_�����O�����̐���[renderer.h]
// Author : Koki Nishiyama
//
// ------------------------------------------

#ifndef _RENDERER_H_
#define _RENDERER_H_	 // �t�@�C������������߂�

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// �}�N���֐�
//
// ------------------------------------------
#if ERROW_ACTION

#define IMGUI_DEBUG	(1)	// ImGui�̃f�o�b�O����

#endif // ERROW_ACTION

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------
class CDebugproc;
class CCamera;
class CLight;

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CRenderer
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_NORMAL = 0,
		// �J�����O�m�[�}��
		TYPE_CULLNORMAL,
		// �J�����O(��)
		TYPE_CULLBACK,
		// �J�����O�Ȃ�
		TYPE_CULLNULL,
		// �ʏ퍇��
		TYPE_NORMALMIX,
		// ���Z����
		TYPE_ADDMIX,
		// ���Z����
		TYPE_DECMIX,
		// ���C�e�B���O�I��
		TYPE_RIGHTINGON,
		// ���C�e�B���O�I�t
		TYPE_RIGHTINGOFF,
		// Z�o�b�t�@�I��
		TYPE_ZBUFFON,
		// Z�o�b�t�@�I�t
		TYPE_ZBUFFOFF,
		// Z�e�X�g�ʏ�
		TYPE_ZTEST_DEFAULT,
		// Z�e�X�g����
		TYPE_ZTEST_OFF,
		// 3D�G�t�F�N�g�p�I��
		TYPE_3DEFFECT_ON,
		// 3D�G�t�F�N�g�p�I�t
		TYPE_3DEFFECT_OFF,
		// �t�H�OON
		TYPE_FOG_ON,
		// �t�H�OOFF
		TYPE_FOG_OFF,
		TYPE_MAX
	} TYPE;

	// �������Z
	typedef enum
	{
		BLEND_NORMAL = 0,		// �ʏ�
		BLEND_TRANSLUCENT,		// ������
		BLEND_ADD,				// ���Z����
		BLEND_ADD_TRANSLUCENT,	// ���Z������
		BLEND_SUBTRACTION,		// ���Z
		BLEND_MULTIPUL1,		// ��Z1
		BLEND_MULTIPUL2,		// ��Z2
		BLEND_SCREEN,			// �X�N���[��
		BLEND_REVERSE,			// ���o�[�X
		BLEND_MAX				// �ő吔
	} BLEND;

	/* �֐� */
	CRenderer();
	~CRenderer();
	HRESULT  Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// ���[�h���̕`�揈��
	void LoadDraw(void);
#if IMGUI_DEBUG

	void CleanupDeviceD3D();
	void ResetDevice();

#endif // IMGUI_DEBUG

	// �`����
	void SetType(TYPE type = TYPE_NORMAL);
	// ��������
	//	blend	: �������Z
	void SetBlend(
		BLEND const &blend = BLEND_TRANSLUCENT	// �������Z
	);
	// ���擾
	LPDIRECT3DDEVICE9 GetDevice(void);	// �f�o�C�X
	CCamera * GetCamera(void);			// �J����
	CLight * GetLight(void) { return m_pLight; }			// ���C�g
protected:

private:
	/* �ϐ� */
	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g�̃|�C���^
	LPDIRECT3DDEVICE9 m_pDevice;		// Direct3D�f�o�C�X�̃|�C���^

	CCamera * m_pCamera;				// �J�������
	CLight * m_pLight;					// ���C�g���
	static bool m_bDebug;				// �f�o�b�O���[�h
	D3DPRESENT_PARAMETERS    m_d3dpp;
#if IMGUI_DEBUG

	// ImGui�̃Z�b�g�A�b�v
	void ImGui_SetUp(HWND hWnd);
	// ImGui�̍X�V
	void UpdateImGui(void);
	ImVec4 m_clear_color;

#endif // IMGUI_DEBUG
#ifdef _DEBUG

	void DrawFPS(void);					// FPS�֐�
#endif // _DEBUG

	// �ϐ��錾 //
	static CDebugproc * m_debugproc;
};

#endif // !_RENDERER_H_