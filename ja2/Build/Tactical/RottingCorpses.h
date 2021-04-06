#ifndef _ROTTING_CORPSES_H
#define _ROTTING_CORPSES_H

#include "TileEngine/TileAnimation.h"

#define NUM_CORPSE_SHADES 17

extern enum {
  NO_CORPSE,
  SMERC_JFK,
  SMERC_BCK,
  SMERC_FWD,
  SMERC_DHD,
  SMERC_PRN,
  SMERC_WTR,
  SMERC_FALL,
  SMERC_FALLF,

  MMERC_JFK,
  MMERC_BCK,
  MMERC_FWD,
  MMERC_DHD,
  MMERC_PRN,
  MMERC_WTR,
  MMERC_FALL,
  MMERC_FALLF,

  FMERC_JFK,
  FMERC_BCK,
  FMERC_FWD,
  FMERC_DHD,
  FMERC_PRN,
  FMERC_WTR,
  FMERC_FALL,
  FMERC_FALLF,

  // CIVS
  M_DEAD1,
  K_DEAD1,
  H_DEAD1,
  FT_DEAD1,
  S_DEAD1,
  W_DEAD1,
  C_DEAD1,
  M_DEAD2,
  K_DEAD2,
  H_DEAD2,

  FT_DEAD2,
  S_DEAD2,
  W_DEAD2,
  C_DEAD2,
  BLOODCAT_DEAD,
  COW_DEAD,
  ADULTMONSTER_DEAD,
  INFANTMONSTER_DEAD,
  LARVAEMONSTER_DEAD,
  ROTTING_STAGE2,

  TANK1_DEAD,
  TANK2_DEAD,

  HUMMER_DEAD,
  ICECREAM_DEAD,
  QUEEN_MONSTER_DEAD,
  ROBOT_DEAD,
  BURNT_DEAD,
  EXPLODE_DEAD,

  // JZ: 06.05.2015 Новая анимация танка, 2 катеров и турели
  // Танк 1
  TANK1NW_DEAD,
  TANK1NE_DEAD,
  TANK1SW_DEAD,
  TANK1SE_DEAD,

  // Танк 2
  TANK2NW_DEAD,
  TANK2NE_DEAD,
  TANK2SW_DEAD,
  TANK2SE_DEAD,

  // Танк 3
  TANK3NW_DEAD,
  TANK3NE_DEAD,
  TANK3SW_DEAD,
  TANK3SE_DEAD,

  // Танк 4
  TANK4NW_DEAD,
  TANK4NE_DEAD,
  TANK4SW_DEAD,
  TANK4SE_DEAD,

  // Танк 5 (с ДЗ)
  TANK5NW_DEAD,
  TANK5NE_DEAD,
  TANK5SW_DEAD,
  TANK5SE_DEAD,

  // Танк 6 (с ДЗ)
  TANK6NW_DEAD,
  TANK6NE_DEAD,
  TANK6SW_DEAD,
  TANK6SE_DEAD,

  // Танк 7 (с ДЗ)
  TANK7NW_DEAD,
  TANK7NE_DEAD,
  TANK7SW_DEAD,
  TANK7SE_DEAD,

  // Танк 8 (с ДЗ)
  TANK8NW_DEAD,
  TANK8NE_DEAD,
  TANK8SW_DEAD,
  TANK8SE_DEAD,

  // Катер
  BOATNW_DEAD,
  BOATNE_DEAD,
  BOATSW_DEAD,
  BOATSE_DEAD,

  // Большой катер
  BIGBOATNW_DEAD,
  BIGBOATNE_DEAD,
  BIGBOATSW_DEAD,
  BIGBOATSE_DEAD,

  // Турель
  TURRET_DEAD,

  //***24.02.2016*** сгоревшая техника
  // Танк 1
  TANK1NW_DEAD2,
  TANK1NE_DEAD2,
  TANK1SW_DEAD2,
  TANK1SE_DEAD2,

  // Танк 2
  TANK2NW_DEAD2,
  TANK2NE_DEAD2,
  TANK2SW_DEAD2,
  TANK2SE_DEAD2,

  // Танк 3
  TANK3NW_DEAD2,
  TANK3NE_DEAD2,
  TANK3SW_DEAD2,
  TANK3SE_DEAD2,

  // Танк 4
  TANK4NW_DEAD2,
  TANK4NE_DEAD2,
  TANK4SW_DEAD2,
  TANK4SE_DEAD2,

  // Танк 5 (с ДЗ)
  TANK5NW_DEAD2,
  TANK5NE_DEAD2,
  TANK5SW_DEAD2,
  TANK5SE_DEAD2,

  // Танк 6 (с ДЗ)
  TANK6NW_DEAD2,
  TANK6NE_DEAD2,
  TANK6SW_DEAD2,
  TANK6SE_DEAD2,

  // Танк 7 (с ДЗ)
  TANK7NW_DEAD2,
  TANK7NE_DEAD2,
  TANK7SW_DEAD2,
  TANK7SE_DEAD2,

  // Танк 8 (с ДЗ)
  TANK8NW_DEAD2,
  TANK8NE_DEAD2,
  TANK8SW_DEAD2,
  TANK8SE_DEAD2,

  // Катер
  BOATNW_DEAD2,
  BOATNE_DEAD2,
  BOATSW_DEAD2,
  BOATSE_DEAD2,

  // Большой катер
  BIGBOATNW_DEAD2,
  BIGBOATNE_DEAD2,
  BIGBOATSW_DEAD2,
  BIGBOATSE_DEAD2,

  // Турель
  TURRET_DEAD2,

  //***07.06.2016***
  APC1_1_DEAD,
  APC1_2_DEAD,
  APC1_3_DEAD,
  APC1_4_DEAD,

  APC2_1_DEAD,
  APC2_2_DEAD,
  APC2_3_DEAD,
  APC2_4_DEAD,

  APC1_1_DEAD2,
  APC1_2_DEAD2,
  APC1_3_DEAD2,
  APC1_4_DEAD2,

  APC2_1_DEAD2,
  APC2_2_DEAD2,
  APC2_3_DEAD2,
  APC2_4_DEAD2,

  NUM_CORPSES,

} RottingCorpseDefines;

