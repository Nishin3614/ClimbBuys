// ----------------------------------------
//
// サウンド処理の説明[sound.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "sound.h"
#include "manager.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define SOUND_FILE "data/LOAD/SOUND/sound_manager.csv"

#ifdef _XBOX //Big-Endian
#define FOURCC_RIFF	'RIFF'
#define FOURCC_DATA	'data'
#define FOURCC_FMT	'fmt '
#define FOURCC_WAVE	'WAVE'
#define FOURCC_XWMA	'XWMA'
#define FOURCC_DPDS	'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define	_FOURCC_RIFF	'FFIR'
#define	_FOURCC_DATA	'atad'
#define	_FOURCC_FMT		' tmf'
#define _FOURCC_WAVE	'EVAW'
#define _FOURCC_XWMA	'AMWX'
#define _FOURCC_DPDS	'sdpd'
#endif

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
std::vector<std::string> CSound::m_vec_Filename = {};						// ファイル名
std::vector<int> CSound::m_vec_nLoop = {};							// ループカウント
std::vector<CSound::SOUND> CSound::m_vec_SoundType = {};				// サウンドタイプ
IXAudio2 * CSound::m_pXAudio2 = NULL;							// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice * CSound::m_pMasteringVoice = NULL;		// マスターボイス
BYTE * CSound::m_apDataAudio[LABEL_MAX] = {};					// オーディオデータ
DWORD CSound::m_aSizeAudio[LABEL_MAX] = {};						// オーディオデータサイズ
IXAudio2SourceVoice *CSound::m_SourceBGM = NULL;				// ソースBGM
IXAudio2SourceVoice *CSound::m_apSourceSE[MAX_SE] = {};			// ソースSE
IXAudio2SourceVoice *CSound::m_pSoureceVOICE = NULL;			// ソースVOICE

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CSound::CSound()
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CSound::~CSound()
{
}

// ----------------------------------------
// 初期化処理(ソースの読み込み)
// ----------------------------------------
HRESULT CSound::Init(HWND hWnd)
{
	// ミュートにしない
	m_bMute = false;

#ifdef _DEBUG
	// デバッグ時ミュートにする
	m_bMute = true;
#endif // _DEBUG

	// スクリプトの読み込み
	Load();
	// オーディオの生成
	AudioCreate(hWnd);
	// サウンドの生成
	SoundCreate(hWnd);
	return S_OK;
}

