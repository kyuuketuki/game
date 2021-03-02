//==============================================================================
//
// 3Dモデルの描画〔object.cpp〕
// AUTHOR : TAKAHASHI SHOTA
//
//==============================================================================
#include "object.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "math.h"
#include "game.h"
#include "effect.h"
#include "car.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_OBJECT_TYPE		(10)								// タイプ数

//==============================================================================
// グローバル変数
//==============================================================================
LPD3DXMESH g_pMeshObject[MAX_OBJECT_TYPE] = {};				// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatObject[MAX_OBJECT_TYPE] = {};		// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatObject = 0;									// マテリアルの数
OBJECT object[MAX_OBJECT];
int g_nCnyDateObject;
int g_nDateObjectNumber = 0;
int g_nDateObjectNumber2 = 0;
int MaxOject = 0;
int MaxOjectType = 0;
float g_CarSpeed = 0;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitObject(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	//グローバル変数に記憶させる
	MaxOject = MAX_OBJECT;
	MaxOjectType = MAX_OBJECT_TYPE;

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/fence101.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[9], NULL, &g_nNumMatObject, &g_pMeshObject[9]);
	D3DXLoadMeshFromX("data/MODEL/bench.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[8], NULL, &g_nNumMatObject, &g_pMeshObject[8]);
	D3DXLoadMeshFromX("data/MODEL/tree000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[7], NULL, &g_nNumMatObject, &g_pMeshObject[7]);
	D3DXLoadMeshFromX("data/MODEL/glass000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[6], NULL, &g_nNumMatObject, &g_pMeshObject[6]);
	D3DXLoadMeshFromX("data/MODEL/building04.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[5], NULL, &g_nNumMatObject, &g_pMeshObject[5]);
	D3DXLoadMeshFromX("data/MODEL/building03.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[4], NULL, &g_nNumMatObject, &g_pMeshObject[4]);
	D3DXLoadMeshFromX("data/MODEL/tower000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[3], NULL, &g_nNumMatObject, &g_pMeshObject[3]);
	D3DXLoadMeshFromX("data/MODEL/噴水公園.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[2], NULL, &g_nNumMatObject, &g_pMeshObject[2]);
	D3DXLoadMeshFromX("data/MODEL/building00.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[1], NULL, &g_nNumMatObject, &g_pMeshObject[1]);
	D3DXLoadMeshFromX("data/MODEL/baloon01.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[0], NULL, &g_nNumMatObject, &g_pMeshObject[0]);

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		// 頂点数を取得
		object[nCntObject].nNumVtx = g_pMeshObject[object[nCntObject].nType]->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		object[nCntObject].sizeFVF = D3DXGetFVFVertexSize(g_pMeshObject[object[nCntObject].nType]->GetFVF());

		// 変数の初期化
		object[nCntObject].pos = D3DXVECTOR3(0.0f, -500.0f, 0.0f);
		object[nCntObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCntObject].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCntObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		object[nCntObject].bUse = false;
		object[nCntObject].nType = 0;

		//// 頂点バッファをロック
		//g_pMeshObject[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&object[nCntObject].pVtxBuff);

		//// 頂点座標の比較
		//for (int nCntVtx = 0; nCntVtx < object[nCntObject].nNumVtx; nCntVtx++)
		//{
		//	object[nCntObject].vtx = *(D3DXVECTOR3*)object[nCntObject].pVtxBuff;	// 頂点座標の代入

		//	// 各座標の最大値の比較
		//	if (object[nCntObject].vtxMaxObject.x < object[nCntObject].vtx.x)
		//	{
		//		object[nCntObject].vtxMaxObject.x = object[nCntObject].vtx.x;
		//	}
		//	if (object[nCntObject].vtxMaxObject.y < object[nCntObject].vtx.y)
		//	{
		//		object[nCntObject].vtxMaxObject.y = object[nCntObject].vtx.y;
		//	}
		//	if (object[nCntObject].vtxMaxObject.z < object[nCntObject].vtx.z)
		//	{
		//		object[nCntObject].vtxMaxObject.z = object[nCntObject].vtx.z;
		//	}

		//	// 各座標の最小値の比較
		//	if (object[nCntObject].vtxMinObject.x > object[nCntObject].vtx.x)
		//	{
		//		object[nCntObject].vtxMinObject.x = object[nCntObject].vtx.x;
		//	}
		//	if (object[nCntObject].vtxMinObject.y > object[nCntObject].vtx.y)
		//	{
		//		object[nCntObject].vtxMinObject.y = object[nCntObject].vtx.y;
		//	}
		//	if (object[nCntObject].vtxMinObject.z > object[nCntObject].vtx.z)
		//	{
		//		object[nCntObject].vtxMinObject.z = object[nCntObject].vtx.z;
		//	}

		//	object[nCntObject].pVtxBuff += object[nCntObject].sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
		//}
		//// 頂点バッファをアンロック
		//g_pMeshObject[0]->UnlockVertexBuffer();
	}
	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitObject(void)
{
	//// メッシュの破棄
	//if (g_pMeshObject[0] != NULL)
	//{
	//	g_pMeshObject[0]->Release();
	//	g_pMeshObject[0] = NULL;
	//}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_OBJECT_TYPE; nCntTexture++)
	{
		// メッシュの破棄
		if (g_pMeshObject[nCntTexture] != NULL)
		{
			g_pMeshObject[nCntTexture]->Release();
			g_pMeshObject[nCntTexture] = NULL;
		}

		// マテリアルの破棄
		if (g_pBuffMatObject[nCntTexture] != NULL)
		{
			g_pBuffMatObject[nCntTexture]->Release();
			g_pBuffMatObject[nCntTexture] = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateObject(void)
{
	//最新のオブジェクトを消す
	if (GetKeyboardTrigger(DIK_H) == true)
	{
		if (g_nDateObjectNumber <= 0)
		{
			object[g_nDateObjectNumber - g_nDateObjectNumber2].bUse = false;

			//最新のオブジェクトを進める
			g_nDateObjectNumber2 += 1;
		}
	}

	if (GetKeyboardTrigger(DIK_G) == true)
	{
		//最新のオブジェクトを進める
		g_nDateObjectNumber2 = 0;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		// 頂点数を取得
		object[nCntObject].nNumVtx = g_pMeshObject[object[nCntObject].nType]->GetNumVertices();
		//object[nCntObject].nNumVtx = g_pMeshObject[object[0].nType]->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		object[nCntObject].sizeFVF = D3DXGetFVFVertexSize(g_pMeshObject[object[nCntObject].nType]->GetFVF());

		// 頂点バッファをロック
		g_pMeshObject[object[nCntObject].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&object[nCntObject].pVtxBuff);

		// 頂点座標の比較
		for (int nCntVtx = 0; nCntVtx < object[nCntObject].nNumVtx; nCntVtx++)
		{
			object[nCntObject].vtx = *(D3DXVECTOR3*)object[nCntObject].pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
			if (object[nCntObject].vtxMaxObject.x < object[nCntObject].vtx.x)
			{
				object[nCntObject].vtxMaxObject.x = object[nCntObject].vtx.x;
			}
			if (object[nCntObject].vtxMaxObject.y < object[nCntObject].vtx.y)
			{
				object[nCntObject].vtxMaxObject.y = object[nCntObject].vtx.y;
			}
			if (object[nCntObject].vtxMaxObject.z < object[nCntObject].vtx.z)
			{
				object[nCntObject].vtxMaxObject.z = object[nCntObject].vtx.z;
			}

			// 各座標の最小値の比較
			if (object[nCntObject].vtxMinObject.x > object[nCntObject].vtx.x)
			{
				object[nCntObject].vtxMinObject.x = object[nCntObject].vtx.x;
			}
			if (object[nCntObject].vtxMinObject.y > object[nCntObject].vtx.y)
			{
				object[nCntObject].vtxMinObject.y = object[nCntObject].vtx.y;
			}
			if (object[nCntObject].vtxMinObject.z > object[nCntObject].vtx.z)
			{
				object[nCntObject].vtxMinObject.z = object[nCntObject].vtx.z;
			}

			object[nCntObject].pVtxBuff += object[nCntObject].sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		g_pMeshObject[object[nCntObject].nType]->UnlockVertexBuffer();
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawObject(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&object[nCntObject].mtxWorld);

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, object[nCntObject].rot.y, object[nCntObject].rot.x, object[nCntObject].rot.z);
		D3DXMatrixMultiply(&object[nCntObject].mtxWorld, &object[nCntObject].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, object[nCntObject].pos.x, object[nCntObject].pos.y, object[nCntObject].pos.z);
		D3DXMatrixMultiply(&object[nCntObject].mtxWorld, &object[nCntObject].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &object[nCntObject].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//// マテリアルデータへのポインタを取得
		//pMat = (D3DXMATERIAL*)g_pBuffMatObject[0]->GetBufferPointer();

		//if (object[nCntObject].nType == 0)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[0]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 1)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[1]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 2)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[2]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 3)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[3]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 4)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[4]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 5)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[5]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 6)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[6]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 7)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[7]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 8)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[8]->GetBufferPointer();
		//}

		//if (object[nCntObject].nType == 9)
		//{
		//	pMat = (D3DXMATERIAL*)g_pBuffMatObject[9]->GetBufferPointer();
		//}

		pMat = (D3DXMATERIAL*)g_pBuffMatObject[object[nCntObject].nType]->GetBufferPointer();

		//使用されているとき
		if (object[nCntObject].bUse == true)
		{
			for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// モデル(パーツ)の描画
				g_pMeshObject[object[nCntObject].nType]->DrawSubset(nCntMat);
			}
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// オブジェクトの取得
//==============================================================================
OBJECT *GetObject(void)
{
	return object;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ローカル変数宣言
	bool bLand = false;
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル
	float fPlayerVec[COLLISION_PARTS];	// 各面のベクトルとの判定（＋－）
	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置

	//int nCarSpeed = GetCarSpeed();

	CAR  *pCar;
	pCar = GetCar();

	int nCntObject = 0;
	int nCntObjectPlayerVec = 0;

	//for (int nCntObjectUse = 0; nCntObjectUse < MAX_OBJECT; nCntObjectUse++)
	{
		//使用されているとき
		//if (object[nCntObjectUse].bUse == true)
		{
			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{
				// オブジェクト矩形の各頂点
				object[nCntObject].aPos[0 + (4 * nCntObject)] = D3DXVECTOR3(object[nCntObject].pos.x + object[nCntObject].vtxMinObject.x, 0.0f, object[nCntObject].pos.z + object[nCntObject].vtxMinObject.z);	// 左手前
				object[nCntObject].aPos[1 + (4 * nCntObject)] = D3DXVECTOR3(object[nCntObject].pos.x + object[nCntObject].vtxMinObject.x, 0.0f, object[nCntObject].pos.z + object[nCntObject].vtxMaxObject.z);	// 左奥
				object[nCntObject].aPos[2 + (4 * nCntObject)] = D3DXVECTOR3(object[nCntObject].pos.x + object[nCntObject].vtxMaxObject.x, 0.0f, object[nCntObject].pos.z + object[nCntObject].vtxMaxObject.z);	// 右奥
				object[nCntObject].aPos[3 + (4 * nCntObject)] = D3DXVECTOR3(object[nCntObject].pos.x + object[nCntObject].vtxMaxObject.x, 0.0f, object[nCntObject].pos.z + object[nCntObject].vtxMinObject.z);	// 右手前

				// オブジェクト矩形の各面のベクトル
				object[nCntObject].aVec[0 + (4 * nCntObject)] = object[nCntObject].aPos[1 + (4 * nCntObject)] - object[nCntObject].aPos[0 + (4 * nCntObject)];
				object[nCntObject].aVec[1 + (4 * nCntObject)] = object[nCntObject].aPos[2 + (4 * nCntObject)] - object[nCntObject].aPos[1 + (4 * nCntObject)];
				object[nCntObject].aVec[2 + (4 * nCntObject)] = object[nCntObject].aPos[3 + (4 * nCntObject)] - object[nCntObject].aPos[2 + (4 * nCntObject)];
				object[nCntObject].aVec[3 + (4 * nCntObject)] = object[nCntObject].aPos[0 + (4 * nCntObject)] - object[nCntObject].aPos[3 + (4 * nCntObject)];
			}

			//初期化処理
			g_nCnyDateObject = 0;
			nCntObject = 0;


			// オブジェクトとの当たり判定
			for (int nCnt = 0; nCnt < COLLISION_PARTS; nCnt++)
			{
				if (nCnt == 0 + g_nCnyDateObject)
				{// 左面
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - object[nCntObject].aPos[nCnt];
				}
				else if (nCnt == 1 + g_nCnyDateObject)
				{// 奥側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - object[nCntObject].aPos[nCnt];
				}
				else if (nCnt == 2 + g_nCnyDateObject)
				{// 右面
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - object[nCntObject].aPos[nCnt];
				}
				else if (nCnt == 3 + g_nCnyDateObject)
				{// 手前
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - object[nCntObject].aPos[nCnt];
				}

				// 数値が「+」の時、当たり判定が有効
				fPlayerVec[nCnt] = (object[nCntObject].aVec[nCnt].z * aVec[nCnt].x) - (object[nCntObject].aVec[nCnt].x * aVec[nCnt].z);

				if (pPos->y <= object[nCntObject].pos.y + object[nCntObject].vtxMaxObject.y - fHeightMin && pPos->y > object[nCntObject].pos.y + object[nCntObject].vtxMinObject.y - fHeightMax)
				{
					if (fPlayerVec[0 + (4 * nCntObject)] > 0.0f && fPlayerVec[1 + (4 * nCntObject)] > 0.0f && fPlayerVec[2 + (4 * nCntObject)] > 0.0f && fPlayerVec[3 + (4 * nCntObject)] > 0.0f)
						//if (fPlayerVec[4] > 0.0f && fPlayerVec[5] > 0.0f && fPlayerVec[6] > 0.0f && fPlayerVec[7] > 0.0f)
					{
						if (pPos->x > pPosOld->x && pPosOld->x < object[nCntObject].pos.x + object[nCntObject].vtxMinObject.x)
						{
							// 左側	
							pPos->x = object[nCntObject].pos.x + object[nCntObject].vtxMinObject.x - fWidthMax;
							SetWater(D3DXVECTOR3(pCar->pos.x + 40.0f, pCar->pos.y, pCar->pos.z));
							//pCar->move = pCar->move / 2;
							pCar->nSpeed = 0.0f;
						}
						else if (pPos->x < pPosOld->x && pPosOld->x > object[nCntObject].pos.x + object[nCntObject].vtxMaxObject.x)
						{
							// 右側	
							pPos->x = object[nCntObject].pos.x + object[nCntObject].vtxMaxObject.x - fWidthMin;
							SetWater(D3DXVECTOR3(pCar->pos.x - 40.0f, pCar->pos.y, pCar->pos.z));
							//pCar->move = pCar->move / 2;
							pCar->nSpeed = 0.0f;
						}
						else if (pPos->z <= pPosOld->z && pPos->z > object[nCntObject].pos.z + object[nCntObject].vtxMaxObject.z)
						{
							// 奥側
							pPos->z = object[nCntObject].pos.z + object[nCntObject].vtxMaxObject.z - fDepthMin;
							SetWater(D3DXVECTOR3(pCar->pos.x, pCar->pos.y, pCar->pos.z - 40.0f));
							//pCar->move = pCar->move / 2;
							pCar->nSpeed = 0.0f;
						}
						else if (pPos->z >= pPosOld->z && pPos->z < object[nCntObject].pos.z + object[nCntObject].vtxMinObject.z)
						{
							// 手前
							pPos->z = object[nCntObject].pos.z + object[nCntObject].vtxMinObject.z - fDepthMax;
							SetWater(D3DXVECTOR3(pCar->pos.x, pCar->pos.y, pCar->pos.z + 40.0f));
							//pCar->move = pCar->move / 2;
							pCar->nSpeed = 0.0f;
						}
					}
				}

				//次のオブジェクトの当たり判定
				if (nCnt == 3 + (4 * nCntObject))
				{
					//条件を変える
					g_nCnyDateObject += 4;

					//次のオブジェクトへ
					nCntObject += 1;
				}
			}
		}
	}

	return bLand;
}

//==============================================================================
// オブジェクトの設定
//==============================================================================
void SetOject(D3DXVECTOR3 Pos, D3DXVECTOR3 rot, int nType)
{
	//VERTEX_3D *pVtx;
	//LPDIRECT3DDEVICE9 pDevice;

	////デバイスの取得
	//pDevice = GetDevice();

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{

		// 頂点バッファをロック
		g_pMeshObject[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&object[nCntObject].pVtxBuff);

		//使用されているとき
		if (object[nCntObject].bUse == false)
		{
			//位置
			object[nCntObject].pos = Pos;

			//向き
			object[nCntObject].rot = rot;

			//タイプ
			object[nCntObject].nType = nType;

			//使用する
			object[nCntObject].bUse = true;

			//一つ前のオブジェクトを記憶
			g_nDateObjectNumber = nCntObject;

			break;
		}
	}
	// 頂点バッファをアンロック
	g_pMeshObject[0]->UnlockVertexBuffer();
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetobjectNumber(void)
{
	return MaxOject;
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetobjectDate(void)
{
	return g_nDateObjectNumber2;
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetobjectType(void)
{
	return MaxOjectType;
}