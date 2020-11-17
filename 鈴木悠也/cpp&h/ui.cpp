// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���[�U�[�C���^�[�t�F�[�X���� [ui.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "ui.h"
/* �`�� */
#include "fade.h"
#include "2Dpresents.h"
#include "game.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TEXTURE_XSPLIT(nX) (1.0f / nX)
#define TEXTURE_YSPLIT(nY) (1.0f / nY)
#define UI_MANAGER_FILE "data/LOAD/UI/ui_manager.txt"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<CUi::UI_LOAD> CUi::m_vec_UiLoad[CUi::UITYPE_MAX] = {};	// Ui�ǂݍ��ݗp�ϐ�
bool	CUi::m_sta_UiUse[CUi::UITYPE_MAX] = {};					// ����UI�̎g�p���
std::vector<std::string> CUi::m_vec_String = {};					// �t�@�C�����ǂݏ����p

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::CUi()
{
	m_pScene_two = NULL;		// �V�[��2D
	m_pPresents = NULL;			// 2D�v���[���c
	m_Uitype = UITYPE_TILTE;	// UI�^�C�v
	m_nUi = 0;					// UI
	//m_nCount = 0;				// �J�E���g
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::~CUi()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Init(void)
{
	// �v���[���̏�񂪃k���ł͂Ȃ��ꍇ
	if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents != NULL)
	{
		// �v���[�����쐬
		m_pPresents = C2DPresents::Create_Self(
			(CScene_TWO::OFFSET_TYPE)m_vec_UiLoad[m_Uitype][m_nUi].nOffsetType,
			m_vec_UiLoad[m_Uitype][m_nUi].pos,
			m_vec_UiLoad[m_Uitype][m_nUi].size,
			m_vec_UiLoad[m_Uitype][m_nUi].nTexType,
			m_vec_UiLoad[m_Uitype][m_nUi].fRot,
			m_vec_UiLoad[m_Uitype][m_nUi].col
		);
		// �e�N�X�`���[���W�ݒ�
		m_pPresents->Set_Vtx_Tex(m_vec_UiLoad[m_Uitype][m_nUi].tex_first, m_vec_UiLoad[m_Uitype][m_nUi].tex_last);
		// ������񂪃k���ł͂Ȃ��ꍇ
		if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation != NULL)
		{
			// �F��񂪃k���ł͂Ȃ��ꍇ
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol != NULL)
			{
				// �F�̐ݒ�
				m_pPresents->SetCooperation(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol->changecol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol->nTimeSwitch,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol->nTimeChange
				);
			}
			// �g�嗦��񂪃k���ł͂Ȃ��ꍇ
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pScal != NULL)
			{
				// �g�嗦�̐ݒ�
				m_pPresents->SetCooperation(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pScal->fChangeScal,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pScal->nTimeChange
				);
			}
		}
		// �t�F�[�h�C����񂪃k���ł͂Ȃ��ꍇ
		if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn != NULL)
		{
			// �t�F�[�h�C��(�ʒu�w��)��񂪃k���ł͂Ȃ��ꍇ
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos != NULL)
			{
				// �t�F�[�h�C��(�ʒu�w��)�̐ݒ�
				m_pPresents->SetFadeIn(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos->BeginPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos->EndPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos->nMaxfram
				);
			}
			// �t�F�[�h�C��(�F�w��)��񂪃k���ł͂Ȃ��ꍇ
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol != NULL)
			{
				// �t�F�[�h�C��(�F�w��)�̐ݒ�
				m_pPresents->SetFadeIn(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol->BeginCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol->EndCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol->nMaxfram
				);
			}
			// �t�F�[�h�C�������I�����ݒ�
			m_pPresents->SetFadeIn_Compulsion(m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->bSkip);
			// �t�F�[�h�C��(�X�^�[�g���Ԏw��)�̐ݒ�
			m_pPresents->SetFadeIn_StarAnim(
				m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->FadeStart
			);
			// �t�F�[�h�C���J�n
			m_pPresents->Start_FadeIn();
		}
		// �t�F�[�h�A�E�g��񂪃k���ł͂Ȃ��ꍇ
		if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut != NULL)
		{
			// �t�F�[�h�A�E�g(�ʒu�w��)��񂪃k���ł͂Ȃ��ꍇ
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos != NULL)
			{
				// �t�F�[�h�A�E�g(�ʒu�w��)�̐ݒ�
				m_pPresents->SetFadeOut(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos->BeginPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos->EndPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos->nMaxfram
				);
			}
			// �t�F�[�h�A�E�g(�F�w��)��񂪃k���ł͂Ȃ��ꍇ
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol != NULL)
			{
				// �t�F�[�h�A�E�g(�F�w��)�̐ݒ�
				m_pPresents->SetFadeOut(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol->BeginCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol->EndCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol->nMaxfram
				);
			}
			// �t�F�[�h�A�E�g�����I�����ݒ�
			m_pPresents->SetFadeOut_Compulsion(m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->bSkip);
			// �t�F�[�h�A�E�g(�X�^�[�g���Ԏw��)�̐ݒ�
			m_pPresents->SetFadeOut_StarAnim(
				m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->FadeStart
			);
			// �t�F�[�h��Ԃ�FADETYPE_END�Ȃ�
			// ->FADETYPE_COOPERATION�ɂ���
			if (m_pPresents->GetFadetype() == C2DPresents::FADETYPE_END)
			{
				m_pPresents->SetFadetype(C2DPresents::FADETYPE_COOPERATION);
			}
		}
	}
	else
	{
		// �V�[��2D���쐬
		m_pScene_two = CScene_TWO::Create_Self(
			(CScene_TWO::OFFSET_TYPE)m_vec_UiLoad[m_Uitype][m_nUi].nOffsetType,
			m_vec_UiLoad[m_Uitype][m_nUi].pos,
			m_vec_UiLoad[m_Uitype][m_nUi].size,
			m_vec_UiLoad[m_Uitype][m_nUi].nTexType,
			m_vec_UiLoad[m_Uitype][m_nUi].fRot,
			m_vec_UiLoad[m_Uitype][m_nUi].col
		);
		// �e�N�X�`���[���W�ݒ�
		m_pScene_two->Set_Vtx_Tex(m_vec_UiLoad[m_Uitype][m_nUi].tex_first, m_vec_UiLoad[m_Uitype][m_nUi].tex_last);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Uninit(void)
{
	// �v���[����񂪃k���ł͂Ȃ��ꍇ
	if (m_pPresents != NULL)
	{
		// �I������
		m_pPresents->Uninit();
		// �������J��
		delete m_pPresents;
		m_pPresents = NULL;
	}
	// �V�[��2D���̏ꍇ
	else if(m_pScene_two != NULL)
	{
		// �I������
		m_pScene_two->Uninit();
		// �������J��
		delete m_pScene_two;
		m_pScene_two = NULL;
	}
	// �g�p��Ԃ�true�ł����false��
	if (m_sta_UiUse[m_Uitype])
	{
		m_sta_UiUse[m_Uitype] = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Update(void)
{
	// �v���[����񂪃k���ł͂Ȃ��ꍇ
	if (m_pPresents != NULL)
	{
		// �X�V����
		m_pPresents->Update();
	}
	// �V�[��2D���̏ꍇ
	else
	{
		// �X�V����
		m_pScene_two->Update();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Draw(void)
{
	// �v���[����񂪃k���ł͂Ȃ��ꍇ
	if (m_pPresents != NULL)
	{
		// �`�揈��
		m_pPresents->Draw();
	}
	// �V�[��2D���̏ꍇ
	else
	{
		// �`�揈��
		m_pScene_two->Draw();
	}
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Debug(void)
{
	// �v���[����񂪃k���ł͂Ȃ��ꍇ
	if (m_pPresents != NULL)
	{
		// �f�o�b�O����
		m_pPresents->Debug();
	}
	// �V�[��2D���̏ꍇ
	else
	{
		// �f�o�b�O����
		m_pScene_two->Debug();
	}
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �V�[��2D�̎擾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO * CUi::GetScene_Two(void)
{
	// �V�[��2D��NULL�ł͂Ȃ��̂Ȃ�
	// ->����Ԃ�
	if (m_pScene_two != NULL) return m_pScene_two;
	// �v���[���c2D��NULL�ł͂Ȃ��̂Ȃ�
	// ->����Ԃ�
	else if (m_pPresents != NULL) return m_pPresents;
	// NULL��Ԃ�
	else return nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���[���c2D�̎擾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents * CUi::GetPresents(void)
{
	// �v���[���c2D��NULL�ł͂Ȃ��̂Ȃ�
	// ->����Ԃ�
	if (m_pPresents != NULL) return m_pPresents;
	// NULL��Ԃ�
	else return nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �e�L�X�g�̓ǂݍ��ݏ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CUi::Load(void)
{
	// �ϐ��錾
	HRESULT hr;
	// UI�}�l�[�W���[�̓ǂݍ���
	hr = UiManagerLoad();
	// UI�X�N���v�g�̓ǂݍ���
	hr = UiScriptLoad();
	// ���ʂ�Ԃ�
	return hr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// UI�}�l�[�W���[�̓ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CUi::UiManagerLoad(void)
{
	// �ϐ��錾
	// �t�@�C���̒��g�i�[�p
	std::vector<std::vector<std::string>> vsvec_Contens;
	// �t�@�C���̒��g���擾����
	vsvec_Contens = CCalculation::FileContens(UI_MANAGER_FILE, '\0');
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
				m_vec_String.emplace_back(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
			default:
				break;
			}
		}
	}
	// �t�@�C���̒��g�̊J��
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		vsvec_Contens[nCntLine].clear();
		vsvec_Contens[nCntLine].shrink_to_fit();
	}
	vsvec_Contens.clear();
	vsvec_Contens.shrink_to_fit();

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// UI�X�N���v�g�̓ǂݍ���
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CUi::UiScriptLoad(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntError = 0;				// �J�E���g���[�h
	int	nCntUi = 0;					// �J�E���gUI
	char cComp[128];				// ����̕�����̔�r
	char cRaedText[128];			// �����Ƃ��ēǂݎ��p
	char cHeadText[128];			// ��r����悤
	char cDie[128];					// �s�K�v�ȕ���

	for (int nCntLoad = 0; nCntLoad < (signed)m_vec_String.size(); nCntLoad++)
	{
		// �ǂݍ���UI�t�@�C���̏���𒴂����Ƃ�
		// ->���[�v�𔲂���
		if (nCntLoad >= UITYPE_MAX)
		{
			break;
		}
		nCntUi = 0;
		cHeadText[0] = '\0';
		cHeadText[0] = '\0';
		// �t�@�C�����J����Ă��Ȃ�������
		if ((pFile = fopen(m_vec_String[nCntLoad].c_str(), "r")) == NULL)
		{// ���b�Z�[�W�̕\��
#ifdef _DEBUG
			CCalculation::Messanger("UI�̃e�L�X�g�t�@�C����������܂���ł���");
#endif // _DEBUG
			continue;
		}
		// �X�N���v�g�����݂��Ă��邩�`�F�b�N
		if (!CCalculation::Existenceofscript(pFile))
		{
#ifdef _DEBUG
			CCalculation::Messanger("UI��[SCRIP]��������܂���\n�I�����Ă�������");
#endif // _DEBUG
			fclose(pFile);
			continue;
		}
		// �G���h�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);

			// ���s��������
			if (strcmp(cHeadText, "\n") == 0)
			{
			}

			// ���f���Z�b�g��������
			else if (strcmp(cHeadText, "SET") == 0)
			{
				// �i�[����
				m_vec_UiLoad[nCntLoad].emplace_back(UI_LOAD());
				// ��r�̏�����
				cComp[0] = '\0';
				// �ǂݍ���Ǖ�������
				sscanf(cRaedText, "%s %s", &cDie, &cComp);
				// �v���[���c�p
				if (strcmp(cComp, "PRESENTS") == 0)
				{
					// �������m��
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents = new PRESENTS_LOAD;
				}
				// �G���h���f���Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_SET") != 0)
				{
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// �ʒu���ǂݍ���
					if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).pos.x,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).pos.y,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).pos.z);
					}
					// �ʒu���ǂݍ���
					else if (strcmp(cHeadText, "COL") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.r,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.b,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.g,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.a
						);
					}
					// �T�C�Y���ǂݍ���
					else if (strcmp(cHeadText, "SIZE") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).size.x,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).size.y);
					}
					// �p�x���ǂݍ���
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).fRot);
					}
					// �e�N�X�`���[���W(�ŏ�)���ǂݍ���
					else if (strcmp(cHeadText, "FIRST_TEX") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_first.x,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_first.y);
					}
					// �e�N�X�`���[���W(�Ō�)���ǂݍ���
					else if (strcmp(cHeadText, "LAST_TEX") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_last.x,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_last.y);
					}
					// �e�N�X�`���[�^�C�v���ǂݍ���
					else if (strcmp(cHeadText, "TEXTYPE") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).nTexType);
					}
					// �I�t�Z�b�g�^�C�v���ǂݍ���
					else if (strcmp(cHeadText, "OFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).nOffsetType);
					}
					// �v���[���c�p�̕ϐ����k���ł͂Ȃ����������
					else if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents != NULL)
					{
						// �t�F�[�h�C�����ǂݍ���
						if (strcmp(cHeadText, "FADE_IN") == 0)
						{
							// �t�F�[�h�C���̃������m��
							m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn = new FADEIN_LOAD;
							// �G���h�t�F�[�h�C��������܂Ń��[�v
							while (strcmp(cHeadText, "END_FADE_IN") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// �X�^�[�g�t�F�[�h�^�C�����ǂݍ���
								if (strcmp(cHeadText, "STARTFADETIME") == 0)
								{
									sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
										&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->FadeStart);
								}
								// �t�F�[�h�X�L�b�v���ǂݍ���
								else if (strcmp(cHeadText, "FADESKIP") == 0)
								{
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->bSkip = true;
								}
								// �t�F�[�h�F���ǂݍ���
								else if (strcmp(cHeadText, "FADE_COL") == 0)
								{
									// �t�F�[�h�J���[�̃������m��
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol = new FADE_COL_LOAD;
									// �G���h�t�F�[�h�J���[������܂Ń��[�v
									while (strcmp(cHeadText, "END_FADE_COL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// �����F���ǂݍ���
										if (strcmp(cHeadText, "BEGIN_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.a);
										}
										// �I���F���ǂݍ���
										else if (strcmp(cHeadText, "END_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.a);
										}
										// �ő�t���[�����ǂݍ���
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->nMaxfram);
										}
									}
								}
								// �t�F�[�h�ʒu���ǂݍ���
								else if (strcmp(cHeadText, "FADE_POS") == 0)
								{
									// �t�F�[�h�ʒu�̃������m��
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos = new FADE_POS_LOAD;
									// �G���h�t�F�[�h�ʒu������܂Ń��[�v
									while (strcmp(cHeadText, "END_FADE_POS") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// �����ʒu���ǂݍ���
										if (strcmp(cHeadText, "BEGIN_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->BeginPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->BeginPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->BeginPos.z);
										}
										// �I���ʒu���ǂݍ���
										else if (strcmp(cHeadText, "END_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->EndPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->EndPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->EndPos.z);
										}
										// �ő�t���[�����ǂݍ���
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->nMaxfram);
										}
									}
								}
							}
						}
						// �t�F�[�h�A�E�g���ǂݍ���
						else if (strcmp(cHeadText, "FADE_OUT") == 0)
						{
							// �t�F�[�h�A�E�g�������m��
							m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut = new FADEOUT_LOAD;
							// �G���h�t�F�[�h�A�E�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_FADE_OUT") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// �X�^�[�g�t�F�[�h�^�C�����ǂݍ���
								if (strcmp(cHeadText, "STARTFADETIME") == 0)
								{
									sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
										&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->FadeStart);
								}
								// �t�F�[�h�X�L�b�v���ǂݍ���
								else if (strcmp(cHeadText, "FADESKIP") == 0)
								{
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->bSkip = true;
								}
								// �t�F�[�h�F���ǂݍ���
								else if (strcmp(cHeadText, "FADE_COL") == 0)
								{
									// �t�F�[�h�J���[�̃������m��
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol = new FADE_COL_LOAD;
									// �G���h�t�F�[�h�J���[������܂Ń��[�v
									while (strcmp(cHeadText, "END_FADE_COL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// �����F���ǂݍ���
										if (strcmp(cHeadText, "BEGIN_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.a);
										}
										// �I���F���ǂݍ���
										else if (strcmp(cHeadText, "END_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.a);
										}
										// �ő�t���[�����ǂݍ���
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->nMaxfram);
										}
									}
								}
								// �t�F�[�h�ʒu���ǂݍ���
								else if (strcmp(cHeadText, "FADE_POS") == 0)
								{
									// �t�F�[�h�ʒu�̃������m��
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos = new FADE_POS_LOAD;
									// �G���h�t�F�[�h�ʒu������܂Ń��[�v
									while (strcmp(cHeadText, "END_FADE_POS") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// �����ʒu���ǂݍ���
										if (strcmp(cHeadText, "BEGIN_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->BeginPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->BeginPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->BeginPos.z);
										}
										// �I���ʒu���ǂݍ���
										else if (strcmp(cHeadText, "END_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->EndPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->EndPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->EndPos.z);
										}
										// �ő�t���[�����ǂݍ���
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->nMaxfram);
										}
									}
								}
							}
						}
						// �������ǂݍ���
						else if (strcmp(cHeadText, "COOPERATION") == 0)
						{
							// �����������m��
							m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation = new COOPERATION_LOAD;
							// �G���h����������܂Ń��[�v
							while (strcmp(cHeadText, "END_COOPERATION") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// �g�嗦���ǂݍ���
								if (strcmp(cHeadText, "SCAL") == 0)
								{
									// �g�嗦�̃������m��
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal = new COOPERATION_SCAL_LOAD;
									// �G���h�g�嗦������܂Ń��[�v
									while (strcmp(cHeadText, "END_SCAL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// �g�嗦���ǂݍ���
										if (strcmp(cHeadText, "CHANGESCAL") == 0)
										{
											sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal->fChangeScal);
										}
										// �ω��̎��ԏ��ǂݍ���
										else if (strcmp(cHeadText, "TIMECHANGE") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal->nTimeChange);
										}
									}
								}
								// �F���ǂݍ���
								else if (strcmp(cHeadText, "COL") == 0)
								{
									// �F�̃������m��
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol = new COOPERATION_COL_LOAD;
									// �G���h�F������܂Ń��[�v
									while (strcmp(cHeadText, "END_COL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// �ω�����F���ǂݍ���
										if (strcmp(cHeadText, "CHANGECOL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.a);
										}
										// �؂�ւ����ԏ��ǂݍ���
										else if (strcmp(cHeadText, "TIMESWITCH") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->nTimeSwitch);
										}
										// �؂�ւ��ɂ����鎞�ԏ��ǂݍ���
										else if (strcmp(cHeadText, "TIMECHANGE") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->nTimeChange);
										}
									}
								}
							}
						}
					}
					// �G���[�J�E���g���C���N�������g
					nCntError++;
					if (nCntError > FILELINE_ERROW)
					{// �G���[
						nCntError = 0;
						fclose(pFile);
						CCalculation::Messanger("UI�Z�b�g������܂���");
						return E_FAIL;
					}
				}
				// UI�J�E���g�A�b�v
				nCntUi++;
			}
			// �G���[�J�E���g���C���N�������g
			nCntError++;
			if (nCntError > FILELINE_ERROW)
			{// �G���[
				nCntError = 0;
				fclose(pFile);
				CCalculation::Messanger("�G���h�X�N���v�g������܂���");
				return E_FAIL;
			}
		}
		fclose(pFile);
	}
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S�\�[�X�̊J������
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::UnLoad(void)
{
	// �SUI�^�C�v�̃��[�v
	for (int nCntLoad = 0; nCntLoad < UITYPE_MAX; nCntLoad++)
	{
		// UI�̃��[�v
		for (int nCntUi = 0; nCntUi < (signed)m_vec_UiLoad[nCntLoad].size(); nCntUi++)
		{
			// �v���[���̏�񂪃k���̏ꍇ���[�v�X�L�b�v
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents == NULL)
			{
				continue;
			}
			// �t�F�[�h�C���̏�񂪃k���ł͂Ȃ��ꍇ�����J������
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn != NULL)
			{
				// �t�F�[�h�F�̏�񂪃k���ł͂Ȃ��ꍇ�����J������
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol = NULL;
				}
				// �t�F�[�h�ʒu�̏�񂪃k���ł͂Ȃ��ꍇ�����J������
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos = NULL;
				}
				// �t�F�[�h�C���̏��̊J��
				delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn;
				m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn = NULL;
			}
			// �t�F�[�h�A�E�g�̏�񂪃k���ł͂Ȃ��ꍇ�����J������
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut != NULL)
			{
				// �t�F�[�h�F�̏�񂪃k���ł͂Ȃ��ꍇ�����J������
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol = NULL;
				}
				// �t�F�[�h�ʒu�̏�񂪃k���ł͂Ȃ��ꍇ�����J������
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos = NULL;
				}
				// �t�F�[�h�A�E�g�̏��̊J��
				delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut;
				m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut = NULL;
			}
			// �����̏�񂪃k���ł͂Ȃ��ꍇ�����J������
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation != NULL)
			{
				// �F��񂪃k���ł͂Ȃ��ꍇ�����J������
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol = NULL;
				}
				// �g�嗦�̏�񂪃k���ł͂Ȃ��ꍇ�����J������
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal = NULL;
				}
				// �����̏��̊J��
				delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation;
				m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation = NULL;
			}
			// �v���[���̏��̊J��
			delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents;
			m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents = NULL;
		}
		// std::vector�̊J��
		std::vector<UI_LOAD>().swap(m_vec_UiLoad[nCntLoad]);
	}
	m_vec_String.clear();
	m_vec_String.shrink_to_fit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����(�V�[���Ǘ�)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi * CUi::Create(
	UITYPE const &Uitype,
	int const &nUi
)
{
	// �ϐ��錾
	CUi * pUi;
	// �������m��
	pUi = new CUi;
	// UI�^�C�v�̐ݒ�
	pUi->m_Uitype = Uitype;
	// UIID�ݒ�
	pUi->m_nUi = nUi;
	// �V�[���Ǘ��ݒ�
	pUi->ManageSetting(CScene::LAYER_UI);
	// ����������
	pUi->Init();
	return pUi;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����(�l�Ǘ�)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CUi> CUi::Create_Self(
	UITYPE const &Uitype,
	int const &nUi
)
{
	// �ϐ��錾
	std::unique_ptr<CUi> pUi(new CUi);
	// UI�^�C�v�̐ݒ�
	pUi->m_Uitype = Uitype;
	// UIID�ݒ�
	pUi->m_nUi = nUi;
	// ����������
	pUi->Init();
	return pUi;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾂��̂̍쐬(�V�[���Ǘ�)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::VEC_P_UI CUi::LoadCreate(UITYPE const &Uitype)
{
	// �g�p��Ԃ�true�Ȃ甲����
	if (m_sta_UiUse[Uitype])
	{
		return VEC_P_UI();
	}
	// �ϐ��錾
	VEC_P_UI vec_pUi;	// UI���i�[�p�ϐ�
	for (int nCntUi = 0; nCntUi < (signed)m_vec_UiLoad[Uitype].size(); nCntUi++)
	{
		// �쐬����(�V�[���Ǘ�)
		vec_pUi.push_back(Create(Uitype, nCntUi));
	}
	// �g�p��Ԃ�true��
	m_sta_UiUse[Uitype] = true;
	// UI����n��
	return vec_pUi;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾂��̂̍쐬(�l�Ǘ�)
// �Ԃ��Ƃ���std::move���g�p���邱��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::VEC_UNI_UI CUi::LoadCreate_Self(UITYPE const & Uitype)
{
	// �ϐ��錾
	VEC_UNI_UI vec_pUi;	// UI���i�[�p�ϐ�
	// �g�p��Ԃ�true�Ȃ甲����
	if (m_sta_UiUse[Uitype])
	{
		return vec_pUi;
	}
	for (int nCntUi = 0; nCntUi < (signed)m_vec_UiLoad[Uitype].size(); nCntUi++)
	{
		// �쐬����(�l�Ǘ�)
		vec_pUi.push_back(std::move(Create_Self(Uitype, nCntUi)));
	}
	// �g�p��Ԃ�true��
	m_sta_UiUse[Uitype] = true;
	// UI����n��
	return vec_pUi;
}