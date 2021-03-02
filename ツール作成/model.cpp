//==============================================================================
//
// 3Dプレイヤーの描画〔player.cpp〕
// AUTHOR : TAKAHASHI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "object.h"
#include "math.h"
#include "effect.h"
#include "meshfield.h"
#include "gamepad.h"
#include <stdio.h>

//==============================================================================
// マクロ定義
//==============================================================================

#define PLAYER_MOVE		(1.0f)
#define PLAYER_TURN		(0.1f)
#define VTX_MINP		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))			// 仮頂点最小値
#define	VTX_MAXP		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))		// 仮頂点最大値
#define PLAYER_X		(50)												// 中心から端までまでの長さ(x)
#define PLAYER_Y		(50)
#define MODEL_MOVE		(3.0f + ModelSpeed)
#define SPEEDDOWN		(0.2f)
#define VTX_MINP		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAXP		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値

//==============================================================================
// グローバル変数
//==============================================================================
PLAYER g_player;							// プレイヤーの構造体
D3DXVECTOR3 g_vtxMinplayer, g_vtxMaxplayer;	// モデルの各座標の最大値・最小値
float ModelSpeed = 0;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	g_player.aModel[0].MinModel = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);

	g_player.aModel[0].MaxModel = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

	// Xファイルの読み込み
	//model読み込み
	D3DXLoadMeshFromX("data/MODEL/腰.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[0].pBuffMatModel, NULL, &g_player.aModel[0].nNumMatModel, &g_player.aModel[0].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/胴体.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[1].pBuffMatModel, NULL, &g_player.aModel[1].nNumMatModel, &g_player.aModel[1].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/顔.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[2].pBuffMatModel, NULL, &g_player.aModel[2].nNumMatModel, &g_player.aModel[2].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/右肩.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[3].pBuffMatModel, NULL, &g_player.aModel[3].nNumMatModel, &g_player.aModel[3].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/右腕.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[4].pBuffMatModel, NULL, &g_player.aModel[4].nNumMatModel, &g_player.aModel[4].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/右手.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[5].pBuffMatModel, NULL, &g_player.aModel[5].nNumMatModel, &g_player.aModel[5].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/左肩.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[6].pBuffMatModel, NULL, &g_player.aModel[6].nNumMatModel, &g_player.aModel[6].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/左腕.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[7].pBuffMatModel, NULL, &g_player.aModel[7].nNumMatModel, &g_player.aModel[7].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/左手.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[8].pBuffMatModel, NULL, &g_player.aModel[8].nNumMatModel, &g_player.aModel[8].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/右腿.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[9].pBuffMatModel, NULL, &g_player.aModel[9].nNumMatModel, &g_player.aModel[9].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/右ひざ下.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[10].pBuffMatModel, NULL, &g_player.aModel[10].nNumMatModel, &g_player.aModel[10].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/右靴.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[11].pBuffMatModel, NULL, &g_player.aModel[11].nNumMatModel, &g_player.aModel[11].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/左腿.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[12].pBuffMatModel, NULL, &g_player.aModel[12].nNumMatModel, &g_player.aModel[12].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/左ひざ下.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[13].pBuffMatModel, NULL, &g_player.aModel[13].nNumMatModel, &g_player.aModel[13].MeshModel);
	D3DXLoadMeshFromX("data/MODEL/左靴.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[14].pBuffMatModel, NULL, &g_player.aModel[14].nNumMatModel, &g_player.aModel[14].MeshModel);

	g_player.nNumModel = MODEL_PARTS;

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_player.aModel[0].MeshModel->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].MeshModel->GetFVF());

	//各パーツの階層構造設定	腰
	g_player.aModel[0].nIdxModelParent = -1;
	g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 52.0f, 0.0f);
	g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	胴体
	g_player.aModel[1].nIdxModelParent = 0;
	g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	頭
	g_player.aModel[2].nIdxModelParent = 1;
	g_player.aModel[2].pos = D3DXVECTOR3(0.0f, 27.0f, 0.0f);
	g_player.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右肩
	g_player.aModel[3].nIdxModelParent = 1;
	g_player.aModel[3].pos = D3DXVECTOR3(-15.5f, 18.5f, 0.0f);
	g_player.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右腕
	g_player.aModel[4].nIdxModelParent = 3;
	g_player.aModel[4].pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	g_player.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右手
	g_player.aModel[5].nIdxModelParent = 4;
	g_player.aModel[5].pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	g_player.aModel[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	左肩
	g_player.aModel[6].nIdxModelParent = 1;
	g_player.aModel[6].pos = D3DXVECTOR3(15.5f, 18.5f, 0.0f);
	g_player.aModel[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	左腕
	g_player.aModel[7].nIdxModelParent = 6;
	g_player.aModel[7].pos = D3DXVECTOR3(12.0f, 0.0f, 0.0f);
	g_player.aModel[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	左手
	g_player.aModel[8].nIdxModelParent = 7;
	g_player.aModel[8].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	g_player.aModel[8].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右腿
	g_player.aModel[9].nIdxModelParent = 0;
	g_player.aModel[9].pos = D3DXVECTOR3(-6.0f, 0.0f, 0.0f);
	g_player.aModel[9].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右ひざ下
	g_player.aModel[10].nIdxModelParent = 9;
	g_player.aModel[10].pos = D3DXVECTOR3(0.0f, -25.0f, 0.0f);
	g_player.aModel[10].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右足
	g_player.aModel[11].nIdxModelParent = 10;
	g_player.aModel[11].pos = D3DXVECTOR3(0.0f, -20.0f, 0.0f);
	g_player.aModel[11].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右足
	g_player.aModel[12].nIdxModelParent = 0;
	g_player.aModel[12].pos = D3DXVECTOR3(6.0f, 0.0f, 0.0f);
	g_player.aModel[12].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右足
	g_player.aModel[13].nIdxModelParent = 12;
	g_player.aModel[13].pos = D3DXVECTOR3(0.0f, -25.0f, 0.0f);
	g_player.aModel[13].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各パーツの階層構造設定	右足
	g_player.aModel[14].nIdxModelParent = 13;
	g_player.aModel[14].pos = D3DXVECTOR3(0.0f, -20.0f, 0.0f);
	g_player.aModel[14].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 変数の初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.motionType = MOTIONTYPE_NEUTRAL;
	g_player.bLoopMotion = false;
	g_player.nNumKey = 0;
	g_player.nKey = 0;
	g_player.nCounterMotion = 0;
	g_vtxMinplayer = VTX_MINP;
	g_vtxMaxplayer = VTX_MAXP;

	// 影の設定
	g_player.nIdx = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z), 30.0f, 30.0f);

	// 頂点バッファをロック
	g_player.aModel[0].MeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点数を取得
	nNumVtx = g_player.aModel[0].MeshModel->GetNumVertices();

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].MeshModel->GetFVF());

		//頂点バッファをロック
		g_player.aModel[0].MeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 各座標の最大値の比較
			if (g_vtxMaxplayer.x < vtx.x)
			{
				g_vtxMaxplayer.x = vtx.x;
			}
			if (g_vtxMaxplayer.y < vtx.y)
			{
				g_vtxMaxplayer.y = vtx.y;
			}
			if (g_vtxMaxplayer.z < vtx.z)
			{
				g_vtxMaxplayer.z = vtx.z;
			}

			// 各座標の最小値の比較
			if (g_vtxMinplayer.x > vtx.x)
			{
				g_vtxMinplayer.x = vtx.x;
			}
			if (g_vtxMinplayer.y > vtx.y)
			{
				g_vtxMinplayer.y = vtx.y;
			}
			if (g_vtxMinplayer.z > vtx.z)
			{
				g_vtxMinplayer.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	//頂点フォーマットのサイズ分を進める
		}
	}

	// 頂点バッファをアンロック
	g_player.aModel[0].MeshModel->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		if (g_player.aModel[nCnt].MeshModel != NULL)
		{// メッシュの破棄
			g_player.aModel[nCnt].MeshModel->Release();
			g_player.aModel[nCnt].MeshModel = NULL;
		}

		if (g_player.aModel[nCnt].pBuffMatModel != NULL)
		{// バッファの破棄
			g_player.aModel[nCnt].pBuffMatModel->Release();
			g_player.aModel[nCnt].pBuffMatModel = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdatePlayer(void)
{
	CAMERA  *pCamera;
	pCamera = GetCamera();

	//位置情報を記憶
	g_player.posOld = g_player.pos;

	if (GetKeyboardTrigger(DIK_6) == true)
	{
		ModelSpeed += 1.0f;
	}

	if (GetKeyboardTrigger(DIK_7) == true)
	{
		ModelSpeed -= 1.0f;
	}

	// モーションカウンターの加算
	g_player.nCounterMotion++;

	// モーションの更新
	UpdateMotion(g_player.motionType);

	// 移動量の加算
	g_player.pos += g_player.move;

	// 影の追従
	//SetPositionShadow(g_player.nIdx, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	// モデルの移動
	if (GetKeyboardPress(DIK_W) == true || IsButtonPush(KEYINFO_LEFT))
	{
		g_player.motionType = MOTIONTYPE_MOVE;

		if (GetKeyboardPress(DIK_A) == true)
		{// 左上方向
			g_player.move.x -= cosf(pCamera->rot.y + D3DX_PI / 4) * MODEL_MOVE;
			g_player.move.z += sinf(pCamera->rot.y + D3DX_PI / 4) * MODEL_MOVE;
			g_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{// 右上方向
			g_player.move.x += cosf(pCamera->rot.y - D3DX_PI / 4) * MODEL_MOVE;
			g_player.move.z -= sinf(pCamera->rot.y - D3DX_PI / 4) * MODEL_MOVE;
			g_player.rotDest.y = pCamera->rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{// 上方向
			g_player.move.x += sinf(pCamera->rot.y) * MODEL_MOVE;
			g_player.move.z += cosf(pCamera->rot.y) * MODEL_MOVE;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_player.motionType = MOTIONTYPE_MOVE;

		if (GetKeyboardPress(DIK_D) == true)
		{// 右上方向
			g_player.move.x += cosf(pCamera->rot.y + D3DX_PI / 4) * MODEL_MOVE;
			g_player.move.z -= sinf(pCamera->rot.y + D3DX_PI / 4) * MODEL_MOVE;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI / -4;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{// 左上方向
			g_player.move.x -= cosf(pCamera->rot.y - D3DX_PI / 4) * MODEL_MOVE;
			g_player.move.z += sinf(pCamera->rot.y - D3DX_PI / 4) * MODEL_MOVE;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI / 4;
		}
		else
		{// 下方向
			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI) * MODEL_MOVE;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI) * MODEL_MOVE;
			g_player.rotDest.y = pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_player.motionType = MOTIONTYPE_MOVE;

		// 左方向
		g_player.move.x -= cosf(pCamera->rot.y) * MODEL_MOVE;
		g_player.move.z += sinf(pCamera->rot.y) * MODEL_MOVE;
		g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / 2);
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_player.motionType = MOTIONTYPE_MOVE;

		// 右方向
		g_player.move.x += cosf(pCamera->rot.y) * MODEL_MOVE;
		g_player.move.z -= sinf(pCamera->rot.y) * MODEL_MOVE;
		g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / -2);
	}

	else
	{
		g_player.motionType = MOTIONTYPE_NEUTRAL;
	}
	
	if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_D) == true || GetKeyboardTrigger(DIK_A) == true || IsButtonDown(KEYINFO_LEFT))
	{
		g_player.nCounterMotion = 0;
		g_player.aModel[0].pos.y = 55.0f;
		g_player.aModel[0].pos.x = 0.0f;
		g_player.aModel[0].pos.z = 0.0f;
	}

	if (GetKeyboardRelease(DIK_W) == true || GetKeyboardRelease(DIK_S) == true || GetKeyboardRelease(DIK_D) == true || GetKeyboardRelease(DIK_A) == true || IsButtonUp(KEYINFO_LEFT))
	{
		g_player.nCounterMotion = 0;
		g_player.nKey = 0;
		g_player.aModel[0].pos.y = 55.0f;
		g_player.aModel[0].pos.x = 0.0f;
		g_player.aModel[0].pos.z = 0.0f;
		g_player.motionType = MOTIONTYPE_NEUTRAL;
	}

	// 目的の回転角の上限
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.1f;

	// 現在の回転角の上限
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// 加速後の減速処理
	g_player.move.x += (0.0f - g_player.move.x) * SPEEDDOWN;
	g_player.move.z += (0.0f - g_player.move.z) * SPEEDDOWN;


	//オブジェクトの当たり判定
	//CollisionVec(&g_player.pos, &g_player.posOld, &g_player.move, g_vtxMaxplayer.x, g_vtxMinplayer.x, g_vtxMaxplayer.z, g_vtxMinplayer.z, g_vtxMaxplayer.y, g_vtxMinplayer.y);
	
	//フィールドの当たり判定
	//CollisionVecMesh(&g_player.pos, &g_player.posOld, &g_player.move, g_vtxMaxplayer.x, g_vtxMinplayer.x, g_vtxMaxplayer.z, g_vtxMinplayer.z, g_vtxMaxplayer.y, g_vtxMinplayer.y);
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

												// プレイヤーのワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// プレイヤーの向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// プレイヤーの位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// プレイヤーのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX mtxParent;					// 親のマトリックス

		// 各パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		// 各パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// 各パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// 各パーツの親マトリックスを設定
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
			//if (g_player.aModel[nCntModel].nIdxModelParent == 0)
			//{// 親が0の時
			//	mtxParent = g_player.aModel[0].mtxWorld;
			//}
			//else if (g_player.aModel[nCntModel].nIdxModelParent != 0)
			//{// 親が0以外の時
			//	mtxParent = g_player.aModel[nCntModel - 1].mtxWorld;
			//}
		}
		else
		{
			mtxParent = g_player.mtxWorld;	// プレイヤーのマトリックスを設定
		}

		// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		// 各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデル(パーツ)の描画
			g_player.aModel[nCntModel].MeshModel->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// モデルの取得
