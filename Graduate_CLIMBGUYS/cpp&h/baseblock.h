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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#define BASEBLOCK_MINUSTOPLUS	(4)						// �s����v���X��
// ��邱��
// �s��̃t�B�[�h�l���i��

//#define BASEBLOCK_RANGE			(50.0f)				// �u���b�N�͈̔�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_FIELD,			// �t�B�[���h�u���b�N
		TYPE_MAX,			// �^�C�v�S�̐�
	} TYPE;

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

	// �x�[�X�u���b�N
	void SetType(TYPE const type)		{ m_type = type; };
	// �x�[�X�u���b�N
	TYPE GetType(void) const			{ return m_type; };
	// �������Ԑݒ�
	void SetFall(bool const & bFall)	{ m_bFall = bFall; };
	// �������Ԏ擾
	bool & GetFall(void)				{ return m_bFall; };
	// �Ֆʏ��擾
	GRID & GetGrid(void)				{ return m_grid; };
	// �Ֆʏ��ݒ�
	void SetGrid(GRID const &grid)		{ m_grid = grid; };
	// �O��̈ʒu�擾
	D3DXVECTOR3 & GetPosOld(void)		{ return m_posOld; };

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
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void  Debug(void);
#endif // _DEBUG
	/* �ϐ��錾 */

protected:
	/* �֐� */
	//D3DXVECTOR3 & GetGridToGrid(GRID const & Grid) {}
	// �ݒ� //
	/* �ϐ��錾 */
	static float			m_fSizeRange;	// �T�C�Y�͈�
	static std::vector<int>	m_nFeedValue;	// �t�F�[�h�̒l
private:
	/* �֐� */
	// ������X�V����
	void Update_Fall(void);
	/* �ϐ� */
	D3DXVECTOR3		m_posOld;		// �O��̈ʒu
	TYPE			m_type;			// �x�[�X�u���b�N
	GRID			m_grid;			// �Ֆʏ��
	bool			m_bFall;		// ��������
	// �����p
	static int m_anHeight[20][20];
};

#endif