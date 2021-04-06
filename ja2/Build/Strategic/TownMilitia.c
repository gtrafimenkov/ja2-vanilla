#include "Strategic/StrategicAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "Strategic/TownMilitia.h"
#include "Tactical/MilitiaControl.h"
#include "Strategic/CampaignTypes.h"
#include "Strategic/Strategic.h"
#include "Strategic/StrategicMap.h"
#include "Tactical/Overhead.h"
#include "Strategic/StrategicTownLoyalty.h"
#include "Utils/Utilities.h"
#include "SGP/Random.h"
#include "Utils/Text.h"
#include "Strategic/MapScreenInterface.h"
#include "Tactical/Interface.h"
#include "Laptop/LaptopSave.h"
#include "Laptop/Finances.h"
#include "Strategic/GameClock.h"
#include "Strategic/Assignments.h"
#include "Tactical/Squads.h"
#include "Tactical/SoldierCreate.h"
#include "Tactical/DialogueControl.h"
#endif

#define SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST 50

// temporary local global variables
UINT8 gubTownSectorServerTownId = BLANK_SECTOR;
INT16 gsTownSectorServerSkipX = -1;
INT16 gsTownSectorServerSkipY = -1;
UINT8 gubTownSectorServerIndex = 0;
BOOLEAN gfYesNoPromptIsForContinue =
    FALSE;  // this flag remembers whether we're starting new training, or continuing
INT32 giTotalCostOfTraining = 0;

// the completed list of sector soldiers for training militia
INT32 giListOfMercsInSectorsCompletedMilitiaTraining[SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST];
SOLDIERCLASS *pMilitiaTrainerSoldier = NULL;

// note that these sector values are STRATEGIC INDEXES, not 0-255!
INT16 gsUnpaidStrategicSector[MAX_CHARACTER_COUNT];

// the selected list of mercs
extern BOOLEAN fSelectedListOfMercsForMapScreen[MAX_CHARACTER_COUNT];

// private prototypes
void PayMilitiaTrainingYesNoBoxCallback(UINT8 bExitValue);
void CantTrainMilitiaOkBoxCallback(UINT8 bExitValue);
void MilitiaTrainingRejected(void);

void InitFriendlyTownSectorServer(UINT8 ubTownId, INT16 sSkipSectorX, INT16 sSkipSectorY);
BOOLEAN ServeNextFriendlySectorInTown(INT16 *sNeighbourX, INT16 *sNeighbourY);

void BuildListOfUnpaidTrainableSectors(void);
INT32 GetNumberOfUnpaidTrainableSectors(void);
void ContinueTrainingInThisSector(void);
void StartTrainingInAllUnpaidTrainableSectors();
void PayForTrainingInSector(UINT8 ubSector);
void ResetDoneFlagForAllMilitiaTrainersInSector(UINT8 ubSector);

#ifdef JA2BETAVERSION
void VerifyTownTrainingIsPaidFor(void);
#endif

//***15.11.2014***
UINT8 MAX_ALLOWABLE_MILITIA_PER_SECTOR = 20;
UINT8 MILITIA_TRAINING_SQUAD_SIZE = 10;
UINT16 MILITIA_TRAINING_COST = 750;
UINT8 MIN_RATING_TO_TRAIN_TOWN = 20;

//***6.11.2007*** сектора блокпостов (x, y)
UINT16 usRoadblockSectors[] = {
    /*	CALCULATE_STRATEGIC_INDEX(15, 12),	// L15 стройка ПВО
            CALCULATE_STRATEGIC_INDEX(9, 3),	// C9 блокпост
            CALCULATE_STRATEGIC_INDEX(3, 7),	// G3 блокпост
            CALCULATE_STRATEGIC_INDEX(6, 7),	// G6 блокпост
            CALCULATE_STRATEGIC_INDEX(12, 7),	// G12 блокпост
            CALCULATE_STRATEGIC_INDEX(3, 10),	// J3 блокпост
            CALCULATE_STRATEGIC_INDEX(6, 11),	// K6 блокпост
            CALCULATE_STRATEGIC_INDEX(10, 11),	// K10 блокпост
            CALCULATE_STRATEGIC_INDEX(14, 11),	// K14 блокпост
            CALCULATE_STRATEGIC_INDEX(9, 14),	// N9 блокпост
    */
    CALCULATE_STRATEGIC_INDEX(3, 6),  // F3 ГЭС
    0};

//***15.11.2014***
void InitMilitiaStartParams() {
  if (gGameOptions.fLimitedMilitia) {
    MAX_ALLOWABLE_MILITIA_PER_SECTOR = 20;
    MILITIA_TRAINING_SQUAD_SIZE = 20;
    MILITIA_TRAINING_COST = 75;
    MIN_RATING_TO_TRAIN_TOWN = 1;
  } else {
    MAX_ALLOWABLE_MILITIA_PER_SECTOR = 20;
    MILITIA_TRAINING_SQUAD_SIZE = 10;
    MILITIA_TRAINING_COST = 750;
    MIN_RATING_TO_TRAIN_TOWN = 20;
  }
}

//***15.11.2014*** оплата труда охраны и рабочих
void PayMilitia(void) {
  INT32 iSalary = 0;
  UINT8 ubSectorID, ubRank;
  INT16 sNumberOfMilitia;

  for (ubSectorID = 0; ubSectorID < 255; ubSectorID++) {
    iSalary += SectorInfo[ubSectorID].ubNumberOfCivsAtLevel[GREEN_MILITIA] * 10 +
               SectorInfo[ubSectorID].ubNumberOfCivsAtLevel[REGULAR_MILITIA] * 20 +
               SectorInfo[ubSectorID].ubNumberOfCivsAtLevel[ELITE_MILITIA] * 30;
  }

  if (LaptopSaveInfo.iCurrentBalance >= iSalary) {
    AddTransactionToPlayersBook(PAY_MILITIA, 0, GetWorldTotalMin(), -iSalary);
  } else {
    CHAR16 zMoney[128];

    swprintf(zMoney, L"%d", iSalary);
    InsertCommasForDollarFigure(zMoney);
    InsertDollarSignInToString(zMoney);

    MapScreenMessage(FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE,
                     pMessageStrings[MSG_CANT_AFFORD_TO_PAY_NPC_DAILY_SALARY_MSG],
                     pTransactionText[17], zMoney);

    for (ubSectorID = 0; ubSectorID < 255; ubSectorID++) {
      for (ubRank = GREEN_MILITIA; ubRank < MAX_MILITIA_LEVELS; ubRank++) {
        sNumberOfMilitia = SectorInfo[ubSectorID].ubNumberOfCivsAtLevel[ubRank];
        if (sNumberOfMilitia > 0) {
          sNumberOfMilitia -= 3 + Random(5);
          if (sNumberOfMilitia < 0) sNumberOfMilitia = 0;
          SectorInfo[ubSectorID].ubNumberOfCivsAtLevel[ubRank] = (UINT8)sNumberOfMilitia;
        }
      }
    }

    StopTimeCompression();
  }
}

