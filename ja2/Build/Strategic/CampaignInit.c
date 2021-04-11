#include "Strategic/StrategicAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "SGP/Types.h"
#include "Strategic/CampaignInit.h"
#include "SGP/Random.h"
#include "Strategic/CampaignTypes.h"
#include "Strategic/QueenCommand.h"
#include "Tactical/Overhead.h"
#include "Strategic/StrategicMovement.h"
#include "Strategic/StrategicStatus.h"
#include "GameSettings.h"
#include "Strategic/CreatureSpreading.h"
#include "Strategic/StrategicAI.h"
#endif

extern void InitStrategicMovementCosts();
void InitKnowFacilitiesFlags();

UNDERGROUND_SECTORINFO *gpUndergroundSectorInfoTail = NULL;

UNDERGROUND_SECTORINFO *NewUndergroundNode(UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ) {
  UNDERGROUND_SECTORINFO *curr;
  curr = (UNDERGROUND_SECTORINFO *)MemAlloc(sizeof(UNDERGROUND_SECTORINFO));
  AssertMsg(curr, "Failed to create an underground sector info node.");
  memset(curr, 0, sizeof(UNDERGROUND_SECTORINFO));

  curr->ubSectorX = ubSectorX;
  curr->ubSectorY = ubSectorY;
  curr->ubSectorZ = ubSectorZ;

  if (gpUndergroundSectorInfoTail) {
    gpUndergroundSectorInfoTail->next = curr;
    gpUndergroundSectorInfoTail = gpUndergroundSectorInfoTail->next;
  } else {
    gpUndergroundSectorInfoHead = curr;
    gpUndergroundSectorInfoTail = gpUndergroundSectorInfoHead;
  }
  return curr;
}

//***06.07.2008*** загрузка дополнительных подземных секторов из файла
#define BUFSIZE 50
void LoadNewUndergroundSectors(void) {
  int i, value;
  FILE *f;
  char c, szBuf[BUFSIZE];
  UINT8 ubSectorX, ubSectorY, ubSectorZ;
  UINT8 ubNumElites, ubNumTroops, ubNumAdmins, ubNumCreatures;
  UINT8 ubAdjacentSectors;
  UNDERGROUND_SECTORINFO *pSector;

  if ((f = fopen(".\\MapSettings\\underground.txt", "r")) == NULL) return;

  while (!feof(f)) {
    //чтение идентификаторов строк параметров
    if (fscanf(f, "%s", &szBuf) <= 0) continue;
    for (i = 0; szBuf[i] != 0; i++) {
      szBuf[i] = (char)toupper(szBuf[i]);
    }

    //обработка строк комментариев
    if (strcmp(szBuf, "REM") == 0) {
      while ((fgetc(f) != '\n') && !feof(f))
        ;
      continue;
    }

    //обработка параметров сектора
    if (strcmp(szBuf, "SECTOR") == 0) {
      // YX
      if (fscanf(f, "%c", &c) <= 0) continue;  // пропускаем Tab
      if (fscanf(f, "%c%d", &c, &value) > 0) {
        c = (char)toupper(c);
        if (c < 'A' || c > 'P') continue;
        ubSectorY = c - 'A' + 1;

        if (value < 1 || value > 16) continue;
        ubSectorX = (UINT8)value;
      }

      // Z
      if (fscanf(f, "%d", &value) > 0) {
        if (value < 1 || value > 3) continue;
        ubSectorZ = (UINT8)value;
      }

      // Соседние сектора (битовая маска)
      if (fscanf(f, "%d", &value) > 0) {
        if (value > 15) continue;
        ubAdjacentSectors = (UINT8)value;
      }

      // Admins
      if (fscanf(f, "%d", &value) > 0) {
        if (value > 32) continue;
        ubNumAdmins = (UINT8)value;
      }

      // Troops
      if (fscanf(f, "%d", &value) > 0) {
        if (value > 32) continue;
        ubNumTroops = (UINT8)value;
      }

      // Elites
      if (fscanf(f, "%d", &value) > 0) {
        if (value > 32) continue;
        ubNumElites = (UINT8)value;
      }

      // Creatures
      if (fscanf(f, "%d", &value) > 0) {
        if (value > 32) continue;
        ubNumCreatures = (UINT8)value;
      }

      pSector = FindUnderGroundSector(ubSectorX, ubSectorY, ubSectorZ);
      if (!pSector) {
        pSector = NewUndergroundNode(ubSectorX, ubSectorY, ubSectorZ);
      }

      if (pSector) {
        pSector->ubAdjacentSectors = ubAdjacentSectors;
        pSector->ubNumCreatures = ubNumCreatures;
        ubNumAdmins = ubNumAdmins * gGameOptions.ubDifficultyLevel;
        ubNumTroops = ubNumTroops * gGameOptions.ubDifficultyLevel;
        ubNumElites = ubNumElites * gGameOptions.ubDifficultyLevel;
        if ((ubNumAdmins + ubNumTroops + ubNumElites) > 32) {
          ubNumAdmins = 0;
          ubNumTroops = 0;
          ubNumElites = 32;
        }
        pSector->ubNumAdmins = ubNumAdmins;
        pSector->ubNumTroops = ubNumTroops;
        pSector->ubNumElites = ubNumElites;
      }
    }
  }  // while
  fclose(f);
}

