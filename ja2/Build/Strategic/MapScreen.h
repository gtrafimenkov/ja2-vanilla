#ifndef __MAPSCREEN_H
#define __MAPSCREEN_H

#include "SGP/Types.h"
#include "Tactical/SoldierControl.h"

#define MAP_WORLD_X 18
#define MAP_WORLD_Y 18

// Sector name identifiers
typedef enum {
  BLANK_SECTOR = 0,
  OMERTA,
  DRASSEN,
  ALMA,
  GRUMM,
  TIXA,
  CAMBRIA,
  SAN_MONA,
  ESTONI,
  ORTA,
  BALIME,
  MEDUNA,
  CHITZENA,
  NUM_TOWNS
} Towns;

#define FIRST_TOWN OMERTA
//#define PALACE			NUM_TOWNS

extern BOOLEAN fCharacterInfoPanelDirty;
extern BOOLEAN fTeamPanelDirty;
extern BOOLEAN fMapPanelDirty;

extern BOOLEAN fMapInventoryItem;
extern BOOLEAN gfInConfirmMapMoveMode;
extern BOOLEAN gfInChangeArrivalSectorMode;

extern BOOLEAN gfSkyriderEmptyHelpGiven;

BOOLEAN SetInfoChar(UINT8 ubSolId);
void EndMapScreen(BOOLEAN fDuringFade);
void ReBuildCharactersList(void);

BOOLEAN HandlePreloadOfMapGraphics(void);
void HandleRemovalOfPreLoadedMapGraphics(void);

void ChangeSelectedMapSector(INT16 sMapX, INT16 sMapY, INT8 bMapZ);

BOOLEAN CanToggleSelectedCharInventory(void);

BOOLEAN CanExtendContractForCharSlot(INT8 bCharNumber);

void TellPlayerWhyHeCantCompressTime(void);

void ChangeSelectedInfoChar(INT8 bCharNumber, BOOLEAN fResetSelectedList);

void MAPEndItemPointer();

void CopyPathToAllSelectedCharacters(PathStPtr pPath);
void CancelPathsOfAllSelectedCharacters();

INT32 GetPathTravelTimeDuringPlotting(PathStPtr pPath);

void AbortMovementPlottingMode(void);

void ExplainWhySkyriderCantFly(void);

BOOLEAN CanChangeSleepStatusForCharSlot(INT8 bCharNumber);
BOOLEAN CanChangeSleepStatusForSoldier(SOLDIERTYPE *pSoldier);

BOOLEAN MapCharacterHasAccessibleInventory(INT8 bCharNumber);

#endif
