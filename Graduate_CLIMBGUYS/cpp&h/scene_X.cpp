// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// X�V�[�������̐���[scene_X.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_X.h"
#include "manager.h"
#include "debugproc.h"
#include "floor.h"
#include "shadow_mapping.h"
#include "collision.h"
#include "scene_three.h"
#include "3Dparticle.h"
#include "camera.h"
#include "spherecollision.h"
#include "rectcollision.h"
#include "columncollision.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TEXTURE_FILE "data/LOAD/MODEL/model_manager.csv"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<std::unique_ptr<CScene_X::MODEL_LOAD>>  CScene_X::m_pModelLoad;	// �v���C���[(��)���̓ǂݍ��ݗp

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C�j�V�����C�U�R���X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::CScene_X() : CScene::CScene()
{
	m_pos = D3DVECTOR3_ZERO;				// �ʒu���
	m_rot = D3DVECTOR3_ZERO;				// ��]���
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �T�C�Y���
	m_nModelId = 0;							// �v���C���[(��)�ԍ�
	m_fShadowAlpha = 1.0f;					// �e�̃��l
	m_fModelAlpha = 1.0f;					// �v���C���[�̃��l
	m_bShadowMap = false;					// �V���h�E�}�b�s���O�ɂ��邩���Ȃ���
	m_pParentMtx = NULL;					// �e�}�g���b�N�X
	m_pShadow = NULL;						// �V���h�E
	m_Collision = NULL;						// �����蔻��
	m_pModelCol = NULL;						// ���f���J���[���
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::~CScene_X()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Init(void)
{
	/*
	if (!m_bShadowMap)
	{
		// �������v�Z
		float fLength = CCalculation::DiffPointSquare(
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(m_pModelLoad[m_nModelId]->size.x, m_pModelLoad[m_nModelId]->size.z));
		// �e�𐶐�
		m_pShadow = std::move(CScene_THREE::Create_Uni(
			CScene_THREE::OFFSET_TYPE_VERTICAL_CENTER,
			m_pos,
			D3DXVECTOR3(fLength, 0.0f, fLength),
			1,
			D3DVECTOR3_ZERO,
			false,
			false,
			false,
			false,
			D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fShadowAlpha)
		));
	}
	*/
	// �s��̏���������
	D3DXMatrixIdentity(&m_mtxWorld);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Uninit(void)
{
	// �܂�e��NULL�ł͂Ȃ��Ȃ�
	// ->�J��
	if (m_pShadow != NULL)
	{
		m_pShadow->Release();
		m_pShadow = NULL;
	}
	// �����蔻����̊J��
	if (m_Collision != NULL)
	{
		m_Collision->CompulsionScene();
		m_Collision->Release();
		m_Collision = NULL;
	}
	// ���f���J���[���NULL�Ȃ�
	if (m_pModelCol != NULL)
	{
		// ���f���J���[���̃������m��
		delete m_pModelCol;
		m_pModelCol = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Update(void)
{
	// �܂�e��NULL�ł͂Ȃ��Ȃ�
	// ->�X�V
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPos(D3DXVECTOR3(m_pos.x,0.1f,m_pos.z));
		m_pShadow->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fShadowAlpha));
	}
	// �����蔻�肪NULL�ł͂Ȃ��Ȃ�
	// �X�V
	if (m_Collision != NULL)
	{
		// �ʒu���̍X�V(�s��n��)
		m_Collision->GetShape()->PassMatrix(m_mtxWorld);
		// �X�V
		m_Collision->Update();
		return;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`�揈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL		*pMat;					// ���݂̃}�e���A���ۑ�
	D3DMATERIAL9		matDef;					// �}�e���A���f�[�^�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	// �X�P�[���𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f //
	// ���s�ړ��s��쐬(�I�t�Z�b�g)
	D3DXMatrixTranslation(&mtxTrans,							// �����̓��ꕨ
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// ���f���̊g��k��
	D3DXMatrixScaling(&m_mtxWorld,
		m_size.x,
		m_size.y,
		m_size.z);

	// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// �e���������Ă���Ƃ�
	// ->�����̃}�g���b�N�X��� * �e�̃}�g���b�N�X���
	if (m_pParentMtx != NULL)
	{
		// �e�Ǝq�̍s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��[�e]�s��)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			m_pParentMtx);
	}
	// �V���h�E�}�b�s���O���I���Ȃ�
	if (m_bShadowMap)
	{
		// �X�e���V���o�b�t�@��L���ɂ���
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
		// �X�e���V���Ώۂ�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
		// �X�e���V���e�X�g�AZ�e�X�g�����Ƃ����i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		// �X�e���V���e�X�g���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		// �X�e���V���e�X�g�s���i�AZ�e�X�g�s���i�̏ꍇ�̔���
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		// �V���h�E�}�b�s���O
		CShadowmapping::Draw(
			pDevice,	// �f�o�C�X���
			m_pModelLoad[m_nModelId].get(),	// �v���C���[(��)���
			m_mtxWorld	// �}�g���b�N�X���
		);
		// �X�e���V���o�b�t�@��L���ɂ���
		pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	}
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelLoad[m_nModelId]->pBuffMat->GetBufferPointer();
	// �J�E���g�}�e���A��
	for (int nCntMat = 0; nCntMat < (int)m_pModelLoad[m_nModelId]->nNumMat; nCntMat++, pMat++)
	{
		// ���f���J���[����NULL�`�F�b�N
		if (m_pModelCol != NULL)
		{
			// ���f���J���[�̐ݒ�
			pMat->MatD3D.Diffuse = *m_pModelCol;
		}
		// �v���C���[(��)�̓����x
		pMat->MatD3D.Diffuse.a = m_fModelAlpha;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat->MatD3D);

		// �e�N�X�`���[�ݒ�
		pDevice->SetTexture(0, m_pModelLoad[m_nModelId]->vec_pTexture[nCntMat]);

		// �`��
		m_pModelLoad[m_nModelId]->pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::Debug(void)
{
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��������I�ɍ폜
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::CollisionDelete(void)
{
	// �����蔻����̊J��
	if (m_Collision != NULL)
	{
		m_Collision->CompulsionScene();
		m_Collision->Release();
		m_Collision = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����(�V�[���Ǘ�)
//	pos			: �ʒu
//	rot			: ��]
//	nModelId	: ���f���ԍ�
//	bShadowMap	: �V���h�E�}�b�s���O���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X * CScene_X::Create(
	D3DXVECTOR3 const & pos,	// �ʒu
	D3DXVECTOR3 const & rot,	// ��]
	int const & nModelId,		// ���f���ԍ�
	bool const & bShadowMap		// �V���h�E�}�b�s���O���
)
{
	// �ϐ��錾
	CScene_X * pScene_X;		// �V�[��3D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_X = new CScene_X();
	// �V�[���Ǘ��ݒ�
	pScene_X->ManageSetting(CScene::LAYER_3DOBJECT);
	// �ݒ�
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// ����������
	pScene_X->Init();

	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����(�l�Ǘ�)
//	pos			: �ʒu
//	rot			: ��]
//	nModelId	: ���f���ԍ�
//	bShadowMap	: �V���h�E�}�b�s���O���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X * CScene_X::Create_Self(
	D3DXVECTOR3 const & pos,	// �ʒu
	D3DXVECTOR3 const & rot,	// ��]
	int const & nModelId,		// ���f���ԍ�
	bool const & bShadowMap		// �V���h�E�}�b�s���O���
)
{
	// �ϐ��錾
	CScene_X * pScene_X;		// �V�[��X�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScene_X = new CScene_X();
	// �ݒ�
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// ����������
	pScene_X->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �쐬����(�l�Ǘ�)
//	pos			: �ʒu
//	rot			: ��]
//	nModelId	: ���f���ԍ�
//	bShadowMap	: �V���h�E�}�b�s���O���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CScene_X> CScene_X::Create_Uni(
	D3DXVECTOR3 const & pos,	// �ʒu
	D3DXVECTOR3 const & rot,	// ��]
	int const & nModelId,		// ���f���ԍ�
	bool const & bShadowMap		// �V���h�E�}�b�s���O���
)
{
	// �ϐ��錾
	std::unique_ptr<CScene_X> pScene_X(new CScene_X);		// �V�[��X�N���X
	// �ݒ�
	pScene_X->m_pos = pos;
	pScene_X->m_rot = rot;
	pScene_X->m_nModelId = nModelId;
	pScene_X->m_bShadowMap = bShadowMap;
	// ����������
	pScene_X->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pScene_X;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[(��)���̓ǂݍ��ݏ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CScene_X::LoadModel(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	std::vector<std::vector<std::string>> vsvec_Contens;		// �e�L�X�g�f�[�^�i�[�p
	// �t�@�C���̒��g���擾����
	vsvec_Contens = CCalculation::FileContens(TEXTURE_FILE, ',');
	// �s���Ƃɉ�
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// ���ڂ��Ƃɉ�
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			// �ϐ��錾
			std::unique_ptr<MODEL_LOAD> pModelLoad(new MODEL_LOAD);	// �v���C���[(��)���̓ǂݍ��ݗp
			switch (nCntItem)
			{
				// �p�X���
			case 0:
				// X�t�@�C���̓ǂݍ���
				if (!D3DXLoadMeshFromX(
					vsvec_Contens[nCntLine][nCntItem].c_str(),
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&pModelLoad->pBuffMat,
					NULL,
					&pModelLoad->nNumMat,
					&pModelLoad->pMesh) == S_OK
					)
				{
					std::string sNum = std::to_string(nCntLine);
					CCalculation::Messanger(sNum.c_str());
					break;
				}
				// �v���C���[(��)���̐ݒ�
				ModelSetting(pModelLoad.get());
				// �v���C���[(��)���̊i�[
				m_pModelLoad.push_back(std::move(pModelLoad));
				break;
			default:
				break;
			}
			// ���f���ǂݍ��ݕϐ��̏�����
			pModelLoad.reset();
		}
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[(��)���̊J������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CScene_X::UnLoadModel(void)
{
	// �v���C���[(��)���̊J��
	for (int nCntModel = 0; nCntModel < (signed)m_pModelLoad.size(); nCntModel++)
	{
		// ���b�V���̊J��
		if (m_pModelLoad[nCntModel]->pMesh != NULL)
		{
			m_pModelLoad[nCntModel]->pMesh->Release();
			m_pModelLoad[nCntModel]->pMesh = NULL;
		}

		// �}�e���A���̊J��
		if (m_pModelLoad[nCntModel]->pBuffMat != NULL)
		{
			m_pModelLoad[nCntModel]->pBuffMat->Release();
			m_pModelLoad[nCntModel]->pBuffMat = NULL;
		}

		// �e�N�X�`���[�̊J��
		for (int nCntTex = 0; nCntTex < (signed)m_pModelLoad[nCntModel]->vec_pTexture.size(); nCntTex++)
		{
			if (m_pModelLoad[nCntModel]->vec_pTexture[nCntTex] != NULL)
			{
				m_pModelLoad[nCntModel]->vec_pTexture[nCntTex]->Release();
				m_pModelLoad[nCntModel]->vec_pTexture[nCntTex] = NULL;
			}
			m_pModelLoad[nCntModel]->vec_pTexture.clear();
			m_pModelLoad[nCntModel]->vec_pTexture.shrink_to_fit();
		}
		// ���f���ǂݍ��ݕϐ��̏�����
		m_pModelLoad[nCntModel].reset();
	}
	m_pModelLoad.clear();
	m_pModelLoad.shrink_to_fit();
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[(��)�ݒ菈��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::ModelSetting(MODEL_LOAD * pModel_load)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X
	D3DXMATERIAL *pMat;				// ���݂̃}�e���A���ۑ�
	int nNumVertex;					// ���_��
	DWORD sizeFVF;					// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVertexBuffer;			// ���_�o�b�t�@�ւ̃|�C���^
	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();
	// �}�e���A���̒��_�ʒu�̍ő�l�E�ŏ��l
	pModel_load->vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	pModel_load->vtxMaxMaterials = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	// ���_�����擾
	nNumVertex = pModel_load->pMesh->GetNumVertices();
	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(pModel_load->pMesh->GetFVF());
	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)pModel_load->pBuffMat->GetBufferPointer();
	// �}�e���A�������擾
	for (int nCntMat = 0; nCntMat < (int)pModel_load->nNumMat; nCntMat++, pMat++)
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTexture);
		pModel_load->vec_pTexture.push_back(std::move(pTexture));
	}

	// ���_�o�b�t�@�����b�N
	pModel_load->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);
	// ���_�����J��Ԃ�
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		// �Ԓn����D3DXVE
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
		// x //
		// �ŏ��l
		if (pModel_load->vtxMinMaterials.x > vtx.x)
		{
			pModel_load->vtxMinMaterials.x = vtx.x;
		}
		// �ő�l
		if (pModel_load->vtxMaxMaterials.x < vtx.x)
		{
			pModel_load->vtxMaxMaterials.x = vtx.x;
		}
		// y //
		// �ŏ��l
		if (pModel_load->vtxMinMaterials.y > vtx.y)
		{
			pModel_load->vtxMinMaterials.y = vtx.y;
		}
		// �ő�l
		if (pModel_load->vtxMaxMaterials.y < vtx.y)
		{
			pModel_load->vtxMaxMaterials.y = vtx.y;
		}
		// z //
		// �ŏ��l
		if (pModel_load->vtxMinMaterials.z > vtx.z)
		{
			pModel_load->vtxMinMaterials.z = vtx.z;
		}
		// �ő�l
		if (pModel_load->vtxMaxMaterials.z < vtx.z)
		{
			pModel_load->vtxMaxMaterials.z = vtx.z;
		}
		// �T�C�Y�|�C���^��i�߂�
		pVertexBuffer += sizeFVF;
	}
	// �A�����b�N
	pModel_load->pMesh->UnlockVertexBuffer();
	// �T�C�Y
	pModel_load->size = pModel_load->vtxMaxMaterials - pModel_load->vtxMinMaterials;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻��ݒ�
// 0:��`�A1:���A2:�~��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::SetCollision(
	int const & nShapeType,
	int const &obj,
	bool const &bPush,
	bool const &bOpponent,
	CScene * pParent,
	D3DXVECTOR3 const &offset_pos
)
{
	// �����蔻�肪NULL�ł͂Ȃ��Ȃ�
	// ->�֐��𔲂���
	if (m_Collision != NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("CScene_X::SetCollision�̒�->���ɂ����蔻��̃f�[�^�������Ă��܂�");
#endif // _DEBUG
		return;
	}
	// �`�̃^�C�v
	switch (nShapeType)
	{
		// ��`
	case CShape::SHAPETYPE_RECT:
		// ��`�̓����蔻�萶��
		m_Collision = CRectCollision::Create(
			m_pModelLoad[m_nModelId]->size,
			offset_pos,
			(CCollision::OBJTYPE)obj,
			this,
			pParent,
			bPush,
			bOpponent,
			&m_pos
		);
		// �ʒu���̍X�V(�s��n��)
		m_Collision->GetShape()->PassMatrix(m_mtxWorld);
		break;
		// ��
	case CShape::SHAPETYPE_SPHERE:
		// ���̓����蔻�萶��
		m_Collision = CSphereCollision::Create(
			m_pModelLoad[m_nModelId]->size.x,
			offset_pos,
			(CCollision::OBJTYPE)obj,
			this,
			pParent,
			bPush,
			bOpponent
		);
		// �ʒu���̍X�V(�s��n��)
		m_Collision->GetShape()->PassMatrix(m_mtxWorld);
		break;
		// �~��
	case CShape::SHAPETYPE_COLUMN:
		// ���̓����蔻�萶��
		m_Collision = CColumnCollision::Create(
			m_pModelLoad[m_nModelId]->size.x,
			m_pModelLoad[m_nModelId]->size.y,
			offset_pos,
			(CCollision::OBJTYPE)obj,
			this,
			pParent,
			bPush,
			bOpponent
		);
		// �ʒu���̍X�V(�s��n��)
		m_Collision->GetShape()->PassMatrix(m_mtxWorld);
		break;
	default:
		break;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���f���J���[���̐ݒ�
//	col	: �J���[
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::SetModelColor(
	D3DXCOLOR const &col
)
{
	// ���f���J���[���NULL�Ȃ�
	if (m_pModelCol == NULL)
	{
		// ���f���J���[���̃������m��
		m_pModelCol = new D3DXCOLOR;
	}
	// �J���[�̑��
	*m_pModelCol = col;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// X�v���C���[(��)�擾����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::MODEL_LOAD * CScene_X::GetModel(int const &nModelId)
{
	return m_pModelLoad[nModelId].get();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻���Ԏ擾����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool const CScene_X::GetbCollision(void)
{
	// �����蔻����NULL�Ȃ�
	// ->�֐��𔲂���
	if (m_Collision == NULL) return false;
	return m_Collision->GetbCollision();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �����蔻����擾����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision * CScene_X::GetCollision(void)
{
	return m_Collision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �v���C���[(��)���擾����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_X::MODEL_LOAD * CScene_X::GetModelLoad(int const & nModelId)
{
	// �v���C���[(��)�ԍ����T�C�Y�ȏ�Ȃ�
	// ->�֐��𔲂���
	if ((signed)m_pModelLoad.size() >= nModelId)
	{
		return m_pModelLoad[0].get();
	}
	return m_pModelLoad[nModelId].get();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�N���v�g��ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CScene_X::LoadScrept(char* add)
{
	FILE *pFile = NULL;											// �t�@�C��
	char cReadText[128] = {};									// ����
	char cHeadText[128] = {};									// ��r
	CScene_X *pObject = NULL;

	pFile = fopen(add, "r");									// �t�@�C�����J���܂��͍��

	if (pFile != NULL)											// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�

		while (strcmp(cHeadText, "End") != 0)
		{
			std::string Data = cReadText;
			std::vector<std::string> splitData = CCalculation::split(Data, ',');

			pObject = CScene_X::Create(D3DXVECTOR3((float)atof(splitData[LOADTYPE_POS_X].c_str()), (float)atof(splitData[LOADTYPE_POS_Y].c_str()), (float)atof(splitData[LOADTYPE_POS_Z].c_str())),
									D3DXVECTOR3((float)atof(splitData[LOADTYPE_ROT_X].c_str()), (float)atof(splitData[LOADTYPE_ROT_Y].c_str()), (float)atof(splitData[LOADTYPE_ROT_Z].c_str())),
									atoi(splitData[LOADTYPE_ADD].c_str()));

			fgets(cReadText, sizeof(cReadText), pFile);									// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);					// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}