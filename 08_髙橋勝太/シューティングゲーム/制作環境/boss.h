//-----------------------------------------------------------------
//
// ボスの処理 (boss.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _BOSS_H_
#define _BOSS_H_

#define MAX_BOSS (256)//敵の最大数
#define MAX_TYPE_BOSS (3)//敵の種類数
#define MAX_BOSS_SIZE (150)

#include "main.h"
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "Explosion.h"
#include "sound.h"
//
//敵の状態
//
typedef enum
{
	BOSSSTATE_NOMAL = 0,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_MAX
}BOSSSTATE;

//
//敵の構造体
//
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	int nType;//種類
	bool bUse;//使用しているかどうか
	int g_nCounterAni;
	int g_nPatternAni;
	int g_nData;
	int g_nP;
	int g_nseetAni;
	BOSSSTATE state;//状態
	int nCounterState;//状態カウンター
	int nLife;//体力
}BOSS;

//===============
//プロトタイプ宣言
//===============
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 Pos, int nType);
void SetBoss2(D3DXVECTOR3 Pos, int nType);
BOSS *GetBoss(void);
bool HitBoss(int nIdx, int nDamage);
int GetBossNomber(void);
#endif
