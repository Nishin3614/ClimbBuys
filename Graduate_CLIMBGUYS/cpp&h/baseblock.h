// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �x�[�X�u���b�N���� [baseblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _BASEBLOCK_H_
#define _BASEBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_x.h"
#include <random>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if ERROW_ACTION

#define BASEBLOCK_DEBUG				(0)					// �f�o�b�O�������

#endif // ERROW_ACTION

#define BASEBLOCK_FIELDMAX			(14)				// �t�B�[���h�̃u���b�N��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCircleshadow;	// �~�V���h�E�N���X

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseblock : public CScene_X
{
public:
	/* �񋓌^ */
	// �x�[�X�u���b�N
	typedef enum
	{
		BLOCKTYPE_NORMAL = 0,	// �ʏ�
		BLOCKTYPE_FIELD,		// �t�B�[���h�u���b�N
		BLOCKTYPE_BOMB,			// �{���u���b�N
		BLOCKTYPE_SPRING,		// �X�v�����O�u���b�N
		BLOCKTYPE_STEEL,		// �|�S�u���b�N
		BLOCKTYPE_PANIC,		// �����u���b�N
		BLOCKTYPE_ELECTRIC,		// �d�C�u���b�N
		BLOCKTYPE_MAX			// �^�C�v�S�̐�
	} BLOCKTYPE;

	// �㉺���E�O��
	typedef enum
	{
		DIRECTION_UP = 0b000001,	// ��
		DIRECTION_UNDER = 0b000010,	// ��
		DIRECTION_LEFT = 0b000100,	// ��
		DIRECTION_RIGHT = 0b001000,	// �E
		DIRECTION_FRONT = 0b010000,	// �O
		DIRECTION_BACK = 0b100000,	// ���
		DIRECTION_MAX
	} DIRECTION;

	/* �\���� */
	// �Ֆʏ��
	typedef struct GRID
	{
		// �R���X�g���N�^
		GRID() {}
		// �R���X�g���N�^
		//	Column	: ��
		//	Height	: ����
		//	Line	: �s
		GRID(int const &Column, int const &Height, int const &Line)
		{
			nColumn = Column;
			nLine = Line;
			nHeight = Height;
		}
		// �s�񍂂��̑����Z�̃I�y���[�V����
		GRID operator + (GRID const & Source) const
		{
			// �l��n���ϐ�
			GRID variable;
			variable.nColumn = nColumn + Source.nColumn;
			variable.nLine = nLine + Source.nLine;
			variable.nHeight = nHeight + Source.nHeight;
			return variable;
		}
		// �s�񍂂��̈����Z�̃I�y���[�V����
		GRID operator - (GRID const & Source) const
		{
			// �l��n���ϐ�
			GRID variable;
			variable.nColumn = nColumn - Source.nColumn;
			variable.nLine = nLine - Source.nLine;
			variable.nHeight = nHeight - Source.nHeight;
			return variable;
		}
		// �s�񍂂��̊|���Z�̃I�y���[�V����
		GRID operator * (int const & Source) const
		{
			// �l��n���ϐ�
			GRID variable;
			variable.nColumn = nColumn + Source;
			variable.nLine = nLine + Source;
			variable.nHeight = nHeight + Source;
			return variable;
		}
		// �s�񍂂���+=�̃I�y���[�V����
		GRID & operator += (GRID const & Grid)
		{
			nColumn += Grid.nColumn;
			nLine += Grid.nLine;
			nHeight += Grid.nHeight;
			return *this;
		}
		// �s�񍂂���+=�̃I�y���[�V����
		bool operator == (GRID & Grid)
		{
			return Grid.nColumn == nColumn && Grid.nLine == nLine && Grid.nHeight == nHeight;
		}
		// �s�񍂂���<=�̃I�y���[�V����
		bool operator <= (GRID & Grid)
		{
			return nColumn <= Grid.nColumn && nLine <= Grid.nLine && nHeight <= Grid.nHeight;
		}
		// �s�񍂂���>=�̃I�y���[�V����
		bool operator >= (GRID & Grid)
		{
			return nColumn >= Grid.nColumn && nLine >= Grid.nLine && nHeight >= Grid.nHeight;
		}
		D3DXVECTOR3 GetPos(float const & fRange)
		{
			D3DXVECTOR3 pos;
			pos.x = nColumn * fRange;
			pos.z = nLine * fRange;
			pos.y = nHeight * fRange;
			return pos;
		}
		int nColumn;	// ��
		int nLine;		// �s
		int nHeight;	// ����
	} GRID;
	// �u���b�N�S�̂ɑ΂��Ă̋����̔�r�ƃu���b�N���Ƃǂ��������Ȃ牟���ꂽ�����
	typedef struct PUSHBLOCK
	{
		PUSHBLOCK() {};
		PUSHBLOCK(CBaseblock * pSouceBlock, float fSouceDistance, COLLISIONDIRECTION SouceDirection)
		{
			pBlock = pSouceBlock;
			fDistance = fSouceDistance;
			Direction = SouceDirection;
		}
		CBaseblock * pBlock;				// �u���b�N���
		float fDistance;					// ����
		COLLISIONDIRECTION Direction;		// ����
	} PUSHBLOCK;
	// �����o������̕ϐ�
	typedef struct PUSHAFTER
	{
		PUSHAFTER() {};
		PUSHAFTER(bool const & SoucePushState,GRID const & SourceGrid) : bPushState(SoucePushState) , PushGrid(SourceGrid)
		{
		}
		void Init()
		{
			bPushState = false;
			PushGrid = GRID(0, 0, 0);
			GoalPos = D3DVECTOR3_ZERO;
		}
		bool		bPushState;	// �����o�����
		GRID		PushGrid;	// �s��
		D3DXVECTOR3	GoalPos;	// �ړI�ʒu
	} PUSHAFTER;


	// ----- �u���b�N�̃X�e�[�^�X ----- //
	typedef struct _BLOCK_STATUS
	{
		_BLOCK_STATUS()
		{
			// �u���b�N�p
			fMove = 0;				// �ړ���
			nAppearance = 0;		// �o�����鍂��
			fBasicShadowSize = 0;	// �V���h�E�T�C�Y
			nMaxSprit = 0;			// �ő啪����
			nChangeTime = 0;		// �ω������鎞��(�ω�����^�C�~���O)
			nAppBlock = 0;			// �u���b�N���o������^�C�~���O

			// �_���[�W���p
			fInitFloor = -100.0f;	// �_���[�W���̏����ʒu
			fFloorMove = 0;			// �_���[�W���̈ړ����x
			nFloorPhase = 0;		// �_���[�W���̏オ��t�F�[�Y�^�C�~���O
		}
		// �u���b�N�p
		float					fMove;					// �ړ���
		int						nAppearance;			// �o�����鍂��
		float					fBasicShadowSize;		// �V���h�E�T�C�Y
		int						nMaxSprit;				// �ő啪����
		int						nChangeTime;			// �ω������鎞��(�ω�����^�C�~���O)
		int						nAppBlock;				// �u���b�N���o������^�C�~���O
		std::vector<INTEGER2>	v_nDropBlock;			// ���Ƃ��u���b�N�̐�
		std::vector<FLOAT2>		v_fBlockGravity;		// �����鑬�x

		// �_���[�W���p
		std::vector<int>		v_nDamageFloorHight;	// �_���[�W���̍���
		float					fInitFloor;				// �_���[�W���̏����ʒu
		float					fFloorMove;				// �_���[�W���̈ړ����x
		int						nFloorPhase;			// �_���[�W���̏オ��t�F�[�Y�^�C�~���O
	} BLOCK_STATUS;

	/* �֐� */
	// �R���X�g���N�^
	CBaseblock();
	// �f�X�g���N�^
	virtual ~CBaseblock();
	// ����������
	virtual void Init(void);
	// �X�V����
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);

	// ����������̔���
	//	Obj		: �I�u�W�F�^�C�v
	//	pScene	: �V�[�����
	virtual void HitCollision(
		COLLISIONDIRECTION const &Direct,	// �O�㍶�E�㉺
		CScene::OBJ const & Obj,			// �I�u�W�F�^�C�v
		CScene * pScene = NULL				// �V�[�����
	) {};

	// ����������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// �������瓖����Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// �����o�������蔻��
	//	Obj		: �I�u�W�F�^�C�v
	//	pos		: �ʒu
	//	posOld	: �O��̈ʒu
	//	move	: �ړ���
	//	size	: �T�C�Y
	COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// �I�u�W�F�^�C�v
		D3DXVECTOR3 * pos,								// �ʒu
		D3DXVECTOR3 * posOld,							// �O��̈ʒu
		D3DXVECTOR3 * move,								// �ړ���
		D3DXVECTOR3 * size,								// �T�C�Y
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// �I�t�Z�b�g�ʒu
	);
	// �����蔻��
	//	Obj		: �I�u�W�F�^�C�v
	//	pos			: �ʒu
	//	size		: �T�C�Y
	//	OffsetPos	: �I�t�Z�b�g�ʒu
	COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// �I�u�W�F�^�C�v
		D3DXVECTOR3 * pos,								// �ʒu
		D3DXVECTOR3 * posOld,							// �O��̈ʒu
		D3DXVECTOR3 * size,								// �T�C�Y
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// �I�t�Z�b�g�ʒu
	);
	// �u���b�N�̉����o������
	//	LineBegin	: ���̎n�_
	//	LineEnd		: ���̏I�_
	//	fDistance	: ����
	COLLISIONDIRECTION PushBlock(
		D3DXVECTOR3 const & LineBegin,	// 	LineBegin	: ���̎n�_
		D3DXVECTOR3 const & LineEnd,	// 	LineEnd		: ���̏I�_
		float &				fDistance	//	fDistance	: ����
	);

	// �x�[�X�u���b�N
	void SetType(BLOCKTYPE const Blocktype)			{ m_BlockType = Blocktype; };
	// �x�[�X�u���b�N
	BLOCKTYPE GetType(void) const					{ return m_BlockType; };
	// �������Ԑݒ�
	void SetFall(bool const & bFall)				{ m_bFall = bFall; };
	// �������Ԏ擾
	bool & GetFall(void)							{ return m_bFall; };
	// �g�p��Ԑݒ�
	void SetUse(bool const & bUse)					{ m_bUse = bUse; };
	// �g�p��Ԏ擾
	bool & GetUse(void)								{ return m_bUse; };
	// �V���h�E�̎g�p��ԏ�Ԑݒ�
	void SetShadow(bool const & bShadow)			{ m_bShadow = bShadow; };
	// �V���h�E�̎g�p��ԏ�Ԏ擾
	bool & GetShadow(void)							{ return m_bShadow; };
	// �Ֆʏ��擾
	GRID & GetGrid(void)							{ return m_grid; };
	// �Ֆʏ��ݒ�
	void SetGrid(GRID const &grid)					{ m_grid = grid; };
	// �����o������擾
	PUSHAFTER & GetPushAfter(void)					{ return m_PushAfeter; };
	// �����o������ݒ�
	void SetPushAfter(PUSHAFTER const &PushAfter);
	// �O��̈ʒu�擾
	D3DXVECTOR3 & GetPosOld(void)					{ return m_posOld; };

	// �x�[�X�u���b�N�̎�ސݒ�
	void SetGravity(float fGravity)					{ m_fGravity = fGravity; };
	// �x�[�X�u���b�N�̎�ގ擾
	float GetGravity()								{ return m_fGravity; };
	// �w�肵���x�[�X�u���b�N���폜���鏈��
	//	pBlock	: �u���b�N���
	static bool DeleteBlock(
		CBaseblock * pBlock	// �u���b�N���
	);
	// �x�[�X�u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �x�[�X�u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);
	// �쐬(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	//	layer		: ���C���[
	static CBaseblock * Create(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId,								// ���f���ԍ�
		CScene::LAYER	const & layer = CScene::LAYER_3DBLOCK				// ���C���[
	);
	// �쐬(�l�Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	static CBaseblock * Create_Self(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId								// ���f���ԍ�
	);
	// unique_ptr�쐬(�l�Ǘ�unique)
	// ���߂�l��std::move�Ŏ󂯎��
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	static std::unique_ptr<CBaseblock> Creat_Unique(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId								// ���f���ԍ�
	);
	// �^�C�g���p �u���b�N����Ăɐ���
	static void CreateInBulkBlock();
	// �s�񍂂�����v����u���b�N���擾����֐�
	static CBaseblock * GetBaseBlock(GRID & Grid);
	// �w�肵���s��̑S�Ă̗������Ԃ�false�Ɋ֐�
	static void FallBlock_Grid(GRID & Grid);
	// �T�C�Y�͈͐ݒ�
	static void SetSizeRange(float const &fSizeRange) { m_fSizeRange = fSizeRange; };
	// �T�C�Y�͈͎擾
	static float & GetSizeRange(void) { return m_fSizeRange; };
	// �t�F�[�h�̒l�ݒ�
	//	nFeedValue	: �t�F�[�h�̒l
	static void SetFeedValue(
		int const & nFeedValud	// �t�F�[�h�̒l
	) { m_nFeedValue.emplace_back(nFeedValud); };
	// �t�F�[�h�̒l�擾
	//	nStage	: �X�e�[�W�ԍ�
	static int GetFeedValue(
		int const & nStage	// �t�F�[�h�̒l
	);
	// �u���b�N�̈ʒu�v�Z
	//static D3DXVECTOR3 CaluBlockPos(GRID const & Grid)
	//{ return D3DXVECTOR3() }

	/* �����p */
	// ���ݐςݏd�Ȃ��Ă���u���b�N�̍������擾
	//	nColumn	: ��
	//	nLine	: �s
	static int GetHeight(
		int const & nColumn,
		int const & nLine
	);
	// ���ݐςݏd�Ȃ��Ă���u���b�N�̍�����ݒ�
	//	nColumn	: ��
	//	nLine	: �s
	//	nHeight	: ����
	static void SetHeight(
		int const & nColumn,
		int const & nLine,
		int const & nHeight
	);
	// ���ݐςݏd�Ȃ��Ă���u���b�N�̍�����ݒ�
	//	Grid	: ��
	static void SetHeight(
		GRID const & Grid
	);
	// �u���b�N�̃X�e�[�^�X���擾
	static BLOCK_STATUS const & GetBlockStatus(void) { return m_BlockStatus; };
	// �u���b�N�̃X�e�[�^�X�ǂݍ��ݏ���
	static void BlockStatusLoad(void);
	// �u���b�N�̃X�e�[�^�X�������ݏ���
	static void BlockStatusSave(void);
	// �u���b�N�̐ÓI�ϐ�������������
	static void BlockStaticValue(void);
	// �u���b�N�̍ő卂�����擾
	static int GetMaxHeight(void) { return m_nMaxHeight; };
	// �u���b�N�̍ő卂�����Đݒ�
	static void SetMaxHeight(void);
	// �t�F�[�Y�̎擾
	static int GetPhase(void)					{ return m_nPhase; };
	// �t�F�[�Y�̐ݒ�
	static void SetPhase(int const & nPhase)	{ m_nPhase = nPhase; };
