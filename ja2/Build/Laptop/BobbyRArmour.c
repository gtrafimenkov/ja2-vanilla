#include "Laptop/LaptopAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "Laptop/Laptop.h"
#include "Laptop/BobbyRArmour.h"
#include "Laptop/BobbyRGuns.h"
#include "Laptop/BobbyR.h"
#include "Utils/Utilities.h"
#include "SGP/WCheck.h"
#include "Utils/WordWrap.h"
#include "Utils/Cursors.h"
#include "Utils/Text.h"
#endif

UINT32 guiArmourBackground;
UINT32 guiArmourGrid;

void GameInitBobbyRArmour() {}

BOOLEAN EnterBobbyRArmour() {
  VOBJECT_DESC VObjectDesc;

  // load the background graphic and add it
  VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
  FilenameForBPP("LAPTOP\\Armourbackground.sti", VObjectDesc.ImageFile);
  CHECKF(AddVideoObject(&VObjectDesc, &guiArmourBackground));

  // load the gunsgrid graphic and add it
  VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
  FilenameForBPP("LAPTOP\\Armourgrid.sti", VObjectDesc.ImageFile);
  CHECKF(AddVideoObject(&VObjectDesc, &guiArmourGrid));

  InitBobbyBrTitle();
  // Draw menu bar
  InitBobbyMenuBar();

  SetFirstLastPagesForNew(IC_ARMOUR);
  //	CalculateFirstAndLastIndexs();

  RenderBobbyRArmour();

  return (TRUE);
}

void ExitBobbyRArmour() {
  DeleteVideoObjectFromIndex(guiArmourBackground);
  DeleteVideoObjectFromIndex(guiArmourGrid);
  DeleteBobbyMenuBar();

  DeleteBobbyBrTitle();
  DeleteMouseRegionForBigImage();

  giCurrentSubPage = gusCurWeaponIndex;
  guiLastBobbyRayPage = LAPTOP_MODE_BOBBY_R_ARMOR;
}

void HandleBobbyRArmour() {}

void RenderBobbyRArmour() {
  HVOBJECT hPixHandle;

  WebPageTileBackground(BOBBYR_NUM_HORIZONTAL_TILES, BOBBYR_NUM_VERTICAL_TILES,
                        BOBBYR_BACKGROUND_WIDTH, BOBBYR_BACKGROUND_HEIGHT, guiArmourBackground);

  // Display title at top of page
  DisplayBobbyRBrTitle();

  // GunForm
  GetVideoObject(&hPixHandle, guiArmourGrid);
  BltVideoObject(FRAME_BUFFER, hPixHandle, 0, giOffsW + BOBBYR_GRIDLOC_X,
                 giOffsH + BOBBYR_GRIDLOC_Y, VO_BLT_SRCTRANSPARENCY, NULL);

  DisplayItemInfo(IC_ARMOUR);

  UpdateButtonText(guiCurrentLaptopMode);
  MarkButtonsDirty();
  RenderWWWProgramTitleBar();
  InvalidateRegion(giOffsW + LAPTOP_SCREEN_UL_X, giOffsH + LAPTOP_SCREEN_WEB_UL_Y,
                   giOffsW + LAPTOP_SCREEN_LR_X, giOffsH + LAPTOP_SCREEN_WEB_LR_Y);
}
