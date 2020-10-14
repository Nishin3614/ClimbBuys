// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�}�b�v�����̐���[3Dmap.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Dmap.h"
#include "manager.h"
#include "debugproc.h"
#include "floor.h"
#include "meshwall.h"
#include "scene_X.h"
#include "scene_three.h"
#include "solider.h"
#include "columncollision.h"
#include "rectcollision.h"
#include "spherecollision.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MANAGER_FILE ("data/LOAD/MAPPING/map_manager.txt")

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<std::vector<C3DMap::OBJECT>> C3DMap::m_vec_obj;
std::vector<std::vector<C3DMap::CHARACTER>> C3DMap::m_vec_char;
std::vector<std::vector<C3DMap::POLYGON>> C3DMap::m_vec_polygon;
std::vector<std::vector<C3DMap::FLOOR>> C3DMap::m_vec_floor;
std::vector<std::vector<C3DMap::WALL>> C3DMap::m_vec_wall;
std::vector<std::vector<C3DMap::COLLISION>> C3DMap::m_vec_Collision;
std::vector<std::string> C3DMap::m_vec_String;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DMap::C3DMap()
{
	m_nType = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DMap::~C3DMap()
{
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::Debug(void)
{
	ImGui::Begin("3DMap::Collision");
	for (int nCntCollision = 0; nCntCollision < (signed)m_vec_Collision[MAP_STAGE_1].size(); nCntCollision++)
	{
		std::string sNum = "Collision[" + std::to_string(nCntCollision) + "]";

		if (ImGui::TreeNode(sNum.c_str()))
		{
			ImGui::DragFloat3("pos", m_vec_Collision[MAP_STAGE_1][nCntCollision].pos);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾏��𐶐�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::LoadCreate(MAP const &map)
{
	// �ϐ��錾
	int nCntMap = 0;
	// �ǂݍ��񂾏��̐���
	// �V�[��X
	for (nCntMap = 0; nCntMap < (signed)m_vec_obj[map].size(); nCntMap++)
	{
		// �V�[��X�̐���
		CScene_X::Create(
			m_vec_obj[map][nCntMap].pos,
			m_vec_obj[map][nCntMap].rot,
			m_vec_obj[map][nCntMap].nModelType,
			true
		);
	}
	// �G�L�����N�^�[�̐���
	for (nCntMap = 0; nCntMap < (signed)m_vec_char[map].size(); nCntMap++)
	{
		// �G1
		if (m_vec_char[map][nCntMap].nCharacter == CCharacter::CHARACTER_NPC)
		{
				CSolider::Create(m_vec_char[map][nCntMap].pos,
				m_vec_char[map][nCntMap].rot
			);
		}
	}
	// �|���S��
	for (nCntMap = 0; nCntMap < (signed)m_vec_polygon[map].size(); nCntMap++)
	{
		// �V�[��X�̐���
		CScene_THREE::Create(
			CScene_THREE::OFFSET_TYPE_VERTICAL_CENTER,
			m_vec_polygon[map][nCntMap].pos,
			D3DXVECTOR3(m_vec_polygon[map][nCntMap].size.x, m_vec_polygon[map][nCntMap].size.y, 0.0f),
			m_vec_polygon[map][nCntMap].nTexType,
			m_vec_polygon[map][nCntMap].rot
		);
	}
	// ��
	for (nCntMap = 0; nCntMap < (signed)m_vec_floor[map].size(); nCntMap++)
	{
		// ���̐���
		CFloor::Create(
			m_vec_floor[map][nCntMap].pos,
			D3DXVECTOR3(m_vec_floor[map][nCntMap].size.x,0.0f, m_vec_floor[map][nCntMap].size.y),
			D3DXCOLOR_INI,
			m_vec_floor[map][nCntMap].rot,
			m_vec_floor[map][nCntMap].nBlockWidth,
			m_vec_floor[map][nCntMap].nBlockDepth,
			m_vec_floor[map][nCntMap].nTexType
			);
	}
	// ��
	for (nCntMap = 0; nCntMap < (signed)m_vec_wall[map].size(); nCntMap++)
	{
		// �ǂ̐���
		CMeshwall::Create(
			m_vec_wall[map][nCntMap].pos,
			D3DXVECTOR3(m_vec_wall[map][nCntMap].size.x, m_vec_wall[map][nCntMap].size.y,0.0f),
			m_vec_wall[map][nCntMap].rot,
			m_vec_wall[map][nCntMap].nBlockWidth,
			m_vec_wall[map][nCntMap].nBlockDepth,
			m_vec_wall[map][nCntMap].nTexType
			);
	}
	// �����蔻��̐���
	for (nCntMap = 0; nCntMap < (signed)m_vec_Collision[map].size(); nCntMap++)
	{
		// ��`����NULL�`�F�b�N
		if (m_vec_Collision[map][nCntMap].uni_Rect != NULL)
		{
			// ��`�̐���
			CRectCollision::Create(
				m_vec_Collision[map][nCntMap].uni_Rect->size,
				D3DVECTOR3_ZERO,
				(CCollision::OBJTYPE)m_vec_Collision[map][nCntMap].nObjType,
				NULL,
				NULL,
				false,
				m_vec_Collision[map][nCntMap].bPush,
				&m_vec_Collision[map][nCntMap].pos
			);
		}
		// ������NULL�`�F�b�N
		else if (m_vec_Collision[map][nCntMap].uni_Sphere != NULL)
		{
			// ���̐���
			CSphereCollision::Create(
				m_vec_Collision[map][nCntMap].uni_Sphere->fRadius,
				D3DVECTOR3_ZERO,
				(CCollision::OBJTYPE)m_vec_Collision[map][nCntMap].nObjType,
				NULL,
				NULL,
				false,
				m_vec_Collision[map][nCntMap].bPush,
				&m_vec_Collision[map][nCntMap].pos
			);
		}
		// �~������NULL�`�F�b�N
		else if (m_vec_Collision[map][nCntMap].uni_Column != NULL)
		{
			// �~���̐���
			CColumnCollision::Create(
				m_vec_Collision[map][nCntMap].uni_Column->fRadius,
				m_vec_Collision[map][nCntMap].uni_Column->fVertical,
				D3DVECTOR3_ZERO,
				(CCollision::OBJTYPE)m_vec_Collision[map][nCntMap].nObjType,
				NULL,
				NULL,
				false,
				m_vec_Collision[map][nCntMap].bPush,
				&m_vec_Collision[map][nCntMap].pos
			);
		}
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�N���v�g�ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::LoadScript(char *add)
{
	FILE *pFile = NULL;																	// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	char cDie[128];

	pFile = fopen(add, "r");				// �t�@�C�����J���܂��͍��

	if (pFile != NULL)						//�t�@�C�����ǂݍ��߂��ꍇ
	{
		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//�X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//�G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "FIELDSET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�
						CFloor *pField;
						pField = CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(50.0f, 50.0f, 50.0f),
							D3DXCOLOR_INI,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							50,
							50,
							7);

						if (pField != NULL)
						{
							//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_FIELDSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								if (strcmp(cHeadText, "POS") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 pos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pos.x,
										&pos.y,
										&pos.z);

									// �ʒu�̐ݒ�
									pField->Scene_SetPos(pos);
								}
								else if (strcmp(cHeadText, "CHECK_HEIGHT") == 0)
								{// �v�Z����K�v�̂��郁�b�V�����ǂ���
								 //int nCal;
								 //sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nCal);
								 //pField->SetCalculation((nCal == 0) ? true : false);
									pField->SetCalculation(true);
								}
								else if (strcmp(cHeadText, "VERTEXINFO") == 0)
								{//�p�[�c�����̂Ƃ�
									pField->vertexMove(pFile);			// ���_���̍쐬(�t�@�C������)
								}
							}
						}
					}
				}
			}
		}
		fclose(pFile);				// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "�n�ʏ��̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���\�[�X���ǂݍ��ސݒ�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::Load(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();
	// �}�l�[�W���[�ǂݍ���
	ManagerLoad();
	// �X�N���v�g�ǂݍ���
	ScriptLoad();

	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�l�[�W���[�ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::ManagerLoad(void)
{
	// �ϐ��錾
	// �t�@�C���̒��g�i�[�p
	std::vector<std::vector<std::string>> vsvec_Contens;
	// �t�@�C���̒��g���擾����
	vsvec_Contens = CCalculation::FileContens(MANAGER_FILE, '\0');
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
	// �t�@�C���̒��g���̊J��
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		vsvec_Contens[nCntLine].clear();
		vsvec_Contens[nCntLine].shrink_to_fit();
	}
	vsvec_Contens.clear();
	vsvec_Contens.shrink_to_fit();
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�N���v�g�ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::ScriptLoad(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	char cRaedText[128] = {};	// �����Ƃ��ēǂݎ��p
	char cHeadText[128] = {};	// ��r����悤
	char cDie[128] = {};			// �s�K�v�ȕ���
	int nCntError = 0;		// �G���[�J�E���g
	int nCntObj,			// �I�u�W�F�J�E���g
		nCntChar,			// �L�����N�^�[�J�E���g
		nCntPoly,			// �|���S���J�E���g
		nCntFloor,			// ���J�E���g
		nCntWall,			// �ǃJ�E���g
		nCntCollision = 0;	// �����蔻��J�E���g
	// ����ۑ�
	for (int nCntScript = 0; nCntScript < (signed)m_vec_String.size(); nCntScript++)
	{
		// �ϐ��錾
		std::vector<OBJECT> vec_obj;
		std::vector<CHARACTER> vec_char;
		std::vector<POLYGON> vec_bill;
		std::vector<FLOOR> vec_floor;
		std::vector<WALL> vec_wall;
		std::vector<COLLISION> vec_collision;
		nCntObj = 0;
		nCntChar = 0;
		nCntPoly = 0;
		nCntFloor = 0;
		nCntWall = 0;
		nCntCollision = 0;
		// �t�@�C�����J����Ă��Ȃ�������
		if ((pFile = fopen(m_vec_String[nCntScript].c_str(), "r")) == NULL)
		{// ���b�Z�[�W�̕\��
#ifdef _DEBUG
			std::string sErrow = m_vec_String[nCntScript] + "��������܂���";
			CCalculation::Messanger(sErrow.c_str());
#endif // _DEBUG
			return E_FAIL;
		}
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
														// �t�@�C���̃p�X���ǂݍ���
			// �G���[�J�E���g���C���N�������g
			nCntError++;
			if (nCntError > FILELINE_ERROW)
			{// �G���[
				nCntError = 0;
				fclose(pFile);
				CCalculation::Messanger("�X�^�e�B�b�N�I�u�W�F�N�g�̃X�N���v�g������܂���");
				return E_FAIL;
			}
		}

		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂Ń��[�v
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// ���s��������
				if (strcmp(cHeadText, "\n") == 0)
				{
				}
				// �I�u�W�F�N�g��������
				else if (strcmp(cHeadText, "OBJECTSET") == 0)
				{
					// �I�u�W�F�N�g�ǉ�
					vec_obj.push_back(OBJECT());
					// �G���h�I�u�W�F�N�g�Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_OBJECTSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "MODELTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_obj[nCntObj].nModelType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_obj[nCntObj].pos.x,
								&vec_obj[nCntObj].pos.y,
								&vec_obj[nCntObj].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_obj[nCntObj].rot.x,
								&vec_obj[nCntObj].rot.y,
								&vec_obj[nCntObj].rot.z);
						}
						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// �G���[
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("�G���h�I�u�W�F�N�g�Z�b�g������܂���");
							return E_FAIL;
						}
					}
					// �J�E���g�I�u�W�F
					nCntObj++;
				}
				// �L�����N�^�[��������
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					// �L�����N�^�[�ǉ�
					vec_char.push_back(CHARACTER());
					// �G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "CHARACTERTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_char[nCntChar].nCharacter);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_char[nCntChar].pos.x,
								&vec_char[nCntChar].pos.y,
								&vec_char[nCntChar].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_char[nCntChar].rot.x,
								&vec_char[nCntChar].rot.y,
								&vec_char[nCntChar].rot.z);
						}
						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// �G���[
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("�G���h�L�����N�^�[�Z�b�g������܂���");
							return E_FAIL;
						}
					}
					// �J�E���g�L����
					nCntChar++;
				}
				// �|���S����������
				else if (strcmp(cHeadText, "POLYGONSET") == 0)
				{
					// �|���S���ǉ�
					vec_bill.push_back(POLYGON());
					// �G���h�|���S���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_POLYGONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e�N�X�`���[�^�C�v���ǂݍ���
						if (strcmp(cHeadText, "TEXTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_bill[nCntPoly].nTexType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_bill[nCntPoly].pos.x,
								&vec_bill[nCntPoly].pos.y,
								&vec_bill[nCntPoly].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_bill[nCntPoly].rot.x,
								&vec_bill[nCntPoly].rot.y,
								&vec_bill[nCntPoly].rot.z);
						}
						// �T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
								&vec_bill[nCntPoly].size.x,
								&vec_bill[nCntPoly].size.y
							);
						}
						// �r���{�[�h�������邩�����Ȃ������ǂݍ���
						else if (strcmp(cHeadText, "BILLBOARD") == 0)
						{
							vec_bill[nCntPoly].bBillboard = true;
						}
						// Z�o�b�t�@�������邩�����Ȃ������ǂݍ���
						else if (strcmp(cHeadText, "ZENEBLE") == 0)
						{
							vec_bill[nCntPoly].bZEneble = true;
						}
						// ���C�e�B���O�������邩�����Ȃ������ǂݍ���
						else if (strcmp(cHeadText, "LIGHTING") == 0)
						{
							vec_bill[nCntPoly].bLighting = true;
						}
						// �A���t�@�u�����h�������邩�����Ȃ������ǂݍ���
						else if (strcmp(cHeadText, "ALPHA") == 0)
						{
							vec_bill[nCntPoly].bAlpha = true;
						}

						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// �G���[
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("�G���h�|���S���Z�b�g������܂���");
							return E_FAIL;
						}
					}
					// �J�E���g�|���S��
					nCntPoly++;
				}
				// ����������
				else if (strcmp(cHeadText, "FLOORSET") == 0)
				{
					// ���ǉ�
					vec_floor.push_back(FLOOR());
					// �G���h���Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_FLOORSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "TEXTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_floor[nCntFloor].nTexType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_floor[nCntFloor].pos.x,
								&vec_floor[nCntFloor].pos.y,
								&vec_floor[nCntFloor].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_floor[nCntFloor].rot.x,
								&vec_floor[nCntFloor].rot.y,
								&vec_floor[nCntFloor].rot.z);
						}
						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_floor[nCntFloor].nBlockDepth
							);
						}
						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_floor[nCntFloor].nBlockWidth
							);
						}
						// �T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
								&vec_floor[nCntFloor].size.x,
								&vec_floor[nCntFloor].size.y
							);
						}

						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// �G���[
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("�G���h���Z�b�g������܂���");
							return E_FAIL;
						}
					}
					// �J�E���g��
					nCntFloor++;
				}
				// �ǂ�������
				else if (strcmp(cHeadText, "WALLSET") == 0)
				{
					// �ǒǉ�
					vec_wall.push_back(WALL());
					// �G���h�ǃZ�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_WALLSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �e���ǂݍ���
						if (strcmp(cHeadText, "TEXTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_wall[nCntWall].nTexType);
						}

						// �ʒu���ǂݍ���
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_wall[nCntWall].pos.x,
								&vec_wall[nCntWall].pos.y,
								&vec_wall[nCntWall].pos.z);
						}

						// ��]���ǂݍ���
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_wall[nCntWall].rot.x,
								&vec_wall[nCntWall].rot.y,
								&vec_wall[nCntWall].rot.z);
						}
						// �c�u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_wall[nCntWall].nBlockDepth
							);
						}
						// ���u���b�N���ǂݍ���
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_wall[nCntWall].nBlockWidth
							);
						}
						// �T�C�Y���ǂݍ���
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f",
								&cDie, &cDie,
								&vec_wall[nCntWall].size.x,
								&vec_wall[nCntWall].size.y
							);
						}

						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// �G���[
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("�G���h�ǃZ�b�g������܂���");
							return E_FAIL;
						}
					}
					// �J�E���g��
					nCntWall++;
				}
				// �����蔻��
				else if (strcmp(cHeadText, "COLLISIONSET") == 0)
				{
					// �����蔻��ǉ�
					vec_collision.push_back(COLLISION());
					// �G���h�����蔻��Z�b�g������܂Ń��[�v
					while (strcmp(cHeadText, "END_COLLISIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// �ʒu���ǂݍ���
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_collision[nCntCollision].pos.x,
								&vec_collision[nCntCollision].pos.y,
								&vec_collision[nCntCollision].pos.z);
						}
						// �I�u�W�F�N�g�^�C�v���ǂݍ���
						else if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_collision[nCntCollision].nObjType);
						}
						// �����o�����ǂݍ���
						else if (strcmp(cHeadText, "PUSH") == 0)
						{
							vec_collision[nCntCollision].bPush = true;
						}
						// ��`����񂪗�����
						else if (strcmp(cHeadText, "RECT") == 0)
						{
							// ��`���̃������m��
							vec_collision[nCntCollision].uni_Rect.reset(new RECT);
							// �G���h��`�Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_RECT") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// ���a���ǂݍ���
								if (strcmp(cHeadText, "SIZE") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Rect->size.x,
										&vec_collision[nCntCollision].uni_Rect->size.y,
										&vec_collision[nCntCollision].uni_Rect->size.z);
								}
								// ��]���ǂݍ���
								else if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Rect->rot.x,
										&vec_collision[nCntCollision].uni_Rect->rot.y,
										&vec_collision[nCntCollision].uni_Rect->rot.z);
								}
							}
						}
						// ������񂪗�����
						else if (strcmp(cHeadText, "SPHERE") == 0)
						{
							// �����̃������m��
							vec_collision[nCntCollision].uni_Sphere.reset(new SPHERE);
							// �G���h���Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_SPHERE") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// ���a���ǂݍ���
								if (strcmp(cHeadText, "RADIUS") == 0)
								{
									sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Sphere->fRadius);
								}
							}
						}
						// �~����񂪗�����
						else if (strcmp(cHeadText, "COLUMN") == 0)
						{
							// �~�����̃������m��
							vec_collision[nCntCollision].uni_Column.reset(new COLUMN);
							// �G���h�~���Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_COLUMN") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// ��]���ǂݍ���
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Column->rot.x,
										&vec_collision[nCntCollision].uni_Column->rot.y,
										&vec_collision[nCntCollision].uni_Column->rot.z);
								}
								// ���a���ǂݍ���
								else if (strcmp(cHeadText, "RADIUS") == 0)
								{
									sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Column->fRadius);
								}
								// �c���ǂݍ���
								else if (strcmp(cHeadText, "VERTICAL") == 0)
								{
									sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Column->fVertical);
								}
							}
						}

						// �G���[�J�E���g���C���N�������g
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// �G���[
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("�G���h�����蔻��Z�b�g������܂���");
							return E_FAIL;
						}
					}
					// �J�E���g�����蔻��
					nCntCollision++;
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
			// �I�u�W�F�N�g�̊i�[
			m_vec_obj.push_back(std::move(vec_obj));
			// �L�����N�^�[�̊i�[
			m_vec_char.push_back(std::move(vec_char));
			// �|���S���̊i�[
			m_vec_polygon.push_back(std::move(vec_bill));
			// ���̊i�[
			m_vec_floor.push_back(std::move(vec_floor));
			// �ǂ̊i�[
			m_vec_wall.push_back(std::move(vec_wall));
			// �����蔻��̊i�[
			m_vec_Collision.push_back(std::move(vec_collision));
		}
		// �t�@�C����
		fclose(pFile);
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾃��\�[�X�����J������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::UnLoad(void)
{
	// �I�u�W�F�N�g���̊J��
	m_vec_obj.clear();
	m_vec_obj.shrink_to_fit();
	// �L�����N�^�[���̊J��
	m_vec_char.clear();
	m_vec_char.shrink_to_fit();
	// �|���S�����̊J��
	m_vec_polygon.clear();
	m_vec_polygon.shrink_to_fit();
	// �����̊J��
	m_vec_floor.clear();
	m_vec_floor.shrink_to_fit();
	// �Ǐ��̊J��
	m_vec_wall.clear();
	m_vec_wall.shrink_to_fit();
	// �����蔻��̊e�`����̊J��
	for (int nCntMap = 0; nCntMap < (signed)m_vec_Collision.size(); nCntMap++)
	{
		for (int nCntCollision = 0; nCntCollision < (signed)m_vec_Collision[nCntMap].size(); nCntCollision++)
		{
			// ��`����NULL�`�F�b�N
			if (m_vec_Collision[nCntMap][nCntCollision].uni_Rect != NULL)
			{
				// ��`���̊J��
				m_vec_Collision[nCntMap][nCntCollision].uni_Rect.reset();
				m_vec_Collision[nCntMap][nCntCollision].uni_Rect = NULL;
			}
			// ������NULL�`�F�b�N
			else if (m_vec_Collision[nCntMap][nCntCollision].uni_Sphere != NULL)
			{
				// �����̊J��
				m_vec_Collision[nCntMap][nCntCollision].uni_Sphere.reset();
				m_vec_Collision[nCntMap][nCntCollision].uni_Sphere = NULL;
			}
			// �~������NULL�`�F�b�N
			else if (m_vec_Collision[nCntMap][nCntCollision].uni_Column != NULL)
			{
				// �~�����̊J��
				m_vec_Collision[nCntMap][nCntCollision].uni_Column.reset();
				m_vec_Collision[nCntMap][nCntCollision].uni_Column = NULL;
			}

		}
	}
	// �����蔻����̊J��
	m_vec_Collision.clear();
	m_vec_Collision.shrink_to_fit();
	// �}�l�[�W���[�}�b�v�̏��̊J��
	m_vec_String.clear();
	m_vec_String.shrink_to_fit();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �L�����N�^�[����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::CharaCreate(void)
{
}
