//-----------------------------------------------------------------
//
// スコアの処理 (score.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "score.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureScore = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//頂点情報
D3DXVECTOR3 g_PosScore;
SCORE g_aScore[256];
int g_nScore;//スコア

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//スコアリセット
	g_nScore = 0;

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		//g_aScore[nCntEnemy].pos = D3DXVECTOR3(200.0f, 50.0f, 0.0f);
		//g_aScore[nCntEnemy].pos = D3DXVECTOR3(200.0f, 50.0f, 0.0f);
		g_aScore[nCntEnemy].nType = 0;
		g_aScore[nCntEnemy].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 256, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < 256; nCntBullet++, pVtx += 4)
	{
		//頂点座標
		//pVtx[0].pos = D3DXVECTOR3(g_aScore[nCntBullet].pos.x - SCORE_SIZE, g_aScore[nCntBullet].pos.y + SCORE_SIZE, 0);
		//pVtx[1].pos = D3DXVECTOR3(g_aScore[nCntBullet].pos.x - SCORE_SIZE, g_aScore[nCntBullet].pos.y - SCORE_SIZE, 0);
		//pVtx[2].pos = D3DXVECTOR3(g_aScore[nCntBullet].pos.x + SCORE_SIZE, g_aScore[nCntBullet].pos.y + SCORE_SIZE, 0);
		//pVtx[3].pos = D3DXVECTOR3(g_aScore[nCntBullet].pos.x + SCORE_SIZE, g_aScore[nCntBullet].pos.y - SCORE_SIZE, 0);

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

		//頂点情報の設定(ランニングマン)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);
	}
	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//==================
//ポリゴンの終了処理
//==================
void UninitScore(void)
{
	//テクスチャの開放
	for (int nCntEnemyType = 0; nCntEnemyType < 256; nCntEnemyType++)
	{
		if (g_pTextureScore != NULL)
		{
			g_pTextureScore->Release();
			g_pTextureScore = NULL;
		}
	}
	//バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
	
}


//==================
//ポリゴンの更新処理
//==================
void UpdateScore(void)
{
	VERTEX_2D *pVtx;
	int nNumber[8];

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	nNumber[0] = g_nScore % 100000000 / 10000000;
	nNumber[1] = g_nScore % 10000000 / 1000000;
	nNumber[2] = g_nScore % 1000000 / 100000;
	nNumber[3] = g_nScore % 100000 / 10000;
	nNumber[4] = g_nScore % 10000 / 1000;
	nNumber[5] = g_nScore % 1000 / 100;
	nNumber[6] = g_nScore % 100 / 10;
	nNumber[7] = g_nScore % 10 / 1;

	for (int nCutEnemy = 0; nCutEnemy < 8; nCutEnemy++)
	{
		pVtx[0].pos = D3DXVECTOR3((g_aScore[nCutEnemy].pos.x - SCORE_SIZE) + nCutEnemy * 10, g_aScore[nCutEnemy].pos.y + SCORE_SIZE, 0);
		pVtx[1].pos = D3DXVECTOR3((g_aScore[nCutEnemy].pos.x - SCORE_SIZE) + nCutEnemy * 10, g_aScore[nCutEnemy].pos.y - SCORE_SIZE, 0);
		pVtx[2].pos = D3DXVECTOR3((g_aScore[nCutEnemy].pos.x + SCORE_SIZE) + nCutEnemy * 10, g_aScore[nCutEnemy].pos.y + SCORE_SIZE, 0);
		pVtx[3].pos = D3DXVECTOR3((g_aScore[nCutEnemy].pos.x + SCORE_SIZE) + nCutEnemy * 10, g_aScore[nCutEnemy].pos.y - SCORE_SIZE, 0);


		//頂点情報の設定(ランニングマン)
		pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber[nCutEnemy], 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber[nCutEnemy], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber[nCutEnemy]+1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber[nCutEnemy]+1), 0.0f);

		pVtx += 4;
	}
}

//==================
//ポリゴンの描画処理
//==================
void DrawScore(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	int nCntVertex = 0;

	SCORE *pScore;
	pScore = &g_aScore[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < 256; nCntBullet++, pScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		if (g_aScore[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		
		nCntVertex += 4;
	}
}

void AddScore(int nValue)
{
	g_nScore += nValue;

}

//-----------------------------------------------------------------------------
// 敵の設定
//-----------------------------------------------------------------------------
void SetScore(D3DXVECTOR3 pos, int nType)
{
	SCORE *pScore;
	pScore = &g_aScore[0];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pScore++)
	{
		if (pScore->bUse == false)
		{

			pScore->pos = pos;

			//頂点座標
			SetVertexScore(nCntEnemy);

			pScore->nType = nType;

			pScore->bUse = true;
			break;
		}
		g_pVtxBuffScore->Unlock(); //アンロック
		//pVtx += 4;
	}
}

//-----------------------------------------------------------------------------
// 敵の習得
//-----------------------------------------------------------------------------
SCORE*GetScore(void)
{
	return &g_aScore[0];
}

//-----------------------------------------------------------------------------
// 敵の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexScore(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aScore[nIdx].pos.x - SCORE_SIZE, g_aScore[nIdx].pos.y + SCORE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aScore[nIdx].pos.x - SCORE_SIZE, g_aScore[nIdx].pos.y - SCORE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aScore[nIdx].pos.x + SCORE_SIZE, g_aScore[nIdx].pos.y + SCORE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aScore[nIdx].pos.x + SCORE_SIZE, g_aScore[nIdx].pos.y - SCORE_SIZE, 0.0f);

	//頂点情報の設定(ランニングマン)
	//pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber[nCutEnemy], 1.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber[nCutEnemy], 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber[nCutEnemy] + 1), 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber[nCutEnemy] + 1), 0.0f);

	//頂点情報の設定(ランニングマン)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

