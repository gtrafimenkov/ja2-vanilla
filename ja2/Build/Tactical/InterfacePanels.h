#ifndef __INTERFACE_PANELS
#define __INTERFACE_PANELS

#include "Tactical/InterfaceItems.h"

// DEFINES FOR VARIOUS PANELS
#define CLOCK_X 554
#define CLOCK_Y 459
#define SM_ITEMDESC_START_X 214
#define SM_ITEMDESC_START_Y 1 + INV_INTERFACE_START_Y
#define SM_ITEMDESC_HEIGHT 128
#define SM_ITEMDESC_WIDTH 358

// SINGLE MERC SCREEN

#define SM_SELMERC_AP_X 70
#define SM_SELMERC_AP_Y 393
#define SM_SELMERC_AP_HEIGHT 12
#define SM_SELMERC_AP_WIDTH 11

#define SM_SELMERC_BREATH_X 75
#define SM_SELMERC_BREATH_Y 387
#define SM_SELMERC_BREATH_WIDTH 3
#define SM_SELMERC_BREATH_HEIGHT 42

#define SM_SELMERC_HEALTH_X 69
#define SM_SELMERC_HEALTH_Y 387
#define SM_SELMERC_HEALTH_WIDTH 3
#define SM_SELMERC_HEALTH_HEIGHT 42

#define SM_SELMERC_MORALE_X 81
#define SM_SELMERC_MORALE_Y 387
#define SM_SELMERC_MORALE_WIDTH 3
#define SM_SELMERC_MORALE_HEIGHT 42

#define SM_SELMERCNAME_X 7
#define SM_SELMERCNAME_Y 395

#define SM_SELMERCNAME_WIDTH 53
#define SM_SELMERCNAME_HEIGHT 9

#define SM_SELMERC_FACE_X 13
#define SM_SELMERC_FACE_Y 346
#define SM_SELMERC_FACE_HEIGHT 42
#define SM_SELMERC_FACE_WIDTH 48

#define SM_SELMERC_PLATE_X 4
#define SM_SELMERC_PLATE_Y 342
#define SM_SELMERC_PLATE_HEIGHT 65
#define SM_SELMERC_PLATE_WIDTH 83

#define SM_BODYINV_X 244
#define SM_BODYINV_Y 346

#define STATS_TITLE_FONT_COLOR 6
#define STATS_TEXT_FONT_COLOR 5

#define SM_TALKB_X 155
#define SM_TALKB_Y 448
#define SM_MUTEB_X 91
#define SM_MUTEB_Y 448
#define SM_STANCEUPB_X 187
#define SM_STANCEUPB_Y 380
#define SM_UPDOWNB_X 91
#define SM_UPDOWNB_Y 413
#define SM_CLIMBB_X 187
#define SM_CLIMBB_Y 348
#define SM_STANCEDOWNB_X 187
#define SM_STANCEDOWNB_Y 448
#define SM_HANDCURSORB_X 123
#define SM_HANDCURSORB_Y 413
#define SM_PREVMERCB_X 9
#define SM_PREVMERCB_Y 410
#define SM_NEXTMERCB_X 51
#define SM_NEXTMERCB_Y 410
#define SM_OPTIONSB_X 9
#define SM_OPTIONSB_Y 445
#define SM_BURSTMODEB_X 155
#define SM_BURSTMODEB_Y 413
#define SM_LOOKB_X 123
#define SM_LOOKB_Y 448
#define SM_STEALTHMODE_X 187
#define SM_STEALTHMODE_Y 413
#define SM_DONE_X 543
#define SM_DONE_Y 344
#define SM_MAPSCREEN_X 589
#define SM_MAPSCREEN_Y 344

#define SM_POSITIONB_X 106 + INTERFACE_START_X
#define SM_POSITIONB_Y 34 + INV_INTERFACE_START_Y
#define SM_POSITIONB_WIDTH 19
#define SM_POSITIONB_HEIGHT 24

#define SM_PERCENT_WIDTH 20
#define SM_PERCENT_HEIGHT 10
#define SM_ARMOR_X 347
#define SM_ARMOR_Y 419
#define SM_ARMOR_LABEL_X 363
#define SM_ARMOR_LABEL_Y 409
#define SM_ARMOR_PERCENT_X 368
#define SM_ARMOR_PERCENT_Y 419

