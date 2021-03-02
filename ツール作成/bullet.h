#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//---------------------------------------------------------------------------- -
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_BULLET (256)				//ビルボードの上限
#define BULLET_SIZE (2)			//ビルボードのサイズ

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
	bool bUse;
}BULLET;

//===============
//プロトタイプ宣言
//===============
HRESULT InitBullet(void);//初期化処理
void UninitBullet(void);//終了処理
void UpdateBullet(void);//更新処理
void DrawBullet(void);//描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nSpeed, int nLife);
void SetVertexBullet(int nIdx);
BULLET * GetBullet(void);
#endif
