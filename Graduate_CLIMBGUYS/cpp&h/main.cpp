// ----------------------------------------------------------------------------------------------------
//
// main�����̐���[main.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
//#include <iomanip>
#include <cstdlib>
#include <crtdbg.h>
//#include <iomanip>
//#include <libucrt.lib>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__,__LINE__)
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------
//
// �}�N���֐���`
//
// ----------------------------------------------------------------------------------------------------
#define CLASS_NAME "WindowClass"	// �E�B���h�E�N���X��
#define WINDOW_NAME "CLIMBGUYS"	// �E�B���h�E��(�L�����Z���\��)
#define ID_BUTTON000 (101)
#define SCREEN_WINDOWS (true)		// �X�N���[���̃E�B���h�E�T�C�Y

// ----------------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc
(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�B���h�E�v���V�[�W���֐�

#ifdef _DEBUG
int			g_nCountFPS = 0;	// FPS�J�E���^
#endif
bool		g_bDestWind = false;	// �Q�[�����I������

// ----------------------------------------------------------------------------------------------------
//
// ���C���֐�
//
// ----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#if ERROW_ACTION
	// ���������[�N�����ݒ�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(99);
#endif
	// ���������[�N���o
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(40474);
	// ���������[�N�����ݒ�
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(39110);

	// �V�[�h�l�����Ԃɂ���
	srand((unsigned)time(NULL));

	// �ϐ��錾
	HWND hWnd;				// �E�B���h�E�n���h��
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�

	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwExecLastTime;		// �Ō�ɏ�����������
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;
	CManager *pManager = NULL;			// �����_�����O�N���X
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	// Window�N���X�̒ǉ��ݒ�
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y�w��
		CS_CLASSDC,						// �\������E�B���h�E�̃X�^�C���ݒ�
		WindowProc,						// �E�B���h�E�v���V�[�W���̃A�h���X(�֐���)���w��
		0,								// �ʏ�͎w�肵�Ȃ��̂�0�w��
		0,								// �ʏ�͎w�肵�Ȃ��̂�0�w��
		hInstance,						// Windows�̈����̃C���X�^���X�n���h���w��
		LoadIcon(NULL,IDI_APPLICATION),	// �^�X�N�o�[�Ɏg�p����A�C�R���w��
		LoadCursor(NULL,IDC_ARROW),		// �g�p����}�E�X�J�[�\���w��
		(HBRUSH)(COLOR_WINDOW - 1),		// �E�B���h�E�N���C�A���g�̈�̐F�ݒ�
		NULL,							// ���j���[�w��
		CLASS_NAME,						// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �������A�C�R���w��
	};
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);
	// �w�肵���N���C�A���g�̈�ɕ����v�Z����
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	// �E�B���h�E�N���X����
	hWnd = CreateWindowEx    // Ex�͊g��
	(
		0,							// �g���E�B���h�E�X�^�C��
		CLASS_NAME,					// �E�B���h�E�N���X��
		WINDOW_NAME,				// �E�B���h�E��
		WS_OVERLAPPEDWINDOW,		// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�B���h�E����x���W
		CW_USEDEFAULT,				// �E�B���h�E����y���W
		(rect.right - rect.left),		 // �E�B���h�E��
		(rect.bottom - rect.top),		 // �E�B���h�E����
		NULL,				 // �e�E�B���h�E�n���h��
		NULL,				 // ���j���[�������͎q�E�B���h�EID
		hInstance,			 // �C���X�^���X�n���h��
		NULL				 // �E�B���h�E�쐬�f�[�^
	);

#if ERROW_ACTION
	// �R���\�[���Ăяo��
	AllocConsole();
	FILE *fp = NULL;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	std::cout << "";
#endif


	// �}�l�[�W���̃k���`�F�b�N
	if (pManager == NULL)
	{
		// �������̐���
		pManager = new CManager();	// �}�l�[�W���̐���
	}

	// ����������(�E�B���h�E�̐�����ɍs��)
	if (FAILED(pManager->Init(hWnd, SCREEN_WINDOWS, hInstance)))
	{
		// �I������
		pManager->Uninit();
		// �}�l�[�W��
		delete pManager;
		pManager = NULL;
		return -1;	// ���s�����甲����
	}

	// �E�B���h�E���� //
	ShowWindow(hWnd, nCmdShow); // �w�肳�ꂽ�E�B���h�E�̕\���ݒ�
	UpdateWindow(hWnd);			// �E�B���h�E�̃N���C�A���g�̈�X�V


	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�ő���
			}
		}

		else
		{
			// DirectX�̏���
			dwCurrentTime = timeGetTime();		// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPS�𑪒�
#endif

				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�
				// �X�V����
				pManager->Update();
				// �`�揈��
				pManager->Draw();
				dwFrameCount++;					// �J�E���g�����Z
				// true�̂��̉�ʂ�j������
				if (g_bDestWind == true)
				{
					DestroyWindow(hWnd);
				}
			}
		}
	}
	/* ����������� */
	// �}�l�[�W���̃k���`�F�b�N
	if (pManager != NULL)
	{

		// �I������
		pManager->Uninit();
		// �}�l�[�W��
		delete pManager;
		pManager = NULL;
	}
	//�E�B���h�E�N���X�̓o�^����� //
	UnregisterClass(CLASS_NAME, wcex.hInstance);

#if ERROW_ACTION
	// ���������[�N���|�[�g
	_CrtDumpMemoryLeaks();
#endif
	return (int)msg.wParam;
}

// ----------------------------------------------------------------------------------------------------
// �I���ݒ�֐�
// ----------------------------------------------------------------------------------------------------
void SetDestWind(bool const &bDest)
{
	g_bDestWind = bDest;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------
// ImGui�v���V�[�W���[�֐�
// ----------------------------------------------------------------------------------------------------
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
// ----------------------------------------------------------------------------------------------------
// �E�B���h�E�v���V�[�W���֐�
// ----------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
#endif // _DEBUG

	switch (uMsg)
	{
	// ���N���b�N�� //
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;
	// �L�[�������ꂽ�Ƃ��̃��b�Z�[�W //
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			// �E�B���h�E��j��(WM_DESTROY����)
			DestroyWindow(hWnd);
		}
		break;
	// �~�I�� //
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	//�E�B���h�E�j�����b�Z�[�W //
	case WM_DESTROY:
		// WN_QUIT���b�Z�[�W��Ԃ��B�E�B���h�E���I��点��
		PostQuitMessage(0);
		break;
	// ���̑� //
	default:
		break;
	}

	// ����̏������ //
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // �I��点��ȊO��msg
}
#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------
// FPS�擾�֐�
// ----------------------------------------------------------------------------------------------------
int GetFPS(void)
{
	return g_nCountFPS;
}

// ----------------------------------------------------------------------------------------------------
// �R���\�[���\��
// ----------------------------------------------------------------------------------------------------
void DispConsol()
{
	AllocConsole();

	FILE *fp = NULL;

	freopen_s(&fp, "CONOUT$", "w", stdout);

	/*
	cout << fixed;
	cout << setprecision(4);
	*/
}
#endif // _DEBUG
