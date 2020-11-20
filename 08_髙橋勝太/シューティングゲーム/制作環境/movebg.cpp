//-----------------------------------------------------------------
//
// 動く背景の処理 (movebg.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "movebg.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN			(4)										// アニメーションパターンNo.の最大数
#define TITLELOGO_X			(1920.0f)								// タイトルロゴ幅
#define TITLELOGO_Y			(1080.0f)								// タイトルロゴの高さ
#define HEIGHT_Y			(1080.0f * -1.0f)						// タイトルロゴの初期中心Y座標
#define MOVE_MOVEBGLOGO		(2.0f)									// タイトルロゴの移動量
#define MOVE_MOVEBGLOGO2	(5.0f)									// タイトルロゴの移動量

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMovebg = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMovebg[4] = {};
D3DXVECTOR3 g_posMovebgLogo;						// タイトルロゴの中心座標
D3DXVECTOR3 g_posMovebgLogo2;
D3DXVECTOR3 g_posMovebgLogo3;
D3DXVECTOR3 g_posMovebgLogo4;
float g_nCntMovebg;									// カウント

//-----------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------
HRESULT InitMovebg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	//// 変数の初期化
	g_posMovebgLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	//g_posMovebgLogo2 = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_posMovebgLogo3 = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	//g_posMovebgLogo4 = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_nCntMovebg = 0;

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureMovebg[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureMovebg[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureMovebg[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureMovebg[3]);
	


	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffMovebg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMovebg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_PATTERN; nCntTitle++)
	{
		// 頂点座標の設定
		SetVertexTextureMovebg(nCntTitle);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffMovebg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------
void UninitMovebg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffMovebg != NULL)
	{
		g_pVtxBuffMovebg->Release();
		g_pVtxBuffMovebg = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		if (g_pTextureMovebg[nCntTexture] != NULL)
		{
			g_pTextureMovebg[nCntTexture]->Release();
			g_pTextureMovebg[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------
void UpdateMovebg(void)
{
	// フェード情報の取得
	//int nFade = GetFade();

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMovebg->Lock(0, 0, (void**)&pVtx, 0);

	// タイトルロゴの位置更新
	pVtx[0].pos = D3DXVECTOR3(g_posMovebgLogo.x - (TITLELOGO_X), g_posMovebgLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posMovebgLogo.x - (TITLELOGO_X), g_posMovebgLogo.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posMovebgLogo.x + (TITLELOGO_X), g_posMovebgLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posMovebgLogo.x + (TITLELOGO_X), g_posMovebgLogo.y - (TITLELOGO_Y / 2), 0.0f);

	pVtx[4].pos = D3DXVECTOR3(g_posMovebgLogo2.x - (TITLELOGO_X), g_posMovebgLogo2.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posMovebgLogo2.x - (TITLELOGO_X), g_posMovebgLogo2.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posMovebgLogo2.x + (TITLELOGO_X), g_posMovebgLogo2.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posMovebgLogo2.x + (TITLELOGO_X), g_posMovebgLogo2.y - (TITLELOGO_Y / 2), 0.0f);

	// タイトルロゴの位置更新
	pVtx[8].pos = D3DXVECTOR3(g_posMovebgLogo3.x - (TITLELOGO_X), g_posMovebgLogo3.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posMovebgLogo3.x - (TITLELOGO_X), g_posMovebgLogo3.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posMovebgLogo3.x + (TITLELOGO_X), g_posMovebgLogo3.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posMovebgLogo3.x + (TITLELOGO_X), g_posMovebgLogo3.y - (TITLELOGO_Y / 2), 0.0f);

	pVtx[12].pos = D3DXVECTOR3(g_posMovebgLogo4.x - (TITLELOGO_X), g_posMovebgLogo4.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[13].pos = D3DXVECTOR3(g_posMovebgLogo4.x - (TITLELOGO_X), g_posMovebgLogo4.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[14].pos = D3DXVECTOR3(g_posMovebgLogo4.x + (TITLELOGO_X), g_posMovebgLogo4.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[15].pos = D3DXVECTOR3(g_posMovebgLogo4.x + (TITLELOGO_X), g_posMovebgLogo4.y - (TITLELOGO_Y / 2), 0.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffMovebg->Unlock();

	// 更新処理
	if ((g_posMovebgLogo.y - SCREEN_HEIGHT) <= SCREEN_HEIGHT/2)
	{
		g_posMovebgLogo.y += MOVE_MOVEBGLOGO;
	}

	else if ((g_posMovebgLogo.y - SCREEN_HEIGHT) >= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT/2, 0.0f);
	}

	if ((g_posMovebgLogo2.y - SCREEN_HEIGHT) <= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo2.y += MOVE_MOVEBGLOGO;
	}

	else if ((g_posMovebgLogo2.y - SCREEN_HEIGHT) >= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo2 = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
	}

	// 更新処理
	if ((g_posMovebgLogo3.y - SCREEN_HEIGHT) <= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo3.y += MOVE_MOVEBGLOGO2;
	}

	else if ((g_posMovebgLogo3.y - SCREEN_HEIGHT) >= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo3 = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
	}

	if ((g_posMovebgLogo4.y - SCREEN_HEIGHT) <= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo4.y += MOVE_MOVEBGLOGO2;
	}

	else if ((g_posMovebgLogo4.y - SCREEN_HEIGHT) >= SCREEN_HEIGHT / 2)
	{
		g_posMovebgLogo4 = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
	}
}

//-----------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------
void DrawMovebg(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMovebg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_PATTERN; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureMovebg[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// タイトル画面に貼るテクスチャの指定
//-----------------------------------------------------------------
void SetVertexTextureMovebg(int nCntTitle)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMovebg->Lock(0, 0, (void**)&pVtx, 0);

	if (nCntTitle == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntTitle == 1)
	{
		pVtx[4].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}

	else if (nCntTitle == 2)
	{
		pVtx[8].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}

	else if (nCntTitle == 3)
	{
		pVtx[12].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[13].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[14].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMovebg->Unlock();
}