#define SM_WEIGHT_LABEL_X 430
#define SM_WEIGHT_LABEL_Y 447
#define SM_WEIGHT_PERCENT_X 449
#define SM_WEIGHT_PERCENT_Y 446
#define SM_WEIGHT_X 428
#define SM_WEIGHT_Y 446

#define SM_CAMMO_LABEL_X 430
#define SM_CAMMO_LABEL_Y 462
#define SM_CAMMO_PERCENT_X 449
#define SM_CAMMO_PERCENT_Y 461
#define SM_CAMMO_X 428
#define SM_CAMMO_Y 461

#define SM_STATS_WIDTH 30
#define SM_STATS_HEIGHT 8
#define SM_AGI_X 99 + 2
#define SM_AGI_Y 347
#define SM_DEX_X 99 + 2
#define SM_DEX_Y 357
#define SM_STR_X 99 + 2
#define SM_STR_Y 367
#define SM_CHAR_X 99 + 2
#define SM_CHAR_Y 377
#define SM_WIS_X 99 + 2
#define SM_WIS_Y 387
#define SM_EXPLVL_X 148
#define SM_EXPLVL_Y 347
#define SM_MRKM_X 148
#define SM_MRKM_Y 357
#define SM_EXPL_X 148
#define SM_EXPL_Y 367
#define SM_MECH_X 148
#define SM_MECH_Y 377
#define SM_MED_X 148
#define SM_MED_Y 387

#define MONEY_X 460
#define MONEY_Y 445
#define MONEY_WIDTH 30
#define MONEY_HEIGHT 22

#define TM_FACE_WIDTH 48
#define TM_FACE_HEIGHT 43

#define TM_APPANEL_HEIGHT 56
#define TM_APPANEL_WIDTH 16

#define TM_ENDTURN_X 507
#define TM_ENDTURN_Y (9 + INTERFACE_START_Y)
#define TM_ROSTERMODE_X 507
#define TM_ROSTERMODE_Y (45 + INTERFACE_START_Y)
#define TM_DISK_X 507
#define TM_DISK_Y (81 + INTERFACE_START_Y)

#define TM_NAME_WIDTH 60
#define TM_NAME_HEIGHT 9
#define TM_LIFEBAR_WIDTH 3
#define TM_LIFEBAR_HEIGHT 42
#define TM_FACEHIGHTL_WIDTH 84
#define TM_FACEHIGHTL_HEIGHT 114
#define TM_AP_HEIGHT 10
#define TM_AP_WIDTH 15

#define TM_INV_WIDTH 58
#define TM_INV_HEIGHT 23
#define TM_INV_HAND1STARTX 8
#define TM_INV_HAND1STARTY (67 + INTERFACE_START_Y)
#define TM_INV_HAND2STARTX 8
#define TM_INV_HAND2STARTY (93 + INTERFACE_START_Y)
#define TM_INV_HAND_SEP 83
#define TM_INV_HAND_SEPY 24

#define TM_BARS_REGION_HEIGHT 47
#define TM_BARS_REGION_WIDTH 26

#define INDICATOR_BOX_WIDTH 12
#define INDICATOR_BOX_HEIGHT 10

extern INV_REGION_DESC gSMInvPocketXY[];

extern INV_REGION_DESC gSMCamoXY;

extern INT16 sTEAMAPPanelXY[];
extern INT16 sTEAMFacesXY[];
extern INT16 sTEAMNamesXY[];
extern INT16 sTEAMFaceHighlXY[];
extern INT16 sTEAMLifeXY[];
extern INT16 sTEAMBreathXY[];
extern INT16 sTEAMMoraleXY[];
extern INT16 sTEAMApXY[];
extern INT16 sTEAMBarsXY[];
extern INT16 sTEAMHandInvXY[];

enum {
  STANCEUP_BUTTON = 0,
  UPDOWN_BUTTON,
  CLIMB_BUTTON,
  STANCEDOWN_BUTTON,
  HANDCURSOR_BUTTON,
  PREVMERC_BUTTON,
  NEXTMERC_BUTTON,
  OPTIONS_BUTTON,
  BURSTMODE_BUTTON,
  LOOK_BUTTON,
  TALK_BUTTON,
  MUTE_BUTTON,
  SM_DONE_BUTTON,
  SM_MAP_SCREEN_BUTTON,
  NUM_SM_BUTTONS
};