//***6.11.2007***
BOOLEAN IsThisSectorARoadblock(INT16 sSectorX, INT16 sSectorY) {
  UINT16 usSector, i = 0;

  usSector = (UINT16)CALCULATE_STRATEGIC_INDEX(sSectorX, sSectorY);

  while (usRoadblockSectors[i] != 0) {
    if (usRoadblockSectors[i] == usSector) return (TRUE);
    i++;
  }

  return (FALSE);
}

//***21.07.2013***
BOOLEAN MilitiaRecruitingAllowedInSector(INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ) {
  INT8 bTownId;
  BOOLEAN fSamSitePresent = FALSE;

  if (bSectorZ != 0) {
    return (FALSE);
  }

  fSamSitePresent = IsThisSectorASAMSector(sSectorX, sSectorY, bSectorZ);

  if (fSamSitePresent) {
    return (TRUE);
  }

  if (IsThisSectorARoadblock(sSectorX, sSectorY)) {
    return (TRUE);
  }

  bTownId = GetTownIdForSector(sSectorX, sSectorY);

  return (MilitiaTrainingAllowedInTown(bTownId));
}

//***24.04.2010*** стоимость ополчения, зависящая от его уже наличиствующего числа, сложности и
//прогресса игры
INT32 GetMilitiaTrainingCost(UINT8 ubSector) {
  INT32 iTrainingCost;

  UINT8 ubMilTrain, ubMilNotTrain, ubTrainCount, ubTownSectors;
  INT8 bTownId;

  if (!gGameOptions.fLimitedMilitia)  //***15.11.2014***
  {
    iTrainingCost = 1000 + 200 /* gGameOptions.ubDifficultyLevel*/ *
                               (INT32)(HighestPlayerProgressPercentage() / 10);

    bTownId = StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX(ubSector)].bNameId;
    if (bTownId != BLANK_SECTOR && (ubTownSectors = GetTownSectorsUnderControl(bTownId)) > 1) {
      if (!gExtGameOptions.fBlueMilitia) {
        ubMilTrain = GetMilitiaCountAtLevelAnywhereInTown(bTownId, GREEN_MILITIA);
        ubMilNotTrain = GetMilitiaCountAtLevelAnywhereInTown(bTownId, REGULAR_MILITIA) +
                        GetMilitiaCountAtLevelAnywhereInTown(bTownId, ELITE_MILITIA);
      } else {
        ubMilTrain = GetMilitiaCountAtLevelAnywhereInTown(bTownId, REGULAR_MILITIA);
        ubMilNotTrain = GetMilitiaCountAtLevelAnywhereInTown(bTownId, ELITE_MILITIA);
      }

      if (ubMilNotTrain >
          MILITIA_TRAINING_SQUAD_SIZE + MAX_ALLOWABLE_MILITIA_PER_SECTOR * (ubTownSectors - 1))
        ubTrainCount = MAX_ALLOWABLE_MILITIA_PER_SECTOR * ubTownSectors - ubMilNotTrain;
      else
        ubTrainCount = MILITIA_TRAINING_SQUAD_SIZE;
    } else {
      if (!gExtGameOptions.fBlueMilitia) {
        ubMilTrain = SectorInfo[ubSector].ubNumberOfCivsAtLevel[GREEN_MILITIA];
        ubMilNotTrain = SectorInfo[ubSector].ubNumberOfCivsAtLevel[REGULAR_MILITIA] +
                        SectorInfo[ubSector].ubNumberOfCivsAtLevel[ELITE_MILITIA];
      } else {
        ubMilTrain = SectorInfo[ubSector].ubNumberOfCivsAtLevel[REGULAR_MILITIA];
        ubMilNotTrain = SectorInfo[ubSector].ubNumberOfCivsAtLevel[ELITE_MILITIA];
      }

      if (ubMilNotTrain > MILITIA_TRAINING_SQUAD_SIZE)
        ubTrainCount = MAX_ALLOWABLE_MILITIA_PER_SECTOR - ubMilNotTrain;
      else
        ubTrainCount = MILITIA_TRAINING_SQUAD_SIZE;
    }

    iTrainingCost = iTrainingCost * ubTrainCount / MILITIA_TRAINING_SQUAD_SIZE;
  } else {
    iTrainingCost = MILITIA_TRAINING_COST *
                    (INT32)(HighestPlayerProgressPercentage() / 10 +
                            1);  //***10.10.2013*** стоимость амуниции для одного ополчена
    //***28.07.2013***
    iTrainingCost =
        iTrainingCost *
        SectorInfo[ubSector]
            .ubNumberOfCivsAtLevel[(gExtGameOptions.fBlueMilitia == FALSE
                                        ? GREEN_MILITIA
                                        : REGULAR_MILITIA)];  // /MILITIA_TRAINING_SQUAD_SIZE;
  }
  if (iTrainingCost == 0) iTrainingCost = 1;  // на всякий случай

  return (iTrainingCost);
}

