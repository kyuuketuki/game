//-----------------------------------------------------------------
//
// スコアの処理 (score.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "gear.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureGear = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGear = NULL;//頂点情報
D3DXVECTOR3 g_PosGear;
GEAR g_aGear[256];
int g_nGear;//スコア

///====================
//ポリゴンの初期化処理
//====================
HRESULT InitGear(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureGear);

	//スコアリセット
	g_nGear = 0;

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		//g_aGear[nCntEnemy].pos = D3DXVECTOR3(200.0f, 50.0f, 0.0f);
		//g_aGear[nCntEnemy].pos = D3DXVECTOR3(200.0f, 50.0f, 0.0f);
		g_aGear[nCntEnemy].nType = 0;
		g_aGear[nCntEnemy].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 256, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGear, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < 256; nCntBullet++, pVtx += 4)
	{
		//頂点座標
		//pVtx[0].pos = D3DXVECTOR3(g_aGear[nCntBullet].pos.x - GEAR_SIZE, g_aGear[nCntBullet].pos.y + GEAR_SIZE, 0);
		//pVtx[1].pos = D3DXVECTOR3(g_aGear[nCntBullet].pos.x - GEAR_SIZE, g_aGear[nCntBullet].pos.y - GEAR_SIZE, 0);
		//pVtx[2].pos = D3DXVECTOR3(g_aGear[nCntBullet].pos.x + GEAR_SIZE, g_aGear[nCntBullet].pos.y + GEAR_SIZE, 0);
		//pVtx[3].pos = D3DXVECTOR3(g_aGear[nCntBullet].pos.x + GEAR_SIZE, g_aGear[nCntBullet].pos.y - GEAR_SIZE, 0);

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
	g_pVtxBuffGear->Unlock();

	return S_OK;
}

//==================
//ポリゴンの終了処理
//==================
void UninitGear(void)
{
	//テクスチャの開放
	for (int nCntEnemyType = 0; nCntEnemyType < 256; nCntEnemyType++)
	{
		if (g_pTextureGear != NULL)
		{
			g_pTextureGear->Release();
			g_pTextureGear = NULL;
		}
	}
	//バッファの開放
	if (g_pVtxBuffGear != NULL)
	{
		g_pVtxBuffGear->Release();
		g_pVtxBuffGear = NULL;
	}
}

//==================
//ポリゴンの更新処理
//==================
void UpdateGear(void)
{
	VERTEX_2D *pVtx;
	int nNumber;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	nNumber = g_nGear % 7 / 1;

	for (int nCutEnemy = 0; nCutEnemy < 1; nCutEnemy++)
	{
		pVtx[0].pos = D3DXVECTOR3((g_aGear[nCutEnemy].pos.x - GEAR_SIZE) + nCutEnemy * 10, g_aGear[nCutEnemy].pos.y + GEAR_SIZE, 0);
		pVtx[1].pos = D3DXVECTOR3((g_aGear[nCutEnemy].pos.x - GEAR_SIZE) + nCutEnemy * 10, g_aGear[nCutEnemy].pos.y - GEAR_SIZE, 0);
		pVtx[2].pos = D3DXVECTOR3((g_aGear[nCutEnemy].pos.x + GEAR_SIZE) + nCutEnemy * 10, g_aGear[nCutEnemy].pos.y + GEAR_SIZE, 0);
		pVtx[3].pos = D3DXVECTOR3((g_aGear[nCutEnemy].pos.x + GEAR_SIZE) + nCutEnemy * 10, g_aGear[nCutEnemy].pos.y - GEAR_SIZE, 0);


		//頂点情報の設定(ランニングマン)
		pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber + 1), 0.0f);

		//pVtx += 4;
	}
}

//==================
//ポリゴンの描画処理
//==================
void DrawGear(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	int nCntVertex = 0;

	GEAR *pGear;
	pGear = &g_aGear[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGear, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < 256; nCntBullet++, pGear++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGear);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		if (g_aGear[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}

		nCntVertex += 4;
	}
}

//-----------------------------------------------------------------------------
// カウント
//-----------------------------------------------------------------------------
void AddGear(int nValue)
{
	g_nGear = nValue;
}

//-----------------------------------------------------------------------------
// ギアの設置
//-----------------------------------------------------------------------------
void SetGear(D3DXVECTOR3 pos, int nType)
{
	GEAR *pGear;
	pGear = &g_aGear[0];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pGear++)
	{
		if (pGear->bUse == false)
		{

			pGear->pos = pos;

			//頂点座標
			//SetVertexGear(nCntEnemy);

			pGear->nType = nType;

			pGear->bUse = true;
			break;
		}
		g_pVtxBuffGear->Unlock(); //アンロック
								   //pVtx += 4;
	}
}

//-----------------------------------------------------------------------------
// 敵の習得
//-----------------------------------------------------------------------------
GEAR*GetGear(void)
{
	return &g_aGear[0];
}

//-----------------------------------------------------------------------------
// 敵の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexGear(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aGear[nIdx].pos.x -GEAR_SIZE, g_aGear[nIdx].pos.y + GEAR_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aGear[nIdx].pos.x -GEAR_SIZE, g_aGear[nIdx].pos.y - GEAR_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aGear[nIdx].pos.x +GEAR_SIZE, g_aGear[nIdx].pos.y + GEAR_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aGear[nIdx].pos.x +GEAR_SIZE, g_aGear[nIdx].pos.y - GEAR_SIZE, 0.0f);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffGear->Unlock();
}

