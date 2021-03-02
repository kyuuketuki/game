//==============================================================================
//
// メッシュフィールドの描画〔meshfield.cpp〕
// AUTHOR : TAKAHASHI SHOTA
//
//==============================================================================
#include "meshfield.h"
#include "model.h"
#include "car.h"
#include "input.h"
#include "score.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define WIDTHNUM	(100)									// 列数(幅)
#define DEPTHNUM	(100)									// 行数(奥行)
#define WIDTH		(100.0f)								// 幅の大きさ
#define DEPTH		(100.0f)								// 奥行の大きさ
//#define DATE_CAR	(g_CarDate)								// プレイヤーの情報をもとに


//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	// バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;	// インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;		// テクスチャへのポインタ
D3DXVECTOR3 g_posMeshfield;							// 位置
D3DXVECTOR3 g_rotMeshfield;							// 向き
D3DXMATRIX g_mtxWorldMeshfield;						// ワールドマトリックス
MeshField g_aField[FIELD_MAX];						//フィールドの情報
int g_nAllPoint;									// 総頂点数
int g_nPolygon;										// ポリゴン数
int g_nIdxPoint;									// インデックスバッファの必要な確保数
float g_fWidth = WIDTH;								// 横幅の端
float g_fDepth = DEPTH;								// 奥行の端
int g_nWidth = WIDTHNUM + 1;						// 横幅の頂点数
int g_nDepth = DEPTHNUM + 1;						// 奥行の頂点数
//float g_SpeedCarDate;
//float g_CarDate;
									
//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitMeshfield(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/field2.jpg",&g_pTextureMeshfield);

	// 変数の初期化
	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshfield = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	g_nAllPoint = DEPTHNUM * 2 * (WIDTHNUM + 2) - 2;				// 総頂点数
	g_nPolygon = WIDTHNUM * DEPTHNUM * 2 + (4 * (DEPTHNUM - 1));	// 総ポリゴン数
	g_nIdxPoint = g_nWidth * g_nDepth;								// インデックスバッファでの頂点数

	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++)
	{
		g_aField[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCnt].g_rotMeshfield = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		g_aField[nCnt].g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aField[nCnt].nAllPoint = 0;
		g_aField[nCnt].nPolygon = 0;
		g_aField[nCnt].nIdxPoint = 0;
		g_aField[nCnt].bUse = false;
	}

	float fWidthMax = g_fWidth * (float)WIDTHNUM;
	float fDepthMax = g_fDepth * (float)DEPTHNUM;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nIdxPoint,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_nDepth; nCnt++)
	{// 奥行軸
		for (int nCntA = 0; nCntA < g_nWidth; nCntA++ )
		{

			// 横軸
			// ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(-fWidthMax / 2.0f + (float)nCntA * g_fWidth, g_posMeshfield.y, fDepthMax / 2.0f - (float)nCnt * g_fDepth);

			//pVtx[0].pos = D3DXVECTOR3(g_aField[nCntA].g_posMeshfield.x + (float)nCntA * g_fWidth, g_posMeshfield.y, g_aField[nCntA].g_posMeshfield.z - (float)nCnt * g_fDepth);
			
			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ頂点情報の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * g_nAllPoint,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield,
		NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < DEPTHNUM; nCntA++)
	{
		for (int nCnt = 0; nCnt < g_nWidth + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == g_nWidth && nCntA != DEPTHNUM - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == DEPTHNUM - 1 && nCnt == g_nWidth)
			{// 終了時に無視する
				break;
			}
			else
			{// 通常配置
				pIdx[0] = g_nWidth + (g_nWidth * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - g_nWidth;
			}
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitMeshfield(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	// テクスチャの開放
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateMeshfield(void)
{
	//CAR  *pCar;
	//pCar = GetCar();

	//g_SpeedCarDate = GetCarSpeed();

	//if (g_SpeedCarDate >= 0.2f)
	//{
	//	g_CarDate = 0.8f;
	//	AddScore(1);
	//}

	//if (g_SpeedCarDate >= 0.4f)
	//{
	//	g_CarDate = 1.5f;
	//	AddScore(2);
	//}

	//if (g_SpeedCarDate >= 0.6f)
	//{
	//	g_CarDate = 2.2f;
	//	AddScore(3);
	//}

	//if (g_SpeedCarDate >= 0.8f)
	//{
	//	g_CarDate = 2.9f;
	//	AddScore(4);
	//}

	//if (g_SpeedCarDate >= 1.0f)
	//{
	//	g_CarDate = 3.6f;
	//	AddScore(5);
	//}
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawMeshfield(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	MeshField *pField;
	pField = &g_aField[0];

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, pField++)
	{
		if (pField->bUse == true)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldMeshfield);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pField->g_rotMeshfield.y, pField->g_rotMeshfield.x, pField->g_rotMeshfield.z);
			D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pField->g_posMeshfield.x, pField->g_posMeshfield.y, pField->g_posMeshfield.z);
			D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshfield);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshfield);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				0,
				0,
				g_nIdxPoint,			// 頂点数
				0,
				g_nPolygon);			// プリミティブ数
		}
	}
}

