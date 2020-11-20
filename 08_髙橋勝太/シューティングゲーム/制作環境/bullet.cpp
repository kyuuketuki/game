//-----------------------------------------------------------------
//
// bullhet (bullet.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "bullet.h"
#include "Explosion.h"
#include "score.h"
#include "sound.h"
#include "enemy.h"
#include "player.h"
#include "game.h"
#include "fade.h"
#include "boss.h"
#include "effect.h"
#include "result.h"
#include "tutorial.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet[3] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];//弾の情報


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\10.png", &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\10.png", &g_pTextureBullet[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\10.png", &g_pTextureBullet[2]);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 10;
		g_aBullet[nCntBullet].nType = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標
		SetVertexBullet(nCntBullet);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		////頂点のカラー設定
		//pVtx[0].col = D3DCOLOR_RGBA(255, 255, 0, 0);
		//pVtx[1].col = D3DCOLOR_RGBA(255, 255, 0, 0);
		//pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 0);
		//pVtx[3].col = D3DCOLOR_RGBA(255, 255, 0, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 255, 255, 255);

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock(); //アンロック

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int nCount = 0; nCount < MAX_TYPE_ENEMY; nCount++)
	{
		//テクスチャの開放
		if (g_pTextureBullet != NULL)
		{
			g_pTextureBullet[nCount]->Release();
			g_pTextureBullet[nCount] = NULL;
		}
	}

	//バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	BULLET *pBullet;
	pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{

		if (pBullet->bUse == true)
		{
			D3DXVECTOR3 move;
			move.x = sinf(D3DX_PI)*4.5;
			move.y = -cosf(D3DX_PI)*4.5;
			SetEffect(pBullet->pos, 2, move, pBullet->nType);

			//位置の更新
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			//位置の更新
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			//頂点座標
			SetVertexBullet(nCntBullet);

			//寿命チェック
			g_aBullet[nCntBullet].nLife--;
			if (g_aBullet[nCntBullet].nLife == 0)
			{
				//SetExplosion(pBullet->pos);
				g_aBullet[nCntBullet].bUse = false;
			}

			//画面外チェック
			if (pBullet->pos.y < -10)
			{
				pBullet->bUse = false;
			}

			//SetEffect(pBullet->pos,5);

			//敵との当たり判定
			ENEMY *pEnemy;
			pEnemy = GetEnemy();

			PLAYER *pPlayer;
			pPlayer = GetPlayer();

			//プレイヤー当たり判定
			if (g_aBullet[nCntBullet].nType == 1 || g_aBullet[nCntBullet].nType == 2)
			{
				if (pPlayer->bUse == true)
				{
					if (pBullet->pos.y > pPlayer->Pos.y - 15 && pBullet->pos.y < pPlayer->Pos.y + 15 &&
						pBullet->pos.x > pPlayer->Pos.x - 15 && pBullet->pos.x < pPlayer->Pos.x + 15)
					{
						//爆発音
						SetExplosion(pPlayer->Pos);

						//プレイヤー消滅
						pPlayer->bUse = false;

						//画面切り替え
						SetFade(FADE_OUT, MODE_RESULT);
					}
				}
			}

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (g_aBullet[nCntBullet].nType == 0)
				{
					if (pEnemy->bUse == true)
					{
						if (pBullet->pos.y > pEnemy->Pos.y - MAX_ENEMY_SIZE && pBullet->pos.y < pEnemy->Pos.y + MAX_ENEMY_SIZE &&
							pBullet->pos.x > pEnemy->Pos.x - MAX_ENEMY_SIZE && pBullet->pos.x < pEnemy->Pos.x + MAX_ENEMY_SIZE)
						{
							//爆発音
							//SetExplosion(pEnemy->Pos);

							//サウンド
							//PlaySound(SOUND_LABEL_SE_EXPLOSION);

							//スコア加算
							//AddScore(100);

							//弾消える
							pBullet->bUse = false;

							HitEnemy(nCntEnemy, 1);

							//画面切り替え
							//SetFade(FADE_OUT, MODE_RESULT);
						}
					}
				}
			}

			//敵との当たり判定
			BOSS *pBoss;
			pBoss = GetBoss();

			for (int nCntEnemy = 0; nCntEnemy < MAX_BOSS; nCntEnemy++, pBoss++)
			{
				if (g_aBullet[nCntBullet].nType == 0)
				{
					if (pBoss->bUse == true)
					{
						if (pBullet->pos.y > pBoss->Pos.y - MAX_BOSS_SIZE && pBullet->pos.y < pBoss->Pos.y + MAX_BOSS_SIZE &&
							pBullet->pos.x > pBoss->Pos.x - MAX_BOSS_SIZE && pBullet->pos.x < pBoss->Pos.x + MAX_BOSS_SIZE)
						{
							//爆発音
							SetExplosion(pBoss->Pos);

							HitBoss(nCntEnemy, 1);

							PlaySound(SOUND_LABEL_SE_EXPLOSION);

							//スコア加算
							AddScore(100);

							//弾消える
							pBullet->bUse = false;
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	BULLET *pBullet;

	//VERTEX_2D *pVtx;

	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBullet = &g_aBullet[0];

	//ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet[pBullet->nType]);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet[pBullet[nCntBullet].nType]);

		if (pBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// 弾の設定
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nType)
{
	BULLET *pBullet;

	VERTEX_2D *pVtx;

	pBullet = &g_aBullet[0];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//位置
			pBullet->pos = pos;

			//頂点座標
			SetVertexBullet(nCntBullet);

			//移動量
			pBullet->move = move;

			//寿命
			pBullet->nLife = nLife;

			pBullet->nType = nType;

			pBullet->bUse = true;

			break;
		}
		pVtx += 4;//4頂点ごとずらす
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------------------
// 弾の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 1.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 1.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 1.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-------------------------------
//弾の情報取得
//-------------------------------
BULLET * GetBullet(void)
{
	return &g_aBullet[0];
}