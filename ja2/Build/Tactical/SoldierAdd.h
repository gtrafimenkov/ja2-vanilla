#ifndef _SOLDIER_ADD_H
#define _SOLDIER_ADD_H

#include "Tactical/SoldierControl.h"

// Finds a gridno given a sweet spot
// Returns a good direction too!
UINT16 FindGridNoFromSweetSpot(SOLDIERCLASS *pSoldier, INT16 sSweetGridNo, INT8 ubRadius,
                               UINT8 *pubDirection);

// Ensures a good path.....
UINT16 FindGridNoFromSweetSpotThroughPeople(SOLDIERCLASS *pSoldier, INT16 sSweetGridNo,
                                            INT8 ubRadius, UINT8 *pubDirection);

// Returns a good sweetspot but not the swetspot!
UINT16 FindGridNoFromSweetSpotExcludingSweetSpot(SOLDIERCLASS *pSoldier, INT16 sSweetGridNo,
                                                 INT8 ubRadius, UINT8 *pubDirection);

UINT16 FindGridNoFromSweetSpotExcludingSweetSpotInQuardent(SOLDIERCLASS *pSoldier,
                                                           INT16 sSweetGridNo, INT8 ubRadius,
                                                           UINT8 *pubDirection, INT8 ubQuardentDir);

// Finds a gridno near a sweetspot but a random one!
UINT16 FindRandomGridNoFromSweetSpot(SOLDIERCLASS *pSoldier, INT16 sSweetGridNo, INT8 ubRadius,
                                     UINT8 *pubDirection);

// Finds a sweetspot but excluding this one!
UINT16 FindRandomGridNoFromSweetSpotExcludingSweetSpot(SOLDIERCLASS *pSoldier, INT16 sSweetGridNo,
                                                       INT8 ubRadius, UINT8 *pubDirection);

// Adds a soldier ( already created in mercptrs[] array )!
// Finds a good placement based on data in the loaded sector and if they are enemy's or not, etc...
BOOLEAN AddSoldierToSector(UINT8 ubID);

BOOLEAN AddSoldierToSectorNoCalculateDirection(UINT8 ubID);

BOOLEAN AddSoldierToSectorNoCalculateDirectionUseAnimation(UINT8 ubID, UINT16 usAnimState,
                                                           UINT16 usAnimCode);

// IsMercOnTeam() checks to see if the passed in Merc Profile ID is currently on the player's team
BOOLEAN IsMercOnTeam(UINT8 ubMercID);
// requires non-intransit assignment, too
BOOLEAN IsMercOnTeamAndInOmertaAlready(UINT8 ubMercID);
// ATE: Added for contract renewals
BOOLEAN IsMercOnTeamAndAlive(UINT8 ubMercID);
// ATE: Added for contract renewals
BOOLEAN IsMercOnTeamAndInOmertaAlreadyAndAlive(UINT8 ubMercID);

// GetSoldierIDFromMercID() Gets the Soldier ID from the Merc Profile ID, else returns -1
INT16 GetSoldierIDFromMercID(UINT8 ubMercID);

UINT16 FindGridNoFromSweetSpotWithStructData(SOLDIERCLASS *pSoldier, UINT16 usAnimState,
                                             INT16 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection,
                                             BOOLEAN fClosestToMerc);

/*
void SoldierInSectorSleep( SOLDIERCLASS *pSoldier, INT16 sGridNo );
*/

UINT16 FindGridNoFromSweetSpotWithStructDataFromSoldier(SOLDIERCLASS *pSoldier, UINT16 usAnimState,
                                                        INT8 ubRadius, UINT8 *pubDirection,
                                                        BOOLEAN fClosestToMerc,
                                                        SOLDIERCLASS *pSrcSoldier);

void SoldierInSectorPatient(SOLDIERCLASS *pSoldier, INT16 sGridNo);
void SoldierInSectorDoctor(SOLDIERCLASS *pSoldier, INT16 sGridNo);
void SoldierInSectorRepair(SOLDIERCLASS *pSoldier, INT16 sGridNo);

BOOLEAN CanSoldierReachGridNoInGivenTileLimit(SOLDIERCLASS *pSoldier, INT16 sGridNo,
                                              INT16 sMaxTiles, INT8 bLevel);

#endif