// ----------------------------------------
// 終了処理(ソースの開放)
// ----------------------------------------
void CSound::Uninit(void)
{
	// ソースBGMの開放
	if (m_SourceBGM != NULL)
	{
		// 一時停止
		m_SourceBGM->Stop(0);

		// ソースボイスの破棄
		m_SourceBGM->DestroyVoice();
		m_SourceBGM = NULL;
	}
	// ソースSEの開放
	for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
	{
		if (m_apSourceSE[nCntSE] != NULL)
		{
			// 一時停止
			m_apSourceSE[nCntSE]->Stop(0);

			// ソースボイスの破棄
			m_apSourceSE[nCntSE]->DestroyVoice();
			m_apSourceSE[nCntSE] = NULL;
		}
	}
	// ソースVOICEの開放
	if (m_pSoureceVOICE != NULL)
	{
		// 一時停止
		m_pSoureceVOICE->Stop(0);

		// ソースボイスの破棄
		m_pSoureceVOICE->DestroyVoice();
		m_pSoureceVOICE = NULL;
	}
	// オーディオの開放
	for (int nCntSound = 0; nCntSound < (signed)m_vec_Filename.size(); nCntSound++)
	{
		if (m_apDataAudio[nCntSound] != NULL)
		{
			// オーディオデータの開放
			delete[] m_apDataAudio[nCntSound];
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	// マスターボイスの破棄
	if (m_pMasteringVoice != NULL)
	{
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = NULL;
	}
	// XAudio2オブジェクトの開放
	if (m_pXAudio2 != NULL)
	{
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	// ファイル名の開放
	m_vec_Filename.clear();
	m_vec_Filename.shrink_to_fit();
	m_vec_nLoop.clear();
	m_vec_nLoop.shrink_to_fit();
	m_vec_SoundType.clear();
	m_vec_SoundType.shrink_to_fit();

	// COMライブラリの終了処理
	CoUninitialize();
}

// ----------------------------------------
// セグメント再生
// ----------------------------------------
HRESULT CSound::PlaySound(
	LABEL const &label		// ラベル
)
{
	// ミュートしていないとき
	if (!m_bMute)
	{
		// 変数宣言
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[label];
		buffer.pAudioData = m_apDataAudio[label];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_vec_nLoop[label];

		// BGMの場合
		if (m_vec_SoundType[label] == SOUND_BGM)
		{
			if (m_SourceBGM == NULL)
			{
				return E_FAIL;
			}
			// 状態取得
			m_SourceBGM->GetState(&xa2state);
			// 再生中
			if (xa2state.BuffersQueued != 0)
			{
				// 一時停止
				m_SourceBGM->Stop(0);
				// オーディオバッファの削除
				m_SourceBGM->FlushSourceBuffers();
			}
			// オーディオバッファの登録
			m_SourceBGM->SubmitSourceBuffer(&buffer);
			//m_SourceBGM->SetVolume(10);
			// 再生
			m_SourceBGM->Start(0);
		}
		// SEの場合
		else if (m_vec_SoundType[label] == SOUND_SE)
		{
			// 使われていないソースで再生する
			for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
			{
				// ヌルだったらループスキップ
				if (m_apSourceSE[nCntSE] == NULL)
				{
					continue;
				}
				// 状態取得
				m_apSourceSE[nCntSE]->GetState(&xa2state);
				// 再生中だったらループスキップ
				if (xa2state.BuffersQueued != 0)
				{
					continue;
				}
				// オーディオバッファの登録
				m_apSourceSE[nCntSE]->SubmitSourceBuffer(&buffer);
				// 再生
				m_apSourceSE[nCntSE]->Start(0);
				return S_OK;
			}
		}
		// VOICEの場合
		else if (m_vec_SoundType[label] == SOUND_VOICE)
		{
			// ヌルだったらループスキップ
			if (m_pSoureceVOICE == NULL)
			{
				return E_FAIL;
			}
			//m_pSoureceVOICE->SetVolume()
			// 状態取得
			m_pSoureceVOICE->GetState(&xa2state);
			// 再生中だったら停止する
			if (xa2state.BuffersQueued != 0)
			{
				// 一時停止
				m_pSoureceVOICE->Stop(0);
				// オーディオバッファの削除
				m_pSoureceVOICE->FlushSourceBuffers();
			}
			// オーディオバッファの登録
			m_pSoureceVOICE->SubmitSourceBuffer(&buffer);
			// 再生
			m_pSoureceVOICE->Start(0);
			return S_OK;
		}
	}
	return S_OK;
}

// ----------------------------------------
// セグメント停止(ラベル指定)
// ----------------------------------------
void CSound::StopSound(LABEL label)
{
	// BGMの場合
	if (m_vec_nLoop[label] == -1)
	{
		// 変数宣言
		XAUDIO2_VOICE_STATE xa2state;
		// 状態取得
		m_SourceBGM->GetState(&xa2state);
		// 再生中
		if (xa2state.BuffersQueued != 0)
		{
			// 一時停止
			m_SourceBGM->Stop(0);
			// オーディオバッファの削除
			m_SourceBGM->FlushSourceBuffers();
		}
	}
}

// ----------------------------------------
// セグメント強制停止(ラベル指定)
// ----------------------------------------
void CSound::StopCompusionSound(LABEL label)
{
	// 変数宣言
	XAUDIO2_VOICE_STATE xa2state;
	// 状態取得
	m_pSoureceVOICE->GetState(&xa2state);
	// 再生中
	if (xa2state.BuffersQueued != 0)
	{
		// 一時停止
		m_pSoureceVOICE->Stop(0);
		// オーディオバッファの削除
		m_pSoureceVOICE->FlushSourceBuffers();
	}
}

// ----------------------------------------
// セグメント停止(全て)
// ----------------------------------------
void CSound::StopSound(void)
{
	// 変数宣言
	XAUDIO2_VOICE_STATE xa2state;
	// BGMを停止する
	if (m_SourceBGM != NULL)
	{
		// 状態取得
		m_SourceBGM->GetState(&xa2state);
		// 再生中
		if (xa2state.BuffersQueued != 0)
		{
			// 一時停止
			m_SourceBGM->Stop(0);
			// オーディオバッファの削除
			m_SourceBGM->FlushSourceBuffers();
		}
	}

	// SEを停止する
	for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
	{
		if (m_apSourceSE[nCntSE] != NULL)
		{
			// 状態取得
			m_apSourceSE[nCntSE]->GetState(&xa2state);
			// 再生中
			if (xa2state.BuffersQueued != 0)
			{
				// 一時停止
				m_apSourceSE[nCntSE]->Stop(0);
				// オーディオバッファの削除
				m_apSourceSE[nCntSE]->FlushSourceBuffers();
			}
		}
	}
	// VOICEを停止する
	if (m_pSoureceVOICE != NULL)
	{
		// 状態取得
		m_pSoureceVOICE->GetState(&xa2state);
		// 再生中
		if (xa2state.BuffersQueued != 0)
		{
			// 一時停止
			m_pSoureceVOICE->Stop(0);
			// オーディオバッファの削除
			m_pSoureceVOICE->FlushSourceBuffers();
		}
	}
}

// ----------------------------------------
// サウンドの生成
// ----------------------------------------
HRESULT CSound::AudioCreate(HWND hWnd)
{
	// 変数宣言
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	// XAudio2オブジェクトの生成に失敗したら抜ける
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("XAudio2オブジェクトの生成に失敗！", hWnd);
#endif // _DEBUG
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	// マスターボイスの生成に失敗したら抜ける
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("マスターボイスの生成に失敗！", hWnd);
#endif // _DEBUG
		// XAudio2オブジェクトの開放
		if (m_pXAudio2)
		{
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}
	return S_OK;
}

// ----------------------------------------
// サウンドの生成
// ----------------------------------------
HRESULT CSound::SoundCreate(HWND hWnd)
{
	// 変数宣言
	HRESULT hr;
	HANDLE hFile = NULL;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	DWORD dwFiletype = 0;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	int nCntSound;

	// サウンドデータの初期化
	for (nCntSound = 0; nCntSound < (signed)m_vec_Filename.size(); nCntSound++)
	{
		// 初期化
		hFile = NULL;
		dwChunkSize = 0;
		dwChunkPosition = 0;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_vec_Filename.at(nCntSound).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
#ifdef _DEBUG
			CCalculation::Messanger("サウンドデータファイルの生成に失敗！(1)", hWnd);
#endif // _DEBUG
			return HRESULT_FROM_WIN32(GetLastError());
		}
		// ファイルポインタを先頭に移動
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
#ifdef _DEBUG
			CCalculation::Messanger("サウンドデータファイルの生成に失敗！(2)", hWnd);
#endif // _DEBUG
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// ----------WAVEファイルのチェック---------- //
		hr = CheckChunk(hFile, _FOURCC_RIFF, &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("WAVEファイルのチェックに失敗！(1)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("WAVEファイルのチェックに失敗！(2)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		if (dwFiletype != _FOURCC_WAVE)
		{
#ifdef _DEBUG
			CCalculation::Messanger("WAVEファイルのチェックに失敗！(3)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		// ----------フォーマットチェック---------- //
		hr = CheckChunk(hFile, _FOURCC_FMT, &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("フォーマットチェックに失敗！(1)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("フォーマットチェックに失敗！(2)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}

		// ----------オーディオデータ読み込み---------- //
		hr = CheckChunk(hFile, _FOURCC_DATA, &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("オーディオデータ読み込みに失敗！(1)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = new BYTE[m_aSizeAudio[nCntSound]];
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("オーディオデータ読み込みに失敗！(2)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		// ファイルをクローズ
		CloseHandle(hFile);
	}

	// ----------ソースボイスの生成---------- //
	// すべて同じフォーマットなら //
	// BGM
	hr = m_pXAudio2->CreateSourceVoice(&m_SourceBGM, &(wfx.Format));
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("ソースBGMの生成に失敗！", hWnd);
#endif // _DEBUG
		return S_FALSE;
	}
	// SE
	for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
	{
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceSE[nCntSE], &(wfx.Format));
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("ソースSEの生成に失敗！", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		// 音の音量を上げる
		m_apSourceSE[nCntSE]->SetVolume(0.1f);
	}
	hr = m_pXAudio2->CreateSourceVoice(&m_pSoureceVOICE, &(wfx.Format));
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("ソースVOICEの生成に失敗！", hWnd);
#endif // _DEBUG
		return S_FALSE;
	}
	// 音の音量を上げる
	m_SourceBGM->SetVolume(0.1f);
	return S_OK;
}

// ----------------------------------------
// チャンクのチェック
// ----------------------------------------
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD * pChunkSize, DWORD * pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	// ファイルポインタを先頭に移動
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		// チャンクの読み込み
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		// チャンクデータの読み込み
		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case _FOURCC_RIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			// ファイルタイプの読み込み
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			// ファイルポインタをチャンクデータ分移動
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
			break;
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

// ----------------------------------------
// チャンクデータの読み込み
// ----------------------------------------
HRESULT CSound::ReadChunkData(HANDLE hFile, void * pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	// ファイルポインタを指定位置まで移動
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// データの読み込み
	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

// ----------------------------------------
// スクリプト読み込み
// ----------------------------------------
HRESULT CSound::Load(void)
{
	// 変数宣言
	// ファイルの中身格納用
	std::vector<std::vector<std::string>> vsvec_Contens;
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(SOUND_FILE, ',');
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
				// ファイル名追加
				m_vec_Filename.push_back(vsvec_Contens.at(nCntLine).at(nCntItem).c_str());
				break;
				// ループ情報
			case 1:
				// ファイル名追加
				m_vec_nLoop.push_back(std::stoi(vsvec_Contens.at(nCntLine).at(nCntItem).c_str()));
				break;
				// サウンドタイプ情報
			case 2:
				// ファイル名追加
				m_vec_SoundType.push_back((SOUND)std::stoi(vsvec_Contens.at(nCntLine).at(nCntItem).c_str()));
				break;
			default:
				break;
			}
		}
	}
	return S_OK;
}
