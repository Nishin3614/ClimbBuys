// ----------------------------------------
//
// �������̐���[floor.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "floor.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"
#include "character.h"
#include "light.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define FLOOR_ROTMOVE (0.01f)						// �p�x�̈ړ���
#define FLOOR_SIZE (100.0f)							// �T�C�Y
#define FLOOR_ANGLE (1.0f)							// �p�Ȃ̊p�x
#define TEXTURE_MASK ("data/TEXTURE/mask.jpg")		// �}�X�N�e�N�X�`���A�h���X

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
//LPD3DXEFFECT CFloor::m_pEffect = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CFloor::CFloor() : CScene()
{
	/* �ϐ��̏����� */
	// ��]��
	m_pTexture = NULL;
	//m_pMaskTex = NULL;
	m_pVtxBuff = NULL;
	m_pIndex = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumberVertex = 4;
	m_nNumIndex = 4;
	m_nNumPolygon = 2;
	m_nBlock_Width = 1;
	m_nBlock_Depth = 1;
	m_nTexType = 0;
	m_bCalculation = false;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CFloor::~CFloor()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CFloor::Init(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;						// ���_���ւ̃|�C���g
	WORD * pIdx;							// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCountDirect;						// �c�̃J�E���g
	int nCountWidth;						// ���̃J�E���g
	int nCntBlock = 0;						// �u���b�N�J�E���g
	float fYAngle;							// y�̊p�x
	float fYRadian;							// y�̃��W�A���l
	// ���擾
	LPDIRECT3DDEVICE9 pDevice =				// �f�o�C�X
		CManager::GetRenderer()->GetDevice();

	/*
	if (m_pEffect == NULL)
	{
		//�V�F�[�_�[��ǂݍ���
		if (FAILED(D3DXCreateEffectFromFile(pDevice, "data/LOAD/MAPPING/MaskShader.fx", NULL, NULL,
			0, NULL, &m_pEffect, NULL)))
		{
			MessageBox(NULL, "�V�F�[�_�[�t�@�C���ǂݍ��ݎ��s", "", MB_OK);
		}
	}

	// �e�N�X�`���[����
	D3DXCreateTextureFromFile(
		CManager::GetRenderer()->GetDevice(),
		TEXTURE_MASK,
		&m_pMaskTex);
*/
	// �u���b�N�`��̌��_�̏����ݒ�
	m_OriginBlock = D3DXVECTOR3(
		m_size.x * -0.5f * m_nBlock_Width,
		0.0f,
		m_size.z * 0.5f * m_nBlock_Depth);
	// �����_���E�C���f�b�N�X�E�|���S���̏����ݒ�(�v�Z)
	m_nNumberVertex =
		(m_nBlock_Depth + 1) * (m_nBlock_Width + 1);
	m_nNumIndex =
		(m_nBlock_Width + 1) * 2 * m_nBlock_Depth +
		2 * (m_nBlock_Depth - 1);
	m_nNumPolygon =
		m_nBlock_Depth * m_nBlock_Width * 2 +
		4 * (m_nBlock_Depth - 1);

	// �p�x�̌v�Z
	fYAngle = FLOOR_ANGLE;
	m_size.y = 100;
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//���_�ݒ� //
	//�s���[�v
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ���W�A���l(Y)
			fYRadian = fYAngle * nCountDirect;
			fYRadian = CCalculation::Rot_One_Limit(fYRadian);

			// ���_���W�̐ݒ�
			pVtx[0].pos =
				D3DXVECTOR3(
					m_OriginBlock.x +
					(m_size.x * nCountWidth),
					m_OriginBlock.y,
					m_OriginBlock.z -
					(m_size.z * nCountDirect));

			// �J���[�̐ݒ�
			pVtx[0].col = m_col;

			// �e�N�X�`���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

			// �|�C���g���킹
			pVtx++;
		}
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();

	CalculationNormalize();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pIndex->Lock(0, 0, (void **)&pIdx, 0);


	// �c�u���b�N��
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth; nCountDirect++)
	{
		// �Q��ڂ̃��[�v�ȍ~
		if (nCountDirect >= 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width + 1;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}

		// ���u���b�N�̒��_��
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// �`�揇�Ԃ̃C���f�b�N�X
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth + m_nBlock_Width + 1;
			pIdx[1] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth;

			// �C���f�b�N�X�̃|�C���g���킹
			pIdx += 2;
		}

		// �k�ރ|���S�������K�v������ꍇ
		if (nCountDirect < m_nBlock_Depth - 1)
		{
			// �k�ރ|���S�����̒��_�ǉ�
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width;
			// �C���f�b�N�X�̃|�C���g���킹
			pIdx++;
		}
	}

	// �A�����b�N
	m_pIndex->Unlock();
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CFloor::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	/*
	// �C���f�b�N�X���̊J��
	if (m_pIndex != NULL)
	{
		m_pIndex->Release();
		m_pIndex = NULL;
	}
	// �V�F�[�_�[�̊J��
	if (m_pEffect != NULL)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}
	*/
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CFloor::Update(void)
{
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CFloor::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =		// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();

	// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f //
	// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,	// �����̓��ꕨ
		m_rot.y, m_rot.x, m_rot.z);			// ���ꂼ��̉�]��
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,					// 2
		&mtxRot);							// 3
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,// �����̓��ꕨ
		m_pos.x, m_pos.y, m_pos.z);	// ���ꂼ��̈ړ���
	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXMATRIX mtxProj, mtxView;

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	/*
	if (m_bCalculation)
	{
		// �V�F�[�_�[����
		if (m_pEffect != NULL)
		{
			m_pEffect->SetTechnique("tecLambert");
			D3DXMATRIX mAll = m_mtxWorld * mtxView * mtxProj;
			m_pEffect->SetMatrix("WVP", &mAll);
			// �������̐ݒ� (�������ƂɌ��߂�)
			m_pEffect->SetFloat("fDivision", 1.0f / 50);
			m_pEffect->Begin(NULL, 0);
			m_pEffect->BeginPass(0);
		}
	}
	*/

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
	pDevice->SetIndices(
		m_pIndex);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(
		0,
		CTexture_manager::GetTexture(m_nTexType));

	/*
	if (m_pEffect != NULL)
	{
		//���[���h�s��̋t�s��̓]�u�s���n��
		D3DXMATRIX mWIT;
		m_pEffect->SetMatrix("WIT", D3DXMatrixTranspose(&mWIT, D3DXMatrixInverse(&mWIT, NULL, &m_mtxWorld)));

		//���C�g�̕����x�N�g����n��
		CLight *pLight = CManager::GetRenderer()->GetLight();
		D3DLIGHT9 Light = pLight->GetLight(CLight::TYPE_MAIN);
		D3DXVECTOR4 direction;
		direction = D3DXVECTOR4(Light.Direction.x, Light.Direction.y, Light.Direction.z, 0.0f);
		m_pEffect->SetVector("LightDir", &direction);

		//���ˌ��i���C�g�j�̋��x��n���@�ڈ�t���邢���F���ɂ��Ă݂�
		m_pEffect->SetVector("LightIntensity", &D3DXVECTOR4(1, 1, 1, 1));

		//���b�V���̃f�B�t���[�Y�F��n��
		m_pEffect->SetValue("Diffuse", (LPVOID)&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16);

		//���b�V���̃f�B�t���[�Y�F��n��
		m_pEffect->SetValue("Ambient", (LPVOID)&D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), 16);

		// �e�N�X�`������n��
		m_pEffect->SetTexture("texDecal", CTexture_manager::GetTexture(m_nTexType));
		m_pEffect->SetTexture("texMask", m_pMaskTex);

		m_pEffect->SetFloat("uv_left", 0.0f);
		m_pEffect->SetFloat("uv_top", 0.00f);
		m_pEffect->SetFloat("uv_width", 1.0f);
		m_pEffect->SetFloat("uv_height", 1.0f);
	}
	*/

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumberVertex,
		0,
		m_nNumPolygon);

	/*
	if (m_bCalculation)
	{
		// �V�F�[�_�[�I��
		if (m_pEffect != NULL)
		{
			m_pEffect->EndPass();
			m_pEffect->End();
		}
	}
	*/
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CFloor::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// ���_�ړ�
// ----------------------------------------
void CFloor::vertexMove(FILE *pFile)
{
	VERTEX_3D *pVtx;
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < m_nBlock_Depth + 1; nDepth++)
	{
		for (int nWide = 0; nWide < m_nBlock_Width + 1; nWide++)
		{
			if (pFile != NULL)
			{

				fgets(cReadText, sizeof(cReadText), pFile);							// �f�[�^����1�s�擾
				sscanf(cReadText, "%s", &cHeadText);								// �s��ǂݍ���

				std::string Data = cReadText;
				std::vector<std::string> vsvec_Contens;		// �e�L�X�g�f�[�^�i�[�p

				vsvec_Contens = CCalculation::split(Data, ',');

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3((float)atof(vsvec_Contens[0].c_str()), (float)atof(vsvec_Contens[1].c_str()), (float)atof(vsvec_Contens[2].c_str()));			// �f�[�^�𓖂Ă͂߂�

				D3DXCOLOR col = D3DXCOLOR((float)atof(vsvec_Contens[3].c_str()), (float)atof(vsvec_Contens[4].c_str()), (float)atof(vsvec_Contens[5].c_str()), (float)atof(vsvec_Contens[6].c_str()));

				//���_�J���[
				pVtx[0].col = col;	// �f�[�^�𓖂Ă͂߂�
			}

			pVtx++;
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();

	// �@���̍Čv�Z
	CalculationNormalize();
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CFloor::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CFloor::UnLoad(void)
{
}

// ----------------------------------------
// ����
//	pos			: �ʒu
//	size		: �T�C�Y
//	col			: �F
//	rot			: ��]
//	nWidth		: ���u���b�N
//	nDepth		: �c�u���b�N
//	nTexType	: �e�N�X�`���[�^�C�v
// ----------------------------------------
CFloor * CFloor::Create(
	D3DXVECTOR3 const &pos,		// �ʒu
	D3DXVECTOR3 const & size,	// �T�C�Y
	D3DXCOLOR const & col,		// �F
	D3DXVECTOR3 const & rot,	// ��]
	int const & nWidth,			// ���u���b�N
	int const & nDepth,			// �c�u���b�N
	int const & nTexType		// �e�N�X�`���[�^�C�v
)
{
	// �ϐ��錾
	CFloor * pFloor;		// �V�[��2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pFloor = new CFloor();
	// �V�[���Ǘ��ݒ�
	pFloor->ManageSetting(CScene::LAYER_3DOBJECT);
	// �ʒu���
	pFloor->m_pos = pos;
	// �T�C�Y���
	pFloor->m_size = size;
	// �F���
	pFloor->m_col = col;
	// ��]���
	pFloor->m_rot = rot;
	// ���u���b�N
	pFloor->m_nBlock_Width = nWidth;
	// �c�u���b�N
	pFloor->m_nBlock_Depth = nDepth;
	// �e�N�X�`���[�^�C�v
	pFloor->m_nTexType = nTexType;
	// ����������
	pFloor->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pFloor;
}

// ----------------------------------------
// �ڂ��Ă���ʂ̍������擾
// ----------------------------------------
float CFloor::GetHeight(D3DXVECTOR3 &pos)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;					// ���_���ւ̃|�C���g
	D3DXVECTOR3 VecA, VecB,VecObject;	// �x�N�g��
	D3DXVECTOR3 Normal;					// �@��
	float fPos_Y = 0;					// ����
	int nCntBlock = 0;					// �u���b�N��
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// �u���b�N���Ƃ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// �E��̃|���S��
			if (CCalculation::CrossCollision(&pos, &pVtx[0].pos, &pVtx[1].pos, &pVtx[m_nBlock_Width + 2].pos))
			{
				// �x�N�g��
				VecA = pVtx[m_nBlock_Width + 2].pos - pVtx[1].pos;	// �x�N�g��B
				VecB = pVtx[0].pos - pVtx[1].pos;					// �x�N�g��A
				VecObject = pos - (m_pos + pVtx[1].pos);			// �I�u�W�F�N�g�܂ł̃x�N�g��
				// �����̑��
				fPos_Y = pVtx[1].pos.y;
				// �O��
				D3DXVec3Cross(&Normal, &VecA, &VecB);
				// ���K��
				D3DXVec3Normalize(&Normal, &Normal);
#ifdef _DEBUG
				//// �u���b�N�ԍ�
				//CDebugproc::Print("�u���b�N�ԍ�[%d]\n", nCntBlock);
				//// �|���S���ԍ�
				//CDebugproc::Print("�|���S���ԍ�[%d]\n", nCntBlock * 2 + 1);
				//// �@��
				//CDebugproc::Print("�@��[%.3f,%.3f,%.3f]\n", Normal.x, Normal.y, Normal.z);
#endif // _DEBUG
				// ���_�f�[�^�͈̔͂��A�����b�N
				m_pVtxBuff->Unlock();
				// ������Ԃ�
				return fPos_Y + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// �����̃|���S��
			else if (CCalculation::CrossCollision(&pos, &pVtx[m_nBlock_Width + 2].pos, &pVtx[m_nBlock_Width + 1].pos, &pVtx[0].pos))
			{
				// �x�N�g��
				VecA = pVtx[0].pos - pVtx[m_nBlock_Width + 1].pos;						// �x�N�g��A
				VecB = pVtx[m_nBlock_Width + 2].pos - pVtx[m_nBlock_Width + 1].pos;		// �x�N�g��B
				VecObject = pos - (m_pos + pVtx[m_nBlock_Width + 1].pos);				// �I�u�W�F�N�g�܂ł̃x�N�g��
				// �����̑��
				fPos_Y = pVtx[m_nBlock_Width + 1].pos.y;
				// �O��
				D3DXVec3Cross(&Normal, &VecA, &VecB);
				// ���K��
				D3DXVec3Normalize(&Normal, &Normal);
#ifdef _DEBUG
				//// �u���b�N�ԍ�
				//CDebugproc::Print("�u���b�N�ԍ�[%d]\n", nCntBlock);
				//// �|���S���ԍ�
				//CDebugproc::Print("�|���S���ԍ�[%d]\n", nCntBlock * 2);
				//// �@��
				//CDebugproc::Print("�@��[%.3f,%.3f,%.3f]\n", Normal.x,Normal.y,Normal.z);
#endif // _DEBUG
				// ���_�f�[�^�͈̔͂��A�����b�N
				m_pVtxBuff->Unlock();
				// ������Ԃ�
				return fPos_Y + ((-Normal.x * VecObject.x) + (-Normal.z * VecObject.z)) / Normal.y;
			}
			// ���_���̍X�V
			pVtx++;
		}
		// ���_���̍X�V
		pVtx++;

	}

	// ���_�f�[�^�͈̔͂��A�����b�N
	m_pVtxBuff->Unlock();
	// ������Ԃ�
	return 0.0f;
}

