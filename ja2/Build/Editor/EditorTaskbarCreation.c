#include "Editor/EditorAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "BuildDefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
// sgp
#include "SGP/ButtonSystem.h"
#include "Utils/FontControl.h"
#include "SGP/Debug.h"
// editor
#include "Editor/EditorDefines.h"
#include "Editor/EditorCallbackPrototypes.h"
#include "Editor/EditorTaskbarUtils.h"
#include "Editor/EditorMercs.h"
#include "Editor/EditorMapInfo.h"
// tactical
#include "Tactical/SoldierCreate.h"
#include "Tactical/OverheadTypes.h"
#endif

// Category tabs of the editor buttons
void InitEditorTerrainToolbar();
void InitEditorBuildingsToolbar();
void InitEditorItemsToolbar();
void InitEditorMercsToolbar();
void InitEditorMapInfoToolbar();
void InitEditorOptionsToolbar();
void InitEditorItemStatsButtons();

void InitEditorItemStatsButtons() {
  iEditorButton[ITEMSTATS_PANEL] =
      CreateTextButton(0, 0, 0, 0, BUTTON_USE_DEFAULT, 480, giScrH - 480 + 361, 160, 99,
                       BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK);
  SpecifyDisabledButtonStyle(iEditorButton[ITEMSTATS_PANEL], DISABLED_STYLE_NONE);
  DisableButton(iEditorButton[ITEMSTATS_PANEL]);
  iEditorButton[ITEMSTATS_HIDDEN_BTN] =
      CreateCheckBoxButton(485, giScrH - 480 + 365, "EDITOR\\SmCheckbox.sti", MSYS_PRIORITY_NORMAL,
                           ItemStatsToggleHideCallback);
  iEditorButton[ITEMSTATS_DELETE_BTN] =
      CreateTextButton(L"Delete", FONT10ARIAL, FONT_RED, FONT_BLACK, BUTTON_USE_DEFAULT, 600 - 72,
                       giScrH - 480 + 441, 36, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL + 1,
                       DEFAULT_MOVE_CALLBACK, ItemStatsDeleteCallback);
}

