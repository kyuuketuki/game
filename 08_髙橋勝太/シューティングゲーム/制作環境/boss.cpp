//-----------------------------------------------------------------
//
// boss (boss.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "boss.h"
#include "sound.h"
#include "score.h"
#include "bullet.h"
#include "fade.h"
#include "appeared.h"

//------------------------------
//グローバル変数
//-------------------------------
LPDIRECT3DTEXTURE9 g_apTextureBoss[3] = {};//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;//頂点バッファへのポインタ
BOSS g_aBoss[256];//敵の情報
D3DXVECTOR3 g_posBoss;
D3DXVECTOR3 g_rotBoss;
D3DXVECTOR3 g_moveBoss;
float g_fLengthBoss;//対角線の長さ
float g_fAngleBoss;//対角線の角度
int g_nDataBoss = 1;//クリア条件

//---------------------------------------
//初期化処理
//----------------------------------------
HRESULT InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();


	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/01.png", &g_apTextureBoss[0]);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		g_aBoss[nCntEnemy].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntEnemy].nType = 0;
		g_aBoss[nCntEnemy].bUse = false;
		g_aBoss[nCntEnemy].state = BOSSSTATE_NOMAL;
		g_aBoss[nCntEnemy].nCounterState = 0;
		g_aBoss[nCntEnemy].nLife = 300;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 256,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x - MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y + MAX_BOSS_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x - MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y - MAX_BOSS_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x + MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y + MAX_BOSS_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x + MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y - MAX_BOSS_SIZE, 0.0f);

		////rhwの設定
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
		pVtx[2].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}
	//頂点バッファをアンロック
	g_pVtxBuffBoss->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//終了処理
