//==============================================================================
//
// 3Dモデルの描画〔item.cpp〕
// AUTHOR : TAKAHASHI SHOTA
//
//==============================================================================
#include "item.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "math.h"
#include "game.h"
#include "effect.h"
#include "car.h"
#include "score.h"
#include "fade.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ITEM_TYPE		(1)								// タイプ数

//==============================================================================
// グローバル変数
//==============================================================================
LPD3DXMESH g_pMeshItem[MAX_ITEM_TYPE] = {};				// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatItem[MAX_ITEM_TYPE] = {};		// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatItem = 10;									// マテリアルの数
ITEM item[MAX_ITEM];
int g_nCnyDateItem;
int g_nDateItemNumberItem = 0;
int g_nDateItemNumberItem2 = 0;
int MaxItem = 0;
int MaxItemType = 0;
int nItemScoe = 0;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitItem(void)
{
	//クリア条件の初期化
	nItemScoe = 0;

	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	//グローバル変数に記憶させる
	MaxItem = MAX_ITEM;
	MaxItemType = MAX_ITEM_TYPE;

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/baloon01.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatItem[0], NULL, &g_nNumMatItem, &g_pMeshItem[0]);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 頂点数を取得
		item[nCntItem].nNumVtx = g_pMeshItem[item[nCntItem].nType]->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		item[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_pMeshItem[item[nCntItem].nType]->GetFVF());

		// 変数の初期化
		item[nCntItem].pos = D3DXVECTOR3(0.0f, -500.0f, 0.0f);
		item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[nCntItem].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		item[nCntItem].bUse = false;
		item[nCntItem].nType = 0;
	}
	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitItem(void)
{
	// メッシュの破棄
	if (g_pMeshItem[0] != NULL)
	{
		g_pMeshItem[0]->Release();
		g_pMeshItem[0] = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_ITEM_TYPE; nCntTexture++)
	{
		// マテリアルの破棄
		if (g_pBuffMatItem[nCntTexture] != NULL)
		{
			g_pBuffMatItem[nCntTexture]->Release();
			g_pBuffMatItem[nCntTexture] = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateItem(void)
{

	if (GetKeyboardTrigger(DIK_Y) == true)
	{
		item[g_nDateItemNumberItem - g_nDateItemNumberItem2].bUse = false;
		g_nDateItemNumberItem2 += 1;
	}

	if (GetKeyboardTrigger(DIK_T) == true)
	{
		g_nDateItemNumberItem2 = 0;
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 頂点数を取得
		item[nCntItem].nNumVtx = g_pMeshItem[item[nCntItem].nType]->GetNumVertices();
		//item[nCntItem].nNumVtx = g_pMeshItem[item[0].nType]->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		item[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_pMeshItem[item[nCntItem].nType]->GetFVF());

		// 頂点バッファをロック
		g_pMeshItem[item[nCntItem].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&item[nCntItem].pVtxBuff);

		// 頂点座標の比較
		for (int nCntVtx = 0; nCntVtx < item[nCntItem].nNumVtx; nCntVtx++)
		{
			item[nCntItem].vtx = *(D3DXVECTOR3*)item[nCntItem].pVtxBuff;	// 頂点座標の代入

			// 各座標の最大値の比較
			if (item[nCntItem].vtxMaxItem.x < item[nCntItem].vtx.x)
			{
				item[nCntItem].vtxMaxItem.x = item[nCntItem].vtx.x;
			}
			if (item[nCntItem].vtxMaxItem.y < item[nCntItem].vtx.y)
			{
				item[nCntItem].vtxMaxItem.y = item[nCntItem].vtx.y;
			}
			if (item[nCntItem].vtxMaxItem.z < item[nCntItem].vtx.z)
			{
				item[nCntItem].vtxMaxItem.z = item[nCntItem].vtx.z;
			}

			// 各座標の最小値の比較
			if (item[nCntItem].vtxMinItem.x > item[nCntItem].vtx.x)
			{
				item[nCntItem].vtxMinItem.x = item[nCntItem].vtx.x;
			}
			if (item[nCntItem].vtxMinItem.y > item[nCntItem].vtx.y)
			{
				item[nCntItem].vtxMinItem.y = item[nCntItem].vtx.y;
			}
			if (item[nCntItem].vtxMinItem.z > item[nCntItem].vtx.z)
			{
				item[nCntItem].vtxMinItem.z = item[nCntItem].vtx.z;
			}

			item[nCntItem].pVtxBuff += item[nCntItem].sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		g_pMeshItem[item[nCntItem].nType]->UnlockVertexBuffer();
	}

	//クリア条件に達したら
	if (nItemScoe == 15)
	{
		//SetFade(FADE_OUT, MODE_RESULT);
		SetFade(FADE_OUT, MODE_RANKING);
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawItem(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&item[nCntItem].mtxWorld);

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, item[nCntItem].rot.y, item[nCntItem].rot.x, item[nCntItem].rot.z);
		D3DXMatrixMultiply(&item[nCntItem].mtxWorld, &item[nCntItem].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, item[nCntItem].pos.x, item[nCntItem].pos.y, item[nCntItem].pos.z);
		D3DXMatrixMultiply(&item[nCntItem].mtxWorld, &item[nCntItem].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &item[nCntItem].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		//pMat = (D3DXMATERIAL*)g_pBuffMatItem[0]->GetBufferPointer();

		pMat = (D3DXMATERIAL*)g_pBuffMatItem[item[nCntItem].nType]->GetBufferPointer();

		//使用されているとき
		if (item[nCntItem].bUse == true)
		{
			for (int nCntMat = 0; nCntMat < (int)g_nNumMatItem; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);
				//pDevice->SetTexture(0, NULL);

				// モデル(パーツ)の描画
				g_pMeshItem[item[nCntItem].nType]->DrawSubset(nCntMat);

				// 保存していたマテリアルを戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVecItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ローカル変数宣言
	bool bLand = false;
	D3DXVECTOR3 aVec[COLLISION_PARTS];	// 矩形頂点から判定対象へのベクトル
	float fPlayerVec[COLLISION_PARTS];	// 各面のベクトルとの判定（＋－）
	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置

	CAR  *pCar;
	pCar = GetCar();

	int nCntItem = 0;
	int nCntItemPlayerVec = 0;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// オブジェクト矩形の各頂点
		item[nCntItem].aPos[0 + (4 * nCntItem)] = D3DXVECTOR3(item[nCntItem].pos.x + item[nCntItem].vtxMinItem.x, 0.0f, item[nCntItem].pos.z + item[nCntItem].vtxMinItem.z);	// 左手前
		item[nCntItem].aPos[1 + (4 * nCntItem)] = D3DXVECTOR3(item[nCntItem].pos.x + item[nCntItem].vtxMinItem.x, 0.0f, item[nCntItem].pos.z + item[nCntItem].vtxMaxItem.z);	// 左奥
		item[nCntItem].aPos[2 + (4 * nCntItem)] = D3DXVECTOR3(item[nCntItem].pos.x + item[nCntItem].vtxMaxItem.x, 0.0f, item[nCntItem].pos.z + item[nCntItem].vtxMaxItem.z);	// 右奥
		item[nCntItem].aPos[3 + (4 * nCntItem)] = D3DXVECTOR3(item[nCntItem].pos.x + item[nCntItem].vtxMaxItem.x, 0.0f, item[nCntItem].pos.z + item[nCntItem].vtxMinItem.z);	// 右手前
																																												// オブジェクト矩形の各面のベクトル
		item[nCntItem].aVec[0 + (4 * nCntItem)] = item[nCntItem].aPos[1 + (4 * nCntItem)] - item[nCntItem].aPos[0 + (4 * nCntItem)];
		item[nCntItem].aVec[1 + (4 * nCntItem)] = item[nCntItem].aPos[2 + (4 * nCntItem)] - item[nCntItem].aPos[1 + (4 * nCntItem)];
		item[nCntItem].aVec[2 + (4 * nCntItem)] = item[nCntItem].aPos[3 + (4 * nCntItem)] - item[nCntItem].aPos[2 + (4 * nCntItem)];
		item[nCntItem].aVec[3 + (4 * nCntItem)] = item[nCntItem].aPos[0 + (4 * nCntItem)] - item[nCntItem].aPos[3 + (4 * nCntItem)];
	}

	for (int nCntItemUse = 0; nCntItemUse < MAX_ITEM; nCntItemUse++)
	{
		//使用されているとき
		if (item[nCntItemUse].bUse == true)
		{
			//初期化処理
			g_nCnyDateItem = 0;
			nCntItem = 0;

			// オブジェクトとの当たり判定
			for (int nCnt = 0; nCnt < COLLISION_PARTS; nCnt++)
			{
				if (nCnt == 0 + g_nCnyDateItem)
				{// 左面
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - item[nCntItem].aPos[nCnt];
				}
				else if (nCnt == 1 + g_nCnyDateItem)
				{// 奥側
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - item[nCntItem].aPos[nCnt];
				}
				else if (nCnt == 2 + g_nCnyDateItem)
				{// 右面
					aVec[nCnt] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - item[nCntItem].aPos[nCnt];
				}
				else if (nCnt == 3 + g_nCnyDateItem)
				{// 手前
					aVec[nCnt] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - item[nCntItem].aPos[nCnt];
				}

				// 数値が「+」の時、当たり判定が有効
				fPlayerVec[nCnt] = (item[nCntItem].aVec[nCnt].z * aVec[nCnt].x) - (item[nCntItem].aVec[nCnt].x * aVec[nCnt].z);

				if (pPos->y <= item[nCntItem].pos.y + item[nCntItem].vtxMaxItem.y - fHeightMin && pPos->y > item[nCntItem].pos.y + item[nCntItem].vtxMinItem.y - fHeightMax)
				{
					if (fPlayerVec[0 + (4 * nCntItem)] > 0.0f && fPlayerVec[1 + (4 * nCntItem)] > 0.0f && fPlayerVec[2 + (4 * nCntItem)] > 0.0f && fPlayerVec[3 + (4 * nCntItem)] > 0.0f)
					{
						if (item[nCntItem].bUse == true)
						{
							//アイテムを消す
							item[nCntItem].bUse = false;

							//スコア加算
							AddScore(1);

							//クリア条件
							nItemScoe++;
						}
					}
				}

				//次のオブジェクトの当たり判定
				if (nCnt == 3 + (4 * nCntItem))
				{
					//条件を変える
					g_nCnyDateItem += 4;

					//次のオブジェクトへ
					nCntItem += 1;
				}
			}
		}
	}

	return bLand;
}

//==============================================================================
// オブジェクトの設定
//==============================================================================
void SetItem(D3DXVECTOR3 Pos, D3DXVECTOR3 rot, int nType)
{
	//VERTEX_3D *pVtx;
	//LPDIRECT3DDEVICE9 pDevice;

	////デバイスの取得
	//pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		// 頂点バッファをロック
		g_pMeshItem[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&item[nCntItem].pVtxBuff);

		//使用されているとき
		if (item[nCntItem].bUse == false)
		{
			//位置
			item[nCntItem].pos = Pos;

			//向き
			item[nCntItem].rot = rot;

			//タイプ
			item[nCntItem].nType = nType;

			//使用する
			item[nCntItem].bUse = true;

			//一つ前のオブジェクトを記憶
			g_nDateItemNumberItem = nCntItem;

			break;
		}
	}
	// 頂点バッファをアンロック
	g_pMeshItem[0]->UnlockVertexBuffer();
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetitemNumber(void)
{
	return MaxItem;
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetitemDate(void)
{
	return g_nDateItemNumberItem2;
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetitemType(void)
{
	return MaxItemType;
}