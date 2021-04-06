#include "Tactical/TacticalAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include <memory.h>
#include "Tactical/InventoryChoosing.h"
#include "Tactical/AnimationData.h"
#include "Tactical/Items.h"
#include "SGP/Random.h"
#include "Tactical/Weapons.h"
#include "Strategic/StrategicStatus.h"
#include "Tactical/Campaign.h"
#include "GameSettings.h"
#endif

/**
#define ENEMYAMMODROPRATE       50      // % of time enemies drop ammunition
#define ENEMYGRENADEDROPRATE    25      // % of time enemies drop grenades
#define ENEMYEQUIPDROPRATE      15      // % of stuff enemies drop equipment

// only 1/10th of what enemies drop...
#define MILITIAAMMODROPRATE      50 ///5      // % of time enemies drop ammunition
#define MILITIAGRENADEDROPRATE	 25 ///3      // % of time enemies drop grenades
#define MILITIAEQUIPDROPRATE     15 ///2      // % of stuff enemies drop equipment
**/
#define MAX_MORTARS_PER_TEAM \
  1  // one team can't randomly roll more than this many mortars per sector

//***06.10.2014*** для гибкой замены констант
UINT8 gubEnemyDropRate[NUM_DROPRATES] = {15, 50, 25, 15, 15, 15, 5, 15};
UINT8 gubMilitiaDropRate[NUM_DROPRATES] = {15, 50, 25, 15, 15, 15, 5, 15};

UINT32 guiMortarsRolledByTeam = 0;

//***28.07.2010*** закомментировано, переделано
// ARMY_GUN_CHOICE_TYPE gRegularArmyGunChoices[ARMY_GUN_LEVELS] =
//{	// INDEX		CLASS				 #CHOICES
//	{ /* 0 - lo pistols		*/	2,	SW38,				DESERTEAGLE,
//-1,				-1,				-1		}, 	{ /* 1 - hi
// pistols		*/	2,	GLOCK_17,			BERETTA_93R,		-1,
//-1,				-1		},
//	{ /* 2 - lo SMG/shotgun	*/	2,	M870,				MP5K,
//-1,				-1,				-1		}, 	{ /* 3 - lo
// rifles		*/	1,	MINI14,				-1,
//-1,				-1,				-1		}, 	{ /* 4 - hi
// SMGs
//*/	2,	MAC10,				COMMANDO,			-1,
//-1,				-1		}, 	{ /* 5 - med rifles  	*/	1,	G41,
//-1,					-1,				-1,
//-1
//},
//	{ /* 6 - sniper rifles	*/	1,	M24,				-1,
//-1,				-1,				-1		}, 	{	/* 7
//- hi rifles	*/	2,	M14,				C7,
// -1, -1,				-1		}, 	{ /* 8 - best rifle		*/
// 1,
// FNFAL,				-1,
//-1,				-1,				-1		}, 	{ /* 9 -
// machine guns	*/	1,	MINIMI,				-1,
// -1, -1,				-1		}, 	{ /* 10- rocket rifle	*/	2,
// ROCKET_RIFLE,		MINIMI,				-1,				-1,
//-1		},
//};

// ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[ARMY_GUN_LEVELS] =
//{	// INDEX		CLASS				 #CHOICES
//	{ /* 0 - lo pistols		*/	5,	SW38,				BARRACUDA,
// DESERTEAGLE,	GLOCK_17,		M1911,	}, 	{ /* 1 - hi pist/shtgn	*/	4,
// GLOCK_18,			BERETTA_93R,		BERETTA_92F,	M870,			-1
//},
//	{ /* 2 - lo SMGs/shtgn	*/	5,	TYPE85,				THOMPSON,
// MP53,			MP5K,			SPAS15	}, 	{ /* 3 - lo rifles    	*/
// 2, MINI14,				SKS,				-1,
// -1, -1		}, 	{ /* 4 - hi SMGs		*/	3,	MAC10,
// AKSU74, COMMANDO,		-1,				-1		}, 	{ /* 5 - med
// rifles
//*/	4,	AKM,				G3A3,				G41,
// AK74,			-1		}, 	{ /* 6 - sniper rifles	*/	2,
// DRAGUNOV, M24,				-1,				-1,
// -1
//},
//	{	/* 7 - hi rifles	*/	4,	FAMAS,				M14,
// AUG,			C7,				-1		}, 	{ /* 8 - best rifle
//*/	1,	FNFAL,				-1,					-1,
//-1,				-1		},
//	{ /* 9 - machine guns	*/	3,	MINIMI,				RPK74,
// HK21E,			-1,				-1		}, 	{ /* 10-
// rocket rifle	*/	4,	ROCKET_RIFLE,		ROCKET_RIFLE,		RPK74,
// HK21E,			-1		},
//};

ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[7][ARMY_GUN_LEVELS];

void RandomlyChooseWhichItemsAreDroppable(SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass);
void EquipTank(SOLDIERCREATE_STRUCT *pp);

void ChooseKitsForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bKitClass);
void ChooseMiscGearForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bMiscClass);
void ChooseBombsForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bBombClass);

//***28.10.2013***
BOOLEAN AllowMortarInSector() {
  INT16 sSectorX, sSectorY, sSectorZ;

  if (GetCurrentBattleSectorXYZ(&sSectorX, &sSectorY, &sSectorZ)) {
    if (IsThisSectorASAMSector(sSectorX, sSectorY, (INT8)sSectorZ)) return (TRUE);

    if (GetTownIdForSector(sSectorX, sSectorY) != BLANK_SECTOR) return (TRUE);
  }

  return (FALSE);
}

void InitArmyGunTypes(void) {
  /* ***28.07.2010*** закомментировано за ненадобностью
          ARMY_GUN_CHOICE_TYPE *pGunChoiceTable;
          UINT32 uiGunLevel;
          UINT32 uiChoice;
          INT8 bItemNo;
          UINT8 ubWeapon;


          // depending on selection of the gun nut option
          if (gGameOptions.fGunNut)
          {
                  // use table of extended gun choices
                  pGunChoiceTable = &(gExtendedArmyGunChoices[0]);
          }
          else
          {
                  // use table of regular gun choices
                  pGunChoiceTable = &(gRegularArmyGunChoices[0]);
          }

          // for each gun category
          for (uiGunLevel = 0; uiGunLevel <	ARMY_GUN_LEVELS; uiGunLevel++)
          {
                  // choose one the of the possible gun choices to be used by the army for this game
     & store it uiChoice = Random(pGunChoiceTable[ uiGunLevel ].ubChoices); bItemNo =
     pGunChoiceTable[ uiGunLevel ].bItemNo[ uiChoice ]; AssertMsg(bItemNo != -1, "Invalid army gun
     choice in table"); gStrategicStatus.ubStandardArmyGunIndex[uiGunLevel] = (UINT8) bItemNo;
          }

          // set all flags that track whether this weapon type has been dropped before to FALSE
          for (ubWeapon = 0; ubWeapon < MAX_WEAPONS; ubWeapon++)
          {
                  gStrategicStatus.fWeaponDroppedAlready[ubWeapon] = FALSE;
          }

          // avoid auto-drops for the gun class with the crappiest guns in it
          //***28.07.2010*** закомментировано
          ///MarkAllWeaponsOfSameGunClassAsDropped( SW38 );
  */
}

INT8 GetWeaponClass(UINT16 usGun, UINT8 ubSoldierClass) {
  UINT32 uiGunLevel, uiLoop;

  // always use the extended list since it contains all guns...
  for (uiGunLevel = 0; uiGunLevel < ARMY_GUN_LEVELS; uiGunLevel++) {
    for (uiLoop = 0; uiLoop < gExtendedArmyGunChoices[ubSoldierClass][uiGunLevel].ubChoices;
         uiLoop++) {
      if (gExtendedArmyGunChoices[ubSoldierClass][uiGunLevel].usItemNo[uiLoop] == usGun) {
        return ((INT8)uiGunLevel);
      }
    }
  }
  return (-1);
}

/**
void MarkAllWeaponsOfSameGunClassAsDropped( UINT16 usWeapon, UINT8 ubSoldierClass )
{
        INT8 bGunClass;
        UINT32 uiLoop;


        // mark that item itself as dropped, whether or not it's part of a gun class
        gStrategicStatus.fWeaponDroppedAlready[ usWeapon ] = TRUE;

        bGunClass = GetWeaponClass( usWeapon, ubSoldierClass );

        // if the gun belongs to a gun class (mortars, GLs, LAWs, etc. do not and are handled
independently) if ( bGunClass != -1 )
        {
                // then mark EVERY gun in that class as dropped
                for ( uiLoop = 0; uiLoop < gExtendedArmyGunChoices[ubSoldierClass][ bGunClass
].ubChoices; uiLoop++ )
                {
                        gStrategicStatus.fWeaponDroppedAlready[
gExtendedArmyGunChoices[ubSoldierClass][ bGunClass ].bItemNo[ uiLoop ] ] = TRUE;
                }
        }
}
**/
//***4.11.2007***
void EquipRobot(SOLDIERCREATE_STRUCT *pp) {
  // OBJECTTYPE Object;

  if (!(pp->Inv[HANDPOS].fFlags & OBJECT_NO_OVERWRITE)) {
    if (HighestPlayerProgressPercentage() > 40 && Chance(60)) {
      if (Chance(40)) {
        CreateItem(55, (INT8)(80 + Random(21)), &(pp->Inv[HANDPOS]));
      } else {
        CreateItem(RG6, (INT8)(80 + Random(21)), &(pp->Inv[HANDPOS]));
      }
    }
  }

  if (!(pp->Inv[BIGPOCK1POS].fFlags & OBJECT_NO_OVERWRITE))
    CreateItem(428, (INT8)(80 + Random(21)), &(pp->Inv[BIGPOCK1POS]));

  /*CreateItems( 6, ( INT8 )( 80 + Random( 21 ) ), 1, &Object );
  PlaceObjectInSoldierCreateStruct( pp, &Object );

  CreateItems( 123, 100, 1, &Object );
  PlaceObjectInSoldierCreateStruct( pp, &Object );
  CreateItems( 121, 100, 1, &Object );
  PlaceObjectInSoldierCreateStruct( pp, &Object );*/

  CreateItem(632, 100, &(pp->Inv[SMALLPOCK1POS]));
  CreateItem(632, 100, &(pp->Inv[SMALLPOCK2POS]));

  CreateItem(198, 100, &(pp->Inv[VESTPOS]));
  CreateItem(198, 100, &(pp->Inv[HELMETPOS]));
  CreateItem(198, 100, &(pp->Inv[LEGPOS]));
}

