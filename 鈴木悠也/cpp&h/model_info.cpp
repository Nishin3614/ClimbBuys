// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f����񏈗� [model_info.cpp]
// Author : KOKI NISHIYAMA
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "model_info.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::CModel_info()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::~CModel_info()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���f���̃e�L�X�g�f�[�^�̓ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CModel_info::TextLoad(
	MODEL_ALL *pmodelAll,
	std::vector<int> &ModelFile,
	int const &nMaxMotion,
	int &nMaxkey,
	int &nMaxParts,
	const char * file_name
)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;				// ���f���J�E���g
	int	nCntMotion = 0;				// ���[�V�����J�E���g
	int	nCntKeySet = 0;				// �t���[����
	int	nCntKey = 0;				// ���f����
	char cRaedText[128];			// �����Ƃ��ēǂݎ��p
	char cHeadText[128];			// ��r����悤
	char cDie[128];					// �s�K�v�ȕ���
	int nCameraType = 0;			// �J�����^�C�v
	int nBeginBlust = 0;			// �����̐�����ѕ�
	int nCntObit_Basic = 0;			// �O�Պ�{�J�E���g
	int nCntObit_Detail = 0;		// �O�Տڍ׃J�E���g
	int nCntAttackCollision = 0;	// �U�������蔻��J�E���g
	int nCntCollision = 0;			// �����蔻��J�E���g
	MOTION_EFFECT motion_effect;	// ���[�V�����G�t�F�N�g���i�[

	//pmodelAll->pMotion = new MOTION[nMaxMotion];
	// �t�@�C���J
	pFile = fopen(file_name, "r");

	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// ������
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
		}

		// �G���h�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// ������
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// ���f���̌�
			if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxkey);
			}
			// ���f���t�@�C������������
			else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
			{
				// �ϐ��錾
				int nModelId;
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nModelId);
				// ���f���ԍ��i�[
				ModelFile.push_back(nModelId);
			}
			// �O�ՃZ�b�g��������
			else if (strcmp(cHeadText, "OBIT_PREPARATION") == 0)
			{
				// �O�Տ��̊m��
				pmodelAll->v_MeshObitLoad.push_back(MESHOBIT_BASIC());
				// �G���h�O�ՃZ�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_OBIT_PREPARATION") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// ���̐����ǂݍ���
					if (strcmp(cHeadText, "LINE") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nLine);
					}
					// �p�[�c���ǂݍ���
					else if (strcmp(cHeadText, "PARTS") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nPart);
					}
					// �n�_�I�t�Z�b�g���ǂݍ���
					else if (strcmp(cHeadText, "BEGINOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.z);
					}
					// �I�_�I�t�Z�b�g���ǂݍ���
					else if (strcmp(cHeadText, "ENDOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.z);
					}
					// �e�N�X�`���[�^�C�v���ǂݍ���
					else if (strcmp(cHeadText, "TEXTUREID") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nTexType);
					}
				}
				// �O�ՃJ�E���g�̍X�V
				nCntObit_Basic++;
			}
			// ���f���̃I�t�Z�b�g�p�[�c��������
			else if (strcmp(cHeadText, "NUM_PARTS") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxParts);
				// ���������f�������������m��
				pmodelAll->pModel_offset = new MODEL_OFFSET[nMaxParts];
			}
			// �p�[�c�Z�b�g��������
			else if (strcmp(cHeadText, "PARTSSET") == 0)
			{
				// �G���h�p�[�c�Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_PARTSSET") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// �e���ǂݍ���
					if (strcmp(cHeadText, "PARENT") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].nParent);
					}
					// �ʒu���ǂݍ���
					else if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].pos.x,
							&pmodelAll->pModel_offset[nCntModel].pos.y,
							&pmodelAll->pModel_offset[nCntModel].pos.z);
					}
					// ��]���ǂݍ���
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].rot.x,
							&pmodelAll->pModel_offset[nCntModel].rot.y,
							&pmodelAll->pModel_offset[nCntModel].rot.z);
					}
				}
				// ���f���̍X�V
				nCntModel++;
			}
			// ���[�V�����Z�b�g��������
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// �J�����^�C�v�̏�����
				nCameraType = 0;	// �J�����^�C�v
				// ���f���̏�����
				nCntKeySet = 0;
				// ���[�V�����̐���
				pmodelAll->pMotion.push_back(new MOTION);
				// �G���h���[�V�����Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