void InitEditorMercsToolbar() {
  CHAR16 TempString[30];
  CHAR16 *FaceDirs[8] = {L"north", L"northeast", L"east", L"southeast",
                         L"south", L"southwest", L"west", L"northwest"};
  INT32 x;

  iEditorButton[MERCS_PLAYERTOGGLE] = CreateCheckBoxButton(
      4, giScrH - 480 + 362, "EDITOR\\SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsTogglePlayers);
  if (gfShowPlayers) ClickEditorButton(MERCS_PLAYERTOGGLE);
  SetButtonFastHelpText(iEditorButton[MERCS_PLAYERTOGGLE], L"Toggle viewing of players");
  DisableButton(iEditorButton[MERCS_PLAYERTOGGLE]);
  iEditorButton[MERCS_ENEMYTOGGLE] = CreateCheckBoxButton(
      4, giScrH - 480 + 382, "EDITOR\\SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleEnemies);
  if (gfShowEnemies) ClickEditorButton(MERCS_ENEMYTOGGLE);
  SetButtonFastHelpText(iEditorButton[MERCS_ENEMYTOGGLE], L"Toggle viewing of enemies");
  iEditorButton[MERCS_CREATURETOGGLE] = CreateCheckBoxButton(
      4, giScrH - 480 + 402, "EDITOR\\SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleCreatures);
  if (gfShowCreatures) ClickEditorButton(MERCS_CREATURETOGGLE);
  SetButtonFastHelpText(iEditorButton[MERCS_CREATURETOGGLE], L"Toggle viewing of creatures");
  iEditorButton[MERCS_REBELTOGGLE] = CreateCheckBoxButton(
      4, giScrH - 480 + 422, "EDITOR\\SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleRebels);
  if (gfShowRebels) ClickEditorButton(MERCS_REBELTOGGLE);
  SetButtonFastHelpText(iEditorButton[MERCS_REBELTOGGLE], L"Toggle viewing of rebels");
  iEditorButton[MERCS_CIVILIANTOGGLE] = CreateCheckBoxButton(
      4, giScrH - 480 + 442, "EDITOR\\SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleCivilians);
  if (gfShowCivilians) ClickEditorButton(MERCS_CIVILIANTOGGLE);
  SetButtonFastHelpText(iEditorButton[MERCS_CIVILIANTOGGLE], L"Toggle viewing of civilians");
  iEditorButton[MERCS_PLAYER] = CreateTextButton(
      L"Player", (UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT, 20, giScrH - 480 + 362, 78,
      19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsPlayerTeamCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_PLAYER], FONT_YELLOW, FONT_BLACK);
  DisableButton(iEditorButton[MERCS_PLAYER]);
  iEditorButton[MERCS_ENEMY] = CreateTextButton(
      L"Enemy", (UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT, 20, giScrH - 480 + 382, 78,
      19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsEnemyTeamCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_ENEMY], FONT_YELLOW, FONT_BLACK);
  iEditorButton[MERCS_CREATURE] =
      CreateTextButton(L"Creature", (UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT, 20,
                       giScrH - 480 + 402, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsCreatureTeamCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_CREATURE], FONT_YELLOW, FONT_BLACK);
  iEditorButton[MERCS_REBEL] = CreateTextButton(
      L"Rebels", (UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT, 20, giScrH - 480 + 422, 78,
      19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsRebelTeamCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_REBEL], FONT_YELLOW, FONT_BLACK);
  iEditorButton[MERCS_CIVILIAN] =
      CreateTextButton(L"Civilian", (UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT, 20,
                       giScrH - 480 + 442, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsCivilianTeamCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_CIVILIAN], FONT_YELLOW, FONT_BLACK);

  iEditorButton[MERCS_1] =
      CreateTextButton(L"DETAILED PLACEMENT", SMALLCOMPFONT, FONT_ORANGE, 60, BUTTON_USE_DEFAULT,
                       100, giScrH - 480 + 362, 68, 20, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK);
  DisableButton(iEditorButton[MERCS_1]);
  SpecifyDisabledButtonStyle(iEditorButton[MERCS_1], DISABLED_STYLE_NONE);
  SpecifyButtonTextOffsets(iEditorButton[MERCS_1], 20, 4, FALSE);
  SpecifyButtonTextWrappedWidth(iEditorButton[MERCS_1], 46);
  iEditorButton[MERCS_DETAILEDCHECKBOX] =
      CreateCheckBoxButton(103, giScrH - 480 + 365, "EDITOR\\checkbox.sti", MSYS_PRIORITY_NORMAL,
                           MercsDetailedPlacementCallback);

  iEditorButton[MERCS_GENERAL] = CreateEasyToggleButton(
      100, giScrH - 480 + 382, "EDITOR\\MercGeneral.sti", MercsGeneralModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_GENERAL], L"General information mode");
  iEditorButton[MERCS_APPEARANCE] = CreateEasyToggleButton(
      134, giScrH - 480 + 382, "EDITOR\\MercAppearance.sti", MercsAppearanceModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_APPEARANCE], L"Physical appearance mode");
  iEditorButton[MERCS_ATTRIBUTES] = CreateEasyToggleButton(
      100, giScrH - 480 + 408, "EDITOR\\MercAttributes.sti", MercsAttributesModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_ATTRIBUTES], L"Attributes mode");
  iEditorButton[MERCS_INVENTORY] = CreateEasyToggleButton(
      134, giScrH - 480 + 408, "EDITOR\\MercInventory.sti", MercsInventoryModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_INVENTORY], L"Inventory mode");
  iEditorButton[MERCS_PROFILE] = CreateEasyToggleButton(
      100, giScrH - 480 + 434, "EDITOR\\MercProfile.sti", MercsProfileModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_PROFILE], L"Profile ID mode");
  iEditorButton[MERCS_SCHEDULE] = CreateEasyToggleButton(
      134, giScrH - 480 + 434, "EDITOR\\MercSchedule.sti", MercsScheduleModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_SCHEDULE], L"Schedule mode");
  // Workaround for identical buttons.
  MSYS_SetBtnUserData(iEditorButton[MERCS_SCHEDULE], 3, 0xffffffff);
  iEditorButton[MERCS_GLOWSCHEDULE] = CreateEasyToggleButton(
      134, giScrH - 480 + 434, "EDITOR\\MercGlowSchedule.sti", MercsScheduleModeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_GLOWSCHEDULE], L"Schedule mode");
  HideEditorButton(MERCS_GLOWSCHEDULE);

  iEditorButton[MERCS_DELETE] =
      CreateTextButton(L"DELETE", (UINT16)SMALLCOMPFONT, FONT_DKBLUE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 600, giScrH - 480 + 362, 40, 20, BUTTON_NO_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsDeleteCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_DELETE], L"Delete currently selected merc (DEL).");
  iEditorButton[MERCS_NEXT] =
      CreateTextButton(L"NEXT", (UINT16)SMALLCOMPFONT, FONT_DKBLUE, FONT_BLACK, BUTTON_USE_DEFAULT,
                       600, giScrH - 480 + 382, 40, 20, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsNextCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_NEXT], L"Find next merc (SPACE).");

  // Priority Existance
  iEditorButton[MERCS_PRIORITYEXISTANCE_CHECKBOX] =
      CreateCheckBoxButton(170, giScrH - 480 + 365, "EDITOR\\checkbox.sti", MSYS_PRIORITY_NORMAL,
                           MercsPriorityExistanceCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_PRIORITYEXISTANCE_CHECKBOX],
                        L"Toggle priority existance");

  // If merc has keys
  iEditorButton[MERCS_HASKEYS_CHECKBOX] = CreateCheckBoxButton(
      170, giScrH - 480 + 390, "EDITOR\\checkbox.sti", MSYS_PRIORITY_NORMAL, MercsHasKeysCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_HASKEYS_CHECKBOX],
                        L"Toggle whether or not placement has/naccess to all doors.");

  // Orders
  iEditorButton[MERCS_ORDERS_STATIONARY] =
      CreateTextButton(L"STATIONARY", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 368, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_ONGUARD] =
      CreateTextButton(L"ON GUARD", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 380, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_ONCALL] =
      CreateTextButton(L"ON CALL", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 392, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_SEEKENEMY] =
      CreateTextButton(L"SEEK ENEMY", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 404, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_CLOSEPATROL] =
      CreateTextButton(L"CLOSE PATROL", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 368, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_FARPATROL] =
      CreateTextButton(L"FAR PATROL", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 380, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_POINTPATROL] =
      CreateTextButton(L"POINT PATROL", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 392, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  iEditorButton[MERCS_ORDERS_RNDPTPATROL] =
      CreateTextButton(L"RND PT PATROL", (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 404, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetOrdersCallback);
  for (x = 0; x < 8; x++) {
    MSYS_SetBtnUserData(iEditorButton[FIRST_MERCS_ORDERS_BUTTON + x], 0, x);
  }

  // Attitudes
  iEditorButton[MERCS_ATTITUDE_DEFENSIVE] =
      CreateTextButton(L"DEFENSIVE", (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 424, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetAttitudeCallback);
  iEditorButton[MERCS_ATTITUDE_BRAVESOLO] =
      CreateTextButton(L"BRAVE SOLO", (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 436, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetAttitudeCallback);
  iEditorButton[MERCS_ATTITUDE_BRAVEAID] =
      CreateTextButton(L"BRAVE AID", (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 200, giScrH - 480 + 448, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetAttitudeCallback);
  iEditorButton[MERCS_ATTITUDE_AGGRESSIVE] =
      CreateTextButton(L"AGGRESSIVE", (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 424, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetAttitudeCallback);
  iEditorButton[MERCS_ATTITUDE_CUNNINGSOLO] =
      CreateTextButton(L"CUNNING SOLO", (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 436, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetAttitudeCallback);
  iEditorButton[MERCS_ATTITUDE_CUNNINGAID] =
      CreateTextButton(L"CUNNING AID", (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 270, giScrH - 480 + 448, 70, 12, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetAttitudeCallback);
  for (x = 0; x < 6; x++) {
    MSYS_SetBtnUserData(iEditorButton[FIRST_MERCS_ATTITUDE_BUTTON + x], 0, x);
  }

  iEditorButton[MERCS_DIRECTION_W] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 7, BUTTON_USE_DEFAULT, 360, giScrH - 480 + 365, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_NW] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 0, BUTTON_USE_DEFAULT, 390, giScrH - 480 + 365, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_N] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 1, BUTTON_USE_DEFAULT, 420, giScrH - 480 + 365, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_NE] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 2, BUTTON_USE_DEFAULT, 420, giScrH - 480 + 395, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_E] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 3, BUTTON_USE_DEFAULT, 420, giScrH - 480 + 425, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_SE] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 4, BUTTON_USE_DEFAULT, 390, giScrH - 480 + 425, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_S] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 5, BUTTON_USE_DEFAULT, 360, giScrH - 480 + 425, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  iEditorButton[MERCS_DIRECTION_SW] = CreateIconButton(
      (INT16)giEditMercDirectionIcons[0], 6, BUTTON_USE_DEFAULT, 360, giScrH - 480 + 395, 30, 30,
      BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback);
  for (x = 0; x < 8; x++) {
    swprintf(TempString, L"Set merc to face %s", FaceDirs[x]);
    SetButtonFastHelpText(iEditorButton[FIRST_MERCS_DIRECTION_BUTTON + x], TempString);
    MSYS_SetBtnUserData(iEditorButton[FIRST_MERCS_DIRECTION_BUTTON + x], 0, x);
  }

  iEditorButton[MERCS_DIRECTION_FIND] = CreateTextButton(
      L"Find", (INT16)FONT12POINT1, FONT_MCOLOR_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT, 390,
      giScrH - 480 + 395, 30, 30, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK,
      MercsFindSelectedMercCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_DIRECTION_FIND], L"Find selected merc");

  iEditorButton[MERCS_EQUIPMENT_BAD] =
      CreateTextButton(L"BAD", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       480, giScrH - 480 + 385, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_EQUIPMENT_BAD], FONT_LTRED, FONT_BLACK);
  iEditorButton[MERCS_EQUIPMENT_POOR] =
      CreateTextButton(L"POOR", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       480, giScrH - 480 + 400, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_EQUIPMENT_POOR], FONT_ORANGE, FONT_BLACK);
  iEditorButton[MERCS_EQUIPMENT_AVERAGE] =
      CreateTextButton(L"AVERAGE", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       480, giScrH - 480 + 415, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_EQUIPMENT_AVERAGE], FONT_YELLOW, FONT_BLACK);
  iEditorButton[MERCS_EQUIPMENT_GOOD] =
      CreateTextButton(L"GOOD", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       480, giScrH - 480 + 430, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_EQUIPMENT_GOOD], FONT_LTGREEN, FONT_BLACK);
  iEditorButton[MERCS_EQUIPMENT_GREAT] =
      CreateTextButton(L"GREAT", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       480, giScrH - 480 + 445, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_EQUIPMENT_GREAT], FONT_LTBLUE, FONT_BLACK);
  for (x = 0; x < 5; x++) {
    MSYS_SetBtnUserData(iEditorButton[FIRST_MERCS_REL_EQUIPMENT_BUTTON + x], 0, x);
  }

  iEditorButton[MERCS_ATTRIBUTES_BAD] =
      CreateTextButton(L"BAD", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       530, giScrH - 480 + 385, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_ATTRIBUTES_BAD], FONT_LTRED, FONT_BLACK);
  iEditorButton[MERCS_ATTRIBUTES_POOR] =
      CreateTextButton(L"POOR", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       530, giScrH - 480 + 400, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_ATTRIBUTES_POOR], FONT_ORANGE, FONT_BLACK);
  iEditorButton[MERCS_ATTRIBUTES_AVERAGE] =
      CreateTextButton(L"AVERAGE", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       530, giScrH - 480 + 415, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_ATTRIBUTES_AVERAGE], FONT_YELLOW, FONT_BLACK);
  iEditorButton[MERCS_ATTRIBUTES_GOOD] =
      CreateTextButton(L"GOOD", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       530, giScrH - 480 + 430, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_ATTRIBUTES_GOOD], FONT_LTGREEN, FONT_BLACK);
  iEditorButton[MERCS_ATTRIBUTES_GREAT] =
      CreateTextButton(L"GREAT", (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
                       530, giScrH - 480 + 445, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback);
  SpecifyButtonDownTextColors(iEditorButton[MERCS_ATTRIBUTES_GREAT], FONT_LTBLUE, FONT_BLACK);
  for (x = 0; x < 5; x++)
    MSYS_SetBtnUserData(iEditorButton[FIRST_MERCS_REL_ATTRIBUTE_BUTTON + x], 0, x);

  iEditorButton[MERCS_ARMY_CODE] =
      CreateCheckBoxButton(575, giScrH - 480 + 410, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MercsSetEnemyColorCodeCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_ARMY_CODE], 0, SOLDIER_CLASS_ARMY);
  iEditorButton[MERCS_ADMIN_CODE] =
      CreateCheckBoxButton(575, giScrH - 480 + 424, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MercsSetEnemyColorCodeCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_ADMIN_CODE], 0, SOLDIER_CLASS_ADMINISTRATOR);
  iEditorButton[MERCS_ELITE_CODE] =
      CreateCheckBoxButton(575, giScrH - 480 + 438, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MercsSetEnemyColorCodeCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_ELITE_CODE], 0, SOLDIER_CLASS_ELITE);

  iEditorButton[MERCS_CIVILIAN_GROUP] =
      CreateTextButton(gszCivGroupNames[0], (INT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 574, giScrH - 480 + 410, 60, 25, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsCivilianGroupCallback);

  iEditorButton[MERCS_TOGGLECOLOR_BUTTON] =
      CreateCheckBoxButton(180, giScrH - 480 + 364, "EDITOR\\checkbox.sti", MSYS_PRIORITY_NORMAL,
                           MercsToggleColorModeCallback);
  iEditorButton[MERCS_HAIRCOLOR_DOWN] = CreateEasyNoToggleButton(
      200, giScrH - 480 + 364, "EDITOR\\leftarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_HAIRCOLOR_UP] = CreateEasyNoToggleButton(
      360, giScrH - 480 + 364, "EDITOR\\rightarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_SKINCOLOR_DOWN] = CreateEasyNoToggleButton(
      200, giScrH - 480 + 388, "EDITOR\\leftarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_SKINCOLOR_UP] = CreateEasyNoToggleButton(
      360, giScrH - 480 + 388, "EDITOR\\rightarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_VESTCOLOR_DOWN] = CreateEasyNoToggleButton(
      200, giScrH - 480 + 412, "EDITOR\\leftarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_VESTCOLOR_UP] = CreateEasyNoToggleButton(
      360, giScrH - 480 + 412, "EDITOR\\rightarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_PANTCOLOR_DOWN] = CreateEasyNoToggleButton(
      200, giScrH - 480 + 436, "EDITOR\\leftarrow.sti", MercsSetColorsCallback);
  iEditorButton[MERCS_PANTCOLOR_UP] = CreateEasyNoToggleButton(
      360, giScrH - 480 + 436, "EDITOR\\rightarrow.sti", MercsSetColorsCallback);
  for (x = FIRST_MERCS_COLOR_BUTTON; x < LAST_MERCS_COLOR_BUTTON; x += 2) {
    SetButtonFastHelpText(iEditorButton[x], L"Previous color set");
    SetButtonFastHelpText(iEditorButton[x + 1], L"Next color set");
    DisableButton(iEditorButton[x]);
    DisableButton(iEditorButton[x + 1]);
  }

  iEditorButton[MERCS_BODYTYPE_DOWN] = CreateEasyNoToggleButton(
      460, giScrH - 480 + 364, "EDITOR\\leftarrow.sti", MercsSetBodyTypeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_BODYTYPE_DOWN], L"Previous body type");
  iEditorButton[MERCS_BODYTYPE_UP] = CreateEasyNoToggleButton(
      560, giScrH - 480 + 364, "EDITOR\\rightarrow.sti", MercsSetBodyTypeCallback);
  SetButtonFastHelpText(iEditorButton[MERCS_BODYTYPE_UP], L"Next body type");

  iEditorButton[MERCS_SCHEDULE_VARIANCE1] =
      CreateCheckBoxButton(309, giScrH - 480 + 375, "EDITOR\\SmCheckBox.sti", MSYS_PRIORITY_NORMAL,
                           MercsScheduleToggleVariance1Callback);
  SetButtonFastHelpText(iEditorButton[MERCS_SCHEDULE_VARIANCE1],
                        L"Toggle time variance (+ or - 15 minutes)");
  iEditorButton[MERCS_SCHEDULE_VARIANCE2] =
      CreateCheckBoxButton(309, giScrH - 480 + 396, "EDITOR\\SmCheckBox.sti", MSYS_PRIORITY_NORMAL,
                           MercsScheduleToggleVariance2Callback);
  SetButtonFastHelpText(iEditorButton[MERCS_SCHEDULE_VARIANCE2],
                        L"Toggle time variance (+ or - 15 minutes)");
  iEditorButton[MERCS_SCHEDULE_VARIANCE3] =
      CreateCheckBoxButton(309, giScrH - 480 + 417, "EDITOR\\SmCheckBox.sti", MSYS_PRIORITY_NORMAL,
                           MercsScheduleToggleVariance3Callback);
  SetButtonFastHelpText(iEditorButton[MERCS_SCHEDULE_VARIANCE3],
                        L"Toggle time variance (+ or - 15 minutes)");
  iEditorButton[MERCS_SCHEDULE_VARIANCE4] =
      CreateCheckBoxButton(309, giScrH - 480 + 438, "EDITOR\\SmCheckBox.sti", MSYS_PRIORITY_NORMAL,
                           MercsScheduleToggleVariance4Callback);
  SetButtonFastHelpText(iEditorButton[MERCS_SCHEDULE_VARIANCE4],
                        L"Toggle time variance (+ or - 15 minutes)");

  iEditorButton[MERCS_SCHEDULE_ACTION1] =
      CreateTextButton(L"No action", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 186,
                       giScrH - 480 + 373, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleAction1Callback);
  iEditorButton[MERCS_SCHEDULE_ACTION2] =
      CreateTextButton(L"No action", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 186,
                       giScrH - 480 + 394, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleAction2Callback);
  iEditorButton[MERCS_SCHEDULE_ACTION3] =
      CreateTextButton(L"No action", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 186,
                       giScrH - 480 + 415, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleAction3Callback);
  iEditorButton[MERCS_SCHEDULE_ACTION4] =
      CreateTextButton(L"No action", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 186,
                       giScrH - 480 + 436, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleAction4Callback);

  iEditorButton[MERCS_SCHEDULE_DATA1A] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 331,
                       giScrH - 480 + 373, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData1ACallback);
  iEditorButton[MERCS_SCHEDULE_DATA1B] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 381,
                       giScrH - 480 + 373, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData1BCallback);
  iEditorButton[MERCS_SCHEDULE_DATA2A] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 331,
                       giScrH - 480 + 394, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData2ACallback);
  iEditorButton[MERCS_SCHEDULE_DATA2B] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 381,
                       giScrH - 480 + 394, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData2BCallback);
  iEditorButton[MERCS_SCHEDULE_DATA3A] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 331,
                       giScrH - 480 + 415, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData3ACallback);
  iEditorButton[MERCS_SCHEDULE_DATA3B] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 381,
                       giScrH - 480 + 415, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData3BCallback);
  iEditorButton[MERCS_SCHEDULE_DATA4A] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 331,
                       giScrH - 480 + 436, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData4ACallback);
  iEditorButton[MERCS_SCHEDULE_DATA4B] =
      CreateTextButton(L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, 381,
                       giScrH - 480 + 436, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleData4BCallback);
  iEditorButton[MERCS_SCHEDULE_CLEAR] =
      CreateTextButton(L"Clear Schedule", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
                       516, giScrH - 480 + 362, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL,
                       DEFAULT_MOVE_CALLBACK, MercsScheduleClearCallback);
  HideEditorButtons(MERCS_SCHEDULE_DATA1A, MERCS_SCHEDULE_DATA4B);

  iEditorButton[MERCS_HEAD_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 61, MERCPANEL_Y + 0, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_HEAD_SLOT], 0, HELMETPOS);
  iEditorButton[MERCS_BODY_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 61, MERCPANEL_Y + 22, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_BODY_SLOT], 0, VESTPOS);
  iEditorButton[MERCS_LEGS_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 62, MERCPANEL_Y + 73, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_LEGS_SLOT], 0, LEGPOS);
  iEditorButton[MERCS_LEFTHAND_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 12, MERCPANEL_Y + 43, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_LEFTHAND_SLOT], 0, HANDPOS);
  iEditorButton[MERCS_RIGHTHAND_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 90, MERCPANEL_Y + 42, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_RIGHTHAND_SLOT], 0, SECONDHANDPOS);
  iEditorButton[MERCS_PACK1_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 166, MERCPANEL_Y + 6, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_PACK1_SLOT], 0, BIGPOCK1POS);
  iEditorButton[MERCS_PACK2_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 166, MERCPANEL_Y + 29, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_PACK2_SLOT], 0, BIGPOCK2POS);
  iEditorButton[MERCS_PACK3_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 166, MERCPANEL_Y + 52, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_PACK3_SLOT], 0, BIGPOCK3POS);
  iEditorButton[MERCS_PACK4_SLOT] =
      CreateCheckBoxButton(MERCPANEL_X + 166, MERCPANEL_Y + 75, "EDITOR\\smCheckbox.sti",
                           MSYS_PRIORITY_NORMAL + 1, MercsInventorySlotCallback);
  MSYS_SetBtnUserData(iEditorButton[MERCS_PACK4_SLOT], 0, BIGPOCK4POS);
}

