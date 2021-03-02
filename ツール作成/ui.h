#ifndef _UI_H_
#define _UI_H_

#include "main.h"

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
}UI;

//===============
//プロトタイプ宣言
//===============
HRESULT InitUi(void);//初期化処理
void UninitUi(void);//終了処理
void UpdateUi(void);//更新処理
void DrawUi(void);//描画処理
//void SetUi(D3DXVECTOR3 pos, int nType);
void SetFire(D3DXVECTOR3 pos);
void SetWater(D3DXVECTOR3 pos);
void SetVertexUi(int nIdx);
#endif
