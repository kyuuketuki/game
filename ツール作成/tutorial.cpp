//-----------------------------------------------------------------
//
// 背景 (bg.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "tutorial.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureTutorial[2] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;//頂点情報
int g_nTutorial;

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_nTutorial = 0;

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ゲーム画面.jpg", &g_pTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/操作説明.jpg", &g_pTextureTutorial[1]);
	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	return S_OK;
}
//==================
//ポリゴンの終了処理
//==================
void UninitTutorial(void)
{
	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		//テクスチャの開放
		if (g_pTextureTutorial[nCntTexture] != NULL)
		{
			g_pTextureTutorial[nCntTexture]->Release();
			g_pTextureTutorial[nCntTexture] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

}
//==================
//ポリゴンの更新処理
//==================
void UpdateTutorial(void)
{
	if (IsButtonDown(KEYINFO_B) || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_nTutorial++;
	}

	if (g_nTutorial == 1)
	{
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報のポインタを取得
		g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[4].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
		pVtx[5].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//rhwの設定
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点情報の設定
		pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点バッファをアンロック
		g_pVtxBuffTutorial->Unlock();
	}

	if (g_nTutorial == 2)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//==================
//ポリゴンの描画処理
//==================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[nCntTexture]);

		//ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

