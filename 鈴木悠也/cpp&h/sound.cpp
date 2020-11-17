// ----------------------------------------
//
// �T�E���h�����̐���[sound.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "sound.h"
#include "manager.h"

// ----------------------------------------
//
// �}�N����`
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
// �O���[�o���ϐ�
//
// ----------------------------------------

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------
std::vector<std::string> CSound::m_vec_Filename = {};						// �t�@�C����
std::vector<int> CSound::m_vec_nLoop = {};							// ���[�v�J�E���g
std::vector<CSound::SOUND> CSound::m_vec_SoundType = {};				// �T�E���h�^�C�v
IXAudio2 * CSound::m_pXAudio2 = NULL;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice * CSound::m_pMasteringVoice = NULL;		// �}�X�^�[�{�C�X
BYTE * CSound::m_apDataAudio[LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
DWORD CSound::m_aSizeAudio[LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y
IXAudio2SourceVoice *CSound::m_SourceBGM = NULL;				// �\�[�XBGM
IXAudio2SourceVoice *CSound::m_apSourceSE[MAX_SE] = {};			// �\�[�XSE
IXAudio2SourceVoice *CSound::m_pSoureceVOICE = NULL;			// �\�[�XVOICE

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CSound::CSound()
{
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CSound::~CSound()
{
}

// ----------------------------------------
// ����������(�\�[�X�̓ǂݍ���)
// ----------------------------------------
HRESULT CSound::Init(HWND hWnd)
{
	// �~���[�g�ɂ��Ȃ�
	m_bMute = false;

#ifdef _DEBUG
	// �f�o�b�O���~���[�g�ɂ���
	m_bMute = true;
#endif // _DEBUG

	// �X�N���v�g�̓ǂݍ���
	Load();
	// �I�[�f�B�I�̐���
	AudioCreate(hWnd);
	// �T�E���h�̐���
	SoundCreate(hWnd);
	return S_OK;
}

// ----------------------------------------
// �I������(�\�[�X�̊J��)
// ----------------------------------------
void CSound::Uninit(void)
{
	// �\�[�XBGM�̊J��
	if (m_SourceBGM != NULL)
	{
		// �ꎞ��~
		m_SourceBGM->Stop(0);

		// �\�[�X�{�C�X�̔j��
		m_SourceBGM->DestroyVoice();
		m_SourceBGM = NULL;
	}
	// �\�[�XSE�̊J��
	for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
	{
		if (m_apSourceSE[nCntSE] != NULL)
		{
			// �ꎞ��~
			m_apSourceSE[nCntSE]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			m_apSourceSE[nCntSE]->DestroyVoice();
			m_apSourceSE[nCntSE] = NULL;
		}
	}
	// �\�[�XVOICE�̊J��
	if (m_pSoureceVOICE != NULL)
	{
		// �ꎞ��~
		m_pSoureceVOICE->Stop(0);

		// �\�[�X�{�C�X�̔j��
		m_pSoureceVOICE->DestroyVoice();
		m_pSoureceVOICE = NULL;
	}
	// �I�[�f�B�I�̊J��
	for (int nCntSound = 0; nCntSound < (signed)m_vec_Filename.size(); nCntSound++)
	{
		if (m_apDataAudio[nCntSound] != NULL)
		{
			// �I�[�f�B�I�f�[�^�̊J��
			delete[] m_apDataAudio[nCntSound];
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	// �}�X�^�[�{�C�X�̔j��
	if (m_pMasteringVoice != NULL)
	{
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = NULL;
	}
	// XAudio2�I�u�W�F�N�g�̊J��
	if (m_pXAudio2 != NULL)
	{
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	// �t�@�C�����̊J��
	m_vec_Filename.clear();
	m_vec_Filename.shrink_to_fit();
	m_vec_nLoop.clear();
	m_vec_nLoop.shrink_to_fit();
	m_vec_SoundType.clear();
	m_vec_SoundType.shrink_to_fit();

	// COM���C�u�����̏I������
	CoUninitialize();
}

// ----------------------------------------
// �Z�O�����g�Đ�
// ----------------------------------------
HRESULT CSound::PlaySound(
	LABEL const &label		// ���x��
)
{
	// �~���[�g���Ă��Ȃ��Ƃ�
	if (!m_bMute)
	{
		// �ϐ��錾
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[label];
		buffer.pAudioData = m_apDataAudio[label];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_vec_nLoop[label];

		// BGM�̏ꍇ
		if (m_vec_SoundType[label] == SOUND_BGM)
		{
			if (m_SourceBGM == NULL)
			{
				return E_FAIL;
			}
			// ��Ԏ擾
			m_SourceBGM->GetState(&xa2state);
			// �Đ���
			if (xa2state.BuffersQueued != 0)
			{
				// �ꎞ��~
				m_SourceBGM->Stop(0);
				// �I�[�f�B�I�o�b�t�@�̍폜
				m_SourceBGM->FlushSourceBuffers();
			}
			// �I�[�f�B�I�o�b�t�@�̓o�^
			m_SourceBGM->SubmitSourceBuffer(&buffer);
			//m_SourceBGM->SetVolume(10);
			// �Đ�
			m_SourceBGM->Start(0);
		}
		// SE�̏ꍇ
		else if (m_vec_SoundType[label] == SOUND_SE)
		{
			// �g���Ă��Ȃ��\�[�X�ōĐ�����
			for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
			{
				// �k���������烋�[�v�X�L�b�v
				if (m_apSourceSE[nCntSE] == NULL)
				{
					continue;
				}
				// ��Ԏ擾
				m_apSourceSE[nCntSE]->GetState(&xa2state);
				// �Đ����������烋�[�v�X�L�b�v
				if (xa2state.BuffersQueued != 0)
				{
					continue;
				}
				// �I�[�f�B�I�o�b�t�@�̓o�^
				m_apSourceSE[nCntSE]->SubmitSourceBuffer(&buffer);
				// �Đ�
				m_apSourceSE[nCntSE]->Start(0);
				return S_OK;
			}
		}
		// VOICE�̏ꍇ
		else if (m_vec_SoundType[label] == SOUND_VOICE)
		{
			// �k���������烋�[�v�X�L�b�v
			if (m_pSoureceVOICE == NULL)
			{
				return E_FAIL;
			}
			//m_pSoureceVOICE->SetVolume()
			// ��Ԏ擾
			m_pSoureceVOICE->GetState(&xa2state);
			// �Đ������������~����
			if (xa2state.BuffersQueued != 0)
			{
				// �ꎞ��~
				m_pSoureceVOICE->Stop(0);
				// �I�[�f�B�I�o�b�t�@�̍폜
				m_pSoureceVOICE->FlushSourceBuffers();
			}
			// �I�[�f�B�I�o�b�t�@�̓o�^
			m_pSoureceVOICE->SubmitSourceBuffer(&buffer);
			// �Đ�
			m_pSoureceVOICE->Start(0);
			return S_OK;
		}
	}
	return S_OK;
}

// ----------------------------------------
// �Z�O�����g��~(���x���w��)
// ----------------------------------------
void CSound::StopSound(LABEL label)
{
	// BGM�̏ꍇ
	if (m_vec_nLoop[label] == -1)
	{
		// �ϐ��錾
		XAUDIO2_VOICE_STATE xa2state;
		// ��Ԏ擾
		m_SourceBGM->GetState(&xa2state);
		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			m_SourceBGM->Stop(0);
			// �I�[�f�B�I�o�b�t�@�̍폜
			m_SourceBGM->FlushSourceBuffers();
		}
	}
}

// ----------------------------------------
// �Z�O�����g������~(���x���w��)
// ----------------------------------------
void CSound::StopCompusionSound(LABEL label)
{
	// �ϐ��錾
	XAUDIO2_VOICE_STATE xa2state;
	// ��Ԏ擾
	m_pSoureceVOICE->GetState(&xa2state);
	// �Đ���
	if (xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		m_pSoureceVOICE->Stop(0);
		// �I�[�f�B�I�o�b�t�@�̍폜
		m_pSoureceVOICE->FlushSourceBuffers();
	}
}

// ----------------------------------------
// �Z�O�����g��~(�S��)
// ----------------------------------------
void CSound::StopSound(void)
{
	// �ϐ��錾
	XAUDIO2_VOICE_STATE xa2state;
	// BGM���~����
	if (m_SourceBGM != NULL)
	{
		// ��Ԏ擾
		m_SourceBGM->GetState(&xa2state);
		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			m_SourceBGM->Stop(0);
			// �I�[�f�B�I�o�b�t�@�̍폜
			m_SourceBGM->FlushSourceBuffers();
		}
	}

	// SE���~����
	for (int nCntSE = 0; nCntSE < MAX_SE; nCntSE++)
	{
		if (m_apSourceSE[nCntSE] != NULL)
		{
			// ��Ԏ擾
			m_apSourceSE[nCntSE]->GetState(&xa2state);
			// �Đ���
			if (xa2state.BuffersQueued != 0)
			{
				// �ꎞ��~
				m_apSourceSE[nCntSE]->Stop(0);
				// �I�[�f�B�I�o�b�t�@�̍폜
				m_apSourceSE[nCntSE]->FlushSourceBuffers();
			}
		}
	}
	// VOICE���~����
	if (m_pSoureceVOICE != NULL)
	{
		// ��Ԏ擾
		m_pSoureceVOICE->GetState(&xa2state);
		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			m_pSoureceVOICE->Stop(0);
			// �I�[�f�B�I�o�b�t�@�̍폜
			m_pSoureceVOICE->FlushSourceBuffers();
		}
	}
}

// ----------------------------------------
// �T�E���h�̐���
// ----------------------------------------
HRESULT CSound::AudioCreate(HWND hWnd)
{
	// �ϐ��錾
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	// XAudio2�I�u�W�F�N�g�̐����Ɏ��s�����甲����
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("XAudio2�I�u�W�F�N�g�̐����Ɏ��s�I", hWnd);
#endif // _DEBUG
		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	// �}�X�^�[�{�C�X�̐����Ɏ��s�����甲����
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("�}�X�^�[�{�C�X�̐����Ɏ��s�I", hWnd);
#endif // _DEBUG
		// XAudio2�I�u�W�F�N�g�̊J��
		if (m_pXAudio2)
		{
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}
		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}
	return S_OK;
}

// ----------------------------------------
// �T�E���h�̐���
// ----------------------------------------
HRESULT CSound::SoundCreate(HWND hWnd)
{
	// �ϐ��錾
	HRESULT hr;
	HANDLE hFile = NULL;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	DWORD dwFiletype = 0;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	int nCntSound;

	// �T�E���h�f�[�^�̏�����
	for (nCntSound = 0; nCntSound < (signed)m_vec_Filename.size(); nCntSound++)
	{
		// ������
		hFile = NULL;
		dwChunkSize = 0;
		dwChunkPosition = 0;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_vec_Filename.at(nCntSound).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
#ifdef _DEBUG
			CCalculation::Messanger("�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", hWnd);
#endif // _DEBUG
			return HRESULT_FROM_WIN32(GetLastError());
		}
		// �t�@�C���|�C���^��擪�Ɉړ�
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
#ifdef _DEBUG
			CCalculation::Messanger("�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", hWnd);
#endif // _DEBUG
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// ----------WAVE�t�@�C���̃`�F�b�N---------- //
		hr = CheckChunk(hFile, _FOURCC_RIFF, &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		if (dwFiletype != _FOURCC_WAVE)
		{
#ifdef _DEBUG
			CCalculation::Messanger("WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		// ----------�t�H�[�}�b�g�`�F�b�N---------- //
		hr = CheckChunk(hFile, _FOURCC_FMT, &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}

		// ----------�I�[�f�B�I�f�[�^�ǂݍ���---------- //
		hr = CheckChunk(hFile, _FOURCC_DATA, &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = new BYTE[m_aSizeAudio[nCntSound]];
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			CCalculation::Messanger("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	// ----------�\�[�X�{�C�X�̐���---------- //
	// ���ׂē����t�H�[�}�b�g�Ȃ� //
	// BGM
	hr = m_pXAudio2->CreateSourceVoice(&m_SourceBGM, &(wfx.Format));
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("�\�[�XBGM�̐����Ɏ��s�I", hWnd);
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
			CCalculation::Messanger("�\�[�XSE�̐����Ɏ��s�I", hWnd);
#endif // _DEBUG
			return S_FALSE;
		}
		// ���̉��ʂ��グ��
		m_apSourceSE[nCntSE]->SetVolume(0.1f);
	}
	hr = m_pXAudio2->CreateSourceVoice(&m_pSoureceVOICE, &(wfx.Format));
	if (FAILED(hr))
	{
#ifdef _DEBUG
		CCalculation::Messanger("�\�[�XVOICE�̐����Ɏ��s�I", hWnd);
#endif // _DEBUG
		return S_FALSE;
	}
	// ���̉��ʂ��グ��
	m_SourceBGM->SetVolume(0.1f);
	return S_OK;
}

// ----------------------------------------
// �`�����N�̃`�F�b�N
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

	// �t�@�C���|�C���^��擪�Ɉړ�
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		// �`�����N�̓ǂݍ���
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		// �`�����N�f�[�^�̓ǂݍ���
		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case _FOURCC_RIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			// �t�@�C���^�C�v�̓ǂݍ���
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
// ----------------------------------------
HRESULT CSound::ReadChunkData(HANDLE hFile, void * pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	// �t�@�C���|�C���^���w��ʒu�܂ňړ�
	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// �f�[�^�̓ǂݍ���
	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

// ----------------------------------------
// �X�N���v�g�ǂݍ���
// ----------------------------------------
HRESULT CSound::Load(void)
{
	// �ϐ��錾
	// �t�@�C���̒��g�i�[�p
	std::vector<std::vector<std::string>> vsvec_Contens;
	// �t�@�C���̒��g���擾����
	vsvec_Contens = CCalculation::FileContens(SOUND_FILE, ',');
	// �s���Ƃɉ�
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// ���ڂ��Ƃɉ�
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			switch (nCntItem)
			{
				// �p�X���
			case 0:
				// �t�@�C�����ǉ�
				m_vec_Filename.push_back(vsvec_Contens.at(nCntLine).at(nCntItem).c_str());
				break;
				// ���[�v���
			case 1:
				// �t�@�C�����ǉ�
				m_vec_nLoop.push_back(std::stoi(vsvec_Contens.at(nCntLine).at(nCntItem).c_str()));
				break;
				// �T�E���h�^�C�v���
			case 2:
				// �t�@�C�����ǉ�
				m_vec_SoundType.push_back((SOUND)std::stoi(vsvec_Contens.at(nCntLine).at(nCntItem).c_str()));
				break;
			default:
				break;
			}
		}
	}
	return S_OK;
}
