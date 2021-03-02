#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//---------------------------------------------------------------------------- -
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_EFFECT (3056)					//ビルボードの上限
#define EFFECT_SIZE (5)						//ビルボードのサイズ

//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	D3DXVECTOR3 move;						//移動量
	D3DXMATRIX mtxWorld;					//ワールドマトリックス
	int nLife;
	int nType;
	float fWidth;
	float fHeigth;
	int nLifeBullet;
	bool bUse;
}EFFECT;

//===============
//プロトタイプ宣言
//===============
HRESULT InitEffect(void);//初期化処理
void UninitEffect(void);//終了処理
void UpdateEffect(void);//更新処理
void DrawEffect(void);//描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nType, int nLifeBullet);
void SetFire(D3DXVECTOR3 pos);
void SetWater(D3DXVECTOR3 pos);
void SetVertexEffect(int nIdx);
EFFECT * GetEffect(void);
#endif
