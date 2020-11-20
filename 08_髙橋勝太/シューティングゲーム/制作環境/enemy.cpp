//-----------------------------------------------------------------
//
// enemy (enemy.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//--------------------------------------------
//インクルードファイル
//--------------------------------------------
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "bullet.h"
#include "fade.h"
#include "boss.h"
#include "effect.h"
#include "bullet.h"
#include "appeared.h"

//----------------------------------------------
//グローバル変数
//----------------------------------------------
LPDIRECT3DTEXTURE9 g_apTextureEnemy[3] = {};//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;//頂点バッファへのポインタ
ENEMY g_aEnemy[256];//敵の情報
D3DXVECTOR3 g_posEnemy;
D3DXVECTOR3 g_rotEnemy;
D3DXVECTOR3 g_moveEnemy;
float g_fLengthEnemy;//対角線の長さ
float g_fAngleEnemy;//対角線の角度
int g_nData = 26;//クリア条件
int g_nTime = 0;

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//クリア条件初期化
	g_nData = 26;

	g_nTime++;

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy001.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy002.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy003.png", &g_apTextureEnemy[2]);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 6;
	}
	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 256,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}
	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitEnemy(void)
{
	//テクスチャの開放
	for (int nCount = 0; nCount < MAX_TYPE_ENEMY; nCount++)
	{
		if (g_apTextureEnemy[nCount] != NULL)
		{
			g_apTextureEnemy[nCount]->Release();
			g_apTextureEnemy[nCount] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	VERTEX_2D *pVtx;

	ENEMY *pEnemy;

	pEnemy = &g_aEnemy[0];

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さ
	g_fLengthEnemy = sqrtf(MAX_ENEMY_SIZE * MAX_ENEMY_SIZE + MAX_ENEMY_SIZE * MAX_ENEMY_SIZE);

	//対角線の角度
	g_fAngleEnemy = atan2f(MAX_ENEMY_SIZE, MAX_ENEMY_SIZE);

	//敵アニメーション
	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].g_nCounterAni++;

			if ((g_aEnemy[nCntEnemy].g_nCounterAni % 80) == 0)
			{
				g_aEnemy[nCntEnemy].g_nPatternAni++;

				g_aEnemy[nCntEnemy].g_nseetAni = (g_aEnemy[nCntEnemy].g_nseetAni + 1) % 10;

				//敵のアニメーション
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f*g_aEnemy[nCntEnemy].g_nseetAni, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f*g_aEnemy[nCntEnemy].g_nseetAni, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f*g_aEnemy[nCntEnemy].g_nseetAni, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f*g_aEnemy[nCntEnemy].g_nseetAni, 0.0f);
			}

			g_aEnemy[nCntEnemy].Pos.x += 2;
			g_aEnemy[nCntEnemy].Pos.z += 0.0f;

			if (g_aEnemy[nCntEnemy].Pos.x >= SCREEN_WIDTH)
			{
				g_aEnemy[nCntEnemy].Pos.x *= -1;
				g_aEnemy[nCntEnemy].Pos.y += 100;
				g_aEnemy[nCntEnemy].Pos.x *= 0.0f;
			}
			else if (g_aEnemy[nCntEnemy].Pos.x <= 0)
			{
				g_aEnemy[nCntEnemy].Pos.y += 100;
				g_aEnemy[nCntEnemy].Pos.y += 0.0f;
			}

			int nCnt = rand() % 50;
			if (nCnt == 1)
			{
				//弾の発射
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x, g_aEnemy[nCntEnemy].Pos.y + 50, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 300, 1);
			}

			//for (int nCntEnemy = 0; nCntEnemy < 7; nCntEnemy++)
			//{
			//	/*D3DXVECTOR3 move;
			//	D3DXVECTOR3 Pos;
			//	float fAngle;


			//	int nLife;

			//	Pos = pEnemy->Pos;
			//	nLife = 50;
			//	fAngle = (float)(rand() % 157 - 78) / 100.0f;
			//	fAngle = (float)(rand() % 628 - 314) / 100.0f;
			//	move.x = sinf(fAngle)*3.5f;
			//	move.y = cosf(fAngle)*3.5f;

			//	move.x = sinf(D3DX_PI)*3.5;
			//	move.y = -cosf(D3DX_PI)*3.5;

			//	SetEffect(Pos,nLife,move);


			//	SetBullet(g_aEnemy[nCntEnemy].Pos, move, nLife, 1);*/
			//}
			

			//敵の位置データ
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].Pos.x + sinf(g_rotEnemy.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].Pos.y + cosf(g_rotEnemy.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].Pos.x - sinf(g_rotEnemy.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].Pos.y - cosf(g_rotEnemy.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].Pos.x + sinf(g_rotEnemy.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].Pos.y + cosf(g_rotEnemy.z + g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].Pos.x - sinf(g_rotEnemy.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].Pos.y - cosf(g_rotEnemy.z - g_fAngleEnemy) * g_fLengthEnemy;
			pVtx[3].pos.z = 0.0f;

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
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
	g_pVtxBuffEnemy->Unlock();
}

//-------------------------------------------------
//敵の描画処理
//-------------------------------------------------
void DrawEnemy(void)
{
	ENEMY *pEnemy;

	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pEnemy = &g_aEnemy[0];

	//ポリゴンの描画
	for (int nCutEnemy = 0; nCutEnemy < 256; nCutEnemy++, pEnemy++)
	{
		pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

		if (pEnemy->bUse == true)//弾を使用しているかどうか
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCutEnemy * 4, 2);
		}
	}
}

//-----------------------------------------------
//敵の出現
//-----------------------------------------------
void SetEnemy(D3DXVECTOR3 Pos, int nType)
{
	ENEMY *pEnemy;

	VERTEX_2D *pVtx;

	pEnemy = &g_aEnemy[0];

	// 頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
	{

		if (pEnemy->bUse == false)//弾を使用しているかどうか
		{
			//位置設定
			g_aEnemy[nCntEnemy].Pos = Pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);

			pEnemy->nType = nType;

			//弾の使用
			pEnemy->bUse = true;

			break;
		}

		pVtx += 4;//4頂点ごとずらす

	}
	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

}

//-----------------------------------------------
//敵情報の取得
//-----------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

bool HitEnemy(int nIdx, int nDamage)
{
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[nIdx];
	VERTEX_2D *pVtx;
	BULLET *pBullet;
	pBullet = GetBullet();

	g_aEnemy[nIdx].nLife -= nDamage;

	//敵のライフ0以下の場合
	if (g_aEnemy[nIdx].nLife <= 0)
	{
		//サウンド
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		//スコア加算
		AddScore(100);

		//敵の数
		g_nTime++;
		g_nData--;

		//敵を消す
		g_aEnemy[nIdx].bUse = false;

		//弾消える
		pBullet->bUse = false;

		//爆発の更新処理
		UpdateExplosion();

		//敵の場所に爆破
		SetExplosion(pEnemy->Pos);

		if (g_nData == 0)
		{
			//ボスの出現
			SetAppeared(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), 300,30);
			SetBoss(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), 0);
			//画面切り替え
			//SetFade(FADE_OUT, MODE_RESULT);
		}

		return true;
	}
	else
	{

		AddScore(10);

		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 10;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
  		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nIdx * 4;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		// 頂点バッファをアンロックする
		g_pVtxBuffEnemy->Unlock();
	}

	return false;
}

//クリア条件を取得
int GetEnemyNomber(void)
{
	return g_nData;
}
