//==============================================================================
//
// ランキング画面の描画〔ranking.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _RANKING_H_
#define _RANKING_H_
#include "main.h"

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitRanking(void);		// ランキング画面の初期化処理
void UninitRanking(void);		// ランキング画面の終了処理
void UpdateRanking(void);		// ランキング画面の更新処理
void DrawRanking(void);			// ランキング画面の描画処理
void SetRanking(int nScore);	// スコアの並び替え
void LoadData(void);			// データの読み込み
void SaveData(void);			// データの保存
void SetInitRanking(void);		// 初期化におけるポリゴン配置

#endif