MeshField * GetMesh(void)
{
	return &g_aField[0];
}

//==============================================================================
// フィールドの設定
//==============================================================================
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float X, float Z)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;

	MeshField *pField;
	pField = &g_aField[0];

	// フィールドの設定
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, pField++)
	{
		if (pField->bUse == false)
		{
			//向き
			pField->g_rotMeshfield = rot;
			
			//位置
			pField->g_posMeshfield = pos;

			//if (pField->g_rotMeshfield.y == 0.0f && pField->g_rotMeshfield.z == 0.2f)
			//{
			//	pField->nType = 1;
			//}

			//if (pField->g_rotMeshfield.y == D3DX_PI)
			//{
			//	pField->nType = 2;
			//}

			//if (pField->g_rotMeshfield.y == D3DX_PI / 2)
			//{
			//	pField->nType = 3;
			//}

			//if (pField->g_rotMeshfield.y == -D3DX_PI / 2)
			//{
			//	pField->nType = 4;
			//}

			pField->posM = pos;

			//使用していいるか
			pField->bUse = true;

			break;
		}
	}
}

//==============================================================================
// フィールドの当たり判定
//==============================================================================
void CollisionVecMesh(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	//// ローカル変数宣言
	//VERTEX_3D *pVtx;

	//// 判定対象の位置
	//D3DXVECTOR3 pos = *pPos;
	//D3DXVECTOR3 posOld = *pPosOld;

	//MeshField *pField;
	//pField = &g_aField[0];

	//PLAYER *pModel;
	//pModel = GetModel();

	//CAR  *pCar;
	//pCar = GetCar();

	//// 頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//if (pField->bUse == true)
	//{
	//	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, pField++)
	//	{
	//		if (pField->nType == 0)
	//		{
	//			//当たり判定　平面
	//			if (pField->posM.x <= pos.x && (pField->posM.x + 1000.0f) >= pos.x &&
	//				pField->posM.z >= pos.z && (pField->posM.z - 400.0f) <= pos.z&&
	//				pField->posM.y + 10 >= pos.y && (pField->posM.y - 10.0f) <= pos.y)
	//			{
	//				//判定内の時の処理　浮上
	//				pModel->pos.y = pModel->posOld.y;
	//				pCar->pos.y = pCar->posOld.y;
	//				pos.y = posOld.y;
	//				break;
	//			}

	//			//重力
	//			else
	//			{
	//				pModel->pos.y -= 0.1f;
	//				pCar->pos.y -= 0.1f;
	//				pos.y -= 0.1f;
	//				//break;
	//			}
	//		}

	//		if (pField->nType == 1)
	//		{
	//			//当たり判定
	//			if (pField->posM.x <= pos.x && (pField->posM.x + 1000.0f) >= pos.x &&
	//				pField->posM.z >= pos.z && (pField->posM.z - 400.0f) <= pos.z)
	//			{
	//				//重力
	//				//pModel->move.y += 0.0f;

	//				//ベクトルの計算保存
	//				pField->vec = D3DXVECTOR3(pField->posM.x + 1000.0f, pField->posM.y + 200.0f, pField->posM.z);
	//				pField->vecA = pField->vec - pField->posM;
	//				pField->vecB = pos - pField->posM;

	//				//ベクトルの計算保存
	//				pField->vec2 = D3DXVECTOR3(pField->posM.x + 1000.0f, pField->posM.y + 210.0f, pField->posM.z);
	//				pField->vec3 = D3DXVECTOR3(pField->posM.x, pField->posM.y + 10.0f, pField->posM.z);
	//				pField->vec2A = pField->vec2 - pField->vec3;
	//				pField->vec2B = pos - pField->vec3;

	//				//当たり判定
	//				if (((pField->vec2A.x * pField->vec2B.y) - (pField->vec2A.y * pField->vec2B.x)) < 0)
	//				{
	//					//当たり判定
	//					if (((pField->vecA.x * pField->vecB.y) - (pField->vecA.y * pField->vecB.x)) < 0)
	//					{
	//						//0以下になったら浮上
	//						pModel->pos.y = pModel->posOld.y + 0.8f;
	//						pCar->pos.y = pCar->posOld.y + DATE_CAR;
	//						//pos.y = posOld.y + 0.8f;
	//						break;
	//					}


	//					else
	//					{

	//						//0以上になったら下降
	//						pModel->pos.y = pModel->posOld.y - 0.8f;
	//						pCar->pos.y = pCar->posOld.y - DATE_CAR;
	//						//pos.y = posOld.y - 0.8f;
	//						break;
	//					}

	//				}
	//			}

	//			//重力
	//			else
	//			{
	//				pModel->pos.y -= 1.0f;
	//				pCar->pos.y -= 1.0f;
	//				//pos.y -= 1.0f;
	//			}
	//		}

	//		//向きによって
	//		if (pField->nType == 2)
	//		{

	//			if (pField->posM.x - 1000.0f <= pos.x && pField->posM.x >= pos.x &&
	//				pField->posM.z + 400.0f >= pos.z && pField->posM.z <= pos.z)
	//			{
	//				//ベクトルの計算保存
	//				pField->vec = D3DXVECTOR3(pField->posM.x - 1000.0f, pField->posM.y + 200.0f, pField->posM.z);
	//				pField->vecA = pField->posM - pField->vec;
	//				pField->vecB = pos - pField->vec;

	//				//ベクトルの計算保存
	//				pField->vec2 = D3DXVECTOR3(pField->posM.x, pField->posM.y + 10.0f, pField->posM.z);
	//				pField->vec3 = D3DXVECTOR3(pField->posM.x - 1000.0f, pField->posM.y + 210.0f, pField->posM.z);
	//				pField->vec2A = pField->vec2 - pField->vec3;
	//				pField->vec2B = pos - pField->vec3;

	//				//当たり判定
	//				if (((pField->vec2A.x * pField->vec2B.y) - (pField->vec2A.y * pField->vec2B.x)) < 0)
	//				{
	//					//当たり判定
	//					if (((pField->vecA.x * pField->vecB.y) - (pField->vecA.y * pField->vecB.x)) < 0)
	//					{
	//						//0以下になったら浮上
	//						pModel->pos.y = pModel->posOld.y + 0.8f;
	//						pCar->pos.y = pCar->posOld.y + DATE_CAR;
	//						//pCar->pos.y = pCar->posOld.y;
	//						//pos.y = posOld.y + 0.8f;
	//						break;
	//					}

	//					else
	//					{
	//						//0以上になったら下降
	//						pModel->pos.y = pModel->posOld.y - 0.8f;
	//						pCar->pos.y = pCar->posOld.y - DATE_CAR;
	//						//pos.y = posOld.y - 0.8f;
	//						break;
	//					}
	//				}
	//			}
	//		}

	//		//向きによって
	//		if (pField->nType == 3)
	//		{

	//			if (pField->posM.x - 400.0f <= pos.x && pField->posM.x >= pos.x &&
	//				pField->posM.z >= pos.z && pField->posM.z - 1000.0f <= pos.z)
	//			{
	//				//ベクトルの計算保存
	//				pField->vec = D3DXVECTOR3(pField->posM.x, pField->posM.y + 200.0f, pField->posM.z - 1000.0f);
	//				pField->vecA = pField->posM - pField->vec;
	//				pField->vecB = pos - pField->vec;

	//				////ベクトルの計算保存
	//				pField->vec2 = D3DXVECTOR3(pField->posM.x, pField->posM.y + 10.0f, pField->posM.z);
	//				pField->vec3 = D3DXVECTOR3(pField->posM.x, pField->posM.y + 210.0f, pField->posM.z - 1000.0f);
	//				pField->vec2A = pField->vec2 - pField->vec3;
	//				pField->vec2B = pos - pField->vec3;

	//				//当たり判定
	//				if (((pField->vec2A.z * pField->vec2B.y) - (pField->vec2A.y * pField->vec2B.z)) < 0)
	//				{
	//					//当たり判定
	//					if (((pField->vecA.z * pField->vecB.y) - (pField->vecA.y * pField->vecB.z)) < 0)
	//					{
	//						//0以下になったら浮上
	//						pModel->pos.y = pModel->posOld.y + 0.8f;
	//						pCar->pos.y = pCar->posOld.y + DATE_CAR;
	//						//pos.y = posOld.y + 0.8f;
	//						break;
	//					}

	//					else
	//					{
	//						//0以上になったら下降
	//						pModel->pos.y = pModel->posOld.y - 0.8f;
	//						pCar->pos.y = pCar->posOld.y - DATE_CAR;
	//						//pos.y = posOld.y - 0.8f;
	//						break;
	//					}
	//				}
	//			}
	//		}

	//		//向きによって
	//		if (pField->nType == 4)
	//		{

	//			if (pField->posM.x <= pos.x && pField->posM.x + 400.0f >= pos.x &&
	//				pField->posM.z + 1000.0f >= pos.z && pField->posM.z <= pos.z)
	//			{
	//				//ベクトルの計算保存
	//				pField->vec = D3DXVECTOR3(pField->posM.x, pField->posM.y + 200.0f, pField->posM.z + 1000.0f);
	//				pField->vecA = pField->vec - pField->posM;
	//				pField->vecB = pos - pField->posM;

	//				////ベクトルの計算保存
	//				pField->vec2 = D3DXVECTOR3(pField->posM.x, pField->posM.y + 10.0f, pField->posM.z);
	//				pField->vec3 = D3DXVECTOR3(pField->posM.x, pField->posM.y + 210.0f, pField->posM.z + 1000.0f);
	//				pField->vec2A = pField->vec3 - pField->vec2;
	//				pField->vec2B = pos - pField->vec2;

	//				//当たり判定
	//				if (((pField->vec2A.z * pField->vec2B.y) - (pField->vec2A.y * pField->vec2B.z)) < 0)
	//				{
	//					//当たり判定
	//					if (((pField->vecA.z * pField->vecB.y) - (pField->vecA.y * pField->vecB.z)) < 0)
	//					{
	//						//0以下になったら浮上
	//						pModel->pos.y = pModel->posOld.y + 0.8f;
	//						pCar->pos.y = pCar->posOld.y + DATE_CAR;
	//						//pos.y = posOld.y + 0.8f;
	//						break;
	//					}

	//					else
	//					{
	//						//0以上になったら下降
	//						pModel->pos.y = pModel->posOld.y - 0.8f;
	//						pCar->pos.y = pCar->posOld.y - DATE_CAR;
	//						//pos.y = posOld.y - 0.8f;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//// 頂点バッファをアンロックする
	//g_pVtxBuffMeshfield->Unlock();
}