void InitEditorBuildingsToolbar() {
  iEditorButton[BUILDING_TOGGLE_ROOF_VIEW] =
      CreateTextButton(L"ROOFS", (UINT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
                       110, giScrH - 480 + 400, 50, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       BUTTON_NO_CALLBACK, BuildingToggleRoofViewCallback);
  iEditorButton[BUILDING_TOGGLE_WALL_VIEW] =
      CreateTextButton(L"WALLS", (UINT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
                       110, giScrH - 480 + 415, 50, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL,
                       BUTTON_NO_CALLBACK, BuildingToggleWallViewCallback);
  iEditorButton[BUILDING_TOGGLE_INFO_VIEW] =
      CreateTextButton(L"ROOM INFO", (UINT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 110, giScrH - 480 + 430, 50, 15, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BuildingToggleInfoViewCallback);
  // Selection method buttons
  iEditorButton[BUILDING_PLACE_WALLS] =
      CreateEasyToggleButton(180, giScrH - 480 + 370, "EDITOR\\wall.sti", BuildingWallCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_WALLS], L"Place walls using selection method");

  // SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_WALLS],
  //	L"Place walls using selection method\nPlace walls using selection method\nPlace walls using
  // selection method\nPlace walls using selection method\nPlace walls using selection method\nPlace
  // walls using selection method\nPlace walls using selection method\nPlace walls using selection
  // method\nPlace walls using selection method\nPlace walls using selection method\nPlace walls
  // using selection method\nPlace walls using selection method\nPlace walls using selection
  // method\nPlace walls using selection method\n");

  iEditorButton[BUILDING_PLACE_DOORS] =
      CreateEasyToggleButton(210, giScrH - 480 + 370, "EDITOR\\door.sti", BuildingDoorCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_DOORS], L"Place doors using selection method");
  iEditorButton[BUILDING_PLACE_ROOFS] =
      CreateEasyToggleButton(240, giScrH - 480 + 370, "EDITOR\\roof.sti", BuildingRoofCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_ROOFS], L"Place roofs using selection method");
  iEditorButton[BUILDING_PLACE_WINDOWS] =
      CreateEasyToggleButton(180, giScrH - 480 + 400, "EDITOR\\window.sti", BuildingWindowCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_WINDOWS],
                        L"Place windows using selection method");
  iEditorButton[BUILDING_PLACE_BROKEN_WALLS] = CreateEasyToggleButton(
      210, giScrH - 480 + 400, "EDITOR\\crackwall.sti", BuildingCrackWallCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_BROKEN_WALLS],
                        L"Place damaged walls using selection method.");
  iEditorButton[BUILDING_PLACE_FURNITURE] = CreateEasyToggleButton(
      240, giScrH - 480 + 400, "EDITOR\\decor.sti", BuildingFurnitureCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_FURNITURE],
                        L"Place furniture using selection method");
  iEditorButton[BUILDING_PLACE_DECALS] =
      CreateEasyToggleButton(180, giScrH - 480 + 430, "EDITOR\\decal.sti", BuildingDecalCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_DECALS],
                        L"Place wall decals using selection method");
  iEditorButton[BUILDING_PLACE_FLOORS] =
      CreateEasyToggleButton(210, giScrH - 480 + 430, "EDITOR\\floor.sti", BuildingFloorCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_FLOORS],
                        L"Place floors using selection method");
  iEditorButton[BUILDING_PLACE_TOILETS] =
      CreateEasyToggleButton(240, giScrH - 480 + 430, "EDITOR\\toilet.sti", BuildingToiletCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_TOILETS],
                        L"Place generic furniture using selection method");

  // Smart method buttons
  iEditorButton[BUILDING_SMART_WALLS] = CreateEasyToggleButton(
      290, giScrH - 480 + 370, "EDITOR\\wall.sti", BuildingSmartWallCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_SMART_WALLS], L"Place walls using smart method");
  iEditorButton[BUILDING_SMART_DOORS] = CreateEasyToggleButton(
      320, giScrH - 480 + 370, "EDITOR\\door.sti", BuildingSmartDoorCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_SMART_DOORS], L"Place doors using smart method");
  iEditorButton[BUILDING_SMART_WINDOWS] = CreateEasyToggleButton(
      290, giScrH - 480 + 400, "EDITOR\\window.sti", BuildingSmartWindowCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_SMART_WINDOWS], L"Place windows using smart method");
  iEditorButton[BUILDING_SMART_BROKEN_WALLS] = CreateEasyToggleButton(
      320, giScrH - 480 + 400, "EDITOR\\crackwall.sti", BuildingSmartCrackWallCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_SMART_BROKEN_WALLS],
                        L"Place damaged walls using smart method");
  iEditorButton[BUILDING_DOORKEY] =
      CreateEasyToggleButton(290, giScrH - 480 + 430, "EDITOR\\key.sti", BuildingDoorKeyCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_DOORKEY], L"Lock or trap existing doors");

  iEditorButton[BUILDING_NEW_ROOM] = CreateEasyToggleButton(
      370, giScrH - 480 + 370, "EDITOR\\newroom.sti", BuildingNewRoomCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_NEW_ROOM], L"Add a new room");
  iEditorButton[BUILDING_CAVE_DRAWING] = CreateEasyToggleButton(
      370, giScrH - 480 + 370, "EDITOR\\caves.sti", BuildingCaveDrawingCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_CAVE_DRAWING], L"Edit cave walls.");
  iEditorButton[BUILDING_SAW_ROOM] = CreateEasyToggleButton(
      370, giScrH - 480 + 400, "EDITOR\\sawroom.sti", BuildingSawRoomCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_SAW_ROOM],
                        L"Remove an area from existing building.");
  iEditorButton[BUILDING_KILL_BUILDING] = CreateEasyToggleButton(
      370, giScrH - 480 + 430, "EDITOR\\delroom.sti", BuildingKillBuildingCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_KILL_BUILDING], L"Remove a building");
  iEditorButton[BUILDING_NEW_ROOF] = CreateEasyToggleButton(
      400, giScrH - 480 + 430, "EDITOR\\newroof.sti", BuildingNewRoofCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_NEW_ROOF],
                        L"Add/replace building's roof with new flat roof.");
  iEditorButton[BUILDING_COPY_BUILDING] = CreateEasyToggleButton(
      430, giScrH - 480 + 430, "EDITOR\\copyroom.sti", BuildingCopyBuildingCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_COPY_BUILDING], L"Copy a building");
  iEditorButton[BUILDING_MOVE_BUILDING] = CreateEasyToggleButton(
      460, giScrH - 480 + 430, "EDITOR\\moveroom.sti", BuildingMoveBuildingCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_MOVE_BUILDING], L"Move a building");
  iEditorButton[BUILDING_DRAW_ROOMNUM] = CreateEasyToggleButton(
      410, giScrH - 480 + 370, "EDITOR\\addTileRoom.sti", BuildingDrawRoomNumCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_DRAW_ROOMNUM], L"Draw room number");
  iEditorButton[BUILDING_ERASE_ROOMNUM] = CreateEasyToggleButton(
      440, giScrH - 480 + 370, "EDITOR\\killTileRoom.sti", BuildingEraseRoomNumCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_ERASE_ROOMNUM], L"Erase room numbers");

  iEditorButton[BUILDING_TOGGLE_ERASEMODE] =
      CreateEasyToggleButton(500, giScrH - 480 + 400, "EDITOR\\eraser.sti", BtnEraseCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_TOGGLE_ERASEMODE], L"Toggle erase mode");
  iEditorButton[BUILDING_UNDO] =
      CreateEasyNoToggleButton(530, giScrH - 480 + 400, "EDITOR\\undo.sti", BtnUndoCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_UNDO], L"Undo last change");
  iEditorButton[BUILDING_CYCLE_BRUSHSIZE] =
      CreateEasyNoToggleButton(500, giScrH - 480 + 430, "EDITOR\\paint.sti", BtnBrushCallback);
  SetButtonFastHelpText(iEditorButton[BUILDING_CYCLE_BRUSHSIZE], L"Cycle brush size");
}

