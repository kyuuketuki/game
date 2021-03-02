//-----------------------------------------------------------------
//
// プレイヤーの処理 (model.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "car.h"
#include"camera.h"
#include "shadow.h"
#include "input.h"
#include "bullet.h"
#include "effect.h"
#include "object.h"
#include "gear.h"
#include "meshfield.h"
#include "gamepad.h"
#include "item.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define CAR_X			(50)									// 中心から端までまでの長さ(x)
#define CAR_Y			(50)
#define CAR_MOVE		(0.3f + g_Car.nSpeed)
#define CAR_MOVE_BACK	(0.2f)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPD3DXMESH g_pMeshCar = NULL;	//頂点情報
LPD3DXBUFFER g_pBuffMetCar = NULL;
LPDIRECT3DTEXTURE9 g_pTextureCar[5] = {};			//テクスチャ
DWORD  g_nNumMatCar = 0;							//マテリアルの数
CAR g_Car;
D3DXMATRIX g_mtxWorldCar;							//ワールドマトリックス
float g_carrot;
float g_SpeedCar = 0;

//-----------------------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitCar(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_Car.MinCar = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);

	g_Car.MaxCar = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

	//model読み込み
	D3DXLoadMeshFromX("data/model/car004.x",D3DXMESH_SYSTEMMEM,pDevice,NULL,&g_pBuffMetCar,NULL,&g_nNumMatCar,&g_pMeshCar);
	//D3DXLoadMeshFromX("data/model/剣.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMetCar, NULL, &g_nNumMatCar, &g_pMeshCar);
	//D3DXLoadMeshFromX("data/model/標識.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMetCar, NULL, &g_nNumMatCar, &g_pMeshCar);

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ 

	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	//頂点数を取得
	nNumVtx = g_pMeshCar->GetNumVertices();

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)g_pBuffMetCar->GetBufferPointer();


	for (int nCntMat = 0; nCntMat < (int)g_nNumMatCar; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャ読み込み
			//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/50.png", &g_pTextureCar[0]);
		}
	}

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshCar->GetFVF());

	//頂点バッファをロック
	g_pMeshCar->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (g_Car.MinCar.x > vtx.x)
		{
			g_Car.MinCar.x = vtx.x;
		}

		if (g_Car.MaxCar.x < vtx.x)
		{
			g_Car.MaxCar.x = vtx.x;
		}

		if (g_Car.MinCar.y > vtx.y)
		{
			g_Car.MinCar.y = vtx.y;
		}

		if (g_Car.MaxCar.y < vtx.y)
		{
			g_Car.MaxCar.y = vtx.y;
		}

		if (g_Car.MinCar.z > vtx.z)
		{
			g_Car.MinCar.z = vtx.z;
		}

		if (g_Car.MaxCar.z < vtx.z)
		{
			g_Car.MaxCar.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	//頂点フォーマットのサイズ分を進める
	}
	//頂点バッファをアンロック
	g_pMeshCar->UnlockVertexBuffer();

	//位置
	g_Car.pos = D3DXVECTOR3(0.0f, 0.0f, -600.0f);

	//向き
	g_Car.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//-----------------------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitCar(void)
{
	//マテリアルの開放
	if (g_pBuffMetCar != NULL)
	{
		g_pBuffMetCar->Release();
		g_pBuffMetCar = NULL;
	}

	//メッシュの開放
	if (g_pMeshCar != NULL)
	{
		g_pMeshCar->Release();
		g_pMeshCar = NULL;
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdateCar(void)
{
	CAMERA  *pCamera;
	pCamera = GetCamera();

	EFFECT *pEffect;
	pEffect = GetEffect();

	//常に上向き
	//g_Car.rotDest.y = pCamera->rot.y - D3DX_PI / 2;
	g_Car.rotDest.y = pCamera->rot.y;

	//移動パッド
	if (IsButtonPush(KEYINFO_A))
	{
		g_Car.move.x += sinf(pCamera->rot.y) * CAR_MOVE;
		g_Car.move.z += cosf(pCamera->rot.y) * CAR_MOVE;

		//煙のエフェクト
		SetFire(D3DXVECTOR3(g_Car.pos.x, g_Car.pos.y, g_Car.pos.z));
	}

	////移動キーボード
	//if (GetKeyboardPress(DIK_SPACE) == true)
	//{
	//	g_Car.move.x += sinf(pCamera->rot.y) * CAR_MOVE;
	//	g_Car.move.z += cosf(pCamera->rot.y) * CAR_MOVE;
	//}

	//バックパッド
	if (IsButtonPush(KEYINFO_B))
	{
		g_Car.move.x -= sinf(pCamera->rot.y) * CAR_MOVE_BACK;
		g_Car.move.z -= cosf(pCamera->rot.y) * CAR_MOVE_BACK;
	}

	////バックキーボード
	//if (GetKeyboardPress(DIK_M) == true)
	//{
	//	g_Car.move.x -= sinf(pCamera->rot.y) * CAR_MOVE_BACK;
	//	g_Car.move.z -= cosf(pCamera->rot.y) * CAR_MOVE_BACK;
	//}

	//// モデルの移動
	//if (GetKeyboardPress(DIK_W) == true)
	//{
	//	if (GetKeyboardPress(DIK_A) == true)
	//	{// 左上方向
	//		g_Car.move.x -= cosf(pCamera->Camera + D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.move.z += sinf(pCamera->Camera + D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.rotDest.y = pCamera->Camera + (D3DX_PI * 3 / 4);
	//	}
	//	else if (GetKeyboardPress(DIK_D) == true)
	//	{// 右上方向
	//		g_Car.move.x += cosf(pCamera->Camera - D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.move.z -= sinf(pCamera->Camera - D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.rotDest.y = pCamera->Camera - (D3DX_PI * 3 / 4);
	//	}
	//	else
	//	{// 上方向
	//		g_Car.move.x += sinf(pCamera->Camera) * CAR_MOVE;
	//		g_Car.move.z += cosf(pCamera->Camera) * CAR_MOVE;
	//		g_Car.rotDest.y = pCamera->Camera + D3DX_PI;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_S) == true)
	//{
	//	if (GetKeyboardPress(DIK_D) == true)
	//	{// 右上方向
	//		g_Car.move.x += cosf(pCamera->Camera + D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.move.z -= sinf(pCamera->Camera + D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.rotDest.y = pCamera->Camera + D3DX_PI / -4;
	//	}
	//	else if (GetKeyboardPress(DIK_A) == true)
	//	{// 左上方向
	//		g_Car.move.x -= cosf(pCamera->Camera - D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.move.z += sinf(pCamera->Camera - D3DX_PI / 4) * CAR_MOVE;
	//		g_Car.rotDest.y = pCamera->Camera + D3DX_PI / 4;
	//	}
	//	else
	//	{// 下方向
	//		g_Car.move.x += sinf(pCamera->Camera - D3DX_PI) * CAR_MOVE;
	//		g_Car.move.z += cosf(pCamera->Camera - D3DX_PI) * CAR_MOVE;
	//		g_Car.rotDest.y = pCamera->Camera;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_A) == true)
	//{// 左方向
	//	g_Car.move.x -= cosf(pCamera->Camera) * CAR_MOVE;
	//	g_Car.move.z += sinf(pCamera->Camera) * CAR_MOVE;
	//	g_Car.rotDest.y = pCamera->Camera + (D3DX_PI / 2);
	//}
	//else if (GetKeyboardPress(DIK_D) == true)
	//{// 右方向
	//	g_Car.move.x += cosf(pCamera->Camera) * CAR_MOVE;
	//	g_Car.move.z -= sinf(pCamera->Camera) * CAR_MOVE;
	//	g_Car.rotDest.y = pCamera->Camera + (D3DX_PI / -2);
	//}

	//ギアチェンジ
	if (IsButtonDown(KEYINFO_R1))
	{
		g_Car.nSpeed += 0.3f;
	}

	if (IsButtonDown(KEYINFO_L1))
	{
		g_Car.nSpeed -= 0.3f;
	}

	//ギア上限
	if (g_Car.nSpeed >= 1.5f)
	{
		g_Car.nSpeed = 1.5f;
	}

	if (g_Car.nSpeed <= 0.0f)
	{
		g_Car.nSpeed = 0.0f;
	}

	//ギアの表示
	if (g_Car.nSpeed >= 0.0f)
	{
		AddGear(1);
	}

	if (g_Car.nSpeed >= 0.3f)
	{
		AddGear(2);
	}

	if (g_Car.nSpeed >= 0.6f)
	{
		AddGear(3);
	}

	if (g_Car.nSpeed >= 0.9f)
	{
		AddGear(4);
	}

	if (g_Car.nSpeed >= 1.2f)
	{
		AddGear(5);
	}

	if (g_Car.nSpeed >= 1.5f)
	{
		AddGear(6);
	}

	if (IsButtonUp(KEYINFO_A))
	{
		AddGear(1);
		g_Car.nSpeed = 0.0f;
	}


	////上昇
	//if (GetKeyboardPress(DIK_W) == true)
	//{
	//	g_Car.pos.y += 0.5f;
	//}

	////下降
	//if (GetKeyboardPress(DIK_S) == true)
	//{
	//	g_Car.pos.y -= 0.5f;
	//}

	////モデル左回転
	//if (GetKeyboardPress(DIK_M) == true)
	//{
	//	g_carrot -= 0.1f;
	//	g_Car.rot = D3DXVECTOR3(0.0f, g_carrot, 0.0f);
	//}

	////モデル右回転
	//if (GetKeyboardPress(DIK_N) == true)
	//{
	//	g_carrot += 0.1f;
	//	g_Car.rot = D3DXVECTOR3(0.0f, g_carrot, 0.0f);
	//}

	////回転リセット
	//if (g_Car.rot.y < -D3DX_PI)
	//{
	//	g_Car.rot.y += D3DX_PI * 2.0f;
	//}

	//if (g_Car.rot.y > D3DX_PI)
	//{
	//	g_Car.rot.y -= D3DX_PI * 2.0f;
	//}

	//弾の発射
	//if (GetKeyboardTrigger(DIK_SPACE) == true)
	//{
	//	//SetBullet(D3DXVECTOR3(g_Car.pos.x, g_Car.pos.y + 10, g_Car.pos.z), D3DXVECTOR3(sinf(g_Car.rot.y), 0.0f, cosf(g_Car.rot.y)), 5, 500);

	//}

	//// 目的の回転角の上限
	//if (g_Car.rotDest.y - g_Car.rot.y < -D3DX_PI)
	//{
	//	g_Car.rotDest.y += D3DX_PI * 2.0f;
	//}
	//else if (g_Car.rotDest.y - g_Car.rot.y > D3DX_PI)
	//{
	//	g_Car.rotDest.y -= D3DX_PI * 2.0f;
	//}

	// 向きの更新
	g_Car.rot.y += (g_Car.rotDest.y - g_Car.rot.y) * 0.1f;

	//移動量の減衰
	g_Car.move.x -= (0 + g_Car.move.x) * 0.05f;
	g_Car.move.y -= (0 + g_Car.move.y) * 0.05f;
	g_Car.move.z -= (0 + g_Car.move.z) * 0.05f;

	//移動量の減衰
	g_Car.rotmove.x -= (0 + g_Car.rotmove.x) * 0.2f;
	g_Car.rotmove.y -= (0 + g_Car.rotmove.y) * 0.2f;
	g_Car.rotmove.z -= (0 + g_Car.rotmove.z) * 0.2f;

	//位置更新
	g_Car.pos.x += g_Car.move.x;
	g_Car.pos.y += g_Car.move.y;
	g_Car.pos.z += g_Car.move.z;

	//位置更新
	g_Car.rot.x += g_Car.rotmove.x;
	g_Car.rot.y += g_Car.rotmove.y;
	g_Car.rot.z += g_Car.rotmove.z;

	//影の位置更新
	SetPositionShadow(0, D3DXVECTOR3(g_Car.pos.x, 0.0f, g_Car.pos.z));

	//if (GetKeyboardPress(DIK_SPACE) == false)
	{
		//g_Car.pos.y -= 1.0f;
	}


	//オブジェクトの当たり判定
	CollisionVec(&g_Car.pos, &g_Car.posOld, &g_Car.move, g_Car.MaxCar.x, g_Car.MinCar.x, g_Car.MaxCar.z, g_Car.MinCar.z, g_Car.MaxCar.y, g_Car.MinCar.y);

	//オブジェクトの当たり判定
	CollisionVecItem(&g_Car.pos, &g_Car.posOld, &g_Car.move, g_Car.MaxCar.x, g_Car.MinCar.x, g_Car.MaxCar.z, g_Car.MinCar.z, g_Car.MaxCar.y, g_Car.MinCar.y);

	//フィールドの当たり判定
	//CollisionVecMesh(&g_Car.pos, &g_Car.posOld, &g_Car.move, g_Car.MaxCar.x, g_Car.MinCar.x, g_Car.MaxCar.z, g_Car.MinCar.z, g_Car.MaxCar.y, g_Car.MinCar.y);
}

//-----------------------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawCar(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存
	D3DXMATERIAL *pMat;				//マテリアルへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldCar);

	//向きの反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Car.rot.y, g_Car.rot.x, g_Car.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldCar, &g_mtxWorldCar, &mtxRot);

	//位置の反転
	D3DXMatrixTranslation(&mtxTrans, g_Car.pos.x, g_Car.pos.y, g_Car.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldCar, &g_mtxWorldCar, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCar);

	//現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)g_pBuffMetCar->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatCar; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		//pDevice->SetTexture(0, g_pTextureCar[nCntMat]);

		//モデルパーツの描画
		g_pMeshCar->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
CAR *GetCar(void)
{
	return &g_Car;
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
float GetCarSpeed(void)
{
	return g_SpeedCar;
}