//==============================================================================
PLAYER *GetModel(void)
{
	return &g_player;
}

//==============================================================================
// モーションの設定
//==============================================================================
void SetMotion(MOTIONTYPE motionType)
{
	//g_player.motionType = motionType;

	if (motionType == MOTIONTYPE_NEUTRAL)
	{// 待機モーション
		g_player.bLoopMotion = true;	// ループする
		g_player.nNumKey = 2;	// キー数
		g_player.aMotionInfo[0].aKeySet[0].nFrame = 40;	// フレーム数

		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fRotX = -0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fRotX = -0.09f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fRotX = 0.3f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fRotY = -0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fRotZ = 1.48f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fRotY = -0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fRotZ = 0.06f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fRotY = -0.22f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fRotZ = 0.16f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fRotY = -0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fRotZ = -1.48f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fRotY = 0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fRotZ = -0.12f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fRotY = 0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fRotZ = -0.16f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fRotX = 0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fRotZ = -0.1f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fRotX = -0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fRotY = -0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fRotZ = 0.06f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fRotX = 0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fRotZ = 0.06f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fRotX = 0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fRotZ = 0.13f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fRotX = -0.13f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fRotY = 0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fRotZ = -0.06f;

		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fRotX = 0.03f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fRotY = 0.00f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fRotZ = -0.06f;

		g_player.aMotionInfo[0].aKeySet[1].nFrame = 40;	// フレーム数

		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fRotX = -0.03f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fRotX = 0.03f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fRotX = -0.09f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fRotX = -0.03f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fRotY = 0.19f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fRotZ = 1.55f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fRotY = -0.13f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fRotZ = 0.06f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fRotY = -0.22f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fRotZ = -0.03f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fRotY = -0.19f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fRotZ = -1.51f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fRotY = 0.13f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fRotZ = -0.06f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fRotY = 0.13f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fRotZ = 0.03f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fRotX = 0.09f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fRotY = 0.13f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fRotZ = -0.06f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fRotX = -0.06f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fRotY = -0.03f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fRotZ = 0.06f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fRotX = -0.06f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fRotZ = 0.06f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fRotX = 0.06f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fRotZ = 0.1f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fRotX = -0.06f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fRotY = 0.03f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fRotZ = -0.06f;

		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fRotX = -0.06f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fRotZ = -0.06f;
	}
	else if (g_player.motionType == MOTIONTYPE_MOVE)
	{// 移動モーション
		g_player.bLoopMotion = true;					// ループする
		g_player.nNumKey = 4;							// キー数
		g_player.aMotionInfo[1].aKeySet[0].nFrame = 4;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fPosY = 3.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fRotY = -0.16f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fRotY = -0.16f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fRotX = 0.44f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fRotY = 0.22f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fRotX = -0.6f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fRotZ = 1.48f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fRotY = -1.07f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fRotX = 0.69f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fRotY = 0.09f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fRotZ = -1.19f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fRotY = 0.79f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fRotZ = -0.35f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fRotX = -0.25f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fRotX = -0.53f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fRotX = -0.50f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fRotX = 1.29f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fRotX = -0.79f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fRotX = 0.16f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].nFrame = 10;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fPosY = -3.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fRotY = -0.16f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fRotY = -0.28f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fRotX = 0.51f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fRotY = 0.32f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fRotZ = 1.38f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fRotY = -1.57f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fRotZ = -0.22f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fRotZ = -1.38f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fRotY = 1.51f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fRotX = -0.50f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fRotX = -1.1f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fRotX = 0.5f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fRotX = 0.85f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fRotX = -0.35f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fRotX = -0.44f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fRotZ = 0.0f;


		g_player.aMotionInfo[1].aKeySet[2].nFrame = 8;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fPosY = 1.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fPosZ = -1.6f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fRotY = 0.16f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fRotY = 0.16f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fRotZ = 0.f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fRotX = 0.41f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fRotY = -0.22f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fRotX = -0.31f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fRotY = 0.19f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fRotZ = 1.29f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fRotY = -1.6f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fRotX = 0.88f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fRotZ = -0.97f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fRotY = 0.75f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fRotZ = -0.5f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fRotX = 1.29f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fRotX = -0.79f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fRotX = 0.16f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fRotX = -0.25f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fRotX = -0.53f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fRotX = -0.5f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].nFrame = 10;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fPosY = -3.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fRotY = 0.16f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fRotX = -0.28f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fRotY = 0.28f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fRotX = 0.41f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fRotY = -0.25f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fRotX = -0.57f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fRotY = 0.25f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fRotZ = 0.97f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fRotY = -1.35f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fRotZ = 0.35f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fRotZ = -1.01f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fRotY = 1.13f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fRotX = 0.85f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fRotX = -0.35f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fRotX = -0.44f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fRotX = -0.5f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fRotX = -1.1f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fRotX = -0.5f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fRotZ = 0.0f;
	}
}