#define ROTTING_CORPSE_FIND_SWEETSPOT_FROM_GRIDNO 0x01  // Find the closest spot to the given gridno
#define ROTTING_CORPSE_USE_NORTH_ENTRY_POINT 0x02  // Find the spot closest to the north entry grid
#define ROTTING_CORPSE_USE_SOUTH_ENTRY_POINT 0x04  // Find the spot closest to the south entry grid
#define ROTTING_CORPSE_USE_EAST_ENTRY_POINT 0x08   // Find the spot closest to the east entry grid
#define ROTTING_CORPSE_USE_WEST_ENTRY_POINT 0x10   // Find the spot closest to the west entry grid
#define ROTTING_CORPSE_USE_CAMMO_PALETTE 0x20      // We use cammo palette here....
#define ROTTING_CORPSE_VEHICLE 0x40                // Vehicle Corpse
#define ROTTING_CORPSE_STATIC 0x80                 //***26.02.2016*** неанимированный

typedef struct {
  UINT8 ubType;
  UINT8 ubBodyType;
  INT16 sGridNo;
  FLOAT dXPos;
  FLOAT dYPos;
  INT16 sHeightAdjustment;

  PaletteRepID HeadPal;  // Palette reps
  PaletteRepID PantsPal;
  PaletteRepID VestPal;
  PaletteRepID SkinPal;

  INT8 bDirection;
  UINT32 uiTimeOfDeath;

  UINT16 usFlags;

  INT8 bLevel;

  INT8 bVisible;
  INT8 bNumServicingCrows;
  UINT8 ubProfile;
  BOOLEAN fHeadTaken;
  UINT8 ubAIWarningValue;
  INT8 bTeam;  //***28.10.2013***
  UINT8 ubFiller[12 - 1];

} ROTTING_CORPSE_DEFINITION;

typedef struct {
  ROTTING_CORPSE_DEFINITION def;
  BOOLEAN fActivated;

  ANITILE *pAniTile;

  SGPPaletteEntry *p8BPPPalette;
  UINT16 *p16BPPPalette;
  UINT16 *pShades[NUM_CORPSE_SHADES];
  INT16 sGraphicNum;
  INT32 iCachedTileID;
  FLOAT dXPos;
  FLOAT dYPos;

  BOOLEAN fAttractCrowsOnlyWhenOnScreen;
  INT32 iID;

} ROTTING_CORPSE;

INT32 AddRottingCorpse(ROTTING_CORPSE_DEFINITION *pCorpseDef);

void RemoveCorpse(INT32 iCorpseID);
void RemoveCorpses();

BOOLEAN TurnSoldierIntoCorpse(SOLDIERCLASS *pSoldier, BOOLEAN fRemoveMerc, BOOLEAN fCheckForLOS);

INT16 FindNearestRottingCorpse(SOLDIERCLASS *pSoldier);

void AllMercsOnTeamLookForCorpse(ROTTING_CORPSE *pCorpse, INT8 bTeam);
void MercLooksForCorpses(SOLDIERCLASS *pSoldier);
void RebuildAllCorpseShadeTables();

UINT16 CreateCorpsePaletteTables(ROTTING_CORPSE *pCorpse);

INT16 FindNearestAvailableGridNoForCorpse(ROTTING_CORPSE_DEFINITION *pCorpseDef, INT8 ubRadius);

void HandleRottingCorpses();
void AddCrowToCorpse(ROTTING_CORPSE *pCorpse);

void VaporizeCorpse(INT16 sGridNo, UINT16 usStructureID);
void CorpseHit(INT16 sGridNo, UINT16 usStructureID);

void HandleCrowLeave(SOLDIERCLASS *pSoldier);

void HandleCrowFlyAway(SOLDIERCLASS *pSoldier);

#define MAX_ROTTING_CORPSES 100

extern ROTTING_CORPSE gRottingCorpse[MAX_ROTTING_CORPSES];
extern INT32 giNumRottingCorpse;
extern UINT8 gb4DirectionsFrom8[8];

ROTTING_CORPSE *GetCorpseAtGridNo(INT16 sGridNo, INT8 bLevel);
BOOLEAN IsValidDecapitationCorpse(ROTTING_CORPSE *pCorpse);
void DecapitateCorpse(SOLDIERCLASS *pSoldier, INT16 sGridNo, INT8 bLevel);

void GetBloodFromCorpse(SOLDIERCLASS *pSoldier);

UINT16 GetCorpseStructIndex(ROTTING_CORPSE_DEFINITION *pCorpseDef, BOOLEAN fForImage);

void LookForAndMayCommentOnSeeingCorpse(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubLevel);

INT16 GetGridNoOfCorpseGivenProfileID(UINT8 ubProfileID);

void DecayRottingCorpseAIWarnings(void);
UINT8 GetNearestRottingCorpseAIWarning(INT16 sGridNo, INT8 bTeam);

#endif
