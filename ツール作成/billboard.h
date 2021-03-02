#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//---------------------------------------------------------------------------- -
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_BILLBOARD (2)				//ビルボードの上限
#define BILLBOARD_SIZE (10)			//ビルボードのサイズ

//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	D3DXVECTOR3 move;						//移動量
	D3DXMATRIX mtxWorld;					//ワールドマトリックス
	bool bUse;
}BILLBOARD;

//===============
//プロトタイプ宣言
//===============
HRESULT InitBillboard(void);//初期化処理
void UninitBillboard(void);//終了処理
void UpdateBillboard(void);//更新処理
void DrawBillboard(void);//描画処理
void SetBillboard(D3DXVECTOR3 pos);
void SetVertexBillboard(int nIdx);
#endif
