#ifndef SOLDIER_MACROS_H
#define SOLDIER_MACROS_H

// MACROS FOR EASIER SOLDIER CONTROL
#include "Tactical/TeamTurns.h"
#include "Tactical/SoldierProfile.h"
#include "Strategic/Assignments.h"

// MACROS
#define RPC_RECRUITED(p)        \
  ((p->ubProfile == NO_PROFILE) \
       ? FALSE                  \
       : (gMercProfiles[p->ubProfile].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED))

#define AM_AN_EPC(p)            \
  ((p->ubProfile == NO_PROFILE) \
       ? FALSE                  \
       : (gMercProfiles[p->ubProfile].ubMiscFlags & PROFILE_MISC_FLAG_EPCACTIVE))

//***20.10.2007***
//#define AM_A_ROBOT( p )	( ( p->ubProfile == NO_PROFILE ) ? FALSE : ( gMercProfiles[
// p->ubProfile ].ubBodyType == ROBOTNOWEAPON ) ) #define AM_A_ROBOT( p )	( ( p->ubProfile ==
// NO_PROFILE ) ? (p->ubBodyType == ROBOTNOWEAPON || (p->uiStatusFlags & SOLDIER_ROBOT)) : (
// gMercProfiles[ p->ubProfile ].ubBodyType == ROBOTNOWEAPON ) )
#define AM_A_ROBOT(p) (p->ubBodyType == ROBOTNOWEAPON || (p->uiStatusFlags & SOLDIER_ROBOT))
#define NOT_A_ROBOT(p) (!AM_A_ROBOT(p))

#define OK_ENEMY_MERC(p) (!p->bNeutral && (!p->IsOnPlayerSide()) && p->bLife >= OKLIFE)

// Checks if our guy can be controllable .... checks bInSector, team, on duty, etc...
///#define OK_CONTROLLABLE_MERC( p ) ( p->bLife >= OKLIFE && p->bActive && p->bInSector && p->bTeam
///== PLAYER_TEAM && p->bAssignment < ON_DUTY  )
#define OK_CONTROLLABLE_MERC(p)                            \
  (p->bLife >= OKLIFE && p->bActive && p->bInSector &&     \
   (p->bTeam == PLAYER_TEAM && p->bAssignment < ON_DUTY || \
    gGameSettings.fOptions[NOPTION_CONTROLLED_MILITIA] &&  \
        p->bTeam == MILITIA_TEAM))  //***26.10.2014***

// Checks if our guy can be controllable .... checks bInSector, team, on duty, etc...
#define OK_INSECTOR_MERC(p)                                                       \
  (p->bLife >= OKLIFE && p->bActive && p->bInSector && p->bTeam == PLAYER_TEAM && \
   p->bAssignment < ON_DUTY)

// Checkf if our guy can be selected and is not in a position where our team has an interupt and he
// does not have one...
#define OK_INTERRUPT_MERC(p) ((INTERRUPT_QUEUED != 0) ? ((p->bMoved) ? FALSE : TRUE) : TRUE)

#define CREATURE_OR_BLOODCAT(p) ((p->uiStatusFlags & SOLDIER_MONSTER) || p->ubBodyType == BLOODCAT)

// JZ: 25.03.2015 Замена макроса "TANK( p )" на функцию
//#define TANK( p ) (p->ubBodyType == TANK_NE || p->ubBodyType == TANK_NW )
// JZ: 25.03.2015 Замена макроса "OK_ENTERABLE_VEHICLE( p )" на функцию
//#define OK_ENTERABLE_VEHICLE( p )	( ( p->uiStatusFlags & SOLDIER_VEHICLE ) && !TANK( p ) &&
// p->bLife >= OKLIFE  )

#endif
