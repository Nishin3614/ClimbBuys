// ----------------------------------------
//
// サウンド処理の説明[sound.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_SE (10)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CSound
{
public:
	/* 列挙型 */
	// 音タイプ
	typedef enum
	{
		SOUND_BGM= 0,	// BGM
		SOUND_SE,		// SE
		SOUND_VOICE,	// VOICE
		SOUND_MAX,
	} SOUND;
	// ラベル
	typedef enum
	{
		LABEL_BGM_GAME = 0,				// ゲーム
		LABEL_BGM_TUTORIAL,				// チュートリアル
		LABEL_SE_GAMESTART,				// ゲームスタート
		LABEL_SE_COUNTDOWN,				// カウントダウン
		LABEL_SE_FINISH,				// フィニッシュ
		LABEL_SE_DETERMINATION,			// 決定音0
		LABEL_SE_SELECT,				// 選択音0
		LABEL_SE_DIE0,					// プレイヤー死亡0
		LABEL_SE_DIE1,					// プレイヤー死亡1
		LABEL_SE_SPRING,				// バネ
		LABEL_SE_PUNCH,					// 殴り音
		LABEL_SE_RESULT,				// リザルト
		LABEL_MAX,						// 最大数
	} LABEL;
	/* 関数 */
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(
		LABEL const &label		// ラベル
	);
	void StopSound(LABEL label);
	void StopCompusionSound(LABEL label);
	void StopSound(void);
	// スクリプト情報を読み込む
	static HRESULT Load(void);
protected:

private:
	/* 関数 */
	HRESULT AudioCreate(HWND hWnd);
	HRESULT SoundCreate(HWND hWnd);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	/* 変数 */
	static std::vector<std::string>		m_vec_Filename;					// ファイル名
	static std::vector<int>				m_vec_nLoop;					// ループカウント
	static std::vector<SOUND>			m_vec_SoundType;				// サウンドタイプ
	static IXAudio2						*m_pXAudio2;					// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice		*m_pMasteringVoice;				// マスターボイス
	static BYTE							*m_apDataAudio[LABEL_MAX];		// オーディオデータ
	static DWORD						m_aSizeAudio[LABEL_MAX];		// オーディオデータサイズ
	static IXAudio2SourceVoice			*m_SourceBGM;					// ソースBGM
	static IXAudio2SourceVoice			*m_apSourceSE[MAX_SE];			// ソースSE
	static IXAudio2SourceVoice			*m_pSoureceVOICE;				// ソースVOICE
	bool								m_bMute;							// ミュートフラグ
};

#endif