#ifdef _DEBUG
					// ���[�V�����̍ő吔�𒴂��Ă��܂����ꍇ
					// ���b�Z�[�W��m�点��
					if (nCntMotion >= nMaxMotion)
					{
						CCalculation::Messanger("model_info��TextLoad�֐�->���[�V�����̍ő吔�𒴂��Ă��܂��܂����B");
					}
#endif // _DEBUG

					// ������
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// ���[�v���邩�ǂ������ǂݍ���
					if (strcmp(cHeadText, "LOOP") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion]->nLoop);
					}
					// �L�[�����ǂݍ���
					else if (strcmp(cHeadText, "NUM_KEY") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pmodelAll->pMotion[nCntMotion]->nNumKey);
						// ���[�V����������
						pmodelAll->pMotion[nCntMotion]->KeyInfo = new KEY_INFO[pmodelAll->pMotion[nCntMotion]->nNumKey];
					}
					// �L�[�����ǂݍ���
					else if (strcmp(cHeadText, "CAMERAINFO") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nCameraType);
					}
					// ������ѕ����ǂݍ���
					else if (strcmp(cHeadText, "BEGINBLUST") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nBeginBlust);
					}
					// �O�Ղ̏ڍ׏��ǂݍ���
					else if (strcmp(cHeadText, "OBIT_DETAIL") == 0)
					{
						// �O�Ղ̏ڍׂ̊i�[
						pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.push_back(MESHOBIT_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nObitID
						);
						// �G���h�L�[������܂Ń��[�v
						while (strcmp(cHeadText, "END_OBIT_DETAIL") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �t���[�����ǂݍ���
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nBeginFram,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nEndFram
								);
							}
							// �n�_�F���ǂݍ���
							else if (strcmp(cHeadText, "BEGINCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.a
								);
							}
							// �I�_�F���ǂݍ���
							else if (strcmp(cHeadText, "ENDCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.a
								);
							}
						}
						// �O�Տڍ׃J�E���g�A�b�v
						nCntObit_Detail++;
					}
					// �����蔻��ڍ׏��ǂݍ���
					else if (strcmp(cHeadText, "COLLISION_DETAIL") == 0)
					{
						// �����蔻��ڍׂ̊i�[
						pmodelAll->pMotion[nCntMotion]->v_Collision.push_back(COLLISION_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nCollisionID
						);
						// �G���h�����蔻��ڍׂ�����܂Ń��[�v
						while (strcmp(cHeadText, "END_COLLISION_DETAIL") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);


							// �{�����ǂݍ���
							if (strcmp(cHeadText, "DOUBLE") == 0)
							{
								sscanf(cRaedText, "%s %s %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).fDouble
								);
							}
							// �t���[�����ǂݍ���
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nBeginFrame,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nEndFrame
								);
							}
							// �_���[�W���ǂݍ���
							else if (strcmp(cHeadText, "DAMAGE") == 0)
							{
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nDamage
								);
							}
						}
						// �O�Տڍ׃J�E���g�A�b�v
						nCntCollision++;
					}
					// �L�[���ǂݍ���
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// �L�[�̏�����
						nCntKey = 0;
						// �O�Ղ̏ڍׂ̏�����
						nCntObit_Detail = 0;
						// �����蔻��J�E���g�̏�����
						nCntCollision = 0;
						// ���f��������
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
						// ������ѕ������ݒ�
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust = nBeginBlust;
						// �G���h�L�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �U�������蔻��̕p�x���ǂݍ���
							if (strcmp(cHeadText, "NUMCOLLISION") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision);
							}
							// �J�������ǂݎ��
							else if (strcmp(cHeadText, "CAMERA") == 0)
							{
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera = new MOTION_CAMERA;
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_CAMERA") != 0)
								{
									// ������
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);
									// �����_�̃I�t�Z�b�g���ǂݍ���
									if (strcmp(cHeadText, "OFFSET_R") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.z);
									}
									// �������ǂݍ���
									else if (strcmp(cHeadText, "LENGTH") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fLength);
									}
									// �������ǂݍ���
									else if (strcmp(cHeadText, "HEIGHT") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fHeight);
									}
									// ��]���ǂݍ���
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.z);
									}
									// �������ǂݍ���
									else if (strcmp(cHeadText, "INTERTIA") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fIntertia);
									}
								}
							}
							// �G�t�F�N�g���ǂݎ��
							else if (strcmp(cHeadText, "EFFECT") == 0)
							{
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_EFFECT") != 0)
								{
									// ������
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// �I�t�Z�b�g���ǂݍ���
									if (strcmp(cHeadText, "OFFSET") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&motion_effect.offset.x,
											&motion_effect.offset.y,
											&motion_effect.offset.z);
									}
									// �J�n�E�I�����ԏ��ǂݍ���
									else if (strcmp(cHeadText, "START_END") == 0)
									{
										sscanf(cRaedText, "%s %s %d %d",
											&cDie, &cDie,
											&motion_effect.nStart,
											&motion_effect.nEnd
										);
									}
									// �p�[�c�ԍ����ǂݍ���
									else if (strcmp(cHeadText, "PARTS") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nKeyID
										);
									}
									// �p�[�e�B�N���ԍ����ǂݍ���
									else if (strcmp(cHeadText, "PARTICLE") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nParticleType
										);
									}
								}
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].v_MotionEffect.push_back(motion_effect);
							}

							// ������ѕ����ǂݍ���
							else if (strcmp(cHeadText, "BLUST") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust);
							}

							// �t���[�����ǂݍ���
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame);
								// ���[�V�����S�̂̃t���[�������Z
								pmodelAll->pMotion[nCntMotion]->nAllFrame +=
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								// �����蔻��̉񐔂��Ȃ��ꍇ�֐��𔲂���
								if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision > 0)
								{
									// �U������p�x
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime =
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame /
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision;
								}
								else
								{
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime = pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								}

							}
							// �L�[���ǂݍ���
							else if (strcmp(cHeadText, "KEY") == 0)
							{
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									// ������
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// �ʒu���ǂݍ���
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

										// �p�[�c�̈ʒu����������
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos += pmodelAll->pModel_offset[nCntKey].pos;
									}

									// ��]���ǂݍ���
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
										// �p�[�c�̈ʒu����������
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot += pmodelAll->pModel_offset[nCntKey].rot;
									}
								}
								// �J�E���g�L�[
								nCntKey++;
							}
						}
						// �J�E���g���f��
						nCntKeySet++;
					}
				}
				// �J�E���g�v���C���[
				nCntMotion++;
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}