void TownMilitiaTrainingCompleted(SOLDIERCLASS *pTrainer, INT16 sMapX, INT16 sMapY) {
  SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR(sMapX, sMapY)]);
  UINT8 ubMilitiaTrained = 0;
  BOOLEAN fFoundOne;
  INT16 sNeighbourX, sNeighbourY;
  UINT8 ubTownId;

  // get town index
  ubTownId = StrategicMap[sMapX + sMapY * MAP_WORLD_X].bNameId;

  if (ubTownId == BLANK_SECTOR) {
    //***6.11.2007*** добавлено условие про блокпост
    if (IsThisSectorARoadblock(sMapX, sMapY) == FALSE)
      Assert(IsThisSectorASAMSector(sMapX, sMapY, 0));
  }

  // force tactical to update militia status
  gfStrategicMilitiaChangesMade = TRUE;

  // ok, so what do we do with all this training?  Well, in order of decreasing priority:
  // 1) If there's room in training sector, create new GREEN militia guys there
  // 2) If not enough room there, create new GREEN militia guys in friendly sectors of the same town
  // 3) If not enough room anywhere in town, promote a number of GREENs in this sector into regulars
  // 4) If not enough GREENS there to promote, promote GREENs in other sectors.
  // 5) If all friendly sectors of this town are completely filled with REGULAR militia, then
  // training effect is wasted
  if (!gGameOptions.fLimitedMilitia)  //***15.11.2014***
  {
    while (ubMilitiaTrained < MILITIA_TRAINING_SQUAD_SIZE) {
      // is there room for another militia in the training sector itself?
      if (CountAllMilitiaInSector(sMapX, sMapY) < MAX_ALLOWABLE_MILITIA_PER_SECTOR) {
        // great! Create a new GREEN militia guy in the training sector
        StrategicAddMilitiaToSector(
            sMapX, sMapY,
            (gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA
                                                   : REGULAR_MILITIA) /*GREEN_MILITIA*/,
            1);
      } else {
        fFoundOne = FALSE;

        if (ubTownId != BLANK_SECTOR) {
          InitFriendlyTownSectorServer(ubTownId, sMapX, sMapY);

          // check other eligible sectors in this town for room for another militia
          while (ServeNextFriendlySectorInTown(&sNeighbourX, &sNeighbourY)) {
            // is there room for another militia in this neighbouring sector ?
            if (CountAllMilitiaInSector(sNeighbourX, sNeighbourY) <
                MAX_ALLOWABLE_MILITIA_PER_SECTOR) {
              // great! Create a new GREEN militia guy in the neighbouring sector
              StrategicAddMilitiaToSector(
                  sNeighbourX, sNeighbourY,
                  (gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA
                                                         : REGULAR_MILITIA) /*GREEN_MILITIA*/,
                  1);

              fFoundOne = TRUE;
              break;
            }
          }
        }

        // if we still haven't been able to train anyone
        if (!fFoundOne) {
          // alrighty, then.  We'll have to *promote* guys instead.

          // are there any GREEN militia men in the training sector itself?
          if (MilitiaInSectorOfRank(sMapX, sMapY,
                                    (gExtGameOptions.fBlueMilitia == FALSE
                                         ? GREEN_MILITIA
                                         : REGULAR_MILITIA) /*GREEN_MILITIA*/) > 0) {
            // great! Promote a GREEN militia guy in the training sector to a REGULAR
            StrategicPromoteMilitiaInSector(
                sMapX, sMapY,
                (gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA
                                                       : REGULAR_MILITIA) /*GREEN_MILITIA*/,
                1);
          } else {
            if (ubTownId != BLANK_SECTOR) {
              // dammit! Last chance - try to find other eligible sectors in the same town with a
              // Green guy to be promoted
              InitFriendlyTownSectorServer(ubTownId, sMapX, sMapY);

              // check other eligible sectors in this town for room for another militia
              while (ServeNextFriendlySectorInTown(&sNeighbourX, &sNeighbourY)) {
                // are there any GREEN militia men in the neighbouring sector ?
                if (MilitiaInSectorOfRank(sNeighbourX, sNeighbourY,
                                          (gExtGameOptions.fBlueMilitia == FALSE
                                               ? GREEN_MILITIA
                                               : REGULAR_MILITIA) /*GREEN_MILITIA*/) > 0) {
                  // great! Promote a GREEN militia guy in the neighbouring sector to a REGULAR
                  StrategicPromoteMilitiaInSector(
                      sNeighbourX, sNeighbourY,
                      (gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA
                                                             : REGULAR_MILITIA) /*GREEN_MILITIA*/,
                      1);

                  fFoundOne = TRUE;
                  break;
                }
              }
            }

            // if we still haven't been able to train anyone
            if (!fFoundOne) {
              // Well, that's it.  All eligible sectors of this town are full of REGULARs or ELITEs.
              // The training goes to waste in this situation.
              break;  // the main while loop
            }
          }
        }
      }

      // next, please!
      ubMilitiaTrained++;
    }
  } else {
    //***28.07.2013***
    ubMilitiaTrained = MilitiaInSectorOfRank(
        sMapX, sMapY, (gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA : REGULAR_MILITIA));
    ubMilitiaTrained = min(pSectorInfo->fMilitiaTrainingPaid, ubMilitiaTrained);  //***10.09.2013***
    if (ubMilitiaTrained > 0) {
      StrategicPromoteMilitiaInSector(
          sMapX, sMapY, (gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA : REGULAR_MILITIA),
          ubMilitiaTrained);
    }
  }

  // if anyone actually got trained
  if (ubMilitiaTrained > 0) {
    // update the screen display
    fMapPanelDirty = TRUE;

    if (ubTownId != BLANK_SECTOR) {
      // loyalty in this town increases a bit because we obviously care about them...
      //***13.05.2008*** повышение величины увеличеия лояльности города от тренинга ополчения
      /// IncrementTownLoyalty( ubTownId, LOYALTY_BONUS_FOR_TOWN_TRAINING );
      IncrementTownLoyalty(ubTownId,
                           (gGameSettings.fOptions[NOPTION_LOYALTY_MILITIA_KILLED] == FALSE
                                ? LOYALTY_BONUS_FOR_TOWN_TRAINING
                                : LOYALTY_BONUS_FOR_TOWN_TRAINING * 4));
    }
  }

  // the trainer announces to player that he's finished his assignment.  Make his sector flash!
  AssignmentDone(pTrainer, TRUE, FALSE);

  // handle completion of town by training group
  HandleCompletionOfTownTrainingByGroupWithTrainer(pTrainer);
}

// feed this a SOLDIER_CLASS_, it will return you a _MITILIA rank, or -1 if the guy's not militia
INT8 SoldierClassToMilitiaRank(UINT8 ubSoldierClass) {
  INT8 bRank = -1;

  switch (ubSoldierClass) {
    case SOLDIER_CLASS_GREEN_MILITIA:
      bRank = GREEN_MILITIA;
      break;
    case SOLDIER_CLASS_REG_MILITIA:
      bRank = REGULAR_MILITIA;
      break;
    case SOLDIER_CLASS_ELITE_MILITIA:
      bRank = ELITE_MILITIA;
      break;
  }

  return (bRank);
}

// feed this a _MITILIA rank, it will return you a SOLDIER_CLASS_, or -1 if the guy's not militia
INT8 MilitiaRankToSoldierClass(UINT8 ubRank) {
  INT8 bSoldierClass = -1;

  switch (ubRank) {
    case GREEN_MILITIA:
      bSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
      break;
    case REGULAR_MILITIA:
      bSoldierClass = SOLDIER_CLASS_REG_MILITIA;
      break;
    case ELITE_MILITIA:
      bSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
      break;
  }

  return (bSoldierClass);
}

void StrategicAddMilitiaToSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT8 ubHowMany) {
  SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR(sMapX, sMapY)]);

  pSectorInfo->ubNumberOfCivsAtLevel[ubRank] += ubHowMany;

  // update the screen display
  fMapPanelDirty = TRUE;
}

void StrategicPromoteMilitiaInSector(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank,
                                     UINT8 ubHowMany) {
  SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR(sMapX, sMapY)]);

  // damn well better have that many around to promote!
  Assert(pSectorInfo->ubNumberOfCivsAtLevel[ubCurrentRank] >= ubHowMany);

  // KM : July 21, 1999 patch fix
  if (pSectorInfo->ubNumberOfCivsAtLevel[ubCurrentRank] < ubHowMany) {
    return;
  }

  pSectorInfo->ubNumberOfCivsAtLevel[ubCurrentRank] -= ubHowMany;
  pSectorInfo->ubNumberOfCivsAtLevel[ubCurrentRank + 1] += ubHowMany;

  // update the screen display
  fMapPanelDirty = TRUE;
}

void StrategicRemoveMilitiaFromSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT8 ubHowMany) {
  SECTORINFO *pSectorInfo = &(SectorInfo[SECTOR(sMapX, sMapY)]);

  // damn well better have that many around to remove!
  /// Assert(pSectorInfo->ubNumberOfCivsAtLevel[ ubRank ] >= ubHowMany);

  // KM : July 21, 1999 patch fix
  if (pSectorInfo->ubNumberOfCivsAtLevel[ubRank] < ubHowMany) {
    return;
  }

  pSectorInfo->ubNumberOfCivsAtLevel[ubRank] -= ubHowMany;

  // update the screen display
  fMapPanelDirty = TRUE;
}

// kill pts are (2 * kills) + assists
UINT8 CheckOneMilitiaForPromotion(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank,
                                  UINT8 ubRecentKillPts) {
  UINT32 uiChanceToLevel = 0;

  switch (ubCurrentRank) {
    case GREEN_MILITIA:
      // 2 kill pts minimum
      if (ubRecentKillPts >= 2) {
        // 25% chance per kill pt
        uiChanceToLevel = 25 * ubRecentKillPts;
      }
      break;
    case REGULAR_MILITIA:
      // 5 kill pts minimum
      if (ubRecentKillPts >= 5) {
        // 10% chance per kill pt.
        uiChanceToLevel = 10 * ubRecentKillPts;
      }
      break;
    case ELITE_MILITIA:
      return 0;
      break;
  }
  // roll the bones, and see if he makes it
  if (Random(100) < uiChanceToLevel) {
    StrategicPromoteMilitiaInSector(sMapX, sMapY, ubCurrentRank, 1);
    if (ubCurrentRank == GREEN_MILITIA) {  // Attempt yet another level up if sufficient points
      if (ubRecentKillPts > 2) {
        if (CheckOneMilitiaForPromotion(
                sMapX, sMapY, REGULAR_MILITIA,
                (UINT8)(ubRecentKillPts - 2))) {  // success, this militia was promoted twice
          return 2;
        }
      }
    }
    return 1;
  }
  return 0;
}

// call this if the player attacks his own militia
void HandleMilitiaDefections(INT16 sMapX, INT16 sMapY) {
  UINT8 ubRank;
  UINT8 ubMilitiaCnt;
  UINT8 ubCount;
  UINT32 uiChanceToDefect;

  for (ubRank = 0; ubRank < MAX_MILITIA_LEVELS; ubRank++) {
    ubMilitiaCnt = MilitiaInSectorOfRank(sMapX, sMapY, ubRank);

    // check each guy at each rank to see if he defects
    for (ubCount = 0; ubCount < ubMilitiaCnt; ubCount++) {
      switch (ubRank) {
        case GREEN_MILITIA:
          uiChanceToDefect = 50;
          break;
        case REGULAR_MILITIA:
          uiChanceToDefect = 75;
          break;
        case ELITE_MILITIA:
          uiChanceToDefect = 90;
          break;
        default:
          Assert(0);
          return;
      }

      // roll the bones; should I stay or should I go now?  (for you music fans out there)
      if (Random(100) < uiChanceToDefect) {
        // B'bye!  (for you SNL fans out there)
        StrategicRemoveMilitiaFromSector(sMapX, sMapY, ubRank, 1);
      }
    }
  }
}

UINT8 CountAllMilitiaInSector(INT16 sMapX, INT16 sMapY) {
  UINT8 ubMilitiaTotal = 0;
  UINT8 ubRank;

  // find out if there are any town militia in this SECTOR (don't care about other sectors in same
  // town)
  for (ubRank = 0; ubRank < MAX_MILITIA_LEVELS; ubRank++) {
    ubMilitiaTotal += MilitiaInSectorOfRank(sMapX, sMapY, ubRank);
  }

  return (ubMilitiaTotal);
}

UINT8 MilitiaInSectorOfRank(INT16 sMapX, INT16 sMapY, UINT8 ubRank) {
  return (SectorInfo[SECTOR(sMapX, sMapY)].ubNumberOfCivsAtLevel[ubRank]);
}

BOOLEAN SectorOursAndPeaceful(INT16 sMapX, INT16 sMapY, INT8 bMapZ) {
  // if this sector is currently loaded
  if ((sMapX == gWorldSectorX) && (sMapY == gWorldSectorY) && (bMapZ == gbWorldSectorZ)) {
    // and either there are enemies prowling this sector, or combat is in progress
    if (gTacticalStatus.fEnemyInSector || (gTacticalStatus.uiFlags & INCOMBAT)) {
      return FALSE;
    }
  }

  // if sector is controlled by enemies, it's not ours (duh!)
  if (!bMapZ && StrategicMap[sMapX + sMapY * MAP_WORLD_X].fEnemyControlled == TRUE) {
    return FALSE;
  }

  if (NumHostilesInSector(sMapX, sMapY, bMapZ)) {
    return FALSE;
  }

  // safe & secure, s'far as we can tell
  return (TRUE);
}

void InitFriendlyTownSectorServer(UINT8 ubTownId, INT16 sSkipSectorX, INT16 sSkipSectorY) {
  // reset globals
  gubTownSectorServerTownId = ubTownId;
  gsTownSectorServerSkipX = sSkipSectorX;
  gsTownSectorServerSkipY = sSkipSectorY;

  gubTownSectorServerIndex = 0;
}

// this feeds the X,Y of the next town sector on the town list for the town specified at
// initialization it will skip an entry that matches the skip X/Y value if one was specified at
// initialization MUST CALL InitFriendlyTownSectorServer() before using!!!
BOOLEAN ServeNextFriendlySectorInTown(INT16 *sNeighbourX, INT16 *sNeighbourY) {
  INT32 iTownSector;
  INT16 sMapX, sMapY;
  BOOLEAN fStopLooking = FALSE;

  do {
    // have we reached the end of the town list?
    if (pTownNamesList[gubTownSectorServerIndex] == BLANK_SECTOR) {
      // end of list reached
      return (FALSE);
    }

    iTownSector = pTownLocationsList[gubTownSectorServerIndex];

    // if this sector is in the town we're looking for
    if (StrategicMap[iTownSector].bNameId == gubTownSectorServerTownId) {
      // A sector in the specified town.  Calculate its X & Y sector compotents
      sMapX = iTownSector % MAP_WORLD_X;
      sMapY = iTownSector / MAP_WORLD_X;

      // Make sure we're not supposed to skip it
      if ((sMapX != gsTownSectorServerSkipX) || (sMapY != gsTownSectorServerSkipY)) {
        // check if it's "friendly" - not enemy controlled, no enemies in it, no combat in progress
        if (SectorOursAndPeaceful(sMapX, sMapY, 0)) {
          // then that's it!
          *sNeighbourX = sMapX;
          *sNeighbourY = sMapY;

          fStopLooking = TRUE;
        }
      }
    }

    // advance to next entry in town list
    gubTownSectorServerIndex++;

  } while (!fStopLooking);

  // found & returning a valid sector
  return (TRUE);
}

