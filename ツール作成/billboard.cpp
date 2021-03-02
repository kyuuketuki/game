//================================================
//
//壁の処理[Billboard.h]
// Author; takahashi shouta
//
//================================================
#include "Billboard.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;//テクスチャ情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;//頂点情報
D3DXMATRIX g_mtxWorldBillboard;//ワールドマトリックス
BILLBOARD g_Billboard[MAX_BILLBOARD];

//====================
//ビルボードの初期化処理
//====================
HRESULT InitBillboard(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureBillboard);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//位置
		g_Billboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//使われているか
		g_Billboard[nCntBillboard].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-5, 5, 0);
	pVtx[1].pos = D3DXVECTOR3(5, 5, 0);
	pVtx[2].pos = D3DXVECTOR3(-5, -5, 0);
	pVtx[3].pos = D3DXVECTOR3(5, -5, 0);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();

	return S_OK;
}

//==================
//ビルボードの終了処理
//==================
void UninitBillboard(void)
{
	//テクスチャの開放
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}
//==================
//ビルボードの更新処理
//==================
void UpdateBillboard(void)
{

}

//====================
//ビルボードの描画処理
//====================
void DrawBillboard(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//ビューマトリックス取得用
	D3DXMATRIX mtxView;

	//計算用マトリックス
	D3DXMATRIX mtxTrans;

	//デバイスの取得
	pDevice = GetDevice();

	//ライティングを無効する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//使用されているとき
		if (g_Billboard[nCntBillboard].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBillboard);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);//逆行列を求める
			g_mtxWorldBillboard._41 = 0.0f;
			g_mtxWorldBillboard._42 = 0.0f;
			g_mtxWorldBillboard._43 = 0.0f;

			//位置の反転
			D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCntBillboard].pos.x, g_Billboard[nCntBillboard].pos.y, g_Billboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	//ライティングを有効する
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void SetBillboard(D3DXVECTOR3 pos)
{
	BILLBOARD *pBillboard;

	VERTEX_3D *pVtx;

	pBillboard = &g_Billboard[0];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (g_Billboard[nCntBillboard].bUse == false)
		{
			//位置
			pBillboard->pos = pos;

			//使用する
			pBillboard->bUse = true;

			break;
		}
		pVtx += 4;//4頂点ごとずらす
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//-----------------------------------------------------------------------------
// 弾の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexBillboard(int nIdx)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}