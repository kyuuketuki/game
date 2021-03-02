//==============================================================================
//
// カメラの設定〔camera.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

//==============================================================================
// カメラの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 posV;			// 現在の視点
	D3DXVECTOR3 posVDest;		// 目的の視点
	D3DXVECTOR3 posR;			// 現在の注視点
	D3DXVECTOR3 posRDest;		// 目的の注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// カメラの角度
	float fLength;				// 視点・注視点間の距離
	float fHeightV;				// 視点の高さ
	float fHeightR;				// 注視点の高さ
	float fFront;				// モデルの前方の距離
	bool bAutoTurn;				// カメラの回り込みON・OFF
	D3DVIEWPORT9 viewport;		//ビューポート
}CAMERA;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
void InitCamera(void);		// カメラの初期化処理
void UninitCamera(void);	// カメラの終了処理
void UpdateCamera(void);	// カメラの更新処理
void SetCamera(void);		// カメラの設定
CAMERA *GetCamera(void);

#endif
