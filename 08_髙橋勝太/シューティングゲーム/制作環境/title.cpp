//-----------------------------------------------------------------
//
// タイトルの処理 (titile.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (3)										// アニメーションパターンNo.の最大数
#define TITLELOGO_X		(1000.0f)								// タイトルロゴ幅
#define TITLELOGO_Y		(500.0f)								// タイトルロゴの高さ
#define HEIGHT_Y		(500.0f * -1.0f)						// タイトルロゴの初期中心Y座標
#define MOVE_TITLELOGO	(5.0f)									// タイトルロゴの移動量
#define PRESSENTER_X	(300.0f)								// プレスエンターの幅
#define PRESSENTER_Y	(100.0f)								// プレスエンターの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitle[3] = {};
D3DXVECTOR3 g_posTitleLogo;						// タイトルロゴの中心座標
D3DXVECTOR3 g_posPressEnter;					// プレスエンターの中心座標
D3DXCOLOR g_color;								// 色
float g_nCnt;									// カウント

//-----------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------
HRESULT InitTitle(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posTitleLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_posPressEnter = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) * 3, 0.0f);
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCnt = 0;

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BG2.jpg", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titile (2).png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTextureTitle[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_PATTERN; nCntTitle++)
	{
		// 頂点座標の設定
		SetVertexTexture(nCntTitle);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_color;
		pVtx[1].col = g_color;
		pVtx[2].col = g_color;
		pVtx[3].col = g_color;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------
void UninitTitle(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		if (g_pTextureTitle[nCntTexture] != NULL)
		{
			g_pTextureTitle[nCntTexture]->Release();
			g_pTextureTitle[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------
void UpdateTitle(void)
{
	// フェード情報の取得
	int nFade = GetFade();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// タイトルロゴの位置更新
	pVtx[4].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);

	// プレスエンターの色更新
	pVtx[8].col = g_color;
	pVtx[9].col = g_color;
	pVtx[10].col = g_color;
	pVtx[11].col = g_color;

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{// エンターキーを押したとき
		if (nFade == FADE_IN)
		{// フェードインの時
			SetFade(FADE_NONE, MODE_TITLE);
			g_posTitleLogo.y = SCREEN_HEIGHT / 4;
		}
		else if (g_posTitleLogo.y <= (SCREEN_HEIGHT / 4))
		{
			g_posTitleLogo.y = SCREEN_HEIGHT / 4;
		}
		else if (nFade == FADE_NONE && (g_posTitleLogo.y >= (SCREEN_HEIGHT / 4)))
		{// 何もしていないとき
			SetFade(FADE_OUT, MODE_TUTORIAL);
			g_nCnt = 0;
		}
	}
	// 更新処理
	if (nFade == FADE_NONE && g_posTitleLogo.y <= (SCREEN_HEIGHT / 4))
	{// タイトルロゴの移動量の更新
		g_posTitleLogo.y += MOVE_TITLELOGO;
	}
	else if (nFade == FADE_NONE)
	{// プレスエンターの色更新
	 // カウントする
		g_nCnt++;
		if (g_nCnt == 10)
		{// 10のとき不透明化
			g_color.a = 1.0f;
		}
		else if (g_nCnt == 70)
		{// 70のとき透明化
			g_color.a = 0.0f;
			g_nCnt = 0;			// カウントを初期化
		}
	}
	else if (nFade == FADE_OUT)
	{ // カウントする
		g_nCnt++;
		if (g_nCnt == 10)
		{// 10のとき不透明化
			g_color.a = 1.0f;
		}
		else if (g_nCnt == 20)
		{// 20とき透明化
			g_color.a = 0.0f;
			g_nCnt = 0;			// カウントを初期化
		}
	}
	else
	{// プレスエンターの透明化
		g_color.a = 0.0f;
	}
}

//-----------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------
void DrawTitle(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTitle[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// タイトル画面に貼るテクスチャの指定
//-----------------------------------------------------------------
void SetVertexTexture(int nCntTitle)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (nCntTitle == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntTitle == 1)
	{
		pVtx[4].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
	}
	else if (nCntTitle == 2)
	{
		pVtx[8].pos = D3DXVECTOR3(g_posPressEnter.x - (PRESSENTER_X / 2), g_posPressEnter.y + (PRESSENTER_Y / 2), 0.0f);
		pVtx[9].pos = D3DXVECTOR3(g_posPressEnter.x - (PRESSENTER_X / 2), g_posPressEnter.y - (PRESSENTER_Y / 2), 0.0f);
		pVtx[10].pos = D3DXVECTOR3(g_posPressEnter.x + (PRESSENTER_X / 2), g_posPressEnter.y + (PRESSENTER_Y / 2), 0.0f);
		pVtx[11].pos = D3DXVECTOR3(g_posPressEnter.x + (PRESSENTER_X / 2), g_posPressEnter.y - (PRESSENTER_Y / 2), 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}