// setup which know facilities are in which cities
void InitKnowFacilitiesFlags() {
  SECTORINFO *pSector;

  // Cambria hospital
  pSector = &SectorInfo[SEC_G8];
  pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
  pSector = &SectorInfo[SEC_F8];
  pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
  pSector = &SectorInfo[SEC_G9];
  pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
  pSector = &SectorInfo[SEC_F9];
  pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;

  // Drassen airport
  /**	pSector = &SectorInfo[SEC_B13];
          pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
          pSector = &SectorInfo[SEC_C13];
          pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
          pSector = &SectorInfo[SEC_D13];
          pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
  **/
  pSector = &SectorInfo[SEC_H9];
  pSector->uiFacilitiesFlags |= SFCF_AIRPORT;

  // Meduna airport & military complex
  pSector = &SectorInfo[SEC_N3];
  pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
  pSector = &SectorInfo[SEC_N4];
  pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
  pSector = &SectorInfo[SEC_N5];
  pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
  pSector = &SectorInfo[SEC_O3];
  pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
  pSector = &SectorInfo[SEC_O4];
  pSector->uiFacilitiesFlags |= SFCF_AIRPORT;

  return;
}

void InitMiningLocations() {
  SECTORINFO *pSector;
  // Set up mining sites

  pSector = &SectorInfo[SEC_D4];
  pSector->uiFlags |= SF_MINING_SITE;
  //	pSector->ubIncomeValue = 33;

  pSector = &SectorInfo[SEC_D13];
  pSector->uiFlags |= SF_MINING_SITE;
  //	pSector->ubIncomeValue = 41;

  pSector = &SectorInfo[SEC_B2];
  pSector->uiFlags |= SF_MINING_SITE;
  //	pSector->ubIncomeValue = 20;

  pSector = &SectorInfo[SEC_H8];
  pSector->uiFlags |= SF_MINING_SITE;
  //	pSector->ubIncomeValue = 64;

  pSector = &SectorInfo[SEC_I14];
  pSector->uiFlags |= SF_MINING_SITE;
  //	pSector->ubIncomeValue = 80;

  // Grumm
  pSector = &SectorInfo[SEC_H3];
  pSector->uiFlags |= SF_MINING_SITE;
  //	pSector->ubIncomeValue = 100;
}

// Mobile groups are handled separately from sectors, because they are on the move.
void GeneratePatrolGroups() {
  GROUP *pGroup;
  UINT8 ubNumTroops;
  ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random(3));
  pGroup = CreateNewEnemyGroupDepartingFromSector(SEC_C7, 0, ubNumTroops, 0);
  pGroup->ubTransportationMask = CAR;
  AddWaypointToPGroup(pGroup, 8, 3);  // C8
  AddWaypointToPGroup(pGroup, 7, 3);  // C7

  ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random(3));
  pGroup = CreateNewEnemyGroupDepartingFromSector(SEC_D9, 0, ubNumTroops, 0);
  AddWaypointToPGroup(pGroup, 9, 5);  // E9
  AddWaypointToPGroup(pGroup, 9, 4);  // D9
  pGroup->ubTransportationMask = TRUCK;

  ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random(3));
  pGroup = CreateNewEnemyGroupDepartingFromSector(SEC_B9, 0, ubNumTroops, 0);
  AddWaypointToPGroup(pGroup, 12, 2);  // B12
  AddWaypointToPGroup(pGroup, 9, 2);   // B9
  pGroup->ubTransportationMask = FOOT;

  ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random(3));
  pGroup = CreateNewEnemyGroupDepartingFromSector(SEC_A14, 0, ubNumTroops, 0);
  pGroup->ubMoveType = ENDTOEND_FORWARDS;
  AddWaypointToPGroup(pGroup, 13, 1);  // A13
  AddWaypointToPGroup(pGroup, 15, 1);  // A15
  AddWaypointToPGroup(pGroup, 15, 5);  // E15
  AddWaypointToPGroup(pGroup, 13, 5);  // E13
  AddWaypointToPGroup(pGroup, 12, 5);  // E12
  AddWaypointToPGroup(pGroup, 12, 3);  // C12
  pGroup->ubTransportationMask = TRACKED;

  ubNumTroops = (UINT8)(5 + gGameOptions.ubDifficultyLevel * 2 + Random(4));
  pGroup = CreateNewEnemyGroupDepartingFromSector(SEC_N6, 0, ubNumTroops, 0);
  AddWaypointToPGroup(pGroup, 9, 14);  // N9
  AddWaypointToPGroup(pGroup, 6, 14);  // N6
  pGroup->ubTransportationMask = CAR;

  ubNumTroops = (UINT8)(5 + gGameOptions.ubDifficultyLevel * 2 + Random(4));
  pGroup = CreateNewEnemyGroupDepartingFromSector(SEC_N10, 0, ubNumTroops, 0);
  AddWaypointToPGroup(pGroup, 10, 11);  // K10
  AddWaypointToPGroup(pGroup, 10, 14);  // N10
  pGroup->ubTransportationMask = CAR;
}

