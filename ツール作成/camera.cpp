//==============================================================================
//
// カメラの設定〔camera.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "camera.h"
#include "model.h"
#include "car.h"
#include "input.h"
#include "game.h"
#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define CAMERA_MOVE				(2.0f)							// カメラの移動量
#define CAMERA_TURN				(D3DX_PI / 100.0f)				// カメラの旋回速度
#define CAMERA_RANGE_CAR		(150.0f)						// 視点・注視点間の距離
#define CAMERA_RANGE_MODEL		(300.0f)						// 視点・注視点間の距離
#define CAMERA_HEIGHT			(500.0f)						// カメラ(視点)の高さ
#define CAMERA_SPDOWN			(0.5f)							// 減衰係数 

//==============================================================================
// グローバル変数
//==============================================================================
CAMERA g_camera;
float nHeight = 0;

//==============================================================================
// 初期化処理
//==============================================================================
void InitCamera(void)
{
	// ローカル変数宣言
	PLAYER *player;
	player = GetModel();

	CAR  *pCar;
	pCar = GetCar();

	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 注視点位置
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量
	g_camera.rot = D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f);		// カメラの角度
	g_camera.fLength = CAMERA_RANGE_CAR;						// 視点・注視点間の距離
	g_camera.fHeightV = -60.0f;									// 視点の高さ
	g_camera.fHeightR = player->pos.y + 50.0f;					// 注視点の高さ
	g_camera.fFront = 15.0f;									// モデルの前方の距離
	g_camera.bAutoTurn = false;									// カメラの回り込みON・OFF
	g_camera.viewport.X = 0;									//描画する画面の左上X座標
	g_camera.viewport.Y = 0;									//描画する画面の左上Y座標
	g_camera.viewport.Width = 860.0f;							//幅
	g_camera.viewport.Height = SCREEN_HEIGHT;					//高
	g_camera.viewport.MinZ = 0.0f;								//固定
	g_camera.viewport.MaxZ = 1.0f;								//固定
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitCamera(void)
{

}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateCamera(void)
{
	// 注視点との距離
	//g_camera.fLength = sqrtf(
	//	(g_camera.posV.y - g_camera.posR.y) * (g_camera.posV.y - g_camera.posR.y) + 
	//	(g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z));

	// ローカル変数宣言
	PLAYER *player;
	player = GetModel();

	CAR  *pCar;
	pCar = GetCar();

	pCar->posOld = pCar->pos;

	// リセット
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの角度
	}

	//視点・注視点の移動
	//if (GetKeyboardPress(KEYINFO_UP) == true)
	//{
	//	g_camera.move.x += sinf(g_camera.rot.y) * CAMERA_MOVE;
	//	g_camera.move.z += cosf(g_camera.rot.y) * CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(KEYINFO_DOWN) == true)
	//{
	//	g_camera.move.x -= sinf(g_camera.rot.y) * CAMERA_MOVE;
	//	g_camera.move.z -= cosf(g_camera.rot.y) * CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(KEYINFO_LEFT) == true)
	//{
	//	g_camera.move.x -= cosf(g_camera.rot.y) * CAMERA_MOVE;
	//	g_camera.move.z += sinf(g_camera.rot.y) * CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	//{
	//	g_camera.move.x += cosf(g_camera.rot.y) * CAMERA_MOVE;
	//	g_camera.move.z -= sinf(g_camera.rot.y) * CAMERA_MOVE;
	//}

	//注視点の移動(左右)
	//if (GetKeyboardPress(KEYINFO_LEFT_CENTER) == true)
	//{
	//	g_camera.rot.y += D3DX_PI / -CAMERA_TURN;

	//	if (g_camera.rot.y < -D3DX_PI)
	//	{
	//		g_camera.rot.y += D3DX_PI * 2.0f;
	//	}
	//}
	//if (GetKeyboardPress(KEYINFO_RIGHT_CENTER) == true)
	//{
	//	g_camera.rot.y += D3DX_PI / CAMERA_TURN;

	//	if (g_camera.rot.y > D3DX_PI)
	//	{
	//		g_camera.rot.y -= D3DX_PI * 2.0f;
	//	}
	//}

	//// 注視点の移動(上下)(T,Bキー)
	//if (GetKeyboardPress(DIK_T) == true)
	//{
	//	g_camera.fHeightR += CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(DIK_B) == true)
	//{
	//	g_camera.fHeightR -= CAMERA_MOVE;
	//}

	if (GetGame() == 1)
	{
		g_camera.fHeightV = 500.0f + nHeight;				// 視点の高さ
		g_camera.fLength = CAMERA_RANGE_MODEL;				//視点の距離
		
		//高さ調整
		if (GetKeyboardPress(DIK_8) == true)
		{
			nHeight += 10.0f;
		}

		if (GetKeyboardPress(DIK_9) == true)
		{
			nHeight -= 10.0f;
		}

		if (GetKeyboardPress(DIK_C) == true)
		{
			g_camera.rot.y += CAMERA_TURN;

			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y -= D3DX_PI * 2.0f;
			}

			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
		}
		if (GetKeyboardPress(DIK_Z) == true)
		{
			g_camera.rot.y -= CAMERA_TURN;

			if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += D3DX_PI * 2.0f;
			}

			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
			g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
		}

		// 目的の注視点
		g_camera.posRDest.x = player->pos.x - g_camera.fFront * sinf(player->rotDest.y);
		g_camera.posRDest.z = player->pos.z - g_camera.fFront * cosf(player->rotDest.y);
		g_camera.posRDest.y = player->pos.y + g_camera.fHeightR;

		// 目的の視点
		g_camera.posVDest.x = player->pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.z = player->pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.y = player->pos.y + cosf(g_camera.rot.x) * g_camera.fLength + g_camera.fHeightV;

	}

	//車の時
	if (GetGame() == 0)
	{
		g_camera.fHeightV = -60.0f;							// 視点の高さ
		g_camera.fHeightR = player->pos.y + 50.0f;			// 注視点の高さ
		g_camera.fLength = CAMERA_RANGE_CAR;				// 視点の距離

		//視点の移動(左右)(Z,Cキー)
		if (IsButtonPush(KEYINFO_A))
		{
			if (IsButtonPush(KEYINFO_RIGHT))
			{
				g_camera.rot.y += CAMERA_TURN;

				/*if (g_camera.rot.y > D3DX_PI)
				{
					g_camera.rot.y -= D3DX_PI * 2.0f;
				}*/

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
				g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
			}
			if (IsButtonPush(KEYINFO_LEFT))
			{
				g_camera.rot.y -= CAMERA_TURN;

				/*if (g_camera.rot.y < -D3DX_PI)
				{
					g_camera.rot.y += D3DX_PI * 2.0f;
				}*/

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
				g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
			}
		}

		if (IsButtonPush(KEYINFO_B))
		{
			if (IsButtonPush(KEYINFO_LEFT))
			{
				g_camera.rot.y += CAMERA_TURN;

				/*if (g_camera.rot.y > D3DX_PI)
				{
					g_camera.rot.y -= D3DX_PI * 2.0f;
				}*/

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
				g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
			}
			if (IsButtonPush(KEYINFO_RIGHT))
			{
				g_camera.rot.y -= CAMERA_TURN;

				/*if (g_camera.rot.y < -D3DX_PI)
				{
					g_camera.rot.y += D3DX_PI * 2.0f;
				}*/

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
				g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
			}
		}

		// 目的の注視点
		g_camera.posRDest.x = pCar->pos.x - g_camera.fFront * sinf(pCar->rotDest.y);
		g_camera.posRDest.z = pCar->pos.z - g_camera.fFront * cosf(pCar->rotDest.y);
		g_camera.posRDest.y = pCar->pos.y + g_camera.fHeightR;

		// 目的の視点
		g_camera.posVDest.x = pCar->pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.z = pCar->pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posVDest.y = pCar->pos.y + cosf(g_camera.rot.x) * g_camera.fLength + g_camera.fHeightV;
	}


	//// 視点の移動(上下)(Y,Nキー)
	//if (GetKeyboardPress(DIK_Y) == true)
	//{
	//	g_camera.fHeightV += CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(DIK_N) == true)
	//{
	//	g_camera.fHeightV -= CAMERA_MOVE;
	//}

	//// 注視点へのズーム処理(U,Mキー)
	//if (GetKeyboardPress(DIK_U) == true && g_camera.fLength >= 100.0f)
	//{
	//	g_camera.fLength -= CAMERA_MOVE;
	//}
	//if (GetKeyboardPress(DIK_M) == true && g_camera.fLength <= 300.0f)
	//{
	//	g_camera.fLength += CAMERA_MOVE;
	//}
	//if (g_camera.fLength > 300.0f)
	//{
	//	g_camera.fLength = 300.0f;
	//}
	//if (g_camera.fLength < 100.0f)
	//{
	//	g_camera.fLength = 100.0f;
	//}

	//// モデル前方の距離
	//if (GetKeyboardPress(DIK_Q) == true && g_camera.fFront <= 25.0f)
	//{
	//	g_camera.fFront += 0.1f;
	//}
	//if (GetKeyboardPress(DIK_E) == true && g_camera.fFront >= 0.0f)
	//{
	//	g_camera.fFront -= 0.1f;
	//}
	//if (g_camera.fFront > 25.0f)
	//{
	//	g_camera.fFront = 25.0f;
	//}
	//if (g_camera.fFront < 0.0f)
	//{
	//	g_camera.fFront = 0.0f;
	//}

	//// 1周した角度の修正
	//if (g_camera.rot.y < -D3DX_PI)
	//{
	//	g_camera.rot.y += D3DX_PI * 2.0f;
	//}
	//else if (g_camera.rot.y > D3DX_PI)
	//{
	//	g_camera.rot.y -= D3DX_PI * 2.0f;
	//}

	// 向きの更新
	//g_camera.rot.y += (g_camera.rotDest.y - g_camera.rot.y) * 0.1f;


	// 移動量の減衰(慣性)
	//g_camera.move.x += (0.0f - g_camera.move.x) * 0.4f;
	//g_camera.move.z += (0.0f - g_camera.move.z) * 0.4f;

	// 移動量の加算
	//g_camera.posV.x += g_camera.move.x;
	//g_camera.posV.z += g_camera.move.z;
	//g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * CAMERA_RANGE;
	//g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * CAMERA_RANGE;

	// モデルの位置へ注視点を置く
	//g_camera.posR.x = model.pos.x;
	//g_camera.posR.z = model.pos.z;
	//g_camera.posV.x = model.pos.x + sinf(g_camera.rot.y) * -g_camera.fLength;
	//g_camera.posV.z = model.pos.z + cosf(g_camera.rot.y) * -g_camera.fLength;
	//g_camera.posV.y = model.pos.y + cosf(g_camera.rot.x) * 200.0f;

	//// 目的の注視点
	//g_camera.posRDest.x = model.pos.x - g_camera.fFront * sinf(model.rotDest.y);
	//g_camera.posRDest.z = model.pos.z - g_camera.fFront * cosf(model.rotDest.y);
	//g_camera.posRDest.y = model.pos.y + g_camera.fHeightR;

	//// 目的の視点
	//g_camera.posVDest.x = model.pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
	//g_camera.posVDest.z = model.pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
	//g_camera.posVDest.y = model.pos.y + cosf(g_camera.rot.x) * g_camera.fLength + g_camera.fHeightV;

	//// 注視点の位置更新
	//g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * CAMERA_SPDOWN;
	//g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * CAMERA_SPDOWN;
	//g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * CAMERA_SPDOWN;

	//// 視点の位置更新
	//g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CAMERA_SPDOWN;
	//g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CAMERA_SPDOWN;
	//g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * CAMERA_SPDOWN;

	//// 目的の注視点
	//g_camera.posRDest.x = player->pos.x - g_camera.fFront * sinf(player->rotDest.y);
	//g_camera.posRDest.z = player->pos.z - g_camera.fFront * cosf(player->rotDest.y);
	//g_camera.posRDest.y = player->pos.y + g_camera.fHeightR;

	/*g_camera.posRDest.x = pCar->pos.x - g_camera.fFront * sinf(pCar->rotDest.y);
	g_camera.posRDest.z = pCar->pos.z - g_camera.fFront * cosf(pCar->rotDest.y);
	g_camera.posRDest.y = pCar->pos.y + g_camera.fHeightR;*/

	//// 目的の視点
	//g_camera.posVDest.x = player->pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
	//g_camera.posVDest.z = player->pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
	//g_camera.posVDest.y = player->pos.y + cosf(g_camera.rot.x) * g_camera.fLength + g_camera.fHeightV;

	/*g_camera.posVDest.x = pCar->pos.x - sinf(g_camera.rot.y) * g_camera.fLength;
	g_camera.posVDest.z = pCar->pos.z - cosf(g_camera.rot.y) * g_camera.fLength;
	g_camera.posVDest.y = pCar->pos.y + cosf(g_camera.rot.x) * g_camera.fLength + g_camera.fHeightV;*/

	// 注視点の位置更新
	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * CAMERA_SPDOWN;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * CAMERA_SPDOWN;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * CAMERA_SPDOWN;

	// 視点の位置更新
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CAMERA_SPDOWN;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CAMERA_SPDOWN;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * CAMERA_SPDOWN;

	//if (GetKeyboardPress(KEYINFO_UP) == true)
	//{
	//	g_camera.moveZVR.x = sinf(g_camera.rot.y) * CAMERA_MOVE;
	//	g_camera.moveZVR.z = cosf(g_camera.rot.y) * CAMERA_MOVE;
	//}
	//else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	//{
	//	g_camera.moveZVR.x = sinf(g_camera.rot.y - D3DX_PI) * CAMERA_MOVE;
	//	g_camera.moveZVR.z = cosf(g_camera.rot.y - D3DX_PI) * CAMERA_MOVE;
	//}
	//else
	//{
	//	g_camera.moveZVR.x = 0.0f;
	//	g_camera.moveZVR.z = 0.0f;
	//}

	//if (GetKeyboardPress(KEYINFO_LEFT) == true)
	//{
	//	g_camera.moveXVR.x = cosf(-g_camera.rot.y - D3DX_PI) * CAMERA_MOVE;
	//	g_camera.moveXVR.z = sinf(-g_camera.rot.y - D3DX_PI) * CAMERA_MOVE;
	//}
	//else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	//{
	//	g_camera.moveXVR.x = cosf(-g_camera.rot.y) * CAMERA_MOVE;
	//	g_camera.moveXVR.z = sinf(-g_camera.rot.y) * CAMERA_MOVE;
	//}
	//else
	//{
	//	g_camera.moveXVR.x = 0.0f;
	//	g_camera.moveXVR.z = 0.0f;
	//}

	//// 注視点の移動(左右)
	//if (GetKeyboardPress(KEYINFO_LEFT_CENTER) == true)
	//{
	//	g_camera.rot.y += D3DX_PI / -CAMERA_TURN;

	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * CAMERA_RANGE;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * CAMERA_RANGE;
	//}
	//else if(GetKeyboardPress(KEYINFO_RIGHT_CENTER) == true)
	//{
	//	g_camera.rot.y += D3DX_PI / CAMERA_TURN;

	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * CAMERA_RANGE;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * CAMERA_RANGE;
	//}

	//// 注視点の移動(上下)
	//if (GetKeyboardPress(KEYINFO_UP_CENTER) == true)
	//{
	//	g_camera.moveR.y = CAMERA_MOVE;
	//}
	//else if (GetKeyboardPress(KEYINFO_DOWN_CENTER) == true)
	//{
	//	g_camera.moveR.y = -CAMERA_MOVE;
	//}
	//else
	//{
	//	g_camera.moveR.y = 0.0f;
	//}

	//// 視点の移動(左右)
	//if (GetKeyboardPress(KEYINFO_LEFT_TURN) == true)
	//{
	//	g_camera.rot.y += D3DX_PI / -CAMERA_TURN;

	//	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * CAMERA_RANGE;
	//	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * CAMERA_RANGE;
	//}
	//else if (GetKeyboardPress(KEYINFO_RIGHT_TURN) == true)
	//{
	//	g_camera.rot.y += D3DX_PI / CAMERA_TURN;

	//	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * CAMERA_RANGE;
	//	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * CAMERA_RANGE;
	//}

	//// 視点の移動(上下)
	//if (GetKeyboardPress(KEYINFO_UP_SIGHT) == true)
	//{
	//	g_camera.moveV.y = CAMERA_MOVE;
	//}
	//else if (GetKeyboardPress(KEYINFO_DOWN_SIGHT) == true)
	//{
	//	g_camera.moveV.y = -CAMERA_MOVE;
	//}
	//else
	//{
	//	g_camera.moveV.y = 0.0f;
	//}

	//if (g_camera.rot.y < -D3DX_PI)
	//{
	//	g_camera.rot.y += D3DX_PI * 2;
	//}
	//else if (g_camera.rot.y > D3DX_PI)
	//{
	//	g_camera.rot.y -= D3DX_PI * 2;
	//}

	//// 移動量の加算
	//g_camera.posV += g_camera.moveV;
	//g_camera.posR += g_camera.moveR;
	//g_camera.posV += g_camera.moveXVR;
	//g_camera.posR += g_camera.moveXVR;
	//g_camera.posV += g_camera.moveZVR;
	//g_camera.posR += g_camera.moveZVR;
}

//==============================================================================
// カメラの設定処理
//==============================================================================
void SetCamera(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	
	// デバイスの取得
	pDevice = GetDevice();

	//ビューポートを取得
	//pDevice->SetViewport(&g_camera.viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&g_camera.mtxProjection,
		D3DXToRadian(45.0f),									// 画角の設定
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,				// 画面比率の設定 ビューポートの幅高さ
		//860 / (float)SCREEN_HEIGHT,							// 画面比率の設定 ビューポートの幅高さ
		10.0f,													// 手前限界の描画距離
		5000.0f);												// 奥限界の描画距離

														// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//==============================================================================
// カメラの取得
//==============================================================================
CAMERA *GetCamera(void)
{
	return &g_camera;
}