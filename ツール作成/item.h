//==============================================================================
//
// 3Dモデルの描画〔item.h〕
// AUTHOR : TAKAHASHI SHOTA
//
//==============================================================================
#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"
#include "game.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ITEM		(16)						// 当たり判定の面の数
#define COLLISION_PARTS	(4 * MAX_ITEM)			// 当たり判定の面の数
//#define COLLISION_PARTS	(80)					// 当たり判定の面の数


//==============================================================================
// モデルの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 vtxMinItem, vtxMaxItem;				// モデルの各座標の最大値・最小値
	LPD3DXBUFFER g_pBuffMatItem = NULL;				// マテリアル(材質情報)へのポインタ
	D3DXVECTOR3 aPos[COLLISION_PARTS];					// 当たり判定頂点情報
	D3DXVECTOR3 vec;
	D3DXVECTOR3 aVec[COLLISION_PARTS];					// 当たり判定ベクトル
	D3DXVECTOR3 vtx;
	DWORD sizeFVF;										// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;										// 頂点バッファへのポインタ
	D3DXVECTOR3 posVec[2];
	D3DXVECTOR3 vecA;
	D3DXVECTOR3 vecB;
	float a;
	bool bUse;
	int nType;
	int nIdx;
	int nNumVtx;										// 頂点数
} ITEM;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitItem(void);	// 3Dオブジェクトの初期化処理
void UninitItem(void);	// 3Dオブジェクトの終了処理
void UpdateItem(void);	// 3Dオブジェクトの更新処理
void DrawItem(void);		// 3Dオブジェクトの描画処理

bool CollisionVecItem(
	D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,
	float fWidthMax,
	float fWidthMin,
	float fDepthMax,
	float fDepthMin,
	float fHeightMax,
	float fheightMin);

void SetItem(D3DXVECTOR3 Pos, D3DXVECTOR3 rot, int nType);
int GetitemNumber(void);
int GetitemDate(void);
int GetitemType(void);

#endif