// ----------------------------------------
// �T�C�Y�ݒ�
// ----------------------------------------
void CFloor::SetSize(D3DXVECTOR3 const & size)
{
	// �ϐ��錾
	VERTEX_3D * pVtx;						// 3D���_���
	// �ݒ�
	m_size = size;							// �T�C�Y
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//���_�ݒ� //
	//�s���[�v
	for (int nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (int nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos =
				D3DXVECTOR3(
					m_OriginBlock.x +
					(m_size.x * nCountWidth),
					m_OriginBlock.y,
					m_OriginBlock.z -
					(m_size.z * nCountDirect));

			// �|�C���g���킹
			pVtx++;
		}
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// �F�ݒ�
// ----------------------------------------
void CFloor::SetCol(D3DXCOLOR const & col)
{
	// �ϐ��錾
	VERTEX_3D * pVtx;						// 3D���_���
											// �ݒ�
	m_col = col;							// �F
											// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//���_�ݒ� //
	//�s���[�v
	for (int nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// �񃋁[�v
		for (int nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// �J���[�̐ݒ�
			pVtx[0].col = m_col;

			// �|�C���g���킹
			pVtx++;
		}
	}
	// �A�����b�N
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// �ʂ̖@�������߂�
// ----------------------------------------
void CFloor::CalculationNormalize(void)
{
	// �ϐ��錾
	VERTEX_3D *pVtx;						// ���_���ւ̃|�C���g
	int nCntBlock = 0;						// �u���b�N�J�E���g
	D3DXVECTOR3 VecA, VecB;					// �x�N�g��
	D3DXVECTOR3 *pCross;					// �|���S���̊O��

	pCross =								// �������m��
		new D3DXVECTOR3[m_nBlock_Width * m_nBlock_Depth * 2];

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S�����Ƃ̖@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// �x�N�g��
			VecA = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 1].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2]);
			// �x�N�g��
			VecA = pVtx[nCntBlock + 1].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			// �O�όv�Z
			pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1] = CCalculation::Cross_product(VecA, VecB);
			// ���K��
			D3DXVec3Normalize(&pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1], &pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1]);
			// ����
			pVtx[nCntBlock].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// �E��
			pVtx[nCntBlock + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// ����
			pVtx[nCntBlock + m_nBlock_Width + 1].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2];
			// �E��
			pVtx[nCntBlock + m_nBlock_Width + 2].nor += pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + pCross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
		}
	}
	// ���_�@���̐ݒ�
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++)
		{
			// �ŏ�
			if (nCntDepth == 0 && nCntWidth == 0)
			{
				pVtx[0].nor /= 2;
			}
			// �Ō�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 2;
			}
			// 1�s�̗�̍Ō�
			else if (nCntDepth == 0 && nCntWidth == m_nBlock_Width)
			{
			}
			// �Ō�s�̗�̍ŏ�
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == 0)
			{
			}
			// �ŏ��̍s�܂��͍Ō�̍s
			else if (nCntDepth == 0 || nCntDepth == m_nBlock_Depth)
			{
				pVtx[0].nor /= 3;
			}
			// �ŏ��̗�܂��͍Ō�̗�
			else if (nCntWidth == 0 || nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 3;
			}
			// ����ȊO
			else
			{
				pVtx[0].nor /= 6;
			}
			pVtx++;
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
	delete[] pCross;
	pCross = NULL;
}