void InitEditorItemsToolbar() {
  SetFontForeground(FONT_MCOLOR_LTRED);
  iEditorButton[ITEMS_WEAPONS] =
      CreateTextButton(L"Weapons", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 100, giScrH - 480 + 440, 59, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsWeaponsCallback);
  iEditorButton[ITEMS_AMMO] =
      CreateTextButton(L"Ammo", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 159, giScrH - 480 + 440, 40, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsAmmoCallback);
  iEditorButton[ITEMS_ARMOUR] =
      CreateTextButton(L"Armour", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 199, giScrH - 480 + 440, 52, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsArmourCallback);
  iEditorButton[ITEMS_EXPLOSIVES] =
      CreateTextButton(L"Explosives", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 251, giScrH - 480 + 440, 69, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsExplosivesCallback);
  iEditorButton[ITEMS_EQUIPMENT1] =
      CreateTextButton(L"E1", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 320, giScrH - 480 + 440, 21, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsEquipment1Callback);
  iEditorButton[ITEMS_EQUIPMENT2] =
      CreateTextButton(L"E2", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 341, giScrH - 480 + 440, 21, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsEquipment2Callback);
  iEditorButton[ITEMS_EQUIPMENT3] =
      CreateTextButton(L"E3", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 362, giScrH - 480 + 440, 21, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsEquipment3Callback);
  iEditorButton[ITEMS_TRIGGERS] =
      CreateTextButton(L"Triggers", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 383, giScrH - 480 + 440, 59, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsTriggersCallback);
  iEditorButton[ITEMS_KEYS] =
      CreateTextButton(L"Keys", (UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK,
                       BUTTON_USE_DEFAULT, 442, giScrH - 480 + 440, 38, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ItemsKeysCallback);

  iEditorButton[ITEMS_LEFTSCROLL] = CreateEasyNoToggleButton(
      1, giScrH - 480 + 361, "EDITOR\\leftscroll.sti", ItemsLeftScrollCallback);
  iEditorButton[ITEMS_RIGHTSCROLL] = CreateEasyNoToggleButton(
      50, giScrH - 480 + 361, "EDITOR\\rightscroll.sti", ItemsRightScrollCallback);
}

void InitEditorMapInfoToolbar() {
  iEditorButton[MAPINFO_ADD_LIGHT1_SOURCE] =
      CreateEasyToggleButton(10, giScrH - 480 + 362, "EDITOR\\light.sti", BtnDrawLightsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_ADD_LIGHT1_SOURCE], L"Add ambient light source");

  iEditorButton[MAPINFO_LIGHT_PANEL] =
      CreateTextButton(0, 0, 0, 0, BUTTON_USE_DEFAULT, 45, giScrH - 480 + 362, 60, 50,
                       BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK);
  SpecifyDisabledButtonStyle(iEditorButton[MAPINFO_LIGHT_PANEL], DISABLED_STYLE_NONE);
  DisableButton(iEditorButton[MAPINFO_LIGHT_PANEL]);
  iEditorButton[MAPINFO_PRIMETIME_LIGHT] =
      CreateCheckBoxButton(48, giScrH - 480 + 365, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MapInfoPrimeTimeRadioCallback);
  iEditorButton[MAPINFO_NIGHTTIME_LIGHT] =
      CreateCheckBoxButton(48, giScrH - 480 + 380, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MapInfoNightTimeRadioCallback);
  iEditorButton[MAPINFO_24HOUR_LIGHT] =
      CreateCheckBoxButton(48, giScrH - 480 + 395, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MapInfo24HourTimeRadioCallback);
  ClickEditorButton(gbDefaultLightType + MAPINFO_PRIMETIME_LIGHT);

  iEditorButton[MAPINFO_TOGGLE_FAKE_LIGHTS] = CreateEasyToggleButton(
      120, giScrH - 480 + 362, "EDITOR\\fakelight.sti", BtnFakeLightCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_TOGGLE_FAKE_LIGHTS], L"Toggle fake ambient lights.");

  iEditorButton[MAPINFO_RADIO_PANEL] =
      CreateTextButton(0, 0, 0, 0, BUTTON_USE_DEFAULT, 207, giScrH - 480 + 362, 70, 50,
                       BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK);
  SpecifyDisabledButtonStyle(iEditorButton[MAPINFO_RADIO_PANEL], DISABLED_STYLE_NONE);
  DisableButton(iEditorButton[MAPINFO_RADIO_PANEL]);
  iEditorButton[MAPINFO_RADIO_NORMAL] =
      CreateCheckBoxButton(210, giScrH - 480 + 365, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MapInfoNormalRadioCallback);
  iEditorButton[MAPINFO_RADIO_BASEMENT] =
      CreateCheckBoxButton(210, giScrH - 480 + 380, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MapInfoBasementRadioCallback);
  iEditorButton[MAPINFO_RADIO_CAVES] =
      CreateCheckBoxButton(210, giScrH - 480 + 395, "EDITOR\\radiobutton.sti", MSYS_PRIORITY_NORMAL,
                           MapInfoCavesRadioCallback);

  iEditorButton[MAPINFO_DRAW_EXITGRIDS] = CreateEasyToggleButton(
      305, giScrH - 480 + 372, "EDITOR\\exitgridbut.sti", MapInfoDrawExitGridCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_DRAW_EXITGRIDS],
                        L"Add exit grids (r-clk to query existing).");
  iEditorButton[MAPINFO_CYCLE_BRUSHSIZE] =
      CreateEasyNoToggleButton(420, giScrH - 480 + 430, "EDITOR\\paint.sti", BtnBrushCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_CYCLE_BRUSHSIZE], L"Cycle brush size");
  iEditorButton[MAPINFO_UNDO] =
      CreateEasyNoToggleButton(510, giScrH - 480 + 430, "EDITOR\\undo.sti", BtnUndoCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_UNDO], L"Undo last change");
  iEditorButton[MAPINFO_TOGGLE_ERASEMODE] =
      CreateEasyToggleButton(540, giScrH - 480 + 430, "EDITOR\\eraser.sti", BtnEraseCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_TOGGLE_ERASEMODE], L"Toggle erase mode");

  iEditorButton[MAPINFO_NORTH_POINT] = CreateEasyToggleButton(
      540, giScrH - 480 + 365, "EDITOR\\north.sti", MapInfoEntryPointsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_NORTH_POINT],
                        L"Specify north point for validation purposes.");

  iEditorButton[MAPINFO_WEST_POINT] = CreateEasyToggleButton(
      525, giScrH - 480 + 386, "EDITOR\\west.sti", MapInfoEntryPointsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_WEST_POINT],
                        L"Specify west point for validation purposes.");

  iEditorButton[MAPINFO_EAST_POINT] = CreateEasyToggleButton(
      555, giScrH - 480 + 386, "EDITOR\\east.sti", MapInfoEntryPointsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_EAST_POINT],
                        L"Specify east point for validation purposes.");

  iEditorButton[MAPINFO_SOUTH_POINT] = CreateEasyToggleButton(
      540, giScrH - 480 + 407, "EDITOR\\south.sti", MapInfoEntryPointsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_SOUTH_POINT],
                        L"Specify south point for validation purposes.");

  iEditorButton[MAPINFO_CENTER_POINT] = CreateEasyToggleButton(
      590, giScrH - 480 + 375, "EDITOR\\center.sti", MapInfoEntryPointsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_CENTER_POINT],
                        L"Specify center point for validation purposes.");

  iEditorButton[MAPINFO_ISOLATED_POINT] = CreateEasyToggleButton(
      590, giScrH - 480 + 396, "EDITOR\\isolated.sti", MapInfoEntryPointsCallback);
  SetButtonFastHelpText(iEditorButton[MAPINFO_ISOLATED_POINT],
                        L"Specify isolated point for validation purposes.");
}