//-----------------------------------------------------------------------------
void UninitBoss(void)
{
	//テクスチャの開放
	for (int nCount = 0; nCount < MAX_TYPE_ENEMY; nCount++)
	{
		if (g_apTextureBoss[nCount] != NULL)
		{
			g_apTextureBoss[nCount]->Release();
			g_apTextureBoss[nCount] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateBoss(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さ
	g_fLengthBoss = sqrtf(MAX_BOSS_SIZE * MAX_BOSS_SIZE + MAX_BOSS_SIZE * MAX_BOSS_SIZE);

	//対角線の角度
	g_fAngleBoss = atan2f(MAX_BOSS_SIZE, MAX_BOSS_SIZE);

	//敵アニメーション
	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		if (g_aBoss[nCntEnemy].bUse == true)
		{
			g_aBoss[nCntEnemy].g_nCounterAni++;

			if ((g_aBoss[nCntEnemy].g_nCounterAni % 5) == 0)
			{
				g_aBoss[nCntEnemy].g_nPatternAni++;

				g_aBoss[nCntEnemy].g_nseetAni = (g_aBoss[nCntEnemy].g_nseetAni + 1) % 10;

				//敵のアニメーション
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f*g_aBoss[nCntEnemy].g_nseetAni, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + 0.125f*g_aBoss[nCntEnemy].g_nseetAni, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.125f + 0.125f*g_aBoss[nCntEnemy].g_nseetAni, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f*g_aBoss[nCntEnemy].g_nseetAni, 0.0f);

				//弾の発射
				//SetBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x, g_aBoss[nCntEnemy].Pos.y + 50, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 300, 1);

			}

			int nCntBullet = rand() % 2;
			int nCntBullet2 = rand() % 3;

			if(nCntBullet == 1)
			{
				D3DXVECTOR3 move;
				D3DXVECTOR3 Pos;

				float fAngle;
				int nLife;

				//Pos = g_aBoss->Pos;
				nLife = 200;
				//fAngle = (float)(rand() % 157 - 78) / 100.0f;
				fAngle = (float)(rand() % 628 - 314) / 100.0f;
				move.x = sinf(fAngle)*2.5f;
				move.y = cosf(fAngle)*2.5f;
				//move.x = sinf(D3DX_PI)*3.5;
				//move.y = -cosf(D3DX_PI)*3.5;

				//SetEffect(Pos,nLife,move);

				//弾の発射
				//SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x, g_aEnemy[nCntEnemy].Pos.y + 50, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 300, 1);
				SetBullet(g_aBoss[nCntEnemy].Pos, move, nLife, 1);
			}

			if (nCntBullet2 == 1)
			{
				if (g_aBoss[nCntEnemy].nLife <= 200)
				{
					D3DXVECTOR3 move;
					D3DXVECTOR3 Pos;

					float fAngle;
					int nLife;

					//Pos = g_aBoss->Pos;
					nLife = 500;
					//fAngle = (float)(rand() % 157 - 78) / 100.0f;
					fAngle = (float)(rand() % 628 - 314) / 100.0f;
					move.x = sinf(fAngle)*1.0f;
					move.y = cosf(fAngle)*1.0f;
					//move.x = sinf(D3DX_PI)*3.5;
					//move.y = -cosf(D3DX_PI)*3.5;

					//SetEffect(Pos,nLife,move);

					//弾の発射
					//SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x, g_aEnemy[nCntEnemy].Pos.y + 50, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 300, 1);
					SetBullet(g_aBoss[nCntEnemy].Pos, move, nLife, 2);
				}
			}

			if (g_aBoss[nCntEnemy].nLife <= 150)
			{
				//画面切り替え
				//SetFade(FADE_OUT, MODE_RESULT);
				int nA = rand() % 1500;
				int nB = rand() % 1000;
				int nC = rand() % 200;
				int nCnt = rand() % 30;

				if (nCnt == 1)
				{
					SetAppeared(D3DXVECTOR3(nA, nB, 0.0f), 100, 30);
				}
			}

			//敵の位置データ
			pVtx[0].pos.x = g_aBoss[nCntEnemy].Pos.x + sinf(g_rotBoss.z - g_fAngleBoss) * g_fLengthBoss;
			pVtx[0].pos.y = g_aBoss[nCntEnemy].Pos.y + cosf(g_rotBoss.z - g_fAngleBoss) * g_fLengthBoss;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBoss[nCntEnemy].Pos.x - sinf(g_rotBoss.z + g_fAngleBoss) * g_fLengthBoss;
			pVtx[1].pos.y = g_aBoss[nCntEnemy].Pos.y - cosf(g_rotBoss.z + g_fAngleBoss) * g_fLengthBoss;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBoss[nCntEnemy].Pos.x + sinf(g_rotBoss.z + g_fAngleBoss) * g_fLengthBoss;
			pVtx[2].pos.y = g_aBoss[nCntEnemy].Pos.y + cosf(g_rotBoss.z + g_fAngleBoss) * g_fLengthBoss;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBoss[nCntEnemy].Pos.x - sinf(g_rotBoss.z - g_fAngleBoss) * g_fLengthBoss;
			pVtx[3].pos.y = g_aBoss[nCntEnemy].Pos.y - cosf(g_rotBoss.z - g_fAngleBoss) * g_fLengthBoss;
			pVtx[3].pos.z = 0.0f;



			switch (g_aBoss[nCntEnemy].state)
			{
			case ENEMYSTATE_DAMAGE:
				g_aBoss[nCntEnemy].nCounterState--;
				if (g_aBoss[nCntEnemy].nCounterState <= 0)
				{
					g_aBoss[nCntEnemy].state = BOSSSTATE_NOMAL;

					//頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;

			}
		}
		pVtx += 4;//4頂点ごとずらす
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBoss->Unlock();
}

//==================
//ボスの描画処理
//==================
void DrawBoss(void)
{
	BOSS *pBoss;

	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pBoss = &g_aBoss[0];

	//ポリゴンの描画
	for (int nCutBoss = 0; nCutBoss < 256; nCutBoss++, pBoss++)
	{
		pDevice->SetTexture(0, g_apTextureBoss[pBoss->nType]);

		if (pBoss->bUse == true)//弾を使用しているかどうか
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCutBoss * 4, 2);
		}
	}
}

void SetBoss(D3DXVECTOR3 Pos, int nType)
{
	BOSS *pBoss;

	VERTEX_2D *pVtx;

	pBoss = &g_aBoss[0];

	// 頂点バッファをロック
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pBoss++)
	{
		if (pBoss->bUse == false)//弾を使用しているかどうか
		{
			//位置設定
			g_aBoss[nCntEnemy].Pos = Pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x - MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y + MAX_BOSS_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x - MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y - MAX_BOSS_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x + MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y + MAX_BOSS_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntEnemy].Pos.x + MAX_BOSS_SIZE, g_aBoss[nCntEnemy].Pos.y - MAX_BOSS_SIZE, 0.0f);

			pBoss->nType = nType;

			//弾の使用
			pBoss->bUse = true;

			break;
		}

		pVtx += 4;//4頂点ごとずらす

	}
	// 頂点バッファをアンロック
	g_pVtxBuffBoss->Unlock();

}

//----------------------------
//ボス情報の取得
//----------------------------
BOSS *GetBoss(void)
{
	return &g_aBoss[0];
}

//----------------------------
//ボス当たり状態
//----------------------------
bool HitBoss(int nIdx, int nDamage)
{
	BOSS *pBoss;
	pBoss = &g_aBoss[nIdx];
	VERTEX_2D *pVtx;

	g_aBoss[nIdx].nLife -= nDamage;

	if (g_aBoss[nIdx].nLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		AddScore(10000);

		//敵の数
		g_nDataBoss--;

		g_aBoss[nIdx].bUse = false;

		//爆発の更新処理
		//UpdateExplosion();

		SetExplosion(pBoss->Pos);

		if (g_nDataBoss == 0)
		{
			//ボスの出現
			//SetBoss(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), 0);

			//画面切り替え
			SetFade(FADE_OUT, MODE_RESULT);
		}

		return true;
	
	}
	else
	{
		AddScore(10);

		g_aBoss[nIdx].state = BOSSSTATE_DAMAGE;
		g_aBoss[nIdx].nCounterState = 10; //ダメージ状態の時間

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nIdx * 4;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		// 頂点バッファをアンロックする
		g_pVtxBuffBoss->Unlock();
	}
	return false;
}

//クリア条件を取得
int GetBossNomber(void)
{
	return g_nDataBoss;
}