void TrashUndergroundSectorInfo() {
  UNDERGROUND_SECTORINFO *curr;
  while (gpUndergroundSectorInfoHead) {
    curr = gpUndergroundSectorInfoHead;
    gpUndergroundSectorInfoHead = gpUndergroundSectorInfoHead->next;
    MemFree(curr);
  }
  gpUndergroundSectorInfoHead = NULL;
  gpUndergroundSectorInfoTail = NULL;
}

// Defines the sectors that can be occupied by enemies, creatures, etc.  It also
// contains the network of cave connections critical for strategic creature spreading, as we can't
// know how the levels connect without loading the maps.  This is completely hardcoded, and any
// changes to the maps, require changes accordingly.
void BuildUndergroundSectorInfoList() {
  UNDERGROUND_SECTORINFO *curr;
  SECTORINFO *pSector = NULL;

  TrashUndergroundSectorInfo();

  //********************
  //* BASEMENT LEVEL 1 *
  //********************

  // Miguel's basement.  Nothing here.
  curr = NewUndergroundNode(10, 1, 1);

  // Chitzena mine.  Nothing here.
  curr = NewUndergroundNode(2, 2, 1);

  // San mona mine.  Nothing here.
  curr = NewUndergroundNode(4, 4, 1);
  curr = NewUndergroundNode(5, 4, 1);

  // J9
  curr = NewUndergroundNode(9, 10, 1);
  switch (gGameOptions.ubDifficultyLevel) {
    case DIF_LEVEL_EASY:
      curr->ubNumTroops = 8;
      break;
    case DIF_LEVEL_MEDIUM:
      curr->ubNumTroops = 11;
      break;
    case DIF_LEVEL_HARD:
      curr->ubNumTroops = 15;
      break;
  }
  // J9 feeding zone
  curr = NewUndergroundNode(9, 10, 2);
  curr->ubNumCreatures = (UINT8)(2 + gGameOptions.ubDifficultyLevel * 2 + Random(2));

  // K4
  curr = NewUndergroundNode(4, 11, 1);
  curr->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2 + Random(3));
  curr->ubNumElites = (UINT8)(4 + gGameOptions.ubDifficultyLevel + Random(2));

  // O3
  curr = NewUndergroundNode(3, 15, 1);
  curr->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel * 2 + Random(3));
  curr->ubNumElites = (UINT8)(4 + gGameOptions.ubDifficultyLevel + Random(2));
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;

  // P3
  curr = NewUndergroundNode(3, 16, 1);
  switch (gGameOptions.ubDifficultyLevel) {
    case DIF_LEVEL_EASY:
      curr->ubNumElites = (UINT8)(8 + Random(3));
      break;
    case DIF_LEVEL_MEDIUM:
      curr->ubNumElites = (UINT8)(10 + Random(5));
      break;
    case DIF_LEVEL_HARD:
      curr->ubNumElites = (UINT8)(14 + Random(6));
      break;
  }
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;

  // Do all of the mandatory underground mine sectors

  // Drassen's mine
  // D13_B1
  curr = NewUndergroundNode(13, 4, 1);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
  // E13_B1
  curr = NewUndergroundNode(13, 5, 1);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // E13_B2
  curr = NewUndergroundNode(13, 5, 2);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
  // F13_B2
  curr = NewUndergroundNode(13, 6, 2);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR | SOUTH_ADJACENT_SECTOR;
  // G13_B2
  curr = NewUndergroundNode(13, 7, 2);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // G13_B3
  curr = NewUndergroundNode(13, 7, 3);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // F13_B3
  curr = NewUndergroundNode(13, 6, 3);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;

  // Cambria's mine
  // H8_B1
  curr = NewUndergroundNode(8, 8, 1);
  curr->ubAdjacentSectors |= EAST_ADJACENT_SECTOR;
  // H9_B1
  curr = NewUndergroundNode(9, 8, 1);
  curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
  // H9_B2
  curr = NewUndergroundNode(9, 8, 2);
  curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
  // H8_B2
  curr = NewUndergroundNode(8, 8, 2);
  curr->ubAdjacentSectors |= EAST_ADJACENT_SECTOR;
  // H8_B3
  curr = NewUndergroundNode(8, 8, 3);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
  // I8_B3
  curr = NewUndergroundNode(8, 9, 3);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR | SOUTH_ADJACENT_SECTOR;
  // J8_B3
  curr = NewUndergroundNode(8, 10, 3);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;

  // Alma's mine
  // I14_B1
  curr = NewUndergroundNode(14, 9, 1);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
  // J14_B1
  curr = NewUndergroundNode(14, 10, 1);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // J14_B2
  curr = NewUndergroundNode(14, 10, 2);
  curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
  // J13_B2
  curr = NewUndergroundNode(13, 10, 2);
  curr->ubAdjacentSectors |= EAST_ADJACENT_SECTOR;
  // J13_B3
  curr = NewUndergroundNode(13, 10, 3);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
  // K13_B3
  curr = NewUndergroundNode(13, 11, 3);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;

  // Grumm's mine
  // H3_B1
  curr = NewUndergroundNode(3, 8, 1);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
  // I3_B1
  curr = NewUndergroundNode(3, 9, 1);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // I3_B2
  curr = NewUndergroundNode(3, 9, 2);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // H3_B2
  curr = NewUndergroundNode(3, 8, 2);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR | EAST_ADJACENT_SECTOR;
  // H4_B2
  curr = NewUndergroundNode(4, 8, 2);
  curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
  curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;
  // H4_B3
  curr = NewUndergroundNode(4, 8, 3);
  curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
  // G4_B3
  curr = NewUndergroundNode(4, 7, 3);
  curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;

  //***11.11.2007*** добавлены подземные сектора
  // P1_b1
  curr = NewUndergroundNode(1, 16, 1);
  switch (gGameOptions.ubDifficultyLevel) {
    case DIF_LEVEL_EASY:
      curr->ubNumElites = 11;
      break;
    case DIF_LEVEL_MEDIUM:
      curr->ubNumElites = 21;
      break;
    case DIF_LEVEL_HARD:
      curr->ubNumElites = 32;
      break;
  }

  // P1_b2
  curr = NewUndergroundNode(1, 16, 2);
  switch (gGameOptions.ubDifficultyLevel) {
    case DIF_LEVEL_EASY:
      curr->ubNumCreatures = 10;
      break;
    case DIF_LEVEL_MEDIUM:
      curr->ubNumCreatures = 20;
      break;
    case DIF_LEVEL_HARD:
      curr->ubNumCreatures = 30;
      break;
  }

  // H13_b1
  curr = NewUndergroundNode(13, 8, 1);
  switch (gGameOptions.ubDifficultyLevel) {
    case DIF_LEVEL_EASY:
      curr->ubNumElites = 5;
      curr->ubNumTroops = 5;
      break;
    case DIF_LEVEL_MEDIUM:
      curr->ubNumElites = 10;
      curr->ubNumTroops = 10;
      break;
    case DIF_LEVEL_HARD:
      curr->ubNumElites = 16;
      curr->ubNumTroops = 16;
      break;
  }

  // H14_b1
  curr = NewUndergroundNode(14, 8, 1);
  switch (gGameOptions.ubDifficultyLevel) {
    case DIF_LEVEL_EASY:
      curr->ubNumElites = 5;
      curr->ubNumTroops = 5;
      break;
    case DIF_LEVEL_MEDIUM:
      curr->ubNumElites = 10;
      curr->ubNumTroops = 10;
      break;
    case DIF_LEVEL_HARD:
      curr->ubNumElites = 16;
      curr->ubNumTroops = 16;
      break;
  }

  //***06.07.2008*** загрузка дополнительных подземных секторов из файла
  LoadNewUndergroundSectors();
}

// This is the function that is called only once, when the player begins a new game.  This will
// calculate starting numbers of the queen's army in various parts of the map, which will vary from
// campaign to campaign. This is also highly effected by the game's difficulty setting.
void InitNewCampaign() {
  // First clear all the sector information of all enemy existance.  Conveniently, the
  // ubGroupType is also cleared, which is perceived to be an empty group.
  memset(&SectorInfo, 0, sizeof(SECTORINFO) * 256);
  InitStrategicMovementCosts();
  RemoveAllGroups();

  InitMiningLocations();
  InitKnowFacilitiesFlags();

  BuildUndergroundSectorInfoList();

  // allow overhead view of omerta A9 on game onset
  SetSectorFlag(9, 1, 0, SF_ALREADY_VISITED);

  // Generates the initial forces in a new campaign.  The idea is to randomize numbers and sectors
  // so that no two games are the same.
  InitStrategicAI();

  InitStrategicStatus();
}
