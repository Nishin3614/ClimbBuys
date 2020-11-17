// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 結合フィールドブロック処理 [connect_fieldblock.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "connect_fieldblock.h"
#include "fieldblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CONNECT_FIELDBLOCK_FILE	("data/LOAD/MAPPING/fieldblock.csv")

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<CConnect_fieldblock::LOAD>	CConnect_fieldblock::m_vpLoad;									// ファイルの読み込み情報

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnect_fieldblock::CConnect_fieldblock() : CScene::CScene()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnect_fieldblock::~CConnect_fieldblock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Init()
{
	// 変数宣言
	CBaseblock::GRID grid;						// 行列高さの番号
	CGame::STAGE Stage = CGame::GetStage();		// ステージ
	grid.nLine = m_vpLoad[Stage].nFeed;		// 行
	grid.nColumn = -(m_vpLoad[Stage].nFeed);	// 列
	grid.nHeight = 0;								// 高さ
	// フィールドループ
	for (size_t nCntField = 0; nCntField < m_vpLoad[Stage].Dvec_pFileLoad.size(); nCntField++)
	{
		// ブロックのループ
		for (size_t nCntBlock = 0; nCntBlock < m_vpLoad[Stage].Dvec_pFileLoad[nCntField].size(); nCntBlock++)
		{
			// 使用状態ではないなら
			// ->ループスキップ
			if (!m_vpLoad[Stage].Dvec_pFileLoad[nCntField][nCntBlock].bUse)
			{
				// 列ダウン
				grid.nColumn++;
				continue;
			}
			// フィールドブロックの生成
			CFieldblock::Create(2, grid);
			// 列ダウン
			grid.nColumn++;
		}
		// 行アップ
		grid.nLine--;
		// 列初期化
		grid.nColumn = -(m_vpLoad[Stage].nFeed);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Uninit(void)
{
	/*
	// フィールドループ
	for (size_t nCntBlock = 0; nCntBlock < m_Dvec_pFieldBlock.size(); nCntBlock++)
	{
		m_Dvec_pFieldBlock[nCntBlock] = NULL;
	}
	m_Dvec_pFieldBlock.clear();
	m_Dvec_pFieldBlock.shrink_to_fit();
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Update(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックが落ちる処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::BlockFall(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Draw(void)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Scene_MyCollision(
	int const & nObjType,
	CScene * pScene
)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 結合フィールドブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CConnect_fieldblock::Load(void)
{
	// CSVファイルの読み込み処理
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// ファイルの中身格納用
	std::vector<std::vector<std::string>> vsvec_Contens;
	std::vector<std::vector<FIELDINFO>>	vvpFileLoad;	// ファイルの読み込み情報
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(CONNECT_FIELDBLOCK_FILE, ',');
	// 行ごとに回す
	for (size_t nCntLine = 0; nCntLine < vsvec_Contens.size(); nCntLine++)
	{
		std::vector<FIELDINFO> vec_Load;					// 読み込み情報
		// 項目ごとに回す
		for (size_t nCntItem = 0; nCntItem < vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			// 読み込んだ情報にEndが出力されていたら
			if (strcmp(vsvec_Contens[nCntLine][nCntItem].c_str(), "End") == 0)
			{
				LOAD load;
				load.Dvec_pFileLoad = vvpFileLoad;
				load.nFeed = ((signed)vvpFileLoad.size() - 1) / 2;
				// フェードの値の設定
				CBaseblock::SetFeedValue(load.nFeed);
				// 読み込んだフィードブロックの情報を格納
				m_vpLoad.emplace_back(load);
				std::vector<std::vector<FIELDINFO>>().swap(vvpFileLoad);
				continue;
			}
			// 情報が入っていない
			if (strcmp(vsvec_Contens[nCntLine][nCntItem].c_str(), "") == 0)
			{
				continue;
			}
			// 変数宣言
			FIELDINFO fieldinfo;	// フィールド情報
			// 番号を代入
			fieldinfo.nFailId = stoi(vsvec_Contens.at(nCntLine).at(nCntItem));
			if (fieldinfo.nFailId == 0)
			{
				fieldinfo.bUse = false;
			}
			else
			{
				fieldinfo.bUse = true;
			}
			vec_Load.emplace_back(fieldinfo);
		}
		vvpFileLoad.emplace_back(vec_Load);
		vec_Load.clear();
		vec_Load.shrink_to_fit();
	}
	// std::vectorの多重配列開放
	std::vector<std::vector<std::string>>().swap(vsvec_Contens);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::UnLoad(void)
{
	// ロードループ
	for (size_t nCntLoad = 0; nCntLoad < m_vpLoad.size(); nCntLoad++)
	{
		// フィールドループ
		for (size_t nCntField = 0; nCntField < m_vpLoad[nCntLoad].Dvec_pFileLoad.size(); nCntField++)
		{
			m_vpLoad[nCntLoad].Dvec_pFileLoad[nCntField].clear();
			m_vpLoad[nCntLoad].Dvec_pFileLoad[nCntField].shrink_to_fit();
		}
	}
	// 読み込んだ情報の開放
	m_vpLoad.clear();
	m_vpLoad.shrink_to_fit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	stage		: ステージ情報
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnect_fieldblock * CConnect_fieldblock::Create(
	CGame::STAGE			const & stage,									// ステージ情報
	CScene::LAYER	const & layer									// レイヤー
)
{
	// 変数宣言
	CConnect_fieldblock * pConnect_fieldblock;		// 結合フィールドブロック
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pConnect_fieldblock = new CConnect_fieldblock();
	// 設定
	// シーン管理設定
	pConnect_fieldblock->ManageSetting(layer);
	// 初期化処理
	pConnect_fieldblock->Init();
	// 生成したオブジェクトを返す
	return pConnect_fieldblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)
//	stage		: ステージ情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CConnect_fieldblock * CConnect_fieldblock::Create_Self(
	CGame::STAGE	const & stage									// ステージ情報
)
{
	// 変数宣言
	CConnect_fieldblock * pConnect_fieldblock;		// 結合フィールドブロック
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pConnect_fieldblock = new CConnect_fieldblock;
	// 初期化処理
	pConnect_fieldblock->Init();
	// 生成したオブジェクトを返す
	return pConnect_fieldblock;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr作成(個人管理unique)
// ※戻り値はstd::moveで受け取る
//	stage		: ステージ情報
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CConnect_fieldblock> CConnect_fieldblock::Creat_Unique(
	CGame::STAGE	const & stage									// ステージ情報
)
{
	// 変数宣言
	std::unique_ptr<CConnect_fieldblock> pConnect_fieldblock(new CConnect_fieldblock);		// 結合フィールドブロック
	// 初期化処理
	pConnect_fieldblock->Init();
	// 生成したオブジェクトを返す
	return pConnect_fieldblock;
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CConnect_fieldblock::Debug(void)
{

}
#endif // _DEBUG