// Chooses equipment based on the relative equipment level (0-4) with best being 4.  It allocates a
// range of equipment to choose from. NOTE:  I'm just winging it for the decisions on which items
// that different groups can have.  Basically,
// there are variations, so a guy at a certain level may get a better gun and worse armour or vice
// versa.
void GenerateRandomEquipment(SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass, INT8 bEquipmentRating) {
  OBJECTTYPE *pItem;
  // general rating information
  INT8 bRating = 0;
  // numbers of items
  INT8 bAmmoClips = 0;
  INT8 bGrenades = 0;
  BOOLEAN fAttachment = FALSE;
  // item levels
  INT8 bWeaponClass = 0;
  INT8 bHelmetClass = 0;
  INT8 bVestClass = 0;
  INT8 bLeggingClass = 0;
  INT8 bAttachClass = 0;
  INT8 bGrenadeClass = 0;
  INT8 bKnifeClass = 0;
  INT8 bKitClass = 0;
  INT8 bMiscClass = 0;
  INT8 bBombClass = 0;
  // special weapons
  BOOLEAN fMortar = FALSE;
  BOOLEAN fGrenadeLauncher = FALSE;
  BOOLEAN fLAW = FALSE;
  INT32 i;
  INT8 bEquipmentModifier;
  UINT8 ubMaxSpecialWeaponRoll;

  Assert(pp);

  // JZ: 06.05.2015 Новая анимация танка, 2 катеров и турели
  /*
  // kids don't get anything 'cause they don't have any weapon animations and the rest is
  inappropriate if ( ( pp->bBodyType == HATKIDCIV ) || ( pp->bBodyType == KIDCIV ) )
  {
          return;
  }

  if ( ( pp->bBodyType == TANK_NE ) || ( pp->bBodyType == TANK_NW ) )
  {
          EquipTank( pp );
          return;
  }

  //***4.11.2007*** экипировка джипов
  if ( pp->bBodyType == ROBOTNOWEAPON )
  {
          pp->bLifeMax	= 100;
          pp->bLife	  	= pp->bLifeMax;
          pp->bAgility	= 100;
          pp->bDexterity	= 100;
          pp->bStrength	= 100;
          pp->bMorale		= 100;
          pp->fOnRoof		= 0;

          EquipRobot( pp );
          return;
  }
  */
  switch (pp->bBodyType) {
    case HATKIDCIV:
    case KIDCIV:
      return;

    case TANK_NW:
    case TANK_NE:
    // Танк 1
    case TANK1_NW:
    case TANK1_NE:
    case TANK1_SW:
    case TANK1_SE:
    // Танк 2
    case TANK2_NW:
    case TANK2_NE:
    case TANK2_SW:
    case TANK2_SE:
    // Танк 3
    case TANK3_NW:
    case TANK3_NE:
    case TANK3_SW:
    case TANK3_SE:
    // Танк 4
    case TANK4_NW:
    case TANK4_NE:
    case TANK4_SW:
    case TANK4_SE:
    // Танк 5 (с ДЗ)
    case TANK5_NW:
    case TANK5_NE:
    case TANK5_SW:
    case TANK5_SE:
    // Танк 6 (с ДЗ)
    case TANK6_NW:
    case TANK6_NE:
    case TANK6_SW:
    case TANK6_SE:
    // Танк 7 (с ДЗ)
    case TANK7_NW:
    case TANK7_NE:
    case TANK7_SW:
    case TANK7_SE:
    // Танк 8 (с ДЗ)
    case TANK8_NW:
    case TANK8_NE:
    case TANK8_SW:
    case TANK8_SE:
      EquipTank(pp);
      return;

    // Катер, Большой катер, турель
    case BOAT_NW:
    case BOAT_NE:
    case BOAT_SW:
    case BOAT_SE:
      if (!(pp->Inv[HANDPOS].fFlags & OBJECT_NO_OVERWRITE)) {
        CreateItem(427, (INT8)(80 + Random(21)), &(pp->Inv[HANDPOS]));
        pp->Inv[HANDPOS].fFlags |= OBJECT_UNDROPPABLE;
      }
      CreateItem(634, 100, &(pp->Inv[SMALLPOCK1POS]));
      pp->Inv[SMALLPOCK1POS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(634, 100, &(pp->Inv[SMALLPOCK2POS]));
      pp->Inv[SMALLPOCK2POS].fFlags |= OBJECT_UNDROPPABLE;

      CreateItem(186, 100, &(pp->Inv[VESTPOS]));
      pp->Inv[VESTPOS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(186, 100, &(pp->Inv[HELMETPOS]));
      pp->Inv[HELMETPOS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(186, 100, &(pp->Inv[LEGPOS]));
      pp->Inv[LEGPOS].fFlags |= OBJECT_UNDROPPABLE;
      return;

    // Большой катер
    case BIG_BOAT_NW:
    case BIG_BOAT_NE:
    case BIG_BOAT_SW:
    case BIG_BOAT_SE:
      if (!(pp->Inv[HANDPOS].fFlags & OBJECT_NO_OVERWRITE)) {
        CreateItem(61, (INT8)(80 + Random(21)), &(pp->Inv[HANDPOS]));
        // CreateItem( 428, ( INT8 )( 80 + Random( 21 ) ), &( pp->Inv[ HANDPOS ] ) );
        pp->Inv[HANDPOS].fFlags |= OBJECT_UNDROPPABLE;
      }

      // CreateItem( 911, 100, &(pp->Inv[ BIGPOCK1POS ]) );
      // pp->Inv[ BIGPOCK1POS ].fFlags |= OBJECT_UNDROPPABLE;
      // CreateItem( 632, 100, &(pp->Inv[ SMALLPOCK1POS ]) );
      // pp->Inv[ SMALLPOCK1POS ].fFlags |= OBJECT_UNDROPPABLE;
      // CreateItem( 632, 100, &(pp->Inv[ SMALLPOCK2POS ]) );
      // pp->Inv[ SMALLPOCK2POS ].fFlags |= OBJECT_UNDROPPABLE;

      CreateItem(198, 100, &(pp->Inv[VESTPOS]));
      pp->Inv[VESTPOS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(198, 100, &(pp->Inv[HELMETPOS]));
      pp->Inv[HELMETPOS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(198, 100, &(pp->Inv[LEGPOS]));
      pp->Inv[LEGPOS].fFlags |= OBJECT_UNDROPPABLE;
      return;

    case TURRET:
      if (!(pp->Inv[HANDPOS].fFlags & OBJECT_NO_OVERWRITE)) {
        CreateItem(428, (INT8)(80 + Random(21)), &(pp->Inv[HANDPOS]));
        pp->Inv[HANDPOS].fFlags |= OBJECT_UNDROPPABLE;
      }
      // CreateItem( 62, 100, &(pp->Inv[ BIGPOCK1POS ]) );
      // pp->Inv[ BIGPOCK1POS ].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(632, 100, &(pp->Inv[SMALLPOCK1POS]));
      pp->Inv[SMALLPOCK1POS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(632, 100, &(pp->Inv[SMALLPOCK2POS]));
      pp->Inv[SMALLPOCK2POS].fFlags |= OBJECT_UNDROPPABLE;

      CreateItem(186, 100, &(pp->Inv[VESTPOS]));
      pp->Inv[VESTPOS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(186, 100, &(pp->Inv[HELMETPOS]));
      pp->Inv[HELMETPOS].fFlags |= OBJECT_UNDROPPABLE;
      CreateItem(186, 100, &(pp->Inv[LEGPOS]));
      pp->Inv[LEGPOS].fFlags |= OBJECT_UNDROPPABLE;
      return;

    case ROBOTNOWEAPON:
      pp->bLifeMax = 100;
      pp->bLife = pp->bLifeMax;
      pp->bAgility = 100;
      pp->bDexterity = 100;
      pp->bStrength = 100;
      pp->bMorale = 100;
      pp->fOnRoof = 0;
      EquipRobot(pp);
      return;

    //***07.06.2016***
    case APC1_1:
    case APC1_2:
    case APC1_3:
    case APC1_4:

    case APC2_1:
    case APC2_2:
    case APC2_3:
    case APC2_4:
      pp->bLifeMax = 100;
      pp->bLife = pp->bLifeMax;
      pp->bAgility = 100;
      pp->bDexterity = 100;
      pp->bStrength = 100;
      pp->bMorale = 100;
      pp->fOnRoof = 0;
      EquipRobot(pp);
      return;
  }
  ///

  Assert((bSoldierClass >= SOLDIER_CLASS_NONE) && (bSoldierClass <= SOLDIER_CLASS_ELITE_MILITIA));
  Assert((bEquipmentRating >= 0) && (bEquipmentRating <= 4));

  // equipment level is modified by 1/10 of the difficulty percentage, -5, so it's between -5 to +5
  // (on normal, this is actually -4 to +4, easy is -5 to +3, and hard is -3 to +5)
  bEquipmentModifier = bEquipmentRating + ((CalcDifficultyModifier(bSoldierClass) / 10) - 5);

  switch (bSoldierClass) {
    case SOLDIER_CLASS_NONE:
      // ammo is here only so that civilians with pre-assigned ammo will get some clips for it!
      bAmmoClips = (INT8)(1 + Random(2));

      // civilians get nothing, anyone who should get something should be preassigned by Linda
      break;

    case SOLDIER_CLASS_ADMINISTRATOR:
    case SOLDIER_CLASS_GREEN_MILITIA:
      bRating = BAD_ADMINISTRATOR_EQUIPMENT_RATING + bEquipmentModifier;
      bRating = (INT8)max(MIN_EQUIPMENT_CLASS, min(MAX_EQUIPMENT_CLASS, bRating));

      bWeaponClass = bRating;

      // Note:  in some cases the class of armour and/or helmet won't be high enough to make
      //			 the lowest level.
      bVestClass = bRating;
      bHelmetClass = bRating;
      // no leggings

      ///			if( Random( 2 ) )
      bKnifeClass = bRating;

      bAmmoClips = (INT8)(1 + Random(2));

      bGrenades = (INT8)(1 + Random(2));  //***22.11.2014***

      if (bRating >= GOOD_ADMINISTRATOR_EQUIPMENT_RATING) {
        bAmmoClips++;

        bKitClass = bRating;
        bMiscClass = bRating;
      }

      /**			if( bRating >= GREAT_ADMINISTRATOR_EQUIPMENT_RATING )
                              {
                                      bGrenades = 1, bGrenadeClass = bRating;
                              }**/

      if ((bRating > MIN_EQUIPMENT_CLASS) &&
          bRating < MAX_EQUIPMENT_CLASS) {  // Randomly decide if there is to be an upgrade of guns
                                            // vs armour (one goes up, the other down)
        switch (Random(5)) {
          case 0:
            bWeaponClass++, bVestClass--;
            break;  // better gun, worse armour
          case 1:
            bWeaponClass--, bVestClass++;
            break;  // worse gun, better armour
        }
      }
      break;

    case SOLDIER_CLASS_ARMY:
    case SOLDIER_CLASS_REG_MILITIA:
      // army guys tend to have a broad range of equipment
      bRating = BAD_ARMY_EQUIPMENT_RATING + bEquipmentModifier;
      bRating = (INT8)max(MIN_EQUIPMENT_CLASS, min(MAX_EQUIPMENT_CLASS, bRating));

      bWeaponClass = bRating;
      bVestClass = bRating;
      bHelmetClass = bRating;
      bGrenadeClass = bRating;

      if ((bRating >= GOOD_ARMY_EQUIPMENT_RATING) && (Random(100) < 33)) {
        fAttachment = TRUE;
        bAttachClass = bRating;
      }

      bAmmoClips = (INT8)(2 + Random(2));

      bGrenades = (INT8)(2 + Random(2));  //***22.11.2014***

      if (bRating >= AVERAGE_ARMY_EQUIPMENT_RATING) {
        ///				bGrenades = (INT8)Random( 2 );
        bKitClass = bRating;
        bMiscClass = bRating;
      }

      if (bRating >= GOOD_ARMY_EQUIPMENT_RATING) {
        bGrenades++;
      }

      if (bRating >= GREAT_ARMY_EQUIPMENT_RATING) {
        bGrenades++;

        bLeggingClass = bRating;

        if (Chance(25)) {
          bBombClass = bRating;
        }
      }

      if (Random(2)) bKnifeClass = bRating;

      if ((bRating > MIN_EQUIPMENT_CLASS) && bRating < MAX_EQUIPMENT_CLASS) {
        switch (Random(7)) {
          case 3:
            bWeaponClass++, bVestClass--;
            break;  // better gun, worse armour
          case 4:
            bWeaponClass--, bVestClass++;
            break;  // worse gun, better armour
          case 5:
            bVestClass++, bHelmetClass--;
            break;  // better armour, worse helmet
          case 6:
            bVestClass--, bHelmetClass++;
            break;  // worse armour, better helmet
        }
      }

      // if well-enough equipped, 1/5 chance of something really special
      if ((bRating >= GREAT_ARMY_EQUIPMENT_RATING) && (Random(100) < 20)) {
        // give this man a special weapon!  No mortars if underground, however
        ubMaxSpecialWeaponRoll = (!IsAutoResolveActive() && (gbWorldSectorZ != 0)) ? 6 : 7;
        switch (Random(ubMaxSpecialWeaponRoll)) {
          case 0:
          case 1:
          case 2:
            if (pp->bExpLevel >= 3) {
              // grenade launcher
              fGrenadeLauncher = TRUE;
              bGrenades = 3 + (INT8)(Random(3));  // 3-5
            }
            break;

          case 3:
          case 4:
          case 5:
            if (pp->bExpLevel >= 4) {
              // LAW rocket launcher
              fLAW = TRUE;
            }
            break;

          case 6:
            // one per team maximum!
            if ((pp->bExpLevel >= 5) && (guiMortarsRolledByTeam < MAX_MORTARS_PER_TEAM) &&
                AllowMortarInSector())  //***28.10.2013***
            {
              // mortar
              fMortar = TRUE;
              guiMortarsRolledByTeam++;

              // the grenades will actually represent mortar shells in this case
              bGrenades = 2 + (INT8)(Random(3));  // 2-4
              bGrenadeClass = MORTAR_GRENADE_CLASS;
            }
            break;
        }
      }
      break;

    case SOLDIER_CLASS_ELITE:
    case SOLDIER_CLASS_ELITE_MILITIA:
      bRating = BAD_ELITE_EQUIPMENT_RATING + bEquipmentModifier;
      bRating = (INT8)max(MIN_EQUIPMENT_CLASS, min(MAX_EQUIPMENT_CLASS, bRating));

      bWeaponClass = bRating;
      bHelmetClass = bRating;
      bVestClass = bRating;
      bLeggingClass = bRating;
      bAttachClass = bRating;
      bGrenadeClass = bRating;
      bKitClass = bRating;
      bMiscClass = bRating;

      if (Chance(25)) {
        bBombClass = bRating;
      }

      bAmmoClips = (INT8)(3 + Random(2));
      bGrenades = (INT8)(4 + Random(3));  //***22.11.2014*** +3

      if ((bRating >= AVERAGE_ELITE_EQUIPMENT_RATING) && (Random(100) < 75)) {
        fAttachment = TRUE;
        bAttachClass = bRating;
      }

      if (Random(2)) bKnifeClass = bRating;

      if ((bRating > MIN_EQUIPMENT_CLASS) && bRating < MAX_EQUIPMENT_CLASS) {
        switch (Random(11)) {
          case 4:
            bWeaponClass++, bVestClass--;
            break;
          case 5:
            bWeaponClass--, bVestClass--;
            break;
          case 6:
            bVestClass++, bHelmetClass--;
            break;
          case 7:
            bGrenades += 2;
            break;
          case 8:
            bHelmetClass++;
            break;
          case 9:
            bVestClass++;
            break;
          case 10:
            bWeaponClass++;
            break;
        }
      }

      // if well-enough equipped, 1/3 chance of something really special
      if ((bRating >= GOOD_ELITE_EQUIPMENT_RATING) && (Random(100) < 33)) {
        // give this man a special weapon!  No mortars if underground, however
        ubMaxSpecialWeaponRoll = (!IsAutoResolveActive() && (gbWorldSectorZ != 0)) ? 6 : 7;
        switch (Random(ubMaxSpecialWeaponRoll)) {
          case 0:
          case 1:
          case 2:
            // grenade launcher
            fGrenadeLauncher = TRUE;
            bGrenades = 4 + (INT8)(Random(4));  // 4-7
            break;
          case 3:
          case 4:
          case 5:
            // LAW rocket launcher
            fLAW = TRUE;
            break;
          case 6:
            // one per team maximum!
            if (guiMortarsRolledByTeam < MAX_MORTARS_PER_TEAM &&
                AllowMortarInSector())  //***28.10.2013***
            {
              // mortar
              fMortar = TRUE;
              guiMortarsRolledByTeam++;

              // the grenades will actually represent mortar shells in this case
              bGrenades = 3 + (INT8)(Random(5));  // 3-7
              bGrenadeClass = MORTAR_GRENADE_CLASS;
            }
            break;
        }
      }
      break;
  }

  for (i = 0; i < NUM_INV_SLOTS; i++) {  // clear items, but only if they have write status.
    if (!(pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)) {
      memset(&(pp->Inv[i]), 0, sizeof(OBJECTTYPE));
      pp->Inv[i].fFlags |= OBJECT_UNDROPPABLE;
    } else {  // check to see what kind of item is here.  If we find a gun, for example, it'll make
              // the
      // bWeaponClass negative to signify that a gun has already been specified, and later
      // code will use that to determine that and to pick ammo for it.
      pItem = &pp->Inv[i];
      if (!pItem) continue;
      switch (Item[pItem->usItem].usItemClass) {
        case IC_GUN:
          if (!IsRocketLauncher(pItem->usItem) /*pItem->usItem != ROCKET_LAUNCHER*/) {
            bWeaponClass *= -1;
          } else  // rocket launcher!
          {
            fLAW = FALSE;
          }
          break;
        case IC_AMMO:
          bAmmoClips = 0;
          break;
        case IC_BLADE:
        case IC_THROWING_KNIFE:
          bKnifeClass = 0;
          break;
        case IC_LAUNCHER:
          fGrenadeLauncher = FALSE;
          fMortar = FALSE;
          break;
        case IC_ARMOUR:
          if (i == HELMETPOS)
            bHelmetClass = 0;
          else if (i == VESTPOS)
            bVestClass = 0;
          else if (i == LEGPOS)
            bLeggingClass = 0;
          break;
        case IC_GRENADE:
          bGrenades = 0;
          bGrenadeClass = 0;
          break;
        case IC_MEDKIT:
        case IC_KIT:
          bKitClass = 0;
          break;
        case IC_MISC:
          bMiscClass = 0;
        case IC_BOMB:
          bBombClass = 0;
          break;
      }
    }
  }

  // special: militia shouldn't drop bombs
  if (!(SOLDIER_CLASS_ENEMY(bSoldierClass))) {
    bBombClass = 0;
  }

  // Now actually choose the equipment!
  ChooseWeaponForSoldierCreateStruct(pp, bWeaponClass, bAmmoClips, bAttachClass, fAttachment);
  ChooseGrenadesForSoldierCreateStruct(pp, bGrenades, bGrenadeClass, fGrenadeLauncher);
  ChooseArmourForSoldierCreateStruct(pp, bHelmetClass, bVestClass, bLeggingClass);
  ChooseSpecialWeaponsForSoldierCreateStruct(pp, bKnifeClass, fGrenadeLauncher, fLAW, fMortar);
  ChooseFaceGearForSoldierCreateStruct(pp);
  ChooseKitsForSoldierCreateStruct(pp, bKitClass);
  ChooseMiscGearForSoldierCreateStruct(pp, bMiscClass);
  ChooseBombsForSoldierCreateStruct(pp, bBombClass);
  ChooseLocationSpecificGearForSoldierCreateStruct(pp);
  RandomlyChooseWhichItemsAreDroppable(pp, bSoldierClass);
}

//***29.07.2013*** добор патронов до необходимого минимума
void RecalcAmmoClips(INT8 *bAmmoClips, UINT16 usAmmoIndex) {
  while ((*bAmmoClips) * Magazine[Item[usAmmoIndex].ubClassIndex].ubMagSize < 30) {
    (*bAmmoClips)++;

    if (*bAmmoClips >= MAX_OBJECTS_PER_SLOT) break;
  }
}

// When using the class values, they should all range from 0-11, 0 meaning that there will be no
// selection for that particular type of item, and 1-11 means to choose an item if possible.  1 is
// the worst class of item, while 11 is the best.

void ChooseWeaponForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bWeaponClass,
                                        INT8 bAmmoClips, INT8 bAttachClass, BOOLEAN fAttachment) {
  INVTYPE *pItem;
  OBJECTTYPE Object;
  UINT16 i;
  UINT16 usRandom;
  UINT16 usNumMatches = 0;
  UINT16 usGunIndex = 0;
  UINT16 usAmmoIndex = 0;
  UINT16 usAttachIndex = 0;
  UINT8 ubChanceStandardAmmo;
  INT8 bStatus;

  // Choose weapon:
  // WEAPONS are very important, and are therefore handled differently using special pre-generated
  // tables. It was requested that enemies use only a small subset of guns with a lot duplication of
  // the same gun type.

  // if gun was pre-selected (rcvd negative weapon class) and needs ammo
  if (bWeaponClass < 0 &&
      bAmmoClips) {  // Linda has added a specific gun to the merc's inventory, but no ammo.  So, we
    // will choose ammunition that works with the gun.
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      if (Item[pp->Inv[i].usItem].usItemClass == IC_GUN) {
        usGunIndex = pp->Inv[i].usItem;
        ubChanceStandardAmmo = 100 - (bWeaponClass * -9);  // weapon class is negative!
        usAmmoIndex = RandomMagazine(usGunIndex, ubChanceStandardAmmo);

        //***19.10.2007***
        /*if ( usGunIndex == ROCKET_RIFLE )
        {
                pp->Inv[ i ].ubImprintID = (NO_PROFILE + 1);
        }*/

        break;
      }
    }
    if (bAmmoClips && usAmmoIndex) {
      RecalcAmmoClips(&bAmmoClips, usAmmoIndex);  //***29.07.2013***

      CreateItems(usAmmoIndex, 100, bAmmoClips, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }

    return;
  }

  if (bWeaponClass < 1) return;  // empty handed / pre-selected

  // reduce anything over 11 to 11
  if (bWeaponClass > ARMY_GUN_LEVELS) {
    bWeaponClass = ARMY_GUN_LEVELS;
  }

  // the weapon class here ranges from 1 to 11, so subtract 1 to get a gun level
  usGunIndex = SelectStandardArmyGun((UINT8)(bWeaponClass - 1), pp->ubSoldierClass);

  if (bAmmoClips) {  // We have a gun, so choose ammo clips

    // check default ammo first...
    usAmmoIndex = DefaultMagazine(usGunIndex);
    /**		switch( Magazine[ Item[ usAmmoIndex ].ubClassIndex ].ubAmmoType )
                    {
                            case AMMO_AP:
                            case AMMO_SUPER_AP:
                                    // assault rifle, rocket rifle (etc) - high chance of having AP
       ammo ubChanceStandardAmmo = 80; break; default: ubChanceStandardAmmo = 100 - (bWeaponClass *
       9); break;
                    }
                    usAmmoIndex = RandomMagazine( usGunIndex, ubChanceStandardAmmo );**/
  }

  // Choose attachment
  if (bAttachClass && fAttachment) {
    usNumMatches = 0;
    while (bAttachClass && !usNumMatches) {  // Count the number of valid attachments.
      for (i = 0; i < MAXITEMS; i++) {
        pItem = &Item[i];
        if (pItem->usItemClass == IC_MISC && pItem->ubCoolness == bAttachClass &&
            ValidAttachment(i, usGunIndex))
          usNumMatches++;
      }
      if (!usNumMatches) {
        bAttachClass--;
      }
    }
    if (usNumMatches) {
      usRandom = (UINT16)Random(usNumMatches);
      for (i = 0; i < MAXITEMS; i++) {
        pItem = &Item[i];
        if (pItem->usItemClass == IC_MISC && pItem->ubCoolness == bAttachClass &&
            ValidAttachment(i, usGunIndex)) {
          if (usRandom)
            usRandom--;
          else {
            usAttachIndex = i;
            break;
          }
        }
      }
    }
  }
  // Now, we have chosen all of the correct items.  Now, we will assign them into the slots.
  // Because we are dealing with enemies, automatically give them full ammo in their weapon.
  if (!(pp->Inv[HANDPOS].fFlags & OBJECT_NO_OVERWRITE)) {
    switch (pp->ubSoldierClass) {
      case SOLDIER_CLASS_ADMINISTRATOR:
      case SOLDIER_CLASS_ARMY:
      case SOLDIER_CLASS_GREEN_MILITIA:
      case SOLDIER_CLASS_REG_MILITIA:
        // Admins/Troops: 60-75% + 1% every 4% progress
        bStatus = (INT8)(60 + Random(16));
        bStatus += (INT8)(HighestPlayerProgressPercentage() / 4);
        bStatus = (INT8)min(100, bStatus);
        break;
      case SOLDIER_CLASS_ELITE:
      case SOLDIER_CLASS_ELITE_MILITIA:
        // 85-90% +  1% every 10% progress
        bStatus = (INT8)(85 + Random(6));
        bStatus += (INT8)(HighestPlayerProgressPercentage() / 10);
        bStatus = (INT8)min(100, bStatus);
        break;
      default:
        bStatus = (INT8)(50 + Random(51));
        break;
    }
    // don't allow it to be lower than marksmanship, we don't want it to affect their chances of
    // hitting
    bStatus = (INT8)max(pp->bMarksmanship, bStatus);

    CreateItem(usGunIndex, bStatus, &(pp->Inv[HANDPOS]));
    pp->Inv[HANDPOS].fFlags |= OBJECT_UNDROPPABLE;

    // Rocket Rifles must come pre-imprinted, in case carrier gets killed without getting a shot off
    //***19.10.2007***
    /*if ( usGunIndex == ROCKET_RIFLE )
    {
            pp->Inv[ HANDPOS ].ubImprintID = (NO_PROFILE + 1);
    }*/
  } else {  // slot locked, so don't add any attachments to it!
    usAttachIndex = 0;
  }
  // Ammo
  if (bAmmoClips && usAmmoIndex) {
    RecalcAmmoClips(&bAmmoClips, usAmmoIndex);  //***29.07.2013***

    CreateItems(usAmmoIndex, 100, bAmmoClips, &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
  }
  if (usAttachIndex) {
    if (usAttachIndex == GUN_BARREL_EXTENDER && gubEnvLightValue < LIGHT_DUSK_CUTOFF)
      usAttachIndex = SNIPERSCOPE;  //***21.11.2014*** замена днём НП на оптику

    CreateItem(usAttachIndex, 100, &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    AttachObject(NULL, &(pp->Inv[HANDPOS]), &Object);
  }
}

/*
void ChooseGrenadesForSoldierCreateStruct_old( SOLDIERCREATE_STRUCT *pp, INT8 bGrenades, INT8
bGrenadeClass, BOOLEAN fGrenadeLauncher )
{
        OBJECTTYPE Object;
        INT16 sNumPoints;
        UINT16 usItem;
        UINT8 ubBaseQuality;
        UINT8 ubQualityVariation;
        //numbers of each type the player will get!
        UINT8 ubNumStun = 0;
        UINT8 ubNumTear = 0;
        UINT8 ubNumMustard = 0;
        UINT8 ubNumMini = 0;
        UINT8 ubNumReg = 0;
        UINT8 ubNumSmoke = 0;
        UINT8 ubNumFlare = 0;

        //determine how many *points* the enemy will get to spend on grenades...
        sNumPoints = bGrenades * bGrenadeClass;

        //no points, no grenades!
        if( !sNumPoints )
                return;

        // special mortar shell handling
        if (bGrenadeClass == MORTAR_GRENADE_CLASS)
        {
                CreateItems( MORTAR_SHELL, (INT8) (80 + Random(21)), bGrenades, &Object );
                Object.fFlags |= OBJECT_UNDROPPABLE;
                PlaceObjectInSoldierCreateStruct( pp, &Object );
                return;
        }

        Assert( bGrenadeClass <= 11 );

        //determine the quality of grenades.  The elite guys get the best quality, while the others
        //get progressively worse.
        ubBaseQuality = (UINT8)min( 45 + bGrenadeClass * 5, 90 );
        ubQualityVariation = 101 - ubBaseQuality;


        //now, purchase the grenades.
        while( sNumPoints > 0 )
        {
                if( sNumPoints >= 20 )
                { //Choose randomly between mustard and regular
                        if( Random( 2 ) && !fGrenadeLauncher )
                                ubNumMustard++, sNumPoints -= 10;
                        else
                                ubNumReg++, sNumPoints -= 9;
                }

                if( sNumPoints >= 10 )
                { //Choose randomly between any
                        switch( Random( 7 ) )
                        {
                                case 0:	if ( !fGrenadeLauncher )
                                                                {
                                                                        ubNumMustard++;
sNumPoints -= 10;	break;
                                                                }
                                                                // if grenade launcher, pick regular
instead case 1: ubNumReg++;				sNumPoints -= 9;		break; case
2: if ( !fGrenadeLauncher )
                                                                {
                                                                        ubNumMini++;
sNumPoints -= 7;		break;
                                                                }
                                                                // if grenade launcher, pick tear
instead case 3: ubNumTear++;			sNumPoints -= 6;		break; case 4:
ubNumStun++;			sNumPoints -= 5;		break; case 5: ubNumSmoke++;
sNumPoints -= 4;		break; case 6: if (!fGrenadeLauncher )
                                                {
                                                        ubNumFlare++; sNumPoints -= 3;
                                                }
                                                break;
                        }
                }

                // JA2 Gold: don't make mini-grenades take all points available, and add chance of
break lights if( sNumPoints >= 1 && sNumPoints < 10 )
                {
                        switch( Random( 10 ) )
                        {
                                case 0:
                                case 1:
                                case 2:
                                        ubNumSmoke++;
                                        sNumPoints -= 4;
                                        break;
                                case 3:
                                case 4:
                                        ubNumTear++;
                                        sNumPoints -= 6;
                                        break;
                                case 5:
                                case 6:
                                        if (!fGrenadeLauncher)
                                        {
                                                ubNumFlare++;
                                                sNumPoints -= 3;
                                        }
                                        break;
                                case 7:
                                case 8:
                                        ubNumStun++;
                                        sNumPoints -= 5;
                                        break;
                                case 9:
                                        if (!fGrenadeLauncher)
                                        {
                                                ubNumMini++;
                                                sNumPoints -= 7;
                                        }
                                        break;
                        }
                }

        }


        //Create the grenades and add them to the soldier

        if( ubNumSmoke )
        {//***23.11.2007*** не меняем гранаты
                if ( fGrenadeLauncher )
                {
                        usItem = GL_SMOKE_GRENADE;
                }
                else
                {
                        usItem = SMOKE_GRENADE;
                }
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )),
ubNumSmoke, &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp,
&Object );
        }
        if( ubNumTear )
        {
                if ( fGrenadeLauncher )
                {
                        usItem = GL_TEARGAS_GRENADE;
                }
                else
                {
                        usItem = TEARGAS_GRENADE;
                }
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )),
ubNumTear, &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp,
&Object );
        }
        if( ubNumStun )
        {
                if ( fGrenadeLauncher )
                {
                        usItem = GL_STUN_GRENADE;
                }
                else
                {
                        usItem = STUN_GRENADE;
                }
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )),
ubNumStun, &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp,
&Object );
        }
        if( ubNumReg )
        {
                if ( fGrenadeLauncher )
                {
                        usItem = GL_HE_GRENADE;
                }
                else
                {
                        //***11.01.2014***
                        switch( pp->ubSoldierClass )
                        {
                                case SOLDIER_CLASS_ARMY:
                                        usItem = 681;
                                        break;
                                case SOLDIER_CLASS_ELITE:
                                        usItem = 682;
                                        break;
                                case SOLDIER_CLASS_REG_MILITIA:
                                case SOLDIER_CLASS_ELITE_MILITIA:
                                        usItem = HAND_GRENADE;
                                        break;
                        }///
                }
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )), ubNumReg,
&Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp, &Object );
        }

        if( ubNumMini )
        {
                //***11.01.2014***
                switch( pp->ubSoldierClass )
                {
                        case SOLDIER_CLASS_ARMY:
                                usItem = 680;
                                break;
                        case SOLDIER_CLASS_ELITE:
                                usItem = (Chance(50) ? 679 : 683);
                                break;
                        case SOLDIER_CLASS_REG_MILITIA:
                        case SOLDIER_CLASS_ELITE_MILITIA:
                                usItem = MINI_GRENADE;
                                break;
                }///
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )),
ubNumMini, &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp,
&Object );
        }
        if( ubNumMustard )
        {
                if ( fGrenadeLauncher )
                {
                        usItem = GL_MUSTARD_GRENADE;
                }
                else
                {
                        usItem = MUSTARD_GRENADE;
                }
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )),
ubNumMustard, &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp,
&Object );
        }
        if( ubNumFlare )
        {
                if ( fGrenadeLauncher )
                {
                        usItem = GL_LIGHT_GRENADE;
                }
                else
                {
                        usItem = BREAK_LIGHT;
                }
                CreateItems( usItem, (INT8)(ubBaseQuality + Random( ubQualityVariation )),
ubNumFlare, &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; PlaceObjectInSoldierCreateStruct( pp,
&Object );
        }

}
*/
/*
void ChooseArmourForSoldierCreateStruct_old( SOLDIERCREATE_STRUCT *pp, INT8 bHelmetClass, INT8
bVestClass, INT8 bLeggingsClass )
{
        UINT16 i;
        INVTYPE *pItem;
        UINT16 usRandom;
        UINT16 usNumMatches;
        INT8 bOrigVestClass = bVestClass;
        OBJECTTYPE Object;

        //Choose helmet
        if( bHelmetClass )
        {
                usNumMatches = 0;
                while( bHelmetClass && !usNumMatches )
                { //First step is to count the number of helmets in the helmet class range.  If we
                        //don't find one, we keep lowering the class until we do.
                        for( i = 0; i < MAXITEMS; i++ )
                        {
                                pItem = &Item[ i ];
                                // NOTE: This relies on treated armor to have a coolness of 0 in
order for enemies not to be equipped with it if( pItem->usItemClass == IC_ARMOUR &&
pItem->ubCoolness == bHelmetClass )
                                {
                                        if( Armour[ pItem->ubClassIndex ].ubArmourClass ==
ARMOURCLASS_HELMET ) usNumMatches++;
                                }
                        }
                        if( !usNumMatches )
                                bHelmetClass--;
                }
                if( usNumMatches )
                { //There is a helmet that we can choose.
                        usRandom = (UINT16)Random( usNumMatches );
                        for( i = 0; i < MAXITEMS; i++ )
                        {
                                pItem = &Item[ i ];
                                if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness ==
bHelmetClass )
                                {
                                        if( Armour[ pItem->ubClassIndex ].ubArmourClass ==
ARMOURCLASS_HELMET )
                                        {
                                                if( usRandom )
                                                        usRandom--;
                                                else
                                                {
                                                        if( !(pp->Inv[ HELMETPOS ].fFlags &
OBJECT_NO_OVERWRITE) )
                                                        {
                                                                CreateItem( i,
(INT8)(70+Random(31)), &(pp->Inv[ HELMETPOS ]) ); pp->Inv[ HELMETPOS ].fFlags |= OBJECT_UNDROPPABLE;
                                                        }
                                                        break;
                                                }
                                        }
                                }
                        }
                }
        }
        //Choose vest
        if( bVestClass )
        {
                usNumMatches = 0;
                while( bVestClass && !usNumMatches )
                { //First step is to count the number of armours in the armour class range.  If we
                        //don't find one, we keep lowering the class until we do.
                        for( i = 0; i < MAXITEMS; i++ )
                        {
                                // these 3 have a non-zero coolness, and would otherwise be
selected, so skip them if ( ( i == TSHIRT ) || ( i == LEATHER_JACKET ) || ( i == TSHIRT_DEIDRANNA )
) continue;

                                pItem = &Item[ i ];
                                // NOTE: This relies on treated armor to have a coolness of 0 in
order for enemies not to be equipped with it if( pItem->usItemClass == IC_ARMOUR &&
pItem->ubCoolness == bVestClass )
                                {
                                        if( Armour[ pItem->ubClassIndex ].ubArmourClass ==
ARMOURCLASS_VEST ) usNumMatches++;
                                }
                        }
                        if( !usNumMatches )
                                bVestClass--;
                }
                if( usNumMatches )
                { //There is an armour that we can choose.
                        usRandom = (UINT16)Random( usNumMatches );
                        for( i = 0; i < MAXITEMS; i++ )
                        {
                                // these 3 have a non-zero coolness, and would otherwise be
selected, so skip them if ( ( i == TSHIRT ) || ( i == LEATHER_JACKET ) || ( i == TSHIRT_DEIDRANNA )
) continue;

                                pItem = &Item[ i ];
                                if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness ==
bVestClass )
                                {
                                        if( Armour[ pItem->ubClassIndex ].ubArmourClass ==
ARMOURCLASS_VEST )
                                        {
                                                if( usRandom )
                                                        usRandom--;
                                                else
                                                {
                                                        if( !(pp->Inv[ VESTPOS ].fFlags &
OBJECT_NO_OVERWRITE) )
                                                        {
                                                                CreateItem( i,
(INT8)(70+Random(31)), &(pp->Inv[ VESTPOS ]) ); pp->Inv[ VESTPOS ].fFlags |= OBJECT_UNDROPPABLE;

                                                                if ( ( i == KEVLAR_VEST ) || ( i ==
SPECTRA_VEST ) )
                                                                {
                                                                        // roll to see if he gets a
CERAMIC PLATES, too.  Higher chance the higher his entitled vest class is if (( INT8 ) Random( 100 )
< ( 15 * ( bOrigVestClass - pItem->ubCoolness ) ) )
                                                                        {
                                                                                CreateItem(
CERAMIC_PLATES, (INT8)(70+Random(31)), &Object ); Object.fFlags |= OBJECT_UNDROPPABLE; AttachObject(
NULL, &(pp->Inv[ VESTPOS ]), &Object );
                                                                        }
                                                                }
                                                        }
                                                        break;
                                                }
                                        }
                                }
                        }
                }
        }
        //Choose Leggings
        if( bLeggingsClass )
        {
                usNumMatches = 0;
                while( bLeggingsClass && !usNumMatches )
                { //First step is to count the number of Armours in the Armour class range.  If we
                        //don't find one, we keep lowering the class until we do.
                        for( i = 0; i < MAXITEMS; i++ )
                        {
                                pItem = &Item[ i ];
                                // NOTE: This relies on treated armor to have a coolness of 0 in
order for enemies not to be equipped with it if( pItem->usItemClass == IC_ARMOUR &&
pItem->ubCoolness == bLeggingsClass )
                                {
                                        if( Armour[ pItem->ubClassIndex ].ubArmourClass ==
ARMOURCLASS_LEGGINGS ) usNumMatches++;
                                }
                        }
                        if( !usNumMatches )
                                bLeggingsClass--;
                }
                if( usNumMatches )
                { //There is a legging that we can choose.
                        usRandom = (UINT16)Random( usNumMatches );
                        for( i = 0; i < MAXITEMS; i++ )
                        {
                                pItem = &Item[ i ];
                                if( pItem->usItemClass == IC_ARMOUR && pItem->ubCoolness ==
bLeggingsClass )
                                {
                                        if( Armour[ pItem->ubClassIndex ].ubArmourClass ==
ARMOURCLASS_LEGGINGS )
                                        {
                                                if( usRandom )
                                                        usRandom--;
                                                else
                                                {
                                                        if( !(pp->Inv[ LEGPOS ].fFlags &
OBJECT_NO_OVERWRITE) )
                                                        {
                                                                CreateItem( i,
(INT8)(70+Random(31)), &(pp->Inv[ LEGPOS ]) ); pp->Inv[ LEGPOS ].fFlags |= OBJECT_UNDROPPABLE;
                                                                break;
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}
*/
void ChooseSpecialWeaponsForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bKnifeClass,
                                                BOOLEAN fGrenadeLauncher, BOOLEAN fLAW,
                                                BOOLEAN fMortar) {
  UINT16 i;
  INVTYPE *pItem;
  UINT16 usRandom;
  UINT16 usNumMatches = 0;
  UINT16 usKnifeIndex = 0;
  OBJECTTYPE Object;

  // Choose knife
  while (bKnifeClass && !usNumMatches) {  // First step is to count the number of weapons in the
                                          // weapon class range.  If we
    // don't find one, we keep lowering the class until we do.
    for (i = 0; i < MAXITEMS; i++) {
      pItem = &Item[i];
      if ((pItem->usItemClass == IC_BLADE || pItem->usItemClass == IC_THROWING_KNIFE) &&
          pItem->ubCoolness == bKnifeClass) {
        usNumMatches++;
      }
    }
    if (!usNumMatches) bKnifeClass--;
  }
  if (usNumMatches) {  // There is a knife that we can choose.
    usRandom = (UINT16)Random(usNumMatches);
    for (i = 0; i < MAXITEMS; i++) {
      pItem = &Item[i];
      if ((pItem->usItemClass == IC_BLADE || pItem->usItemClass == IC_THROWING_KNIFE) &&
          pItem->ubCoolness == bKnifeClass) {
        if (usRandom)
          usRandom--;
        else {
          usKnifeIndex = i;
          break;
        }
      }
    }
  }

  if (usKnifeIndex) {
    CreateItem(usKnifeIndex, (INT8)(70 + Random(31)), &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
  }

  if (fGrenadeLauncher) {
    // give grenade launcher
    CreateItem(GLAUNCHER, (INT8)(50 + Random(51)), &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
    //***23.11.2007*** заряд для гранатомёта
    CreateItem(GL_HE_GRENADE, (INT8)(80 + Random(21)), &Object);
    PlaceObjectInSoldierCreateStruct(pp, &Object);
  }

  if (fLAW) {
    // give rocket launcher
    CreateItem(ROCKET_LAUNCHER, (INT8)(50 + Random(51)), &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
    //***23.11.2007*** заряд для РПГ
    /*CreateItem( C1, (INT8)(80 + Random( 21 )), &Object );
    PlaceObjectInSoldierCreateStruct( pp, &Object );*/
  }

  if (fMortar) {
    // make sure we're not distributing them underground!
    Assert(IsAutoResolveActive() || (gbWorldSectorZ == 0));

    // give mortar
    CreateItem(MORTAR, (INT8)(50 + Random(51)), &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
    //***23.11.2007*** заряд для миномёта
    CreateItem(MORTAR_SHELL, (INT8)(80 + Random(21)), &Object);
    PlaceObjectInSoldierCreateStruct(pp, &Object);
  }
}

void ChooseFaceGearForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp) {
  INT32 i;
  INT8 bDifficultyRating = CalcDifficultyModifier(pp->ubSoldierClass);

  if (gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y &&
      StrategicMap[TIXA_SECTOR_X + TIXA_SECTOR_Y * MAP_WORLD_X]
          .fEnemyControlled) {  // Tixa is a special case that is handled differently.
    return;
  }

  // Look for any face item in the big pocket positions (the only place they can be added in the
  // editor) If any are found, then don't assign any
  for (i = BIGPOCK1POS; i < BIGPOCK4POS; i++) {
    if (Item[pp->Inv[i].usItem].usItemClass == IC_FACE) {
      return;
    }
  }

  // KM: (NEW)
  // Note the lack of overwritable item checking here.  This is because faceitems are not
  // supported in the editor, hence they can't have this status.
  switch (pp->ubSoldierClass) {
    case SOLDIER_CLASS_ELITE:
    case SOLDIER_CLASS_ELITE_MILITIA:
      // All elites get a gasmask and either night goggles or uv goggles.
      if (Chance(75)) {
        CreateItem(GASMASK, (INT8)(70 + Random(31)), &(pp->Inv[HEAD1POS]));
        pp->Inv[HEAD1POS].fFlags |= OBJECT_UNDROPPABLE;
      } else {
        CreateItem(EXTENDEDEAR, (INT8)(70 + Random(31)), &(pp->Inv[HEAD1POS]));
        pp->Inv[HEAD1POS].fFlags |= OBJECT_UNDROPPABLE;
        //***17.12.2009*** батарейное питание усилителя звуков
        if (Chance(50 - gGameOptions.ubDifficultyLevel * 10)) {
          pp->Inv[HEAD1POS].usAttachItem[0] = BATTERIES;
          pp->Inv[HEAD1POS].bAttachStatus[0] = (INT8)(5 + Random(16));
        }
      }
      //***08.08.2011*** добавлено условие ночной выдачи ПНВ
      if (gubEnvLightValue >= LIGHT_DUSK_CUTOFF) {
        if (Chance(75)) {
          CreateItem(NIGHTGOGGLES, (INT8)(70 + Random(31)), &(pp->Inv[HEAD2POS]));
          pp->Inv[HEAD2POS].fFlags |= OBJECT_UNDROPPABLE;
          //***15.12.2009*** батарейное питание у ПНВ
          if (Chance(50 - gGameOptions.ubDifficultyLevel * 10)) {
            pp->Inv[HEAD2POS].usAttachItem[0] = BATTERIES;
            pp->Inv[HEAD2POS].bAttachStatus[0] = (INT8)(5 + Random(16));
          }
        } else {
          CreateItem(UVGOGGLES, (INT8)(70 + Random(31)), &(pp->Inv[HEAD2POS]));
          pp->Inv[HEAD2POS].fFlags |= OBJECT_UNDROPPABLE;
          //***15.12.2009*** батарейное питание у ПНВ
          if (Chance(50 - gGameOptions.ubDifficultyLevel * 10)) {
            pp->Inv[HEAD2POS].usAttachItem[0] = BATTERIES;
            pp->Inv[HEAD2POS].bAttachStatus[0] = (INT8)(5 + Random(16));
          }
        }
      }
      break;
    case SOLDIER_CLASS_ARMY:
    case SOLDIER_CLASS_REG_MILITIA:
      if (Chance(bDifficultyRating / 2)) {  // chance of getting a face item
        if (Chance(50)) {
          CreateItem(GASMASK, (INT8)(70 + Random(31)), &(pp->Inv[HEAD1POS]));
          pp->Inv[HEAD1POS].fFlags |= OBJECT_UNDROPPABLE;
        } else
            //***08.08.2011*** добавлено условие ночной выдачи ПНВ
            if (gubEnvLightValue >= LIGHT_DUSK_CUTOFF) {
          CreateItem(NIGHTGOGGLES, (INT8)(70 + Random(31)), &(pp->Inv[HEAD1POS]));
          pp->Inv[HEAD1POS].fFlags |= OBJECT_UNDROPPABLE;
          //***15.12.2009*** батарейное питание у ПНВ
          if (Chance(60 - gGameOptions.ubDifficultyLevel * 10)) {
            pp->Inv[HEAD1POS].usAttachItem[0] = BATTERIES;
            pp->Inv[HEAD1POS].bAttachStatus[0] = (INT8)(5 + Random(11));
          }
        }
      }
      if (Chance(bDifficultyRating / 3)) {  // chance of getting a extended ear
        CreateItem(EXTENDEDEAR, (INT8)(70 + Random(31)), &(pp->Inv[HEAD2POS]));
        pp->Inv[HEAD2POS].fFlags |= OBJECT_UNDROPPABLE;
        //***17.12.2009*** батарейное питание усилителя звуков
        if (Chance(60 - gGameOptions.ubDifficultyLevel * 10)) {
          pp->Inv[HEAD2POS].usAttachItem[0] = BATTERIES;
          pp->Inv[HEAD2POS].bAttachStatus[0] = (INT8)(5 + Random(11));
        }
      }
      break;
    case SOLDIER_CLASS_ADMINISTRATOR:
    case SOLDIER_CLASS_GREEN_MILITIA:
      break;
  }
}

void ChooseKitsForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bKitClass) {
  UINT16 i;
  INVTYPE *pItem;
  UINT16 usRandom;
  UINT16 usNumMatches = 0;
  OBJECTTYPE Object;
  UINT16 usKitItem = 0;

  // we want these mostly to be first aid and medical kits, and for those kit class doesn't matter,
  // they're always useful
  if (Chance(50)) {
    usKitItem = FIRSTAIDKIT;
  } else if (Chance(25)) {
    ///		usKitItem = MEDICKIT;
  } else {
    // count how many non-medical KITS are eligible ( of sufficient or lower coolness)
    for (i = 0; i < MAXITEMS; i++) {
      pItem = &Item[i];
      // skip toolkits
      if ((pItem->usItemClass == IC_KIT) && (pItem->ubCoolness > 0) &&
          pItem->ubCoolness <= bKitClass && (i != TOOLKIT)) {
        usNumMatches++;
      }
    }

    // if any are eligible, pick one of them at random
    if (usNumMatches) {
      usRandom = (UINT16)Random(usNumMatches);
      for (i = 0; i < MAXITEMS; i++) {
        pItem = &Item[i];
        // skip toolkits
        if ((pItem->usItemClass == IC_KIT) && (pItem->ubCoolness > 0) &&
            pItem->ubCoolness <= bKitClass && (i != TOOLKIT)) {
          if (usRandom)
            usRandom--;
          else {
            usKitItem = i;
            break;
          }
        }
      }
    }
  }

  if (usKitItem != 0) {
    CreateItem(usKitItem, (INT8)(80 + Random(21)), &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
  }
}

void ChooseMiscGearForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bMiscClass) {
  UINT16 i;
  INVTYPE *pItem;
  UINT16 usRandom;
  UINT16 usNumMatches = 0;
  OBJECTTYPE Object;

  // not all of these are IC_MISC, some are IC_PUNCH (not covered anywhere else)
  //***23.11.2007*** изменён список
  INT32 iMiscItemsList[] = {CANTEEN, CANTEEN, CANTEEN, CANTEEN,
                            //		STRING,
                            ALCOHOL, ALCOHOL, ADRENALINE_BOOSTER, ADRENALINE_BOOSTER,
                            BREAK_LIGHT,        // REGEN_BOOSTER,
                            CIGARETTE_LIGHTER,  // BRASS_KNUCKLES,
                            CHEWING_GUM, CIGARS, GOLDWATCH,
                            //		STRING,
                            TIN_CAN, -1};

  // count how many are eligible
  i = 0;
  while (iMiscItemsList[i] != -1) {
    pItem = &Item[iMiscItemsList[i]];
    if ((pItem->ubCoolness > 0) && (pItem->ubCoolness <= bMiscClass)) {
      // exclude REGEN_BOOSTERs unless Sci-Fi flag is on
      if ((iMiscItemsList[i] != REGEN_BOOSTER) || (gGameOptions.fSciFi)) {
        usNumMatches++;
      }
    }

    i++;
  }

  // if any are eligible, pick one of them at random
  if (usNumMatches) {
    usRandom = (UINT16)Random(usNumMatches);

    i = 0;
    while (iMiscItemsList[i] != -1) {
      pItem = &Item[iMiscItemsList[i]];
      if ((pItem->ubCoolness > 0) && (pItem->ubCoolness <= bMiscClass)) {
        // exclude REGEN_BOOSTERs unless Sci-Fi flag is on
        if ((iMiscItemsList[i] != REGEN_BOOSTER) || (gGameOptions.fSciFi)) {
          if (usRandom)
            usRandom--;
          else {
            CreateItem((UINT16)iMiscItemsList[i], (INT8)(80 + Random(21)), &Object);
            Object.fFlags |= OBJECT_UNDROPPABLE;
            PlaceObjectInSoldierCreateStruct(pp, &Object);
            break;
          }
        }
      }

      i++;
    }
  }
}

void ChooseBombsForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bBombClass) {
  UINT16 i;
  INVTYPE *pItem;
  UINT16 usRandom;
  UINT16 usNumMatches = 0;
  OBJECTTYPE Object;

  // count how many are eligible
  for (i = 0; i < MAXITEMS; i++) {
    pItem = &Item[i];
    if ((pItem->usItemClass == IC_BOMB) && (pItem->ubCoolness > 0) &&
        (pItem->ubCoolness <= bBombClass)) {
      usNumMatches++;
    }
  }

  // if any are eligible, pick one of them at random
  if (usNumMatches) {
    usRandom = (UINT16)Random(usNumMatches);
    for (i = 0; i < MAXITEMS; i++) {
      pItem = &Item[i];
      if ((pItem->usItemClass == IC_BOMB) && (pItem->ubCoolness > 0) &&
          (pItem->ubCoolness <= bBombClass)) {
        if (usRandom)
          usRandom--;
        else {
          CreateItem(i, (INT8)(80 + Random(21)), &Object);
          Object.fFlags |= OBJECT_UNDROPPABLE;
          PlaceObjectInSoldierCreateStruct(pp, &Object);
          break;
        }
      }
    }
  }
}

void ChooseLocationSpecificGearForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp) {
  OBJECTTYPE Object;

  // If this is Tixa and the player doesn't control Tixa then give all enemies gas masks,
  // but somewhere on their person, not in their face positions
  if (gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y &&
      StrategicMap[TIXA_SECTOR_X + TIXA_SECTOR_Y * MAP_WORLD_X].fEnemyControlled) {
    CreateItem(GASMASK, (INT8)(95 + Random(6)), &Object);
    PlaceObjectInSoldierCreateStruct(pp, &Object);
  }
}

BOOLEAN PlaceObjectInSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, OBJECTTYPE *pObject) {
  INT8 i;
  if (!Item[pObject->usItem].ubPerPocket) {  // ubPerPocket == 0 will only fit in large pockets.
    pObject->ubNumberOfObjects = 1;
    for (i = BIGPOCK1POS; i <= BIGPOCK4POS; i++) {
      if (!(pp->Inv[i].usItem) && !(pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)) {
        memcpy(&(pp->Inv[i]), pObject, sizeof(OBJECTTYPE));
        return TRUE;
      }
    }
    return FALSE;
  } else {
    pObject->ubNumberOfObjects =
        (UINT8)min(Item[pObject->usItem].ubPerPocket, pObject->ubNumberOfObjects);
    // try to get it into a small pocket first
    for (i = SMALLPOCK1POS; i <= SMALLPOCK8POS; i++) {
      if (!(pp->Inv[i].usItem) && !(pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)) {
        memcpy(&(pp->Inv[i]), pObject, sizeof(OBJECTTYPE));
        return TRUE;
      }
    }
    for (i = BIGPOCK1POS; i <= BIGPOCK4POS;
         i++) {  // no space free in small pockets, so put it into a large pocket.
      if (!(pp->Inv[i].usItem) && !(pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)) {
        memcpy(&(pp->Inv[i]), pObject, sizeof(OBJECTTYPE));
        return TRUE;
      }
    }
  }
  return FALSE;
}

void RandomlyChooseWhichItemsAreDroppable(SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass) {
  INT32 i;
  //	UINT16 usRandomNum;
  UINT32 uiItemClass;
  UINT8 ubNumMatches = 0;
  UINT16 usItem;
  UINT8 ubAmmoDropRate;
  UINT8 ubGrenadeDropRate;
  UINT8 ubOtherDropRate;
  BOOLEAN fWeapon = FALSE;
  BOOLEAN fGrenades = FALSE;  // this includes all  grenades!
  BOOLEAN fAmmo = FALSE;
  BOOLEAN fArmour = FALSE;
  BOOLEAN fKnife = FALSE;
  BOOLEAN fKit = FALSE;
  BOOLEAN fFace = FALSE;
  BOOLEAN fMisc = FALSE;

  // only enemy soldiers use auto-drop system!
  // don't use the auto-drop system in auto-resolve: player won't see what's being used & enemies
  // will often win & keep'em
  /**	if ( SOLDIER_CLASS_ENEMY( bSoldierClass ) && !IsAutoResolveActive() )
          {
                  // SPECIAL handling for weapons: we'll always drop a weapon type that has never
  been dropped before for( i = 0; i < NUM_INV_SLOTS; i++ )
                  {
                          usItem = pp->Inv[ i ].usItem;
                          // if it's a weapon (monster parts included - they won't drop due to
  checks elsewhere!) if ((usItem > NONE) && (usItem < MAX_WEAPONS))
                          {
                                  // and we're allowed to change its flags
                                  if(! (pp->Inv[ i ].fFlags & OBJECT_NO_OVERWRITE ))
                                  {
                                          // and it's never been dropped before in this game
                                          if (!gStrategicStatus.fWeaponDroppedAlready[usItem])
                                          {
                                                  // mark it as droppable, and remember we did so.
  If the player never kills this particular dude, oh well,
                                                  // tough luck, he missed his chance for an easy
  reward, he'll have to wait til next time and need some luck... pp->Inv[ i ].fFlags &=
  ~OBJECT_UNDROPPABLE;

                                                  MarkAllWeaponsOfSameGunClassAsDropped( usItem,
  bSoldierClass );
                                          }
                                  }
                          }
                  }
          }
  **/

  /**
          if ( SOLDIER_CLASS_MILITIA( bSoldierClass ) )
          {
                  // militia (they drop much less stuff)
                  ubAmmoDropRate		= MILITIAAMMODROPRATE;
                  ubGrenadeDropRate = MILITIAGRENADEDROPRATE;
                  ubOtherDropRate = MILITIAEQUIPDROPRATE;
          }
          else
          {
                  // enemy army
                  ubAmmoDropRate  = ENEMYAMMODROPRATE;
                  ubGrenadeDropRate = ENEMYGRENADEDROPRATE;
                  ubOtherDropRate = ENEMYEQUIPDROPRATE;
          }



          if( Random(100) < ubAmmoDropRate )
                  fAmmo = TRUE;

          if( Random(100) < ubOtherDropRate )
                  fWeapon = TRUE;

          if( Random(100) < ubOtherDropRate )
                  fArmour = TRUE;

          if( Random(100) < ubOtherDropRate )
                  fKnife = TRUE;

          if( Random(100) < ubGrenadeDropRate )
                  fGrenades = TRUE;

          if( Random(100) < ubOtherDropRate )
                  fKit = TRUE;

          if( Random(100) < (UINT32)(ubOtherDropRate / 3) )
                  fFace = TRUE;

          if( Random(100) < ubOtherDropRate )
                  fMisc = TRUE;
  **/
  //***06.10.2014*** переделано на индивидуальное назначение вероятности каждой группе
  if (SOLDIER_CLASS_MILITIA(bSoldierClass)) {
    if (Random(100) < gubMilitiaDropRate[AMMODROPRATE]) fAmmo = TRUE;

    if (Random(100) < gubMilitiaDropRate[WEAPONDROPRATE]) fWeapon = TRUE;

    if (Random(100) < gubMilitiaDropRate[ARMOURDROPRATE]) fArmour = TRUE;

    if (Random(100) < gubMilitiaDropRate[KNIFEDROPRATE]) fKnife = TRUE;

    if (Random(100) < gubMilitiaDropRate[GRENADEDROPRATE]) fGrenades = TRUE;

    if (Random(100) < gubMilitiaDropRate[KITDROPRATE]) fKit = TRUE;

    if (Random(100) < gubMilitiaDropRate[FACEDROPRATE]) fFace = TRUE;

    if (Random(100) < gubMilitiaDropRate[MISCDROPRATE]) fMisc = TRUE;
  } else {
    if (Random(100) < gubEnemyDropRate[AMMODROPRATE]) fAmmo = TRUE;

    if (Random(100) < gubEnemyDropRate[WEAPONDROPRATE]) fWeapon = TRUE;

    if (Random(100) < gubEnemyDropRate[ARMOURDROPRATE]) fArmour = TRUE;

    if (Random(100) < gubEnemyDropRate[KNIFEDROPRATE]) fKnife = TRUE;

    if (Random(100) < gubEnemyDropRate[GRENADEDROPRATE]) fGrenades = TRUE;

    if (Random(100) < gubEnemyDropRate[KITDROPRATE]) fKit = TRUE;

    if (Random(100) < gubEnemyDropRate[FACEDROPRATE]) fFace = TRUE;

    if (Random(100) < gubEnemyDropRate[MISCDROPRATE]) fMisc = TRUE;
  }

  // Now, that the flags are set for each item, we now have to search through the item slots to
  // see if we can find a matching item, however, if we find any items in a particular class that
  // have the OBJECT_NO_OVERWRITE flag set, we will not make any items droppable for that class
  // because the editor would have specified it already.
  if (fAmmo) {
    // now drops ALL ammo found, not just the first slot
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_AMMO) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          continue;
        else {
          pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
        }
      }
    }
  }

  if (fWeapon) {
    ubNumMatches = 0;
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else
          ubNumMatches++;
      }
    }
    if (ubNumMatches > 0) {
      for (i = 0; i < NUM_INV_SLOTS; i++) {
        uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
        if (uiItemClass == IC_GUN || uiItemClass == IC_LAUNCHER) {
          if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
            break;
          else if (!Random(ubNumMatches--)) {
            pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
            break;
          }
        }
      }
    }
  }

  if (fArmour) {
    ubNumMatches = 0;
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_ARMOUR) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else
          ubNumMatches++;
      }
    }
    if (ubNumMatches > 0) {
      for (i = 0; i < NUM_INV_SLOTS; i++) {
        uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
        if (uiItemClass == IC_ARMOUR) {
          if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
            break;
          else if (!Random(ubNumMatches--)) {
            pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
            break;
          }
        }
      }
    }
  }

  if (fKnife) {
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      // drops FIRST knife found
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_BLADE || uiItemClass == IC_THROWING_KNIFE) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else {
          pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
          break;
        }
      }
    }
  }

  // note that they'll only drop ONE TYPE of grenade if they have multiple types (very common)
  if (fGrenades) {
    ubNumMatches = 0;
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass & IC_GRENADE) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else
          ubNumMatches++;
      }
    }
    if (ubNumMatches > 0) {
      for (i = 0; i < NUM_INV_SLOTS; i++) {
        uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
        if (uiItemClass & IC_GRENADE) {
          if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
            break;
          else if (!Random(ubNumMatches--)) {
            pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
            break;
          }
        }
      }
    }
  }

  if (fKit) {
    ubNumMatches = 0;
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_MEDKIT || uiItemClass == IC_KIT) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else
          ubNumMatches++;
      }
    }
    if (ubNumMatches > 0) {
      for (i = 0; i < NUM_INV_SLOTS; i++) {
        uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
        if (uiItemClass == IC_MEDKIT || uiItemClass == IC_KIT) {
          if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
            break;
          else if (!Random(ubNumMatches--)) {
            pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
            break;
          }
        }
      }
    }
  }

  if (fFace) {
    ubNumMatches = 0;
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_FACE) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else
          ubNumMatches++;
      }
    }
    if (ubNumMatches > 0) {
      for (i = 0; i < NUM_INV_SLOTS; i++) {
        uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
        if (uiItemClass == IC_FACE) {
          if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
            break;
          else if (!Random(ubNumMatches--)) {
            pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
            break;
          }
        }
      }
    }
  }

  if (fMisc) {
    ubNumMatches = 0;
    for (i = 0; i < NUM_INV_SLOTS; i++) {
      uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
      if (uiItemClass == IC_MISC) {
        if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
          break;
        else
          ubNumMatches++;
      }
    }
    if (ubNumMatches > 0) {
      for (i = 0; i < NUM_INV_SLOTS; i++) {
        uiItemClass = Item[pp->Inv[i].usItem].usItemClass;
        if (uiItemClass == IC_MISC) {
          if (pp->Inv[i].fFlags & OBJECT_NO_OVERWRITE)
            break;
          else if (!Random(ubNumMatches--)) {
            pp->Inv[i].fFlags &= ~OBJECT_UNDROPPABLE;
            break;
          }
        }
      }
    }
  }
}

