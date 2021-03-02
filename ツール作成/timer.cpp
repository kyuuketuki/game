//-----------------------------------------------------------------
//
// スコアの処理 (score.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "timer.h"

#define  MAX_TIMER (4)

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureTimer = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;//頂点情報
D3DXVECTOR3 g_PosTimer;
TIMER g_aTimer[MAX_TIMER];
int g_nTimer;//スコア
int g_nTimerCnt;//スコア

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTimer);

	//スコアリセット
	g_nTimer = 0;
	g_nTimerCnt = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_TIMER; nCntEnemy++)
	{
		g_aTimer[nCntEnemy].nType = 0;
		g_aTimer[nCntEnemy].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimer, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_TIMER; nCntBullet++, pVtx += 4)
	{
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);
	}
	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();

	return S_OK;
}

//==================
//ポリゴンの終了処理
//==================
void UninitTimer(void)
{
	//テクスチャの開放
	for (int nCntEnemyType = 0; nCntEnemyType < MAX_TIMER; nCntEnemyType++)
	{
		if (g_pTextureTimer != NULL)
		{
			g_pTextureTimer->Release();
			g_pTextureTimer = NULL;
		}
	}
	//バッファの開放
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}

}


//==================
//ポリゴンの更新処理
//==================
void UpdateTimer(void)
{
	VERTEX_2D *pVtx;
	int nNumber2[MAX_TIMER];

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	nNumber2[0] = g_nTimer % 6000 / 1000;
	nNumber2[1] = g_nTimer % 1000 / 60;
	nNumber2[2] = g_nTimer % 60 / 10;
	nNumber2[3] = g_nTimer % 10 / 1;

	for (int nCutEnemy = 0; nCutEnemy < MAX_TIMER; nCutEnemy++)
	{
		pVtx[0].pos = D3DXVECTOR3((g_aTimer[nCutEnemy].pos.x - TIMER_SIZE) + nCutEnemy * 10, g_aTimer[nCutEnemy].pos.y + TIMER_SIZE, 0);
		pVtx[1].pos = D3DXVECTOR3((g_aTimer[nCutEnemy].pos.x - TIMER_SIZE) + nCutEnemy * 10, g_aTimer[nCutEnemy].pos.y - TIMER_SIZE, 0);
		pVtx[2].pos = D3DXVECTOR3((g_aTimer[nCutEnemy].pos.x + TIMER_SIZE) + nCutEnemy * 10, g_aTimer[nCutEnemy].pos.y + TIMER_SIZE, 0);
		pVtx[3].pos = D3DXVECTOR3((g_aTimer[nCutEnemy].pos.x + TIMER_SIZE) + nCutEnemy * 10, g_aTimer[nCutEnemy].pos.y - TIMER_SIZE, 0);

		//頂点情報の設定(ランニングマン)
		pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber2[nCutEnemy], 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber2[nCutEnemy], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber2[nCutEnemy] + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber2[nCutEnemy] + 1), 0.0f);

		pVtx += 4;
	}
}

//==================
//ポリゴンの描画処理
//==================
void DrawTimer(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	int nCntVertex = 0;

	TIMER *pTimer;
	pTimer = &g_aTimer[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < MAX_TIMER; nCntBullet++, pTimer++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTimer);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		if (g_aTimer[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}

		nCntVertex += 4;
	}
}

///=================
//加算処理
//==================
void AddTimer(int nValue)
{
	g_nTimerCnt += nValue;
	if ((g_nTimerCnt % 60) == 0)
	{
		g_nTimer += 1;
	}

}

//-----------------------------------------------------------------------------
// 時間の設定
//-----------------------------------------------------------------------------
void SetTimer(D3DXVECTOR3 pos, int nType)
{
	TIMER *pTimer;
	pTimer = &g_aTimer[0];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_TIMER; nCntEnemy++, pTimer++)
	{
		if (pTimer->bUse == false)
		{

			pTimer->pos = pos;

			//頂点座標
			SetVertexTimer(nCntEnemy);

			pTimer->nType = nType;

			pTimer->bUse = true;
			break;
		}
		g_pVtxBuffTimer->Unlock(); //アンロック
	}
}

//-----------------------------------------------------------------------------
// 敵の習得
//-----------------------------------------------------------------------------
int GetTimer(void)
{
	return g_nTimer;
}

//-----------------------------------------------------------------------------
// 敵の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexTimer(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aTimer[nIdx].pos.x - TIMER_SIZE, g_aTimer[nIdx].pos.y + TIMER_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTimer[nIdx].pos.x - TIMER_SIZE, g_aTimer[nIdx].pos.y - TIMER_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTimer[nIdx].pos.x + TIMER_SIZE, g_aTimer[nIdx].pos.y + TIMER_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTimer[nIdx].pos.x + TIMER_SIZE, g_aTimer[nIdx].pos.y - TIMER_SIZE, 0.0f);

	//頂点情報の設定(ランニングマン)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}


