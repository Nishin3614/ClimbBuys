// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�p�[�e�B�N���̏���[3Dparticle.h]
// Author : Nishiyama Koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Dparticle.h"
#include "3Deffect.h"
#include "2Deffect.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PARTICLE_FILEPATH ("data/LOAD/ParticleInfo.txt")	// �t�@�C���p�X

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::PARTICLE_INFO *C3DParticle::m_ParticleOffset[C3DParticle::PARTICLE_ID_MAX] = {};

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::C3DParticle() : CScene()
{
	m_offsetID = C3DParticle::PARTICLE_ID_NONE;	// �I�t�Z�b�gID
	m_b2D = false;								// 2D�`����
	m_nFrameCount = 0;							// �t���[���̃J�E���g
	m_Origin = D3DVECTOR3_ZERO;					// ���_
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::~C3DParticle()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Init(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Uninit(void)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update(void)
{
	// �t���[���J�E���g�A�b�v
	m_nFrameCount++;
	// �t���[���J�E���g���p�[�e�B�N�����̃t���[���𒴂�����
	if (m_nFrameCount > m_ParticleOffset[m_offsetID]->nFrame)
	{
		// �t���[���J�E���g������
		m_nFrameCount = 0;
		// �J������
		Release();
	}
	// ����ȊO
	else
	{
		// �ϐ��錾
		D3DXVECTOR3	move		= D3DVECTOR3_ZERO;	// �ړ���
		D3DXVECTOR3	pos			= D3DVECTOR3_ZERO;	// �ʒu
		D3DXVECTOR3 rot			= D3DVECTOR3_ZERO;	// ��]
		D3DXVECTOR2	size		= D3DVECTOR2_ZERO;	// �T�C�Y
		D3DXVECTOR2 sizeValue	= D3DVECTOR2_ZERO;	// �T�C�Y�ω��l
		D3DXCOLOR	col			= D3DXCOLOR_INI;	// �F
		D3DXVECTOR3	posRand		= D3DVECTOR3_ZERO;	// �ʒu�����_��
		D3DXVECTOR3 vector = D3DVECTOR3_ZERO;		// �x�N�g��
		int			nLife		= 0;				// ���C�t
		int			nColRand	= 0;				// �F�̃����_��
		int			nRand = 0;						// �����_���l
		float		fSpeed		= 0;				// ���x
		float		fAngle[3];						// �p�x
		float		fAlphaDerease = 0.0f;			// ���ω��l
		// ������
		fAngle[0] = 0;	// �p�x0
		fAngle[1] = 0;	// �p�x1
		fAngle[2] = 0;	// �p�x2

		// �G�t�F�N�g�̌����[�v
		for (int nCntEffect = 0; nCntEffect < m_ParticleOffset[m_offsetID]->nNumber; nCntEffect++)
		{
			/* ���C�t�ݒ� */
			nLife = m_ParticleOffset[m_offsetID]->nLife;
			// �����_���̃��C�t��ݒ�
			if (m_ParticleOffset[m_offsetID]->nLifeRand.nMax > 0)
			{
				nLife +=
					(rand() % m_ParticleOffset[m_offsetID]->nLifeRand.nMax +
						m_ParticleOffset[m_offsetID]->nLifeRand.nMin);
			}
			/* �J���[�ݒ� */
			col = m_ParticleOffset[m_offsetID]->Col;
			if (m_ParticleOffset[m_offsetID]->bRedRand)
			{
				// �����_���l�擾
				nRand = (int)(col.r * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.r = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bGreenRand)
			{
				// �����_���l�擾
				nRand = (int)(col.g * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.g = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bBlueRand)
			{
				// �����_���l�擾
				nRand = (int)(col.b * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.b = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bAlphaRand)
			{
				// �����_���l�擾
				nRand = (int)(col.a * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.a = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bAlphaDecrease)
			{
				// ���l�̌���
				fAlphaDerease = col.a / nLife;
			}

			/* ���a�ݒ� */
			// �ǂݍ��񂾃T�C�Y����
			size = m_ParticleOffset[m_offsetID]->Size;
			// �����_���Őݒ肵�������Z
			if (m_ParticleOffset[m_offsetID]->SizeXRand.nMax > 0)
			{
				size.x += (float)(rand() % m_ParticleOffset[m_offsetID]->SizeXRand.nMax + m_ParticleOffset[m_offsetID]->SizeXRand.nMin);
			}
			if (m_ParticleOffset[m_offsetID]->SizeYRand.nMax > 0)
			{
				size.y += (float)(rand() % m_ParticleOffset[m_offsetID]->SizeYRand.nMax + m_ParticleOffset[m_offsetID]->SizeXRand.nMin);
			}
			// �T�C�Y������Ԃ�true�Ȃ�
			if (m_ParticleOffset[m_offsetID]->bSizeDecrease)
			{
				// �T�C�Y�l�̌���
				sizeValue = -size / (float)nLife;
			}
			// ����ȊO
			else
			{
				// �T�C�Y�̕ω��l���
				sizeValue.x = sizeValue.y =
					m_ParticleOffset[m_offsetID]->fSizeChange;
			}
			/* ���x�̐ݒ� */
			fSpeed = m_ParticleOffset[m_offsetID]->fSpeed;
			// �����_���̃X�s�[�h�ݒ�
			if (m_ParticleOffset[m_offsetID]->nSpeedRand.nMax > 0)
			{
				fSpeed +=
					(float)(rand() % m_ParticleOffset[m_offsetID]->nSpeedRand.nMax +
						m_ParticleOffset[m_offsetID]->nSpeedRand.nMin);
			}

			/* �p�x�̐ݒ� */
			fAngle[0] = m_ParticleOffset[m_offsetID]->Rot.x;
			fAngle[1] = m_ParticleOffset[m_offsetID]->Rot.y;
			fAngle[2] = m_ParticleOffset[m_offsetID]->Rot.z;
			// �����_���̊p�x�ݒ�
			if (m_ParticleOffset[m_offsetID]->nAngleRand.nMax > 0)
			{
				fAngle[0] +=
					(rand() % m_ParticleOffset[m_offsetID]->nAngleRand.nMax +
						m_ParticleOffset[m_offsetID]->nAngleRand.nMin)*0.01f;
				fAngle[1] +=
					(rand() % m_ParticleOffset[m_offsetID]->nAngleRand.nMax +
						m_ParticleOffset[m_offsetID]->nAngleRand.nMin)*0.01f;
				fAngle[2] +=
					(rand() % m_ParticleOffset[m_offsetID]->nAngleRand.nMax +
						m_ParticleOffset[m_offsetID]->nAngleRand.nMin)*0.01f;
			}

			/* �ʒu�ݒ� */
			// �����_���̊p�x�ݒ�
			if (m_ParticleOffset[m_offsetID]->PosXRand.nMax > 0)
			{
				posRand.x = (float)(rand() % m_ParticleOffset[m_offsetID]->PosXRand.nMax + m_ParticleOffset[m_offsetID]->PosXRand.nMin);
			}
			if (m_ParticleOffset[m_offsetID]->PosYRand.nMax > 0)
			{
				posRand.y = (float)(rand() % m_ParticleOffset[m_offsetID]->PosYRand.nMax + m_ParticleOffset[m_offsetID]->PosYRand.nMin);
			}
			if (m_ParticleOffset[m_offsetID]->PosZRand.nMax > 0)
			{
				posRand.z = (float)(rand() % m_ParticleOffset[m_offsetID]->PosZRand.nMax + m_ParticleOffset[m_offsetID]->PosZRand.nMin);
			}

			// �ʒu
			pos =
			{
				posRand.x + m_ParticleOffset[m_offsetID]->Pos.x,
				posRand.y + m_ParticleOffset[m_offsetID]->Pos.y,
				posRand.z + m_ParticleOffset[m_offsetID]->Pos.z
			};

			// �p�[�e�B�N���^�C�v
			if (m_ParticleOffset[m_offsetID]->nParticleType == TYPE_ANGLE_TO_VECTOR)
			{
				// �ʒu�ɂ���Ĉړ���
				/* �ړ��ʐݒ� */
				move =
				{
					sinf(fAngle[1])*sinf(fAngle[2])*fSpeed,
					cosf(fAngle[0])*cosf(fAngle[2])*fSpeed,
					cosf(fAngle[0])*sinf(fAngle[1])*fSpeed
				};
			}
			else if (m_ParticleOffset[m_offsetID]->nParticleType == TYPE_ANGLE_AND_VECTOR)
			{
				/* �����ݒ� */
				vector = m_ParticleOffset[m_offsetID]->Vector;
				// �����_���̕����ݒ�
				if (m_ParticleOffset[m_offsetID]->VectorXRand.nMax > 0)
				{
					vector.x += (float)(rand() % m_ParticleOffset[m_offsetID]->VectorXRand.nMax + m_ParticleOffset[m_offsetID]->VectorXRand.nMin);
				}
				if (m_ParticleOffset[m_offsetID]->VectorYRand.nMax > 0)
				{
					vector.y += (float)(rand() % m_ParticleOffset[m_offsetID]->VectorYRand.nMax + m_ParticleOffset[m_offsetID]->VectorYRand.nMin);
				}
				if (m_ParticleOffset[m_offsetID]->VectorZRand.nMax > 0)
				{
					vector.z += (float)(rand() % m_ParticleOffset[m_offsetID]->VectorZRand.nMax + m_ParticleOffset[m_offsetID]->VectorZRand.nMin);
				}
				D3DXVec3Normalize(&vector, &vector);
				/* �ړ��ʐݒ� */
				move = vector * fSpeed;
			}
			// 2D�`���ԂȂ�
			if (m_b2D)
			{
				// 2D�G�t�F�N�g����
				C2DEffect::Set2DEffect(
					(C2DEffect::EFFECT_TYPE)m_ParticleOffset[m_offsetID]->nEffectType,
					m_ParticleOffset[m_offsetID]->nEffectTexType,
					pos + m_Origin,
					D3DXVECTOR3(
						fAngle[0],
						fAngle[1],
						fAngle[2]
					),
					move,
					col,
					size,
					nLife,
					m_ParticleOffset[m_offsetID]->Anim,
					m_ParticleOffset[m_offsetID]->Blend,
					sizeValue,
					fAlphaDerease
				);

			}
			// ����ȊO
			else
			{
				// 3D�G�t�F�N�g����
				C3DEffect::Set3DEffect(
					(C3DEffect::EFFECT_TYPE)m_ParticleOffset[m_offsetID]->nEffectType,
					m_ParticleOffset[m_offsetID]->nEffectTexType,
					pos + m_Origin,
					D3DXVECTOR3(
						fAngle[0],
						fAngle[1],
						fAngle[2]
					),
					move,
					col,
					size,
					nLife,
					m_ParticleOffset[m_offsetID]->Anim,
					m_ParticleOffset[m_offsetID]->Blend,
					sizeValue,
					fAlphaDerease
				);
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Draw(void)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����
//	ParticleId	: �p�[�e�B�N���ԍ�
//	origin		: �����ʒu
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle * C3DParticle::Create(
	PARTICLE_ID const	&ParticleId,	// �p�[�e�B�N���ԍ�
	D3DXVECTOR3 const	&origin,		// �����ʒu
	bool		const	&b2D			// 2D�`����
)
{
	// �ϐ��錾
	C3DParticle *p3DParticle = new C3DParticle;
	// �V�[���Ǘ��ݒ�
	p3DParticle->ManageSetting(CScene::LAYER_3DPARTICLE);
	// �ݒ�
	p3DParticle->SetParticle(ParticleId);
	p3DParticle->SetOrigin(origin);
	p3DParticle->m_b2D = b2D;
	return p3DParticle;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DParticle::Load(void)
{
	// �ϐ��錾
	FILE *pFile = NULL;				// �t�@�C���̃|�C���^
	char cRead[128] = {};				// �ǂݍ��ݗp
	char cComp[128] = {};				// ��r�p
	char cEmpty[128] = {};				// �v��Ȃ����̗p
	char cName[64] = {};					// �p�[�e�B�N����
	int nCntError = 0;				// �G���[�p
	int nCntOffset = 0;				// �I�t�Z�b�g�̃J�E���g
	int nId = 0;					// �p�[�e�B�N���ԍ�
									// �ꎞ�ۑ��p
									// �t�@�C�����J����Ă��Ȃ�������
	if ((pFile = fopen(PARTICLE_FILEPATH, "r")) == NULL)
	{
		CCalculation::Messanger("�p�[�e�B�N��cpp��LOAD�֐�->�t�@�C���������݂��܂���B");
		return E_FAIL;
	}
	// �X�N���v�g���ǂݍ��܂��܂Ń��[�v
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			CCalculation::Messanger("�p�[�e�B�N��cpp��LOAD�֐�->�e�L�X�g���ɁuSCRIPT�v�����݂��܂���B");
			return E_FAIL;
		}
	}
	// �G���h�X�N���v�g���ǂݍ��܂��܂Ń��[�v
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPT�܂Ń��[�v
	 // 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			CCalculation::Messanger("�p�[�e�B�N��cpp��LOAD�֐�->�e�L�X�g���ɁuEND_SCRIPT�v�����݂��܂���B");
			return E_FAIL;
		}
		// # ���ǂݍ��܂ꂽ��
		else if (strcmp(cComp, "#") == 0)
		{
			// �ʒux�̍ő�l�ƍŏ��l����
			sscanf(cRead, "%s [%d] %s", &cEmpty,
				&nId,
				&cName
			);
		}
		else if (strcmp(cComp, "OFFSET") == 0)
		{
			// �p�[�e�B�N���I�t�Z�b�g�̏��m��
			m_ParticleOffset[nCntOffset] = new PARTICLE_INFO;
			// �p�[�e�B�N�����̏�����
			cName[0] = '\0';
			while (strcmp(cComp, "END_OFFSET") != 0)
			{
				// 1�s�ǂݍ���
				fgets(cRead, sizeof(cRead), pFile);
				// �ǂݍ���Ǖ�������
				sscanf(cRead, "%s", &cComp);
				// �Z�b�g�ʒu��������
				// ->����
				if (strcmp(cComp, "SET_POSRAND") == 0)
				{
					// �G���h�Z�b�g�ʒu������܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_POSRAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// �ʒux���ǂݍ��܂ꂽ��
						if (strcmp(cComp, "POSX") == 0)
						{
							// �ʒux�̍ő�l�ƍŏ��l����
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->PosXRand.nMax,
								&m_ParticleOffset[nCntOffset]->PosXRand.nMin
							);
						}
						// �ʒuy���ǂݍ��܂ꂽ��
						else if (strcmp(cComp, "POSY") == 0)
						{
							// �ʒuy�̍ő�l�ƍŏ��l����
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->PosYRand.nMax,
								&m_ParticleOffset[nCntOffset]->PosYRand.nMin
							);
						}
						// �ʒuz���ǂݍ��܂ꂽ��
						else if (strcmp(cComp, "POSZ") == 0)
						{
							// �ʒuz�̍ő�l�ƍŏ��l����
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->PosZRand.nMax,
								&m_ParticleOffset[nCntOffset]->PosZRand.nMin
							);
						}
					}
				}
				// �Z�b�g�F��������
				// ->����
				else if (strcmp(cComp, "SET_COLRAND") == 0)
				{
					// �G���h�Z�b�g�T�C�Y������܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_COLRAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// ��
						if (strcmp(cComp, "RED") == 0)
						{
							m_ParticleOffset[nCntOffset]->bRedRand = true;
						}
						// ��
						else if (strcmp(cComp, "GREEN") == 0)
						{
							m_ParticleOffset[nCntOffset]->bGreenRand = true;
						}
						// ��
						else if (strcmp(cComp, "BLUE") == 0)
						{
							m_ParticleOffset[nCntOffset]->bBlueRand = true;
						}
						// �����x
						else if (strcmp(cComp, "ALPHA") == 0)
						{
							m_ParticleOffset[nCntOffset]->bAlphaRand = true;
						}
					}
				}
				// �Z�b�g�T�C�Y��������
				// ->����
				else if (strcmp(cComp, "SET_SIZERAND") == 0)
				{
					// �G���h�Z�b�g�T�C�Y������܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_SIZERAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// �T�C�Yx
						if (strcmp(cComp, "SIZEX") == 0)
						{
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->SizeXRand.nMax,
								&m_ParticleOffset[nCntOffset]->SizeXRand.nMin
							);
						}
						// �T�C�Yy
						else if (strcmp(cComp, "SIZEY") == 0)
						{
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->SizeYRand.nMax,
								&m_ParticleOffset[nCntOffset]->SizeYRand.nMin
							);
						}
					}
				}
				// �Z�b�g�p�x��������
				// ->����
				else if (strcmp(cComp, "SET_ANGLERAND") == 0)
				{
					// �G���h�Z�b�g�p�x������܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_ANGLERAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// �ő�p�x
						if (strcmp(cComp, "MAX") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nAngleRand.nMax
							);
						}
						// �ŏ��p�x
						else if (strcmp(cComp, "MIN") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nAngleRand.nMin
							);
						}
					}
				}
				// �Z�b�g������������
				// ->����
				if (strcmp(cComp, "SET_VECTORRAND") == 0)
				{
					// �G���h�Z�b�g����������܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_VECTORRAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// ����x���ǂݍ��܂ꂽ��
						if (strcmp(cComp, "VECTORX") == 0)
						{
							// ����x�̍ő�l�ƍŏ��l����
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->VectorXRand.nMax,
								&m_ParticleOffset[nCntOffset]->VectorXRand.nMin
							);
						}
						// ����y���ǂݍ��܂ꂽ��
						else if (strcmp(cComp, "VECTORY") == 0)
						{
							// ����y�̍ő�l�ƍŏ��l����
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->VectorYRand.nMax,
								&m_ParticleOffset[nCntOffset]->VectorYRand.nMin
							);
						}
						// ����z���ǂݍ��܂ꂽ��
						else if (strcmp(cComp, "VECTORZ") == 0)
						{
							// ����z�̍ő�l�ƍŏ��l����
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->VectorZRand.nMax,
								&m_ParticleOffset[nCntOffset]->VectorZRand.nMin
							);
						}
					}
				}
				// �Z�b�g�X�s�[�h��������
				// ->����
				else if (strcmp(cComp, "SET_SPEEDRAND") == 0)
				{
					// �G���h�Z�b�g�X�s�[�h���ǂݍ��܂��܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_SPEEDRAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// �ő�X�s�[�h���ǂݍ��܂��܂�
						if (strcmp(cComp, "MAX") == 0)
						{
							// �X�s�[�h�����_���̍ő�l����
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nSpeedRand.nMax
							);
						}
						// �ŏ��X�s�[�h���ǂݍ��܂��܂�
						else if (strcmp(cComp, "MIN") == 0)
						{
							// �X�s�[�h�����_���̍ŏ��l����
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nSpeedRand.nMin
							);
						}
					}
				}
				// �Z�b�g���C�t�����_����������
				// ->����
				else if (strcmp(cComp, "SET_LIFERAND") == 0)
				{
					// �G���h�Z�b�g���C�t�����_��������܂�
					// ->���[�v
					while (strcmp(cComp, "ENDSET_LIFERAND") != 0)
					{
						// ������̏�����
						cComp[0] = '\0';
						// 1�s�ǂݍ���
						fgets(cRead, sizeof(cRead), pFile);
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s", &cComp);
						// �ő僉�C�t
						if (strcmp(cComp, "MAX") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nLifeRand.nMax
							);
						}
						// �ŏ����C�t
						else if (strcmp(cComp, "MIN") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nLifeRand.nMin
							);
						}
					}
				}
				// �ʒu���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "POS") == 0)
				{
					// �ʒu���̑��
					sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Pos.x,
						&m_ParticleOffset[nCntOffset]->Pos.y,
						&m_ParticleOffset[nCntOffset]->Pos.z);
					// ������̏�����
					cComp[0] = '\0';
				}
				// ��]���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "ROT") == 0)
				{
					// ��]���̑��
					sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Rot.x,
						&m_ParticleOffset[nCntOffset]->Rot.y,
						&m_ParticleOffset[nCntOffset]->Rot.z);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �F���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "COL") == 0)
				{
					// �F���̑��
					sscanf(cRead, "%s %s %f %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Col.r,
						&m_ParticleOffset[nCntOffset]->Col.g,
						&m_ParticleOffset[nCntOffset]->Col.b,
						&m_ParticleOffset[nCntOffset]->Col.a);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �F�̌�����Ԃ��ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "COL_DESCREASE") == 0)
				{
					// �T�C�Y�̌�����Ԃ�true��
					m_ParticleOffset[nCntOffset]->bAlphaDecrease = true;
					// ������̏�����
					cComp[0] = '\0';
				}
				// �T�C�Y���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "SIZE") == 0)
				{
					// �T�C�Y�̑��
					sscanf(cRead, "%s %s %f %f",
						&cEmpty,
						&cEmpty,
						&m_ParticleOffset[nCntOffset]->Size.x,
						&m_ParticleOffset[nCntOffset]->Size.y
					);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �T�C�Y�̌������ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "SIZE_DESCREASE") == 0)
				{
					// �T�C�Y�̌�����Ԃ�true��
					m_ParticleOffset[nCntOffset]->bSizeDecrease = true;
					// ������̏�����
					cComp[0] = '\0';
				}
				// �T�C�Y�̕ω��l���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "SIZE_CHANGE") == 0)
				{
					// �T�C�Y�̕ω��l�̑��
					sscanf(cRead, "%s %s %f",
						&cEmpty,
						&cEmpty,
						&m_ParticleOffset[nCntOffset]->fSizeChange
					);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �������ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "VECTOR") == 0)
				{
					// �������̑��
					sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Vector.x,
						&m_ParticleOffset[nCntOffset]->Vector.y,
						&m_ParticleOffset[nCntOffset]->Vector.z);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �X�s�[�h���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "SPEED") == 0)
				{
					// �X�s�[�h�̑��
					sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->fSpeed);
					// ������̏�����
					cComp[0] = '\0';
				}
				// ���C�t���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "LIFE") == 0)
				{
					// ���C�t�̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nLife);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �t���[�����ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "FRAME") == 0)
				{
					// �t���[���̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nFrame);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �ԍ����ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "NUMBER") == 0)
				{
					// �ԍ��̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nNumber);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �e�N�X�`���[�^�C�v���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "TEXTYPE") == 0)
				{
					// �e�N�X�`���[�^�C�v�̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nEffectTexType);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �A�j���[�V�������ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "ANIMATION") == 0)
				{
					/*
					// �A�j���[�V��������NULL�`�F�b�N
					if (!m_ParticleOffset[nCntOffset]->Anim)
					{
					m_ParticleOffset[nCntOffset]->Anim = new ANIMATION_LOAD;
					}
					*/
					// �A�j���[�V���������
					sscanf(cRead, "%s %s %d %d %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Anim.nMaxCntAnim,
						&m_ParticleOffset[nCntOffset]->Anim.nMaxHorizonAnim,
						&m_ParticleOffset[nCntOffset]->Anim.nMaxVirticalAnim
					);
					// �����A�j���[�V�����Ɛ����A�j���[�V�����̐ݒ�
					m_ParticleOffset[nCntOffset]->Anim.fHorizonSize = 1.0f / m_ParticleOffset[nCntOffset]->Anim.nMaxHorizonAnim;
					m_ParticleOffset[nCntOffset]->Anim.fVirticalSize = 1.0f / m_ParticleOffset[nCntOffset]->Anim.nMaxVirticalAnim;
					m_ParticleOffset[nCntOffset]->Anim.bUse = true;
					// ������̏�����
					cComp[0] = '\0';
				}
				// �G�t�F�N�g�^�C�v���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "EFFECTTYPE") == 0)
				{
					// �G�t�F�N�g�^�C�v�̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nEffectType);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �p�[�e�B�N���^�C�v���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "PARTICLETYPE") == 0)
				{
					// �p�[�e�B�N���^�C�v�̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nParticleType);
					// ������̏�����
					cComp[0] = '\0';
				}
				// �u�����h�^�C�v���ǂݍ��܂ꂽ��
				else if (strcmp(cComp, "BLEND") == 0)
				{
					// �p�[�e�B�N���^�C�v�̑��
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Blend);
					// ������̏�����
					cComp[0] = '\0';
				}
			}
			// �I�t�Z�b�g�J�E���g��i�߂�
			nCntOffset++;
		}
	}
	fclose(pFile);
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �S���\�[�X���̊J��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Unload(void)
{
	// �p�[�e�B�N�����̊J��
	for (int nCntParticle = 0; nCntParticle < C3DParticle::PARTICLE_ID_MAX; nCntParticle++)
	{
		if (m_ParticleOffset[nCntParticle] == NULL)continue;
		delete m_ParticleOffset[nCntParticle];
		m_ParticleOffset[nCntParticle] = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �p�[�e�B�N���ԍ��ݒ�
//	ParticleId	: �p�[�e�B�N���ԍ�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::SetParticle(
	PARTICLE_ID const	&ParticleId	// �p�[�e�B�N���ԍ�
)
{
	m_offsetID = ParticleId;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����ʒu�ݒ�
//	origin	: �����ʒu
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::SetOrigin(
	D3DXVECTOR3 const	&origin		// �����ʒu
)
{
	m_Origin = origin;
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Debug(void)
{
}
#endif // _DEBUG