void HandleInterfaceMessageForCostOfTrainingMilitia(SOLDIERCLASS *pSoldier) {
  CHAR16 sString[128];
  SGPRect pCenteringRect = {0, 0, 640, INV_INTERFACE_START_Y};
  INT32 iNumberOfSectors = 0;

  pCenteringRect.iRight = giScrW;
  pCenteringRect.iBottom = giScrH - 140;

  pMilitiaTrainerSoldier = pSoldier;

  // grab total number of sectors
  //***24.07.2010*** переделано на индивидуальный расчёт стоимости тренировки по секторам
  /*iNumberOfSectors = GetNumberOfUnpaidTrainableSectors( );
  Assert( iNumberOfSectors > 0 );

  // get total cost
  giTotalCostOfTraining = MILITIA_TRAINING_COST * iNumberOfSectors;
  Assert( giTotalCostOfTraining > 0 );*/
  giTotalCostOfTraining = GetNumberOfUnpaidTrainableSectors();  ///

  gfYesNoPromptIsForContinue = FALSE;

  if (LaptopSaveInfo.iCurrentBalance < giTotalCostOfTraining) {
    swprintf(sString, pMilitiaConfirmStrings[8], giTotalCostOfTraining);
    DoScreenIndependantMessageBox(sString, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback);
    return;
  }

  // ok to start training, ask player

  if (iNumberOfSectors > 1) {
    swprintf(sString, pMilitiaConfirmStrings[7], iNumberOfSectors, giTotalCostOfTraining,
             pMilitiaConfirmStrings[1]);
  } else {
    swprintf(sString, L"%s%d. %s", pMilitiaConfirmStrings[0], giTotalCostOfTraining,
             pMilitiaConfirmStrings[1]);
  }

  // if we are in mapscreen, make a pop up
  if (guiCurrentScreen == MAP_SCREEN) {
    DoMapMessageBox(MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO,
                    PayMilitiaTrainingYesNoBoxCallback);
  } else {
    DoMessageBox(MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, MSG_BOX_FLAG_YESNO,
                 PayMilitiaTrainingYesNoBoxCallback, &pCenteringRect);
  }

  return;
}

void DoContinueMilitiaTrainingMessageBox(INT16 sSectorX, INT16 sSectorY, STR16 str, UINT16 usFlags,
                                         MSGBOX_CALLBACK ReturnCallback) {
  if (sSectorX <= 10 && sSectorY >= 6 && sSectorY <= 11) {
    DoLowerScreenIndependantMessageBox(str, usFlags, ReturnCallback);
  } else {
    DoScreenIndependantMessageBox(str, usFlags, ReturnCallback);
  }
}

void HandleInterfaceMessageForContinuingTrainingMilitia(SOLDIERCLASS *pSoldier) {
  CHAR16 sString[128];
  INT16 sSectorX = 0, sSectorY = 0;
  CHAR16 sStringB[128];
  INT8 bTownId;

  sSectorX = pSoldier->sSectorX;
  sSectorY = pSoldier->sSectorY;

  Assert(SectorInfo[SECTOR(sSectorX, sSectorY)].fMilitiaTrainingPaid == FALSE);

  pMilitiaTrainerSoldier = pSoldier;

  gfYesNoPromptIsForContinue = TRUE;

  // is there enough loyalty to continue training
  if (DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia(pSoldier) == FALSE) {
    // loyalty too low to continue training
    swprintf(sString, pMilitiaConfirmStrings[9], pTownNames[GetTownIdForSector(sSectorX, sSectorY)],
             MIN_RATING_TO_TRAIN_TOWN);
    DoContinueMilitiaTrainingMessageBox(sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK,
                                        CantTrainMilitiaOkBoxCallback);
    return;
  }

  if (IsMilitiaTrainableFromSoldiersSectorMaxed(pSoldier)) {
    // we're full!!! go home!
    bTownId = GetTownIdForSector(sSectorX, sSectorY);
    if (bTownId == BLANK_SECTOR) {
      // wilderness SAM site
      GetSectorIDString(sSectorX, sSectorY, 0, sStringB, TRUE);
      swprintf(sString, pMilitiaConfirmStrings[10], sStringB, GetSectorIDString,
               MIN_RATING_TO_TRAIN_TOWN);
    } else {
      // town
      swprintf(sString, pMilitiaConfirmStrings[10], pTownNames[bTownId], MIN_RATING_TO_TRAIN_TOWN);
    }
    DoContinueMilitiaTrainingMessageBox(sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK,
                                        CantTrainMilitiaOkBoxCallback);
    return;
  }

  // continue training always handles just one sector at a time
  giTotalCostOfTraining =
      GetMilitiaTrainingCost(SECTOR(sSectorX, sSectorY));  /// MILITIA_TRAINING_COST;

  // can player afford to continue training?
  if (LaptopSaveInfo.iCurrentBalance < giTotalCostOfTraining) {
    // can't afford to continue training
    swprintf(sString, pMilitiaConfirmStrings[8], giTotalCostOfTraining);
    DoContinueMilitiaTrainingMessageBox(sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK,
                                        CantTrainMilitiaOkBoxCallback);
    return;
  }

  // ok to continue, ask player

  GetSectorIDString(sSectorX, sSectorY, 0, sStringB, TRUE);
  swprintf(sString, pMilitiaConfirmStrings[3], sStringB, pMilitiaConfirmStrings[4],
           giTotalCostOfTraining);

  // ask player whether he'd like to continue training
  // DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_YESNO,
  // PayMilitiaTrainingYesNoBoxCallback );
  DoMapMessageBox(MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO,
                  PayMilitiaTrainingYesNoBoxCallback);
}

// IMPORTANT: This same callback is used both for initial training and for continue training prompt
// use 'gfYesNoPromptIsForContinue' flag to tell them apart
void PayMilitiaTrainingYesNoBoxCallback(UINT8 bExitValue) {
  CHAR16 sString[128];

  Assert(giTotalCostOfTraining > 0);

  // yes
  if (bExitValue == MSG_BOX_RETURN_YES) {
    // does the player have enough
    if (LaptopSaveInfo.iCurrentBalance >= giTotalCostOfTraining) {
      if (gfYesNoPromptIsForContinue) {
        ContinueTrainingInThisSector();
      } else {
        StartTrainingInAllUnpaidTrainableSectors();
      }

#ifdef JA2BETAVERSION
      // put this BEFORE training gets handled to avoid detecting an error everytime a sector
      // completes training
      VerifyTownTrainingIsPaidFor();
#endif

      // this completes the training prompt sequence
      pMilitiaTrainerSoldier = NULL;
    } else  // can't afford it
    {
      StopTimeCompression();

      swprintf(sString, L"%s", pMilitiaConfirmStrings[2]);
      DoMapMessageBox(MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK,
                      CantTrainMilitiaOkBoxCallback);
    }
  } else if (bExitValue == MSG_BOX_RETURN_NO) {
    StopTimeCompression();

    MilitiaTrainingRejected();
  }

  return;
}

