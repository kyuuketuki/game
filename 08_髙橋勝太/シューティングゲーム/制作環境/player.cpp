//-----------------------------------------------------------------
//
// プレイヤーの処理 (player.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "player.h"
#include "bullet.h"
#include "sound.h"
#include "effect.h"
#include "tutorial.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define PLAYER_X		(30)									// 中心から端までまでの長さ(x)
#define PLAYER_Y		(30)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点情報
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//テクスチャ
int g_nCounterAnim;									//アニメーションカウンター
int g_nPatternAnim;									//アニメーションパターンNo.
int g_nPatternAnim2;								//アニメーションパターンNo.
D3DXVECTOR3 g_rotPlayer;							//
float g_fLengthPlayer;								//対角線の長さ
float g_fAnglePlayer;								//対角線の角度
D3DXVECTOR3 g_movePlayer;							//移動量
PLAYER g_Player;									//ポリゴンの位置

//-----------------------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void)
{

	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &g_pTexturePlayer);

	g_Player.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.bUse = true;
	g_Player.nLife = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置
	g_Player.Pos = D3DXVECTOR3(1000, 800, 0.0f);

	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//対角線の長さ
	g_fLengthPlayer = sqrtf(PLAYER_X * PLAYER_X + PLAYER_Y * PLAYER_Y);

	//対角線の角度
	g_fAnglePlayer = atan2f(PLAYER_X, PLAYER_Y);

	//頂点の座標
	pVtx[0].pos.x = (g_Player.Pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.y = (g_Player.Pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.z = (g_Player.Pos.z = 0.0f);

	pVtx[1].pos.x = (g_Player.Pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.y = (g_Player.Pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.z = (g_Player.Pos.z = 0.0f);

	pVtx[2].pos.x = (g_Player.Pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.y = (g_Player.Pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.z = (g_Player.Pos.z = 0.0f);

	pVtx[3].pos.x = (g_Player.Pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.y = (g_Player.Pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.z = (g_Player.Pos.z = 0.0f);

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

	//頂点の情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);



	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{

	//バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{

	VERTEX_2D *pVtx;

	BULLET *pBullet;
	pBullet = GetBullet();

	//g_nCounterAnim++;									//アニメーションカウンター更新
	//if ((g_nCounterAnim % 10) == 0)
	//{
	//	g_nPatternAnim = (g_nPatternAnim + 1) % 8;		//アニメーションパターンNo.更新
	//}

	////テクスチャ座標
	//pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_nPatternAnim), 1.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f * g_nPatternAnim), 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f * g_nPatternAnim), 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_nPatternAnim), 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置を更新
	if (g_Player.bUse == true)
	{
		//Aを押したとき
		if (GetKeyboardPress(DIK_A) == true)
		{
			//Wを押したとき
			if (GetKeyboardPress(DIK_W) == true)
			{
				g_Player.Pos.x -= sinf(D3DX_PI) * MOVE_NUMBER;
				g_Player.Pos.y -= cosf(D3DX_PI) * MOVE_NUMBER;
			}
			//AとSを押したとき
			else if (GetKeyboardPress(DIK_S) == true)
			{
				g_Player.Pos.x += sinf(-D3DX_PI) * MOVE_NUMBER;
				g_Player.Pos.y += cosf(D3DX_PI) * MOVE_NUMBER;
			}
			//Aを押したとき
			else
			{
				g_Player.Pos.x -= MOVE_NUMBER;
				g_Player.Pos.y += 0;
				g_Player.Pos.z += 0.0f;
			}

		}

		//Dを押したとき
		if (GetKeyboardPress(DIK_D) == true)
		{
			//DとWを押したとき
			if (GetKeyboardPress(DIK_W) == true)
			{
				g_Player.Pos.x += sinf(D3DX_PI) * MOVE_NUMBER;
				g_Player.Pos.y -= cosf(D3DX_PI) * MOVE_NUMBER;
			}
			//DとSを押したとき
			else if (GetKeyboardPress(DIK_S) == true)
			{
				g_Player.Pos.x += sinf(D3DX_PI) * MOVE_NUMBER;
				g_Player.Pos.y += cosf(D3DX_PI) * MOVE_NUMBER;
			}
			//Dを押したとき
			else
			{
				g_Player.Pos.x -= -MOVE_NUMBER;
				g_Player.Pos.y -= 0;
				g_Player.Pos.z += 0.0f;
			}
		}

		//Wを押したとき
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_Player.Pos.y -= MOVE_NUMBER;
		}

		//Sを押したとき
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_Player.Pos.y += MOVE_NUMBER;
		}

		//矢印左を押したとき
		if (GetKeyboardPress(DIK_LEFT) == true)
		{
			g_rotPlayer.z -= 0.1f;
			if (g_rotPlayer.z > D3DX_PI)
			{
				g_rotPlayer.z -= D3DX_PI * 2.0f;
			}

			g_movePlayer.z -= (g_fAnglePlayer) * 0.01f;
		}

		//矢印右を押したとき
		if (GetKeyboardPress(DIK_RIGHT) == true)
		{
			g_rotPlayer.z += 0.1f;
			if (g_rotPlayer.z > D3DX_PI)
			{
				g_rotPlayer.z -= D3DX_PI * 2.0f;
			}
			g_movePlayer.z += (g_fAnglePlayer) * 0.01f;
		}

		//Aを押したときの加速量
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_movePlayer.x -= sinf(g_fAnglePlayer) * 1;
		}

		//Dを押したときの加速量
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_movePlayer.x += sinf(g_fAnglePlayer) * 1;
		}

		//Wを押したときの加速量
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.y -= cosf(g_fAnglePlayer) * 1;
		}

		//Sを押したときの加速量
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.y += cosf(g_fAnglePlayer) * 1;
		}

		//弾の発射
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			
			PlaySound(SOUND_LABEL_SE_SHOT);
			SetBullet(g_Player.Pos, D3DXVECTOR3(0.0f, -7.0f, 0.0f), 300, 0);

			for (int nCntBullet = 0; nCntBullet < 1; nCntBullet++)
			{
				D3DXVECTOR3 move;
				D3DXVECTOR3 Pos;
				float fAngle;
				int nLife;
				Pos = g_Player.Pos;
				nLife = 200;
				fAngle = (float)(275) / 100.0f;
				move.x = sinf(fAngle)*7.0f;
				move.y = cosf(fAngle)*7.0f;

				//弾の発射
				SetBullet(g_Player.Pos, move, nLife, 0);
			}

			for (int nCntBullet = 0; nCntBullet < 1; nCntBullet++)
			{
				D3DXVECTOR3 move;
				D3DXVECTOR3 Pos;
				float fAngle;
				int nLife;
				Pos = g_Player.Pos;
				nLife = 200;
				fAngle = (float)(-275) / 100.0f;
				move.x = sinf(fAngle)*7.0f;
				move.y = cosf(fAngle)*7.0f;

				//弾の発射
				SetBullet(g_Player.Pos, move, nLife, 0);
			}

		}

		//移動制限
		if (g_Player.Pos.x <= 0)
		{
			g_Player.Pos.x = PLAYER_X / 2;
		}

		if (g_Player.Pos.x >= SCREEN_WIDTH)
		{
			g_Player.Pos.x = SCREEN_WIDTH - PLAYER_X / 2;
		}

		if (g_Player.Pos.y <= 0)
		{
			g_Player.Pos.y = PLAYER_Y / 2;
		}

		if (g_Player.Pos.y >= SCREEN_HEIGHT)
		{
			g_Player.Pos.y = SCREEN_HEIGHT - PLAYER_Y / 2;
		}

		//位置更新
		g_Player.Pos.x += g_movePlayer.x;
		g_Player.Pos.y += g_movePlayer.y;
		g_rotPlayer.z += g_movePlayer.z;

		//移動量の減衰
		g_movePlayer.x += (0 - g_movePlayer.x) * 0.2f;
		g_movePlayer.y += (0 - g_movePlayer.y) * 0.2f;
		g_movePlayer.z += (0 - g_movePlayer.z) * 0.5f;

		//頂点の座標
		pVtx[0].pos.x = (g_Player.Pos.x - sinf(g_fAnglePlayer + g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[0].pos.y = (g_Player.Pos.y + cosf(g_fAnglePlayer + g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[0].pos.z = (g_Player.Pos.z = 0.0f);

		pVtx[1].pos.x = (g_Player.Pos.x - sinf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[1].pos.y = (g_Player.Pos.y - cosf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[1].pos.z = (g_Player.Pos.z = 0.0f);

		pVtx[2].pos.x = (g_Player.Pos.x + sinf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[2].pos.y = (g_Player.Pos.y + cosf(g_fAnglePlayer - g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[2].pos.z = (g_Player.Pos.z = 0.0f);

		pVtx[3].pos.x = (g_Player.Pos.x + sinf(g_fAnglePlayer + g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[3].pos.y = (g_Player.Pos.y - cosf(g_fAnglePlayer + g_rotPlayer.z) * g_fLengthPlayer);
		pVtx[3].pos.z = (g_Player.Pos.z = 0.0f);
	}

	g_pVtxBuffPlayer->Unlock(); //アンロック
}
//-----------------------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bUse == true)//弾を使用しているかどうか
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}

PLAYER * GetPlayer(void)
{
	return &g_Player;
}