void InitEditorOptionsToolbar() {
  iEditorButton[OPTIONS_NEW_MAP] =
      CreateEasyNoToggleButton(71, giScrH - 480 + 401, "EDITOR\\new.sti", BtnNewMapCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_NEW_MAP], L"New map");

  iEditorButton[OPTIONS_NEW_BASEMENT] =
      CreateEasyNoToggleButton(101, giScrH - 480 + 401, "EDITOR\\new.sti", BtnNewBasementCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_NEW_BASEMENT], L"New basement");

  iEditorButton[OPTIONS_NEW_CAVES] =
      CreateEasyNoToggleButton(131, giScrH - 480 + 401, "EDITOR\\new.sti", BtnNewCavesCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_NEW_CAVES], L"New cave level");

  iEditorButton[OPTIONS_SAVE_MAP] =
      CreateEasyNoToggleButton(161, giScrH - 480 + 401, "EDITOR\\save.sti", BtnSaveCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_SAVE_MAP], L"Save map");

  iEditorButton[OPTIONS_LOAD_MAP] =
      CreateEasyNoToggleButton(191, giScrH - 480 + 401, "EDITOR\\load.sti", BtnLoadCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_LOAD_MAP], L"Load map");

  iEditorButton[OPTIONS_CHANGE_TILESET] = CreateEasyNoToggleButton(
      221, giScrH - 480 + 401, "EDITOR\\tileset.sti", BtnChangeTilesetCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_CHANGE_TILESET], L"Select tileset");

  iEditorButton[OPTIONS_LEAVE_EDITOR] =
      CreateEasyNoToggleButton(251, giScrH - 480 + 401, "EDITOR\\cancel.sti", BtnCancelCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_LEAVE_EDITOR], L"Leave Editor mode");

  iEditorButton[OPTIONS_QUIT_GAME] =
      CreateEasyNoToggleButton(281, giScrH - 480 + 401, "EDITOR\\cancel.sti", BtnQuitCallback);
  SetButtonFastHelpText(iEditorButton[OPTIONS_QUIT_GAME], L"Exit game.");
}

