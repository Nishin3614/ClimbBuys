// ----------------------------------------
//
// �T�E���h�����̐���[sound.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define MAX_SE (10)

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CSound
{
public:
	/* �񋓌^ */
	// ���^�C�v
	typedef enum
	{
		SOUND_BGM= 0,	// BGM
		SOUND_SE,		// SE
		SOUND_VOICE,	// VOICE
		SOUND_MAX,
	} SOUND;
	// ���x��
	typedef enum
	{
		LABEL_BGM_GAME = 0,				// �Q�[��
		LABEL_BGM_TUTORIAL,				// �`���[�g���A��
		LABEL_SE_GAMESTART,				// �Q�[���X�^�[�g
		LABEL_SE_COUNTDOWN,				// �J�E���g�_�E��
		LABEL_SE_FINISH,				// �t�B�j�b�V��
		LABEL_SE_DETERMINATION,			// ���艹0
		LABEL_SE_SELECT,				// �I����0
		LABEL_SE_DIE0,					// �v���C���[���S0
		LABEL_SE_DIE1,					// �v���C���[���S1
		LABEL_SE_SPRING,				// �o�l
		LABEL_SE_PUNCH,					// ���艹
		LABEL_SE_RESULT,				// ���U���g
		LABEL_MAX,						// �ő吔
	} LABEL;
	/* �֐� */
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(
		LABEL const &label		// ���x��
	);
	void StopSound(LABEL label);
	void StopCompusionSound(LABEL label);
	void StopSound(void);
	// �X�N���v�g����ǂݍ���
	static HRESULT Load(void);
protected:

private:
	/* �֐� */
	HRESULT AudioCreate(HWND hWnd);
	HRESULT SoundCreate(HWND hWnd);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	/* �ϐ� */
	static std::vector<std::string>		m_vec_Filename;					// �t�@�C����
	static std::vector<int>				m_vec_nLoop;					// ���[�v�J�E���g
	static std::vector<SOUND>			m_vec_SoundType;				// �T�E���h�^�C�v
	static IXAudio2						*m_pXAudio2;					// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice		*m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static BYTE							*m_apDataAudio[LABEL_MAX];		// �I�[�f�B�I�f�[�^
	static DWORD						m_aSizeAudio[LABEL_MAX];		// �I�[�f�B�I�f�[�^�T�C�Y
	static IXAudio2SourceVoice			*m_SourceBGM;					// �\�[�XBGM
	static IXAudio2SourceVoice			*m_apSourceSE[MAX_SE];			// �\�[�XSE
	static IXAudio2SourceVoice			*m_pSoureceVOICE;				// �\�[�XVOICE
	bool								m_bMute;							// �~���[�g�t���O
};

#endif