void CantTrainMilitiaOkBoxCallback(UINT8 bExitValue) {
  MilitiaTrainingRejected();
  return;
}

// IMPORTANT: This same callback is used both for initial training and for continue training prompt
// use 'gfYesNoPromptIsForContinue' flag to tell them apart
void MilitiaTrainingRejected(void) {
  if (gfYesNoPromptIsForContinue) {
    // take all mercs in that sector off militia training
    ResetAssignmentOfMercsThatWereTrainingMilitiaInThisSector(pMilitiaTrainerSoldier->sSectorX,
                                                              pMilitiaTrainerSoldier->sSectorY);
  } else {
    // take all mercs in unpaid sectors EVERYWHERE off militia training
    ResetAssignmentsForMercsTrainingUnpaidSectorsInSelectedList(0);
  }

#ifdef JA2BETAVERSION
  // put this BEFORE training gets handled to avoid detecting an error everytime a sector completes
  // training
  VerifyTownTrainingIsPaidFor();
#endif

  // this completes the training prompt sequence
  pMilitiaTrainerSoldier = NULL;
}

void HandleMilitiaStatusInCurrentMapBeforeLoadingNewMap(void) {
  if (gTacticalStatus.Team[MILITIA_TEAM].bSide != PLAYER_SIDE) {
    // handle militia defections and reset team to friendly
    HandleMilitiaDefections(gWorldSectorX, gWorldSectorY);
    gTacticalStatus.Team[MILITIA_TEAM].bSide = PLAYER_SIDE;
  } else if (!gfAutomaticallyStartAutoResolve) {  // Don't promote militia if we are going directly
                                                  // to autoresolve to finish the current battle.
    HandleMilitiaPromotions();
  }
}

BOOLEAN CanNearbyMilitiaScoutThisSector(INT16 sSectorX, INT16 sSectorY) {
  INT16 sSectorValue = 0, sSector = 0;
  INT16 sCounterA = 0, sCounterB = 0;
  UINT8 ubScoutingRange = 1;

  // get the sector value
  sSector = sSectorX + sSectorY * MAP_WORLD_X;

  for (sCounterA = sSectorX - ubScoutingRange; sCounterA <= sSectorX + ubScoutingRange;
       sCounterA++) {
    for (sCounterB = sSectorY - ubScoutingRange; sCounterB <= sSectorY + ubScoutingRange;
         sCounterB++) {
      // skip out of bounds sectors
      if ((sCounterA < 1) || (sCounterA > 16) || (sCounterB < 1) || (sCounterB > 16)) {
        continue;
      }

      sSectorValue = SECTOR(sCounterA, sCounterB);

      // check if any sort of militia here
      if (SectorInfo[sSectorValue].ubNumberOfCivsAtLevel[GREEN_MILITIA]) {
        return (TRUE);
      } else if (SectorInfo[sSectorValue].ubNumberOfCivsAtLevel[REGULAR_MILITIA]) {
        return (TRUE);
      } else if (SectorInfo[sSectorValue].ubNumberOfCivsAtLevel[ELITE_MILITIA]) {
        return (TRUE);
      }
    }
  }

  return (FALSE);
}

BOOLEAN IsTownFullMilitia(INT8 bTownId) {
  INT32 iCounter = 0;
  INT16 sSectorX = 0, sSectorY = 0;
  INT32 iNumberOfMilitia = 0;
  INT32 iMaxNumber = 0;

  while (pTownNamesList[iCounter] != 0) {
    if (pTownNamesList[iCounter] == bTownId) {
      // get the sector x and y
      sSectorX = pTownLocationsList[iCounter] % MAP_WORLD_X;
      sSectorY = pTownLocationsList[iCounter] / MAP_WORLD_X;

      // if sector is ours get number of militia here
      if (SectorOursAndPeaceful(sSectorX, sSectorY, 0)) {
        // don't count GREEN militia, they can be trained into regulars first
        //***5.11.2007*** дотренировка до элитных
        iNumberOfMilitia += MilitiaInSectorOfRank(sSectorX, sSectorY, ELITE_MILITIA);
        if (!gExtGameOptions.fBlueMilitia)
          iNumberOfMilitia += MilitiaInSectorOfRank(sSectorX, sSectorY, REGULAR_MILITIA);
        iMaxNumber += MAX_ALLOWABLE_MILITIA_PER_SECTOR;
      }
    }

    iCounter++;
  }

  // now check the number of militia
  if (iMaxNumber > iNumberOfMilitia) {
    return (FALSE);
  }

  return (TRUE);
}

BOOLEAN IsSAMSiteFullOfMilitia(INT16 sSectorX, INT16 sSectorY) {
  BOOLEAN fSamSitePresent = FALSE;
  INT32 iNumberOfMilitia = 0;
  INT32 iMaxNumber = 0;

  // check if SAM site is ours?
  fSamSitePresent = IsThisSectorASAMSector(sSectorX, sSectorY, 0);

  if (fSamSitePresent == FALSE) {
    return (FALSE);
  }

  if (SectorOursAndPeaceful(sSectorX, sSectorY, 0)) {
    // don't count GREEN militia, they can be trained into regulars first
    //***5.11.2007*** дотренировка до элитных
    iNumberOfMilitia += MilitiaInSectorOfRank(sSectorX, sSectorY, ELITE_MILITIA);
    if (!gExtGameOptions.fBlueMilitia)
      iNumberOfMilitia += MilitiaInSectorOfRank(sSectorX, sSectorY, REGULAR_MILITIA);
    iMaxNumber += MAX_ALLOWABLE_MILITIA_PER_SECTOR;
  }

  // now check the number of militia
  if (iMaxNumber > iNumberOfMilitia) {
    return (FALSE);
  }

  return (TRUE);
}

//***6.11.2007***
BOOLEAN IsRoadblockFullOfMilitia(INT16 sSectorX, INT16 sSectorY) {
  INT32 iNumberOfMilitia = 0;
  INT32 iMaxNumber = 0;

  if (IsThisSectorARoadblock(sSectorX, sSectorY) == FALSE) {
    return (FALSE);
  }

  if (SectorOursAndPeaceful(sSectorX, sSectorY, 0)) {
    // don't count GREEN militia, they can be trained into regulars first
    //***5.11.2007*** дотренировка до элитных
    iNumberOfMilitia += MilitiaInSectorOfRank(sSectorX, sSectorY, ELITE_MILITIA);
    if (!gExtGameOptions.fBlueMilitia)
      iNumberOfMilitia += MilitiaInSectorOfRank(sSectorX, sSectorY, REGULAR_MILITIA);
    iMaxNumber += MAX_ALLOWABLE_MILITIA_PER_SECTOR;
  }

  // now check the number of militia
  if (iMaxNumber > iNumberOfMilitia) {
    return (FALSE);
  }

  return (TRUE);
}