#ifdef _DEBUG
	// ���[�V�����̍ő吔�𒴂��Ă��܂����ꍇ
	// ���b�Z�[�W��m�点��
	if (nCntMotion < nMaxMotion)
	{
		CCalculation::Messanger("model_info��TextLoad�֐�->���[�V�����̐�������܂���B");
	}
#endif // _DEBUG
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���f���̃e�L�X�g�f�[�^�̓ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CModel_info::TextLoad(
	MODEL_ALL *pmodelAll,
	std::vector<int> &ModelFile,
	int &nMaxkey,
	int &nMaxParts,
	const char * file_name
)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;				// ���f���J�E���g
	int	nCntMotion = 0;				// ���[�V�����J�E���g
	int	nCntKeySet = 0;				// �t���[����
	int	nCntKey = 0;				// ���f����
	char cRaedText[128];			// �����Ƃ��ēǂݎ��p
	char cHeadText[128];			// ��r����悤
	char cDie[128];					// �s�K�v�ȕ���
	int nCameraType = 0;			// �J�����^�C�v
	int nBeginBlust = 0;			// �����̐�����ѕ�
	int nCntObit_Basic = 0;			// �O�Պ�{�J�E���g
	int nCntObit_Detail = 0;		// �O�Տڍ׃J�E���g
	int nCntAttackCollision = 0;	// �U�������蔻��J�E���g
	int nCntCollision = 0;			// �����蔻��J�E���g
	MOTION_EFFECT motion_effect;	// ���[�V�����G�t�F�N�g���i�[

	// �t�@�C���J
	pFile = fopen(file_name, "r");

	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// ������
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
		}

		// �G���h�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// ������
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// ���f���̌�
			if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxkey);
			}
			// ���f���t�@�C������������
			else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
			{
				// �ϐ��錾
				int nModelId;
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nModelId);
				// ���f���ԍ��i�[
				ModelFile.push_back(nModelId);
			}
			// �O�ՃZ�b�g��������
			else if (strcmp(cHeadText, "OBIT_PREPARATION") == 0)
			{
				// �O�Տ��̊m��
				pmodelAll->v_MeshObitLoad.push_back(MESHOBIT_BASIC());
				// �G���h�O�ՃZ�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_OBIT_PREPARATION") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// ���̐����ǂݍ���
					if (strcmp(cHeadText, "LINE") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nLine);
					}
					// �p�[�c���ǂݍ���
					else if (strcmp(cHeadText, "PARTS") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nPart);
					}
					// �n�_�I�t�Z�b�g���ǂݍ���
					else if (strcmp(cHeadText, "BEGINOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.z);
					}
					// �I�_�I�t�Z�b�g���ǂݍ���
					else if (strcmp(cHeadText, "ENDOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.z);
					}
					// �e�N�X�`���[�^�C�v���ǂݍ���
					else if (strcmp(cHeadText, "TEXTUREID") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nTexType);
					}
				}
				// �O�ՃJ�E���g�̍X�V
				nCntObit_Basic++;
			}
			// ���f���̃I�t�Z�b�g�p�[�c��������
			else if (strcmp(cHeadText, "NUM_PARTS") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxParts);
				// ���������f�������������m��
				pmodelAll->pModel_offset = new MODEL_OFFSET[nMaxParts];
			}
			// �p�[�c�Z�b�g��������
			else if (strcmp(cHeadText, "PARTSSET") == 0)
			{
				// �G���h�p�[�c�Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_PARTSSET") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// �e���ǂݍ���
					if (strcmp(cHeadText, "PARENT") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].nParent);
					}
					// �ʒu���ǂݍ���
					else if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].pos.x,
							&pmodelAll->pModel_offset[nCntModel].pos.y,
							&pmodelAll->pModel_offset[nCntModel].pos.z);
					}
					// ��]���ǂݍ���
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].rot.x,
							&pmodelAll->pModel_offset[nCntModel].rot.y,
							&pmodelAll->pModel_offset[nCntModel].rot.z);
					}
				}
				// ���f���̍X�V
				nCntModel++;
			}
			// ���[�V�����Z�b�g��������
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// �J�����^�C�v�̏�����
				nCameraType = 0;	// �J�����^�C�v
									// ���f���̏�����
				nCntKeySet = 0;
				// ���[�V�����̐���
				pmodelAll->pMotion.push_back(new MOTION);
				// �G���h���[�V�����Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// ���[�v���邩�ǂ������ǂݍ���
					if (strcmp(cHeadText, "LOOP") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion]->nLoop);
					}
					// �L�[�����ǂݍ���
					else if (strcmp(cHeadText, "NUM_KEY") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pmodelAll->pMotion[nCntMotion]->nNumKey);
						// ���[�V����������
						pmodelAll->pMotion[nCntMotion]->KeyInfo = new KEY_INFO[pmodelAll->pMotion[nCntMotion]->nNumKey];
					}
					// �L�[�����ǂݍ���
					else if (strcmp(cHeadText, "CAMERAINFO") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nCameraType);
					}
					// ������ѕ����ǂݍ���
					else if (strcmp(cHeadText, "BEGINBLUST") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nBeginBlust);
					}
					// �O�Ղ̏ڍ׏��ǂݍ���
					else if (strcmp(cHeadText, "OBIT_DETAIL") == 0)
					{
						// �O�Ղ̏ڍׂ̊i�[
						pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.push_back(MESHOBIT_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nObitID
						);
						// �G���h�L�[������܂Ń��[�v
						while (strcmp(cHeadText, "END_OBIT_DETAIL") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �t���[�����ǂݍ���
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nBeginFram,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nEndFram
								);
							}
							// �n�_�F���ǂݍ���
							else if (strcmp(cHeadText, "BEGINCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.a
								);
							}
							// �I�_�F���ǂݍ���
							else if (strcmp(cHeadText, "ENDCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.a
								);
							}
						}
						// �O�Տڍ׃J�E���g�A�b�v
						nCntObit_Detail++;
					}
					// �����蔻��ڍ׏��ǂݍ���
					else if (strcmp(cHeadText, "COLLISION_DETAIL") == 0)
					{
						// �����蔻��ڍׂ̊i�[
						pmodelAll->pMotion[nCntMotion]->v_Collision.push_back(COLLISION_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nCollisionID
						);
						// �G���h�����蔻��ڍׂ�����܂Ń��[�v
						while (strcmp(cHeadText, "END_COLLISION_DETAIL") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);


							// �{�����ǂݍ���
							if (strcmp(cHeadText, "DOUBLE") == 0)
							{
								sscanf(cRaedText, "%s %s %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).fDouble
								);
							}
							// �t���[�����ǂݍ���
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nBeginFrame,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nEndFrame
								);
							}
							// �_���[�W���ǂݍ���
							else if (strcmp(cHeadText, "DAMAGE") == 0)
							{
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nDamage
								);
							}
						}
						// �O�Տڍ׃J�E���g�A�b�v
						nCntCollision++;
					}
					// �L�[���ǂݍ���
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// �L�[�̏�����
						nCntKey = 0;
						// �O�Ղ̏ڍׂ̏�����
						nCntObit_Detail = 0;
						// �����蔻��J�E���g�̏�����
						nCntCollision = 0;
						// ���f��������
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
						// ������ѕ������ݒ�
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust = nBeginBlust;
						// �G���h�L�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �U�������蔻��̕p�x���ǂݍ���
							if (strcmp(cHeadText, "NUMCOLLISION") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision);
							}
							// �J�������ǂݎ��
							else if (strcmp(cHeadText, "CAMERA") == 0)
							{
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera = new MOTION_CAMERA;
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_CAMERA") != 0)
								{
									// ������
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);
									// �����_�̃I�t�Z�b�g���ǂݍ���
									if (strcmp(cHeadText, "OFFSET_R") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.z);
									}
									// �������ǂݍ���
									else if (strcmp(cHeadText, "LENGTH") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fLength);
									}
									// �������ǂݍ���
									else if (strcmp(cHeadText, "HEIGHT") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fHeight);
									}
									// ��]���ǂݍ���
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.z);
									}
									// �������ǂݍ���
									else if (strcmp(cHeadText, "INTERTIA") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fIntertia);
									}
								}
							}
							// �G�t�F�N�g���ǂݎ��
							else if (strcmp(cHeadText, "EFFECT") == 0)
							{
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_EFFECT") != 0)
								{
									// ������
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// �I�t�Z�b�g���ǂݍ���
									if (strcmp(cHeadText, "OFFSET") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&motion_effect.offset.x,
											&motion_effect.offset.y,
											&motion_effect.offset.z);
									}
									// �J�n�E�I�����ԏ��ǂݍ���
									else if (strcmp(cHeadText, "START_END") == 0)
									{
										sscanf(cRaedText, "%s %s %d %d",
											&cDie, &cDie,
											&motion_effect.nStart,
											&motion_effect.nEnd
										);
									}
									// �p�[�c�ԍ����ǂݍ���
									else if (strcmp(cHeadText, "PARTS") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nKeyID
										);
									}
									// �p�[�e�B�N���ԍ����ǂݍ���
									else if (strcmp(cHeadText, "PARTICLE") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nParticleType
										);
									}
								}
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].v_MotionEffect.push_back(motion_effect);
							}

							// ������ѕ����ǂݍ���
							else if (strcmp(cHeadText, "BLUST") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust);
							}

							// �t���[�����ǂݍ���
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame);
								// ���[�V�����S�̂̃t���[�������Z
								pmodelAll->pMotion[nCntMotion]->nAllFrame +=
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								// �����蔻��̉񐔂��Ȃ��ꍇ�֐��𔲂���
								if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision > 0)
								{
									// �U������p�x
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime =
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame /
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision;
								}
								else
								{
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime = pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								}

							}
							// �L�[���ǂݍ���
							else if (strcmp(cHeadText, "KEY") == 0)
							{
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									// ������
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// �ʒu���ǂݍ���
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

										// �p�[�c�̈ʒu����������
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos += pmodelAll->pModel_offset[nCntKey].pos;
									}

									// ��]���ǂݍ���
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
										// �p�[�c�̈ʒu����������
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot += pmodelAll->pModel_offset[nCntKey].rot;
									}
								}
								// �J�E���g�L�[
								nCntKey++;
							}
						}
						// �J�E���g���f��
						nCntKeySet++;
					}
				}
				// �J�E���g�v���C���[
				nCntMotion++;
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "���f���f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾃e�L�X�g�f�[�^�̔j��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel_info::TextUnload(
	MODEL_ALL * pmodelAll	// ���f���E���[�V�����̏��
)
{
	// ���f�����̊J��
	if (pmodelAll != NULL)
	{
		// �I�t�Z�b�g���̊J��
		if (pmodelAll->pModel_offset != NULL)
		{
			delete[] pmodelAll->pModel_offset;
			pmodelAll->pModel_offset = NULL;
		}
		// �O�Ղ̊�{���̊J��
		if (pmodelAll->v_MeshObitLoad.empty() == true)
		{
			std::vector<MESHOBIT_BASIC>().swap(pmodelAll->v_MeshObitLoad);
		}
		for (int nCntMotion = 0; nCntMotion < (signed)pmodelAll->pMotion.size(); nCntMotion++)
		{
			for (int nCntKeyInfo = 0; nCntKeyInfo < pmodelAll->pMotion[nCntMotion]->nNumKey; nCntKeyInfo++)
			{
				// �J�������̊J��
				if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].pMotionCamera != NULL)
				{
					delete pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].pMotionCamera;
					pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].pMotionCamera = NULL;
				}
				// �G�t�F�N�g���̊J��
				if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].v_MotionEffect.empty() == false)
				{
					std::vector<MOTION_EFFECT>().swap(pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].v_MotionEffect);
				}
				// �L�[�̔j��
				delete[] pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].Key;
				pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].Key = NULL;
			}

			//�L�[���̔j��
			delete[] pmodelAll->pMotion[nCntMotion]->KeyInfo;
			pmodelAll->pMotion[nCntMotion]->KeyInfo = NULL;
			// �U���̓����蔻����̊J��
			if (pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.empty() == false)
			{
				// �O�Ղ̏ڍ׏��̊J��
				std::vector<MESHOBIT_DETAILS>().swap(pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail);
			}
			// �U���̓����蔻����̊J��
			if (pmodelAll->pMotion[nCntMotion]->v_Collision.empty() == false)
			{
				// �����蔻����̊J��
				std::vector<COLLISION_DETAILS>().swap(pmodelAll->pMotion[nCntMotion]->v_Collision);
			}
			// ���[�V�����̔j��
			delete pmodelAll->pMotion[nCntMotion];
			pmodelAll->pMotion[nCntMotion] = NULL;
		}
		// ���[�V�����̔j��
		pmodelAll->pMotion.clear();
		pmodelAll->pMotion.shrink_to_fit();

		// ���f�����S�̂̔j��
		delete pmodelAll;
		pmodelAll = NULL;
	}
}
