//-----------------------------------------------------------------
//
// explosion (explosion.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "appeared.h"

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	int nCounterAnimEx;						//アニメーションカウンター
	int nPatternAnimEx;						//アニメーションパターンNo.
	bool bUse;								//爆発の使用状況
	int nCut;
}APPEARED;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureAppeared = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAppeared = NULL;
APPEARED g_Appeared[MAX_APPEARED];							//爆発の情報
int g_nAPPEARED_SIZE;//サイズ変更

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitAppeared(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\02.png", &g_pTextureAppeared);

	for (int nCntExplosion = 0; nCntExplosion < MAX_APPEARED; nCntExplosion++)
	{
		g_Appeared[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Appeared[nCntExplosion].nCounterAnimEx = 0;
		g_Appeared[nCntExplosion].nPatternAnimEx = 0;
		g_Appeared[nCntExplosion].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_APPEARED, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffAppeared, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAppeared->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_APPEARED; nCntExplosion++)
	{
		//頂点座標
		SetVertexAppeared(nCntExplosion);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.125f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 0.125f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffAppeared->Unlock(); //アンロック

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitAppeared(void)
{
	//バッファの開放
	if (g_pVtxBuffAppeared != NULL)
	{
		g_pVtxBuffAppeared->Release();
		g_pVtxBuffAppeared = NULL;
	}
	//テクスチャの開放
	if (g_pVtxBuffAppeared != NULL)
	{
		g_pVtxBuffAppeared->Release();
		g_pVtxBuffAppeared = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateAppeared(void)
{
	APPEARED *pAppeared;
	pAppeared = &g_Appeared[0];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAppeared->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_APPEARED; nCntBullet++, pAppeared++)
	{
		if (pAppeared->bUse == true)
		{

			pAppeared->nCounterAnimEx++;//アニメーションカウンター更新
			if ((pAppeared->nCounterAnimEx % pAppeared->nCut) == 0)
			{
				pAppeared->nPatternAnimEx = (pAppeared->nPatternAnimEx + 1) % 8;//アニメーションパターンNo.更新
			}

			//頂点座標
			SetVertexAppeared(nCntBullet);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * pAppeared->nPatternAnimEx), 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f * pAppeared->nPatternAnimEx), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f * pAppeared->nPatternAnimEx), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * pAppeared->nPatternAnimEx), 0.0f);

			if (pAppeared->nPatternAnimEx == 7)
			{
				pAppeared->bUse = false;
				pAppeared->nPatternAnimEx = 0;
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffAppeared->Unlock();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawAppeared(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	int nCntVertex = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAppeared, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureAppeared);

	//ポリゴンの描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_APPEARED; nCntExplosion++)
	{
		if (g_Appeared[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// 爆発の設定
//-----------------------------------------------------------------------------
void SetAppeared(D3DXVECTOR3 pos,int size,int nCut)
{
	APPEARED *pAppeared;
	pAppeared = &g_Appeared[0];

	for (int nCntExplosion = 0; nCntExplosion < MAX_APPEARED; nCntExplosion++, pAppeared++)
	{
		if (pAppeared->bUse == false)
		{
			pAppeared->pos = pos;

			pAppeared->nCut = nCut;

			g_nAPPEARED_SIZE = size;

			//頂点座標
			SetVertexAppeared(nCntExplosion);

			pAppeared->bUse = true;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 爆発の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexAppeared(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAppeared->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_Appeared[nIdx].pos.x - (APPEARED_SIZE + g_nAPPEARED_SIZE), g_Appeared[nIdx].pos.y + (APPEARED_SIZE + g_nAPPEARED_SIZE), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Appeared[nIdx].pos.x - (APPEARED_SIZE + g_nAPPEARED_SIZE), g_Appeared[nIdx].pos.y - (APPEARED_SIZE + g_nAPPEARED_SIZE), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Appeared[nIdx].pos.x + (APPEARED_SIZE + g_nAPPEARED_SIZE), g_Appeared[nIdx].pos.y + (APPEARED_SIZE + g_nAPPEARED_SIZE), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Appeared[nIdx].pos.x + (APPEARED_SIZE + g_nAPPEARED_SIZE), g_Appeared[nIdx].pos.y - (APPEARED_SIZE + g_nAPPEARED_SIZE), 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffAppeared->Unlock();
}
