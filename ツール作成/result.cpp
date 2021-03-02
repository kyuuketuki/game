//-----------------------------------------------------------------
//
// リザルト画面 (result.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "result.h"
#include "input.h"
#include "fade.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (3)										// アニメーションパターンNo.の最大数
#define TITLELOGO_X		(1000.0f)								// タイトルロゴ幅
#define TITLELOGO_Y		(200.0f)								// タイトルロゴの高さ
#define HEIGHT_Y		(500.0f * -1.0f)						// タイトルロゴの初期中心Y座標
#define PRESSENTER_X	(800.0f)								// プレスエンターの幅
#define PRESSENTER_Y	(300.0f)								// プレスエンターの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResult[3] = {};
D3DXVECTOR3 g_posResultLogo;						// タイトルロゴの中心座標
D3DXVECTOR3 g_posPressEnterResult;					// プレスエンターの中心座標
D3DXCOLOR g_Resultcolor;
RESULT  g_aResult[256];
float g_nCntResult;
int g_nCntResult2;
int g_resultclear;

//-----------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------
HRESULT InitResult(void)
{
	g_resultclear = 0;

	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 変数の初期化
	g_posResultLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_posPressEnterResult = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4), 0.0f);
	g_Resultcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntResult = 0;

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/space_1.png", &g_pTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear.png", &g_pTextureResult[1]);

	for (int nCntResult = 0; nCntResult < 256; nCntResult++)
	{
		g_aResult[nCntResult].nType = 0;
		g_aResult[nCntResult].nData = 0;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_PATTERN; nCntTitle++)
	{
		// 頂点座標の設定
		SetVertexTextureResult(nCntTitle);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_Resultcolor;
		pVtx[1].col = g_Resultcolor;
		pVtx[2].col = g_Resultcolor;
		pVtx[3].col = g_Resultcolor;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------
void UninitResult(void)
{
	g_resultclear += 1;

	// 頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		if (g_pTextureResult[nCntTexture] != NULL)
		{
			g_pTextureResult[nCntTexture]->Release();
			g_pTextureResult[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------
void UpdateResult(void)
{
	//if (GetKeyboardPress(DIK_RETURN) == true)
	if (IsButtonDown(KEYINFO_B) || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//画面切り替え
		//SetMode(MODE_TITLE);
		SetMode(MODE_RANKING);
	}
}

//-----------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------
void DrawResult(void)
{
	RESULT *pResult;

	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	pResult = &g_aResult[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}


	/*	pDevice->SetTexture(0, g_pTextureResult[pResult->nData]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCutEnemy * 4, 2);*/

}

//-----------------------------------------------------------------
// タイトル画面に貼るテクスチャの指定
//-----------------------------------------------------------------
void SetVertexTextureResult(int nCntResult)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	g_nCntResult2 = nCntResult;

	if (g_nCntResult2 == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}

	else if (g_nCntResult2 == 1)
	{
		pVtx[4].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	}

	/*else if (g_nCntResult2 == 2)
	{
	pVtx[8].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	}*/

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}

void SetResult(int nData)
{
	RESULT  *pResult;

	VERTEX_2D *pVtx;

	pResult = &g_aResult[0];

	// 頂点バッファをロック
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	/*if (nData == 0)
	{
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);

	pVtx[8].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pResult->nData = nData;
	}

	else if (nData == 1)
	{
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);

	pVtx[8].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posPressEnterResult.x - (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y + (PRESSENTER_Y / 2), 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posPressEnterResult.x + (PRESSENTER_X / 2), g_posPressEnterResult.y - (PRESSENTER_Y / 2), 0.0f);
	pResult->nData = nData;
	}*/
	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}

int GetResultclear(void)
{
	return g_resultclear;
}