enum { TEAM_DONE_BUTTON = 0, TEAM_MAP_SCREEN_BUTTON, CHANGE_SQUAD_BUTTON, NUM_TEAM_BUTTONS };

#define NEW_ITEM_CYCLE_COUNT 19
#define NEW_ITEM_CYCLES 4
#define NUM_TEAM_SLOTS 6

#define PASSING_ITEM_DISTANCE_OKLIFE 3
#define PASSING_ITEM_DISTANCE_NOTOKLIFE 2

#define SHOW_LOCATOR_NORMAL 1
#define SHOW_LOCATOR_FAST 2

BOOLEAN CreateSMPanelButtons();
void RemoveSMPanelButtons();
BOOLEAN InitializeSMPanel();
BOOLEAN ShutdownSMPanel();
void RenderSMPanel(BOOLEAN *pfDirty);
void EnableSMPanelButtons(BOOLEAN fEnable, BOOLEAN fFromItemPickup);

BOOLEAN CreateTEAMPanelButtons();
void RemoveTEAMPanelButtons();
BOOLEAN InitializeTEAMPanel();
BOOLEAN ShutdownTEAMPanel();
void RenderTEAMPanel(BOOLEAN fDirty);
void UpdateTEAMPanel();

void SetSMPanelCurrentMerc(UINT8 ubNewID);
void SetTEAMPanelCurrentMerc(UINT8 ubNewID);
UINT16 GetSMPanelCurrentMerc();
void UpdateSMPanel();

BOOLEAN InitTEAMSlots();
void AddPlayerToInterfaceTeamSlot(UINT8 ubID);
BOOLEAN RemovePlayerFromInterfaceTeamSlot(UINT8 ubID);
BOOLEAN GetPlayerIDFromInterfaceTeamSlot(UINT8 ubPanelSlot, UINT8 *pubID);
void RemoveAllPlayersFromSlot();
BOOLEAN PlayerExistsInSlot(UINT8 ubID);
BOOLEAN RemovePlayerFromTeamSlotGivenMercID(UINT8 ubMercID);
void CheckForAndAddMercToTeamPanel(SOLDIERCLASS *pSoldier);

void DisableTacticalTeamPanelButtons(BOOLEAN fDisable);
void RenderTownIDString();
void KeyRingSlotInvClickCallback(MOUSE_REGION *pRegion, INT32 iReason);

// ATE TO BE MOVED TO INTERFACE_ITEMS.C
// extern INT8						 gbNewItem[ NUM_INV_SLOTS ] ;
extern INT8 gbNewItemCycle[NUM_INV_SLOTS];
extern UINT8 gubNewItemMerc;

void ShowRadioLocator(UINT8 ubID, UINT8 ubLocatorSpeed);
void EndRadioLocator(UINT8 ubID);

extern MOUSE_REGION gSMPanelRegion;

extern UINT32 guiSecItemHiddenVO;

extern BOOLEAN gfDisableTacticalPanelButtons;

typedef struct {
  UINT8 ubID;
  BOOLEAN fOccupied;

} TEAM_PANEL_SLOTS_TYPE;

extern TEAM_PANEL_SLOTS_TYPE gTeamPanel[NUM_TEAM_SLOTS];

// Used when the shop keeper interface is active
void DisableSMPpanelButtonsWhenInShopKeeperInterface(BOOLEAN fDontDrawButtons);
// void DisableSMPpanelButtonsWhenInShopKeeperInterface( );

//
void ReEvaluateDisabledINVPanelButtons();
void CheckForReEvaluateDisabledINVPanelButtons();

void CheckForDisabledForGiveItem();
void ReevaluateItemHatches(SOLDIERCLASS *pSoldier, BOOLEAN fEnable);

void HandlePanelFaceAnimations(SOLDIERCLASS *pSoldier);

void GoToMapScreenFromTactical(void);

void HandleTacticalEffectsOfEquipmentChange(SOLDIERCLASS *pSoldier, UINT32 uiInvPos,
                                            UINT16 usOldItem, UINT16 usNewItem);

void FinishAnySkullPanelAnimations();

UINT8 FindNextMercInTeamPanel(SOLDIERCLASS *pSoldier, BOOLEAN fGoodForLessOKLife,
                              BOOLEAN fOnlyRegularMercs);

#endif
