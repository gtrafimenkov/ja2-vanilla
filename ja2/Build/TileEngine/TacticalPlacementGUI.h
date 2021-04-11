#ifndef __TACTICAL_PLACEMENT_GUI_H
#define __TACTICAL_PLACEMENT_GUI_H

#include "Tactical/SoldierControl.h"
#include "SGP/MouseSystem.h"

void InitTacticalPlacementGUI();
void KillTacticalPlacementGUI();
void TacticalPlacementHandle();
void RenderTacticalPlacementGUI();

void HandleTacticalPlacementClicksInOverheadMap(MOUSE_REGION *reg, INT32 reason);

extern BOOLEAN gfTacticalPlacementGUIActive;
extern BOOLEAN gfEnterTacticalPlacementGUI;

extern SOLDIERCLASS *gpTacticalPlacementSelectedSoldier;
extern SOLDIERCLASS *gpTacticalPlacementHilightedSoldier;

// Saved value.  Contains the last choice for future battles.
extern UINT8 gubDefaultButton;

#endif
