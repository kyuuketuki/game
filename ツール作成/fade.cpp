//-----------------------------------------------------------------
//
// fade (fade.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "fade.h"
#include "input.h"
//#include "result.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;//頂点情報
FADE g_fade;
MODE g_modeNext;
D3DXCOLOR g_colorFade;

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BG3.jpg", &g_pTextureFade);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	////頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(0, 500, 1.0f);//2Dは0.0固定
	//pVtx[1].pos = D3DXVECTOR3(0, 0, 1.0f);
	//pVtx[2].pos = D3DXVECTOR3(500, 500, 1.0f);
	//pVtx[3].pos = D3DXVECTOR3(500, 0, 1.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();

	return S_OK;
}
//==================
//ポリゴンの終了処理
//==================
void UninitFade(void)
{
	//テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}

}
//==================
//ポリゴンの更新処理
//==================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();

	if (g_fade == FADE_IN)
	{
		g_colorFade.a -= 0.005f;
		if (g_colorFade.a <= 0.0f)
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;
		}
		/*if (GetKeyboardPress(DIK_RETURN) == true)
		{
		g_colorFade.a = 0.0f;
		g_fade = FADE_NONE;
		}*/
	}

	else if (g_fade == FADE_OUT)
	{
		g_colorFade.a += 0.01f;
		if (g_colorFade.a >= 1.0f)
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;
			SetMode(g_modeNext);
		}
	}
}

//==================
//ポリゴンの描画処理
//==================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
}

void SetFade2(FADE fade, MODE modeNext, int nData)
{
	//SetResult(nData);
	g_fade = fade;
	g_modeNext = modeNext;
}

FADE GetFade(void)
{
	return FADE();
}

