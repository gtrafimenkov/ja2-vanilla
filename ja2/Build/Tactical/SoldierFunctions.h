#ifndef _SOLDIER_FUNCTIONS_H
#define _SOLDIER_FUNCTIONS_H

#include "Tactical/SoldierControl.h"

void ContinueMercMovement(SOLDIERCLASS *pSoldier);

BOOLEAN IsValidStance(SOLDIERCLASS *pSoldier, INT8 bNewStance);
void SelectMoveAnimationFromStance(SOLDIERCLASS *pSoldier);
BOOLEAN IsValidMovementMode(SOLDIERCLASS *pSoldier, INT16 usMovementMode);
FLOAT CalcSoldierNextBleed(SOLDIERCLASS *pSoldier);
FLOAT CalcSoldierNextUnmovingBleed(SOLDIERCLASS *pSoldier);
void SoldierCollapse(SOLDIERCLASS *pSoldier);

BOOLEAN ReevaluateEnemyStance(SOLDIERCLASS *pSoldier, UINT16 usAnimState);

void HandlePlacingRoofMarker(SOLDIERCLASS *pSoldier, INT16 sGridNo, BOOLEAN fSet, BOOLEAN fForce);

void PickPickupAnimation(SOLDIERCLASS *pSoldier, INT32 iItemIndex, INT16 sGridNo, INT8 bZLevel);

void MercStealFromMerc(SOLDIERCLASS *pSoldier, SOLDIERCLASS *pTarget);

void HandleCrowShadowVisibility(SOLDIERCLASS *pSoldier);

#endif
