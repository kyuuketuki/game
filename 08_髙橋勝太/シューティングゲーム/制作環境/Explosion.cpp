//-----------------------------------------------------------------
//
// explosion (explosion.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "Explosion.h"

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	int nCounterAnimEx;						//アニメーションカウンター
	int nPatternAnimEx;						//アニメーションパターンNo.
	bool bUse;								//爆発の使用状況
}EXPLOSION;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION];							//爆発の情報

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitExplosion(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion000.png", &g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnimEx = 0;
		g_aExplosion[nCntExplosion].nPatternAnimEx = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標
		SetVertexExplosion(nCntExplosion);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.125f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 0.125f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock(); //アンロック

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitExplosion(void)
{
	//バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
	//テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateExplosion(void)
{
	EXPLOSION *pExplosion;
	pExplosion = &g_aExplosion[0];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{

			pExplosion->nCounterAnimEx++;//アニメーションカウンター更新
			if ((pExplosion->nCounterAnimEx % 3) == 0)
			{
				pExplosion->nPatternAnimEx = (pExplosion->nPatternAnimEx + 1) % 8;//アニメーションパターンNo.更新
			}

			//頂点座標
			SetVertexExplosion(nCntBullet);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * pExplosion->nPatternAnimEx), 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f * pExplosion->nPatternAnimEx), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f * pExplosion->nPatternAnimEx), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * pExplosion->nPatternAnimEx), 0.0f);

			if (pExplosion->nPatternAnimEx == 7)
			{
				pExplosion->bUse = false;
				pExplosion->nPatternAnimEx = 0;
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawExplosion(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	int nCntVertex = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	//ポリゴンの描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// 爆発の設定
//-----------------------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos)
{
	EXPLOSION *pExplosion;
	pExplosion = &g_aExplosion[0];

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			pExplosion->pos = pos;

			//頂点座標
			SetVertexExplosion(nCntExplosion);

			pExplosion->bUse = true;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 爆発の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexExplosion(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
 	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y + EXPLOSION_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y - EXPLOSION_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y + EXPLOSION_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y - EXPLOSION_SIZE, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
