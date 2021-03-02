//-----------------------------------------------------------------
//
// プレイヤーの処理 (model.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _CAR_H_
#define _CAR_H_

#include "main.h"


typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotmove;		//向き
	D3DXVECTOR3 rotDest;		//向き
	D3DXVECTOR3 oldrotDest;		//向き
	D3DXVECTOR3 MinCar;
	D3DXVECTOR3 MaxCar;
	float nSpeed;
}CAR;

//===============
//プロトタイプ宣言
//===============
HRESULT InitCar(void);
void UninitCar(void);
void UpdateCar(void);
void DrawCar(void);
CAR *GetCar(void);
float GetCarSpeed(void);
#endif

