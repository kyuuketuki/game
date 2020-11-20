//-----------------------------------------------------------------
//
// effect (effect.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "Effect.h"

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	int nCounterAnimEx;						//アニメーションカウンター
	int nPatternAnimEx;						//アニメーションパターンNo.
	bool bUse;								//爆発の使用状況
	D3DXVECTOR3 move;
	int nType;
	int nLife;
}EFFECT;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
EFFECT g_aEffect[MAX_EFFECT];							//爆発の情報

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitEffect(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.jpg", &g_pTextureEffect);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EFFECT; nCntExplosion++)
	{
		g_aEffect[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntExplosion].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntExplosion].nCounterAnimEx = 0;
		g_aEffect[nCntExplosion].nPatternAnimEx = 0;
		g_aEffect[nCntExplosion].bUse = false;
		g_aEffect[nCntExplosion].nType = 0;
		g_aEffect[nCntExplosion].nLife = 0;

	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標
		SetVertexEffect(nCntEffect);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		/*pVtx[0].col = D3DCOLOR_RGBA(0, 0, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 0, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 0, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 0, 255, 255);*/

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock(); //アンロック

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitEffect(void)
{
	//バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
	//テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateEffect(void)
{
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			pEffect->pos += pEffect->move;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - EFFECT_SIZE, pEffect->pos.y + EFFECT_SIZE, 1.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - EFFECT_SIZE, pEffect->pos.y - EFFECT_SIZE, 1.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + EFFECT_SIZE, pEffect->pos.y + EFFECT_SIZE, 1.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + EFFECT_SIZE, pEffect->pos.y - EFFECT_SIZE, 1.0f);

			pEffect->nLife--;
			if (pEffect->nLife == 0)
			{
				pEffect->bUse = false;
			}

			pVtx += 4;
		}
	}

	g_pVtxBuffEffect->Unlock();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawEffect(void)
{


	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	int nCntVertex = 0;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//ポリゴンの描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_EFFECT; nCntExplosion++)
	{
		if (g_aEffect[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//-----------------------------------------------------------------------------
// 爆発の設定
//-----------------------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 move,int nType)
{
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EFFECT; nCntExplosion++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			pEffect->pos = pos;

			pEffect->nLife = nLife;

			pEffect->nType = nType;

			if (nType == 0)
			{
				//頂点のカラー設定
				pVtx[0].col = D3DCOLOR_RGBA(0, 0, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(0, 0, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(0, 0, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(0, 0, 255, 255);
			}

			if (nType == 1)
			{
				//頂点のカラー設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}

			if (nType == 2)
			{
				//頂点のカラー設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 255, 255);
			}

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - EFFECT_SIZE, pEffect->pos.y + EFFECT_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - EFFECT_SIZE, pEffect->pos.y - EFFECT_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + EFFECT_SIZE, pEffect->pos.y + EFFECT_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + EFFECT_SIZE, pEffect->pos.y - EFFECT_SIZE, 0.0f);

			pEffect->bUse = true;
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//-----------------------------------------------------------------------------
// 爆発の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexEffect(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);



	////頂点座標
	//pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - EXPLOSION_SIZE, g_aEffect[nIdx].pos.y + EXPLOSION_SIZE, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - EXPLOSION_SIZE, g_aEffect[nIdx].pos.y - EXPLOSION_SIZE, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + EXPLOSION_SIZE, g_aEffect[nIdx].pos.y + EXPLOSION_SIZE, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + EXPLOSION_SIZE, g_aEffect[nIdx].pos.y - EXPLOSION_SIZE, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