#if IMGUI_DEBUG

	// �S�̂̃f�o�b�O����
	static void AllDebug(void);

#endif // ERROW_ACTION

#ifdef _DEBUG
	// �f�o�b�O����
	virtual void  Debug(void);
#endif // _DEBUG
#if BASEBLOCK_DEBUG
	static void NumAllDebug(void);				// �S�̌���\������f�o�b�O����
#endif // BASEBLOCK_DEBUG
	/* �ϐ��錾 */


protected:
	/* �֐� */

	// �ݒ� //
	/* �ϐ��錾 */
	static float			m_fSizeRange;	// �T�C�Y�͈�
	static std::vector<int>	m_nFeedValue;	// �t�F�[�h�̒l
private:
	/* �֐� */
	// ������X�V����
	void Update_Fall(void);
	// �����o����Ԃ̃u���b�N�̍X�V����
	void Update_PushState(void);
	// ���g�̃V���h�E�̏o����������
	void Update_MyShadow(void);
	// �������
	void Update_Limit(void);
	// �����蔻�菈��
	void Collision(CBaseblock * pBlock);
	// ���M�Ƒ��̃u���b�N�̔�r���A�V���h�E���X�V�����鏈��
	void Update_OtherShadow(CBaseblock * pBlock);
	/* �ϐ� */
	static BLOCK_STATUS		m_BlockStatus;		// �u���b�N�̃X�e�[�^�X
	CCircleshadow *			m_pShadowPolygon;	// �V���h�E�|���S��
	D3DXVECTOR3				m_posOld;			// �O��̈ʒu
	BLOCKTYPE				m_BlockType;		// �x�[�X�u���b�N
	GRID					m_grid;				// �Ֆʏ��
	PUSHAFTER				m_PushAfeter;		// �����o������v�̕ϐ�
	float					m_fGravity;			// �d��
	bool					m_bFall;			// ��������
	bool					m_bShadow;			// �V���h�E�̎g�p���
	bool					m_bUse;				// �\�����
	static int				m_nPhase;			// �t�F�[�Y
	// �����p
	static int				m_anHeight[BASEBLOCK_FIELDMAX][BASEBLOCK_FIELDMAX];	// ���ꂼ��̍s��̍���
	static int				m_nMaxHeight;		// �ő卂��
#if BASEBLOCK_DEBUG
	static int				m_nAll;				// �S�̌�
#endif // BASEBLOCK_DEBUG

};

#endif