void HandleCompletionOfTownTrainingByGroupWithTrainer(SOLDIERCLASS *pTrainer) {
  INT16 sSectorX = 0, sSectorY = 0;
  INT8 bSectorZ = 0;
  SOLDIERCLASS *pSoldier = NULL;
  INT32 iCounter = 0;

  // get the sector values
  sSectorX = pTrainer->sSectorX;
  sSectorY = pTrainer->sSectorY;
  bSectorZ = pTrainer->bSectorZ;

  for (iCounter = 0; iCounter < MAX_CHARACTER_COUNT; iCounter++) {
    // valid character?
    if (gCharactersList[iCounter].fValid == FALSE) {
      // nope
      continue;
    }

    pSoldier = &Menptr[gCharactersList[iCounter].usSolID];

    // valid soldier?
    if (pSoldier->bActive == FALSE) {
      continue;
    }

    if ((pSoldier->bAssignment == TRAIN_TOWN) && (pSoldier->sSectorX == sSectorX) &&
        (pSoldier->sSectorY == sSectorY) && (pSoldier->bSectorZ == bSectorZ)) {
      // done assignment
      AssignmentDone(pSoldier, FALSE, FALSE);
    }
  }

  return;
}

void AddSectorForSoldierToListOfSectorsThatCompletedMilitiaTraining(SOLDIERCLASS *pSoldier) {
  INT32 iCounter = 0;
  INT16 sSector = 0, sCurrentSector = 0;
  SOLDIERCLASS *pCurrentSoldier = NULL;

  // get the sector value
  sSector = pSoldier->sSectorX + pSoldier->sSectorY * MAP_WORLD_X;

  while (giListOfMercsInSectorsCompletedMilitiaTraining[iCounter] != -1) {
    // get the current soldier
    pCurrentSoldier = &Menptr[giListOfMercsInSectorsCompletedMilitiaTraining[iCounter]];

    // get the current sector value
    sCurrentSector = pCurrentSoldier->sSectorX + pCurrentSoldier->sSectorY * MAP_WORLD_X;

    // is the merc's sector already in the list?
    if (sCurrentSector == sSector) {
      // already here
      return;
    }

    iCounter++;

    Assert(iCounter < SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST);
  }

  // add merc to the list
  giListOfMercsInSectorsCompletedMilitiaTraining[iCounter] = pSoldier->ubID;

  return;
}

// clear out the list of training sectors...should be done once the list is posted
void ClearSectorListForCompletedTrainingOfMilitia(void) {
  INT32 iCounter = 0;

  for (iCounter = 0; iCounter < SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST; iCounter++) {
    giListOfMercsInSectorsCompletedMilitiaTraining[iCounter] = -1;
  }

  return;
}

void HandleContinueOfTownTraining(void) {
  SOLDIERCLASS *pSoldier = NULL;
  INT32 iCounter = 0;
  BOOLEAN fContinueEventPosted = FALSE;

  while (giListOfMercsInSectorsCompletedMilitiaTraining[iCounter] != -1) {
    // get the soldier
    pSoldier = &Menptr[giListOfMercsInSectorsCompletedMilitiaTraining[iCounter]];

    if (pSoldier->bActive) {
      fContinueEventPosted = TRUE;
      SpecialCharacterDialogueEvent(DIALOGUE_SPECIAL_EVENT_CONTINUE_TRAINING_MILITIA,
                                    pSoldier->ubProfile, 0, 0, 0, 0);

      // now set all of these peoples assignment done too
      // HandleInterfaceMessageForContinuingTrainingMilitia( pSoldier );
    }

    // next entry
    iCounter++;
  }

  // now clear the list
  ClearSectorListForCompletedTrainingOfMilitia();

  if (fContinueEventPosted) {
    // ATE: If this event happens in tactical mode we will be switching at some time to mapscreen...
    if (guiCurrentScreen == GAME_SCREEN) {
      gfEnteringMapScreen = TRUE;
    }

    // If the militia view isn't currently active, then turn it on when prompting to continue
    // training.
    if (!fShowMilitia) {
      ToggleShowMilitiaMode();
    }
  }

  return;
}

void BuildListOfUnpaidTrainableSectors(void) {
  INT32 iCounter = 0, iCounterB = 0;
  SOLDIERCLASS *pSoldier = NULL;

  memset(gsUnpaidStrategicSector, 0, sizeof(INT16) * MAX_CHARACTER_COUNT);

  if (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN) {
    for (iCounter = 0; iCounter < MAX_CHARACTER_COUNT; iCounter++) {
      // valid character?
      if (gCharactersList[iCounter].fValid) {
        // selected?
        if ((fSelectedListOfMercsForMapScreen[iCounter] == TRUE) ||
            (iCounter == bSelectedAssignChar)) {
          pSoldier = &Menptr[gCharactersList[iCounter].usSolID];

          if (CanCharacterTrainMilitia(pSoldier) == TRUE) {
            if (SectorInfo[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)].fMilitiaTrainingPaid ==
                FALSE) {
              // check to see if this sector is a town and needs equipment
              gsUnpaidStrategicSector[iCounter] =
                  CALCULATE_STRATEGIC_INDEX(pSoldier->sSectorX, pSoldier->sSectorY);
            }
          }
        }
      }
    }
  } else {
    // handle for tactical
    pSoldier = &Menptr[gusUIFullTargetID];
    iCounter = 0;

    if (CanCharacterTrainMilitia(pSoldier) == TRUE) {
      if (SectorInfo[SECTOR(pSoldier->sSectorX, pSoldier->sSectorY)].fMilitiaTrainingPaid ==
          FALSE) {
        // check to see if this sector is a town and needs equipment
        gsUnpaidStrategicSector[iCounter] =
            CALCULATE_STRATEGIC_INDEX(pSoldier->sSectorX, pSoldier->sSectorY);
      }
    }
  }

  // now clean out repeated sectors
  for (iCounter = 0; iCounter < MAX_CHARACTER_COUNT - 1; iCounter++) {
    if (gsUnpaidStrategicSector[iCounter] > 0) {
      for (iCounterB = iCounter + 1; iCounterB < MAX_CHARACTER_COUNT; iCounterB++) {
        if (gsUnpaidStrategicSector[iCounterB] == gsUnpaidStrategicSector[iCounter]) {
          gsUnpaidStrategicSector[iCounterB] = 0;
        }
      }
    }
  }
}