void AssignCreatureInventory(SOLDIERCLASS *pSoldier) {
  UINT32 uiChanceToDrop = 0;
  BOOLEAN fMaleCreature = FALSE;
  BOOLEAN fBloodcat = FALSE;

  // all creature items in this first section are only offensive/defensive placeholders, and
  // never get dropped, because they're not real items!
  //***3.11.2007*** унификация когтей и слюней
  switch (pSoldier->ubBodyType) {
    case ADULTFEMALEMONSTER:
      CreateItem(CREATURE_CLAWS /*CREATURE_OLD_FEMALE_CLAWS*/, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(CREATURE_OLD_FEMALE_HIDE, 100, &(pSoldier->inv[HELMETPOS]));
      CreateItem(CREATURE_OLD_FEMALE_HIDE, 100, &(pSoldier->inv[VESTPOS]));
      CreateItem(CREATURE_OLD_FEMALE_HIDE, 100, &(pSoldier->inv[LEGPOS]));
      uiChanceToDrop = 30;
      break;
    case AM_MONSTER:
      CreateItem(CREATURE_CLAWS /*CREATURE_OLD_MALE_CLAWS*/, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(CREATURE_OLD_MALE_SPIT, 100, &(pSoldier->inv[SECONDHANDPOS]));
      CreateItem(CREATURE_OLD_MALE_HIDE, 100, &(pSoldier->inv[HELMETPOS]));
      CreateItem(CREATURE_OLD_MALE_HIDE, 100, &(pSoldier->inv[VESTPOS]));
      CreateItem(CREATURE_OLD_MALE_HIDE, 100, &(pSoldier->inv[LEGPOS]));
      uiChanceToDrop = 30;
      fMaleCreature = TRUE;
      break;
    case YAF_MONSTER:
      CreateItem(CREATURE_CLAWS /*CREATURE_YOUNG_FEMALE_CLAWS*/, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(CREATURE_YOUNG_FEMALE_HIDE, 100, &(pSoldier->inv[HELMETPOS]));
      CreateItem(CREATURE_YOUNG_FEMALE_HIDE, 100, &(pSoldier->inv[VESTPOS]));
      CreateItem(CREATURE_YOUNG_FEMALE_HIDE, 100, &(pSoldier->inv[LEGPOS]));
      uiChanceToDrop = 15;
      break;
    case YAM_MONSTER:
      CreateItem(CREATURE_CLAWS /*CREATURE_YOUNG_MALE_CLAWS*/, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(CREATURE_YOUNG_MALE_SPIT, 100, &(pSoldier->inv[SECONDHANDPOS]));
      CreateItem(CREATURE_YOUNG_MALE_HIDE, 100, &(pSoldier->inv[HELMETPOS]));
      CreateItem(CREATURE_YOUNG_MALE_HIDE, 100, &(pSoldier->inv[VESTPOS]));
      CreateItem(CREATURE_YOUNG_MALE_HIDE, 100, &(pSoldier->inv[LEGPOS]));
      uiChanceToDrop = 15;
      fMaleCreature = TRUE;
      break;
    case INFANT_MONSTER:
      CreateItem(CREATURE_INFANT_SPIT, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(CREATURE_INFANT_HIDE, 100, &(pSoldier->inv[HELMETPOS]));
      CreateItem(CREATURE_INFANT_HIDE, 100, &(pSoldier->inv[VESTPOS]));
      CreateItem(CREATURE_INFANT_HIDE, 100, &(pSoldier->inv[LEGPOS]));
      uiChanceToDrop = 5;
      break;
    case LARVAE_MONSTER:
      uiChanceToDrop = 0;
      break;
    case QUEENMONSTER:
      CreateItem(CREATURE_QUEEN_SPIT, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(CREATURE_QUEEN_TENTACLES, 100, &(pSoldier->inv[SECONDHANDPOS]));
      CreateItem(CREATURE_QUEEN_HIDE, 100, &(pSoldier->inv[HELMETPOS]));
      CreateItem(CREATURE_QUEEN_HIDE, 100, &(pSoldier->inv[VESTPOS]));
      CreateItem(CREATURE_QUEEN_HIDE, 100, &(pSoldier->inv[LEGPOS]));
      // she can't drop anything, because the items are unreachable anyways (she's too big!)
      uiChanceToDrop = 0;
      break;
    case BLOODCAT:
      CreateItem(BLOODCAT_BITE /*BLOODCAT_CLAW_ATTACK*/, 100, &(pSoldier->inv[HANDPOS]));
      CreateItem(BLOODCAT_BITE, 100, &(pSoldier->inv[SECONDHANDPOS]));
      fBloodcat = TRUE;
      uiChanceToDrop = 0;  /// 30;
      break;

    default:
      AssertMsg(FALSE, String("Invalid creature bodytype %d", pSoldier->ubBodyType));
      return;
  }

#ifndef JA2DEMO

  // decide if the creature will drop any REAL bodyparts
  if (Random(100) < uiChanceToDrop) {
    CreateItem((UINT16)(fBloodcat ? BLOODCAT_CLAWS : CREATURE_PART_CLAWS), (INT8)(80 + Random(21)),
               &(pSoldier->inv[BIGPOCK1POS]));
  }

  if (Random(100) < uiChanceToDrop) {
    CreateItem((UINT16)(fBloodcat ? BLOODCAT_TEETH : CREATURE_PART_FLESH), (INT8)(80 + Random(21)),
               &(pSoldier->inv[BIGPOCK2POS]));
  }

  // as requested by ATE, males are more likely to drop their "organs" (he actually suggested this,
  // I'm serious!)
  if (fMaleCreature) {
    // increase chance by 50%
    uiChanceToDrop += (uiChanceToDrop / 2);
  }

  if (Random(100) < uiChanceToDrop) {
    CreateItem((UINT16)(fBloodcat ? BLOODCAT_PELT : CREATURE_PART_ORGAN), (INT8)(80 + Random(21)),
               &(pSoldier->inv[BIGPOCK3POS]));
  }

#endif
}
/* ***28.07.2010*** закомментировано
void ReplaceExtendedGuns( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass )
{
        UINT32				uiLoop, uiLoop2, uiAttachDestIndex;
        INT8					bWeaponClass;
        OBJECTTYPE		OldObj;
        UINT16				usItem, usNewGun, usAmmo, usNewAmmo;

        for ( uiLoop = 0; uiLoop < NUM_INV_SLOTS; uiLoop++ )
        {
                usItem = pp->Inv[ uiLoop ].usItem;

                if ( ( Item[ usItem ].usItemClass & IC_GUN ) && ExtendedGunListGun( usItem ) )
                {
                        if ( bSoldierClass == SOLDIER_CLASS_NONE )
                        {
                                usNewGun = StandardGunListReplacement( usItem );
                        }
                        else
                        {
                                bWeaponClass = GetWeaponClass( usItem );
                                AssertMsg( bWeaponClass != -1, String( "Gun %d does not have a match
in the extended gun array", usItem ) ); usNewGun = SelectStandardArmyGun( bWeaponClass );
                        }

                        if ( usNewGun != NOTHING )
                        {
                                // have to replace!  but first (ugh) must store backup (b/c of
attachments) CopyObj( &(pp->Inv[ uiLoop ]), &OldObj ); CreateItem( usNewGun, OldObj.bGunStatus,
&(pp->Inv[ uiLoop ]) ); pp->Inv[ uiLoop ].fFlags = OldObj.fFlags;

                                // copy any valid attachments; for others, just drop them...
                                if ( ItemHasAttachments( &OldObj ) )
                                {
                                        // we're going to copy into the first attachment position
first :-) uiAttachDestIndex = 0;
                                        // loop!
                                        for ( uiLoop2 = 0; uiLoop2 < MAX_ATTACHMENTS; uiLoop2++ )
                                        {
                                                if ( ( OldObj.usAttachItem[ uiLoop2 ] != NOTHING )
&& ValidAttachment( OldObj.usAttachItem[ uiLoop2 ], usNewGun ) )
                                                {
                                                        pp->Inv[ uiLoop ].usAttachItem[
uiAttachDestIndex ] = OldObj.usAttachItem[ uiLoop2 ]; pp->Inv[ uiLoop ].bAttachStatus[
uiAttachDestIndex ] = OldObj.bAttachStatus[ uiLoop2 ]; uiAttachDestIndex++;
                                                }
                                        }
                                }

                                // must search through inventory and replace ammo accordingly
                                for ( uiLoop2 = 0; uiLoop2 < NUM_INV_SLOTS; uiLoop2++ )
                                {
                                        usAmmo = pp->Inv[ uiLoop2 ].usItem;
                                        if ( (Item[ usAmmo ].usItemClass & IC_AMMO) )
                                        {
                                                usNewAmmo = FindReplacementMagazineIfNecessary(
usItem, usAmmo, usNewGun ); if (usNewAmmo != NOTHING )
                                                {
                                                        // found a new magazine, replace...
                                                        CreateItems( usNewAmmo, 100, pp->Inv[
uiLoop2 ].ubNumberOfObjects, &( pp->Inv[ uiLoop2 ] ) );
                                                }
                                        }
                                }
                        }
                }

        }
}
*/

UINT16 SelectStandardArmyGun(UINT8 uiGunLevel, UINT8 ubSoldierClass) {
  ARMY_GUN_CHOICE_TYPE *pGunChoiceTable;
  UINT32 uiChoice;
  UINT16 usGunIndex;

  // pick the standard army gun for this weapon class from table
  //	usGunIndex = gStrategicStatus.ubStandardArmyGunIndex[uiGunLevel];

  // decided to randomize it afterall instead of repeating the same weapon over and over

  // depending on selection of the gun nut option
  /* ***28.07.2010*** закомментировано, переделано
          if (gGameOptions.fGunNut)
          {
                  // use table of extended gun choices
                  pGunChoiceTable = &(gExtendedArmyGunChoices[0]);
          }
          else gExtendedArmyGunChoices
          {
                  // use table of regular gun choices
                  pGunChoiceTable = &(gRegularArmyGunChoices[0]);
          }
  */
  pGunChoiceTable = gExtendedArmyGunChoices[ubSoldierClass];

  // choose one the of the possible gun choices
  uiChoice = Random(pGunChoiceTable[uiGunLevel].ubChoices);
  usGunIndex = pGunChoiceTable[uiGunLevel].usItemNo[uiChoice];

  Assert(usGunIndex);

  return (usGunIndex);
}

void EquipTank(SOLDIERCREATE_STRUCT *pp) {
  OBJECTTYPE Object;

  // tanks get special equipment, and they drop nothing (MGs are hard-mounted & non-removable)

  //***3.11.2007*** изменена экипировка
  // main cannon
  CreateItem(TANK_CANNON, (INT8)(80 + Random(21)), &(pp->Inv[HANDPOS]));
  pp->Inv[HANDPOS].fFlags |= OBJECT_UNDROPPABLE;

  // machine gun
  CreateItems(427 /*MINIMI*/, (INT8)(80 + Random(21)), 1, &Object);
  Object.fFlags |= OBJECT_UNDROPPABLE;
  PlaceObjectInSoldierCreateStruct(pp, &Object);

  // tanks don't deplete shells or ammo...
  CreateItems(TANK_SHELL, 100, 1, &Object);
  Object.fFlags |= OBJECT_UNDROPPABLE;
  PlaceObjectInSoldierCreateStruct(pp, &Object);

  // armour equal to spectra all over (for vs explosives)
  CreateItem(187 /*SPECTRA_VEST*/, 100, &(pp->Inv[VESTPOS]));
  pp->Inv[VESTPOS].fFlags |= OBJECT_UNDROPPABLE;
  CreateItem(187 /*SPECTRA_HELMET*/, 100, &(pp->Inv[HELMETPOS]));
  pp->Inv[HELMETPOS].fFlags |= OBJECT_UNDROPPABLE;
  CreateItem(187 /*SPECTRA_LEGGINGS*/, 100, &(pp->Inv[LEGPOS]));
  pp->Inv[LEGPOS].fFlags |= OBJECT_UNDROPPABLE;
}

void ResetMortarsOnTeamCount(void) { guiMortarsRolledByTeam = 0; }

//***23.06.2013*** Отдельные таблицы брони по классам солдат
void ChooseArmourForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bHelmetClass,
                                        INT8 bVestClass, INT8 bLeggingsClass) {
  UINT16 i;
  INVTYPE *pItem;
  UINT16 usRandom;
  UINT16 usNumMatches;
  INT8 bOrigVestClass = bVestClass;
  OBJECTTYPE Object;

  ARMY_GUN_CHOICE_TYPE *pArmourChoiceTable;

  pArmourChoiceTable = gExtendedArmyGunChoices[pp->ubSoldierClass];

  // Choose helmet
  if (bHelmetClass) {
    usNumMatches = 0;
    bHelmetClass--;

    for (i = 0; i < pArmourChoiceTable[bHelmetClass].ubArmourChoices; i++) {
      pItem = &Item[pArmourChoiceTable[bHelmetClass].usArmourItemNo[i]];

      if (pItem->usItemClass == IC_ARMOUR) {
        if (Armour[pItem->ubClassIndex].ubArmourClass == ARMOURCLASS_HELMET) usNumMatches++;
      }
    }

    if (usNumMatches) {  // There is a helmet that we can choose.
      usRandom = (UINT16)Random(usNumMatches);
      for (i = 0; i < pArmourChoiceTable[bHelmetClass].ubArmourChoices; i++) {
        pItem = &Item[pArmourChoiceTable[bHelmetClass].usArmourItemNo[i]];
        if (pItem->usItemClass == IC_ARMOUR) {
          if (Armour[pItem->ubClassIndex].ubArmourClass == ARMOURCLASS_HELMET) {
            if (usRandom)
              usRandom--;
            else {
              if (!(pp->Inv[HELMETPOS].fFlags & OBJECT_NO_OVERWRITE)) {
                CreateItem(pArmourChoiceTable[bHelmetClass].usArmourItemNo[i],
                           (INT8)(70 + Random(31)), &(pp->Inv[HELMETPOS]));
                pp->Inv[HELMETPOS].fFlags |= OBJECT_UNDROPPABLE;
              }
              break;
            }
          }
        }
      }
    }
  }
  // Choose vest
  if (bVestClass) {
    usNumMatches = 0;
    bVestClass--;

    for (i = 0; i < pArmourChoiceTable[bVestClass].ubArmourChoices; i++) {
      pItem = &Item[pArmourChoiceTable[bVestClass].usArmourItemNo[i]];

      if (pItem->usItemClass == IC_ARMOUR) {
        if (Armour[pItem->ubClassIndex].ubArmourClass == ARMOURCLASS_VEST) usNumMatches++;
      }
    }

    if (usNumMatches) {  // There is an armour that we can choose.
      usRandom = (UINT16)Random(usNumMatches);
      for (i = 0; i < pArmourChoiceTable[bVestClass].ubArmourChoices; i++) {
        pItem = &Item[pArmourChoiceTable[bVestClass].usArmourItemNo[i]];
        if (pItem->usItemClass == IC_ARMOUR) {
          if (Armour[pItem->ubClassIndex].ubArmourClass == ARMOURCLASS_VEST) {
            if (usRandom)
              usRandom--;
            else {
              if (!(pp->Inv[VESTPOS].fFlags & OBJECT_NO_OVERWRITE)) {
                CreateItem(pArmourChoiceTable[bVestClass].usArmourItemNo[i],
                           (INT8)(70 + Random(31)), &(pp->Inv[VESTPOS]));
                pp->Inv[VESTPOS].fFlags |= OBJECT_UNDROPPABLE;

                ///								if ( ( i ==
                /// KEVLAR_VEST
                ///)
                ///||
                ///(
                /// i
                ///== SPECTRA_VEST ) )
                if (ValidAttachment(CERAMIC_PLATES, pp->Inv[VESTPOS].usItem))  //***27.06.2013***
                {
                  // roll to see if he gets a CERAMIC PLATES, too.  Higher chance the higher his
                  // entitled vest class is
                  if ((INT8)Random(100) < (15 * (bOrigVestClass - pItem->ubCoolness))) {
                    CreateItem(CERAMIC_PLATES, (INT8)(70 + Random(31)), &Object);
                    Object.fFlags |= OBJECT_UNDROPPABLE;
                    AttachObject(NULL, &(pp->Inv[VESTPOS]), &Object);
                  }
                }
              }
              break;
            }
          }
        }
      }
    }
  }
  // Choose Leggings
  if (bLeggingsClass) {
    usNumMatches = 0;
    bLeggingsClass--;

    for (i = 0; i < pArmourChoiceTable[bLeggingsClass].ubArmourChoices; i++) {
      pItem = &Item[pArmourChoiceTable[bLeggingsClass].usArmourItemNo[i]];
      if (pItem->usItemClass == IC_ARMOUR) {
        if (Armour[pItem->ubClassIndex].ubArmourClass == ARMOURCLASS_LEGGINGS) usNumMatches++;
      }
    }

    if (usNumMatches) {  // There is a legging that we can choose.
      usRandom = (UINT16)Random(usNumMatches);
      for (i = 0; i < pArmourChoiceTable[bLeggingsClass].ubArmourChoices; i++) {
        pItem = &Item[pArmourChoiceTable[bLeggingsClass].usArmourItemNo[i]];
        if (pItem->usItemClass == IC_ARMOUR) {
          if (Armour[pItem->ubClassIndex].ubArmourClass == ARMOURCLASS_LEGGINGS) {
            if (usRandom)
              usRandom--;
            else {
              if (!(pp->Inv[LEGPOS].fFlags & OBJECT_NO_OVERWRITE)) {
                CreateItem(pArmourChoiceTable[bLeggingsClass].usArmourItemNo[i],
                           (INT8)(70 + Random(31)), &(pp->Inv[LEGPOS]));
                pp->Inv[LEGPOS].fFlags |= OBJECT_UNDROPPABLE;
                break;
              }
            }
          }
        }
      }
    }
  }
}

//***21.11.2014***
UINT16 SelectGrenade(UINT8 ubGrenadeLevel, UINT8 ubSoldierClass) {
  ARMY_GUN_CHOICE_TYPE *pGrenadeChoiceTable;
  UINT32 uiChoice;
  UINT16 usGrenadeIndex;

  pGrenadeChoiceTable = gExtendedArmyGunChoices[ubSoldierClass];

  uiChoice = Random(pGrenadeChoiceTable[ubGrenadeLevel].ubGrenadeChoices);
  usGrenadeIndex = pGrenadeChoiceTable[ubGrenadeLevel].usGrenadeItemNo[uiChoice];

  return (usGrenadeIndex);
}

//***21.11.2014*** Отдельные таблицы ручных гранат по классам солдат
void ChooseGrenadesForSoldierCreateStruct(SOLDIERCREATE_STRUCT *pp, INT8 bGrenades,
                                          INT8 bGrenadeClass, BOOLEAN fGrenadeLauncher) {
  OBJECTTYPE Object;
  INT16 sNumPoints;
  UINT16 usItem;
  UINT8 ubBaseQuality;
  UINT8 ubQualityVariation;
  // numbers of each type the player will get!
  UINT8 ubNumStun = 0;
  UINT8 ubNumTear = 0;
  UINT8 ubNumMustard = 0;
  UINT8 ubNumMini = 0;
  UINT8 ubNumReg = 0;
  UINT8 ubNumSmoke = 0;
  UINT8 ubNumFlare = 0;

  // special mortar shell handling
  if (bGrenadeClass == MORTAR_GRENADE_CLASS) {
    CreateItems(MORTAR_SHELL, (INT8)(80 + Random(21)), bGrenades, &Object);
    Object.fFlags |= OBJECT_UNDROPPABLE;
    PlaceObjectInSoldierCreateStruct(pp, &Object);
    return;
  }

  if (bGrenadeClass > ARMY_GUN_LEVELS) {
    bGrenadeClass = ARMY_GUN_LEVELS;
  }

  // determine the quality of grenades.  The elite guys get the best quality, while the others
  // get progressively worse.
  ubBaseQuality = (UINT8)min(45 + bGrenadeClass * 5, 90);
  ubQualityVariation = 101 - ubBaseQuality;

  if (!fGrenadeLauncher) {
    for (sNumPoints = 0; sNumPoints < bGrenades; sNumPoints++) {
      if (!sNumPoints && (pp->ubSoldierClass == SOLDIER_CLASS_ELITE ||
                          pp->ubSoldierClass == SOLDIER_CLASS_ELITE_MILITIA))
        usItem = SMOKE_GRENADE;
      else
        usItem = SelectGrenade((UINT8)(bGrenadeClass - 1), pp->ubSoldierClass);

      if (usItem == NONE) continue;

      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), 1, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
  } else  //для гранатомёта оставлен оригинальный механизм
  {
    // determine how many *points* the enemy will get to spend on grenades...
    sNumPoints = bGrenades * bGrenadeClass;

    // no points, no grenades!
    if (!sNumPoints) return;

    // now, purchase the grenades.
    while (sNumPoints > 0) {
      if (sNumPoints >= 20) {
        ubNumReg++, sNumPoints -= 9;
      }

      // Choose randomly between any
      switch (Random(7)) {
        case 0:
        case 1:
          ubNumReg++;
          sNumPoints -= 9;
          break;
        case 2:
        case 3:
          ubNumTear++;
          sNumPoints -= 6;
          break;
        case 4:
          ubNumStun++;
          sNumPoints -= 5;
          break;
        case 5:
          ubNumSmoke++;
          sNumPoints -= 4;
          break;
        case 6:
          ubNumFlare++;
          sNumPoints -= 3;
          break;
      }
    }

    // Create the grenades and add them to the soldier
    if (ubNumSmoke) {
      usItem = GL_SMOKE_GRENADE;
      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), ubNumSmoke, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
    if (ubNumTear) {
      usItem = GL_TEARGAS_GRENADE;
      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), ubNumTear, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
    if (ubNumStun) {
      usItem = GL_STUN_GRENADE;
      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), ubNumStun, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
    if (ubNumReg) {
      usItem = GL_HE_GRENADE;
      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), ubNumReg, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
    if (ubNumMustard) {
      usItem = GL_MUSTARD_GRENADE;
      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), ubNumMustard,
                  &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
    if (ubNumFlare && gubEnvLightValue >= LIGHT_DUSK_CUTOFF) {
      usItem = GL_LIGHT_GRENADE;
      CreateItems(usItem, (INT8)(ubBaseQuality + Random(ubQualityVariation)), ubNumFlare, &Object);
      Object.fFlags |= OBJECT_UNDROPPABLE;
      PlaceObjectInSoldierCreateStruct(pp, &Object);
    }
  }
}