void InitEditorTerrainToolbar() {
  iEditorButton[TERRAIN_FGROUND_TEXTURES] =
      CreateEasyToggleButton(100, giScrH - 480 + 400, "EDITOR\\downgrid.sti", BtnFgGrndCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_FGROUND_TEXTURES], L"Draw ground textures");

  iEditorButton[TERRAIN_BGROUND_TEXTURES] =
      CreateEasyToggleButton(130, giScrH - 480 + 400, "EDITOR\\upgrid.sti", BtnBkGrndCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_BGROUND_TEXTURES], L"Set map ground textures");

  iEditorButton[TERRAIN_PLACE_CLIFFS] =
      CreateEasyToggleButton(160, giScrH - 480 + 400, "EDITOR\\banks.sti", BtnBanksCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_CLIFFS], L"Place banks and cliffs");

  iEditorButton[TERRAIN_PLACE_ROADS] =
      CreateEasyToggleButton(190, giScrH - 480 + 400, "EDITOR\\road.sti", BtnRoadsCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_ROADS], L"Draw roads");

  iEditorButton[TERRAIN_PLACE_DEBRIS] =
      CreateEasyToggleButton(220, giScrH - 480 + 400, "EDITOR\\debris.sti", BtnDebrisCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_DEBRIS], L"Draw debris");

  iEditorButton[TERRAIN_PLACE_TREES] =
      CreateEasyToggleButton(250, giScrH - 480 + 400, "EDITOR\\tree.sti", BtnObjectCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_TREES], L"Place trees & bushes");

  iEditorButton[TERRAIN_PLACE_ROCKS] =
      CreateEasyToggleButton(280, giScrH - 480 + 400, "EDITOR\\num1.sti", BtnObject1Callback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_ROCKS], L"Place rocks");

  iEditorButton[TERRAIN_PLACE_MISC] =
      CreateEasyToggleButton(310, giScrH - 480 + 400, "EDITOR\\num2.sti", BtnObject2Callback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_MISC], L"Place barrels & other junk");

  iEditorButton[TERRAIN_FILL_AREA] =
      CreateEasyToggleButton(100, giScrH - 480 + 430, "EDITOR\\fill.sti", BtnFillCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_FILL_AREA], L"Fill area");

  iEditorButton[TERRAIN_UNDO] =
      CreateEasyNoToggleButton(130, giScrH - 480 + 430, "EDITOR\\undo.sti", BtnUndoCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_UNDO], L"Undo last change");

  iEditorButton[TERRAIN_TOGGLE_ERASEMODE] =
      CreateEasyToggleButton(160, giScrH - 480 + 430, "EDITOR\\eraser.sti", BtnEraseCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_TOGGLE_ERASEMODE], L"Toggle erase mode");

  iEditorButton[TERRAIN_CYCLE_BRUSHSIZE] =
      CreateEasyNoToggleButton(190, giScrH - 480 + 430, "EDITOR\\paint.sti", BtnBrushCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_CYCLE_BRUSHSIZE], L"Cycle brush size");

  iEditorButton[TERRAIN_RAISE_DENSITY] = CreateEasyNoToggleButton(
      280, giScrH - 480 + 430, "EDITOR\\uparrow.sti", BtnIncBrushDensityCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_RAISE_DENSITY], L"Raise brush density");

  iEditorButton[TERRAIN_LOWER_DENSITY] = CreateEasyNoToggleButton(
      350, giScrH - 480 + 430, "EDITOR\\downarrow.sti", BtnDecBrushDensityCallback);
  SetButtonFastHelpText(iEditorButton[TERRAIN_LOWER_DENSITY], L"Lower brush density");
}