INT32 GetNumberOfUnpaidTrainableSectors(void) {
  INT32 iCounter = 0;
  INT32 iNumberOfSectors = 0;

  BuildListOfUnpaidTrainableSectors();

  // now count up the results
  for (iCounter = 0; iCounter < MAX_CHARACTER_COUNT; iCounter++) {
    if (gsUnpaidStrategicSector[iCounter] > 0) {
      /// iNumberOfSectors++;
      //***24.07.2010*** вычисляем стоимость тренировки для каждого сектора
      iNumberOfSectors +=
          GetMilitiaTrainingCost(STRATEGIC_INDEX_TO_SECTOR_INFO(gsUnpaidStrategicSector[iCounter]));
    }
  }

  // return the result
  return (iNumberOfSectors);
}

void StartTrainingInAllUnpaidTrainableSectors() {
  INT32 iCounter = 0;
  UINT8 ubSector;

  SetAssignmentForList(TRAIN_TOWN, 0);

  BuildListOfUnpaidTrainableSectors();

  // pay up in each sector
  for (iCounter = 0; iCounter < MAX_CHARACTER_COUNT; iCounter++) {
    if (gsUnpaidStrategicSector[iCounter] > 0) {
      // convert strategic sector to 0-255 system
      ubSector = STRATEGIC_INDEX_TO_SECTOR_INFO(gsUnpaidStrategicSector[iCounter]);
      PayForTrainingInSector(ubSector);
    }
  }
}

void ContinueTrainingInThisSector(void) {
  UINT8 ubSector;

  Assert(pMilitiaTrainerSoldier);

  // pay up in the sector where pMilitiaTrainerSoldier is
  ubSector = SECTOR(pMilitiaTrainerSoldier->sSectorX, pMilitiaTrainerSoldier->sSectorY);
  PayForTrainingInSector(ubSector);
}

void PayForTrainingInSector(UINT8 ubSector) {
  Assert(SectorInfo[ubSector].fMilitiaTrainingPaid == FALSE);

  // spend the money
  AddTransactionToPlayersBook(TRAIN_TOWN_MILITIA, ubSector, GetWorldTotalMin(),
                              -(GetMilitiaTrainingCost(ubSector) /*MILITIA_TRAINING_COST*/));

  if (!gGameOptions.fLimitedMilitia)  //***15.11.2014***
  {
    // mark this sector sectors as being paid up
    SectorInfo[ubSector].fMilitiaTrainingPaid = TRUE;
  } else {
    //***10.09.2013***
    SectorInfo[ubSector].fMilitiaTrainingPaid = SectorInfo[ubSector].ubNumberOfCivsAtLevel[(
        gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA : REGULAR_MILITIA)];
  }
  // reset done flags
  ResetDoneFlagForAllMilitiaTrainersInSector(ubSector);
}

void ResetDoneFlagForAllMilitiaTrainersInSector(UINT8 ubSector) {
  INT32 iCounter = 0;
  SOLDIERCLASS *pSoldier = NULL;

  for (iCounter = 0; iCounter <= gTacticalStatus.Team[OUR_TEAM].bLastID; iCounter++) {
    pSoldier = &Menptr[iCounter];

    if (pSoldier->bActive) {
      if (pSoldier->bAssignment == TRAIN_TOWN) {
        if ((SECTOR(pSoldier->sSectorX, pSoldier->sSectorY) == ubSector) &&
            (pSoldier->bSectorZ == 0)) {
          pSoldier->fDoneAssignmentAndNothingToDoFlag = FALSE;
          pSoldier->usQuoteSaidExtFlags &= ~SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT;
        }
      }
    }
  }
}

BOOLEAN MilitiaTrainingAllowedInSector(INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ) {
  INT8 bTownId;
  BOOLEAN fSamSitePresent = FALSE;

  if (bSectorZ != 0) {
    return (FALSE);
  }

  //***28.07.2013*** при моментальном наборе разрешено только повышение квалфикации охраны
  if (gGameOptions.fLimitedMilitia  //***15.11.2014***
      && SectorInfo[SECTOR(sSectorX, sSectorY)].ubNumberOfCivsAtLevel[(
             gExtGameOptions.fBlueMilitia == FALSE ? GREEN_MILITIA : REGULAR_MILITIA)] == 0)
    return (FALSE);

  fSamSitePresent = IsThisSectorASAMSector(sSectorX, sSectorY, bSectorZ);

  if (fSamSitePresent) {
    // all SAM sites may have militia trained at them
    return (TRUE);
  }

  //***6.11.2007*** блокпост?
  if (IsThisSectorARoadblock(sSectorX, sSectorY) /*&& gStrategicStatus.ubHighestProgress > 10*/) {
    return (TRUE);
  }

  bTownId = GetTownIdForSector(sSectorX, sSectorY);

  return (MilitiaTrainingAllowedInTown(bTownId));
}

BOOLEAN MilitiaTrainingAllowedInTown(INT8 bTownId) {
  switch (bTownId) {
    case DRASSEN:
    case ALMA:
    case GRUMM:
    case CAMBRIA:
    case BALIME:
    case MEDUNA:
    case CHITZENA:
      return (TRUE);

    case OMERTA:
    case ESTONI:
    case SAN_MONA:
    case TIXA:
    case ORTA:
      // can't keep militia in these towns
      return (FALSE);

    case BLANK_SECTOR:
    default:
      // not a town sector!
      return (FALSE);
  }
}

void BuildMilitiaPromotionsString(STR16 str) {
  CHAR16 pStr[256];
  BOOLEAN fAddSpace = FALSE;
  swprintf(str, L"");

  if (!gbMilitiaPromotions) {
    return;
  }
  if (gbGreenToElitePromotions > 1) {
    swprintf(pStr, gzLateLocalizedString[22], gbGreenToElitePromotions);
    wcscat(str, pStr);
    fAddSpace = TRUE;
  } else if (gbGreenToElitePromotions == 1) {
    wcscat(str, gzLateLocalizedString[29]);
    fAddSpace = TRUE;
  }

  if (gbGreenToRegPromotions > 1) {
    if (fAddSpace) {
      wcscat(str, L" ");
    }
    swprintf(pStr, gzLateLocalizedString[23], gbGreenToRegPromotions);
    wcscat(str, pStr);
    fAddSpace = TRUE;
  } else if (gbGreenToRegPromotions == 1) {
    if (fAddSpace) {
      wcscat(str, L" ");
    }
    wcscat(str, gzLateLocalizedString[30]);
    fAddSpace = TRUE;
  }

  if (gbRegToElitePromotions > 1) {
    if (fAddSpace) {
      wcscat(str, L" ");
    }
    swprintf(pStr, gzLateLocalizedString[24], gbRegToElitePromotions);
    wcscat(str, pStr);
  } else if (gbRegToElitePromotions == 1) {
    if (fAddSpace) {
      wcscat(str, L" ");
    }
    wcscat(str, gzLateLocalizedString[31]);
    fAddSpace = TRUE;
  }

  // Clear the fields
  gbGreenToElitePromotions = 0;
  gbGreenToRegPromotions = 0;
  gbRegToElitePromotions = 0;
  gbMilitiaPromotions = 0;
}
