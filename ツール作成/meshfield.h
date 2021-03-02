//==============================================================================
//
// メッシュフィールドの描画〔meshfield.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define FIELD_MAX	(1)			// フィールドの最大数
#define COLLISION_PARTS_MESH	(4 * FIELD_MAX)			// 当たり判定の面の数

// メッシュフィールドの構造体
typedef struct
{
	D3DXVECTOR3 pos;									// 位置
	D3DXVECTOR3 rot;									// 角度
	D3DXMATRIX mtxWorld;								// ワールドマトリックス
	D3DXVECTOR3 g_posMeshfield;							// 位置
	D3DXVECTOR3 g_rotMeshfield;							// 向き
	D3DXVECTOR3 aPos[COLLISION_PARTS_MESH];				// 当たり判定頂点情報
	D3DXVECTOR3 aVec[COLLISION_PARTS_MESH];				// 当たり判定ベクトル
	D3DXVECTOR3 vec;
	D3DXVECTOR3 vecA;
	D3DXVECTOR3 vecB;
	D3DXVECTOR3 vec2;
	D3DXVECTOR3 vec2A;
	D3DXVECTOR3 vec2B;
	D3DXVECTOR3 vec3;
	D3DXVECTOR3 vtx;
	D3DXVECTOR3 posM;
	bool bUse;											// 使用しているか
	int nAllPoint;										// 総頂点数
	int nPolygon;										// ポリゴン数
	int nIdxPoint;										// インデックスバッファの必要な確保数
	float fWidth;										// ポリゴン一つあたりの幅
	float fDepth;										// ポリゴン一つあたりの奥行
	int nWidth;											// 横幅の頂点数
	int nDepth;											// 奥行の頂点数
	float fWidthMax;									// 横幅の最大
	float fDepthMax;									// 奥行の最大
	float X;
	float Z;
	float nDate;
	float nDate2;
	int nType;
}MeshField;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitMeshfield(void);					// 3Dポリゴンの初期化処理
void UninitMeshfield(void);						// 3Dポリゴンの終了処理
void UpdateMeshfield(void);						// 3Dポリゴンの更新処理
void DrawMeshfield(void);						// 3Dポリゴンの描画処理
MeshField *GetMesh(void);						// プレイヤーの取得
void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float X, float Z);
void CollisionVecMesh(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

#endif
