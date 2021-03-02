//================================================
//
//壁の処理[Bullet.h]
// Author; takahashi shouta
//
//================================================
#include "Bullet.h"
#include "model.h"
#include"effect.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//テクスチャ情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//頂点情報
D3DXMATRIX g_mtxWorldBullet;//ワールドマトリックス
BULLET g_Bullet[MAX_BULLET];

//====================
//ビルボードの初期化処理
//====================
HRESULT InitBullet(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//位置
		g_Bullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//寿命
		g_Bullet[nCntBullet].nLife = 0;

		//使われているか
		g_Bullet[nCntBullet].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE, BULLET_SIZE, 0);
	pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE, BULLET_SIZE, 0);
	pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, 0);
	pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE, -BULLET_SIZE, 0);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	//黄色
	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 255, 255);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//==================
//ビルボードの終了処理
//==================
void UninitBullet(void)
{
	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//==================
//ビルボードの更新処理
//==================
void UpdateBullet(void)
{
	BULLET *pBullet;
	pBullet = &g_Bullet[0];

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{
			//
			SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, -0.0f), 10, 1,10);

			//寿命を減らす
			pBullet->nLife--;

			//寿命チェック
			if (pBullet->nLife == 0)
			{
				pBullet->bUse = false;
			}
		}

		//pBullet->move.z -= 0.02;

		//位置の更新
		pBullet->pos.x += pBullet->move.x;
		pBullet->pos.y += pBullet->move.y;
		pBullet->pos.z += pBullet->move.z;
	}

	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//====================
//ビルボードの描画処理
//====================
void DrawBullet(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//ビューマトリックス取得用
	D3DXMATRIX mtxView;

	//計算用マトリックス
	D3DXMATRIX mtxTrans;

	//デバイスの取得
	pDevice = GetDevice();

	//Zバッファ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライティングを無効する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//使用されているとき
		if (g_Bullet[nCntBullet].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBullet);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldBullet, NULL, &mtxView);//逆行列を求める
			g_mtxWorldBullet._41 = 0.0f;
			g_mtxWorldBullet._42 = 0.0f;
			g_mtxWorldBullet._43 = 0.0f;

			//位置の反転
			D3DXMatrixTranslation(&mtxTrans, g_Bullet[nCntBullet].pos.x, g_Bullet[nCntBullet].pos.y, g_Bullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	//ライティングを有効する
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zバッファ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nSpeed, int nLife)
{
	BULLET *pBullet;

	VERTEX_3D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pBullet = &g_Bullet[0];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (g_Bullet[nCntBullet].bUse == false)
		{
			//位置
			pBullet->pos = pos;

			//移動量
			pBullet->move = -move *nSpeed;

			pBullet->nLife = nLife;

			//使用する
			pBullet->bUse = true;

			break;
		}
		pVtx += 4;//4頂点ごとずらす
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-------------------------------
//弾の情報取得
//-------------------------------
BULLET * GetBullet(void)
{
	return &g_Bullet[0];
}