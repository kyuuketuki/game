//-----------------------------------------------------------------
//
// プレイヤーの処理 (player.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
//#include "player.h"
#include"camera.h"
#include "shadow.h"
#include "model.h"

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点情報
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//テクスチャ
SHADOW g_Shadow[MAX_SHADOW];

//-----------------------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitShadow(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	// 変数の初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].fWidth = 1.0f;
		g_Shadow[nCntShadow].fDepth = 1.0f;
		g_Shadow[nCntShadow].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++, pVtx += 4)
	{
		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_Shadow[nCnt].pos.x - g_Shadow[nCnt].fWidth, g_Shadow[nCnt].pos.y, g_Shadow[nCnt].pos.z - g_Shadow[nCnt].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_Shadow[nCnt].pos.x - g_Shadow[nCnt].fWidth, g_Shadow[nCnt].pos.y, g_Shadow[nCnt].pos.z + g_Shadow[nCnt].fDepth);
		pVtx[2].pos = D3DXVECTOR3(g_Shadow[nCnt].pos.x + g_Shadow[nCnt].fWidth, g_Shadow[nCnt].pos.y, g_Shadow[nCnt].pos.z - g_Shadow[nCnt].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_Shadow[nCnt].pos.x + g_Shadow[nCnt].fWidth, g_Shadow[nCnt].pos.y, g_Shadow[nCnt].pos.z + g_Shadow[nCnt].fDepth);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitShadow(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	// テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdateShadow(void)
{

}

//-----------------------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawShadow(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	SHADOW *pShadow;
	pShadow = &g_Shadow[0];

	//デバイスの取得
	pDevice = GetDevice();

	// 減算合成の生成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_Shadow[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// 向きを反映
			pShadow->mtxWorld._11 = pShadow->fWidth;
			pShadow->mtxWorld._33 = pShadow->fDepth;

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
//int SetShadow(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	SHADOW *pShadow;
	pShadow = &g_Shadow[0];
	int nIdx = -1;

	// 影の設定
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++, pShadow++)
	{
		if (pShadow->bUse == false)
		{
			pShadow->pos = pos;			// 位置の設定

			pShadow->fWidth = fWidth;	// 幅の設定

			pShadow->fDepth = fDepth;	// 奥行の設定

			pShadow->bUse = true;		// 使用状態への移行

			nIdx = nCnt;				// インデックスへの代入

			break;
		}
	}

	return nIdx;	// nIdx番目として返す
}

void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_Shadow[nIdx].pos = pos;

	//VERTEX_3D *pVtx;
	//SHADOW *pShadow;
	//pShadow = &g_Shadow[0];

	////頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	////pSpeed += nIdx;

	//pVtx += nIdx * 4;

	////頂点座標
	//pVtx[0].pos = D3DXVECTOR3(pos.x - SHADOW_SIZE, 0.0f, pos.z + SHADOW_SIZE);
	//pVtx[1].pos = D3DXVECTOR3(pos.x + SHADOW_SIZE, 0.0f, pos.z + SHADOW_SIZE);
	//pVtx[2].pos = D3DXVECTOR3(pos.x - SHADOW_SIZE, 0.0f, pos.z - SHADOW_SIZE);
	//pVtx[3].pos = D3DXVECTOR3(pos.x + SHADOW_SIZE, 0.0f, pos.z - SHADOW_SIZE);

	////頂点バッファをアンロックする
	//g_pVtxBuffShadow->Unlock();
}