void CreateEditorTaskbarInternal() {
  // Create the tabs for the editor taskbar
  iEditorButton[TAB_TERRAIN] =
      CreateTextButton(L"Terrain", (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI,
                       BUTTON_USE_DEFAULT, 100, giScrH - 480 + 460, 90, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_HIGH, BUTTON_NO_CALLBACK, TaskTerrainCallback);
  SpecifyButtonDownTextColors(iEditorButton[TAB_TERRAIN], FONT_YELLOW, FONT_ORANGE);
  iEditorButton[TAB_BUILDINGS] =
      CreateTextButton(L"Buildings", (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI,
                       BUTTON_USE_DEFAULT, 190, giScrH - 480 + 460, 90, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_HIGH, BUTTON_NO_CALLBACK, TaskBuildingCallback);
  SpecifyButtonDownTextColors(iEditorButton[TAB_BUILDINGS], FONT_YELLOW, FONT_ORANGE);
  iEditorButton[TAB_ITEMS] =
      CreateTextButton(L"Items", (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
                       280, giScrH - 480 + 460, 90, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
                       BUTTON_NO_CALLBACK, TaskItemsCallback);
  SpecifyButtonDownTextColors(iEditorButton[TAB_ITEMS], FONT_YELLOW, FONT_ORANGE);
  iEditorButton[TAB_MERCS] =
      CreateTextButton(L"Mercs", (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
                       370, giScrH - 480 + 460, 90, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
                       BUTTON_NO_CALLBACK, TaskMercsCallback);
  SpecifyButtonDownTextColors(iEditorButton[TAB_MERCS], FONT_YELLOW, FONT_ORANGE);
  iEditorButton[TAB_MAPINFO] =
      CreateTextButton(L"Map Info", (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI,
                       BUTTON_USE_DEFAULT, 460, giScrH - 480 + 460, 90, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_HIGH, BUTTON_NO_CALLBACK, TaskMapInfoCallback);
  SpecifyButtonDownTextColors(iEditorButton[TAB_MAPINFO], FONT_YELLOW, FONT_ORANGE);
  iEditorButton[TAB_OPTIONS] =
      CreateTextButton(L"Options", (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI,
                       BUTTON_USE_DEFAULT, 550, giScrH - 480 + 460, 90, 20, BUTTON_TOGGLE,
                       MSYS_PRIORITY_HIGH, BUTTON_NO_CALLBACK, TaskOptionsCallback);
  SpecifyButtonDownTextColors(iEditorButton[TAB_OPTIONS], FONT_YELLOW, FONT_ORANGE);

  // Create the buttons within each tab.
  InitEditorTerrainToolbar();
  InitEditorBuildingsToolbar();
  InitEditorItemsToolbar();
  InitEditorMercsToolbar();
  InitEditorMapInfoToolbar();
  InitEditorOptionsToolbar();
  InitEditorItemStatsButtons();
}

#endif