//==============================================================================
// モーションの更新
//==============================================================================
void UpdateMotion(MOTIONTYPE motionType)
{
	SetMotion(motionType);

	for (int nCnt = 0; nCnt < MODEL_PARTS; nCnt++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
		{
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			//g_player.aModel[nCnt].rot.x = pKey->fPosX + (pKeyTop->fPosX - pKey->fPosX) * (fCntMotion / fFrame[g_player.nKey]);
			//g_player.aModel
			//g_player.aModel[nCnt].rot.x = rot[g_player.nKey][nCnt].x + (rot[0][nCnt].x - rot[g_player.nKey][nCnt].x) * (fCntMotion / fFrame[g_player.nKey]);
			//g_player.aModel[nCnt].rot.y = rot[g_player.nKey][nCnt].y + (rot[0][nCnt].y - rot[g_player.nKey][nCnt].y) * (fCntMotion / fFrame[g_player.nKey]);
			//g_player.aModel[nCnt].rot.z = rot[g_player.nKey][nCnt].z + (rot[0][nCnt].z - rot[g_player.nKey][nCnt].z) * (fCntMotion / fFrame[g_player.nKey]);
		}
		else
		{
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			//g_player.aModel[nCnt].rot.x = rot[g_player.nKey][nCnt].x + (rot[g_player.nKey + 1][nCnt].x - rot[g_player.nKey][nCnt].x) * (fCntMotion / fFrame[g_player.nKey]);
			//g_player.aModel[nCnt].rot.y = rot[g_player.nKey][nCnt].y + (rot[g_player.nKey + 1][nCnt].y - rot[g_player.nKey][nCnt].y) * (fCntMotion / fFrame[g_player.nKey]);
			//g_player.aModel[nCnt].rot.z = rot[g_player.nKey][nCnt].z + (rot[g_player.nKey + 1][nCnt].z - rot[g_player.nKey][nCnt].z) * (fCntMotion / fFrame[g_player.nKey]);


			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
			{
				break;
			}
		}

		if (g_player.nCounterMotion == g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame)
		{
			g_player.nCounterMotion = 0;

			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
			{
				g_player.nKey = 0;
			}
			else
			{
				g_player.nKey++;
			}
		}
	}
}