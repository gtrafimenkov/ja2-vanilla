#ifndef __INVENTORY_CHOOSING_H
#define __INVENTORY_CHOOSING_H

#include "SGP/Types.h"
#include "Tactical/SoldierCreate.h"
#include "Strategic/StrategicStatus.h"

#define MORTAR_GRENADE_CLASS 100

typedef struct ARMY_GUN_CHOICE_TYPE {
  UINT8 ubChoices;   // how many valid choices there are in this category
  INT8 bItemNo[10];  // room for up to 5 choices of gun in each category
  //***23.06.2013***
  UINT8 ubArmourChoices;
  UINT16 usArmourItemNo[10];
} ARMY_GUN_CHOICE_TYPE;

enum {
  // administrator ratings (1-5)
  BAD_ADMINISTRATOR_EQUIPMENT_RATING = 1,
  POOR_ADMINISTRATOR_EQUIPMENT_RATING,
  AVERAGE_ADMINISTRATOR_EQUIPMENT_RATING,
  GOOD_ADMINISTRATOR_EQUIPMENT_RATING,
  GREAT_ADMINISTRATOR_EQUIPMENT_RATING,
  // army ratings (3-7)
  BAD_ARMY_EQUIPMENT_RATING =
      POOR_ADMINISTRATOR_EQUIPMENT_RATING,  /// AVERAGE_ADMINISTRATOR_EQUIPMENT_RATING,
  POOR_ARMY_EQUIPMENT_RATING,
  AVERAGE_ARMY_EQUIPMENT_RATING,
  GOOD_ARMY_EQUIPMENT_RATING,
  GREAT_ARMY_EQUIPMENT_RATING,
  // elite ratings (5-9)
  BAD_ELITE_EQUIPMENT_RATING = POOR_ARMY_EQUIPMENT_RATING,  /// AVERAGE_ARMY_EQUIPMENT_RATING,
  POOR_ELITE_EQUIPMENT_RATING,
  AVERAGE_ELITE_EQUIPMENT_RATING,
  GOOD_ELITE_EQUIPMENT_RATING,
  GREAT_ELITE_EQUIPMENT_RATING,

  // militia ratings (1-5)
  BAD_MILITIA_EQUIPMENT_RATING = BAD_ADMINISTRATOR_EQUIPMENT_RATING,
  POOR_MILITIA_EQUIPMENT_RATING,
  AVERAGE_MILITIA_EQUIPMENT_RATING,
  GOOD_MILITIA_EQUIPMENT_RATING,
  GREAT_MILITIA_EQUIPMENT_RATING,
};

#define MIN_EQUIPMENT_CLASS 1
#define MAX_EQUIPMENT_CLASS 11

//***17.10.2007***
// extern ARMY_GUN_CHOICE_TYPE gRegularArmyGunChoices[ARMY_GUN_LEVELS];
// extern ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[ARMY_GUN_LEVELS];
//***28.07.2010*** переделано
extern ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[7][ARMY_GUN_LEVELS];

// Selects at the start of the game the set of guns the Queen's army will use during this game
void InitArmyGunTypes(void);

// Chooses equipment based on the equipment level (1-11) with best being 11.  It allocates a range
// of equipment to choose from.
void GenerateRandomEquipment(SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass, INT8 bEquipmentRating);

void ChooseWeaponForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bWeaponClass,
                                        INT8 bAmmoClips, INT8 bAttachClass, BOOLEAN fAttachment);

void ChooseGrenadesForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bGrenades,
                                          INT8 bGrenadeClass, BOOLEAN fGrenadeLauncher);

void ChooseArmourForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bHelmetClass,
                                        INT8 bArmourClass, INT8 bLeggingsClass);

void ChooseSpecialWeaponsForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bKnifeClass,
                                                BOOLEAN fGrenadeLauncher, BOOLEAN fLAW,
                                                BOOLEAN fMortar);

void ChooseFaceGearForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp);

void ChooseLocationSpecificGearForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp);

BOOLEAN PlaceObjectInSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, OBJECTTYPE *pObject);

void AssignCreatureInventory(SOLDIERCLASS *pSoldier);

void ReplaceExtendedGuns(SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass);

UINT16 SelectStandardArmyGun(UINT8 uiGunLevel, UINT8 ubSoldierClass);

INT8 GetWeaponClass(UINT16 usGun, UINT8 ubSoldierClass);
void MarkAllWeaponsOfSameGunClassAsDropped(UINT16 usWeapon, UINT8 ubSoldierClass);

void ResetMortarsOnTeamCount(void);

#endif
