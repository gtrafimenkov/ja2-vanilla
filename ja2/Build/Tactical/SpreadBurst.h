#ifndef _SPREAD_BURST_H
#define _SPREAD_BURST_H

#include "Tactical/SoldierControl.h"

void ResetBurstLocations();
void AccumulateBurstLocation(INT16 sGridNo);
void PickBurstLocations(SOLDIERCLASS *pSoldier);
void AIPickBurstLocations(SOLDIERCLASS *pSoldier, INT8 bTargets, SOLDIERCLASS *pTargets[5]);

void RenderAccumulatedBurstLocations();

#endif
