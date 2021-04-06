#ifndef __HANDLEUIPLAN_H
#define __HANDLEUIPLAN_H

#include "SGP/Types.h"
#include "Tactical/SoldierControl.h"

#define UIPLAN_ACTION_MOVETO 1
#define UIPLAN_ACTION_FIRE 2

BOOLEAN BeginUIPlan(SOLDIERTYPE *pSoldier);
BOOLEAN AddUIPlan(UINT16 sGridNo, UINT8 ubPlanID);
void EndUIPlan();
BOOLEAN InUIPlanMode();

#endif