// ----------------------------------------------------------------------------------------------------
//
// main処理の説明[main.cpp]
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
// マクロ関数定義
//
// ----------------------------------------------------------------------------------------------------
#define CLASS_NAME "WindowClass"	// ウィンドウクラス名
#define WINDOW_NAME "CLIMBGUYS"	// ウィンドウ名(キャンセル表示)
#define ID_BUTTON000 (101)
#define SCREEN_WINDOWS (true)		// スクリーンのウィンドウサイズ

// ----------------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ----------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc
(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウィンドウプロシージャ関数

#ifdef _DEBUG
int			g_nCountFPS = 0;	// FPSカウンタ
#endif
bool		g_bDestWind = false;	// ゲームを終了する

// ----------------------------------------------------------------------------------------------------
//
// メイン関数
//
// ----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#if ERROW_ACTION
	// メモリリーク検索設定
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(99);
#endif
	// メモリリーク検出
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(40474);
	// メモリリーク検索設定
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(39110);

	// シード値を時間にする
	srand((unsigned)time(NULL));

	// 変数宣言
	HWND hWnd;				// ウィンドウハンドル
	MSG msg;				// メッセージを格納する変数

	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwExecLastTime;		// 最後に処理した時刻
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;
	CManager *pManager = NULL;			// レンダリングクラス
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	// Windowクラスの追加設定
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ指定
		CS_CLASSDC,						// 表示するウィンドウのスタイル設定
		WindowProc,						// ウィンドウプロシージャのアドレス(関数名)を指定
		0,								// 通常は指定しないので0指定
		0,								// 通常は指定しないので0指定
		hInstance,						// Windowsの引数のインスタンスハンドル指定
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーに使用するアイコン指定
		LoadCursor(NULL,IDC_ARROW),		// 使用するマウスカーソル指定
		(HBRUSH)(COLOR_WINDOW - 1),		// ウィンドウクライアント領域の色設定
		NULL,							// メニュー指定
		CLASS_NAME,						// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// 小さいアイコン指定
	};
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	// 指定したクライアント領域に幅を計算する
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウクラス生成
	hWnd = CreateWindowEx    // Exは拡張
	(
		0,							// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラス名
		WINDOW_NAME,				// ウィンドウ名
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウ左上x座標
		CW_USEDEFAULT,				// ウィンドウ左上y座標
		(rect.right - rect.left),		 // ウィンドウ幅
		(rect.bottom - rect.top),		 // ウィンドウ高さ
		NULL,				 // 親ウィンドウハンドル
		NULL,				 // メニューもしくは子ウィンドウID
		hInstance,			 // インスタンスハンドル
		NULL				 // ウィンドウ作成データ
	);

#if ERROW_ACTION
	// コンソール呼び出し
	AllocConsole();
	FILE *fp = NULL;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	std::cout << "";
#endif


	// マネージャのヌルチェック
	if (pManager == NULL)
	{
		// メモリの生成
		pManager = new CManager();	// マネージャの生成
	}

	// 初期化処理(ウィンドウの生成後に行う)
	if (FAILED(pManager->Init(hWnd, SCREEN_WINDOWS, hInstance)))
	{
		// 終了処理
		pManager->Uninit();
		// マネージャ
		delete pManager;
		pManager = NULL;
		return -1;	// 失敗したら抜ける
	}

	// ウィンドウ生成 //
	ShowWindow(hWnd, nCmdShow); // 指定されたウィンドウの表示設定
	UpdateWindow(hWnd);			// ウィンドウのクライアント領域更新


	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

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
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);	// ウィンドウプロシージャにメッセージで送る
			}
		}

		else
		{
			// DirectXの処理
			dwCurrentTime = timeGetTime();		// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPSを測定
#endif

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存
				// 更新処理
				pManager->Update();
				// 描画処理
				pManager->Draw();
				dwFrameCount++;					// カウントを加算
				// trueのこの画面を破棄する
				if (g_bDestWind == true)
				{
					DestroyWindow(hWnd);
				}
			}
		}
	}
	/* メモリを解放 */
	// マネージャのヌルチェック
	if (pManager != NULL)
	{

		// 終了処理
		pManager->Uninit();
		// マネージャ
		delete pManager;
		pManager = NULL;
	}
	//ウィンドウクラスの登録を解析 //
	UnregisterClass(CLASS_NAME, wcex.hInstance);

#if ERROW_ACTION
	// メモリリークレポート
	_CrtDumpMemoryLeaks();
#endif
	return (int)msg.wParam;
}

// ----------------------------------------------------------------------------------------------------
// 終了設定関数
// ----------------------------------------------------------------------------------------------------
void SetDestWind(bool const &bDest)
{
	g_bDestWind = bDest;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------
// ImGuiプロシージャー関数
// ----------------------------------------------------------------------------------------------------
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
// ----------------------------------------------------------------------------------------------------
// ウィンドウプロシージャ関数
// ----------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
#endif // _DEBUG

	switch (uMsg)
	{
	// 左クリック時 //
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;
	// キーが押されたときのメッセージ //
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			// ウィンドウを破棄(WM_DESTROY発動)
			DestroyWindow(hWnd);
		}
		break;
	// ×終了 //
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	//ウィンドウ破棄メッセージ //
	case WM_DESTROY:
		// WN_QUITメッセージを返す。ウィンドウを終わらせる
		PostQuitMessage(0);
		break;
	// その他 //
	default:
		break;
	}

	// 既定の処理を提供 //
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 終わらせる以外のmsg
}
#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------
// FPS取得関数
// ----------------------------------------------------------------------------------------------------
int GetFPS(void)
{
	return g_nCountFPS;
}

// ----------------------------------------------------------------------------------------------------
// コンソール表示
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
